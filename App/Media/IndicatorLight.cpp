#include "IndicatorLight.h"

#include <errno.h>
#include <unistd.h>
#include "PAL/Misc.h"
#include "Log/DebugDef.h"
#include "Types/Defs.h"
#include "../tuya/Tuya_common.h"
#include "Manager/ConfigManager.h"
#include "ExchangeAL/ExchangeKind.h"


#include "Common.h"


CIndicatorLight* CIndicatorLight::m_instance = NULL;

CIndicatorLight::CIndicatorLight() : CThread("IndicaotrLight", TP_PLAY)
{
	m_powerIndicatorLightStatus = ENUM_INDICATOR_LIGHT_NONE;
	m_linkIndicatorLightStatus = ENUM_INDICATOR_LIGHT_NONE;

	
	Json::Value table;
	g_configManager.getConfig(getConfigName(CFG_LIGHT), table);
	
	m_iIndicatorLightCfg = table["Light"].asInt();

	g_configManager.attach(getConfigName(CFG_LIGHT), IConfigManager::Proc(&CIndicatorLight::onConfigIndicatorLight, this));
}

CIndicatorLight::~CIndicatorLight()
{

}

CIndicatorLight* CIndicatorLight::instance()
{
	if(NULL == CIndicatorLight::m_instance)
	{
		CIndicatorLight::m_instance = new CIndicatorLight();
	}
	return CIndicatorLight::m_instance;
}

bool CIndicatorLight::start()
{
	CreateThread();
	return true;
}

bool CIndicatorLight::stop()
{
	DestroyThread();
	return true;
}

void CIndicatorLight::onConfigIndicatorLight(const CConfigTable &table, int &ret)
{
	int iNewCfg = table["Light"].asInt();

	m_iIndicatorLightCfg = iNewCfg;
}


void CIndicatorLight::ThreadProc()
{
	int ret;
	unsigned char slow_flicker_count = 0;
	unsigned char fast_flicker_count = 0;
	while (m_bLoop) 
	{
		if( 0 == m_iIndicatorLightCfg ) 	//关闭指示灯功能
		{
			SystemSetLight_Power(0);
			SystemSetLight_Link(0);
			while(0 == m_iIndicatorLightCfg)
				usleep(100000);
		}
		
		//电源指示灯
		if( ENUM_POWER_INDICATOR_LIGHT_SLOW_FLICKER == m_powerIndicatorLightStatus ) 			//慢闪
		{
			if( 0 == slow_flicker_count ) 			//亮
			{
				SystemSetLight_Power(1);
			}
			else 									//灭
			{
				SystemSetLight_Power(0);
			}
			slow_flicker_count++;
			if( slow_flicker_count >= 15 )
				slow_flicker_count = 0;
		}
		else if( ENUM_POWER_INDICATOR_LIGHT_FAST_FLICKER == m_powerIndicatorLightStatus ) 		//快闪
		{
			if( 0 == fast_flicker_count ) 			//亮
			{
				SystemSetLight_Power(1);
			}
			else 									//灭
			{
				SystemSetLight_Power(0);
			}
			fast_flicker_count++;
			if( fast_flicker_count >= 3 )
				fast_flicker_count = 0;			
		}
		else if( ENUM_POWER_INDICATOR_LIGHT_ALWAYS_ON == m_powerIndicatorLightStatus ) 		//常亮
		{
			SystemSetLight_Power(1);
		}
		else if( ENUM_POWER_INDICATOR_LIGHT_ALWAYS_OFF == m_powerIndicatorLightStatus ) 		//常灭
		{
			SystemSetLight_Power(0);
		}
		
		//联网指示灯
		if( ENUM_LINK_INDICATOR_LIGHT_SLOW_FLICKER == m_linkIndicatorLightStatus ) 			//慢闪
		{
			if( 0 == slow_flicker_count ) 			//亮
			{
				SystemSetLight_Link(1);
			}
			else 									//灭
			{
				SystemSetLight_Link(0);
			}
			slow_flicker_count++;
			if( slow_flicker_count >= 15 )
				slow_flicker_count = 0;
		}
		else if( ENUM_LINK_INDICATOR_LIGHT_FAST_FLICKER == m_linkIndicatorLightStatus ) 		//快闪
		{
			if( 0 == fast_flicker_count ) 			//亮
			{
				SystemSetLight_Link(1);
			}
			else 									//灭
			{
				SystemSetLight_Link(0);
			}
			fast_flicker_count++;
			if( fast_flicker_count >= 3 )
				fast_flicker_count = 0;			
		}
		else if( ENUM_LINK_INDICATOR_LIGHT_ALWAYS_ON == m_linkIndicatorLightStatus ) 		//常亮
		{
			
			SystemSetLight_Link(1);
		}
		else if( ENUM_LINK_INDICATOR_LIGHT_ALWAYS_OFF == m_linkIndicatorLightStatus ) 		//常灭
		{
			
			SystemSetLight_Link(0);
		}

		//网口指示灯
		if( ENUM_EPI_INDICATOR_LIGHT_SLOW_FLICKER == m_epiIndicatorLightStatus ) 			//慢闪
		{
			if( 0 == slow_flicker_count ) 			//亮
			{
				SystemSetLight_Epi(1);
			}
			else 									//灭
			{
				SystemSetLight_Epi(0);
			}
			slow_flicker_count++;
			if( slow_flicker_count >= 15 )
				slow_flicker_count = 0;
		}
		else if( ENUM_EPI_INDICATOR_LIGHT_FAST_FLICKER == m_epiIndicatorLightStatus ) 		//快闪
		{
			if( 0 == fast_flicker_count ) 			//亮
			{
				SystemSetLight_Epi(1);
			}
			else 									//灭
			{
				SystemSetLight_Epi(0);
			}
			fast_flicker_count++;
			if( fast_flicker_count >= 3 )
				fast_flicker_count = 0;			
		}
		else if( ENUM_EPI_INDICATOR_LIGHT_ALWAYS_ON == m_epiIndicatorLightStatus ) 		//常亮
		{
			SystemSetLight_Epi(1);
		}
		else if( ENUM_EPI_INDICATOR_LIGHT_ALWAYS_OFF == m_epiIndicatorLightStatus ) 		//常灭
		{
			SystemSetLight_Epi(0);
		}
		
		usleep(200000); 	//200ms
	}
}

int CIndicatorLight::setLightStatus(ENUM_INDICATOR_LIGHT_TYPE status)
{
	CGuard guard(m_Mutex);

	if( ENUM_INDICATOR_LIGHT_NONE <= status && status < ENUM_LINK_INDICATOR_LIGHT_SLOW_FLICKER )
		m_powerIndicatorLightStatus = status;
	else if( status >= ENUM_LINK_INDICATOR_LIGHT_SLOW_FLICKER && status < ENUM_EPI_INDICATOR_LIGHT_SLOW_FLICKER) 
		m_linkIndicatorLightStatus = status;
	else if( status >= ENUM_EPI_INDICATOR_LIGHT_SLOW_FLICKER)
		m_epiIndicatorLightStatus = status;
	
	return 0;
}


