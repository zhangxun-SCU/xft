//
// Created by cw on 2024/5/11.
//

#include "acceptor.hpp"

Acceptor::Acceptor(const InetAddr& addr, xft::EventLoop* loop) {
    socket_ = std::make_unique<xft::Socket>();
    socket_->setReuseAddr(true);
    socket_->setReusePort(true);
    socket_->setTcpNoDelay(true);
    assert(socket_->bind(addr) != -1);
    socket_->listen();
    std::printf("listen on %s:%d\n", addr.ip(), addr.port());
    channel_ = std::make_unique<xft::Channel>(loop, socket_->fd());
    // on accept
    channel_->onDataReceived([this]() {
        InetAddr clientAddr{};
        auto clientSocket = std::make_unique<xft::Socket>(socket_->accept(clientAddr));
        if (clientSocket->fd() == -1) {
            std::printf("accept error\n");
            fflush(stdout);
            return;
        }
        std::cout << "new connection from " << clientAddr.ip() << ":" << clientAddr.port() << std::endl;
        xft::xftPrompt();
        fflush(stdout);
        newConnectionHandler_(std::move(clientSocket));
    });
    channel_->enableRead();
}

Acceptor::~Acceptor() {
    socket_.reset();
    channel_.reset();
}

void Acceptor::onConnect(std::function<void(std::unique_ptr<xft::Socket> socket)> &&handler) {
    newConnectionHandler_ = std::move(handler);
}
