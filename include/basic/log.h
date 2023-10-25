#pragma once

#include <cstdio>

#ifdef LOGE
#undef LOGE
#endif

#ifdef LOGI
#undef LOGI
#endif

#ifdef LOGD
#undef LOGD
#endif

#ifdef LOGW
#undef LOGW
#endif

#define COLOR_NOR "\e[m"
#define COLOR_RED "\e[31m"
#define COLOR_GREEN "\e[32m"
#define COLOR_YELLOW "\e[33m"
#define COLOR_DPGREEN "\e[36m"
#define COLOR_WHITE "\e[37m"

#define LOG_BASE(color, tag, fmt, ...) \
    printf(color "[%s][%s:%d] " fmt "\n" COLOR_NOR, tag, __func__, __LINE__, ##__VA_ARGS__)

#define LOGE(tag, fmt, ...) \
    LOG_BASE(COLOR_RED, "E/" tag, fmt, ##__VA_ARGS__)

#define LOGD(tag, fmt, ...) \
    LOG_BASE(COLOR_DPGREEN, "D/" tag, fmt, ##__VA_ARGS__)

#define LOGI(tag, fmt, ...) \
    LOG_BASE(COLOR_GREEN, "I/" tag, fmt, ##__VA_ARGS__)

#define LOGW(tag, fmt, ...) \
    LOG_BASE(COLOR_YELLOW, "W/" tag, fmt, ##__VA_ARGS__)

#define LOGV(tag, fmt, ...) \
    LOG_BASE(COLOR_WHITE, "V/" tag, fmt, ##__VA_ARGS__)
