#include <stdio.h>
#include "Infra/Infra.h"
#include "Infra/Thread.h"
#include "Infra/Timer.h"
#include "Timer.h"
#include "Thread.h"
#include "Log/DebugDef.h"

CInfra::LogProc g_InfraLogHook;

PATTERN_SINGLETON_IMPLEMENT(CInfra);

CInfra::CInfra()
{
}

CInfra::~CInfra()
{
}

// Infra 组件启动
bool CInfra::start()
{
	AppInfo("===========================Infra Build in:%s, %s===================================\n", __DATE__, __TIME__);
	g_TimerManager.Start();
	g_ThreadManager.RegisterMainThread(CThread::GetCurrentID());	// 注册主线程
	return true;
}

bool CInfra::setLogHook(LogProc proc)
{
	g_InfraLogHook = proc;
	return true;
}
