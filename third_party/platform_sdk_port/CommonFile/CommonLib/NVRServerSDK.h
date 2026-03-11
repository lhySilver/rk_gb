
/************************************************************************/
/* 带流媒体和存储功能的SDK                                                                     */
/************************************************************************/
///////////////////////////NVRServerSDK

#ifndef _NVR_SERVER_SDK_H
#define _NVR_SERVER_SDK_H

#ifdef WIN32
#ifdef   NVR_SERVER_SDK_EXPORT_API//动态库编译
#define _EXPORT_NVRSERVERSDK_CLASS __declspec(dllexport)
#define _EXPORT_NVRSERVERSDK_API  __declspec(dllexport)
#else//使用者编译
#define _EXPORT_NVRSERVERSDK_CLASS __declspec(dllimport)
#define _EXPORT_NVRSERVERSDK_API  __declspec(dllimport)
#endif

#else
#define _EXPORT_NVRSERVERSDK_CLASS __attribute__ ((visibility("default")))
#define _EXPORT_NVRSERVERSDK_API  __attribute__ ((visibility("default")))
#endif

#include "base_type_define.h"
#include "BufferData.h"
#include "FrameData.h"
#include "LoginStateObserver.h"
#include "Interlocked.h"
#include "N_SDKCommonObserver.h"
#include "GB28181Def_Base.h"
#include<list>
#include<map>
#include<string>
//很多功能函数都使用了以下这个通知模式，在初始化时将自己加入观察者队列，退出时移除
//执行操作时返回一个唯一的ID，这个ID在观察者回调函数里面用来判断是不是本次操作
//没有使用在调用函数时传入观察者的原因是因为可能存在操作返回时对象已经被销毁的情况


///////////////////////////////以下内容转移到 "N_SDKCommonObserver.h"里面去了
//全部阻塞函数返回CBufferData*指针的，都需要调用者自己Dec一次，失败时会返回NULL,错误原因通过NVRServer_SDK_GetLastError获得，_ErrorDefine.h里面定义
// class CWaitObserver
// {
// public:
// 	CWaitObserver(void){};
// public:
// 	virtual ~CWaitObserver(void){};
// 
// 	//几乎所有的函数都具有操作对象，destID就是操作对象的ID
// 	virtual void OnWaitForResult(tuint32 optID,tuint32 destID,bool isOK,const void *pFrameData, tuint32 frameDataLen,LPVOID lParam)=0;
// };
// 
// class CStreamCaptureObserver
// {
// public:
// 	CStreamCaptureObserver(void){};
// public:
// 	virtual ~CStreamCaptureObserver(void){};
// 
// 	/////////////////////////////////////////
// 	virtual void StreamCapture(const tuint32 streamID, const char *pBuf,tuint32 iLen,tuint32 errorCode)=0;
// };

//通道名字变更后回通过这个观察者通知上来
class CDevChannelNameUpdateObserver
{
public:
	CDevChannelNameUpdateObserver(void){};
public:
	virtual ~CDevChannelNameUpdateObserver(void){};

	/////////////////////////////////////////
	virtual void ChannelNameUpdateObserver(const tuint32 nodeID,char *pChName)=0;//可以用CString直接赋值
};

//sensor名称变更后会通过这个观察者通知上来
class CDevSensorlNameUpdateObserver
{
public:
	CDevSensorlNameUpdateObserver(void){};
public:
	virtual ~CDevSensorlNameUpdateObserver(void){};

	/////////////////////////////////////////
	virtual void SensorNameUpdateObserver(const tuint32 nodeID,char *pChName)=0;//可以用CString直接赋值
};

//sensor名称变更后会通过这个观察者通知上来
class CDevAlarmOutNameUpdateObserver
{
public:
	CDevAlarmOutNameUpdateObserver(void){};
public:
	virtual ~CDevAlarmOutNameUpdateObserver(void){};

	/////////////////////////////////////////
	virtual void AlarmOutNameUpdateObserver(const tuint32 nodeID,char *pChName)=0;//可以用CString直接赋值
};

//设备名字变更后回通过这个观察者通知上来
class CDeviceNameUpdateObserver
{
public:
	CDeviceNameUpdateObserver(void){};
public:
	virtual ~CDeviceNameUpdateObserver(void){};

	/////////////////////////////////////////
	virtual void DeviceNameUpdateObserver(const tuint32 nodeID,char *pDeviceName)=0;//可以用CString直接赋值
};
//附加信息变更后会通过这个观察者通知上来
class CNodeAppendInfoUpdateObserver
{
public:
	CNodeAppendInfoUpdateObserver(void){};
public:
	virtual ~CNodeAppendInfoUpdateObserver(void){};

	/////////////////////////////////////////
	virtual void NodeAppendInfoUpdateObserver(const tuint32 nodeID,const char *pInfo)=0;//可以用CString直接赋值
}; 

// 通道国标ID变更后会通过这个观察者通知上来
class CNodeGB28181IDUpdateObserver
{
public:
	CNodeGB28181IDUpdateObserver(void){};
public:
	virtual ~CNodeGB28181IDUpdateObserver(void){};

	/////////////////////////////////////////
	virtual void NodeGB28181IDUpdateObserver(const tuint32 nodeID,const char *pInfo)=0;//可以用CString直接赋值
}; 

//本地与网络端对讲的接口
class CTalkbackCtrlObserver
{
public:
	CTalkbackCtrlObserver(void){;}
	virtual ~CTalkbackCtrlObserver(void){;}

	/////////////////////////////////////////
    virtual bool OpenTalkback(const GUID &netNodeGUID, const GUID &TaskGUID, tuint32 &dwErrorCode)=0;//
    virtual void CloseTalkback(const GUID &netNodeGUID, const GUID &TaskGUID)=0;//
}; 

