//
// Created by cw on 2024/5/9.
//
#include "socket.hpp"

bool xft::setNonBlock(int fd) {
    int flags = fcntl(fd, F_GETFL, 0);
    if(flags == -1) {
        return false;
    }
    if(fcntl(fd, F_SETFL, flags | O_NONBLOCK) == -1) {
        return false;
    }
    return true;
}

xft::Socket::Socket(int fd): fd_(fd){}

xft::Socket::Socket(): fd_(::socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0)) {
    if (fd_ == -1) {
        std::printf("%s:%s:%d socket create error: %d\n", __FILE__, __FUNCTION__, __LINE__, errno);
        exit(errno);
    }
    setNonBlock(fd_);
}

xft::Socket::~Socket() {
    if(fd_ != -1) {
        close();
    }
}

int xft::Socket::close() {
    if (fd_ == -1) { return 0; }
    fd_ = -1;
    return ::close(fd_);
}

int xft::Socket::fd() const {
    return fd_;
}

int xft::Socket::listen(int n)  {
    int ret = ::listen(fd_, n);
    if (ret == -1) {
        std::printf("%s:%s:%d listen error: %d\n", __FILE__, __FUNCTION__, __LINE__, errno);
        close();
    }
    return ret;
}

int xft::Socket::bind(const InetAddr &addr)  {
    int ret = ::bind(fd_, addr.addr(), sizeof(sockaddr));
    if (ret == -1) {
        std::printf("%s:%s:%d bind error: %d\n", __FILE__, __FUNCTION__, __LINE__, errno);
        close();
    }
    return ret;
}

int xft::Socket::accept(InetAddr &clientAddr) const {
    sockaddr_in addr{};
    socklen_t len = sizeof(sockaddr_in);
//    clientAddr = InetAddr{addr};
    int cfd = ::accept4(fd_, reinterpret_cast<sockaddr *>(&addr), &len, SOCK_NONBLOCK);
    setNonBlock(cfd);
    clientAddr.setAddr(addr);
    return cfd;
}

int xft::Socket::connect(const InetAddr &addr) const {
    return ::connect(fd_, addr.addr(), sizeof(sockaddr));
}

int xft::Socket::setReuseAddr(bool on) const {
    int opt = on ? 1 : 0;
    return setsockopt(fd_, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == 0;
}

int xft::Socket::setReusePort(bool on) const {
    int opt = on ? 1 : 0;
    return setsockopt(fd_, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt)) == 0;
}

int xft::Socket::setTcpNoDelay(bool on) const {
    int opt = on ? 1 : 0;
    return setsockopt(fd_, IPPROTO_TCP, TCP_NODELAY, &opt, sizeof(opt)) == 0;
}

