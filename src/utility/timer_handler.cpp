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

#define TAG "timer"
const int INIT_THREAD_NUM = 4;

CTimerHandler &CTimerHandler::instance(void) {
    static CTimerHandler handler;
    return handler;
}

CTimerHandler::CTimerHandler() {
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

    {  // 唤醒一个线程去处理
        std::unique_lock<std::mutex> lock(m_mtx);
        m_readyFlag = true;
    }
    m_cond.notify_one();
}

int CTimerHandler::single_shot(int delay_ms, CTimer::func_type handler) {
    if (delay_ms < 0 || !static_cast<bool>(handler)) {
        LOGE(TAG, "invalid param");
        return -1;
    }

    std::lock_guard<std::mutex> guard(m_poolMtx);
    auto it = find_one_free_entry();
    if (it == m_entrySet.end()) {
        m_entrySet.emplace_back("", delay_ms, -1, handler);
        return 0;
    }
    fill_entry(it, "", delay_ms, -1, handler);
    return 0;
}

int CTimerHandler::cycle_shot(const char *desc, int delay_ms, int interval, CTimer::func_type handler) {
    if (!desc || interval < 0 || delay_ms < 0 || !static_cast<bool>(handler)) {
        LOGE(TAG, "param invalid");
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

void CTimerHandler::fill_entry(std::vector<handler_entry>::iterator it, const char *desc, int delay, int interval,
                               CTimer::func_type handler) {
    it->deleted = false;
    it->interval = interval;
    it->delay = delay;
    it->description = desc;
    it->func = handler;
    it->work_flag = false;
}

auto CTimerHandler::find_one_free_entry() -> std::vector<handler_entry>::iterator {
    for (auto it = m_entrySet.begin(); it != m_entrySet.end(); ++it) {
        if (it->deleted) {
            return it;
        }
    }
    return m_entrySet.end();
}

void CTimerHandler::onHandler() {
    while (!m_exitFlag.load()) {
        std::unique_lock<std::mutex> lock(m_mtx);
        m_freeThrNum += 1;
        while (!m_readyFlag) {  // 防止意外唤醒
            m_cond.wait(lock);
        }
        m_freeThrNum -= 1;
        m_readyFlag = false;
        lock.unlock();

        /// 找一个待处理的句柄
        auto index = find_one_ready_entry();
        if (index == -1) {
            LOGE(TAG, "Are you quitting?");
            continue;
        }

        // 检查是否有空闲的线程,如果有提升一个线程来检查准备好的handler
        while (m_freeThrNum == 0) {
            std::this_thread::yield();
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
        onCallback(index);
    }
    LOGW(TAG, "exit flag is set, and thread exit");
}

auto CTimerHandler::find_one_ready_entry() -> int {
    std::lock_guard<std::mutex> guard(m_poolMtx);
    while (!m_exitFlag.load()) {
        for (auto it = m_entrySet.begin(); it != m_entrySet.end(); ++it) {
            if (it->deleted) {
                continue;
            }
        }
        std::this_thread::yield();
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
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
        item.work_flag = true;
        item.tp = std::chrono::steady_clock::now();
        cb = item.func;
    }

    cb();  // 执行准备好的handler
    {
        std::lock_guard<std::mutex> guard(m_poolMtx);
        auto &item = m_entrySet.at(index);
        item.work_flag = false;
    }
}

}  // namespace wheel
