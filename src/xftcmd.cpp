//
// Created by cw on 2024/4/30.
//
#include "xftcmd.hpp"

CmdManager::CmdManager() {
    parser.add("help", '?', "show help");
    parser.add<std::string>("host", 'h', "host name, ip or domain", false, XFT_HOST);
    parser.add<uint16_t>("port", 'p', "server port", false, XFT_PORT);
    parser.add<std::string>("file", 'f', "file name", false, "");
    parser.add<std::string>("path", '/', "file path", false, "");
    parser.add<int>("ss", 's', "src server", false, 0);
}

CmdManager &CmdManager::getInstance() {
    static CmdManager instance;
    return instance;
}

extern const std::unordered_map<CMD_TYPE, std::string> CMD_NAME = {
        {CMD_TYPE::HELP,        "help"},
        {CMD_TYPE::EXIT,        "exit"},
        {CMD_TYPE::UPLOAD,      "upload"},
        {CMD_TYPE::GET,         "get"},
        {CMD_TYPE::GETFILELIST, "ilstfile"},
        {CMD_TYPE::LSC, "lsc"},
        {CMD_TYPE::CH,          "ch"},
        {CMD_TYPE::CONNECT,     "connect"},
        {CMD_TYPE::UNKNOWN,     "unknown"}
};


CMD_TYPE CmdManager::getCmdType(const std::string &cmd) {
    if (cmd == "help") {
        return CMD_TYPE::HELP;
    } else if (cmd == "exit") {
        return CMD_TYPE::EXIT;
    } else if (cmd == "upload") {
        return CMD_TYPE::UPLOAD;
    } else if(cmd == "connect") {
        return CMD_TYPE::CONNECT;
    } else if(cmd == "get") {
        return CMD_TYPE::GET;
    }  else if(cmd == "getfilelist") {
        return CMD_TYPE::GETFILELIST;
    }   else if(cmd == "lsc") {
        return CMD_TYPE::LSC;
    }   else {
        return CMD_TYPE::UNKNOWN;
    }
}

std::string CmdManager::getCmdName(CMD_TYPE cmd) {
    return CMD_NAME.at(cmd);
}


std::string trim(const std::string& input) {
    std::string result = input;
    // 去除开头的空白字符
    result.erase(0, result.find_first_not_of(" \t\n\r\f\v"));
    // 去除结尾的空白字符
    result.erase(result.find_last_not_of(" \t\n\r\f\v") + 1, std::string::npos);
    return result;
}

CMD_TYPE CmdManager::parseCmd(const std::string& cmdStr) {
    size_t space_index = cmdStr.find_first_of(' ');
    std::string cmd{};
    std::string args{};
    if(space_index != std::string::npos) {
        cmd = trim(cmdStr.substr(0, space_index));
        parser.parse_check(trim(cmdStr));
    } else {
        cmd = trim(cmdStr);
    }
    return getCmdType(cmd);
}

cmdline::parser &CmdManager::getParser() {
    return parser;
}
