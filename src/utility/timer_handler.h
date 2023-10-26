/**
 * @file timer_handler.h
 * @author your name (you@domain.com)
 * @brief 定时器句柄
 * @version 0.1
 * @date 2023-10-25
 *
 * @copyright Copyright (c) 2023
 *
 */

#pragma once

#include <atomic>
#include <chrono>
#include <condition_variable>
#include <mutex>

#include "utility/timer.h"

namespace wheel {

class CTimerHandler {
   public:
    struct handler_entry {
        CTimer::func_type func;
        std::chrono::time_point<std::chrono::steady_clock> tp;
        std::string description;
        int delay = 0;
        int interval = 0;  // <0仅运行一次后删除, 0-运行一次保留状态, 其他运行间隔
        bool deleted = true;
        bool work_flag = false;

        handler_entry() = default;
        handler_entry(const char *desc, int delay_ms, int interval, CTimer::func_type handler)
            : description(desc), delay(delay_ms), interval(interval), func(handler) {}
    };

   public:
    static CTimerHandler &instance();

   private:
    CTimerHandler();
    CTimerHandler(const CTimerHandler &) = delete;
    CTimerHandler(CTimerHandler &&) = delete;

   public:
    int single_shot(int delay_ms, CTimer::func_type handler);
    int cycle_shot(const char *desc, int delay_ms, int interval, CTimer::func_type handler);

   private:
    void onHandler();
    auto find_one_ready_entry() -> int;
    auto find_one_free_entry() -> std::vector<handler_entry>::iterator;
    void fill_entry(std::vector<handler_entry>::iterator it, const char *desc, int delay, int interval,
                    CTimer::func_type handler);
    void onCallback(int index);

   private:
    std::atomic<bool> m_exitFlag{false};
    bool m_readyFlag = false;
    int m_freeThrNum = 0;
    std::mutex m_mtx;
    std::mutex m_poolMtx;
    std::condition_variable m_cond;
    std::vector<handler_entry> m_entrySet;
};

}  // namespace wheel
