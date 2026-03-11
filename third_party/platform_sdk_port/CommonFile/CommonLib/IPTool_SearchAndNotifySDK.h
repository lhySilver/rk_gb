
///////////////////////////IPTool_SearchAndNotifySDK.h

#ifndef _IPTOOL_SEARCH_AND_NOTIFY_SDK_H
#define _IPTOOL_SEARCH_AND_NOTIFY_SDK_H

#include "base_type_define.h"

#ifdef WIN32

#ifdef IPTOOL_SEARCH_AND_NOTIFY_SDK_EXPORT_API//动态库编译
#define _EXPORT_IPTOOL_SEARCH_AND_NOTIFY_SDK __declspec(dllexport)
#else//使用者编译
#define _EXPORT_IPTOOL_SEARCH_AND_NOTIFY_SDK __declspec(dllimport)
#endif

#else
#define _EXPORT_IPTOOL_SEARCH_AND_NOTIFY_SDK  __attribute__ ((visibility("default")))
#endif

#define DEV_SEARCH_ADD    1 //设备上线
#define DEV_SEARCH_DEC    2 //设备下线
#define DEV_SEARCH_UPDATE 3 //设备信息变化了

#define SERVER_SEARCH_ADD    1 //服务上线
#define SERVER_SEARCH_DEC    2 //服务下线
#define SERVER_SEARCH_UPDATE 3 //服务信息变化了

typedef enum _search_type_
{
	SEARCH_TVT_STANDARD_DEV	=0x001,
	SEARCH_ONVIF_DEV		=0x002,
	SEARCH_UPNP_DEV			=0x004,
	SEARCH_AIPSTAR_DEV		=0x008,
	SEARCH_DAHUA_DEV		=0x010,
	SEARCH_HIK_DEV			=0x020,
    SEARCH_UNIVIEW_DEV		=0x040,
    SEARCH_YCX_DEV          =0x080,
	SEARCH_SPECO_DEV		=0x100,
	SEARCH_ALL_DEV          =0xffff//全部设备类型
}SEARCH_TYPE;

enum IPTOOL_SETIP_STATUS_CODE
{
	IPTOOL_SETIP_STATUS_OK = 0, 
	IPTOOL_SETIP_STATUS_UNAUTHORIZED = 1, // 用户名密码错误
	IPTOOL_SETIP_STATUS_OTHER
} ;

typedef struct _deviceInfo
{	
	char			series[64];
	char			devName[64];
	char	        deviceType[16];
	char			szproductModel[16];
	char			szVersion[32];
	char			szFactoryName[16];
    char            szEthName[16];
	unsigned short	netport;
    unsigned short	nHttpPort;
	tuint32			ipaddr;
	tuint32			gateway;
	tuint32			netmask;
	tuint32			dns1;
	tuint32			dns2;
    tuint16  nChannelCount;  //NVR 通道数

	_deviceInfo()
	{
		memset(series,0,sizeof(series));
		memset(devName,0,sizeof(devName));
		memset(deviceType,0,sizeof(deviceType));
		memset(szproductModel,0,sizeof(szproductModel));
		memset(szVersion,0,sizeof(szVersion));
		memset(szFactoryName, 0, sizeof(szFactoryName));
        memset(szEthName, 0, sizeof(szEthName));
        ipaddr = gateway = netmask = netport = nHttpPort = dns1 = dns2 = 0;
        nChannelCount = 0;
	}
}DEVICE_INFO;

typedef void (WINAPI *Handle_IPTool_SearchDataCallBack)(char* hwaddr, char *szDevIP, int opt, const char* szXmlData, void *pParam, const char *szRecvFromNIC);
typedef void (WINAPI *Handle_IPTool_SearchDataCallBackEx)(char* hwaddr, char *szDevIP, int opt, const DEVICE_INFO *pData, void *pParam, const char *szRecvFromNIC);
typedef bool (WINAPI *Handle_IPTool_NotifyReplySearch)(char *szXmlBuffer, int nMaxBufferLen);
typedef void (WINAPI *Handle_IPTool_NotifySetNetwork)(char *hwaddr, tuint32 dwIP, tuint32 dwNetmask, tuint32 dwGateway, tuint32 dwDNS1, tuint32 dwDNS2, char *sPwd);
typedef void (WINAPI *Handle_IPTool_NotifyRestorDefault)(char *hwaddr);
typedef void (WINAPI *Handle_IPTool_OpTelnetCallBack)(char *hwaddr, char *sPwd, bool bIsOpen);
typedef void (WINAPI *Handle_IPTool_NotifySetMac)(char *hwaddr,const char* szNewMac);
typedef void (WINAPI *Handle_IPTool_SearchServerDataCallBack)(char* hwaddr, int opt, const char* szXmlData, void *pParam);
typedef bool (WINAPI *Handle_IPTool_NotifyReplySearch_Server)(char *szXmlBuffer, int nMaxBufferLen,void *pParam);
/* Handle_IPTool_NotifyReplySearch 回调出的 szXmlBuffer 数据格式：
<?xml version="1.0" encoding="UTF-8"?>
<notifyIPInfos count="1">
       <info devName="test:{BFFD5A4E-58D7-41DB-8071-A4739903A355}" MAC="00:E0:4D:BA:D6:4B" ipAddr="192.168.6.95" maskAddr="255.255.255.0"
            route="192.168.6.1" dns1="8.8.8.8" dns2="202.96.134.133" netPort="6003" webPort="80" softVer="270532"
            ipAddr_2="" maskAddr_2="" hardwareVer="270532" softBuildDate="20131014" kernelVer="538181936" productSeries="3"
            productType="4865" productName="Demo" channelCount="16">
       </info>
</notifyIPInfos>
*/

