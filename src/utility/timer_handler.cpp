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

    {  // 唤醒一个线程去处理
        std::unique_lock<std::mutex> lock(m_mtx);
        m_readyFlag.store(true);
    }
    m_cond.notify_one();
}

int CTimerHandler::single_shot(int deley_ms, CTimer::func_type handler) {
    LOGE("ctimer", "single_shot");
    return -1;
}

void CTimerHandler::onHandler() {
    while (!m_exitFlag.load()) {
        std::unique_lock<std::mutex> lock(m_mtx);
        m_freeThrNum.fetch_add(1);
        m_cond.wait(lock);
        m_freeThrNum.fetch_sub(1);
        if (!m_readyFlag.load()) {  // 意外唤醒
            m_readyFlag.store(false);
            continue;
        }
        m_readyFlag.store(false);

        auto index = find_one_ready_entry();
        if (index == -1) {
            LOGE(TAG, "Are you quitting?");
            continue;
        }

        // 检查是否有空闲的线程,如果有提升一个线程来检查准备好的handler
        while (m_freeThrNum.load() == 0) {
            std::this_thread::yield();
        }
    }
    LOGW(TAG, "exit flag is set, and thread exit");
}

auto CTimerHandler::find_one_ready_entry() -> int {
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
    auto &item = m_entrySet.at(index);
    item.work_flag = true;
    item.tp = std::chrono::steady_clock::now();
    lock.unlock();
    m_cond.notify_one();
    item.func();  // 执行准备好的handler
}

}  // namespace wheel
