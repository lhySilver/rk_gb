#include <stdio.h>
#include <string.h>
#include <sys/times.h>
#include "Infra/Timer.h"
#include "Timer.h"
#include <unistd.h>
CMutex CTimeHandle::m_Mutex(MUTEX_RECURSIVE);

void TimerValue::operator+= (ulong  a)
{
	lo += a;
	if(lo < a) //进位
	{
		hi++;
	}
}

int compare(TimerValue& a, TimerValue& b)
{
	if(a.hi < b.hi)
	{
		return -1;
	}
	else if(a.hi > b.hi)
	{
		return 1;
	}
	else
	{
		if(a.lo < b.lo)
		{
			return -1;
		}
		else if(a.lo > b.lo)
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}
}

bool operator> (TimerValue a, TimerValue b)
{
	return (compare(a, b) > 0);
}

bool operator< (TimerValue a, TimerValue b)
{
	return (compare(a, b) < 0);
}

CTimer::CTimer(const char * pName)
{
	m_timerHandle = new CTimeHandle(pName);
}

CTimer::~CTimer()
{
	delete static_cast<CTimeHandle *>(m_timerHandle);
}

void CTimer::Start(Proc pProc, uint delayTime, uint period, uint param /* = 0 */, uint timeout /* = 0 */)
{
	static_cast<CTimeHandle *>(m_timerHandle)->Start(pProc, delayTime, period, param, timeout );
}

void CTimer::Stop(bool bCallNow /* = FALSE */)
{
	static_cast<CTimeHandle *>(m_timerHandle)->Stop(bCallNow);
}

const char * CTimer::GetName()
{
	return static_cast<CTimeHandle *>(m_timerHandle)->GetName();
}

void CTimer::SetName(const char * pszName)
{
	static_cast<CTimeHandle *>(m_timerHandle)->SetName(pszName);
}

bool CTimer::IsStarted()
{
	return static_cast<CTimeHandle *>(m_timerHandle)->IsStarted();
}

bool CTimer::IsCalled()
{
	return static_cast<CTimeHandle *>(m_timerHandle)->IsCalled();
}

//static void DumpThread();
void CTimer::DumpThread()		// zzb 2013.8.20 add
{
    g_TimerManager.DumpTimers();
}

CTimeHandle::CTimeHandle(const char * pName)
{
	SetName(pName);
	m_Started = false;
	m_Called = false;
}

CTimeHandle::~CTimeHandle()
{
	CGuard guard(m_Mutex);

	if(m_Started)
	{
		g_TimerManager.RemoveTimer(this);
	}
}

void CTimeHandle::Start(Proc pProc, uint delayTime, uint period, uint param /* = 0 */, uint timeout /* = 0 */)
{
	CGuard guard(m_Mutex);
	m_pTimerFun = pProc;
	m_CallTime = g_TimerManager.m_CurTime;
	m_CallTime += delayTime;
	m_Priod = period;
	m_timeout = timeout;
	m_param = param;
	m_Called = false;
	if(m_Started)
	{
		g_TimerManager.RemoveTimer(this);
	}
	g_TimerManager.AddTimer(this);
	m_Started = true;
}

void CTimeHandle::Stop(bool bCallNow /* = FALSE */)
{
	CGuard guard(m_Mutex);
	if(!m_Started)
	{
		return;
	}

	//结束时立即调用一次回调函数，在需要提前结束非周期定时器时有用
	if(bCallNow && m_Priod == 0)
	{
		m_pTimerFun(m_param);
	}
	g_TimerManager.RemoveTimer(this);
	m_Started = false;
}

void CTimeHandle::run()
{
	g_TimerManager.RemoveTimer(this);
	if(m_Priod)
	{
		m_CallTime += m_Priod;
		g_TimerManager.AddTimer(this);
	}
	else
	{
		m_Started = false;
		m_Called = true;
	}
	m_threadlet.run(m_Name, m_pTimerFun, m_param, m_timeout);
}

const char * CTimeHandle::GetName()
{
	return m_Name;
}

void CTimeHandle::SetName(const char * pszName)
{
	int len = MIN(strlen(pszName), 31);
	memcpy(m_Name, pszName, len);
	m_Name[len]='\0';
}

