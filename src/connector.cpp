//
// Created by cw on 2024/5/11.
//

#include "connector.hpp"

Connector::Connector(const InetAddr& addr, xft::EventLoop* loop) {
    socket_ = std::make_unique<xft::Socket>();
    channel_ = std::make_unique<xft::Channel>(loop, socket_->fd());
}