//
// Created by cw on 2024/5/9.
//

#include "inetaddr.hpp"

InetAddr::InetAddr(const std::string &ip, uint16_t port) {
    addr_ = {
            .sin_family = AF_INET,
            .sin_port = ::htons(port),
            .sin_addr = {
                    .s_addr = ::inet_addr(ip.c_str())
            }
    };
}

InetAddr::InetAddr(const sockaddr_in &addr): addr_(addr) {}

InetAddr::InetAddr(uint16_t port) {
    addr_ = {
            .sin_family = AF_INET,
            .sin_port = ::htons(port),
            .sin_addr = {
                    .s_addr = ::htonl(INADDR_ANY)
            }
    };
}

const char *InetAddr::ip() const {
    return ::inet_ntoa(addr_.sin_addr);
}

uint16_t InetAddr::port() const {
    return ::ntohs(addr_.sin_port);
}

const sockaddr *InetAddr::addr() const {
    return reinterpret_cast<const sockaddr *>(&addr_);
}

void InetAddr::setAddr(const sockaddr_in &addr) {
    addr_ = addr;
}