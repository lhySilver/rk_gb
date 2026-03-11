
#ifndef __NET_CLIENT_SDK_H__
#define __NET_CLIENT_SDK_H__
#include "base_type_define.h"
#include "DevDefine.h"
#ifdef WIN32
#define strcasecmp  stricmp
#define strncasecmp strnicmp

#include <string>
#ifdef ONVIF_SDK_EXPORT_API//动态库编译
#define _EXPORT_ONVIF_SDK  __declspec(dllexport)
#else//使用者编译
#define _EXPORT_ONVIF_SDK __declspec(dllimport)
#endif

#else
#define _EXPORT_ONVIF_SDK  __attribute__ ((visibility("default")))
#endif

///////////////////搜索模块

#define ONVIF_DEV_SEARCH_ADD     	1 //设备上线
#define ONVIF_DEV_SEARCH_DEC     	2 //设备下线

#define DEV_TYPE_ONVIF "ONVIF"			//ONVIF设备类型名称

typedef enum _eliminate_type_
{
	ONVIF_ELIMINATE_HK=0x020,
	ONVIF_ELIMINATE_DH=0x040,
}ELIMINATE_TYPE;

_EXPORT_ONVIF_SDK bool ONVIF_SEARCH_Initial(void(WINAPI *HandleDataCallBack)(char* hwaddr, tint32 opt, const char* szXmlData, void *pParam), void *pParam,tuint32 eliminateTypeMask);
_EXPORT_ONVIF_SDK void ONVIF_SEARCH_Quit();

_EXPORT_ONVIF_SDK bool ONVIF_SEARCH_StartManual(); //手动发送一次搜索的指令
_EXPORT_ONVIF_SDK bool ONVIF_SEARCH_Set_Config(char* szServerAddr, const char *szXmlData);


class CONVIFAlarmStateObserver
{
public:
	CONVIFAlarmStateObserver(void){};
	virtual ~CONVIFAlarmStateObserver(void){};
	virtual void UpdateMotionState(tint32 devHandle, bool isAlarm) = 0;
	virtual void UpdateAlarmInState(tint32 devHandle, tint32 nSensorToken, bool isAlarm)=0;
};

_EXPORT_ONVIF_SDK bool ONVIF_CLIENT_Initial();
_EXPORT_ONVIF_SDK bool ONVIF_CLIENT_Quit();

const tuint32 DEV_HANDLE_NULL = 0Xffffffff;
//DevHandle:0xffffffff 未初始化， 返回值0xffffffff登陆失败。
_EXPORT_ONVIF_SDK tuint32 ONVIF_CLIENT_Login( tuint32 dwDevHandle, const char *addr,const tuint16 port,const char *pUser, const char *pPwd,tuint32 *pErr);
_EXPORT_ONVIF_SDK bool ONVIF_CLIENT_Logout( tuint32 dwDevHandle);

_EXPORT_ONVIF_SDK bool ONVIF_CLIENT_GetCfg(tuint32 dwDevHandle,const char *pszReqXml, char* &pszRespXml, tint32* pRespDataLen);
_EXPORT_ONVIF_SDK bool ONVIF_CLIENT_SetCfg(tuint32 dwDevHandle,const char *pszReqXml, char* &pszRespXml);
_EXPORT_ONVIF_SDK void ONVIF_CLIENT_DelResponseBuff(char *&pszRespXml);

_EXPORT_ONVIF_SDK bool ONVIF_CLIENT_AddAlarmStateObserver( tuint32 dwDevHandle, CONVIFAlarmStateObserver *pObserver);
_EXPORT_ONVIF_SDK void ONVIF_CLIENT_DelAlarmStateObserver( tuint32 dwDevHandle );
/*
class CRTSPReceiver
{
public:
	virtual tint32 StreamInput( NETNS::RTSP_FRAME &frameInfo, void *pFrameData) = 0;

	virtual void   LinkErr() = 0;
};

_EXPORT_ONVIF_SDK tint32 ONVIF_CLIENT_LIVESTREAM_Start( tint32 nDevHandle, tint32 nStreamIndex, NETNS::RTSP_NETPROTOCOL protocol, NETNS::RTSP_MEDIADATA_TYPE type, CRTSPReceiver *pStreamReceiver, tuint32* pErr);
_EXPORT_ONVIF_SDK bool   ONVIF_CLIENT_LIVESTREAM_Stop( tint32 nDevHandle, tint32 nSreamID);
*/
#endif