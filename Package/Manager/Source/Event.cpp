#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "Event.h"
#include "Log/Log.h"
#include "Infra/Time.h"
#include "Log/DebugDef.h"

PATTERN_SINGLETON_IMPLEMENT(CEventManager);

static const char *s_actionDispMap[256] = {"EventStart", "EventStop", "EventConfig", "EventLatch"};

CEventManager::CEventManager() : m_sigEventHandler(32), m_mutex(MUTEX_RECURSIVE), 
m_timer("EventManager"), m_latchAllowed(false)
{
	m_timer.Start(CTimer::Proc(&CEventManager::onTimer, this), 0, 1000);
	CTime::getCurrentTime(m_oldTime);
}

CEventManager::~CEventManager()
{
}

// 注册时间处理者
bool CEventManager::attach(IEventManager::Proc proc)
{
	return (m_sigEventHandler.Attach(proc) >= 0);
}

// 注销时间处理者
bool CEventManager::detach(IEventManager::Proc proc)
{
	return (m_sigEventHandler.Detach(proc) >= 0);
}

// 时间段检查和事件停止计数定时器
void CEventManager::onTimer(uint arg)
{
	CGuard guard(m_mutex);
	SystemTime curTime;
	MAP_EVENT::iterator pi, temp;
	int i;
	EventPair unicode;
	const EventHandler* param = NULL;
	EVENT_LATCH latchTemp;
	bool eraseEvent;

	CTime::getCurrentTime(curTime);

	// 检查事件的延时时间
	for(pi = m_mapEventActive.begin(); pi != m_mapEventActive.end();)
	{
		eraseEvent = false;
		for(i = 0; i < MAX_EVENT_LATCH_NUMBER; i++)         // 4
		{
			EVENT_LATCH & latch = (*pi).second.latch[i];
			if(latch.handler)
			{
				latch.second--;
				if(latch.second == 0) // 延时时间到
				{
					bool showLog = true;	// 记录日志

					unicode = (*pi).first;
					param = (*pi).second.parameter;
					latchTemp = latch;
					latch.handler = NULL;	// 删除该处理者
					(*pi).second.latchNumber--;
					showLog = (param && param->bLog) ? true : false;
					if((*pi).second.latchNumber == 0)
					{
						if((*pi).second.inStopping)
						{
							eraseEvent = true;
							temp = pi;
							temp++;
							m_mapEventActive.erase(pi); // 删除该事件
							showLog = false;
						}
					}

					// map更新后再通知事件处理者
					m_mutex.Leave();
					if (showLog)
					{
//<shang>						ILog::instance()->append("EventLatch", "%s,%d", (*pi).first.first.c_str(), unicode.second + 1);
					}
					latchTemp.proc(unicode.first, unicode.second, appEventLatch, param, NULL);
					m_mutex.Enter();
				}
				if(eraseEvent)
				{
					break;
				}
			}
		}
		if(eraseEvent)
		{
			pi = temp;
		}
		else
		{
			pi++;
		}
	}

	// 每分钟检查一次时间段
	if(m_oldTime.year != curTime.year
		|| m_oldTime.month != curTime.month
		|| m_oldTime.day != curTime.day
		|| m_oldTime.hour != curTime.hour
		|| m_oldTime.minute != curTime.minute)
	{
		checkTimeSectionAll();
	}
	m_oldTime = curTime;
}

// 更新是否在时间段之内的标志（并不真正删除或添加事件），并根据需要通知处理者
// 只在内部使用，否则锁使用出错
void CEventManager::checkTimeSectionAll()
{
	MAP_EVENT::iterator pi;

	for(pi = m_mapEventActive.begin(); pi != m_mapEventActive.end(); pi++)
	{
		if ((!(*pi).second.parameter) || (!(*pi).second.workSheet))
		{
			continue;
		}

		if(checkTimeSection((*pi).second.workSheet))
		{
			if(!(*pi).second.inSection)
			{
				(*pi).second.inSection = true; // must be here, for getTopEvent() from m_sigEventHandler() will use it
				m_mutex.Leave();
				// 记录日志
				if ((*pi).second.parameter && (*pi).second.parameter->bLog)
				{
//<shang>					ILog::instance()->append("EventStart", "%s,%d", (*pi).first.first.c_str(), (*pi).first.second + 1);
				}
				m_sigEventHandler((*pi).first.first, (*pi).first.second, appEventStart, (*pi).second.parameter, NULL);
				m_mutex.Enter();
			}
		}
		else
		{
			if((*pi).second.inSection)
			{
				m_latchAllowed = false;         // 立即触发延时结束动作（直接调用）
				(*pi).second.inSection = false; // must be here, for getTopEvent() from m_sigEventHandler() will use it
				m_mutex.Leave();
				if ((*pi).second.parameter && (*pi).second.parameter->bLog)
				{
//<shang>					ILog::instance()->append("EventStop", "%s,%d", (*pi).first.first.c_str(), (*pi).first.second + 1);
				}
				m_sigEventHandler((*pi).first.first, (*pi).first.second, appEventStop, (*pi).second.parameter, NULL);
				m_mutex.Enter();

				//清空事件的延时处理
				(*pi).second.latchNumber = 0;
				memset((*pi).second.latch, 0, sizeof(EVENT_LATCH) * MAX_EVENT_LATCH_NUMBER);
			}
		}
	}
}

