//
// Created by cw on 2024/5/9.
//

#include "xftnode.hpp"

XFTNode::XFTNode(const InetAddr &addr) {
    mainLoop_ = std::make_unique<xft::EventLoop>();
    acceptor_ = std::make_unique<Acceptor>(addr, mainLoop_.get());
    acceptorInit_();
    cmdListener_ = std::make_unique<xft::CmdListener>(mainLoop_.get());
    listenerInit_();
}

XFTNode::~XFTNode() {
    mainLoop_.reset();
    acceptor_.reset();
}

void XFTNode::acceptorInit_() {
    acceptor_->onConnect([this](std::unique_ptr<xft::Socket> socket) {
        // a new connect
        std::unique_ptr<Connection> connection = std::make_unique<Connection>(std::move(socket), mainLoop_.get());
        // set server connection read event
        connection->onRead([](Connection* conn,  XunFtpMessage* message) {
            XunFtpMessage respMsg{};
            std::string data;
            std::string line;
            std::string path;
            std::filesystem::path filePath;
            std::vector<char> buf(XFT_FILE_CHUNK_SIZE);
//            std::istringstream iss{};
            std::fstream fileInfoFile(XFT_FILE_INFO_FILE, std::ios::in);
            std::fstream transferFile{};
            if (!fileInfoFile.is_open()) {
                return;
            }
            switch (static_cast<MessageType>(message->header.type)) {
                case MessageType::FILE_LIST_REQUEST:
                        while (std::getline(fileInfoFile, line)) {
                            data += (line.substr(0, line.find(' ')) + "\n");
                        }
                    XunFtpMessage::seal(respMsg, MessageType::FILE_LIST_RESPONSE, data);
                    conn->send(respMsg);
                    break;
                case MessageType::FILE_REQUEST:
                    fflush(stdout);
                    // todo: seal file resp
                    while(std::getline(fileInfoFile, line)) {
                        std::istringstream iss{line};
                        if(std::getline(iss, data, ' ') && data == std::string {message->body.data.begin(), message->body.data.end()}) {
                            std::getline(iss, path, ' ');
                            break;
                        }
                    }
                    filePath = path;
                    // set the file info of transfering
                    conn->setTransferringFile({
                        .name = filePath.filename(),
                        .path = std::filesystem::absolute(filePath),
                        .size = std::filesystem::file_size(filePath),
                        .hash = std::filesystem::hash_value(filePath),
                    });
                    // resp file info
                    XunFtpMessage::seal(respMsg, MessageType::FILE_INFO_RESPONSE, std::regex_replace(line, std::regex(" "),  "\n"));
                    conn->send(respMsg);
                    break;
                case MessageType::FILE_INFO_ACK:
                    std::cout << "this is a file info ack." << std::endl;
                    transferFile.open(conn->getTransferringFile().path, std::ios::binary | std::ios::in);
                    if (!transferFile.is_open()) {
                        std::cout << "open file failed. tran" << std::endl;
                        return;
                    }
                    while(!transferFile.eof()) {
                        transferFile.read(buf.data(), XFT_FILE_CHUNK_SIZE);
                        if(transferFile.gcount() < XFT_FILE_CHUNK_SIZE) {
                            buf.resize(transferFile.gcount());
                        }
                        XunFtpMessage respFileMsg{};
                        XunFtpMessage::seal(respMsg, MessageType::FILE_RESPONSE, buf);
                        conn->send(respMsg);
                    }
                    break;
                case MessageType::FILE_ACK:
                    std::cout << "this is a file ack." << std::endl;
                    fflush(stdout);
                    break;
            }
        });
        clientConns_[connection->fd()] = std::move(connection);
    });
}

