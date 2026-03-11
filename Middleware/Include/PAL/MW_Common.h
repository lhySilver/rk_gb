
#ifndef __PAL_AVCOMMON_H__
#define __PAL_AVCOMMON_H__

#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>
#include <sys/time.h>
#include <errno.h>

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>
#include <time.h>

#define LOG_VERSION "0.1.0"

typedef struct {
	va_list ap;
	const char *fmt;
	const char *file;
	struct tm *time;
	void *udata;
	int line;
	int level;
} log_Event;

typedef void (*log_LogFn)(log_Event *ev);
typedef void (*log_LockFn)(bool lock, void *udata);

enum { LOG_TRACE, LOG_DEBUG, LOG_INFO, LOG_WARN, LOG_ERROR, LOG_FATAL };

#define log_trace(...) log_log(LOG_TRACE, __FILE__, __LINE__, __VA_ARGS__)
#define log_debug(...) log_log(LOG_DEBUG, __FILE__, __LINE__, __VA_ARGS__)
#define log_info(...)  log_log(LOG_INFO,  __FILE__, __LINE__, __VA_ARGS__)
#define log_warn(...)  log_log(LOG_WARN,  __FILE__, __LINE__, __VA_ARGS__)
#define log_error(...) log_log(LOG_ERROR, __FILE__, __LINE__, __VA_ARGS__)
#define log_fatal(...) log_log(LOG_FATAL, __FILE__, __LINE__, __VA_ARGS__)

const char* log_level_string(int level);
void log_set_lock(log_LockFn fn, void *udata);
void log_set_level(int level);
void log_set_quiet(bool enable);
int  log_add_callback(log_LogFn fn, void *udata, int level);
int  log_add_fp(FILE *fp, int level);

void log_log(int level, const char *file, int line, const char *fmt, ...);

#define MW_START_PROCESS( file, cmd, ... ) \
{\
	int status;\
	pid_t pid = vfork( );\
	if ( pid  == 0 )\
	{\
		execlp ( file, cmd, ##__VA_ARGS__ );\
		exit ( -1 );\
	}\
	waitpid ( pid, &status, 0 );\
}

#define MW_START_PROCESS_CLOSE_FD( file, cmd, ... ) \
{\
	int status;\
	pid_t pid = vfork( );\
	if ( pid  == 0 )\
	{\
		int pid;\
		char path[256];\
		DIR *dir = NULL;\
		struct dirent *entry = NULL;\
		pid = getpid();\
		snprintf(path, sizeof(path), "/proc/%d/fd", pid);\
		dir = opendir(path);\
		if(NULL == dir)\
		{\
			printf("open %s fail!\n", path);\
		}\
		else\
		{\
			while( ( entry = readdir(dir) ) != NULL )\
			{\
				char path2[256];\
				char realpath[256];\
				snprintf(path2, sizeof(path2), "%s/%s", path, entry->d_name);\
				memset(realpath, 0, sizeof(realpath));\
				readlink(path2, realpath, sizeof(realpath)-1);\
				if( strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0 && \
					strcmp(entry->d_name, "0") != 0 && strcmp(entry->d_name, "1") != 0 && strcmp(entry->d_name, "2") != 0 )\
				{\
					if( strcmp(path, realpath) != 0 )\
					{\
						int fd = atoi(entry->d_name);\
						close(fd);\
					}\
				}\
			}\
			closedir(dir);\
		}\
		\
		execlp ( file, cmd, ##__VA_ARGS__ );\
		exit ( -1 );\
	}\
	waitpid ( pid, &status, 0 );\
}



#ifdef EMSG
#undef EMSG
#endif

#define EMSG log_error

#ifdef MSG
#undef MSG
#endif

#define MSG log_info

#define IS_NULL(p)  ((p) == NULL)
#define NOT_NULL(p) ((p) != NULL)

#ifdef __cplusplus
}
#endif

#endif

