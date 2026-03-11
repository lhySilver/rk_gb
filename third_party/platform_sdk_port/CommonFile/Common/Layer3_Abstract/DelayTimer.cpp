
#include "MySingleLock.h"
#include "DelayTimer.h"
#include "ShareSDK.h"
#include "PFTime.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CDelayTimer::CDelayTimer()
{
	m_Status = TIMER_NO_ACTION;
	m_ElapseTimeus = 0;
	m_Delayus = 0;
}

///////析构函数
CDelayTimer::~CDelayTimer()
{
}




/////////////定时  计时   开始计时  单位为毫秒
//void StartTiming(ULONGLONG ullTimeLength = -1);/////永不停止
bool CDelayTimer::StartTiming(bool isTimer)
{
	ASSERT(TIMER_NO_ACTION == m_Status || TIMER_TIMER_END == m_Status);	//当计时结束时,可以直接再StartTiming
/*	ASSERT(time > 0);*/
	
	CMySingleLock   LockAction(m_Lock);
	m_IsTimer=isTimer;
	if(m_IsTimer)
		m_ElapseTimeus = 0;	
	else
		m_FirstTime=ZERO_FILE_TIME;
	m_Status = TIMER_TIMER_ING;
	return true;
}

//开始倒计时
bool CDelayTimer::StartDelay(ULONGLONG delayus)
{
	ASSERT(TIMER_NO_ACTION == m_Status || TIMER_TIMER_END == m_Status);	//当计时结束时,可以直接再StartTiming
	CMySingleLock   LockAction(m_Lock);
	m_Delayus=delayus;
	m_Status = TIMER_TIMER_ING;
	return true;
}

void CDelayTimer::StopDelay()
{
	CMySingleLock   LockAction(m_Lock);
	m_Delayus=0;
	m_Status	= TIMER_NO_ACTION;
}

CDelayTimer::TIMER_STATUS CDelayTimer::Delaykeeper(ULONGLONG ullAbsoluteTimems,ULONGLONG &ulTimems)
{
	CMySingleLock   LockAction(m_Lock);
	if( TIMER_TIMER_ING == m_Status )
	{
		if(m_Delayus != ~((ULONGLONG)0))
		{
			if(m_Delayus > ullAbsoluteTimems)
				m_Delayus-= ullAbsoluteTimems;
			else
			{
				m_Delayus=0;
				m_Status=TIMER_TIMER_END;
			}
		}
	}
	else
	{
		ASSERT(m_Delayus == 0);
	}
	ulTimems=m_Delayus;
	return m_Status;
}

CDelayTimer::TIMER_STATUS CDelayTimer::StopTiming()
{
/*	ASSERT(TIMER_NO_ACTION != m_Status);*/
	CMySingleLock   LockAction(m_Lock);

	CDelayTimer::TIMER_STATUS  OldStatus = m_Status;
	m_Status	= TIMER_NO_ACTION;
	m_ElapseTimeus = 0;
	m_FirstTime=ZERO_FILE_TIME;
	return OldStatus;
}

CDelayTimer::TIMER_STATUS CDelayTimer::GetCurStatus()
{
	return m_Status;
}

CDelayTimer::TIMER_STATUS CDelayTimer::Timekeeper(const ULONGLONG &ullAbsoluteTimeus, \
												  ULONGLONG &ullElapseTimeus)
{
	CMySingleLock   LockAction(m_Lock);

	if( TIMER_TIMER_ING == m_Status )
	{
		m_ElapseTimeus += ullAbsoluteTimeus;

		ullElapseTimeus = m_ElapseTimeus;
	}

	return m_Status;
}
CDelayTimer::TIMER_STATUS CDelayTimer::Timekeeper(const FILETIME  &ftAbsoluteTime, \
												  ULONGLONG &ullElapseTimeus)
{
	CMySingleLock   LockAction(m_Lock);
	ASSERT(!m_IsTimer);

	if( TIMER_TIMER_ING == m_Status )
	{
		ASSERT (ftAbsoluteTime != ZERO_FILE_TIME);
		if(m_FirstTime == ZERO_FILE_TIME)
		{
			m_FirstTime=ftAbsoluteTime;
		}
		//ASSERT((ftAbsoluteTime > m_FirstTime) || (ftAbsoluteTime == m_FirstTime));
		if( ftAbsoluteTime < m_FirstTime) 
		{
			NORMAL_OUTPUT("设备时间错误 !");
			ullElapseTimeus = ~0;
		}
		else
		{
			ullElapseTimeus=GetElapseTimeus(m_FirstTime,ftAbsoluteTime);
		}
	}

	return m_Status;
}
//得到流逝的时间，微秒为单位
ULONGLONG CDelayTimer::GetElapseTimeus(const FILETIME &baseTime,const FILETIME &curTime)
{
	ULONGLONG ulbastTime=*((ULONGLONG*)&baseTime);
	ULONGLONG ulcurTime=*((ULONGLONG*)&curTime);
	ASSERT(ulcurTime >= ulbastTime);
	return ((ulcurTime-ulbastTime)/10);
}
