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
#include "basic/log.h"
#include <iostream>

using namespace wheel;

void onTimer() {
    static int count = 0;
    LOG_I("test-timer", "%d", ++count);
}

void onTimer1() {
    static int count = 0;
    LOG_I("test-timer1", "%d", ++count);
}

void onTimer2() {
    static int count = 0;
    LOG_I("test-timer2", "%d", ++count);
}

int main() {
    // int count = 0;
    // CTimer::single_shot(100, onTimer);

    CTimer timer{"test-timer1", 1000, 1000, onTimer1};
    timer.start();

    CTimer timer2{"test-timer2", 1000, 10000, onTimer2};
    timer2.start();

    char c;
    std::cin >> c;
    LOG_W("", "%c", c);
    return 0;
}
