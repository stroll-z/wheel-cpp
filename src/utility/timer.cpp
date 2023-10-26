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

#include "timer_handler.h"

namespace wheel {

CTimer::CTimer(const char *desc, int delay, int interval, func_type func)
    : m_delay(delay), m_interval(interval), m_handler(func) {
    auto &handler = CTimerHandler::instance();
    m_id = handler.cycle_shot(desc, delay, interval, func);
}

int CTimer::single_shot(int delay, func_type func) {
    auto &handler = CTimerHandler::instance();
    return handler.single_shot(delay, func);
}

int CTimer::set_interval(int ms) {}

/// @brief 启动定时器
/// @param
/// @return
int CTimer::start(void) {}

/// @brief 停止定时器
/// @param
/// @return
int CTimer::stop(void) {}

}  // namespace wheel
