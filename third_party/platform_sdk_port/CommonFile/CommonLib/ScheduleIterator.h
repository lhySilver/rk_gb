
#ifndef __SCHEDULE_ITERATOR__
#define __SCHEDULE_ITERATOR__

#ifdef WIN32
#pragma warning (disable:4251)
#endif 

#include "ScheduleSDKAPI.h"
#include "ConfigPack_Struct_Def.h"
#include <list>

class _EXPORT_SCHEDULE_CLASS CScheduleNode
{
public:
	virtual tuint32 GetNodeID(void) const = 0;
	virtual const GUID &GetNodeGUID(void) const = 0;
	virtual bool GetNodeName(char *szName, int nSize) const = 0;

	virtual bool GetSchTimeSection(std::list<CFGPACK_DEFINE::RES_SCHEDULE_TIME_SECTION_INFO> &TimeSectionList) = 0;

protected:
	CScheduleNode(void) {}
	virtual ~CScheduleNode(void) {}
};


class _EXPORT_SCHEDULE_CLASS CTimeSecRateNode
{
public:
	virtual tuint32 GetNodeID(void) const = 0;
	virtual const GUID &GetNodeGUID(void) const = 0;
	virtual bool GetNodeName(char *szName, int nSize) const = 0;
	virtual bool GetNodeAppendInfo(char *szBuf, int bufSize) const = 0;

protected:
	CTimeSecRateNode() {}
	virtual ~CTimeSecRateNode(void) {}
};

class CScheduleNodeEx;
class _EXPORT_SCHEDULE_CLASS CSchItemIterator
{
public:
	CSchItemIterator(void);
	explicit CSchItemIterator(tuint32 dwScheduleID);
	explicit CSchItemIterator(const GUID &ScheduleGUID);
	explicit CSchItemIterator(const char *szName);

	~CSchItemIterator(void);

	bool HasNext(void);
	CScheduleNode *GetNext(void);

private:
	std::list<CScheduleNodeEx *> m_ScheduleNodeList;
	std::list<CScheduleNodeEx *>::iterator m_Iter;
};

//以列表方式迭代出全部的时间段帧率模板
class CTimeSecRateNodeEx;
class _EXPORT_SCHEDULE_CLASS CTimeSecRateIterator
{
public:
	CTimeSecRateIterator(void);
	explicit CTimeSecRateIterator(tuint32 dwNodeID);
	explicit CTimeSecRateIterator(const GUID &NodeGUID);
	explicit CTimeSecRateIterator(const char *szName);
	~CTimeSecRateIterator(void);

	bool HasNext(void);
	CTimeSecRateNode *GetNext(void);

private:
	std::list<CTimeSecRateNodeEx *> m_TimeRateNodeList;
	std::list<CTimeSecRateNodeEx *>::iterator m_Iter;
};

#endif //__SCHEDULE_ITERATOR__