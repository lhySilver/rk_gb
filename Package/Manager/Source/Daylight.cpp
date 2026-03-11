#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "Types/Defs.h"
#include "ExchangeAL/ManagerExchange.h"
#include "ExchangeAL/ExchangeKind.h"
#include "ExchangeAL/Exchange.h"
#include "Manager/ConfigManager.h"
#include "Infra/Time.h"
#include "Daylight.h"
#include <time.h>
#include "MagicBox.h"
#include "Log/DebugDef.h"
#include "PathDefinition.h"


PATTERN_SINGLETON_IMPLEMENT(CDaylight);

CDaylight::CDaylight() : m_DSTTimer("Daylight")
{
	memset(&m_LastTime, 0, sizeof(SystemTime)); //触发开机时定时器调用prepareTime()
}

CDaylight::~CDaylight()
{
	IConfigManager::instance()->detach(getConfigName(CFG_LOCATION), IConfigManager::Proc(&CDaylight::onConfigLocation, this));
}

void CDaylight::start()
{	
	
	return ;//ale
	ManagerInfo("CDaylight::start()...\n");
	int iRet = 0;
	Json::Value table;

	m_bTuned = CMagicBox::instance()->readFlag(CMagicBox::CMOS_DST_TUNE);
	IConfigManager::instance()->getConfig(getConfigName(CFG_LOCATION), table);
	//printJsonValue(table);
	//TExchangeAL<LocationConfig>::getConfig(table, m_CcfgLocation);
	//onConfigLocation(table, iRet);
	IConfigManager::instance()->attach(getConfigName(CFG_LOCATION), IConfigManager::Proc(&CDaylight::onConfigLocation, this));
}

void CDaylight::onConfigLocation(const Json::Value& table, int& ret)
{
	SystemTime stCurTime;

	//process time
	CTime::getCurrentTime(stCurTime);

	TExchangeAL<LocationConfig>::getConfig(table, m_CcfgLocation);
	prepareTime(stCurTime.year);
	if (m_CcfgLocation.iDSTRule != DST_RULE_OFF)
	{
		if (!m_DSTTimer.IsStarted())
		{
			m_DSTTimer.Start(CTimer::Proc(&CDaylight::onDSTTimer, this), 0, 1000 * 1);
		}
	}
	else
	{
		if(m_DSTTimer.IsStarted())
		{
			m_DSTTimer.Stop();
		
			if(m_bNorth)
			{
				if(m_bTuned == false)//恢复为正常时间
				{
					TimeAdd(&stCurTime, &stCurTime, -3600);
					CTime::setCurrentTime(stCurTime);
					m_bTuned = true;
					CMagicBox::instance()->writeFlag(CMagicBox::CMOS_DST_TUNE, m_bTuned);
				}
			}
			else
			{
				if(m_bTuned == true)
				{
					TimeAdd(&stCurTime, &stCurTime, 3600);
					CTime::setCurrentTime(stCurTime);
					// 修改系统时间时间，当前时间为修改后
					m_bTuned = false;
					CMagicBox::instance()->writeFlag(CMagicBox::CMOS_DST_TUNE, m_bTuned);
				}
			}
		}
	}	
}

//随时检测夏令时
void CDaylight::onDSTTimer(uint par)
{
	SystemTime stCurTime;
	bool flag;

	CTime::getCurrentTime(stCurTime);
	if(stCurTime.year != m_LastTime.year) //年变化， 重新准备转换时刻
	{
		if (prepareTime(stCurTime.year))
		{
			m_DSTTimer.Stop();
		}
	}
	//北半球:m_bTuned=true表示当前是非夏令时状态
	//南半球:m_bTuned=true表示当前是夏令时状态,两者恰好相反
	if(m_bTuned)
	{
		if(m_bNorth)
		{
			flag = TimeCompare(&stCurTime, &m_DecreasedTime) < 0
				&& TimeCompare(&stCurTime, &m_IncreasingTime) >= 0;
		}
		else
		{
			flag = TimeCompare(&stCurTime, &m_DecreasedTime) < 0
				|| TimeCompare(&stCurTime, &m_IncreasingTime) >= 0;
		}
		if(flag && m_bTuned) //调快一小时
		{
			ManagerDebug("DayLight Increasing time:%d-%d-%d %d:%d:%d!\n", stCurTime.year, stCurTime.month, stCurTime.day, stCurTime.hour, stCurTime.minute, stCurTime.second);
			TimeAdd(&stCurTime, &stCurTime, 3600);
			CTime::setCurrentTime(stCurTime);
			m_bTuned = false;
			CMagicBox::instance()->writeFlag(CMagicBox::CMOS_DST_TUNE, m_bTuned);
		}
	}
	else
	{
		if(m_bNorth)
		{
			flag = TimeCompare(&stCurTime, &m_IncreasedTime) < 0
				|| TimeCompare(&stCurTime, &m_DecreasingTime) >= 0;
		}
		else
		{
			flag = TimeCompare(&stCurTime, &m_IncreasedTime) < 0
				&& TimeCompare(&stCurTime, &m_DecreasingTime) >= 0;
		}
		if(flag && !m_bTuned) //调慢一小时
		{
			ManagerDebug("Daylight Decreasing time:%d-%d-%d %d:%d:%d!\n", stCurTime.year, stCurTime.month, stCurTime.day, stCurTime.hour, stCurTime.minute, stCurTime.second);
			TimeAdd(&stCurTime, &stCurTime, -3600);
			CTime::setCurrentTime(stCurTime);
			m_bTuned = true;
			CMagicBox::instance()->writeFlag(CMagicBox::CMOS_DST_TUNE, m_bTuned);
		}
	}

	m_LastTime = stCurTime;
}

