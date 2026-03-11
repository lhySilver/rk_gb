#ifndef __MANAGER_DAYLIGHTN_H__
#define __MANAGER_DAYLIGHTN_H__

#include "Manager/MagicBox.h"
#include "Infra/Timer.h"
#include "Infra/Singleton.h"

class CDaylight
{
	PATTERN_SINGLETON_DECLARE(CDaylight);
public:

	void start();

private:
	void onConfigLocation(const Json::Value& pConfig, int& ret);
	void onDSTTimer(uint par);
	int prepareTime(int year);
	void prepareDSTtime(SystemTime* pDesTime, GeneralDSTPoint* pDST, int year);

private:
	SystemTime m_LastTime;			//最后记录的时间
	SystemTime m_DecreasingTime;	//当年减一小时的时刻
	SystemTime m_IncreasingTime;	//当年加一小时的时刻
	SystemTime m_DecreasedTime;	//当年减一小时的后的时间
	SystemTime m_IncreasedTime;	//当年加一小时的后的时间
	CTimer m_DSTTimer;				//定时器
	int	m_iType;				//夏令时规则
	bool m_bTuned;				//夏令时标志
	bool m_bNorth;				//是否在北半球
	LocationConfig	m_CcfgLocation;
};

#endif
