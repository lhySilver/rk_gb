#ifndef __INDICATOR_LIGHT_H__
#define __INDICATOR_LIGHT_H__

#include "Infra/Thread.h"
#include "ExchangeAL/Exchange.h"



class CIndicatorLight : public CThread
{
public:
	typedef enum _ENUM_INDICATOR_LIGHT_TYPE
	{
		ENUM_INDICATOR_LIGHT_NONE = 0,
		ENUM_POWER_INDICATOR_LIGHT_SLOW_FLICKER,
		ENUM_POWER_INDICATOR_LIGHT_FAST_FLICKER,
		ENUM_POWER_INDICATOR_LIGHT_ALWAYS_ON,
		ENUM_POWER_INDICATOR_LIGHT_ALWAYS_OFF,

		ENUM_LINK_INDICATOR_LIGHT_SLOW_FLICKER,
		ENUM_LINK_INDICATOR_LIGHT_FAST_FLICKER,
		ENUM_LINK_INDICATOR_LIGHT_ALWAYS_ON,
		ENUM_LINK_INDICATOR_LIGHT_ALWAYS_OFF,

		ENUM_EPI_INDICATOR_LIGHT_SLOW_FLICKER,
		ENUM_EPI_INDICATOR_LIGHT_FAST_FLICKER,
		ENUM_EPI_INDICATOR_LIGHT_ALWAYS_ON,
		ENUM_EPI_INDICATOR_LIGHT_ALWAYS_OFF,
	}ENUM_INDICATOR_LIGHT_TYPE;


public:
	CIndicatorLight();
	~CIndicatorLight();

	static CIndicatorLight* instance();

	/// 开始语音对讲
	bool start();
	
	/// 停止语音对讲
	bool stop();
	
	void onConfigIndicatorLight(const CConfigTable &table, int &ret);

	void ThreadProc();

	int setLightStatus(ENUM_INDICATOR_LIGHT_TYPE status);

private:

	ENUM_INDICATOR_LIGHT_TYPE m_powerIndicatorLightStatus;
	ENUM_INDICATOR_LIGHT_TYPE m_linkIndicatorLightStatus;
	ENUM_INDICATOR_LIGHT_TYPE m_epiIndicatorLightStatus;
	
	CMutex m_Mutex;
	static CIndicatorLight *m_instance;

	int m_iIndicatorLightCfg;
};

#define g_IndicatorLight (*CIndicatorLight::instance())

#endif 		//__INDICATOR_LIGHT_H__
