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
#include "basic/log.h"

namespace wheel
{

    CTimerHandler &CTimerHandler::instance(void)
    {
        static CTimerHandler handler;
        return handler;
    }

    int CTimerHandler::single_shot(int deley_ms, CTimer::func_type handler) {
        LOGE("ctimer", "single_shot");
        return -1;
    }

} // namespace wheel
