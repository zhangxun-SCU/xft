//
// Created by cw on 2024/5/12.
//

#include "connection.hpp"

Connection::Connection(std::unique_ptr<xft::Socket> socket, xft::EventLoop *loop) {
    socket_ = std::move(socket);
    channel_ = std::make_unique<xft::Channel>(loop, socket_->fd());

    // 初始化服务器端
//        channel_->setWriterHandler(std::bind(&Connection::handleWrite, this));
    channel_->enableET();
    channel_->enableRead();
//    channel_->enableWrite();
//    channel_->onWrite([]()->void {
//        std::cout << "write" << std::endl;
//    });
}


Connection::~Connection() {
    socket_.reset();
    channel_.reset();
}

Connection::Connection(Connection &&conn) noexcept {
    socket_ = std::move(conn.socket_);
    channel_ = std::move(conn.channel_);
    readHandler_ = std::move(conn.readHandler_);
}

int Connection::fd() const {
    return socket_->fd();
}

void Connection::send(const XunFtpMessage& message) {
    ssize_t headLeft = XFT_HEADER_SIZE;
    ssize_t bodyLeft = message.header.length;
    while (headLeft > 0) {
        ssize_t bytes = ::send(socket_->fd(), &message.header + XFT_HEADER_SIZE - headLeft, headLeft, 0);
        if (bytes == -1 && (errno == EAGAIN || errno == EWOULDBLOCK)) {
//            std::this_thread::sleep_for(std::chrono::milliseconds(50));
            continue;
        } else if(bytes == 0) {
            // close
            return;
        }
        headLeft -= bytes;
        if (bytes > 0) {headLeft -= bytes;}
    }
    while (bodyLeft > 0) {
        ssize_t bytes = ::send(socket_->fd(), message.body.data.data() + bodyLeft - message.header.length, bodyLeft, 0);
        if (bytes == -1 && errno != EWOULDBLOCK && errno != EAGAIN) {
            return;
        } else if(bytes == 0) {
            // close
            return;
        }
        if (bytes > 0) {
            bodyLeft -= bytes;
        }
//        std::this_thread::sleep_for(std::chrono::milliseconds(50));
//        std::cout << " body left:" << bodyLeft << std::endl;
    }
}

void Connection::recv() {
//    ::recv(socket_->fd(), buf_, 1024, 0);
}

void Connection::onRead(std::function<void(Connection* conn, XunFtpMessage*message)> &&handler) {
    readHandler_ = std::move(handler);
    channel_->onDataReceived([this]() -> void {
        char header[4];
        auto* message = new XunFtpMessage{};
//        std::unique_ptr<XunFtpMessage> message{};
        ssize_t headerLeft = XFT_HEADER_SIZE;
        while (headerLeft > 0) {
            ssize_t bytes = ::recv(socket_->fd(), header + XFT_HEADER_SIZE - headerLeft, headerLeft, 0);
            if (bytes == -1 && errno != EWOULDBLOCK && errno != EAGAIN) {
                return;
            } else if(bytes == 0) {
                // todo: close connection
                return;
            }
            if (bytes > 0) {
                headerLeft -= bytes;
            }
        }
        memcpy(&(message->header), header, XFT_HEADER_SIZE);

        // recv body
        ssize_t bodyLeft = message->header.length;
        std::vector<char> body(bodyLeft);
        while (bodyLeft > 0) {
            ssize_t bytes = ::recv(socket_->fd(), body.data() + message->header.length - bodyLeft,  bodyLeft, 0);
            if (bytes == -1 && errno != EWOULDBLOCK && errno != EAGAIN) {
                return;
            } else if(bytes == 0){
                // todo: close
                return;
            }
            if (bytes > 0) {
                bodyLeft -= bytes;
            }
        }
        XunFtpMessage::seal(*message, static_cast<MessageType>(message->header.type), body);
        if(readHandler_) {
            readHandler_(this, message);
        }
    });

}

void Connection::setTransferringFile(XftFile&& file) {
    transferringFile_ = std::move(file);
}

XftFile Connection::getTransferringFile() {
    return transferringFile_;
}

void Connection::onWrite(std::function<void()> &&handler) {
    channel_->onWrite(std::move(handler));
}
