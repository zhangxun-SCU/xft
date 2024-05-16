//
// Created by cw on 2024/5/12.
//

#ifndef XFT_GLOBAL_HPP
#define XFT_GLOBAL_HPP

#include <iostream>

#define XFT_BUF_SIZE 1024
#define XFT_PORT 12000
#define XFT_HOST "127.0.0.1"
#define XFT_HEADER_SIZE 4
#define XFT_FILE_CHUNK_SIZE (1024 * 1024)
#define XFT_TEMP_FILE_DIR "./temp/"
#define XFT_UPLOAD_FILE_DIR "./upload/"
#define XFT_FILE_INFO_FILE "./.xftinfo"

#define ANSI_COLOR_RED     "\x1B[31m"
#define ANSI_COLOR_GREEN   "\x1B[32m"
#define ANSI_COLOR_YELLOW  "\x1B[33m"
#define ANSI_COLOR_BLUE    "\x1B[34m"
#define ANSI_COLOR_MAGENTA "\x1B[35m"
#define ANSI_COLOR_CYAN    "\x1B[36m"
#define ANSI_COLOR_RESET   "\x1B[0m"

namespace xft {
    void xftPrompt();
    void xftProcessBar(int progress,int total, int barWidth=50);
}

#endif //XFT_GLOBAL_HPP