//网络模块的初始化和销毁
_EXPORT_NVRSERVERSDK_API bool NVRServer_SDK_Initial(tuint32 localNodeType, Interlocked *pGlobalNodeIDBase, const char *szExeFilePath/*路径最后带斜杠*/, tuint16 custorNO=0, \
                                                    CTalkbackCtrlObserver *pTalkbackCtrlObserver=NULL, const char* pBufForXMLData=NULL , \
													const std::string &strDBFile="nvms_systemdb",const std::string &strDBAddress="127.0.0.1",\
													tuint32 nDBPort=3306, const std::string &strDBUser="root", const std::string &strDBPassword="123456", tuint32 nPlatformType=0);///tuint16 custorNO=0留给客户订制的参数

_EXPORT_NVRSERVERSDK_API void NVRServer_SDK_Quit();
_EXPORT_NVRSERVERSDK_API bool NVRServer_SDK_Start();
_EXPORT_NVRSERVERSDK_API void NVRServer_SDK_Stop();
_EXPORT_NVRSERVERSDK_API bool NVRServer_SDK_Ready();
_EXPORT_NVRSERVERSDK_API void NVRServer_SDK_UnReady();
_EXPORT_NVRSERVERSDK_API tuint32 NVRServer_SDK_GetLastError(); //返回_ErrorCodeDef.h里面的值

//初始化国标配置文件
_EXPORT_NVRSERVERSDK_API bool NVRServer_SDK_SetGBConfigInfo(const tagXmlConfig &info);


_EXPORT_NVRSERVERSDK_API int NVRServer_SDK_GetNodeConnectTimes(tuint32 nodeID);

_EXPORT_NVRSERVERSDK_API void NVRServer_SDK_AddLiveStreamObserver(CStreamCaptureObserver *pStreamObserver);//接收帧数据的观察者
_EXPORT_NVRSERVERSDK_API void NVRServer_SDK_DelLiveStreamObserver(CStreamCaptureObserver *pStreamObserver);//接收帧数据的观察者

_EXPORT_NVRSERVERSDK_API void NVRServer_SDK_AddPlaybackStreamObserver(CStreamCaptureObserver *pStreamObserver);//回放帧数据的观察者一般是回放
_EXPORT_NVRSERVERSDK_API void NVRServer_SDK_DelPlaybackStreamObserver(CStreamCaptureObserver *pStreamObserver);//回放帧数据的观察者

_EXPORT_NVRSERVERSDK_API void NVRServer_SDK_AddTalkbackStreamObserver(CStreamCaptureObserver *pStreamObserver);//对讲帧数据的观察者
_EXPORT_NVRSERVERSDK_API void NVRServer_SDK_DelTalkbackStreamObserver(CStreamCaptureObserver *pStreamObserver);//对讲帧数据的观察者

_EXPORT_NVRSERVERSDK_API void NVRServer_SDK_AddChannelNameChangeObserver(CDevChannelNameUpdateObserver *pObserver);//通道名字变更的观察者
_EXPORT_NVRSERVERSDK_API void NVRServer_SDK_DelChannelNameChangeObserver(CDevChannelNameUpdateObserver *pObserver);//通道名字变更的观察者

_EXPORT_NVRSERVERSDK_API void NVRServer_SDK_AddNodeAppendInfoChangeObserver(CNodeAppendInfoUpdateObserver *pObserver);//通道流变更的观察者
_EXPORT_NVRSERVERSDK_API void NVRServer_SDK_DelNodeAppendInfoChangeObserver(CNodeAppendInfoUpdateObserver *pObserver);//通道流变更的观察者

////////////////////////////////////////////////////现场视频+音频流的相关接口
//打开网络流
_EXPORT_NVRSERVERSDK_API tuint32 NVRServer_SDK_RequestOpenLiveStream(tuint32 dwNodeID, tuint32 streamType, LPVOID userPara, CWaitObserver *pWaitObserver, CStreamCaptureObserver *pStreamObserver);

_EXPORT_NVRSERVERSDK_API tuint32 NVRServer_SDK_RequestOpenLiveStream(tuint32 chID,tuint32 streamType,LPVOID userPara,CWaitObserver *pWaitObserver=NULL);
_EXPORT_NVRSERVERSDK_API tuint32 NVRServer_SDK_RequestOpenTalkStream(tuint32 devID,LPVOID userPara,CWaitObserver *pWaitObserver=NULL);
_EXPORT_NVRSERVERSDK_API tuint32 NVRServer_SDK_RequestOpenBroadcastStream(tuint32 devID,LPVOID userPara,CWaitObserver *pWaitObserver=NULL);

//关闭网络流
_EXPORT_NVRSERVERSDK_API bool NVRServer_SDK_RequestCloseLiveStream(tuint32 streamID,bool waitResult);
_EXPORT_NVRSERVERSDK_API bool NVRServer_SDK_RequestCloseTalkStream(tuint32 streamID,bool waitResult);
_EXPORT_NVRSERVERSDK_API bool NVRServer_SDK_RequestCloseBroadcastStream(tuint32 streamID,bool waitResult);

//发送对讲数据
_EXPORT_NVRSERVERSDK_API bool  NVRServer_SDK_SendTalkStreamData(tuint32 devID,const void* pBuf,const tuint32 iLen);
//发送对讲数据
_EXPORT_NVRSERVERSDK_API bool  NVRServer_SDK_SendTalkStreamData(const GUID &netNodeGUID,const void* pBuf,const tuint32 iLen);
_EXPORT_NVRSERVERSDK_API bool  NVRServer_SDK_TalkComplete(const GUID &NetNodeGUID, const GUID &TaskGUID);