void XFTNode::listenerInit_() {
    cmdListener_->onHelp([this]() {

    });
    cmdListener_->onExit([this]() {
        mainLoop_->stop();
        // todo: release all resources

    });
    cmdListener_->onConnect([this](std::unique_ptr<xft::Socket> socket) {
        std::unique_ptr<Connection> connection = std::make_unique<Connection>(std::move(socket), mainLoop_.get());
        usingConn_ = connection->fd();
        connection->onRead([](Connection* conn,  XunFtpMessage* message) {
            std::istringstream iss;
            std::string line;
            std::string name;
            std::string path;
            std::string hash;
            std::string size;
            std::fstream recvFile;
            XunFtpMessage msg{};
            switch (static_cast<MessageType>(message->header.type)) {
                case MessageType::FILE_LIST_RESPONSE:
                    iss.str(std::string {message->body.data.begin(), message->body.data.end()});
                    while (std::getline(iss, line, '\n')) {
                        std::cout << ANSI_COLOR_GREEN << line << ANSI_COLOR_RESET << std::endl;
                    }
                    xft::xftPrompt();
                    break;
                case MessageType::FILE_INFO_RESPONSE:
                    iss.str(std::string {message->body.data.begin(), message->body.data.end()});
                    std::getline(iss, name, '\n');
                    std::getline(iss, path, '\n');
                    std::getline(iss, size, '\n');
                    std::getline(iss, hash, '\n');
                    conn->setTransferringFile({
                        .name = name,
                        .path = path,
                        .size = std::stoul(size),
                        .hash = std::stoul(hash),
                    });
                    recvFile.open("./" + name, std::ios::binary | std::ios::out);
                    if(!recvFile.is_open()) {
                        std::cout << "open file failed.1" << std::endl;
                        return;
                    }
                    recvFile.close();
                    // 上传
                    recvFile.open(std::filesystem::path(XFT_FILE_INFO_FILE), std::ios::app);
                    if (!recvFile.is_open()) {
                        std::cout << "file info file not found" << std::endl;
                        return;
                    }
                    recvFile << name << " " << "./" << name << " " << size << " " << hash << std::endl;
                    recvFile.close();

                    XunFtpMessage::seal(msg, MessageType::FILE_INFO_ACK, "");
                    conn->send(msg);
                    break;
                case MessageType::FILE_RESPONSE:
                    recvFile.open("./" + conn->getTransferringFile().name, std::ios::binary | std::ios::app);
                    if(!recvFile.is_open()) {
                        std::cout << "open file failed." << std::endl;
                        return;
                    }
                    std::vector<char> buf(XFT_FILE_CHUNK_SIZE);
                    recvFile.write(message->body.data.data(), message->body.data.size());
                    recvFile.close();
                    if(message->header.moreChunks == 0) {
                        std::cout << "file recv complete" << std::endl;
                        recvFile.open(std::filesystem::path(XFT_FILE_INFO_FILE), std::ios::app);
                        if (!recvFile.is_open()) {
                            std::cout << "file info file not found" << std::endl;
                            return;
                        }
                        recvFile << conn->getTransferringFile().name << " " << "./" << conn->getTransferringFile().name << " " << conn->getTransferringFile().size << " " << conn->getTransferringFile().hash << std::endl;
                        recvFile.close();
                    }
                    recvFile.close();
                    break;
            }
//            std::cout << "read" << std::endl;
        });
        serverConns_[connection->fd()] = std::move(connection);

    });
    cmdListener_->onGetFileList([this]() {
        auto conn = this->serverConns_[usingConn_].get();
        // 获取 source 的文件列表
        XunFtpMessage message{};
        XunFtpMessage::seal(message, MessageType::FILE_LIST_REQUEST, std::vector<char>{});
        conn->send(message);
    });
    cmdListener_->onLsc([this]() {
        std::cout << serverConns_.size() << std::endl;
        for (auto &conn: serverConns_) {
            std::printf("%d\n", conn.first);
        }
        fflush(stdout);
    });
    cmdListener_->onUpload([this]() {
        std::fstream fileInfoFile{std::filesystem::path(XFT_FILE_INFO_FILE), std::ios::app};
        if (!fileInfoFile.is_open()) {
            std::cout << "file info file not found" << std::endl;
            return;
        }
        cmdline::parser & parser =CmdManager::getInstance().getParser();
        if(parser.exist("path")) {
            std::string filepath = parser.get<std::string>("path");
            if (std::filesystem::exists(filepath) && std::filesystem::is_regular_file(filepath)) {
                std::cout << "path: " << parser.get<std::string>("path") << std::endl;
                std::filesystem::path path(filepath);
                fileInfoFile << path.filename().string() << " " << std::filesystem::absolute(path).string() << " " << std::filesystem::file_size(path) << " " << std::filesystem::hash_value(path) << std::endl;
            }
        }
        fileInfoFile.close();
    });
    cmdListener_->onGet([this]() {
        auto conn = this->serverConns_[usingConn_].get();
        cmdline::parser & parser = CmdManager::getInstance().getParser();
        if (parser.exist("file")) {
            XunFtpMessage message{};
            XunFtpMessage::seal(message, MessageType::FILE_REQUEST, parser.get<std::string>("file"));
            conn->send(message);
        }
    });
    cmdListener_->onUnknown([this]() {
    });
}

void XFTNode::start() {
    mainLoop_->run();
}