void CDaylight::prepareDSTtime(SystemTime* pDesTime, GeneralDSTPoint* pDST, int year)
{
	memset(pDesTime, 0, sizeof(SystemTime));
	if (pDST->iWeek == 0) // 按天计算
	{
		//按天计算的年需要校验
		pDesTime->year = pDST->iYear;
		pDesTime->month = pDST->iMonth;
		pDesTime->day = pDST->iWeekDay;
	}
	else
	{
		SystemTime sysTime;
		memset(&sysTime, 0, sizeof(SystemTime));
		sysTime.year = year;
		sysTime.day = 1; //1号
		if (pDST->iWeek > 0)
		{
			//正数第几周,从第一天开始算
			sysTime.month = pDST->iMonth;
			*pDesTime = sysTime;
		}
		else
		{
			//从下月开始计算
			sysTime.month = pDST->iMonth + 1;
			TimeAdd(&sysTime,pDesTime, -1);
		}

		struct tm t;
		memset(&t, 0, sizeof(tm));
		t.tm_year = year - 1900;

		t.tm_mday = pDesTime->day;
		t.tm_mon = pDesTime->month - 1;
		mktime(&t);

		if (pDST-> iWeek> 0)
		{

			if (pDST->iWeekDay < t.tm_wday)
			{//计算上一周的
				pDesTime->day = t.tm_mday + pDST-> iWeek * 7 + (pDST->iWeekDay - t.tm_wday);
				//有可能无效
			}
			else
			{
				pDesTime->day = t.tm_mday + (pDST-> iWeek - 1) * 7 + (pDST->iWeekDay - t.tm_wday);
			}
		}
		else
		{
			//最后一周
			//转到前一天
			if (pDST->iWeekDay <= t.tm_wday)
			{
				pDesTime->day = t.tm_mday + (pDST-> iWeek + 1) * 7 - (t.tm_wday - pDST->iWeekDay);
			}
			else
			{//计算下一周的
				pDesTime->day = t.tm_mday + pDST-> iWeek * 7 + (pDST->iWeekDay - t.tm_wday);
			}
		}
	}
	pDesTime->hour = pDST->Hour;
	pDesTime->minute = pDST->Minute;
	pDesTime->second = 0;// 用周计算时会改变为59,所以在此重新设置
	//trace("++++++++preparedsttime:month:%d old:week:%d weekday:%d new:day:%d hour:%d min:%d\n",
		//pDST->iMonth, pDST->iWeek, pDST->iWeekDay, pDesTime->day,pDesTime->hour, pDesTime->minute);
}

//准备好当年的转换时刻
int CDaylight::prepareTime(int year)
{
	int iRet = 1;
	
	LocationConfig cfglocation = m_CcfgLocation;
	prepareDSTtime(&m_IncreasingTime, &cfglocation.dDSTStart, year);
	prepareDSTtime(&m_DecreasingTime, &cfglocation.dDSTEnd, year);

	m_bNorth = (TimeCompare(&m_IncreasingTime, &m_DecreasingTime) < 0);
	//按天表示，判断有效期	
	TimeAdd(&m_IncreasingTime, &m_IncreasedTime, 3600);
	TimeAdd(&m_DecreasingTime, &m_DecreasedTime, -3600);
	if ((m_IncreasingTime.year <= year) && (m_DecreasingTime.year >= year))
	{
		iRet = 0;
	}
	return iRet;
}
