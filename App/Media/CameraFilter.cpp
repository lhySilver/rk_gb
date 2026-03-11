#include "Media/Camera.h"
#include "PAL/Camera.h"
#include "PAL/Capture.h"
#include "Manager/ConfigManager.h"

#include "../Manager/ManagerDefaultConfig.h"
#include "ExchangeAL/ExchangeKind.h"
#include "ExchangeAL/ManagerExchange.h"
#include "PAL/Misc.h"
#include "Common.h"
#include "Log/DebugDef.h"


#include "CameraFilter.h"

const int CHECK_FREQUENT_SWITCH_DURATION = 120; //检测频繁切换的时长，单位秒
const int FREQUENT_SWITCH_THRESHOLD = 6; 	    //定义为频繁切换的次数。
const int FORCE_RED_NIGHT_TIME = 30*60;         

bool bStartTiming = true; 	//开始计时
int begin_time = 0; 		//开始时间
int status_change_count = 0; 	//状态切换计数
int end_time = 0; 	//强制成夜视状态的结束时间
int curstatus = -1;

bool isSwitchFreq = false;   //Switch frequently


int CameraFilterStart()
{
	bStartTiming = true;
	begin_time = 0;
	end_time = 0;
	status_change_count = 0;
	curstatus = -1;
	isSwitchFreq = false;
	return 0;
}

bool CameraFilter(int status)
{
	if (bStartTiming)
	{
		if (-1 == curstatus)
		{
			curstatus = status;
			begin_time = time(0);
		}

		if (curstatus != status)
		{
			status_change_count++;
			curstatus = status;
		}

		if (time(0) > (begin_time + CHECK_FREQUENT_SWITCH_DURATION))
		{
			AppErr("check camera switch frequently timeout\n");
			begin_time = time(0);
			status_change_count = 0;
		}
		if (status_change_count > FREQUENT_SWITCH_THRESHOLD) /* 判断为频繁切换 */
		{
			AppErr("camera switch is frequently [%d]\n",FREQUENT_SWITCH_THRESHOLD);
			end_time = time(0) + FORCE_RED_NIGHT_TIME; 	//标记结束时间，FORCE_RED_NIGHT_TIME后要重启判断状态
			bStartTiming = false;
			status_change_count = 0;
			curstatus = -1;
			isSwitchFreq = true;
		}
	}
	else
	{
		if ( (end_time > 0 ) && (time(0) > end_time) )
		{
			AppErr("restart check camera switch frequently\n");
			end_time = 0;
			bStartTiming = true;
			isSwitchFreq = false;
		}
	}
	

	return isSwitchFreq;
}