////////////////////READ_RECORD_DEVICE指设备上的数据，  READ_RECORD_STORAGE指存储服务器上的数据，  READ_RECORD_LOCAL指系统本地的录像数据提供给OCX调用的时候使用   
_EXPORT_NVRSERVERSDK_API bool  NVRServer_SDK_PlaybackControlPlay(tuint32 chID, tuint32 eventMask,tuint32 startime,tuint32 endTime,READ_RECORD_POSITION recordDataPosition);//存储服务器和设备回放
_EXPORT_NVRSERVERSDK_API tuint32 NVRServer_SDK_PlaybackControlPlay(tuint32 chID, tuint32 eventMask,tuint32 startime,tuint32 endTime,LPVOID userPara,READ_RECORD_POSITION recordDataPosition,CWaitObserver *pWaitObserver=NULL);//存储服务器和设备回放

//多通道回放，必须是同一个设备的
_EXPORT_NVRSERVERSDK_API bool  NVRServer_SDK_PlaybackControlPlayMultiCh(int chCount,tuint32 *pChID, tuint32 eventMask,tuint32 startime,tuint32 endTime);
_EXPORT_NVRSERVERSDK_API tuint32 NVRServer_SDK_PlaybackControlPlayMultiCh(int chCount,tuint32 *pChID, tuint32 eventMask,tuint32 startime,tuint32 endTime,LPVOID userPara,CWaitObserver *pWaitObserver=NULL);

_EXPORT_NVRSERVERSDK_API bool NVRServer_SDK_PlaybackRequestEvent(tuint32 chID, tuint32 eventMask,tuint32 startime,tuint32 endTime,READ_RECORD_POSITION recordDataPosition,CBufferData* &pBuffData);//存储服务器和设备回放
_EXPORT_NVRSERVERSDK_API tuint32 NVRServer_SDK_PlaybackRequestEvent(tuint32 chID,tuint32 eventMask,tuint32 startime,tuint32 endTime,LPVOID userPara,READ_RECORD_POSITION recordDataPosition,CWaitObserver *pWaitObserver=NULL);//存储服务器和设备回放

//回放控制
_EXPORT_NVRSERVERSDK_API bool  NVRServer_SDK_PlaybackControlStop(tuint32 streamID,bool waitResult);//根据流ID停止回放，内部会知道是设备还是存储服务器
_EXPORT_NVRSERVERSDK_API bool  NVRServer_SDK_PlaybackControlPause(tuint32 streamID,bool waitResult);//根据流ID停止回放，内部会知道是设备还是存储服务器
_EXPORT_NVRSERVERSDK_API bool  NVRServer_SDK_PlaybackControlResume(tuint32 streamID,bool waitResult);//根据流ID停止回放，内部会知道是设备还是存储服务器
_EXPORT_NVRSERVERSDK_API bool  NVRServer_SDK_OpenOnlyKeyFrame(tuint32 streamID, const FILETIME &ftFrameTime);//开始只回放关键帧
_EXPORT_NVRSERVERSDK_API bool  NVRServer_SDK_CloseOnlyKeyFrame(tuint32 streamID, const FILETIME &ftFrameTime);//关闭只回放关键帧

_EXPORT_NVRSERVERSDK_API bool  NVRServer_SDK_PlaybackControlSendFrameIndex(tuint32 streamID,tuint32 frameIndex);//根据流ID停止回放，内部会知道是设备还是存储服务器

//////备份
_EXPORT_NVRSERVERSDK_API bool NVRServer_SDK_RecDataBackupStart(tuint32 chID, tuint32 eventMask,tuint32 startime,tuint32 endTime,READ_RECORD_POSITION recordDataPosition);//录像数据备份
_EXPORT_NVRSERVERSDK_API tuint32 NVRServer_SDK_RecDataBackupStart(tuint32 chID, tuint32 eventMask,tuint32 startime,tuint32 endTime,LPVOID userPara,READ_RECORD_POSITION recordDataPosition,CWaitObserver *pWaitObserver/*=NULL*/);//录像数据备份
_EXPORT_NVRSERVERSDK_API bool  NVRServer_SDK_RecDataBackupStop(tuint32 streamID,bool waitResult);//备份停止


_EXPORT_NVRSERVERSDK_API bool  NVRServer_SDK_StartAutoReport(tuint16 port);
_EXPORT_NVRSERVERSDK_API void  NVRServer_SDK_StopAutoReport();
_EXPORT_NVRSERVERSDK_API bool  NVRServer_SDK_SetAutoHuaanReport(tuint16 port);


//开始数据端口监听
_EXPORT_NVRSERVERSDK_API bool  NVRServer_SDK_StartDataServer(tuint16 port, tuint16 p2pPort, const char *pSN);
_EXPORT_NVRSERVERSDK_API void  NVRServer_SDK_StopDataServer();

//开始/停止自动连接
_EXPORT_NVRSERVERSDK_API bool  NVRServer_SDK_StartAutoConnect();
_EXPORT_NVRSERVERSDK_API void  NVRServer_SDK_StopAutoConnect();

//增加全局观察者，在函数不传入自己的观察者有时非阻塞模式时会通知到全部全局观察者，以适应上层应用的需要
_EXPORT_NVRSERVERSDK_API bool  NVRServer_SDK_AddWaitForResultObserver(CWaitObserver *pWaitObserver);
_EXPORT_NVRSERVERSDK_API bool  NVRServer_SDK_DeleteWaitForResultObserver(CWaitObserver *pWaitObserver);

_EXPORT_NVRSERVERSDK_API void  NVRServer_SDK_Run(tuint32 optID);//投递非阻塞模式操作，参数为各非阻塞模式函数返回值

