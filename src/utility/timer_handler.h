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

#include "basic/log.h"
#include "utility/timer.h"

namespace wheel {

#define TIMER_DEBUG_TRACE 1

#if defined(TIMER_DEBUG_TRACE)
#define TIMER_DEBUG(FMT, ...) LOG_D("timer", FMT, ##__VA_ARGS__)
#else
#define TIMER_DEBUG(FMT, ...)
#endif

class CTimerHandler {
   public:
    struct handler_entry {
        CTimer::func_type func_;
        std::chrono::time_point<std::chrono::steady_clock> tp_;
        std::string description_;
        int delay_ = 0;     // -1第一次运行后, 其他延迟时长
        int interval_ = 0;  // -1仅运行一次后删除, 0-运行一次保留状态, 其他运行间隔
        bool deleted_ = false;
        bool work_flag_ = false;
        bool pause_ = false;

        handler_entry() = default;
        handler_entry(const std::string &desc, int delay_ms, int interval, CTimer::func_type handler)
            : func_(handler), description_(desc), delay_(delay_ms), interval_(interval) {
            tp_ = std::chrono::steady_clock::now();
        }
    };

   public:
    static CTimerHandler &instance();
    ~CTimerHandler() {
        LOG_D("timer-handler", "destructor");
        m_exitFlag = true;
        m_cond.notify_all();
    }

   private:
    CTimerHandler();
    CTimerHandler(const CTimerHandler &) = delete;
    CTimerHandler(CTimerHandler &&) = delete;

   public:
    int single_shot(int delay_ms, CTimer::func_type handler);
    int launch(int id);
    int stop(int id);
    int remove(int id);
    int cycle_shot(const std::string &desc, int delay_ms, int interval, CTimer::func_type handler);
    int update_entry_interval(int id, int ms);

   private:
    void onHandler();
    auto find_one_ready_entry() -> int;
    auto find_one_free_entry() -> std::vector<handler_entry>::iterator;
    void fill_entry(std::vector<handler_entry>::iterator it, const std::string &desc, int delay, int interval,
                    CTimer::func_type handler);
    void onCallback(int index);
    int onPause(int index, bool pause);

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
