//
// Created by cw on 2024/4/29.
//

#include "xunftp.h"

XunFtpHeader::XunFtpHeader(MessageType type, uint32_t length): type(static_cast<uint32_t>(type)), moreChunks(0),reserved(0), length(length) {}

void XunFtpMessage::seal(XunFtpMessage &message, MessageType type, const std::string &data) {
    message.header.type = static_cast<uint32_t>(type);
    message.header.length = data.length();
    message.body.data.assign(data.begin(), data.end());
    switch (type) {
        case MessageType::FILE_LIST_REQUEST:
            break;
        case MessageType::FILE_LIST_RESPONSE:
            break;
        case MessageType::FILE_REQUEST:
            break;
        case MessageType::FILE_INFO_RESPONSE:
            break;
        case MessageType::FILE_RESPONSE:
            if(data.size() != XFT_FILE_CHUNK_SIZE) {
                message.header.moreChunks = 0;
            } else {
                message.header.moreChunks = 1;
            }
            break;
        case MessageType::FILE_ACK: {
            break;
        }
    }
}

void XunFtpMessage::seal(XunFtpMessage &message, MessageType type, const std::vector<char> &data) {
    message.header.type = static_cast<uint32_t>(type);
    message.header.length = data.size();
    message.body.data.assign(data.begin(), data.end());
    switch (type) {
        case MessageType::FILE_LIST_REQUEST:
            break;
        case MessageType::FILE_LIST_RESPONSE:
            break;
        case MessageType::FILE_REQUEST:
            break;
        case MessageType::FILE_INFO_RESPONSE:
            break;
        case MessageType::FILE_RESPONSE:
            if(data.size() != XFT_FILE_CHUNK_SIZE) {
                message.header.moreChunks = 0;
            } else {
                message.header.moreChunks = 1;
            }
            break;
        case MessageType::FILE_ACK: {
            break;
        }
    }
}