//成功后在CBufferData含有通道数量，报警数据数量，报警输出数量，设备类型等信息，以XML方式解析，用完要dec,失败返回NULL，GetLastError得到错误原因
_EXPORT_NVRSERVERSDK_API bool NVRServer_SDK_TestLoginDev(tuint32 devType,char *pDevIP,tuint16 devPort,char *pDevUser,char *pDevPwd,char *pUrl,CBufferData* &pFrameData);
_EXPORT_NVRSERVERSDK_API void NVRServer_SDK_TestLoginDev(tuint32 devType,char *pDevIP,tuint16 devPort,char *pDevUser,char *pDevPwd,char *pUrl,LPVOID userPara,CWaitObserver *pWaitObserver);

//等到结果后返回，且自己要对返回值Dec
_EXPORT_NVRSERVERSDK_API bool  NVRServer_SDK_RequestDeviceDiskInfo(tuint32 devID,CBufferData* &pFrameData);
//等到结果后pWaitObserver->OnWaitForResult(函数返回值ID,userPara)，函数返回0表示不成功一般是找不到节点或节点不在线之类的
_EXPORT_NVRSERVERSDK_API tuint32 NVRServer_SDK_RequestDeviceDiskInfo(tuint32 devID,LPVOID userPara,CWaitObserver *pWaitObserver=NULL);

//等到结果后返回，且自己要对返回值Dec
_EXPORT_NVRSERVERSDK_API bool  NVRServer_SDK_RequestDeviceOnlineUsers(tuint32 devID,CBufferData* &pFrameData);
//等到结果后pWaitObserver->OnWaitForResult(函数返回值ID,userPara)，函数返回0表示不成功一般是找不到节点或节点不在线之类的
_EXPORT_NVRSERVERSDK_API tuint32 NVRServer_SDK_RequestDeviceOnlineUsers(tuint32 devID,LPVOID userPara,CWaitObserver *pWaitObserver=NULL);

//等到结果后返回，且自己要对返回值Dec
_EXPORT_NVRSERVERSDK_API bool  NVRServer_SDK_RequestDeviceLog(tuint32 devID,tuint32 dwStartTime,tuint32 dwEndTime,CBufferData* &pFrameData);
//等到结果后pWaitObserver->OnWaitForResult(函数返回值ID,userPara)，函数返回0表示不成功一般是找不到节点或节点不在线之类的
_EXPORT_NVRSERVERSDK_API tuint32 NVRServer_SDK_RequestDeviceLog(tuint32 devID,tuint32 dwStartTime,tuint32 dwEndTime,LPVOID userPara,CWaitObserver *pWaitObserver=NULL);

//手动控制报警输出
_EXPORT_NVRSERVERSDK_API bool NVRServer_SDK_SetDevAlarmOut(tuint32 devID,int alarmOutNO,bool isOn);
_EXPORT_NVRSERVERSDK_API tuint32 NVRServer_SDK_SetDevAlarmOut(bool waitResult,tuint32 devID,int alarmOutNO,bool isOn,LPVOID waitLPara,CWaitObserver *pWaitObserver);

_EXPORT_NVRSERVERSDK_API bool  NVRServer_SDK_AFControl(tuint32 chID, tuint32 afCtrlType, tuint32 nValue);///////NVMS_AF_CTRL_TYPE      NVMS_AF_CONTROL
_EXPORT_NVRSERVERSDK_API tuint32  NVRServer_SDK_AFControl(tuint32 chID, tuint32 afCtrlType, tuint32 nValue, LPVOID userPara, CWaitObserver *pWaitObserver);//////NVMS_AF_CTRL_TYPE      NVMS_AF_CONTROL

//云台控制
_EXPORT_NVRSERVERSDK_API bool  NVRServer_SDK_PTZMove(tuint32 chID,tuint32 opt,tuint32 speed);//opt为ECMS_PTZ_MOVE里面的值
_EXPORT_NVRSERVERSDK_API tuint32  NVRServer_SDK_PTZMove(tuint32 chID,tuint32 opt,tuint32 speed,LPVOID userPara,CWaitObserver *pWaitObserver=NULL);//opt为ECMS_PTZ_MOVE里面的值
_EXPORT_NVRSERVERSDK_API bool  NVRServer_SDK_PTZControl(tuint32 chID,tuint32 opt,tuint32 speed,tuint32 dwParam);//opt为ECMS_PTZ_CONTROL里面的值
_EXPORT_NVRSERVERSDK_API tuint32  NVRServer_SDK_PTZControl(tuint32 chID,tuint32 opt,tuint32 speed,tuint32 dwParam,LPVOID userPara,CWaitObserver *pWaitObserver=NULL);//opt为ECMS_PTZ_CONTROL里面的值


_EXPORT_NVRSERVERSDK_API bool  NVRServer_SDK_PTZ3DControl(tuint32 chID, tint32 dwX, tint32 dwY, tint16 zoom);//
_EXPORT_NVRSERVERSDK_API tuint32  NVRServer_SDK_PTZ3DControl(tuint32 chID, tint32 dwX, tint32 dwY, tint16 zoom, LPVOID userPara, CWaitObserver *pWaitObserver=NULL);//opt为ECMS_PTZ_CONTROL里面的值

//预置点和巡航线信息
_EXPORT_NVRSERVERSDK_API bool  NVRServer_SDK_PTZGetPresetInfo(tuint32 chID,CBufferData* &pFrameData);
_EXPORT_NVRSERVERSDK_API tuint32 NVRServer_SDK_PTZGetPresetInfo(tuint32 chID,LPVOID userPara,CWaitObserver *pWaitObserver=NULL);

_EXPORT_NVRSERVERSDK_API bool  NVRServer_SDK_PTZGetCruiseInfo(tuint32 chID,CBufferData* &pFrameData);
_EXPORT_NVRSERVERSDK_API tuint32 NVRServer_SDK_PTZGetCruiseInfo(tuint32 chID,LPVOID userPara,CWaitObserver *pWaitObserver=NULL);

