//
// Created by cw on 2024/5/10.
//

#include "channel.hpp"

xft::Channel::Channel(EventLoop* eventLoop, int fd)
        : loop_(eventLoop), fd_(fd), exist_(false), listenEvents_(0), readyEvents_(0) {}

int xft::Channel::fd() const {
    return fd_;
}

xft::EventLoop* xft::Channel::eventLoop()const {
    return loop_;
}

uint32_t xft::Channel::listenEvents() const {
    return listenEvents_;
}

uint32_t xft::Channel::readyEvents() const {
    return readyEvents_;
}

bool xft::Channel::exist() const {
    return exist_;
}

void xft::Channel::setExist(bool in) {
    exist_ = in;
}

void xft::Channel::setReadyEvent(uint32_t event) {
    readyEvents_ = event;
}

void xft::Channel::onDataReceived(std::function<void()> &&handler) {
    readHandler_ = std::move(handler);
}


//void xft::Channel::setReaderHandler(std::function<void()> &&handler) {
//    readHandler_ = std::move(handler);
//}

void xft::Channel::onWrite(std::function<void()> &&handler) {
    writeHandler_ = std::move(handler);
}

void xft::Channel::enableET() {
    listenEvents_ |= EPOLLET;
    loop_->updateChannel(this);
}

void xft::Channel::enableRead() {
    listenEvents_ |= EPOLLIN;
    loop_->updateChannel(this);
}

void xft::Channel::enableWrite() {
    listenEvents_ |= EPOLLOUT;
    loop_->updateChannel(this);
}

void xft::Channel::handleEvent() {
    if (readyEvents_ & EPOLLRDHUP) {
        std::printf("client closed\n");
        ::close(fd_);
    }
    if (readyEvents_ & EPOLLIN | EPOLLPRI) {
        readHandler_();
    }
    if (readyEvents_ & EPOLLOUT) {
        std::cout << "write\n";
        fflush(stdout);
        if (writeHandler_) {
            writeHandler_();
        }
    }
}


