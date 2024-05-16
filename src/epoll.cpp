//
// Created by cw on 2024/5/9.
//

#include "epoll.hpp"


xft::Epoll::Epoll(): events_() {
    epollFd_ = ::epoll_create(1); // 创建epoll句柄
    if (epollFd_ < 0) {
        std::printf("%s:%s:%d epoll create error: %d\n", __FILE__, __FUNCTION__, __LINE__, errno);
        exit(errno);
    }
}

xft::Epoll::~Epoll() {
    ::close(epollFd_);
}

int xft::Epoll::fd() const {
    return epollFd_;
}


std::vector<xft::Channel *> xft::Epoll::wait(int timeout) {
    std::vector<Channel*> channels{};
    bzero(events_, sizeof(events_));
    int eventNum = ::epoll_wait(epollFd_, events_, MAX_EVENTS, timeout);
    if (eventNum < 0) { // epoll error
        std::printf("%s:%s:%d epoll wait error: %d\n", __FILE__, __FUNCTION__, __LINE__, errno);
        exit(errno);
    }
    if (eventNum  == 0)  { // no event,timeout
        return {};
    }
    // event
    Channel *temp = nullptr;
    for (int i = 0; i < eventNum; ++i) {
        temp = reinterpret_cast<Channel*>(events_[i].data.ptr);
        temp->setReadyEvent(events_[i].events);
        channels.push_back(temp);
    }
    return channels;
}