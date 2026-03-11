#ifndef __ONVIF__
#define __ONVIF__


#include "Manager/User.h"
#include "Manager/Console.h"
#include "Infra/Thread.h"
#include "Infra/Function.h"
#include "Infra/Singleton.h"
#include "Infra/Wrapper.h"
#include "Infra/Timer.h"

#include "Manager/EventManager.h"
#include <map>
#include <string>

#include "PAL/Motor.h"

#include "ExchangeAL/Exchange.h"
#include "ExchangeAL/CommExchange.h"



class COnvif : public CThread
{
public:
	
	PATTERN_SINGLETON_DECLARE(COnvif);

	/// 线程执行体
	void ThreadProc();
	void onAppEvent(std::string code, int index, appEventAction action, const EventHandler *param, Json::Value* data);
	void onConfigOnvif(const CConfigTable &table, int &ret);
	int Init();
	int UnInit();
	int Start();
	int Stop();

private:
	int m_onvifStart;
	OnvifConf_S m_stOnvifCfg;
	bool m_bIsUpdate;
};

#define g_OnvifHandle (*COnvif::instance())



#endif// __ONVIF__

