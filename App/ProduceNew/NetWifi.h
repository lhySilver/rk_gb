#ifndef _NETWIFI_H
#define _NETWIFI_H

#include "ExchangeAL/NetWorkExchange.h"
#include "ExchangeAL/Exchange.h"
#include "Manager/ConfigManager.h"
#include "Infra/Singleton.h"
#include "Infra/Thread.h"
#include "PAL/Net.h"
#include "Common.h"
#define WIFI_ESSID_LEN 64 
class CNetWifi : public CThread
{
	PATTERN_SINGLETON_DECLARE(CNetWifi);

public:

	typedef enum
	{
		NET_WIFI_DISCONNECTED 		= 0, 	//WIFI未连接
		NET_WIFI_CONNECTING 		= 1, 	//WIFI正在连接
		NET_WIFI_CONNECTED 			= 2,	//WIFI已连接
		NET_WIFI_PSW_ERROR 			= 3,	//WIFI密码错误
		NET_WIFI_CONN_FAILED		= 4,	//WIFI连接失败
	}NTE_LINK_STATE;

	typedef	enum {
		NET_CHECK_ETH0_VALID = 0,
		NET_CHECK_ETH0_INLINE,
		NET_CHECK_WLAN0_VALID,
		NET_CHECK_WLAN0_INLINE,
		NET_CHECK_WAITE_FOR,
		NET_CHECK_WAITE_NONE,
	}NTE_CHECK_STATE;

	typedef struct
	{
		char essid[WIFI_ESSID_LEN + 1];	//essid
		char mode;						//mode_type_enum
		char encryptType;				//encrypt_type_enum
		char quality;                   //max is 100  
	}WIFI_LIST;
	
	int Init();
	int UnInit();
	
	/// 启动wifi station模式
	int StartWifiStationMode(void);
	/// 停止wifi station 模式
	int StopWifiStationMode(void);
    void SetWifi(std::string ssid, std::string pwd,std::string ip, std::string gw, std::string mask);
	int GetEthStatus(void);
	int GetWifiConnetStatus(void);
	int GetWifiWifiList(WIFI_LIST* list, int MAX_Len);
	int GetLocalIp(const char *eth_inf, char *ip);
	int GetNetIfInLine(char *name);
	void onAppEvent(std::string code, int index, appEventAction action, const EventHandler *param, Json::Value* data);

private:
	int Check_netlink_status(const char * if_name);
	/// 网络状态监视线程
	void ThreadProc(void);
	/// 检查网口连接状态
	int CreateWpaConfig(int encrypt, const char *ssid, const char *passwd);

private:
	int m_eth;
	NTE_CHECK_STATE m_wifi_check_status;
	NTE_LINK_STATE m_wifi_connect_result;

	std::string m_ssid;
	std::string m_pwd;
	std::string m_ip; 
	std::string m_gw; 
	std::string m_mask;
};

#define g_NetWifi (*CNetWifi::instance())
#endif

