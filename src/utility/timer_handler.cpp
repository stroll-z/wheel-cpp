/**
 * @file timer_handler.cpp
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2023-10-25
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "timer_handler.h"

#include <thread>

#include "basic/log.h"

namespace wheel {

#define TAG "timer-handler"
const int INIT_THREAD_NUM = 8;

#define CHECK_HANDLER_ID(ID, VC)                                                                                       \
    do {                                                                                                               \
        if (ID == -1 || ID >= VC.size()) {                                                                             \
            LOG_E(TAG, "invalid id");                                                                                  \
            return -1;                                                                                                 \
        }                                                                                                              \
    } while (0)

CTimerHandler &CTimerHandler::instance(void) {
    static CTimerHandler handler;
    return handler;
}

CTimerHandler::CTimerHandler() {
    TIMER_DEBUG("timer-handler", "constructor");
    // 创建线程可能会失败而抛出异常, 构造里抛异常肯定不行. 这里要catch异常
    // TODO catch exception
    for (auto i = 0; i < INIT_THREAD_NUM; ++i) {
        std::thread thr{&CTimerHandler::onHandler, this};
        thr.detach();
    }

    // 等待子线程在条件变量上阻塞
    while (m_freeThrNum != INIT_THREAD_NUM) {
        std::this_thread::yield();
    }

    TIMER_DEBUG(TAG, "free thread num:%d", m_freeThrNum);
    {  // 唤醒一个线程去处理
        std::unique_lock<std::mutex> lock(m_mtx);
        m_readyFlag = true;
    }
    m_cond.notify_one();
}

int CTimerHandler::single_shot(int delay_ms, CTimer::func_type handler) {
    if (delay_ms < 0 || !static_cast<bool>(handler)) {
        LOG_E(TAG, "invalid param");
        return -1;
    }

    std::lock_guard<std::mutex> guard(m_poolMtx);
    auto it = find_one_free_entry();
    if (it == m_entrySet.end()) {
        m_entrySet.emplace_back("unknown", delay_ms, -1, handler);
        TIMER_DEBUG(TAG, "emplace entry");
        return 0;
    }
    fill_entry(it, std::string("unknown"), delay_ms, -1, handler);
    return 0;
}

int CTimerHandler::cycle_shot(const std::string &desc, int delay_ms, int interval, CTimer::func_type handler) {
    if (interval < 0 || delay_ms < 0 || !static_cast<bool>(handler)) {
        LOG_E(TAG, "param invalid");
        return -1;
    }

    std::lock_guard<std::mutex> guard(m_poolMtx);
    auto it = find_one_free_entry();
    if (it == m_entrySet.end()) {
        m_entrySet.emplace_back(desc, delay_ms, interval, handler);
        return m_entrySet.size() - 1;
    }
    fill_entry(it, desc, delay_ms, interval, handler);
    return it - m_entrySet.begin();
}

void CTimerHandler::fill_entry(std::vector<handler_entry>::iterator it, const std::string &desc, int delay,
                               int interval, CTimer::func_type handler) {
    it->deleted_ = false;
    it->interval_ = interval;
    it->delay_ = delay;
    it->description_ = desc;
    it->func_ = handler;
    it->work_flag_ = false;
    it->pause_ = false;
    it->tp_ = std::chrono::steady_clock::now();
}

auto CTimerHandler::find_one_free_entry() -> std::vector<handler_entry>::iterator {
    auto end_it = m_entrySet.end();
    for (auto it = m_entrySet.begin(); it != end_it; ++it) {
        if (it->deleted_) {
            TIMER_DEBUG("find free entry: %d", static_cast<int>(end_it - it));
            return it;
        }
    }
    return m_entrySet.end();
}

int CTimerHandler::update_entry_interval(int id, int ms) {
    if (id < 0 || id >= m_entrySet.size() || ms < 0) {
        LOG_E(TAG, "invalid params");
        return -1;
    }

    std::lock_guard<std::mutex> guard(m_poolMtx);
    auto &item = m_entrySet[id];
    item.interval_ = ms;
    return 0;
}

int CTimerHandler::launch(int id) { return onPause(id, false); }

int CTimerHandler::stop(int id) { return onPause(id, true); }

int CTimerHandler::onPause(int index, bool pause) {
    CHECK_HANDLER_ID(index, m_entrySet);
    LOG_I(TAG, "pause: %d", pause);
    std::lock_guard<std::mutex> guard(m_poolMtx);
    auto &item = m_entrySet[index];
    item.pause_ = pause;
    item.tp_ = std::chrono::steady_clock::now();
    return 0;
}

int CTimerHandler::remove(int id) {
    CHECK_HANDLER_ID(id, m_entrySet);
    std::lock_guard<std::mutex> guard(m_poolMtx);
    auto &item = m_entrySet[id];
    item.deleted_ = true;
    item.func_ = nullptr;
    return 0;
}

void CTimerHandler::onHandler() {
    while (!m_exitFlag.load()) {
        std::unique_lock<std::mutex> lock(m_mtx);
        m_freeThrNum += 1;
        TIMER_DEBUG(TAG, "onHandler: %d", m_freeThrNum);
        while (!m_readyFlag) {  // 防止意外唤醒
            m_cond.wait(lock);
        }
        m_freeThrNum -= 1;
        m_readyFlag = false;
        lock.unlock();

        TIMER_DEBUG(TAG, "to find ready entry: %d", m_freeThrNum);
        /// 找一个待处理的句柄
        auto index = find_one_ready_entry();
        if (index == -1) {
            LOG_E(TAG, "Are you quitting?");
            continue;
        }

        // 检查是否有空闲的线程,如果有提升一个线程来检查准备好的handler
        while (m_freeThrNum == 0) {
            std::this_thread::yield();
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
        onCallback(index);
    }
    LOG_W(TAG, "exit flag is set, and thread exit");
}

auto CTimerHandler::find_one_ready_entry() -> int {
    while (!m_exitFlag.load()) {
        {
            auto curr_tp = std::chrono::steady_clock::now();
            std::lock_guard<std::mutex> guard(m_poolMtx);
            auto end_it = m_entrySet.end();
            for (auto it = m_entrySet.begin(); it != end_it; ++it) {
                if (it->deleted_ || it->work_flag_ || it->pause_) {
                    continue;
                }

                if (it->delay_ > 0) { /// 首次延迟执行
                    if (it->tp_ + std::chrono::milliseconds(it->delay_) < curr_tp) {
                        it->delay_ = -1;
                        return it - m_entrySet.begin();
                    }
                    continue;
                }

                if (it->delay_== 0) { /// 首次执行 无延迟
                    it->delay_ = -1;
                    return it - m_entrySet.begin();
                }

                /// 间隔执行
                if (it->tp_ + std::chrono::milliseconds(it->interval_) < curr_tp) {
                    return it - m_entrySet.begin();
                }
            }
        }
        std::this_thread::yield();
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    return -1;
}

void CTimerHandler::onCallback(int index) {
    m_mtx.lock();
    m_readyFlag = true;
    m_mtx.unlock();
    m_cond.notify_one();

    CTimer::func_type cb;
    {
        std::lock_guard<std::mutex> guard(m_poolMtx);
        auto &item = m_entrySet.at(index);
        item.work_flag_ = true;
        //< 没有执行间隔的, 直接设置为挂起状态
        item.pause_ = item.interval_ <= 0? true: item.pause_;
        item.tp_ = std::chrono::steady_clock::now();
        cb = item.func_;
        if (item.interval_ == -1) {
            item.deleted_ = true;
        }
    }

    if (cb) cb();  // 执行准备好的handler
    {
        std::lock_guard<std::mutex> guard(m_poolMtx);
        auto &item = m_entrySet.at(index);
        item.work_flag_ = false;
    }
}

}  // namespace wheel
