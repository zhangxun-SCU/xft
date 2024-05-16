//
// Created by cw on 2024/5/9.
//

#ifndef XFT_EVENTLOOP_HPP
#define XFT_EVENTLOOP_HPP

#include <deque>
#include <functional>
#include <atomic>
#include <thread>
#include <future>
#include <memory>

#include "epoll.hpp"
#include "channel.hpp"

namespace xft{
    class Channel;
    class Epoll;
    class EventLoop {
    private:
        std::unique_ptr<xft::Epoll> epoll_;
        std::atomic<bool> running_;
    public:
        EventLoop();
        ~EventLoop();
    public:
        std::unique_ptr<xft::Epoll>& epoll();
        void updateChannel(xft::Channel *channel);
        void deleteChannel(xft::Channel *channel);
        [[noreturn]] void run();
        void stop();
    };
}



#endif //XFT_EVENTLOOP_HPP
