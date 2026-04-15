#ifndef __COMMON_H__
#define __COMMON_H__

#include "PathDefinition.h"

#include "Infra/Infra.h"
#include "Infra/Packet.h"
#include "Infra/Security.h"
#include "Infra/Singleton.h"
#include "Infra/Thread.h"
#include "Infra/Timer.h"

#include "Types/Defs.h"

#include "PAL/Misc.h"
#include "PAL/MotionDetect.h"
#include "PAL/WatchDog.h"
#include "PAL/Camera.h"
#include "PAL/Capture.h"
#include "PAL/Audio.h"
#include "PAL/System.h"
#include "PAL/Motor.h"
#include "PAL/MotionTrack.h"
#include "PAL/Net.h"

#include "ExchangeAL/ExchangeKind.h"
#include "ExchangeAL/Exchange.h"
#include "ExchangeAL/ManagerExchange.h"
#include "ExchangeAL/NetIPAbilitySet.h"
#include "ExchangeAL/NetWorkExchange.h"

#include "Media/Record.h"
#include "Media/Alarm.h"
#include "Media/AudioManager.h"
#include "Media/AVManager.h"
#include "Media/IndicatorLight.h"
//#include "Media/AnBaoLightManager.h"
#include "Media/Siren.h"
#include "Media/Camera.h"
#include "Media/Storage.h"
#include "Media/MediaDefaultConfig.h"
#include "Media/AudioPrompt.h"

#include "Net/NetConfigHook.h"
#include "Manager/ManagerDefaultConfig.h"
#include "Manager/EventManager.h"
#include "Manager/MagicBox.h"
#include "Log/Log.h"

#include "Log/DebugDef.h"
#include "Crypto/crypto.h"
#include "DebugInfo/debuginfo.h"
#include "QrCode/QrCode.h"
#include "Encryption.h"
#include "ProductCof.h"
#include "ProduceNew/Produce.h"
#include "Ptz/Ptz.h"
#include "Pcba/Pcba.h"
#include "Wdt/Wdt.h"
#include "tuya/Tuya.h"
// #include "update.h"

#include "Rtsp/live_rtsp.h"
#include "Onvif/Onvif.h"

#include <list>
#include <deque>
#include <vector>
#include <signal.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <dirent.h>
#include <limits.h>
#include <string.h>
#include <stdio.h>
#include <limits.h>
#include <errno.h>
#include <sys/sysinfo.h>
#include <sys/times.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <sys/syscall.h> 

typedef void *(*funcThreadRoute)(void*);
bool CreateDetachedThread(funcThreadRoute route, void *param, bool scope);
bool CreateDefaultThread(pthread_t *tid, funcThreadRoute route, void *param);


extern int DeviceMode_g;


#define START_PROCESS( file, cmd, ... ) \
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

#if 0
#define START_PROCESS_CLOSE_FD( file, cmd, ... ) \
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
		printf("pid : %d\n", pid);\
		snprintf(path, sizeof(path), "/proc/%d/fd", pid);\
		printf("path: %s\n", path);\
		dir = opendir(path);\
		if(NULL == dir)\
		{\
			printf("open %s fail!\n", path);\
		}\
		else\
		{\
			while( ( entry = readdir(dir) ) != NULL )\
			{\
				printf("%s ", entry->d_name);\
				char path2[256];\
				char realpath[256];\
				snprintf(path2, sizeof(path2), "%s/%s", path, entry->d_name);\
				memset(realpath, 0, sizeof(realpath));\
				int ret = readlink(path2, realpath, sizeof(realpath)-1);\
				printf(" -> %s\n", realpath);\
				if( strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0 && \
					strcmp(entry->d_name, "0") != 0 && strcmp(entry->d_name, "1") != 0 && strcmp(entry->d_name, "2") != 0 )\
				{\
					if( strcmp(path, realpath) != 0 )\
					{\
						int fd = atoi(entry->d_name);\
						printf("close fd -> %d\n", fd);\
						close(fd);\
					}\
				}\
			}\
			printf("\n");\
			closedir(dir);\
		}\
		\
		execlp ( file, cmd, ##__VA_ARGS__ );\
		exit ( -1 );\
	}\
	waitpid ( pid, &status, 0 );\
}
#else
#define START_PROCESS_CLOSE_FD( file, cmd, ... ) \
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
#endif

#define CAP_START_PROCESS( x, cmd) \
{\
	if(0 == x)\
	{\
		x = 1;\
		cmd;\
	}\
}
#define CAP_STOP_PROCESS( x, cmd) \
{\
	if(1 == x)\
	{\
		x = 0;\
		cmd;\
	}\
}

int dg_save_file(const char *path, unsigned char *buffer, int size);
int dg_read_file(const char *path, unsigned char *buffer, int size);

int SaveLicenseToFlash(unsigned char *buffer, int size);
int ReadLicenseFromFlash(unsigned char *buffer, int size);

//ɾ���ļ�
void delete_file(const char *path);

void NormalRestart();
void AbnormalRestart();
void SystemReset();

bool CreateDetachedThread(char *threadName,funcThreadRoute route, void *param, bool scope);
bool CreateDefaultThread(pthread_t *tid, funcThreadRoute route, void *param);

#endif
