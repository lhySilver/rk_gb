#ifndef _STORAGECOMMON_H_
#define _STORAGECOMMON_H_

#include <map>
#include <list>
#include <cstddef>
#include <stdio.h>
#include <stdarg.h>
#include <sstream>

#define DG_STORAGE_MOUDLE_VER 	"3.0.0" 		//录像模块版本号
#define __STORAGE_SD_MOUNT_PATH__ 	"/mnt/sdcard" 		//SD卡挂载路径

#if 0
#define __STORAGE_SYSTEM__ 		system
#else
#define __STORAGE_SYSTEM__( file, cmd, ... ) \
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
#endif



typedef signed char 		   Int8;
typedef unsigned char		   UInt8;
typedef signed short		   Int16;
typedef unsigned short		   UInt16;
typedef signed int			   Int32;
typedef unsigned int		   UInt32;
typedef signed long 		   IntPtr;
typedef unsigned long		   UIntPtr;
typedef signed long long   Int64;
typedef unsigned long long UInt64;
typedef unsigned long			DWORD;
typedef unsigned short			WORD;
typedef int BOOL;
#define TRUE 1
#define FALSE 0


typedef enum 
{
	LOG_NONE			= 0,
	LOG_ALARM			= 1,      /**< 日志类型，1-报警; 2-异常; 3-操作; 255-全部**/
	LOG_ABNORMAL		= 2,      
	LOG_OPERATION		= 3,     
	LOG_TYPENUM,    	
	ALL_LOG_TYPE	= 255,   
} LOG_TYPE;

typedef struct {
	short type;				// 日志类型
	short videoId;			// 视频通道号
	short diskNumber;		// 硬盘号
	short AlarmInPort;		// 告警输入端口
	short AlarmOutPort;		// 告警输出端口
	short netIpPort;			// IP端口
	char netIpAddr[16];
	char netUser[32];			// 网络用户名	
} T_LogRecInfo;

//需要根据协议，在重新设计,问题
typedef struct 
{
	UInt32 uiTime;  
	T_LogRecInfo recInfo; //40
	char cContent[200];
} T_Log;

typedef enum{
	STORAGE_VIDEO_ENC_H264,
	STORAGE_VIDEO_ENC_H265,
}STORAGE_VIDEO_ENC_TYPE_E;

#endif 	//_STORAGECOMMON_H_