//初始化
_EXPORT_IPTOOL_SEARCH_AND_NOTIFY_SDK bool IPTOOL_SEARCH_AND_NOTIFY_Initial(void);
//退出
_EXPORT_IPTOOL_SEARCH_AND_NOTIFY_SDK void IPTOOL_SEARCH_AND_NOTIFY_Quit(void);

//开始Notify功能（即可以向IPTool报告，以及被IPTool修改IP）
_EXPORT_IPTOOL_SEARCH_AND_NOTIFY_SDK bool IPTOOL_SEARCH_AND_NOTIFY_Start_Notify(Handle_IPTool_NotifyReplySearch ReplySearchCallBack, Handle_IPTool_NotifySetNetwork SetNetworkCallBack);
//停止Notify功能
_EXPORT_IPTOOL_SEARCH_AND_NOTIFY_SDK void IPTOOL_SEARCH_AND_NOTIFY_Stop_Notify(void);
//注册恢复出厂设置的回调函数
_EXPORT_IPTOOL_SEARCH_AND_NOTIFY_SDK void IPTOOL_SEARCH_AND_NOTIFY_Register_RestorDefault(Handle_IPTool_NotifyRestorDefault pRestorDefaultCallBack);
//注册设置MAC的回调函数
_EXPORT_IPTOOL_SEARCH_AND_NOTIFY_SDK void IPTOOL_SEARCH_AND_NOTIFY_Register_SetMac(Handle_IPTool_NotifySetMac pSetMacCallBack);

//开始Search IPC 功能
//nMaxRecordCount 指定最多搜索记录条数(TVT/ONVIF/UPNP各自计数
_EXPORT_IPTOOL_SEARCH_AND_NOTIFY_SDK tuint32 IPTOOL_SEARCH_AND_NOTIFY_Start_Search(Handle_IPTool_SearchDataCallBack SearchCallBack,Handle_IPTool_SearchDataCallBackEx SearchCallBackEx, void *pParam, tuint32 SearchTypeMask=SEARCH_TVT_STANDARD_DEV, int nMaxRecordCount=2000);
//停止Search IPC 功能
_EXPORT_IPTOOL_SEARCH_AND_NOTIFY_SDK void IPTOOL_SEARCH_AND_NOTIFY_Stop_Search(tuint32 hSearch);

//设置设备的网络配置(IP、子网掩码、DNS等)
/* szXmlData 的格式为
<?xml version="1.0" encoding="UTF-8"?>
<searchSetIP password="123456" serialNo="01:AE:18:46:57:12" productSeries="TVT_IPC" dhcpSwitch="false" ipAddr="192.168.6.251" maskAddr="255.255.255.0"
  route="192.168.6.1" dns1="8.8.8.8" dns2="202.96.134.133" userName="admin" oldIpAddr="192.168.6.250" port="80">
</searchSetIP>
其中 serialNo 为网卡MAC(for TVT)或者GUID(for ONVIF)
 */
//更改IPC设备的网络配置，hwaddr为 BYTE[6]，即MAC的bin数据
_EXPORT_IPTOOL_SEARCH_AND_NOTIFY_SDK bool IPTOOL_SEARCH_AND_NOTIFY_Set_DevIP(char* hwaddr, const char *szXmlData);
//设置设备MAC地址
_EXPORT_IPTOOL_SEARCH_AND_NOTIFY_SDK bool IPTOOL_SEARCH_AND_NOTIFY_SET_DevMac(char* hwaddr,const char* szNewMac);

//通知设备恢复缺省设置
_EXPORT_IPTOOL_SEARCH_AND_NOTIFY_SDK bool IPTOOL_SEARCH_AND_NOTIFY_Set_DevRestoreDefault(char* hwaddr);

//通知更新设备信息
_EXPORT_IPTOOL_SEARCH_AND_NOTIFY_SDK void IPTOOL_SEARCH_AND_NOTIFY_UpdateDeviceInfo();

_EXPORT_IPTOOL_SEARCH_AND_NOTIFY_SDK void IPTOOL_SEARCH_AND_NOTIFY_Register_OpTelnet(Handle_IPTool_OpTelnetCallBack pOpTelnetCallBack);


//发送查询局域网服务器的命令
_EXPORT_IPTOOL_SEARCH_AND_NOTIFY_SDK bool IPTOOL_SEARCH_AND_NOTIFY_Start_Search_Server(Handle_IPTool_SearchServerDataCallBack SearchServerCallBack, void *pParam);
//停止查询局域网服务器的命令
_EXPORT_IPTOOL_SEARCH_AND_NOTIFY_SDK void IPTOOL_SEARCH_AND_NOTIFY_Stop_Search_Server(void);
//开始Notify服务功能
_EXPORT_IPTOOL_SEARCH_AND_NOTIFY_SDK bool IPTOOL_SEARCH_AND_NOTIFY_Start_Notify_Server(Handle_IPTool_NotifyReplySearch_Server ReplySearchServerCallBack, void *pParam);
//停止Notify服务功能
_EXPORT_IPTOOL_SEARCH_AND_NOTIFY_SDK void IPTOOL_SEARCH_AND_NOTIFY_Stop_Notify_Server(void);


#endif
