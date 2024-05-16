//
// Created by cw on 2024/4/28.
//

#ifndef XFT_XUNFTP_H
#define XFT_XUNFTP_H
#include <cstdint>
#include <string>
#include <vector>


#include "global.hpp"

enum class MessageType {
    TEST = 0b0000,
    FILE_LIST_REQUEST = 0b0001, // client request for the file list of server
    FILE_LIST_RESPONSE = 0b0010, // server response the file list
    FILE_REQUEST = 0b0011, // client request for the file
    FILE_INFO_RESPONSE = 0b0100, // server response the file info from
    FILE_RESPONSE = 0b0101, // server response the file: chunk
    FILE_ACK = 0b0110, // client ack the file
    FILE_INFO_ACK, // client ack the file info
};

struct XunFtpHeader {
    uint32_t type: 4;
    uint32_t moreChunks: 1;
    uint32_t reserved: 6;
    uint32_t length: 21;
    XunFtpHeader(MessageType type, uint32_t length);
};

struct XunFtpBody {
    std::vector<char> data;
};

struct XunFtpMessage {
    XunFtpHeader header{MessageType::TEST, 0};
    XunFtpBody body{};
    static void seal(XunFtpMessage& message, MessageType type, const std::string& data);
    static void seal(XunFtpMessage& message, MessageType type, const std::vector<char>& data);
};

struct XftFile {
    std::string name;
    std::string path;
    uint64_t size;
    std::size_t hash;
};

#endif //XFT_XUNFTP_H
