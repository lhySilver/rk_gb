
///////////////////////////IPTool_SearchSDK.h

#ifndef _IPTOOL_SEARCH_SDK_H
#define _IPTOOL_SEARCH_SDK_H

#include "base_type_define.h"

#ifdef WIN32

#ifdef IPTOOL_SEARCH_SDK_EXPORT_API//动态库编译
#define _EXPORT_IPTOOL_SEARCH_SDK  __declspec(dllexport)
#else//使用者编译
#define _EXPORT_IPTOOL_SEARCH_SDK __declspec(dllimport)
#endif

#else
#define _EXPORT_IPTOOL_SEARCH_SDK  __attribute__ ((visibility("default")))
#endif

#define DEV_SEARCH_ADD    1 //设备上线
#define DEV_SEARCH_DEC    2 //设备下线
#define DEV_SEARCH_UPDATE 3 //设备信息变化了

typedef enum _search_type_
{
	SEARCH_TVT_STANDARD_DEV	=0x020,
	SEARCH_ONVIF_DEV		=0x040,
	SEARCH_UPNP_DEV			=0x080
}SEARCH_TYPE;

///初始化  默认是禁用自动搜索的
_EXPORT_IPTOOL_SEARCH_SDK bool IPTOOL_SEARCH_Initial( void(WINAPI *HandleDataCallBack)(char* hwaddr, int opt, const char* szXmlData, void *pParam), void *pParam,tuint32 SearchTypeMask=SEARCH_TVT_STANDARD_DEV);
_EXPORT_IPTOOL_SEARCH_SDK void IPTOOL_SEARCH_Quit(void);

_EXPORT_IPTOOL_SEARCH_SDK tuint32 IPTOOL_SEARCH_SetCallBack( void(WINAPI *HandleDataCallBack)(char* hwaddr, int opt, const char* szXmlData, void *pParam), void *pParam, tuint32 SearchTypeMask=SEARCH_TVT_STANDARD_DEV);
_EXPORT_IPTOOL_SEARCH_SDK void IPTOOL_SEARCH_CancelCallBack( tuint32 handle );

_EXPORT_IPTOOL_SEARCH_SDK void IPTOOL_SEARCH_Manual(void);	 ////手动发送一次搜索的指令

_EXPORT_IPTOOL_SEARCH_SDK void IPTOOL_SEARCH_StartAuto(int nIntervalTimeSecond);	 ////自动发送搜索指令   参数间隔时间  单位为秒 ， 
_EXPORT_IPTOOL_SEARCH_SDK void IPTOOL_SEARCH_StopAuto();	 ////自动发送搜索指令  

//设置设备的网络配置(IP、子网掩码、DNS等) 
/* szXmlData 的格式为
<?xml version="1.0" encoding="UTF-8"?>
<searchSetIP password="123456" serialNo="01:AE:18:46:57:12" dhcpSwitch="false" ipAddr="192.168.6.251" maskAddr="255.255.255.0"
  route="192.168.6.1" dns1="8.8.8.8" dns2="202.96.134.133">
</searchSetIP>
其中 serialNo 为网卡MAC(for TVT)或者GUID(for ONVIF)
 */
_EXPORT_IPTOOL_SEARCH_SDK bool IPTOOL_SET_CONFIG(char* hwaddr, const char *szXmlData);
//通知设备恢复缺省设置
_EXPORT_IPTOOL_SEARCH_SDK bool IPTOOL_SET_DevRestoreDefault(char* hwaddr);

#endif  ///////#ifndef _IPTOOL_SEARCH_SDK_H
