#ifndef __EVENTQUE_H__
#define __EVENTQUE_H__

#include <list>
#include <map>
#include <queue>
#include <vector>
#include "PAL/System.h"
#include "Infra/Mutex.h"
#include "Infra/Semaphore.h"
#include "Infra/PoolAllocator.h"
#include "Infra/Signals.h"
#include "Infra/Time.h"
#include "Types/Types.h"
#include "TPBase/TPSocket/TPSocket.h"

enum EVENT_TYPE
{
	EVENT_GERNERL = 0,
	EVENT_NET = 1,
	EVENT_TIMER = 2,
	EVENT_NUM,
};

typedef TSignal4<void *, void *, uint, uint>::SigProc CB_EVENT;

typedef struct tagEvent 
{
	uint iMsgId;
	void *cbarg;
	CB_EVENT callback;
	uint  dwArg1;
	uint  dwArg2;
}Event;

typedef std::list<Event, pool_allocator<Event> > EventQueue;

class CEventQue
{
public:
	CEventQue();
	virtual ~CEventQue();

	bool Init(EVENT_TYPE eType = EVENT_GERNERL, int iPushFd = 0, int iPopFd = 0, int iInterval = 0);
	bool pushEvent(const Event &cEvent);
	bool popEvent(Event &cEvent);
	bool dealEvent();
	void clearEvent();
	int getLength();
	int getPushFd() const { return m_iPushFd; }
	int getPopFd() const {return m_iPopFd; }
	int getType() const {return m_iType; }
	void updateRunTime() { m_uiRunTime = m_uiInterval + CTime::getCurrentMicroSecond(); }
	int getRunTime() const {return m_uiRunTime; }
protected:
private:
	EventQueue m_Queue;
	CMutex m_Mutex;		// 事件操作的互斥锁
	Event m_timerEvent; // 定时事件
	Event m_netEvent;	// 网络事件
	int m_iPushFd;
	int m_iPopFd;
	int m_iSize;		//list.size()效率比较低，故自己计数
	int m_iType;
	uint m_uiRunTime;	//ms为单位
	uint m_uiInterval;	//ms为单位
};

struct cmp
{
	bool operator()(const CEventQue *pQueA, const CEventQue *pQueB)
	{
		return pQueA->getRunTime() < pQueB->getRunTime();
	}
};

typedef std::pair<int, int> QUEUEID;
typedef std::map<QUEUEID, CEventQue*> QueueList;
typedef std::priority_queue<CEventQue*, std::vector<CEventQue*>, cmp> PQueue;

#define	EV_TIMEOUT	0x01
#define EV_READ		0x02
#define EV_WRITE	0x04
#define EV_SIGNAL	0x08
#define EV_PERSIST	0x10

enum _EVENT_
{
	ADD_EVENT = 0,
	DEL_EVENT = 1,
};

typedef struct tagEventParam 
{
	int iFd;
	int iMode;
	CEventQue *pEventQue;
	CB_EVENT callback;
	uint  dwArg1;
	uint  dwArg2;
}EventParam;

class CEventManage
{
public:
	CEventManage();
	virtual ~CEventManage();

	bool Init();
	bool delEvent(int iFd, int iMode, uint dwArg1, uint dwArg2, CEventQue *pQue, CB_EVENT callback = 0);
	bool addEvent(int iFd, int iMode, uint dwArg1, uint dwArg2, CEventQue *pQue, CB_EVENT callback = 0);
	bool clearAllEvent();
	void dispatchEvent(int iTimeOut = 3000);	// 默认超时为3s
	void loopEvent();
	void destroyEvent();

	void onDump(int argc, char **argv);
private:	
	bool delEvent(QUEUEID QueID, CEventQue *pQue);
	bool addEvent(QUEUEID QueID, CEventQue *pQue);
	bool updateEventList();

	void onDemon(void *pArg1, void *pArg2, uint nArg3, uint nArg4);
protected:
private:
	QueueList m_QueList;	/// 所有event queue
	QueueList m_DelList;	/// 所有删除 queue
	QueueList m_AddList;	/// 所有新增 queue
	PQueue m_PrioQueue;		/// timer event 处理对列
	CEventQue m_demonQue;	/// 为了减少同步操作,将CEventQue的增加和删除,使用event为事件
	bool m_bLoop;
	fd_set	m_ReadSetIn;
	fd_set	m_WriteSetIn;
	fd_set	m_ReadSetOut;
	fd_set	m_WriteSetOut;
	int m_iMaxFd;
	ulong m_uiRunTime;
};
#endif// __NETDECODER_EVENTQUE_H__

