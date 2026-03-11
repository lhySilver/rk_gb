#ifndef __ANBAO_LIGHT_MANAGER_H__
#define __ANBAO_LIGHT_MANAGER_H__

#include "Common.h"

typedef enum{
	AnBaoLightManualOperation_CLOSE,
	AnBaoLightManualOperation_OPEN,
	AnBaoLightManualOperation_AUTO
} AnBaoLightManualOperation_E;

typedef enum{
	AnBaoLightStatus_OPEN,
	AnBaoLightStatus_CLOSE,
} AnBaoLightStatus_E;


class CAnBaoLightManager : public CThread
{
public:
	typedef enum _ENUM_INDICATOR_LIGHT_TYPE
	{
		ENUM_INDICATOR_LIGHT_NONE = 0,
		ENUM_INDICATOR_LIGHT_SLOW_FLICKER,
		ENUM_INDICATOR_LIGHT_FAST_FLICKER,
		ENUM_INDICATOR_LIGHT_ALWAYS_ON,

        ENUM_INDICATOR_LED23_LIGHT_FAST_FLICKER,
        ENUM_INDICATOR_LED2_LIGHT_FAST_FLICKER,
        ENUM_INDICATOR_LED3_LIGHT_FAST_FLICKER,
        ENUM_INDICATOR_LED23_LIGHT_ALWAYS_ON,
	}ENUM_INDICATOR_LIGHT_TYPE;


public:
	CAnBaoLightManager();
	~CAnBaoLightManager();

	static CAnBaoLightManager* instance();
	
	void onConfigFlightWarn(const CConfigTable &table, int &ret);

	bool Start();
	bool Stop();
	
	void ThreadProc();

	void SetAnBaoLightStatus(bool enable);
	bool GetAnBaoLightStatus();
	void SetAnBaoLightlightness();

	void SetMDStatus(int iMDStatus);
	void SetMDEnable(bool enable);

private:

	ENUM_INDICATOR_LIGHT_TYPE m_indicatorLightStatus;
	CMutex m_Mutex;
	static CAnBaoLightManager *m_instance;

	int m_iMD_status;
	FlightWarnConfig m_CfgFlightWarn;

	AnBaoLightManualOperation_E m_eOperation;
	AnBaoLightStatus_E m_eAnBaoLightStatus;

	bool m_bMD_Enable;
};

#define g_AnBaoLightManager (*CAnBaoLightManager::instance())

#endif 		//__ANBAO_LIGHT_MANAGER_H__
