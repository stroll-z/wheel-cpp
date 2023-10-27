/**
 * @file round_queue.h
 * @author your name (you@domain.com)
 * @brief 循环队列
 * @version 0.1
 * @date 2023-10-27
 *
 * @copyright Copyright (c) 2023
 *
 */

/**
 *            注意:
 * 考虑到性能问题, 这里采用无锁设计
 * 基于一个生产者, 一个消费者模式来实现
 * 其他情况下有竞争问题,不要使用
 */

#pragma once

#include <atomic>
#include <cstddef>
#include <vector>

namespace wheel {

template <class T, int N>
class CRoundQueue {
    static_assert(N > 1, "N n must be greater than 1\n");

   public:
    using index_type = std::atomic<int>;

   public:
    CRoundQueue() : m_buffer(N) {}

    bool empty() { return next_index(m_readIndex) == m_writeIndex; }

    bool full() { return m_readIndex == m_writeIndex; }

    int next_index(int index) { return (index + 1 == N) ? 0 : index + 1; }

    int push(const T &ele) {
        if (full()) {
            return -1;
        }
        m_buffer[m_writeIndex] = ele;
        m_writeIndex = next_index(m_writeIndex);
        return m_writeIndex;
    }

    int push(T &&rhs) {
        if (full()) {
            return -1;
        }
        m_buffer[m_writeIndex] = std::move(rhs);
        m_writeIndex = next(m_writeIndex);
        return m_writeIndex;
    }

    /// @brief 用户保证队列非空, 否则结果未知
    /// @return
    T pop() {
        auto index = next_index(m_readIndex);
        auto v = std::move(m_buffer[index]);
        m_readIndex = index;
        return v;
    }

   private:
    index_type m_writeIndex {1};
    index_type m_readIndex {0};
    std::vector<T> m_buffer;
};

}  // namespace wheel
