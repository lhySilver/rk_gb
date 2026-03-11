#ifndef __MANAGER_AUTOMAINTAIN_H__
#define __MANAGER_AUTOMAINTAIN_H__


#include "Infra/File.h"
#include "Infra/Timer.h"
#include "Infra/Function.h"
#include "ExchangeAL/ManagerExchange.h"
#include "Infra/Singleton.h"

//自动维护各项值说明：
enum auto_reboot_day 
{
	DAY_NERVER = 0,		//从不
	EVERYDAY,			//每天
	SUNDAY,			  //周日
	MONDAY,       //周一
	TUESDAY,      //周二
	WEDNESDAY,    //周三
	THURSDAY,     //周四
	FRIDAY,       //周五
	SATURDAY     //周六
};

enum auto_delete_file_time
{
	TIME_NERVER = 0,	  //从不
	ONE_DAY,      //24小时
	TWO_DAY,      //48小时
	THREE_DAY,    //72小时
	FOUR_DAY,     //96小时
	ONE_WEEK,     //一周
	ONE_MONTH		//一个月
};

enum maintain_type
{
	MAINTAIN_REBOOT,
	MAINTAIN_DELETE_FILES,

	MAINTAIN_TYPES,
};

//char * const AUTO_VERSION = "0308";		// zzb 2013.9.3 modify
const char * const AUTO_VERSION = "0308";

#define AUTO_CUR_VERSION ((uint)((uint)'A'<<24 | (uint)'T'<<16 | (uint)'O'<<8 | (uint)AUTO_VERSION))

typedef TFunction1<void, uint> MAINTAINPROC;

class CAutoMaintain
{
	PATTERN_SINGLETON_DECLARE(CAutoMaintain);

public:

	void Start();
	void OnConfigAutoMaintain(const Json::Value& table, int& ret);
private:
	void OnReboot(uint par);
	void OnTimer(uint par);
	bool IsMaintainNow(int iMaintainType);

protected:
private:
	MAINTAINPROC m_pfnMaintain[MAINTAIN_TYPES];
	SystemTime m_LastTime[MAINTAIN_TYPES];
	CTimer m_timerAuto;
	CFile m_fileAutoConfig;
	AutoMaintainConfig m_ConfigAutoMaintain;
	uint m_timeMsCount;
	CMutex m_mutexCout;
};

#define g_AutoMaintain (*CAutoMaintain::instance())

#endif
