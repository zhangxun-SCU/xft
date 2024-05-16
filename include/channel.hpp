//
// Created by cw on 2024/5/10.
//

#ifndef XFT_CHANNEL_HPP
#define XFT_CHANNEL_HPP

#include <future>
#include <functional>

#include "epoll.hpp"
#include "inetaddr.hpp"
#include "socket.hpp"
#include "eventloop.hpp"

namespace xft {
    class EventLoop;
    class Channel {
    private:
        const int fd_; // channel - fd: 1-1
        EventLoop *loop_;
        bool exist_; // EPOLL_CTL_ADD / EPOLL_CTL_MOD
        uint32_t listenEvents_; // event listened by epoll: EPOLLIN / EPOLLOUT
        uint32_t readyEvents_; // event ready by epoll
        std::function<void()> readHandler_;
        std::function<void()> writeHandler_;
    public:
        Channel(EventLoop *eventLoop, int fd);
        ~Channel() = default;
    public:
        int fd() const;

        EventLoop* eventLoop() const;

        uint32_t listenEvents() const;

        uint32_t readyEvents() const;

        bool exist() const;

        void setExist(bool in = true);

//        void setReaderHandler(std::function<void()> handler);
        void onDataReceived(std::function<void()> &&handler);
        void onWrite(std::function<void()> &&handler);

        void setReadyEvent(uint32_t event);

        void enableET();

        void enableRead();

        void enableWrite();

        void handleEvent();
    };

}

#endif //XFT_CHANNEL_HPP
