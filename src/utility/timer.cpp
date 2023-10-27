/**
 * @file timer.cpp
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2023-10-25
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "utility/timer.h"

#include "basic/log.h"
#include "timer_handler.h"

namespace wheel {

#define TAG "timer"

CTimer::CTimer(const char *desc, int delay, int interval, func_type func)
    : m_delay(delay), m_interval(interval), m_func(func), m_description(desc) {}

CTimer::~CTimer() {
    LOG_D(TAG, "c timer destructor");
    if (m_id == -1) {
        return;
    }
    stop();
    CTimerHandler::instance().remove(m_id);
}

int CTimer::single_shot(int delay, func_type func) {
    auto &handler = CTimerHandler::instance();
    return handler.single_shot(delay, func);
}

int CTimer::set_interval(int ms) {
    m_interval = ms;
    if (m_id == -1) {
        return 0;
    }

    auto &handler = CTimerHandler::instance();
    return handler.update_entry_interval(m_id, ms);
}

/// @brief 启动定时器
/// @param
/// @return
int CTimer::start(void) {
    auto &handler = CTimerHandler::instance();
    if (m_id != -1) {
        return handler.launch(m_id);
    }

    m_id = handler.cycle_shot(m_description, m_delay, m_interval, m_func);
    LOG_D(TAG, "m_id:%d", m_id);
    return 0;
}

/// @brief 停止定时器
/// @param
/// @return
int CTimer::stop(void) {
    if (m_id == -1) {
        LOG_E(TAG, "not start");
        return -1;
    }

    auto &handler = CTimerHandler::instance();
    return handler.stop(m_id);
}

}  // namespace wheel
