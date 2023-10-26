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

/**
 * 执行体耗时应小于设置的执行间隔
 * 如果超过间隔, 在间隔时间到时,并不会再次执行
 * 需要等待执行体执行完成后,避免一个长耗时的handler阻塞了整个定时器
 */

#pragma once

#include <functional>

namespace wheel {

class CTimer {
   public:
    /// @brief
    using func_type = std::function<void(void)>;

   public:
    /// @brief 构造定时器
    /// @param delay 第一次延迟执行
    /// @param interval 执行间隔
    /// @param func 执行体
    CTimer(int delay, int interval, func_type func);

    /// @brief 单次调用, 仅调用一次
    /// @param delay 延迟执行,单位ms
    /// @param handler 执行句柄
    /// @return 0-成功, 其他失败
    static int single_shot(int delay, func_type func);

    /// @brief 设置执行间隔
    /// @param ms 新的间隔
    /// @return
    int set_interval(int ms);

    /// @brief 启动定时器
    /// @param
    /// @return
    int start(void);

    /// @brief 停止定时器
    /// @param
    /// @return
    int stop(void);

   private:
    int delay = 0;
    int interval = 0;
    func_type handler;
};

}  // namespace wheel
