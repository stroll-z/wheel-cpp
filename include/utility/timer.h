/**
 * @file timer.h
 * @author your name (you@domain.com)
 * @brief 定时器
 * @version 0.1
 * @date 2023-10-25
 *
 * @copyright Copyright (c) 2023
 *
 */

#pragma once

#include <functional>

namespace wheel {

class CTimer
{
public:
    /// @brief
    using func_type = std::function<void(void)>;

public:
    CTimer() = default;
    CTimer(int delay, int interval, func_type func);

    /// @brief 单次调用, 仅调用一次
    /// @param delay 延迟执行,单位ms
    /// @param handler 执行句柄
    /// @return 0-成功, 其他失败
    static int single_shot(int delay, func_type func);

private:
    int delay =0;
    int interval = 0;
    func_type handler;
};

} // namespace wheel
