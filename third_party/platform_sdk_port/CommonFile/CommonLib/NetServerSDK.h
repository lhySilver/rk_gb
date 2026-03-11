
#ifndef __NET_SERVER_SDK_H__
#define __NET_SERVER_SDK_H__
#include "base_type_define.h"
#include "DevInterface.h"

#ifdef WIN32
#define strcasecmp  stricmp
#define strncasecmp strnicmp


#ifdef NET_SERVER_SDK_EXPORT_API//动态库编译
#define _EXPORT_NET_SERVER_SDK  __declspec(dllexport)
#else//使用者编译
#define _EXPORT_NET_SERVER_SDK __declspec(dllimport)
#endif

#else
#define _EXPORT_NET_SERVER_SDK  __attribute__ ((visibility("default")))
#endif

enum NET_PROTOCOL_TYPE
{
	PROTOCOL_TVT_API    = 0x00000001,
	PROTOCOL_ONVIF      = 0x00000002,
};

class CStreamCtrl;
// 服务器端
_EXPORT_NET_SERVER_SDK bool	   NET_SERVER_Initial( tuint32 dwChnNum, tuint32 dwAlarmOutNum, tuint32 dwSensorNum, CDevInterface* pDevInterface, CStreamCtrl *pStreamCtrl, tuint32 nLoadProtocol = (PROTOCOL_TVT_API | PROTOCOL_ONVIF), NETNS::DEV_TYPE type = NETNS::DEVICE_IPC);
_EXPORT_NET_SERVER_SDK bool    NET_SERVER_Quit(); 
_EXPORT_NET_SERVER_SDK tuint32 NET_SERVER_GetVersion();
_EXPORT_NET_SERVER_SDK tuint32 NET_SERVER_GetOnvifVersion();
_EXPORT_NET_SERVER_SDK tuint32 NET_SERVER_GetAPIVersion();
_EXPORT_NET_SERVER_SDK tint32  NET_SERVER_Start(tuint16 nWebPort, const char* pWebRoot, const char* pConfigPath);
//启动默认无报警，有变化设置一次
_EXPORT_NET_SERVER_SDK tint32  NET_SERVER_SetMotionStatus(tuint32 dwChnID, bool isAlarm);
_EXPORT_NET_SERVER_SDK tint32  NET_SERVER_SetSensorStatus(tuint32 dwSensorID, bool isAlarm);
_EXPORT_NET_SERVER_SDK tint32  NET_SERVER_SetVideoLossStatus(tuint32 dwChnID, bool isAlarm);

_EXPORT_NET_SERVER_SDK tint32  NET_SERVER_RTSPStart( tuint16 wRtspPort, CStreamCtrl *pStreamCtrl, CDevInterface* pDevInterface);
_EXPORT_NET_SERVER_SDK tint32  NET_SERVER_RTSPStop( );
 //设置现场流的RTSP URL
_EXPORT_NET_SERVER_SDK tint32  NET_SERVER_SetLiveRTSPUrl(tuint32 dwChnID, tint32 streamIndex, const char*rtspurl);

_EXPORT_NET_SERVER_SDK tint32  NET_SERVER_LiveStreamIn( tuint32 dwChnID, tint32 streamIndex , const NETNS::RTSP_FRAME& frame, void* pData);
_EXPORT_NET_SERVER_SDK tint32  NET_SERVER_PlayStreamIn( tuint32 dwStreamID, const NETNS::RTSP_FRAME& frame, void* pData);
_EXPORT_NET_SERVER_SDK tint32  NET_SERVER_PlayStreamComplete( tuint32 dwStreamID);

class CStreamCtrl
{
public:
	CStreamCtrl(void) {}
	virtual ~CStreamCtrl(){}

	//打开现场流, -1(0xffffffff):失败，errCode存储错误原因，pUser存储了用户名 密码 IP地址
	virtual tuint32 OpenLiveStream(const char* pUser, tuint32 dwChnID ,tint32 streamIndex ,tuint32 &errCode) = 0;
	//现场即时关键帧
	virtual bool RequestKeyFrame(tuint32 dwChnID ,tint32 streamIndex) = 0;
	//关闭现场流streamID为OpenLiveStream的返回值
	virtual void CloseLiveStream( tuint32 streamID) = 0;

	//打开回放流, -1(0xffffffff):失败，errCode存储错误原因，pUser存储了用户名 密码 IP地址
	virtual tuint32 OpenPlayStream(const char* pUser, tuint32 dwChnID, const NETNS::DEV_DATE& recDate, const NETNS::DEV_TIME& recTime, tuint32 dwTimeLen ,tuint32 &errCode) = 0;
	//关闭回放流streamID为OpenPlayStream的返回值
	virtual void ClosePlayStream( tuint32 streamID) = 0;
};

#endif