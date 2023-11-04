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

#include <iostream>
#include <thread>
#include <chrono>

#include "basic/log.h"
#include "utility/timer.h"

using namespace wheel;

void onTimer() {
    static int count = 0;
    LOG_I("test-timer", "timer: %d", ++count);
}

void onTimer1() {
    static int count = 0;
    LOG_I("test-timer1", "timer1: %d", ++count);
    std::this_thread::sleep_for(std::chrono::milliseconds(1300));
}

void onTimer2() {
    static int count = 0;
    LOG_I("test-timer2", "timer2: %d", ++count);
}

int main() {
    LOG_I("TEST", "single shot");
    CTimer::single_shot(1000, onTimer);

    CTimer timer2{"test-timer2", 1000, 0, onTimer2};
    timer2.start();

    while (1) {
        char c;
        std::cin >> c;
        LOG_W("", "%c", c);
        if (c == 'q') {
            break;
        }

        if (c == 's') {
            timer2.set_interval(1000);
        }

        if (c == 'p') {
            timer2.stop();
        }

        if (c == 'r') {
            timer2.start();
        }
    }
    return 0;
}
