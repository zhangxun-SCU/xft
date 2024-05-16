//
// Created by cw on 2024/5/9.
//

#ifndef XFT_INETADDR_HPP
#define XFT_INETADDR_HPP

#include <arpa/inet.h>
#include <netinet/in.h>
#include <string>

class InetAddr {
private:
    sockaddr_in addr_;
public:
    InetAddr(const std::string &ip, uint16_t port);
    InetAddr(uint16_t port);
    InetAddr(const sockaddr_in &addr);
    InetAddr() = default;
    ~InetAddr() = default;
public:
    const char* ip() const;
    uint16_t port() const;
    const sockaddr* addr() const;
    void setAddr(const sockaddr_in &addr);
};


#endif //XFT_INETADDR_HPP
