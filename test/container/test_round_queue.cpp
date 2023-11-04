/**
 * @file test_round_queue.cpp
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2023-10-27
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "basic/log.h"
#include "container/round_queue.h"
#include "utility/timer.h"
#include <iostream>
#include <thread>

using namespace wheel;

#define TAG "test"

CRoundQueue<int, 4> s_list;

void sender() {
    static int count  = 0;
    auto r = s_list.push(++count);
    if (r == -1) {
        LOG_E(TAG, "push fail");
    }
}

void recv() {

    if (s_list.empty()) {
        LOG_W(TAG, "list is empty");
        return;
    }

    auto r = s_list.pop();
    LOG_D(TAG, "r: %d", r);
}

void test() {
    CTimer t1("t1", 0, 1000, sender);
    CTimer t2("t2", 0, 900, recv);

    t1.start();
    t2.start();

    char c;
    std::cin >> c;
}

int main() {
    CRoundQueue<int, 2> list;
    test();
    return 0;
}
