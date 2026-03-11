#ifndef __PCBA_H__
#define __PCBA_H__

#include "Common.h"

#include <map>
#include <string>

class CPcba : public CThread
{
public:
	
	PATTERN_SINGLETON_DECLARE(CPcba);

	static void motion_cb();

	/// 线程执行体
	void ThreadProc();

	int init();
	/// 开启
	bool start();

	/// 停止
	bool stop();
	
	void onAppEvent(std::string code, int index, appEventAction action, const EventHandler *param, Json::Value* data);
	
};

#define g_PcbaHandle (*CPcba::instance())



#endif// __TUTK_H__