_EXPORT_NVRSERVERSDK_API bool  NVRServer_SDK_GetJPEGPicture(tuint32 chID,CBufferData* &pFrameData);
_EXPORT_NVRSERVERSDK_API tuint32 NVRServer_SDK_GetJPEGPicture(tuint32 chID,LPVOID userPara,CWaitObserver *pWaitObserver=NULL);

//获取设备配置
_EXPORT_NVRSERVERSDK_API bool  NVRServer_SDK_RequestDeviceCfg(tuint32 devID,const char *pSendBuf, const int iLen,CBufferData* &pFrameData);
_EXPORT_NVRSERVERSDK_API tuint32 NVRServer_SDK_RequestDeviceCfg(tuint32 devID,const char *pSendBuf, const int iLen,LPVOID userPara,CWaitObserver *pWaitObserver=NULL);

//保存设备配置
_EXPORT_NVRSERVERSDK_API bool  NVRServer_SDK_RequestSaveDeviceCfg(tuint32 devID,const char *pSendBuf, const int iLen);
_EXPORT_NVRSERVERSDK_API tuint32 NVRServer_SDK_RequestSaveDeviceCfg(tuint32 devID,const char *pSendBuf, const int iLen,LPVOID userPara,CWaitObserver *pWaitObserver=NULL);

//重启设备
_EXPORT_NVRSERVERSDK_API bool  NVRServer_SDK_RequestRebootDevice(tuint32 devID,CBufferData* &pFrameData);
_EXPORT_NVRSERVERSDK_API tuint32 NVRServer_SDK_RequestRebootDevice(tuint32 devID,LPVOID userPara,CWaitObserver *pWaitObserver=NULL);

//恢复设备默认配置
_EXPORT_NVRSERVERSDK_API bool  NVRServer_SDK_RequestDeviceRestoreDefault(tuint32 devID,CBufferData* &pFrameData);
_EXPORT_NVRSERVERSDK_API tuint32 NVRServer_SDK_RequestDeviceRestoreDefault(tuint32 devID,LPVOID userPara,CWaitObserver *pWaitObserver=NULL);

_EXPORT_NVRSERVERSDK_API bool  NVRServer_SDK_SetChannelColor(tuint32 chID,long bright,long contrast,long hue,long saturation);//opt为ECMS_PTZ_CONTROL里面的值

_EXPORT_NVRSERVERSDK_API bool  NVRServer_SDK_SetDevTime(tuint32 devID,tuint16 year,tuint16 month,tuint16 day,tuint16 hour,tuint16 minute,tuint16 second);
_EXPORT_NVRSERVERSDK_API tuint32  NVRServer_SDK_SetDevTime(tuint32 devID,tuint16 year,tuint16 month,tuint16 day,tuint16 hour,tuint16 minute,tuint16 second, LPVOID userPara, CWaitObserver *pWaitObserver = NULL);
//同步IPC时间，在每次往IPC发送命令之前取当前系统时间进行同步
_EXPORT_NVRSERVERSDK_API tuint32 NVRServer_SDK_SetDevTime(tuint32 devID, LPVOID userPara, CWaitObserver *pWaitObserver = NULL);

_EXPORT_NVRSERVERSDK_API bool  NVRServer_SDK_SearchWifi(tuint32 devID,CBufferData* &pFrameData);
_EXPORT_NVRSERVERSDK_API tuint32  NVRServer_SDK_SearchWifi(tuint32 devID,LPVOID userPara,CWaitObserver *pWaitObserver=NULL);

//透明传输数据到设备		   
_EXPORT_NVRSERVERSDK_API bool  NVRServer_SDK_RequestTransport(tuint32 nodeID,const char *pSendBuf, const int iLen,CBufferData* &pFrameData);
_EXPORT_NVRSERVERSDK_API tuint32 NVRServer_SDK_RequestTransport(tuint32 nodeID,const char *pSendBuf, const int iLen,LPVOID userPara,CWaitObserver *pWaitObserver=NULL);

_EXPORT_NVRSERVERSDK_API bool  NVRServer_SDK_GetLanDevList(std::list<ECMS_IPTOOL_SEND_DEV_INFO>& devlist/*主动*/,std::list<ECMS_IPTOOL_SEND_DEV_INFO>& autoReportDevlist/*被动*/);
_EXPORT_NVRSERVERSDK_API bool  NVRServer_SDK_GetStateMap(std::map<GUID,SYSTEM_RUNSTATE_INFO>& mapServerState,\
		std::map<GUID,SYSTEM_RUNSTATE_INFO>& mapPUState,std::map<GUID,SYSTEM_RUNSTATE_INFO>& mapDecoderState,\
		std::map<GUID,SYSTEM_RUNSTATE_INFO>& mapAlarmHostState,std::map<GUID,SYSTEM_RUNSTATE_INFO>& mapRecordState,\
		std::map<GUID,SYSTEM_RUNSTATE_INFO>& mapChannelState,std::map<GUID,SYSTEM_RUNSTATE_INFO>& mapAcsSystemState);
/*查询系统状态日志*/
_EXPORT_NVRSERVERSDK_API tuint32 NVRServer_SDK_LOG_CreateQueryStateLog(tuint64 dwLogTypeMask, tuint32 dwStartTime, tuint32 dwEndTime);
_EXPORT_NVRSERVERSDK_API tuint32 NVRServer_SDK_LOG_GetStateLogTotalCount(tuint32 handle);
//从第startIndex条开始取count条日志，startIndex从1开始
_EXPORT_NVRSERVERSDK_API tuint32 NVRServer_SDK_LOG_GetStateLog(tuint32 handle, tuint32 startIndex, tuint32 count, char *&pBuf);
_EXPORT_NVRSERVERSDK_API bool NVRServer_SDK_LOG_DestroyQueryStateLog(tuint32 handle);
//删除dwTime这个时间点之前的日志
_EXPORT_NVRSERVERSDK_API bool NVRServer_SDK_LOG_RemoveHistoryStateLog(tuint32 dwTime);
/*查询系统状态日志*/

