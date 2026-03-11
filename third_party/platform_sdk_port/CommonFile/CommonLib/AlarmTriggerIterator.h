
#ifndef __ALARM_TRIGGER_ITERATOR_HEAD__
#define __ALARM_TRIGGER_ITERATOR_HEAD__

#ifdef WIN32
#pragma warning (disable:4251)
#endif 

/************************************************************************/

/************************************************************************/
#include <list>
#include "NVMS_Define.h"
#include "TriggerManagerSDK.h"

/************************************************************************/
/*用来访问报警联动的信息                                                */
/************************************************************************/

class CTriggerNode;
class CTriggerNodeEx;

class _EXPORT_TRIGGERMANAGER_CLASS CAlarmTriggerIteratorBase
{
public:
	bool HasNext(void);
	CTriggerNode *GetNext(void);

protected:
	CAlarmTriggerIteratorBase(void);
	virtual ~CAlarmTriggerIteratorBase(void);

protected:
	std::list<CTriggerNodeEx *> m_NodeList;			//用来存储全部
	std::list<CTriggerNodeEx *>::iterator m_Iter;	//这个用来记录当前迭代位置
};

class _EXPORT_TRIGGERMANAGER_CLASS CAlarmTriggerIterator : public CAlarmTriggerIteratorBase
{
public:
	CAlarmTriggerIterator(void);					//所有报警联动
	CAlarmTriggerIterator(tuint32 dwNodeID);		//CTriggerNode节点ID
	CAlarmTriggerIterator(const GUID &NodeGUID);

	virtual ~CAlarmTriggerIterator(void) {}
};

//联动迭代器,迭代出指定被联动目标节点的报警联动信息
class _EXPORT_TRIGGERMANAGER_CLASS CTriggerIterator : public CAlarmTriggerIteratorBase
{
public:
	CTriggerIterator(const GUID &triggerNodeGUID);
	CTriggerIterator(const GUID &triggerNodeGUID, NVMS_DEFINE::ALARM_TRIGGER_TYPE TriggerType);

	virtual ~CTriggerIterator(void) {}
};

//报警迭代器,迭代出指定报警的报警联动信息
class _EXPORT_TRIGGERMANAGER_CLASS CAlarmIterator : public CAlarmTriggerIteratorBase
{
public:
	CAlarmIterator(const GUID &alarmNodeGUID);
	CAlarmIterator(const GUID &alarmNodeGUID, NVMS_DEFINE::ECMS_ALARM_TYPE AlarmType);
	CAlarmIterator(const GUID &alarmNodeGUID, NVMS_DEFINE::ECMS_ALARM_TYPE AlarmType, NVMS_DEFINE::ALARM_TRIGGER_TYPE TriggerType);
    CAlarmIterator(const GUID &alarmNodeGUID, NVMS_DEFINE::ECMS_ALARM_TYPE AlarmType, NVMS_DEFINE::ALARM_TRIGGER_TYPE TriggerType, bool quickSearch);
	CAlarmIterator(const GUID &alarmNodeGUID, NVMS_DEFINE::ECMS_ALARM_TYPE AlarmType, NVMS_DEFINE::ALARM_TRIGGER_TYPE TriggerType, const GUID &TriggerNodeGUID, const char* dwTrigerInfoEx);

	virtual ~CAlarmIterator(void) {}
};

//联动类型迭代器
class _EXPORT_TRIGGERMANAGER_CLASS CAlarmTriggerTypeIterator : public CAlarmTriggerIteratorBase
{
public:
	CAlarmTriggerTypeIterator(NVMS_DEFINE::ALARM_TRIGGER_TYPE TriggerType);
	//这个暂时只给存储，去订阅区域入侵联动抓图去使用。
	CAlarmTriggerTypeIterator(NVMS_DEFINE::ECMS_ALARM_TYPE AlarmType,NVMS_DEFINE::ALARM_TRIGGER_TYPE TriggerType);
	virtual ~CAlarmTriggerTypeIterator(void) {}
};
#endif //__ALARM_TRIGGER_ITERATOR_HEAD__