bool CEventManager::checkTimeSection(const WorkSheet* workSheet)
{
	CGuard guard(m_mutex);

	int n;
	int current, start, end;
	int week;
	SystemTime systime;
	const TimeSection* timesect;

	assert(workSheet);

	// 时间段比较
	CTime::getCurrentTime(systime);
	week = systime.wday % 7;
	timesect = workSheet->tsSchedule[week];
	current = (systime.hour * 60 + systime.minute) * 60 + systime.second;

	for (n = 0; n < N_TSECT; n++)
	{
		if(!timesect[n].enable) //未使能
		{
			continue;
		}
		start = (timesect[n].startHour * 60 + timesect[n].startMinute) * 60 + timesect[n].startSecond;
		end = (timesect[n].endHour * 60 + timesect[n].endMinute) * 60 + timesect[n].endSecond;

		//只找选中的时间段
		if (current >= start && current < end)
		{
			break;
		}
	}

	if(n == N_TSECT) //时间段不符合
	{
		return false;
	}

	return true;
}

// 处理事件，调用者应该保证param内容在事件活动期间一直有效，事件处理中心会用到这个值
// 对于appEventConfig动作，这里会根据时间段和当前事件的状态转换为appEventStart或appEventStop后转发
// 对于已经start的事件再次被start时，事件的处理者需要检查其联动参数是否变化。
void CEventManager::notify(
						   std::string code,
							  int index /* = 0 */,
							  appEventAction action /* = appEventStart */,
							  const EventHandler *param /* = NULL */,
							  const WorkSheet *workSheet /* = NULL */,
							  Json::Value* data /* = NULL */)
{
	m_mutex.Enter();

	EventPair unicode(code,index); 
	EVENT_ACTIVE ea;
	MAP_EVENT::iterator pi;

	if(action == appEventStart)
	{
		memset(&ea, 0, sizeof(EVENT_ACTIVE));
		ea.parameter = param;
		ea.workSheet = workSheet;
		ea.inStopping = false;
		if(!param || !workSheet || checkTimeSection(workSheet))
		{
			ea.inSection = true;
		}
		else
		{
			ea.inSection = false;
		}
		if((pi = m_mapEventActive.find(unicode)) != m_mapEventActive.end())
		{
			if(ea.inSection)
			{
				(*pi).second = ea; // 覆盖老的参数，包括延时处理
			}
			else
			{
				(*pi).second.parameter = param;     // 覆盖老的参数，不包括延时处理
				(*pi).second.workSheet = workSheet; // 覆盖老的参数，不包括延时处理
				(*pi).second.inStopping = false;
				(*pi).second.inSection = false;
			}
		}
		else
		{
			m_mapEventActive.insert(MAP_EVENT::value_type(unicode, ea));
		}
		if(ea.inSection)
		{
			m_mutex.Leave();
			if (param && param->bLog)
			{
//<shang>				ILog::instance()->append(s_actionDispMap[action],  "%s,%d", code.c_str(), index + 1);
			}
			m_sigEventHandler(code, index, action, param, data);
			return;
		}
	}
	else if(action == appEventStop)
	{
		if((pi = m_mapEventActive.find(unicode)) != m_mapEventActive.end())
		{
			(*pi).second.inStopping = true;
		}
		else
		{
			m_mutex.Leave();
			return;
		}
		m_latchAllowed = true;

		m_mutex.Leave();
		if ((*pi).second.inSection)
		{
			// 记录日志
			if (param && param->bLog)
			{
//<shang>				ILog::instance()->append(s_actionDispMap[action], "%s,%d", code.c_str(), index + 1);
			}
			m_sigEventHandler(code, index, action, param, data);
		}
		m_mutex.Enter();
		
		// 这里删除的可能是刚添加的事件，未有效处理
		if((pi = m_mapEventActive.find(unicode)) != m_mapEventActive.end())
		{
			if((*pi).second.latchNumber == 0) // 不用延时，直接删除
			{
				m_mapEventActive.erase(pi);
			}
		}
	}
	else if(action == appEventConfig)
	{
		memset(&ea, 0, sizeof(EVENT_ACTIVE));
		ea.parameter = param;
		ea.workSheet = workSheet;
		if(param == NULL || workSheet == NULL)
		{
			m_mutex.Leave();
			return;
		}
		ea.inSection = checkTimeSection(workSheet);

		if((pi = m_mapEventActive.find(unicode)) != m_mapEventActive.end())
		{
			if (!ea.inSection)
			{
				m_latchAllowed = false;
				
				m_mutex.Leave();

				// 记录日志
				if (param && param->bLog)
				{
//<shang>					ILog::instance()->append("EventStop", "%s,%d", code.c_str(), index + 1);
				}
				m_sigEventHandler(code, index, appEventStop, param, data);
				return;
			}
			else
			{
				(*pi).second.parameter = param;     // 覆盖老的参数，不包括延时处理
				(*pi).second.workSheet = workSheet; // 覆盖老的参数，不包括延时处理
				(*pi).second.inStopping = false;
				(*pi).second.inSection = true;

				m_mutex.Leave();
				// 记录日志
				if (param && param->bLog)
				{
//<shang>					ILog::instance()->append("EventStart", "%s,%d", code.c_str(), index + 1);
				}
				m_sigEventHandler(code, index, appEventStart, param, data);
				return;
			}
		}
	}
	else if (action == appEventPulse)
	{
		// 脉冲事件，直接回调
		m_sigEventHandler(code, index, appEventPulse, param, data);
	}
	else
	{
		ManagerWraning("CEventManager::notify invalid event code: %s\n", code.c_str());
	}
	m_mutex.Leave();
}

