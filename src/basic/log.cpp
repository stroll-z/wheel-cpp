/**
 * @file log.cpp
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2023-10-27
 *
 * @copyright Copyright (c) 2023
 *
 */

#include <ctime>
#include <cstdio>

namespace wheel {

const char *make_current_ts() {
    static char s_buffer[64] = {0};
    timespec ts;
    clock_gettime(_CLOCK_REALTIME, &ts);
    auto day = localtime(&ts.tv_sec);
    snprintf(s_buffer, sizeof(s_buffer), "%d%02d%02d %02d:%02d:%02d.%03d", 
        day->tm_year + 1900, 
        day->tm_mon + 1, 
        day->tm_mday,
        day->tm_hour,
        day->tm_min,
        day->tm_sec,
        static_cast<int>(ts.tv_nsec/1000/1000));
    return s_buffer;
}

}  // namespace wheel
