//
// Created by cw on 2024/5/11.
//

#include "cmdlistener.hpp"

xft::CmdListener::CmdListener(xft::EventLoop *loop) {
//    parser_.parse_check("connect -i 127.0.0.1 -p 12005");
    channel_ = std::make_unique<xft::Channel>(loop, STDIN_FILENO);
    channel_->onDataReceived([this]() {
        xft::xftPrompt();
        std::string instruction;
        ssize_t byte_read = 0;
        char buffer[XFT_BUF_SIZE]{};
        do {
            byte_read = ::read(STDIN_FILENO, buffer, sizeof(buffer));
            if (byte_read == -1 && errno == EAGAIN || errno == EWOULDBLOCK) { // read over
                break;
            } else if (byte_read < 0) {
                perror("read");
            }
            instruction.append(buffer, byte_read);
        } while (byte_read == XFT_BUF_SIZE);
        CMD_TYPE cmdType = CmdManager::getInstance().parseCmd(instruction);
        auto &parser = CmdManager::getInstance().getParser();
        switch (cmdType) {
            case CMD_TYPE::CONNECT: {
                std::string host = parser.get<std::string>("host");
                uint16_t port = parser.get<uint16_t>("port");
                std::unique_ptr<xft::Socket> socket = std::make_unique<xft::Socket>();
                socket->connect(InetAddr(host, port));
                connectHandler_(std::move(socket));
                break;
            }
            case CMD_TYPE::GETFILELIST: {
                getFilelistHandler_();
                break;
            }
            case CMD_TYPE::LSC:
                lscHandler_();
                break;
            case CMD_TYPE::HELP: {
                helpHandler_();
                break;
            }
            case CMD_TYPE::EXIT: {
                exitHandler_();
                break;
            }
            case CMD_TYPE::UPLOAD:
//                std::cout << "upload" << std::endl;
//                for (int i = 0; i < 100; ++i) {
//                    xft::xftProcessBar(i, 100);
//                    std::this_thread::sleep_for(std::chrono::milliseconds(50));
//                }
//                std::cout << std::endl;
//                xft::xftPrompt();
                uploadHandler_();
                break;
            case CMD_TYPE::GET:
                getHandler_();
                break;
            case CMD_TYPE::UNKNOWN:
                unknownHandler_();
                break;
        }
    });
    channel_->enableRead();
    channel_->enableET();
    xft::xftPrompt();
}

xft::CmdListener::~CmdListener() {
    channel_.reset();
}

void xft::CmdListener::onHelp(std::function<void()> &&handler) {
    helpHandler_ = std::move(handler);
}

void xft::CmdListener::onConnect(std::function<void(std::unique_ptr<xft::Socket>)> &&handler) {
    connectHandler_ = std::move(handler);
}

void xft::CmdListener::onExit(std::function<void()> &&handler) {
    exitHandler_ = std::move(handler);
}

void xft::CmdListener::onUpload(std::function<void()> &&handler) {
    uploadHandler_ = std::move(handler);
}

void xft::CmdListener::onGet(std::function<void()> &&handler) {
    getHandler_ = std::move(handler);
}

void xft::CmdListener::onGetFileList(std::function<void()> &&handler) {
    getFilelistHandler_ = std::move(handler);
}

void xft::CmdListener::onUnknown(std::function<void()> &&handler) {
    unknownHandler_ = std::move(handler);
}

void xft::CmdListener::onLsc(std::function<void()> &&handler) {
    lscHandler_ = std::move(handler);
}