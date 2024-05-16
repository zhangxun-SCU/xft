//
// Created by cw on 2024/5/9.
//

#ifndef XFT_EPOLL_HPP
#define XFT_EPOLL_HPP

#include <vector>
#include <string>
#include <cstring>
#include <iostream>

#include <sys/epoll.h>
#include <unistd.h>
#include "channel.hpp"

namespace xft {
    class Channel;
    class Epoll {
    private:
        static const int MAX_EVENTS = 1024;
        int epollFd_;
        epoll_event events_[MAX_EVENTS];
    public:
        Epoll();
        ~Epoll();
    public:
        int fd() const;
        std::vector<xft::Channel *> wait(int timeout = -1);

    };
}
#endif //XFT_EPOLL_HPP
