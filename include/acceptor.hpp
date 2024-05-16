//
// Created by cw on 2024/5/11.
//

#ifndef XFT_ACCEPTOR_HPP
#define XFT_ACCEPTOR_HPP

#include <functional>
#include <memory>
#include <type_traits>
#include <cassert>

#include "socket.hpp"
#include "inetaddr.hpp"
#include "channel.hpp"
#include "global.hpp"

class Acceptor {
private:
    std::unique_ptr<xft::Socket> socket_;
    std::unique_ptr<xft::Channel> channel_;
    std::function<void(std::unique_ptr<xft::Socket> socket)> newConnectionHandler_;
public:
    Acceptor(const InetAddr& addr, xft::EventLoop* loop);
    ~Acceptor();
public:
    void onConnect(std::function<void(std::unique_ptr<xft::Socket> socket)> &&handler);
};


#endif //XFT_ACCEPTOR_HPP
