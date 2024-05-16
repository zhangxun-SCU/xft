#include <iostream>
#include <filesystem>
#include <fstream>
#include <thread>
#include <functional>
#include <chrono>
#include <future>

#include "global.hpp"
#include "xftnode.hpp"
#include "cmdline.hpp"


#define FILE_SLICE_SIZE 0x80000 // 64KB

void init() {
    std::filesystem::path tempDir =XFT_TEMP_FILE_DIR;
    std::filesystem::path uploadDir = XFT_UPLOAD_FILE_DIR;
    std::filesystem::path infoFile = XFT_FILE_INFO_FILE;
    if (!std::filesystem::exists(tempDir)) {
        std::filesystem::create_directories(XFT_TEMP_FILE_DIR);
    }
    if (!std::filesystem::exists(uploadDir)) {
        std::filesystem::create_directories(XFT_UPLOAD_FILE_DIR);
    }
    if(!std::filesystem::exists(infoFile)) {
        std::fstream infoFileStream(XFT_FILE_INFO_FILE, std::ios::out);
        if(!infoFileStream.is_open()) {
            std::cout << "create info file failed" << std::endl;
            exit(errno);
        }
        infoFileStream.close();
    }
}


int main(int argc, char *argv[]) {
    init();
    cmdline::parser argvParser;
    argvParser.add<uint16_t>("port", 'p', "server port", false, XFT_PORT);
    argvParser.set_program_name("xft");
    argvParser.parse_check(argc, argv);
    XFTNode node{InetAddr{"127.0.0.1", argvParser.get<uint16_t>("port")}};
    node.start();
    return 0;
}
