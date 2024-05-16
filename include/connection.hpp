//
// Created by cw on 2024/5/12.
//

#ifndef XFT_CONNECTION_HPP
#define XFT_CONNECTION_HPP

#include <functional>
#include <memory>

#include "channel.hpp"
#include "socket.hpp"
#include "xunftp.h"
#include "global.hpp"


class Connection {
private:
    std::unique_ptr<xft::Socket> socket_;
    std::unique_ptr<xft::Channel> channel_;
    std::function<void(Connection* conn,  XunFtpMessage* message)> readHandler_;
    std::function<void()> writeHandler_;
    XftFile transferringFile_;
public:
    Connection(std::unique_ptr<xft::Socket> socket, xft::EventLoop *loop);
    Connection(Connection&& conn) noexcept;
    ~Connection();
public:
    int fd() const;
    void send(const XunFtpMessage& message);
    void recv();
    void onRead(std::function<void(Connection* conn,  XunFtpMessage* message)> &&handler);
    void onWrite(std::function<void()> &&handler);
//    void onUnknown();
    void setTransferringFile(XftFile &&file);
    XftFile getTransferringFile();
};


#endif //XFT_CONNECTION_HPP