bool CTimeHandle::IsStarted()
{
	return m_Started;
}

bool CTimeHandle::IsCalled()
{
	return m_Called;
}

////////////////////////////////////////////////////////////////////////////////////
// CTimerManager
////////////////////////////////////////////////////////////////////////////////////

PATTERN_SINGLETON_IMPLEMENT(CTimerManager);

CTimerManager::CTimerManager():CThread("TimerManager", TP_TIMER)
{
	m_pHead = NULL;
	//<shang>
#if 0
	m_CurTime.lo = times(NULL)*10;
#else
	m_CurTime.lo = 0;
#endif
	m_CurTime.hi = 0;
//	trace("CTimerManager::CTimerManager()>>>>>>>>>\n");
}

CTimerManager::~CTimerManager()
{
	DestroyThread();
}

void CTimerManager::Start()
{
	CreateThread();
}

bool CTimerManager::AddTimer(CTimeHandle* pTimeHandle)
{
	CTimeHandle *pp,*pn;

	pp = NULL;
	pn = m_pHead;

	while(pn && (pTimeHandle->m_CallTime > pn->m_CallTime)) // 按m_CallTime从小到大排序
	{
		pp = pn;
		pn = pn->m_pNext;
	}

	if(pp)
	{
		pp->m_pNext = pTimeHandle;
	}
	else
	{
		m_pHead = pTimeHandle;
	}
	pTimeHandle->m_pPrev = pp;

	if(pn)
	{
		pn->m_pPrev = pTimeHandle;
	}
	pTimeHandle->m_pNext = pn;
	
	return true;
}

bool CTimerManager::RemoveTimer(CTimeHandle * pTimeHandle)
{
	if(pTimeHandle->m_pPrev == NULL)
	{
		m_pHead = pTimeHandle->m_pNext;
		if(m_pHead)
		{
			m_pHead->m_pPrev = NULL;
		}

		return true;
	}

	pTimeHandle->m_pPrev->m_pNext = pTimeHandle->m_pNext;
	if(pTimeHandle->m_pNext != NULL)
		pTimeHandle->m_pNext->m_pPrev = pTimeHandle->m_pPrev;

	return true;
}

void CTimerManager::DumpTimers()
{
	CTimeHandle * p = m_pHead;

	CGuard guard(CTimeHandle::m_Mutex);
//	trace("Timers: ( %u Milli-Seconds Elapsed )\n", m_CurTime.lo);
	trace("_________________________________________\n");
	while (p)
	{
		trace("%16s %lu %s\n", p->GetName(), p->m_CallTime.lo, p->m_threadlet.isRunning() ? "Running" : "Idel");
		p = p->m_pNext;
	}
}

uint SystemGetMSCount(void)		// zzb 2013.9.24 add: from wdx
{
	//<shang>
#if 0
	uint t;
	
	t = (times(NULL) & 0xffffffff) * 10;

	return	t;
	//return	times(NULL) * 10;
#else
	static uint vir_t = 0;
	static uint last_t = 0;
	uint t;
	
//	t = times(NULL);
	t++;
	if( last_t != t )
	{
		last_t = t;
		vir_t += 10;
	}

	return vir_t;
#endif
} /* end SystemGetMSCount */

void CTimerManager::ThreadProc()
{
	do 
	{
		usleep(10000);
    //    SystemSleep(1);					// zzb 2013.9.24 test
		CGuard guard(CTimeHandle::m_Mutex); 

		uint OldTime = m_CurTime.lo;
//		m_CurTime.lo = SystemGetMSCount();//times(NULL) * 10;	// zzb 2013.9.24 test
		m_CurTime.lo += 10;

		// 计时没有改变，可能是因为计时器精度不高
		if(m_CurTime.lo == OldTime)
		{
			continue;
		}

		if(m_CurTime.lo < OldTime)
		{
			trace("CTimerManager::ThreadProc() MSCount Overflowed, %lu < %u .\n", m_CurTime.lo, OldTime);
			if(m_CurTime.lo < 10000) //判定为溢出
			{
				m_CurTime.hi++;
			}
		}
		while(m_pHead && m_pHead->m_CallTime < m_CurTime)
		{
			m_pHead->run();
		}
	} while(m_bLoop);
}
