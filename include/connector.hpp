//
// Created by cw on 2024/5/11.
//

#ifndef XFT_CONNECTOR_HPP
#define XFT_CONNECTOR_HPP

#include <memory>
#include <functional>

#include "socket.hpp"
#include "channel.hpp"

class Connector {
private:
    std::unique_ptr<xft::Socket> socket_;
    std::unique_ptr<xft::Channel> channel_;

public:
    Connector(const InetAddr& addr, xft::EventLoop* loop);
    ~Connector();
public:

};


#endif //XFT_CONNECTOR_HPP
