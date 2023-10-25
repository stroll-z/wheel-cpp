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

#include "utility/timer.h"

namespace wheel
{

class CTimerHandler
{
public:
    static CTimerHandler &instance(void);

private:
    CTimerHandler() = default;
    CTimerHandler(const CTimerHandler &) =delete;
    CTimerHandler(CTimerHandler &&) =delete;

public:
    int single_shot(int deley_ms, CTimer::func_type handler);
};

} // namespace wheel