//写配置日志到数据库
_EXPORT_NVRSERVERSDK_API bool NVRServer_SDK_LOG_AddConfigLogRecord(tuint64 dwLogID, const GUID &UserGUID, const char *szUserName, tuint32 clientType, const GUID &OperateNodeGUID, const char *pContent);
//写操作日志到数据库
_EXPORT_NVRSERVERSDK_API bool NVRServer_SDK_LOG_AddOperateLogRecord(tuint64 dwLogID, const GUID &UserGUID, const char *szUserName, tuint32 clientType, const GUID &OperateNodeGUID, tuint32 dwOcrTime, const GUID *pLinkGUID, int nGUIDNumber, const char *pContent);

//获取局域网内空闲的服务
_EXPORT_NVRSERVERSDK_API bool  NVRServer_SDK_GetLanServerMap(std::map< GUID,std::list<LAN_SERVER> >& lanServerMap);

//同步当前服务器的IP到指定服务器,现在主要用于管理服务器电脑改完IP地址后将自己的IP发送给电视墙服务器，电视墙服务器校验一个能连上的IP发送给解码器
_EXPORT_NVRSERVERSDK_API bool  NVRServer_SDK_SynIPList(const char * ipList);

//////////////////////////////////////////////////////////////////////////
//                      智能分析业务， 全部采用异步方式
//////////////////////////////////////////////////////////////////////////
// 获取智能事件状态， CMD_REQUEST_GET_SMART_STATE
_EXPORT_NVRSERVERSDK_API tuint32 NVRServer_SDK_GetSmartEventState(tuint32 chnlID, LPVOID para, tint32 paraLen, CWaitObserver* observer = NULL);

// 订阅消息 CMD_REQUEST_SMART_SUBSCRIBE， para中附加topic类型
_EXPORT_NVRSERVERSDK_API tuint32 NVRServer_SDK_SubSmartTopic(tuint32 chnlID, LPVOID para, tint32 paraLen, CWaitObserver* observer = NULL);

// 退定消息 CMD_REQUEST_SMART_UNSUBSCRIBE
_EXPORT_NVRSERVERSDK_API tuint32 NVRServer_SDK_UnsubSmartTopic(tuint32 chnlID, LPVOID para, tint32 paraLen, CWaitObserver* observer = NULL);

// 订阅消息 CMD_REQUEST_SMART_SUBSCRIBE， para中附加topic类型
_EXPORT_NVRSERVERSDK_API tuint32 NVRServer_SDK_SubVehicleTopic(tuint32 chnlID, LPVOID para, tint32 paraLen, CWaitObserver* observer = NULL);

// 退定消息 CMD_REQUEST_SMART_UNSUBSCRIBE
_EXPORT_NVRSERVERSDK_API tuint32 NVRServer_SDK_UnsubVehicleTopic(tuint32 chnlID, LPVOID para, tint32 paraLen, CWaitObserver* observer = NULL);

//订阅消息 订阅区域入侵检测
_EXPORT_NVRSERVERSDK_API tuint32 NVRServer_SDK_SubPermiterTopic(tuint32 chnlID, LPVOID para, tint32 paraLen, CWaitObserver* observer = NULL);
//订阅消息 退订订阅区域入侵检测
_EXPORT_NVRSERVERSDK_API tuint32 NVRServer_SDK_UnsubPermiterTopic(tuint32 chnlID, LPVOID para, tint32 paraLen, CWaitObserver* observer = NULL);
//////////////////////////////////////////////////////////////////////////
// 智能分析服务器向转发服务器数据请求指令
// 参数说明：
//	devID : NVR节点ID
//  cmdCode: 命令字，间ClientIAProtocol中定义
//  pSendBuf： 命令字所对应的数据序列化之后的数据
//	iLen: 命令字所对应的数据序列化之后的数据长度
//	waitLPara：接收结果的指针，void* ptr = NULL， 将ptr按照引用方式传入，获取结果时，需要转换为CBufferData
//	pWaitObserver: 异步模式使用，智能分析服务器与转发服务器之间关于NVR的数据交互按照同步逻辑处理， 故此处默认值即可
_EXPORT_NVRSERVERSDK_API tuint32 NVRServer_SDK_Ans_Request_Mdu_Cmd(tuint32 devID, tuint32 cmdCode, const char *pSendBuf, const int iLen, LPVOID& waitLPara, CWaitObserver *pWaitObserver = NULL);

// 智能分析服务器向管理服务器发送数据通知
// 参数说明：
//  cmdCode: 命令字，间ClientIAProtocol中定义
//  pSendBuf： 命令字所对应的数据序列化之后的数据
//	iLen: 命令字所对应的数据序列化之后的数据长度
//	waitLPara：接收结果的指针，void* ptr = NULL， 将ptr按照引用方式传入，获取结果时，需要转换为CBufferData
//	pWaitObserver: 异步模式使用，智能分析服务器与转发服务器之间关于NVR的数据交互按照同步逻辑处理， 故此处默认值即可
_EXPORT_NVRSERVERSDK_API tuint32 NVRServer_SDK_Ans_Request_Rsu_Cmd(tuint32 cmdCode, const char *pSendBuf, const int iLen, LPVOID& waitLPara, CWaitObserver *pWaitObserver = NULL);

