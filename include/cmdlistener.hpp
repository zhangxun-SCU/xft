//
// Created by cw on 2024/5/11.
//

#ifndef XFT_CMDLISTENER_HPP
#define XFT_CMDLISTENER_HPP

#include <functional>
#include <memory>

#include "channel.hpp"
#include "cmdline.hpp"
#include "xftcmd.hpp"
#include "global.hpp"
#include "inetaddr.hpp"

namespace xft {
    class CmdListener {
    private:
        std::unique_ptr<xft::Channel> channel_;
    private:
        std::function<void()> helpHandler_;
        std::function<void()> exitHandler_;
        std::function<void(std::unique_ptr<xft::Socket> socket)> connectHandler_;
        std::function<void()> getFilelistHandler_;
        std::function<void()> lscHandler_;
        std::function<void()> uploadHandler_;
        std::function<void()> getHandler_;
        std::function<void()> unknownHandler_;
    public:
        explicit CmdListener(xft::EventLoop* loop);
        ~CmdListener();
    public:
        void onHelp(std::function<void()> &&handler);
        void onExit(std::function<void()> &&handler);
        void onConnect(std::function<void(std::unique_ptr<xft::Socket> socket)> &&handler);
        void onGetFileList(std::function<void()> &&handler);
        void onLsc(std::function<void()> &&handler);
        void onUpload(std::function<void()> &&handler);
        void onGet(std::function<void()> &&handler);
        void onUnknown(std::function<void()> &&handler);
    };
}


#endif //XFT_CMDLISTENER_HPP
