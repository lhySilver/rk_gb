#ifndef __INTER_MANAGER_EVENT_H__
#define __INTER_MANAGER_EVENT_H__

#include "Manager/EventManager.h"
#include "Infra/Guard.h"
#include "Infra/Timer.h"
#include "Infra/PoolAllocator.h"
#include "Infra/Singleton.h"
#include "Infra/Signals.h"
#include "ExchangeAL/CommExchange.h"
#include "Manager/ConfigManager.h"
#include <map>
#include <string>

#define MAX_EVENT_LATCH_NUMBER 4


typedef struct tagEVENT_LATCH
{
	void*	handler;			// 需要进行事件闩锁的处理者
	IEventManager::Proc proc;	// 回调函数
	int		second;				// 事件闩锁的时间
}EVENT_LATCH;

typedef struct tagEVENT_ACTIVE
{
	bool			inSection;			    // 在时间段之内
	bool			inStopping;			    // 正在停止，因为延时的存在，所以没有删除
	const EventHandler*	parameter;			// 事件处理参数
	const WorkSheet*	workSheet;			// 事件处理工作表
	EVENT_LATCH		latch[MAX_EVENT_LATCH_NUMBER]; // 事件闩锁处理
	int				latchNumber;		    // 事件闩锁个数
}EVENT_ACTIVE;

class CEventManager : public IEventManager
{
	PATTERN_SINGLETON_DECLARE(CEventManager);

public:

	bool attach(Proc proc);
	bool detach(Proc proc);
	bool checkTimeSection(const WorkSheet*workSheet);
	void notify(std::string code, int index = 0, appEventAction action = appEventStart, const EventHandler *param = NULL, const WorkSheet *workSheet = NULL, Json::Value* data = NULL);

	void getNextEvent();
	bool getNextEvent(void* handler, EVENT_INFO* info);
	void latchEvent(void *handler, IEventManager::Proc proc, std::string code, int index, int second);

private:
	typedef std::pair<std::string, int> EventPair;

	struct Less
	{
		inline bool operator()(const EventPair& x, const EventPair& y) const
		{
			if(x.first == y.first)
			{
				return (x.second < y.second);
			}
			return (x.first < y.first);
		}
	};

	typedef std::map<EventPair, EVENT_ACTIVE, Less, pool_allocator<EVENT_ACTIVE> > MAP_EVENT;

	void onTimer(uint arg);
	void checkTimeSectionAll();
	int onConsole(int argc, char **argv);

private:
	
	SIG_EVENT m_sigEventHandler;
	MAP_EVENT m_mapEventActive;
	CMutex m_mutex;
	CTimer m_timer;
	SystemTime m_oldTime;
	bool	m_latchAllowed;		// 是否允许延时处理：配置修改或者定时时间段到，不允许延时；事件结束，允许延时
	MAP_EVENT::iterator m_pi;	// 事件遍历迭代器
};

#endif
