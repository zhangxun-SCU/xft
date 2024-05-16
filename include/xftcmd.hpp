//
// Created by cw on 2024/4/30.
//

#ifndef XFT_XFTCMD_HPP
#define XFT_XFTCMD_HPP

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

#include "cmdline.hpp"
#include "global.hpp"
#include "xftcmd.hpp"

enum class CMD_TYPE {
    HELP,
    EXIT,
    CONNECT,
    UPLOAD,
    GET,
    GETFILELIST,
    LSC,
    CH,
    UNKNOWN
};

extern const std::unordered_map<CMD_TYPE, std::string> CMD_NAME;


class CmdManager {
private:
    CmdManager();
    cmdline::parser parser;
public:
    static CmdManager &getInstance();
    cmdline::parser& getParser();
//    CmdManager(const CmdManager &) = default;

//    CmdManager &operator=(const CmdManager &) = delete;

//    CmdManager(CmdManager &&) = default;

//    CmdManager &operator=(CmdManager &&) = delete;

    ~CmdManager() = default;
     CMD_TYPE getCmdType(const std::string &cmd);
     std::string getCmdName(CMD_TYPE cmd);
     CMD_TYPE parseCmd(const std::string& cmdStr);
};

#endif //XFT_XFTCMD_HPP
