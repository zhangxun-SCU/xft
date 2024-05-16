//
// Created by cw on 2024/5/9.
//

#ifndef XFT_SOCKET_HPP
#define XFT_SOCKET_HPP

#include <string>
#include <cstring>
#include <cstdint>
#include <cerrno>
#include <memory>

#include <sys/socket.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <fcntl.h>

#include "inetaddr.hpp"

namespace xft {
    bool setNonBlock(int fd);

    class Socket {
    private:
        int fd_;
    public:
        explicit Socket(int fd);
        Socket();
        ~Socket();
    public:
        [[nodiscard]] int fd() const;
        int bind(const InetAddr &addr) ;
        int listen(int n = 128) ;
        int accept(InetAddr &clientAddr) const;
        int connect(const InetAddr& addr) const;
        int close();
        // 禁用 Nagle 算法
        int setTcpNoDelay(bool on) const;
        int setReuseAddr(bool on) const;
        int setReusePort(bool on) const;
    };
}


#endif //XFT_SOCKET_HPP
