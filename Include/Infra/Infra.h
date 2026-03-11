#ifndef __INFRA_STARTUP_H__
#define __INFRA_STARTUP_H__

#include "Singleton.h"
#include "Function.h"

class CInfra
{
	PATTERN_SINGLETON_DECLARE(CInfra);

public:

	// 系统日志回调函数
	typedef TFunction2<void, const char *, const char *> LogProc;

	bool start();

	/// 设置日志回调函数
	bool setLogHook(LogProc proc);

};
#endif