// 事件遍历初始化，只能在事件处理函数里面调用。
void CEventManager::getNextEvent()
{
	CGuard guard(m_mutex);
	m_pi = m_mapEventActive.begin();
}

// 得到下一个事件，只能在事件处理函数里面调用，且只能在getNextEvent()之后调用。
bool CEventManager::getNextEvent(void* handler, EVENT_INFO* info)
{
	CGuard guard(m_mutex);
	int i;

	for(; m_pi != m_mapEventActive.end(); m_pi++)
	{
		if(!(*m_pi).second.inSection || (*m_pi).second.inStopping)
		{
			for(i = 0; i < MAX_EVENT_LATCH_NUMBER; i++)
			{
				if(handler == (*m_pi).second.latch[i].handler)
				{
					break;
				}
			}
			if(i == MAX_EVENT_LATCH_NUMBER)
			{
				continue;
			}
		}

		//如果在时间段之内或者该处理者发起了延时，调用回调函数检查参数
		info->code = (*m_pi).first.first;
		info->index = (*m_pi).first.second;
		info->parameter = (*m_pi).second.parameter;
		m_pi++;
		return true;
	}

	return false;
}

// 如果需要对事件结束进行延时，需要在响应结束事件时调用这个函数。
void CEventManager::latchEvent(void *handler, IEventManager::Proc proc, std::string code, int index, int second)
{
	CGuard guard(m_mutex);
	EventPair unicode(code, index);
	MAP_EVENT::iterator pi;
	int i;

	// 事件存在且在时间段之内
	if((pi = m_mapEventActive.find(unicode)) != m_mapEventActive.end())
	{
		//如果不允许延时，直接调用回调函数
		if(!m_latchAllowed)
		{
			proc(code, index, appEventLatch, (*pi).second.parameter, NULL);
			return;
		}

		for(i = 0; i < MAX_EVENT_LATCH_NUMBER; i++)
		{
			EVENT_LATCH & latch = (*pi).second.latch[i];

			if(latch.proc == proc) // 延时已经开启，不处理
			{
				return;
			}
		}
		for(i = 0; i < MAX_EVENT_LATCH_NUMBER; i++)
		{
			EVENT_LATCH & latch = (*pi).second.latch[i];

			if(!latch.handler)
			{
				latch.handler = handler;
				latch.proc = proc;
				latch.second = second;
				(*pi).second.latchNumber++;
				break;
			}
		}
		if(i == MAX_EVENT_LATCH_NUMBER)
		{
			ManagerErr("CEventManager::latchEvent failed, max latches reached.\n");
		}
	}
}

int CEventManager::onConsole(int argc, char **argv)
{
	CGuard guard(m_mutex);
	MAP_EVENT::iterator pi;
	int i;

	trace("Active Application Events:\n");
	trace("Name                        Index InSection InStopping Latches\n");
	for(pi = m_mapEventActive.begin(); pi != m_mapEventActive.end(); pi++)
	{
		trace("%-26s     %2d      %d        %d      [",	
			(*pi).first.first.c_str(), (*pi).first.second,	(*pi).second.inSection,	(*pi).second.inStopping);
		for(i = 0; i < MAX_EVENT_LATCH_NUMBER; i++)
		{
			trace(" %d ", (*pi).second.latch[i].second);
		}
		trace("]\n");
	}
	return 0;
}

IEventManager* IEventManager::instance()
{
	return CEventManager::instance();
}

IEventManager::~IEventManager()
{
}
