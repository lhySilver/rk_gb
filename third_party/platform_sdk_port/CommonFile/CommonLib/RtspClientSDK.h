#ifndef __RTSP_CLIENT_SDK_H__
#define __RTSP_CLIENT_SDK_H__


#include "base_type_define.h"
#include "DevDefine.h"
#include "WarningDisable.h"

#ifdef WIN32
#define strcasecmp  stricmp
#define strncasecmp strnicmp


#ifdef RTSPCLIENT_EXPORTS //¶¯Ì¬¿â±àÒë
#define _EXPORT_RTSP_CLIENT_SDK  __declspec(dllexport)
#else//Ê¹ÓÃÕß±àÒë
#define _EXPORT_RTSP_CLIENT_SDK __declspec(dllimport)
#endif

#else
#define _EXPORT_RTSP_CLIENT_SDK  __attribute__ ((visibility("default")))
#endif

enum RtspReqStatus
{
	RTSP_REQ_SUCCESS = 0,
	RTSP_REQ_EXIST,
	RTSP_REQ_FAILED,
    RTSP_REQ_UNSUPPORT,
    RTSP_REQ_UNAUTHORIZED
};

enum STREAM_TYPE
{
	STREAM_VIDEO		= 0,
	STREAM_AUDIO		= 1,
	STREAM_TALK			= 2,
	STREAM_VIDEO_AUDIO  = 3,
};

typedef enum st_PLAYBACK_MODE{
    FastForward = 1,
    FastForward_2  = 2,
    FastForward_4  = 4,
    FastForward_8  = 6,
    FastForward_16  = 16,
    FastForward_32  = 32,
    SlowForward_2  = -2,
    SlowForward_4  = -4,
    SlowForward_8  = -8,
    SlowForward_16  = -16,
    SlowForward_32  = -32,
}PLAYBACK_MODE;

const tuint32 RTSP_CLIENT_STREAMID_NULL = 0Xffffffff;

class CRTSPReceiver
{
public:
	virtual tint32 StreamInput( NETNS::RTSP_FRAME &frameInfo, void *pFrameData) = 0;

	virtual void   LinkErr() = 0;
};

class CRtspVirtualLoginObserver
{
public:
	virtual void   StateChange(bool state) = 0;
	virtual void   loginInfo(unsigned short iWidth, unsigned short iHeight) = 0;
};

_EXPORT_RTSP_CLIENT_SDK tuint32 RTSP_CLIENT_Start(const char *szUsername, const char *szPassword, CRTSPReceiver *pRtspReceiver, const char * szRtspUrl, tuint64 nStreamIndex, STREAM_TYPE nDataType, RtspReqStatus *pStatus , const  tuint64 nStartTime = 0,const PLAYBACK_MODE nPlayMode = FastForward);

//_EXPORT_RTSP_CLIENT_SDK tuint32 RTSP_CLIENT_Start_EX(const char *szUsername, const char *szPassword, CRTSPReceiver *pRtspReceiver, const char * szRtspUrl, tuint64 nStreamIndex, STREAM_TYPE nDataType, RtspReqStatus *pStatus ,const  tuint64 nStartTime,const PLAYBACK_MODE nPlayMode);

_EXPORT_RTSP_CLIENT_SDK bool    RTSP_CLIENT_Stop( tuint32 dwStreamID);

_EXPORT_RTSP_CLIENT_SDK bool    RTSP_CLIENT_InvokeHeartBeat( tuint32 dwStreamID);

_EXPORT_RTSP_CLIENT_SDK bool    RTSP_CLIENT_ImmediatelyPause( tuint32 dwStreamID);

_EXPORT_RTSP_CLIENT_SDK tuint32 RTSP_CLIENT_TalkbackInput( tuint32 dwStreamID, void* pFrameData, tuint32 dwFrameLen);

_EXPORT_RTSP_CLIENT_SDK void  RTSP_CLIENT_GetKeyFrame( tuint32 dwStreamID);

/*Virtual: 平台接入rtsp协议，rtsp做长连接时，使用option方法来模拟登录*/
_EXPORT_RTSP_CLIENT_SDK tuint32    RTSP_CLIENT_VirtualLogin(const char *szUsername, const char *szPassword, const char * szRtspUrl, tuint32& state, CRtspVirtualLoginObserver* pObserver);//发送option判断是否在线并且支持rtsp
_EXPORT_RTSP_CLIENT_SDK bool    RTSP_CLIENT_VirtualLoginOut(tuint32 dwSessionID);

///////////////////////////////////////
#endif
