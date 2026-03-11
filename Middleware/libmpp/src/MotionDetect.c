#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <time.h>
#include <getopt.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/sysinfo.h>
#include "PAL/MW_Common.h"

//平台库头文件
#include "log.h"
#include "video.h"

/// 创建动态检测设备
/// 
/// \param 无
/// \retval <0 创建失败
/// \retval 0 创建成功
int MotionDetectCreate(motion_detect_callback cb, int level, int bRegionEnable, unsigned int uiRegion[])
{
	LOG_INFO("MotionDetectCreate [level=%d]\n",level);
	// rkipc_ivs_init();
	rkipc_ivs_set_sen(level+1,cb);
	return 0;
}

/// 销毁动态检测设备
/// 
/// \param 无
/// \retval <0 销毁失败
/// \retval 0 销毁成功
int MotionDetectDestory(void)
{
	LOG_INFO("MotionDetectDestory\n");
	// rkipc_ivs_deinit();
	return 0;
}

int SetMotionDetect(int flag)
{
	return 0;
}


int MotionDetectGetState(unsigned int* pData)
{
	return 0;
}

int MotionDetectGetResult(int channel, MOTION_DETECT_RESULT *pResult)
{
	return 0;
}

int MotionDetectShowHint(int channel, int enable)
{
	return 0;
}

int MotionDetectSetParameter(int channel, MOTION_DETECT_PARAM *pParam)
{
	return 0;
}

int MotionDetectGetCaps(MOTION_DETECT_CAPS * pCaps)
{
	return 0;
}

