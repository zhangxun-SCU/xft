//
// Created by cw on 2024/5/9.
//

#ifndef XFT_XFTNODE_HPP
#define XFT_XFTNODE_HPP

#include <memory>
#include <unordered_map>
#include <fstream>
#include <filesystem>
#include <regex>

#include "eventloop.hpp"
#include "inetaddr.hpp"
#include "acceptor.hpp"
#include "cmdlistener.hpp"
#include "connection.hpp"
#include "xunftp.h"


class XFTNode {
private:
    std::unique_ptr<xft::EventLoop> mainLoop_;
    std::unique_ptr<Acceptor> acceptor_;
    std::unique_ptr<xft::CmdListener> cmdListener_;
    std::unordered_map<int, std::unique_ptr<Connection>> clientConns_;
    std::unordered_map<int, std::unique_ptr<Connection>> serverConns_;
    int usingConn_ = 0;
private:
    void acceptorInit_();
    void listenerInit_();
public:
    explicit XFTNode(const InetAddr &addr);
    ~XFTNode();
public:
    void start();
};


#endif //XFT_XFTNODE_HPP
