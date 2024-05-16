//
// Created by cw on 2024/5/11.
//

#ifndef XFT_THREADPOLL_HPP
#define XFT_THREADPOLL_HPP

#include <atomic>
#include <thread>
#include <deque>
#include <functional>
#include <future>

class ThreadPoll {
private:
    std::deque<std::function<void()> > tasks_;
public:
    ThreadPoll();
    ~ThreadPoll();
public:
    template<typename F, typename...Args>
    [[maybe_unused]] auto submit(F& fun, Args&&...args);
};

//template<typename F, typename ...Args>
//[[maybe_unused]] auto ThreadPoll::submit(F &fun, Args &&...args) -> std::future<decltype(fun(args))> {
//
//            std::function<decltype(f(args...))()> func = std::bind(std::forward<F>(fun), std::forward<Args>(args)...); // 连接函数和参数定义，特殊函数类型，避免左右值错误
//            // Encapsulate it into a shared pointer in order to be able to copy construct
//            auto task_ptr = std::make_shared<std::packaged_task<decltype(f(args...))()>>(func);
//            // Warp packaged task into void function
//            std::function<void()> warpper_func = [task_ptr](){
//                (*task_ptr)();};
//            // 队列通用安全封包函数，并压入安全队列
//            tasks_.push_back(warpper_func);
//            // 唤醒一个等待中的线程
////            m_conditional_lock.notify_one();
//            // 返回先前注册的任务指针
//            return task_ptr->get_future();
//}

#endif //XFT_THREADPOLL_HPP