// 转发服务器向智能分析服务器推送告警信息
// 异步推送，只管将是数据推送出去，不用管是否成功
// 参数说明:
// ansId: 智能分析服务的id
// cmdCode: 命令字，参见ClientIAProtocol中的定义
// pSendBuf: 发送数据序列化
// iLen:  发送数据的长度
//	waitLPara：接收结果的指针，void* ptr = NULL， 将ptr按照引用方式传入，获取结果时，需要转换为CBufferData
//	pWaitObserver: 
_EXPORT_NVRSERVERSDK_API tuint32 NVRServer_SDK_Mdu_Notify_Alarm_to_Mdu_Cmd(tuint32 ansId, tuint32 cmdCode, const char *pSendBuf, const int iLen, LPVOID& waitLPara, CWaitObserver *pWaitObserver = NULL);

//////////////////////////////////////////////////////////////////////////
// 转发服务器向 人脸比对|NVR数据请求命令， 同步命令， useless
_EXPORT_NVRSERVERSDK_API tuint32 NVRServer_SDK_Mdu_Request_FrNvr_Cmd(tuint32 devID, const char *pSendBuf, const int iLen, CBufferData*& buf);

//分页查询报警操作配置等日志 返回值为查询句柄销毁用
_EXPORT_NVRSERVERSDK_API tuint32 NVRServer_SDK_LOG_CreateQueryRecord_ByPage(const char* pLogTypeMask, tuint32 dwStartTime, tuint32 dwEndTime, const GUID &UserGUID,\
	tuint32 pageIndex/*第几页*/, tuint32 pageSize/*每页条数*/, char *&pBuf , double &totalCount, bool isExportLog);
_EXPORT_NVRSERVERSDK_API bool NVRServer_SDK_LOG_DestroyQueryRecord_ByPage(tuint32 handle);

//用于精确查询报警日志 返回值为查询句柄销毁用
_EXPORT_NVRSERVERSDK_API tuint32 NVRServer_SDK_LOG_CreateQueryAlarmRecord_ByPageAndNode(const char* pLogTypeMask, const char* pLogNodeId, tuint32 dwStartTime, tuint32 dwEndTime, const GUID &UserGUID, tuint32 pageIndex/*第几页*/, tuint32 pageSize/*每页条数*/, char *&pBuf , double &totalCount, bool isExportLog);
_EXPORT_NVRSERVERSDK_API bool NVRServer_SDK_LOG_DestroyQueryAlarmRecord_ByPageAndNode(tuint32 handle);

//分页查询状态日志
_EXPORT_NVRSERVERSDK_API tuint32 NVRServer_SDK_LOG_CreateQueryStateLog_ByPage(tuint64 dwLogID, tuint32 dwStartTime, tuint32 dwEndTime,\
	tuint32 pageIndex/*第几页*/, tuint32 pageSize/*每页条数*/, char *&pBuf,double &totalCount);
_EXPORT_NVRSERVERSDK_API bool NVRServer_SDK_LOG_DestroyQueryStateLog_ByPage(tuint32 handle);

//分页查询通行记录日志
_EXPORT_NVRSERVERSDK_API tuint32 NVRServer_SDK_LOG_CreateQueryPassRecordLog_ByPage(const CFGPACK_DEFINE::PMS_LOG_PASS_RECORD* pLogPassRecord,tuint32 dwStartTime, tuint32 dwEndTime,const GUID &UserGUID,\
	tuint32 pageIndex/*第几页*/, tuint32 pageSize/*每页条数*/, char *&pBuf,double &totalCount);
_EXPORT_NVRSERVERSDK_API bool NVRServer_SDK_LOG_DestroyQueryPassRecordLog_ByPage(tuint32 handle);
_EXPORT_NVRSERVERSDK_API tuint32 NVRServer_SDK_LOG_QueryPassRecordLog_ByPage(const CFGPACK_DEFINE::PMS_LOG_PASS_RECORD* pLogPassRecord,std::list<std::string>& laneGUIDList,tuint32 dwStartTime,tuint32 dwEndTime,\
	tuint32 pageIndex, tuint32 pageSize, char *&pBuf,double &totalCount);
_EXPORT_NVRSERVERSDK_API bool NVRServer_SDK_LOG_CountPassRecordLog(NVMS_NET_PROTOCOL::PMS_INFO &pmsInfo);
_EXPORT_NVRSERVERSDK_API void NVRServer_SDK_LOG_QueryPassRecord_OverTime(const tuint32 tempParkingTimeLimit, std::list<CFGPACK_DEFINE::PMS_LOG_PASS_RECORD> &resultList);

//分页查询通行收费日志
_EXPORT_NVRSERVERSDK_API tuint32 NVRServer_SDK_LOG_CreateQueryPassChargeLog_ByPage(const CFGPACK_DEFINE::PMS_LOG_PASS_CHARGE* pLogPassCharge, tuint32 dwStartTime, tuint32 dwEndTime, const GUID &UserGUID, tuint32 pageIndex/*第几页*/, tuint32 pageSize/*每页条数*/, char *&pBuf, double &totalCount);
_EXPORT_NVRSERVERSDK_API bool NVRServer_SDK_LOG_DestroyQueryPassChargeLog_ByPage(tuint32 handle);
_EXPORT_NVRSERVERSDK_API tuint32 NVRServer_SDK_LOG_AddPassChargeLog(const CFGPACK_DEFINE::PMS_LOG_PASS_CHARGE* pLogPassCharge);

//分页查询固定车缴费日志
_EXPORT_NVRSERVERSDK_API tuint32 NVRServer_SDK_LOG_CreateQueryAccountChargeLog_ByPage(const CFGPACK_DEFINE::PMS_LOG_ACCOUNT_CHARGE* pLogAccountCharge, tuint32 dwStartTime, tuint32 dwEndTime, const GUID &UserGUID, tuint32 pageIndex/*第几页*/, tuint32 pageSize/*每页条数*/, char *&pBuf, double &totalCount);
_EXPORT_NVRSERVERSDK_API bool NVRServer_SDK_LOG_DestroyQueryAccountChargeLog_ByPage(tuint32 handle);
_EXPORT_NVRSERVERSDK_API tuint32 NVRServer_SDK_LOG_AddAccountChargeLog(const CFGPACK_DEFINE::PMS_LOG_ACCOUNT_CHARGE* pLogAccountCharge);

