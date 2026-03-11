/***************************************************************************
 *   Copyright (C) 2009 by stargui   *
 *   stargui@localhost.localdomain   *
 *                                   *
 * @author	guijianzhang
   @date	2009-05-21
 ***************************************************************************/
#include "debuginfo.h"
static int debug_level = LEVEL_MESSAGE;
static char model_name[10] = "";


#define NONE         "\033[m"			/// 默认颜色(黑、白)
#define RED          "\033[0;32;31m"	/// 红色
#define LIGHT_RED    "\033[1;31m"		/// 鲜红
#define GREEN        "\033[0;32;32m"	/// 绿色
#define LIGHT_GREEN  "\033[1;32m"		/// 鲜绿
#define BLUE         "\033[0;32;34m"	/// 蓝色
#define LIGHT_BLUE   "\033[1;34m"		/// 鲜蓝
#define DARY_GRAY    "\033[1;30m"		/// 灰黑
#define CYAN         "\033[0;36m"		/// 蓝绿色
#define LIGHT_CYAN   "\033[1;36m"		/// 高亮蓝绿
#define PURPLE       "\033[0;35m"		/// 紫色
#define LIGHT_PURPLE "\033[1;35m"		/// 高亮紫色
#define BROWN        "\033[0;33m"		/// 褐色
#define YELLOW       "\033[1;33m"		/// 黄色
#define LIGHT_GRAY   "\033[0;37m"		/// 灰白色
#define WHITE        "\033[1;37m"		/// 白色


int DebugSetLevel( int level )
{
	int ret = debug_level;
	debug_level = level;
	return ret;
}

int DebugSetModelName(char *name )
{
	strncpy(model_name,name,10);
	model_name[9] = '\0';
	return 0;
}

void DebugPrintf(int level, const char *file, const char *func, int line, const char *fmt, ...)
{	
	if( level <= debug_level )
	{
        //printf("\n");
		va_list args;
		struct timeval tv;
		gettimeofday(&tv, NULL);
		//printf("[%s:%s:%d: Time:%ld.%03ld] ", rindex(file,'/')+1, func, line, tv.tv_sec, tv.tv_usec / 1000);
		//printf("[%ld.%03ld] ", tv.tv_sec, tv.tv_usec / 1000);

		
		time_t __curTime; 
		struct tm __curDate; 
		__curTime = time(NULL); 
		localtime_r(&__curTime, &__curDate);

		char color[50]={0};
		if (LEVEL_ERROR	== level)
		{
			strcpy(color,RED);
		}
		else if (LEVEL_CRITICAL == level)
		{
			strcpy(color,RED);
		}
		else if (LEVEL_WARNING == level)
		{
			strcpy(color,YELLOW);
		}
		else if (LEVEL_MESSAGE == level)
		{
			strcpy(color,GREEN);
		}
		else if (LEVEL_DEBUG == level)
		{
			strcpy(color,CYAN);
		}
		else
		{
			strcpy(color,NONE);
		}
		printf("%s%d-%02d-%02d %02d:%02d:%02d [%ld:%03ld] %s %s %04d %s(): ", \
				color, \
				__curDate.tm_year + 1900, \
				__curDate.tm_mon + 1, \
				__curDate.tm_mday, \
				__curDate.tm_hour, \
				__curDate.tm_min,  \
				__curDate.tm_sec,  \
				tv.tv_sec, \
				(tv.tv_usec / 1000), \
				model_name,
				file, \
				line, \
				func);
				
		va_start (args, fmt);
		vprintf(fmt, args);
		va_end (args);
		printf("\n" NONE);
	}
}

int DebugFramerate( unsigned long *frames, unsigned long *ticks, int len, const char *fmt, ...)
{
	va_list args;
	float framerate;
	unsigned long nowticks;
	struct timeval tv;
	if(debug_level >= LEVEL_DEBUG)
	{
		*frames = *frames + len;
		gettimeofday(&tv, NULL);
		nowticks = (tv.tv_sec) * 1000 + (tv.tv_usec) / 1000;
		if ( (nowticks - *ticks) >= (15 * 1000) )
		{
			framerate = (*frames * 1000.0) / (nowticks - *ticks);
			*ticks = nowticks;
			*frames = 0;
			va_start (args, fmt);
			vprintf (fmt, args);
			va_end (args);
			return printf("%.2f\n", framerate);
		}
	}
	return 0;
}


