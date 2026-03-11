#ifndef __WDT_H__
#define __WDT_H__

#include <string>
#include "Infra/Timer.h"
#include <Infra/Singleton.h>
#include "Common.h"

class CFeedDog : public CThread
{
	PATTERN_SINGLETON_DECLARE(CFeedDog);
public:
	bool start();
	void ThreadProc();
	bool stop();
	bool create();
	bool destory();
};
#endif
