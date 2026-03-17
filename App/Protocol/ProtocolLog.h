#ifndef APP_PROTOCOL_PROTOCOLLOG_H_
#define APP_PROTOCOL_PROTOCOLLOG_H_

#include <stdarg.h>
#include <stdio.h>
#include <sys/time.h>
#include <time.h>

namespace protocol {

inline int ProtocolVfprintf(FILE* stream, const char* fmt, va_list args) {
    if (stream == NULL) {
        stream = stdout;
    }

    struct timeval tv;
    gettimeofday(&tv, NULL);

    time_t now = tv.tv_sec;
    struct tm tm_now;
    localtime_r(&now, &tm_now);

    char time_text[32] = {0};
    strftime(time_text, sizeof(time_text), "%Y-%m-%d %H:%M:%S", &tm_now);

    const int prefix_ret = fprintf(stream, "%s.%03ld ", time_text, (long)(tv.tv_usec / 1000));
    const int body_ret = vfprintf(stream, fmt == NULL ? "" : fmt, args);
    fflush(stream);

    if (prefix_ret < 0 || body_ret < 0) {
        return -1;
    }
    return prefix_ret + body_ret;
}

inline int ProtocolPrintf(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    const int ret = ProtocolVfprintf(stdout, fmt, args);
    va_end(args);
    return ret;
}

inline int ProtocolFprintf(FILE* stream, const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    const int ret = ProtocolVfprintf(stream, fmt, args);
    va_end(args);
    return ret;
}

}  // namespace protocol

#endif  // APP_PROTOCOL_PROTOCOLLOG_H_
