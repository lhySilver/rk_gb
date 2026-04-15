#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include "Onvif.h"
#include "../Ptz/Ptz.h"
#include "../Rtsp/live_rtsp.h"
#include "PAL/Net.h"

#include "Common.h"

//static int gProcessRun = 1;
#ifdef __cplusplus
extern "C" {
#endif

int StartOnvifDiscoveryPthread(void);
int StartOnvifPthread();
void StopOnvifPthread(void);
int motor_ctrl(int (*)(int));
void setOnvifPassword(const char *password);
void setDeviceSerialNumber(const char *num);
void setOnvifAuth(char value);
#ifdef __cplusplus
}
#endif


int onvif_motor_ctrl(int dir)
{
	printf("dir is %d\n",dir);
	g_Alarm.SetAllowMotionDetTime(time(NULL)+10);
	PTZDirection_e ptzdir;
	if(dir > 0)
	{
		switch (dir)
		{
			case 1:
				ptzdir = PTZ_LEFT;
				break;
			case 2:
				ptzdir = PTZ_RIGHT;
				break;
			case 3:
				ptzdir = PTZ_UP;
				break;
			case 4:
				ptzdir = PTZ_DOWN;
				break;
			default :
				return 0;
		}
		g_PtzHandle.DoMotorMove(ptzdir);
	}
	else
	{
		g_PtzHandle.DoMotorStop();
	}
	
	return 0;
}



PATTERN_SINGLETON_IMPLEMENT(COnvif);

COnvif::COnvif():CThread("NetInterFace", TP_DEFAULT)
{
	m_bIsUpdate = false;
	m_onvifStart = 0;

	Json::Value table;
	g_configManager.getConfig(getConfigName(CFG_ONVIF), table);
	TExchangeAL<OnvifConf_S>::getConfig(table, m_stOnvifCfg);

//m_stOnvifCfg.Enable = true; //<shang> debug
	g_configManager.attach(getConfigName(CFG_ONVIF), IConfigManager::Proc(&COnvif::onConfigOnvif, this));
	IEventManager::instance()->attach(IEventManager::Proc(&COnvif::onAppEvent, this));
}

COnvif::~COnvif()
{
	g_configManager.detach(getConfigName(CFG_ONVIF), IConfigManager::Proc(&COnvif::onConfigOnvif, this));
}

void COnvif::onConfigOnvif(const CConfigTable &table, int &ret)
{
	OnvifConf_S stNewOnvifCfg;

	TExchangeAL<OnvifConf_S>::getConfig(table, stNewOnvifCfg);

	if( stNewOnvifCfg.Enable!= m_stOnvifCfg.Enable )
	{
		if( stNewOnvifCfg.Enable )
		{
			Start();
		}
		else
		{
			Stop();
		}
	}
	if( stNewOnvifCfg.Password != m_stOnvifCfg.Password )
	{
		setOnvifPassword(stNewOnvifCfg.Password.c_str());
	}

	m_stOnvifCfg = stNewOnvifCfg;
}

int COnvif::Start()
{
	if (m_onvifStart)
		return 0;
	
	unsigned char wifi_mac[6] = {0};
	char num[64] = {0};
	NetGetMac_2("wlan0", wifi_mac);
	snprintf(num, sizeof(num), "%02X-%02X-%02X-%02X-%02X-%02X", wifi_mac[0], wifi_mac[1], wifi_mac[2], wifi_mac[3], wifi_mac[4],wifi_mac[5]);
	setDeviceSerialNumber(num);
	setOnvifPassword(m_stOnvifCfg.Password.c_str());
	// setOnvifAuth(ProductCof_g.onvif_auth);
	setOnvifAuth(0);

	//rtsp server
	printf("StartRtspPthread start!\n");
	StartRtspPthread();
	printf("StartRtspPthread end!\n");
	
	printf("StartOnvifPthread start!\n");
	StartOnvifPthread();
	printf("StartOnvifPthread end!\n");
	
	motor_ctrl(onvif_motor_ctrl);
	m_onvifStart = 1;
	return 0;
}

int COnvif::Stop()
{
	if (!m_onvifStart)
		return 0;
	
	motor_ctrl(NULL);
	
	printf("StopOnvifPthread start!\n");
	StopOnvifPthread();
	printf("StopOnvifPthread end!\n");
	
	printf("StopRtspPthread start!\n");
	StopRtspPthread();
	printf("StopRtspPthread end!\n");
	m_onvifStart = 0;
	return 0;
}

int COnvif::Init()
{
	AppErr("COnvif::Start\n");
	if( false == IsThreadOver() ) 	//防止多次调用
		return 0;
	
	return CreateThread();
}
int COnvif::UnInit()
{
	return DestroyThread();
}

void COnvif::ThreadProc()
{
	int ret = -1;

	while(m_bLoop)
	{
		sleep(1);

		if (m_bIsUpdate)
			continue;
		
		if( m_stOnvifCfg.Enable )
		{
			//上报onvif ip地址
			static char strOldIpAddr[20] = {0};
			static int ilastCheckTime = 0;
			if( (time(0) - ilastCheckTime) > 60 )
			{
				ilastCheckTime = time(0);
				char ip[20] = {0};
				
				g_NetConfigHook.GetNetWorkIp(ip,sizeof(ip));
				
				printf("ThreadProc get ip : %s\n",ip);
				
				if( (strcmp(strOldIpAddr, ip) != 0) && (ip[0] != '\0') )
				{
//					if (0 == g_TuyaHandle.ReportOnvifIp(ip))
					{
						strcpy(strOldIpAddr, ip);
					}
				}
			}
			static int s_status = -1;
			if (-1 == s_status)
			{
				int status = g_NetConfigHook.GetNetWorkLindMode(); 	// 1-eth0 2-AP 3-STA
				if (status !=0 )
				{
					s_status = status;
					Start();
				}
			}
			else
			{
				int status = g_NetConfigHook.GetNetWorkLindMode(); 	// 1-eth0 2-AP 3-STA
				if (s_status && status && (status != s_status)) 
				{
					//网络切换重启 onvif and rtsp
					Stop();
					Start();
					s_status = status;
				}
			}
		}
	}
}
void COnvif::onAppEvent(std::string code, int index, appEventAction action, const EventHandler *param, Json::Value* data)
{
	if( "UpgradeReleaseResource" == code )
	{
		AppErr("stop onvif\n");
		m_bIsUpdate = true; 	//正在升级
		Stop();
	}
}