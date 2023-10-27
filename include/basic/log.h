/**
 * @file log.h
 * @author your name (you@domain.com)
 * @brief 打印部分封装
 * @version 0.1
 * @date 2023-10-25
 *
 * @copyright Copyright (c) 2023
 *
 */

#pragma once

#include <cstdio>

#ifdef LOG_E
#undef LOG_E
#endif

#ifdef LOG_I
#undef LOG_I
#endif

#ifdef LOG_D
#undef LOG_D
#endif

#ifdef LOG_W
#undef LOG_W
#endif

#define COLOR_NOR "\e[m"
#define COLOR_RED "\e[31m"
#define COLOR_GREEN "\e[32m"
#define COLOR_YELLOW "\e[33m"
#define COLOR_DP_GREEN "\e[36m"
#define COLOR_WHITE "\e[37m"

#define LOG_BASE(color, tag, fmt, ...)                                                                                 \
    printf(color "[%s][%s:%d] " fmt "\n" COLOR_NOR, tag, __func__, __LINE__, ##__VA_ARGS__)

#define LOG_E(tag, fmt, ...) LOG_BASE(COLOR_RED, "E/" tag, fmt, ##__VA_ARGS__)

#if defined(DEBUG_TRACE_POINT)
#define LOG_D(tag, fmt, ...) LOG_BASE(COLOR_WHITE, "D/" tag, fmt, ##__VA_ARGS__)
#else
#define LOG_D(tag, fmt, ...)
#endif

#define LOG_I(tag, fmt, ...) LOG_BASE(COLOR_GREEN, "I/" tag, fmt, ##__VA_ARGS__)

#define LOG_W(tag, fmt, ...) LOG_BASE(COLOR_YELLOW, "W/" tag, fmt, ##__VA_ARGS__)

#define LOGV(tag, fmt, ...) LOG_BASE(COLOR_WHITE, "V/" tag, fmt, ##__VA_ARGS__)
