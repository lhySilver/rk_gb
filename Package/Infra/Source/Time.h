#ifndef __SUPPORT_H__
#define __SUPPORT_H__

#include <stdarg.h>

#define errorf(format, ...) logError(format, ## __VA_ARGS__)

// 记录上次输出最后一个字符是否是换行，还不完全可靠
static bool returned = true;

#define LOG_MSG(x) \
	char buffer[8192]; \
	if(returned) \
{ \
	CTime t = CTime::getCurrentTime(); \
	printf("%02d:%02d:%02d|", t.hour, t.minute, t.second); \
	printf(x); \
} \
	size_t n; \
	va_list ap; \
	va_start(ap, fmt); \
	assert(strlen(fmt) < 8192); \
	n = vsprintf(buffer, fmt, ap); \
	fputs(buffer, stdout); \
	returned = (n && buffer[n - 1] == '\n'); \
	return(n);

int logError(const char* fmt, ...)
{
	LOG_MSG("error ");
}

#endif
