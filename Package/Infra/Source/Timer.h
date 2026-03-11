#ifndef __TIMER_HANDLE_H__
#define __TIMER_HANDLE_H__

//#include "Infra/Mutex.h"
#include "Infra/Types.h"
#include "Infra/Thread.h"
#include "MsgQue.h"
#include "Thread.h"


#ifndef MIN
#define MIN(a,b)				(((a) < (b)) ? (a) : (b))
#endif

class TimerValue
{
public:
	ulong lo;
	ulong hi;
	void operator+= (ulong a);
};

int compare(TimerValue& a, TimerValue& b);
bool operator> (TimerValue a, TimerValue b);


class CTimeHandle
{
	friend class CTimerManager;

public:
	
	typedef TFunction1<void, uint> Proc;
	
	CTimeHandle(const char * pName = "NoName");
	virtual ~CTimeHandle();	

	void Start(Proc pTimerFun, uint delayTime, uint period, uint param = 0, uint timeout = 0);
	void Stop(bool bCallNow = false);
	void run();
	const char * GetName();
	void SetName(const char *);
	bool IsStarted();
	bool IsCalled();

private:
	TimerValue m_CallTime;
	ulong m_Priod;
	ulong m_timeout;
	Proc m_pTimerFun;
	uint m_param;
	char  m_Name[32];
	bool  m_Started;
	bool  m_Called;
	static CMutex m_Mutex;
	CTimeHandle* m_pPrev;		//上一个定时器
	CTimeHandle* m_pNext;		//下一个定时器
	Threadlet m_threadlet;
};

class CTimerManager : public CThread
{
	friend class CTimeHandle;

public:
	PATTERN_SINGLETON_DECLARE(CTimerManager);
	void Start();
	bool AddTimer(CTimeHandle * pTimeHandle);
	bool RemoveTimer(CTimeHandle * pTimeHandle);
	void DumpTimers();
	void ThreadProc();

protected:
private:
	TimerValue m_CurTime;	//毫秒计数
	CTimeHandle* m_pHead;
};

#define g_TimerManager (*CTimerManager::instance())

#endif


