//
// Created by cw on 2024/5/13.
//

#include "global.hpp"

void xft::xftPrompt() {
    std::cout << "xft> ";
    fflush(stdout);
}

void xft::xftProcessBar(int progress,int total, int barWidth) {
    if (total == 0) {
        return; // 防止除以零错误
    }
    float fraction = static_cast<float>(progress) / total;
    int filledWidth = static_cast<int>(fraction * barWidth);
    std::cout << "[";
    for (int i = 0; i < barWidth; ++i) {
        if (i < filledWidth) {
            std::cout << "=";
        } else if (i == filledWidth) {
            std::cout << ">";
        } else {
            std::cout << " ";
        }
    }
    std::cout << "] " << static_cast<int>(fraction * 100.0) << "%\r";
    fflush(stdout); // 强制刷新缓冲区，即时显示进度条
}
