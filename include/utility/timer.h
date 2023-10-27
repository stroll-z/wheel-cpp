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
#include <string>

namespace wheel {

class CTimer final {
   public:
    /// @brief
    using func_type = std::function<void(void)>;

   public:
    /// @brief 构造定时器
    /// @param desc 描述语句, 异常时可辨识
    /// @param delay 第一次延迟执行
    /// @param interval 执行间隔
    /// @param func 执行体
    CTimer(const char *desc, int delay, int interval, func_type func);

    ~CTimer();

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
    int m_id = -1;
    int m_delay = 0;
    int m_interval = 0;
    func_type m_func;
    std::string m_description;
};

}  // namespace wheel
