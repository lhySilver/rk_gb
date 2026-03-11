
#ifndef __MY_SINGLE_LOCK__
#define __MY_SINGLE_LOCK__
#include "Lock.h"

class CMySingleLock
{
public:
	inline explicit CMySingleLock(CLock &Object):m_Object(Object)
	{
		Object.Lock();
	}

	inline ~CMySingleLock(void)
	{
		m_Object.Unlock();
	}

private:
	CLock &m_Object;
};


class CMySingleEvent
{
public:
	inline explicit CMySingleEvent(CMyEvent &Object):m_Object(Object)
	{
		Object.ResetEvent();
	}

	inline ~CMySingleEvent(void)
	{
		m_Object.SetEvent();
	}

private:
	CMyEvent &m_Object;
};

#endif