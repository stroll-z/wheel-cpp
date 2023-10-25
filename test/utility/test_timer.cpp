/**
 * @file test_timer.cpp
 * @author your name (you@domain.com)
 * @brief 测试定时器
 * @version 0.1
 * @date 2023-10-25
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "utility/timer.h"

using namespace wheel;

int main() {
    CTimer timer;
    timer.single_shot(100, []() -> void {});
}
