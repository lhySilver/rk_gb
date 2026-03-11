#include "Wdt.h"

//------------------看门狗---------------
PATTERN_SINGLETON_IMPLEMENT(CFeedDog);

CFeedDog::CFeedDog() : CThread("FeedDog", 1)
{
}

CFeedDog::~CFeedDog()
{
}

void CFeedDog::ThreadProc()
{
	while (m_bLoop)
	{
		AppWarning("WatchdogFeed\n");
		WatchdogFeed();
		CTime::sleep(1 * 1000);
	}
}

bool CFeedDog::create()
{
	int ret;
	ret = WatchdogCreate(45);
	return (0 == ret);
}

bool CFeedDog::destory()
{
	int ret;
	ret = WatchdogDestory();
	return (0 == ret);
}

bool CFeedDog::start()
{
	CreateThread();
	return true;
}
bool CFeedDog::stop()
{
	AppErr("CFeedDog stop\n");
	DestroyThread();
	m_bLoop = false;
	return true;
}