// 仿照 PassRecord 类 建的 VisitorRecord 类 
/*****************************************/
//分页查询通行记录日志
_EXPORT_NVRSERVERSDK_API tuint32 NVRServer_SDK_LOG_FuzzyQueryVisitorRecordLog_ByPage(tuint32 dwStartTime, tuint32 dwEndTime, std::string dwKeyValue, \
	std::string sexType, std::string allowedVisitType, std::string verifyType, std::string remindType, const GUID &UserGUID, \
	tuint32 pageIndex/*第几页*/, tuint32 pageSize/*每页条数*/, char *&pBuf, double &totalCount);

//分页查询通行记录日志
_EXPORT_NVRSERVERSDK_API tuint32 NVRServer_SDK_LOG_CreateQueryVisitorRecordLog_ByPage(const CFGPACK_DEFINE::LOG_VISITOR_RECORD* pLogVisitorRecord, const GUID &UserGUID, \
	tuint32 pageIndex/*第几页*/, tuint32 pageSize/*每页条数*/, char *&pBuf, double &totalCount);

_EXPORT_NVRSERVERSDK_API bool NVRServer_SDK_LOG_DestroyQueryVisitorRecordLog_ByPage(tuint32 handle);

_EXPORT_NVRSERVERSDK_API bool NVRServer_SDK_LOG_EditVisitorRecord(const CFGPACK_DEFINE::LOG_VISITOR_RECORD *pVisitorRecord);
_EXPORT_NVRSERVERSDK_API bool NVRServer_SDK_LOG_DelVisitorRecord(const string strIndex);
_EXPORT_NVRSERVERSDK_API std::string NVRServer_SDK_LOG_AddVisitorRecord(const CFGPACK_DEFINE::LOG_VISITOR_RECORD* pLogVisitorRecord);
/*****************************************/
//查询sop日志 INN24银行项目
_EXPORT_NVRSERVERSDK_API tuint32 NVRServer_SDK_LOG_QueryContent(tuint64 dwLogKey, tuint32 iOccurTime, char *&pOutBuf,tuint32 nOutLen, char*& pUserName, GUID curUserGUID, const char* pCurUserName);

//INN24银行项目
_EXPORT_NVRSERVERSDK_API tuint32 NVRServer_SDK_LOG_EditContent(tuint64 dwLogKey, tuint32 iOccurTime, char *pContent,tuint32 nLen);

//INN24银行项目
_EXPORT_NVRSERVERSDK_API tuint32 NVRServer_SDK_LOG_CloseContent(tuint64 dwLogKey);

//设备总通道数量大于授权数量，部分设备离线
_EXPORT_NVRSERVERSDK_API void NVRServer_SDK_OutLicenseForAddChannel(NET_OUT_LICENSE_FOR_ADDCHANNEL st);
//根据存储服务器GUID获取分区信息
_EXPORT_NVRSERVERSDK_API void NVRServer_SDK_GetPartitionList(GUID nodeGUID,std::list<NVMS_NET_PROTOCOL::NET_PARTITION_INFO>& PartList,char* &pData/*传出*/,tuint32 &nDataLen);
//转发服务向存储服务推送区域入侵订阅后的信息
// 异步推送，只管将是数据推送出去，不用管是否成功
// 参数说明:
// ansId: 存储服务器ID
// cmdCode: 命令字，
// pSendBuf: 发送数据序列化
// iLen:  发送数据的长度
//	waitLPara：接收结果的指针，void* ptr = NULL， 将ptr按照引用方式传入，获取结果时，需要转换为CBufferData
//	pWaitObserver: 
_EXPORT_NVRSERVERSDK_API tuint32 NVRServer_SDK_Msu_Notify_Info_to_Msu_Cmd(tuint32 channid, tuint32 cmdCode, const char *pSendBuf, const int iLen, LPVOID& waitLPara, CWaitObserver *pWaitObserver = NULL);

// 转发服务器向存储服务器推送告警信息
// 异步推送，只管将是数据推送出去，不用管是否成功
// 参数说明:
// msuid: 智能分析服务id
// cmdCode: 命令字，参见ClientIAProtocol中的定义
// pSendBuf: 发送数据序列化
// iLen:  发送数据的长度
//	waitLPara：接收结果的指针，void* ptr = NULL， 将ptr按照引用方式传入，获取结果时，需要转换为CBufferData
//	pWaitObserver: 
_EXPORT_NVRSERVERSDK_API tuint32 NVRServer_SDK_Mdu_Notify_Msu_Cmd(tuint32 msuid, tuint32 cmdCode, const char *pSendBuf, const int iLen, LPVOID& waitLPara, CWaitObserver *pWaitObserver = NULL);

//初始化数据加密key
_EXPORT_NVRSERVERSDK_API bool NVRServer_SDK_SetEncryptKeyDB(const std::string & strKey);
_EXPORT_NVRSERVERSDK_API bool NVRServer_SDK_SetEncryptKeyCfg(const std::string & strKey);

//获取目标库信息
_EXPORT_NVRSERVERSDK_API string NVRServer_SDK_GetAlbumInfo(GUID ansGUID);

//is Mysql
_EXPORT_NVRSERVERSDK_API bool NVRServer_SDK_IsMysqlDatabase();

_EXPORT_NVRSERVERSDK_API bool NVRServer_SDK_LOG_QueryLogWithSop(tuint32 dwStartTime, tuint32 dwEndTime, std::list<tuint64> &LogKeyList);

#endif  //////_NVR_SERVER_SDK_H











