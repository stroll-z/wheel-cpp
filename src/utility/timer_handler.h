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
        int delay = 0;
        int interval = 0;
        bool deleted = true;
        bool work_flag = false;
    };

   public:
    static CTimerHandler &instance();

   private:
    CTimerHandler();
    CTimerHandler(const CTimerHandler &) = delete;
    CTimerHandler(CTimerHandler &&) = delete;

   public:
    int single_shot(int deley_ms, CTimer::func_type handler);

private:
    void onHandler();
    auto find_one_ready_entry() -> int;
    void onCallback(int index);

   private:
    std::atomic<bool> m_exitFlag{false};
    std::atomic<bool> m_readyFlag{false};
    std::atomic<int> m_freeThrNum{0};
    std::mutex m_mtx;
    std::condition_variable m_cond;
    std::vector<handler_entry> m_entrySet;
};

}  // namespace wheel
