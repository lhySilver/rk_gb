#ifndef _RTSP_SDK_H
#define _RTSP_SDK_H

#ifdef WIN32

#ifdef RTSP_SDK_EXPORT_API//动态库编译
#define RTSP_SDK  __declspec(dllexport)
#define RTSP_CLASS  __declspec(dllexport)
#else
#ifdef RTSP_SDK_EXPORT_STATIC//静态库编译和使用
#define RTSP_SDK
#define RTSP_CLASS
#else//使用者编译
#define RTSP_SDK  __declspec(dllimport)
#define RTSP_CLASS  __declspec(dllimport)
#endif
#endif

#else
#define RTSP_SDK __attribute__ ((visibility("default")))
#define RTSP_CLASS __attribute__ ((visibility("default")))
#endif

#include "base_type_define.h"  
#include "N_SDKCommonObserver.h"
#include "DevDefine.h"

class CRtspSreamCtrl
{
public:
	CRtspSreamCtrl(void) {}
	virtual ~CRtspSreamCtrl(){}

	//打开流
	virtual tuint32 OpenLiveStream( const char* pUser, const char *szUrlParam, tuint32 uLen, tuint32 &errCode) = 0;
	virtual tuint32 OpenPlayStream( const char* pUser, const char *szUrlParam, tuint32 uLen, tuint32 &errCode) = 0;
	virtual tuint32 OpenTalkbackStream( const char* pUser, tuint32 &errCode) = 0;
	
    virtual void StreamRun(tuint32 dwStreamID) = 0;

	virtual bool OpenAudio(tuint32 dwStreamID) = 0;
	virtual void CloseAudio(tuint32 dwStreamID) = 0;
	//关闭流
	virtual void CloseLiveStream(tuint32 dwStreamID) = 0;
	virtual void ClosePlayStream(tuint32 dwStreamID) = 0;
	virtual void CloseTalkbackStream( tuint32 dwStreamID) = 0;
    virtual bool Rewind(tuint32 StreamID, const FILETIME &ftTime) = 0;
	virtual bool CancelRewind(tuint32 StreamID, const FILETIME &ftTime) = 0;
	
	virtual bool RefreshPlay(tuint32 StreamID, tuint32 frameIndex) = 0;

	virtual void StreamInput(tuint32 dwStreamID,  tuint32 hAudioFrame, tint32 nAudioLen) = 0;

    virtual void GetUserInfo( NETNS::DEV_USER_INFO *pCfg, tuint32 nSize, tuint32& nNeedSize) = 0;
protected:
};

RTSP_SDK bool RTSP_Init(CRtspSreamCtrl *rtspStreamCtrl = NULL);//初始化RTSP,如果传NULL就不会支持RTSP流服务
RTSP_SDK void RTSP_Quit(void);//销毁RTSP 

RTSP_SDK bool RTSP_Start(tuint16 usRtspPort);//
RTSP_SDK void RTSP_Stop(void);//停止RTSP 
RTSP_SDK void RTSP_AnonymousAccess( bool bAccess);//rtsp匿名访问接口。

RTSP_SDK tint32 RTSP_GetClientInfo( char* pOutClient, tint32 nBufferLen, tint32& nNeedBufferLen);
	   
RTSP_SDK tuint32 RTSP_CreatRtspStream(const char *szXMLParam, tuint32 uLen, CStreamCaptureObserver *pStreamObserver); //xml包括rul和访问的类型（UDP、TCP、BROADCASK）	 
RTSP_SDK void DestroyStream(tuint32 dwTaskID);  

/////////执行创建的任务
RTSP_SDK bool ExecuteTask(tuint32 dwTaskID);

///通过这个函数接收数据流 CStreamCaptureObserver 

//////音视频数据输入
RTSP_SDK bool RTSP_StreamInput(tuint32 dwStreamID, const char *pBuf, tuint32 nLen);	//输入流数据	NVMS私有格式

#endif //_RTSP_SDK_H
