//
// Created by cw on 2024/5/9.
//

#include "eventloop.hpp"

xft::EventLoop::EventLoop(): thread_pool_(std::thread::hardware_concurrency()) {
    epoll_ = std::make_unique<xft::Epoll>();
    running_ = true;
}

xft::EventLoop::~EventLoop() {
    epoll_.reset();
}

std::unique_ptr<xft::Epoll> &xft::EventLoop::epoll() {
    return epoll_;
}

void xft::EventLoop::updateChannel(xft::Channel *channel) {
    epoll_event event{};
    event.data.ptr = channel;
    event.events = channel->listenEvents();
    if(channel->exist()) { // exist
        if (epoll_ctl(epoll_->fd(), EPOLL_CTL_MOD, channel->fd(), &event) == -1){
            std::printf("%s:%s:%d epoll updateChannel error: %d\n", __FILE__, __FUNCTION__, __LINE__, errno);
            exit(errno);
        }
    } else { // not exist
        if (epoll_ctl(epoll_->fd(), EPOLL_CTL_ADD, channel->fd(), &event) == -1) {
            std::printf("%s:%s:%d epoll updateChannel error: %d\n", __FILE__, __FUNCTION__, __LINE__, errno);
            exit(errno);
        }
        channel->setExist(true);
    }
}

[[noreturn]] void xft::EventLoop::run() {
    std::vector<xft::Channel*> channels{};
    while (running_) {
        channels = epoll_->wait(10);
        for (auto& channel : channels) {
//            thread_pool_.submit_task([channel](){
//                channel->handleEvent();
//            });
            channel->handleEvent();
        }
    }
}

void xft::EventLoop::stop() {
    running_ = false;
}

