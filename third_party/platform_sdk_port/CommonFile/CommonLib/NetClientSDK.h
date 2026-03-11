
///////////////////////////NetClientSDK.h

#ifndef _NET_CLIENT_SDK_H
#define _NET_CLIENT_SDK_H

#ifdef WIN32
    #ifdef NET_CLIENT_SDK_STATIC//静态库编译和使用
        #define _EXPORT_NETCLIENT_CLASS 
        #define _EXPORT_NETCLIENT_SDK 
    #else
        #ifdef NET_CLIENT_SDK_EXPORT_API//动态库编译
            #define _EXPORT_NETCLIENT_CLASS __declspec(dllexport)
            #define _EXPORT_NETCLIENT_SDK  __declspec(dllexport)
        #else//使用者编译
            #define _EXPORT_NETCLIENT_CLASS __declspec(dllimport)
            #define _EXPORT_NETCLIENT_SDK  __declspec(dllimport)
        #endif
    #endif
#else
    #define _EXPORT_NETCLIENT_CLASS __attribute__ ((visibility("default")))
    #define _EXPORT_NETCLIENT_SDK __attribute__ ((visibility("default")))
#endif

#include <map>
using std::map;

#include "LoginStateObserver.h"
#include "FrameData.h"
#include "BufferData.h"
#include "N_SDKCommonObserver.h"
#include "base_type_define.h"

typedef struct custom_info
{
	bool useP2P; //客户是否定制p2p
	bool useProtocol; //客户是否定制接入设备协议
    bool useThermalImage; //0-标准版客户端不允许显示测温模块 1-忽略该字段,用客户端自己的判断逻辑 (TSS版本传1)2020-07-24从rev分出 防止TSS客户端登录A300-MTC后有测温功能
	char protocols[64];//定制接入设备协议
	char tvtManufacturer[32];//客户定制tvt设备的生产厂商，非定制tvt设备显示onvif,默认为空
	char tvtManufacturerNvr[16];//客户定制tvt设备的生产厂商，非定制tvt设备显示onvif,默认为空
	char supportP2pType;//1：支持p2p1.0   2：支持p2p2.0   3：同时支持1.0 2.0
    char rev1[223];//保留
}CUSTOM_INFO_CLIENT;

class Interlocked;
class CAlarmTriggerObserver;
class CPassRecordObserver;
class CPrPassRecordObserver;
class AppPushStreamCtrl;
class CProcessObserver;
class CTrajectRectObserver;
//很多功能函数都使用了以下这个通知模式，在初始化时将自己加入观察者队列，退出时移除
//执行操作时返回一个唯一的ID，这个ID在观察者回调函数里面用来判断是不是本次操作
//没有使用在调用函数时传入观察者的原因是因为可能存在操作返回时对象已经被销毁的情况

///////////////////////////////以下内容转移到 "N_SDKCommonObserver.h"里面去了

// class CWaitObserver
// {
// public:
// 	CWaitObserver(void){};
// public:
// 	virtual ~CWaitObserver(void){};
// 
// 	//几乎所有的函数都具有操作对象，destID就是操作对象的ID
// 	virtual void OnWaitForResult(tuint32 optID,tuint32 destID,bool isOK,const void *pFrameData, size_t frameDataLen,LPVOID lParam)=0;
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
// 	virtual void StreamCapture(const tuint32 streamID, const char *pBuf,size_t iLen,tuint32 errorCode)=0;
// };

//网络模块的初始化和销毁
//第五个参数nIsDecoder,设置当前是否是解码器调用，由于lite版本限制其他客户端登录，解码器和平台SDK以客户端的名义登录的时候也被限制了，为了解决这个问题，在登录结构体里面加一个标识 1表示是解码器 2表示平台SDK
//第六个参数nPlatformType,设置当前要连接的管理服务器的平台版本  //Enterprise_Type=0企业版,Standard_Type=1标准版,Lite_Type=2单机版
_EXPORT_NETCLIENT_SDK bool NET_CLIENT_Initial(tuint32 localNodeType, Interlocked *pGlobalNodeIDBase, const char *exePath, const char* p2pServerAddr=NULL,tuint32 nIsOtherType=0/*1表示是解码器 2表示平台SDK*/,tuint32 nPlatformType=0 );
_EXPORT_NETCLIENT_SDK void NET_CLIENT_Quit();
_EXPORT_NETCLIENT_SDK bool NET_CLIENT_Start();
_EXPORT_NETCLIENT_SDK bool NET_CLIENT_ReStart();//注册服务器断开后重新登陆前调用
_EXPORT_NETCLIENT_SDK void NET_CLIENT_Stop();
_EXPORT_NETCLIENT_SDK bool NET_CLIENT_LoginServerUnit(tuint32 serverUnitType, char *pRSUIP,tuint16 RSUPort,char *pName,char* pPwd,BYTE *pMac,bool bReconnect,CLoginStateObserver *pObServer);
_EXPORT_NETCLIENT_SDK bool NET_CLIENT_LoginServerUnit(tuint32 serverUnitType, char *pIP,tuint16 Port,char *pName,char* pPwd, bool bReconnect,CLoginStateObserver *pObServer);

_EXPORT_NETCLIENT_SDK void NET_CLIENT_StopLoginServerUnit();
_EXPORT_NETCLIENT_SDK bool NET_CLIENT_ReadyServerUnit(tuint32 serverUnitType, tuint16 serverPort=0,char *pExtInfo=NULL,int exInfoLen=0);//参数里面的值只有电视墙服务器使用
_EXPORT_NETCLIENT_SDK tuint32 NET_CLIENT_GetLastError(); //返回_ErrorCodeDef.h里面的值

_EXPORT_NETCLIENT_SDK void NET_CLIENT_AddLiveStreamObserver(CStreamCaptureObserver *pStreamObserver);//接收帧数据的观察者
_EXPORT_NETCLIENT_SDK void NET_CLIENT_DelLiveStreamObserver(CStreamCaptureObserver *pStreamObserver);//接收帧数据的观察者

_EXPORT_NETCLIENT_SDK void NET_CLIENT_AddPlaybackStreamObserver(CStreamCaptureObserver *pStreamObserver);//回放帧数据的观察者一般是回放
_EXPORT_NETCLIENT_SDK void NET_CLIENT_DelPlaybackStreamObserver(CStreamCaptureObserver *pStreamObserver);//回放帧数据的观察者

_EXPORT_NETCLIENT_SDK void NET_CLIENT_AddTalkbackStreamObserver(CStreamCaptureObserver *pStreamObserver);//对讲帧数据的观察者
_EXPORT_NETCLIENT_SDK void NET_CLIENT_DelTalkbackStreamObserver(CStreamCaptureObserver *pStreamObserver);//对讲帧数据的观察者

_EXPORT_NETCLIENT_SDK void NET_CLIENT_AddAlarmTriggerObserver(CAlarmTriggerObserver *pStreamObserver);//报警联动的观察者
_EXPORT_NETCLIENT_SDK void NET_CLIENT_DelAlarmTriggerObserver(CAlarmTriggerObserver *pStreamObserver);//报警联动的观察者

_EXPORT_NETCLIENT_SDK void NET_CLIENT_AddPassRecordObserver(CPassRecordObserver *pPassRecordObserver);//PMS从转发服务器获取图片及通行记录
_EXPORT_NETCLIENT_SDK void NET_CLIENT_DelPassRecordObserver(CPassRecordObserver *pPassRecordObserver);//PMS从转发服务器获取图片及通行记录

_EXPORT_NETCLIENT_SDK void NET_CLIENT_AddPrPassRecordObserver(CPrPassRecordObserver *pPassRecordObserver);//PMS从转发服务器获取图片及通行记录
_EXPORT_NETCLIENT_SDK void NET_CLIENT_DelPrPassRecordObserver(CPrPassRecordObserver *pPassRecordObserver);//PMS从转发服务器获取图片及通行记录

_EXPORT_NETCLIENT_SDK void NET_CLIENT_AddTrajectRectObserver(CTrajectRectObserver *pTrajectRectObserver); //转发发送目标框数据
_EXPORT_NETCLIENT_SDK void NET_CLIENT_DelTrajectRectObserver(CTrajectRectObserver *pTrajectRectObserver); //转发发送目标框数据


_EXPORT_NETCLIENT_SDK void NET_CLIENT_AddProcessObserver(CProcessObserver *pProcessObserver);//获取升级进度
_EXPORT_NETCLIENT_SDK void NET_CLIENT_DelProcessObserver(CProcessObserver *pProcessObserver);//
// 接收电视墙服务器推送的数据
_EXPORT_NETCLIENT_SDK void NET_CLIENT_AddTvWallPushStreamObserver(CWaitObserver* pObserver);
_EXPORT_NETCLIENT_SDK void NET_CLIENT_DelTvWallPushStreamObserver(CWaitObserver* pObserver);

// 接收智能分析服务器推送事件
_EXPORT_NETCLIENT_SDK void NET_CLIENT_AddIASrvPushStreamObserver(CWaitObserver* pObserver);
_EXPORT_NETCLIENT_SDK void NET_CLIENT_DelIASrvPushStreamObserver(CWaitObserver* pObserver);

// 接收接入服务器推送事件
_EXPORT_NETCLIENT_SDK void NET_CLIENT_AddJoinSrvPushStreamObserver(CWaitObserver* pObserver);
_EXPORT_NETCLIENT_SDK void NET_CLIENT_DelJoinSrvPushStreamObserver(CWaitObserver* pObserver);

// 接收存储服务器推送事件
_EXPORT_NETCLIENT_SDK void NET_CLIENT_AddMsuPushStreamObserver(CWaitObserver* pObserver);
_EXPORT_NETCLIENT_SDK void NET_CLIENT_DelMsuPushStreamObserver(CWaitObserver* pObserver);

// 接收1400推送事件
_EXPORT_NETCLIENT_SDK void NET_CLIENT_AddGAT1400StreamObserver(CWaitObserver* pObserver);
_EXPORT_NETCLIENT_SDK void NET_CLIENT_DelGAT1400StreamObserver(CWaitObserver* pObserver);

//EMAPManager内部使用，用来动态下载地图文件
_EXPORT_NETCLIENT_SDK CBufferData* NET_CLIENT_GetEmapFile(tuint32 emapID);//返回值自己要减少计数
//发送对讲数据
_EXPORT_NETCLIENT_SDK bool NET_CLIENT_SendTalkStreamData(tuint32 devID,const void* pBuf,const tuint32 iLen);

//回放控制
_EXPORT_NETCLIENT_SDK bool NET_CLIENT_PlaybackControlStop(tuint32 streamID,bool waitResult);//根据流ID停止回放，内部会知道是设备还是存储服务器
_EXPORT_NETCLIENT_SDK bool NET_CLIENT_PlaybackControlPause(tuint32 streamID,bool waitResult);//存储服务器及网络设备回放暂停
_EXPORT_NETCLIENT_SDK bool NET_CLIENT_PlaybackControlResume(tuint32 streamID,bool waitResult);//存储服务器回放
_EXPORT_NETCLIENT_SDK bool NET_CLIENT_PlaybackControlSendFrameIndex(tuint32 streamID,tuint32 frameIndex);//存储服务器回放
_EXPORT_NETCLIENT_SDK bool NET_CLIENT_PlaybackControlChangePlaySpeed(tuint32 streamID,tuint32 speed);// 国标28181

_EXPORT_NETCLIENT_SDK bool NET_CLIENT_RecDataBackupStart(tuint32 chID, tuint32 eventMask,tuint32 startime,tuint32 endTime,READ_RECORD_POSITION recordDataPosition, tint32 bMainStream);//录像数据备份
_EXPORT_NETCLIENT_SDK tuint32 NET_CLIENT_RecDataBackupStart(tuint32 chID, tuint32 eventMask,tuint32 startime,tuint32 endTime,LPVOID userPara,READ_RECORD_POSITION recordDataPosition, tint32 bMainStream,CWaitObserver *pWaitObserver/*=NULL*/);//录像数据备份
// 从指定存储备份
_EXPORT_NETCLIENT_SDK tuint32 NET_CLIENT_RecDataBackupStart(tuint32 chID, tuint32 eventMask,tuint32 startime,tuint32 endTime,LPVOID userPara,READ_RECORD_POSITION recordDataPosition,tuint32 msuId, tint32 bMainStream,CWaitObserver *pWaitObserver/*=NULL*/);//录像数据备份

_EXPORT_NETCLIENT_SDK bool NET_CLIENT_OpenKeyFrameStream(tuint32 streamID, const FILETIME &ftFrameTime);		//开始关键帧快进
_EXPORT_NETCLIENT_SDK bool NET_CLIENT_CloseKeyFrameStream(tuint32 streamID, const FILETIME &ftFrameTime);		//关闭关键帧快进
_EXPORT_NETCLIENT_SDK bool NET_CLIENT_PlaybackRewind(tuint32 streamID, const FILETIME &ftFrameTime, tuint32 dwSectionTime);		//倒放
_EXPORT_NETCLIENT_SDK bool NET_CLIENT_PlaybackCancelRewind(tuint32 streamID, const FILETIME &ftFrameTime, tuint32 dwSectionTime);	//取消倒放
_EXPORT_NETCLIENT_SDK bool NET_CLIENT_PlaybackPrevious(tuint32 streamID, const FILETIME &ftFrameTime, tuint32 dwSectionTime);		//前一帧
_EXPORT_NETCLIENT_SDK bool NET_CLIENT_PlaybackResume(tuint32 streamID, const FILETIME &ftFrameTime);		//恢复正常播放
_EXPORT_NETCLIENT_SDK bool NET_CLIENT_RecDataBackupStop(tuint32 streamID,bool waitResult);//备份停止

//关闭网络流
_EXPORT_NETCLIENT_SDK bool NET_CLIENT_RequestCloseLiveStream(tuint32 streamID,bool waitResult);
_EXPORT_NETCLIENT_SDK bool NET_CLIENT_RequestCloseTalkStream(tuint32 streamID,bool waitResult);
_EXPORT_NETCLIENT_SDK bool NET_CLIENT_RequestCloseBroadcastStream(tuint32 streamID,bool waitResult);

//增加全局观察者，在函数不传入自己的观察者有时非阻塞模式时会通知到全部全局观察者，以适应上层应用的需要
_EXPORT_NETCLIENT_SDK bool NET_CLIENT_AddWaitForResultObserver(CWaitObserver *pWaitObserver);
_EXPORT_NETCLIENT_SDK bool NET_CLIENT_DeleteWaitForResultObserver(CWaitObserver *pWaitObserver);
_EXPORT_NETCLIENT_SDK void NET_CLIENT_Run(tuint32 optID, tuint8 priority = 1);//投递非阻塞模式操作，参数为各非阻塞模式函数返回值
//priority:0为高优先级,1为低优先级

//在系统里面记录LOG，只针对那些转发和存储服务器无法处理的LOG，如轮询，logType为LogSDK.h里面定义的值
_EXPORT_NETCLIENT_SDK bool NET_CLIENT_RecordLog(tuint32 logType,tuint32 devOrChID);//不需要返回服务器ID的，也不需要等待，服务器只是记录一下LOG记录
//一次带有多个节点的
_EXPORT_NETCLIENT_SDK bool NET_CLIENT_RecordLogMultiNode(tuint32 logType,const tuint32 count,const tuint32 *pNodeID);//不需要返回服务器ID的，也不需要等待，服务器只是记录一下LOG记录


//修改当前登录的用户密码
_EXPORT_NETCLIENT_SDK bool NET_CLIENT_ChangeCurUserPWD(char *pOldPwd,char *pNewPwd);
_EXPORT_NETCLIENT_SDK tuint32 NET_CLIENT_ChangeCurUserPWD(char *pOldPwd,char *pNewPwd,LPVOID userPara,CWaitObserver *pWaitObserver=NULL);

//获取配置服务端口
_EXPORT_NETCLIENT_SDK tuint32 NET_CLIENT_GetConfigServerPort();
//获取PMS超时停车时长（小时）
_EXPORT_NETCLIENT_SDK tuint32 NET_CLIENT_GetPmsParkOverHour();
_EXPORT_NETCLIENT_SDK bool NET_CLIENT_SetPmsParkOverHour(tuint32 hour);
//获取图片服务器端口
_EXPORT_NETCLIENT_SDK tuint32 NET_CLIENT_GetPictureServerPort();
//获取车牌存储位置：0:存储服务器 1:图片服务器
_EXPORT_NETCLIENT_SDK tuint32 NET_CLIENT_GetPictureServer();
//获取与配置服务器通讯方式，0：http;1:https
_EXPORT_NETCLIENT_SDK tuint32 NET_CLIENT_GetConfigServerHttpStatus();

//等到结果后返回，且自己要对返回值Dec
_EXPORT_NETCLIENT_SDK bool NET_CLIENT_RequestDeviceDiskInfo(tuint32 devID,CBufferData* &pFrameData);
//等到结果后pWaitObserver->OnWaitForResult(函数返回值ID,userPara)，函数返回0表示不成功一般是找不到节点或节点不在线之类的
_EXPORT_NETCLIENT_SDK tuint32 NET_CLIENT_RequestDeviceDiskInfo(tuint32 devID,LPVOID userPara,CWaitObserver *pWaitObserver=NULL);

//与转发交互发送xml的通用协议
_EXPORT_NETCLIENT_SDK bool NET_CLIENT_RequestOperate(tuint32 devID,const char *pSendBuf, tuint32 dwLen,CBufferData* &pFrameData, NET_PROTOCOL_CMD_DEF cmd);
_EXPORT_NETCLIENT_SDK tuint32 NET_CLIENT_RequestOperate(tuint32 devID,const char *pSendBuf, tuint32 dwLen, LPVOID userPara,NET_PROTOCOL_CMD_DEF cmd, CWaitObserver *pWaitObserver);

//等到结果后返回，且自己要对返回值Dec
_EXPORT_NETCLIENT_SDK bool NET_CLIENT_RequestDeviceLog(tuint32 devID,tuint32 dwStartTime,tuint32 dwEndTime,CBufferData* &pFrameData);
//等到结果后pWaitObserver->OnWaitForResult(函数返回值ID,userPara)，函数返回0表示不成功一般是找不到节点或节点不在线之类的
_EXPORT_NETCLIENT_SDK tuint32 NET_CLIENT_RequestDeviceLog(tuint32 devID,tuint32 dwStartTime,tuint32 dwEndTime,LPVOID userPara,CWaitObserver *pWaitObserver=NULL);

//等到结果后返回，且自己要对返回值Dec
_EXPORT_NETCLIENT_SDK bool NET_CLIENT_SearchSystemLogInfo(tuint32 selectMask,tuint32 userID,tuint32 dwStartTime,tuint32 dwEndTime,CBufferData* &pFrameData);
//等到结果后pWaitObserver->OnWaitForResult(函数返回值ID,userPara)，函数返回0表示不成功一般是找不到节点或节点不在线之类的
_EXPORT_NETCLIENT_SDK tuint32 NET_CLIENT_SearchSystemLogInfo(tuint32 selectMask,tuint32 userID,tuint32 dwStartTime,tuint32 dwEndTime,LPVOID userPara,CWaitObserver *pWaitObserver=NULL);

//等到结果后返回，且自己要对返回值Dec
_EXPORT_NETCLIENT_SDK bool NET_CLIENT_SearchAbnormalLogInfo(tuint32 dwStartTime,tuint32 dwEndTime,CBufferData* &pFrameData);
//等到结果后pWaitObserver->OnWaitForResult(函数返回值ID,userPara)，函数返回0表示不成功一般是找不到节点或节点不在线之类的
_EXPORT_NETCLIENT_SDK tuint32 NET_CLIENT_SearchAbnormalLogInfo(tuint32 dwStartTime,tuint32 dwEndTime,LPVOID userPara,CWaitObserver *pWaitObserver=NULL);

//手动报警输出
_EXPORT_NETCLIENT_SDK bool NET_CLIENT_SetAlarmOut(tuint32 devID, tuint32 alarmID, bool bOn);
_EXPORT_NETCLIENT_SDK tuint32 NET_CLIENT_SetAlarmOut(tuint32 devID, tuint32 alarmID, bool bOn, LPVOID userPara,CWaitObserver *pWaitObserver=NULL);

//一键对焦
_EXPORT_NETCLIENT_SDK bool NET_CLIENT_AFCtrl(tuint32 chID, tuint32 afCtrlType, tuint32 nValue);//

//云台控制
_EXPORT_NETCLIENT_SDK bool NET_CLIENT_PTZMove(tuint32 chID,tuint32 opt,tuint32 speed);//opt为ECMS_PTZ_MOVE里面的值
_EXPORT_NETCLIENT_SDK bool NET_CLIENT_PTZMove(tuint32 chID,tuint32 opt,tuint32 speed,LPVOID userPara,CWaitObserver *pWaitObserver=NULL);//opt为ECMS_PTZ_MOVE里面的值
_EXPORT_NETCLIENT_SDK bool NET_CLIENT_PTZControl(tuint32 chID,tuint32 opt,tuint32 speed,tuint32 dwParam);//opt为ECMS_PTZ_CONTROL里面的值
_EXPORT_NETCLIENT_SDK bool NET_CLIENT_PTZControl(tuint32 chID,tuint32 opt,tuint32 speed,tuint32 dwParam,LPVOID userPara,CWaitObserver *pWaitObserver=NULL,char isPresetUsed = '0');//opt为ECMS_PTZ_CONTROL里面的值
_EXPORT_NETCLIENT_SDK bool NET_CLIENT_PTZModifyName(tuint32 chID,const char* name,tuint32 presetIndex);//opt为ECMS_PTZ_MOVE里面的值
_EXPORT_NETCLIENT_SDK bool NET_CLIENT_PTZModifyName(tuint32 chID,const char* name,tuint32 presetIndex,LPVOID userPara,CWaitObserver *pWaitObserver=NULL);//opt为ECMS_PTZ_MOVE里面的值
_EXPORT_NETCLIENT_SDK bool NET_CLIENT_PTZSetGuardInfo(tuint32 chID,bool bSwitch,const char* location,tuint32 nNumber,tuint32 waitTime);//opt为ECMS_PTZ_MOVE里面的值
_EXPORT_NETCLIENT_SDK bool NET_CLIENT_PTZSetGuardInfo(tuint32 chID,bool bSwitch,const char* location,tuint32 nNumber,tuint32 waitTime,LPVOID userPara,CWaitObserver *pWaitObserver=NULL);//opt为ECMS_PTZ_MOVE里面的值

_EXPORT_NETCLIENT_SDK bool NET_CLIENT_PTZGetGuardInfo(tuint32 chID,CBufferData* &pFrameData);
_EXPORT_NETCLIENT_SDK tuint32 NET_CLIENT_PTZGetGuardInfo(tuint32 chID,LPVOID userPara,CWaitObserver *pWaitObserver=NULL);

// 云台3D控制
_EXPORT_NETCLIENT_SDK bool NET_CLIENT_PTZ3DControl(tuint32 chID, tint32 dx, tint32 dy, tint16 zoom);
_EXPORT_NETCLIENT_SDK bool NET_CLIENT_PTZ3DControl(tuint32 chID, tint32 dx, tint32 dy, tint16 zoom, LPVOID userPara,CWaitObserver *pWaitObserver=NULL);

//预置点和巡航线信息
_EXPORT_NETCLIENT_SDK bool NET_CLIENT_PTZGetPresetInfo(tuint32 chID,CBufferData* &pFrameData);
_EXPORT_NETCLIENT_SDK tuint32 NET_CLIENT_PTZGetPresetInfo(tuint32 chID,LPVOID userPara,CWaitObserver *pWaitObserver=NULL);

_EXPORT_NETCLIENT_SDK bool NET_CLIENT_PTZGetCruiseInfo(tuint32 chID,CBufferData* &pFrameData);
_EXPORT_NETCLIENT_SDK tuint32 NET_CLIENT_PTZGetCruiseInfo(tuint32 chID,LPVOID userPara,CWaitObserver *pWaitObserver=NULL);

_EXPORT_NETCLIENT_SDK bool NET_CLIENT_PTZGetTraceInfo(tuint32 chID,CBufferData* &pFrameData);
_EXPORT_NETCLIENT_SDK tuint32 NET_CLIENT_PTZGetTraceInfo(tuint32 chID,LPVOID userPara,CWaitObserver *pWaitObserver=NULL);
//IPC升级
_EXPORT_NETCLIENT_SDK bool NET_CLIENT_IPCUpdate(tuint32 chID,const char* pData,tuint32 nLen);
_EXPORT_NETCLIENT_SDK bool NET_CLIENT_IPCUpdate(tuint32 chID,const char* pData,tuint32 nLen,LPVOID userPara,CWaitObserver *pWaitObserver=NULL);
//不需要关系设备类型的接口，IPC和NVR预置点和巡航线设置不自己组装，交由相关设备适配器完成,nOpt为操作类型，0：增加 1：编辑 2：删除
/*
<request version="2.0" systemType="NVMS-9000/IPC" clientType="WEB" url="requestSavePreset">
<content chno="1">
<chlId>1234</chlId>
<preset index="" name=""/>
</content>
</requset>
*/
_EXPORT_NETCLIENT_SDK bool NET_CLIENT_PTZ_RequesSavePreset(tuint32 chID, const char* szXmlPreset, const tuint32 nXmlLen);
/*
<request version="2.0" systemType="NVMS-9000/IPC" clientType="WEB" url="requsetConfigCurise">
<content chno="1" opt="add/delete">
<chlId>1234</chlId>
<cruise index="" name="" presetCount="2">
<preset index="1" speed="" holdtime=""/>
<preset index="2" speed="" holdtime=""/>
</cruise>
</content>
</requset>
*/
_EXPORT_NETCLIENT_SDK bool NET_CLIENT_PTZ_RequestConfigCruise(tuint32 chID, const char* szXmlCruise, const tuint32 nXmlLen);

//获取设备配置
_EXPORT_NETCLIENT_SDK bool NET_CLIENT_RequestDeviceCfg(tuint32 devID,const char *pSendBuf, const int iLen,CBufferData* &pFrameData);
_EXPORT_NETCLIENT_SDK tuint32 NET_CLIENT_RequestDeviceCfg(tuint32 devID,const char *pSendBuf, const int iLen,LPVOID userPara,CWaitObserver *pWaitObserver=NULL);
//保存设备配置
_EXPORT_NETCLIENT_SDK bool NET_CLIENT_RequestSaveDeviceCfg(tuint32 devID,const char *pSendBuf, const int iLen);
_EXPORT_NETCLIENT_SDK tuint32 NET_CLIENT_RequestSaveDeviceCfg(tuint32 devID,const char *pSendBuf, const int iLen,LPVOID userPara,CWaitObserver *pWaitObserver=NULL);

// 获取设备下所有通道信息（PL18获取NVR下所有ipc的ip地址使用）
_EXPORT_NETCLIENT_SDK tuint32 NET_CLIENT_RequestAllChannelsInfo(tuint32 devID, CWaitObserver *pWaitObserver = NULL);

// 断开与设备的网络连接
_EXPORT_NETCLIENT_SDK tuint32 NET_CLIENT_ForceDisconnectDev(tuint32 devID);

//重启设备
_EXPORT_NETCLIENT_SDK bool NET_CLIENT_RequestRebootDevice(tuint32 devID);
_EXPORT_NETCLIENT_SDK tuint32 NET_CLIENT_RequestRebootDevice(tuint32 devID,LPVOID userPara,CWaitObserver *pWaitObserver=NULL);

//恢复设备默认配置
_EXPORT_NETCLIENT_SDK bool NET_CLIENT_RequestDeviceRestoreDefault(tuint32 devID);
_EXPORT_NETCLIENT_SDK tuint32 NET_CLIENT_RequestDeviceRestoreDefault(tuint32 devID,LPVOID userPara,CWaitObserver *pWaitObserver=NULL);

_EXPORT_NETCLIENT_SDK bool  NET_CLIENT_SetChannelColor(tuint32 chID,long bright,long contrast,long hue,long saturation);//optÎȘECMS_PTZ_CONTROLÀïĂæ”ÄÖ”

_EXPORT_NETCLIENT_SDK bool  NET_CLIENT_SetDevTime(tuint32 devID,tuint16 year,tuint16 month,tuint16 day,tuint16 hour,tuint16 minute,tuint16 second);
_EXPORT_NETCLIENT_SDK bool  NET_CLIENT_SetDevTime(tuint32 devID,tuint16 year,tuint16 month,tuint16 day,tuint16 hour,tuint16 minute,tuint16 second, LPVOID userPara, CWaitObserver *pWaitObserver = NULL);

//打开网络流
_EXPORT_NETCLIENT_SDK tuint32 NET_CLIENT_RequestOpenLiveStream(tuint32 chID,BYTE byStreamType,BYTE opType,LPVOID userPara,CWaitObserver *pWaitObserver=NULL);
_EXPORT_NETCLIENT_SDK tuint32 NET_CLIENT_RequestOpenTalkStream(tuint32 devID,LPVOID userPara,CWaitObserver *pWaitObserver=NULL);
_EXPORT_NETCLIENT_SDK tuint32 NET_CLIENT_RequestOpenBroadcastStream(tuint32 devID,LPVOID userPara,CWaitObserver *pWaitObserver=NULL);

// 从存储回放，通道所属的当前存储
_EXPORT_NETCLIENT_SDK tuint32 NET_CLIENT_PlaybackControlPlay(tuint32 chID, tuint32 eventMask,tuint32 startime,tuint32 endTime,READ_RECORD_POSITION recordDataPosition, bool IsGBDownload = false/*是否是国标备份*/);//存储服务器回放
_EXPORT_NETCLIENT_SDK tuint32 NET_CLIENT_PlaybackControlPlay(tuint32 chID, tuint32 eventMask,tuint32 startime,tuint32 endTime,LPVOID userPara, \
	READ_RECORD_POSITION recordDataPosition,CWaitObserver *pWaitObserver=NULL, tint32 bMainStream = 1, bool bIsImmediateReplay=false, bool IsGBDownload = false/*是否是国标备份*/);//存储服务器回放

// 从指定存储服务器回放，指定存储服务器id	 READ_RECORD_POSITION::READ_RECORD_STORAGE 时msuId 有效
_EXPORT_NETCLIENT_SDK tuint32 NET_CLIENT_PlaybackControlPlay(tuint32 chID, tuint32 eventMask,tuint32 startime,tuint32 endTime,LPVOID userPara, \
	READ_RECORD_POSITION recordDataPosition, const tuint32 msuId, CWaitObserver *pWaitObserver=NULL, tint32 bMainStream = 1, bool bIsImmediateReplay=false, bool IsGBDownload = false/*是否是国标备份*/);//存储服务器回放

//手动录像开始和结束
_EXPORT_NETCLIENT_SDK bool  NET_CLIENT_ManualRecordStart(tint32 chlId);
_EXPORT_NETCLIENT_SDK bool  NET_CLIENT_ManualRecordStop(tint32 chlId);
//多通道回放，必须是同一个设备的
_EXPORT_NETCLIENT_SDK bool  NET_CLIENT_PlaybackControlPlayMultiCh(int chCount,tuint32 *pChID, tuint32 eventMask,tuint32 startime,tuint32 endTime, bool IsGBDownload = false/*是否是国标备份*/);
_EXPORT_NETCLIENT_SDK tuint32 NET_CLIENT_PlaybackControlPlayMultiCh(int chCount,tuint32 *pChID, tuint32 eventMask,tuint32 startime,tuint32 endTime,LPVOID userPara,CWaitObserver *pWaitObserver=NULL, bool IsGBDownload = false/*是否是国标备份*/);

//GPS回放
_EXPORT_NETCLIENT_SDK bool NET_CLIENT_PlaybackControlPlayGPS(tuint32 devID, tuint32 startime,tuint32 endTime,BYTE opType,bool isDevPlay);//存储服务器回放
_EXPORT_NETCLIENT_SDK tuint32 NET_CLIENT_PlaybackControlPlayGPS(tuint32 devID, tuint32 startime,tuint32 endTime,BYTE opType,LPVOID userPara,bool isDevPlay,CWaitObserver *pWaitObserver=NULL);//存储服务器回放

_EXPORT_NETCLIENT_SDK bool NET_CLIENT_PlaybackRequestEvent(tuint32 chID, tuint32 eventMask,tuint32 startime,tuint32 endTime,bool bWithDataLen,READ_RECORD_POSITION recordDataPosition,CBufferData* &pFrameData);//存储服务器回放  阻塞模式
_EXPORT_NETCLIENT_SDK tuint32 NET_CLIENT_PlaybackRequestEvent(tuint32 chID, tuint32 eventMask,tuint32 startime,tuint32 endTime,bool bWithDataLen,LPVOID userPara,READ_RECORD_POSITION recordDataPosition,CWaitObserver *pWaitObserver=NULL);//存储服务器回放， 非阻塞模式  单存储
_EXPORT_NETCLIENT_SDK tuint32 NET_CLIENT_PlaybackRequestEvent(tuint32 chID, tuint32 msuId, tuint32 eventMask,tuint32 startime,tuint32 endTime,bool bWithDataLen,LPVOID userPara,READ_RECORD_POSITION recordDataPosition, CWaitObserver *pWaitObserver=NULL);//存储服务器回放， 非阻塞模式  多存储

_EXPORT_NETCLIENT_SDK bool NET_CLIENT_PlaybackHasRecDays(tuint32 chID, tuint32 msuId, const char *pSendBuf, const int iLen, READ_RECORD_POSITION recordDataPosition, CBufferData* &pFrameData);//有录像的日期 阻塞模式
_EXPORT_NETCLIENT_SDK tuint32 NET_CLIENT_PlaybackHasRecDays(tuint32 chID, tuint32 msuId, const char *pSendBuf, const int iLen, READ_RECORD_POSITION recordDataPosition, LPVOID userPara, CWaitObserver *pWaitObserver=NULL);//有录像的日期 非阻塞模式

// 标签管理， 搜索、添加、删除
_EXPORT_NETCLIENT_SDK bool  NET_CLIENT_RequestSearchTag(tuint32 devId,tuint32 chID, tuint32 startime,tuint32 endTime,bool bWithDataLen,READ_RECORD_POSITION recordDataPosition,CBufferData* &pFrameData);
_EXPORT_NETCLIENT_SDK tuint32 NET_CLIENT_RequestSearchTag(tuint32 devId,tuint32 chID, tuint32 startime,tuint32 endTime,bool bWithDataLen,LPVOID userPara,READ_RECORD_POSITION recordDataPosition,CWaitObserver *pWaitObserver=NULL);

_EXPORT_NETCLIENT_SDK bool  NET_CLIENT_RequestAddTag(tuint32 devId, tuint32 chID, tuint32 tagTime,char* discript, int discriptLen,bool bWithDataLen,READ_RECORD_POSITION recordDataPosition,CBufferData* &pFrameData);
_EXPORT_NETCLIENT_SDK tuint32 NET_CLIENT_RequestAddTag(tuint32 devId, tuint32 chID, tuint32 tagTime,char* discript, int discriptLen,bool bWithDataLen,LPVOID userPara,READ_RECORD_POSITION recordDataPosition,CWaitObserver *pWaitObserver=NULL);

_EXPORT_NETCLIENT_SDK bool  NET_CLIENT_RequestDelTag(tuint32 devId, tuint32 chID, tuint32 tagTime, bool bWithDataLen, READ_RECORD_POSITION recordDataPosition, CBufferData* &pFrameData);
_EXPORT_NETCLIENT_SDK tuint32 NET_CLIENT_RequestDelTag(tuint32 devId, tuint32 chID, tuint32 tagTime, bool bWithDataLen, LPVOID userPara, READ_RECORD_POSITION recordDataPosition, CWaitObserver *pWaitObserver=NULL);

_EXPORT_NETCLIENT_SDK tuint32 NET_CLIENT_RequestRECDataLen(tuint32 dwChID, tuint32 dwStartime, tuint32 dwEndTime, LPVOID userPara, READ_RECORD_POSITION recordDataPosition, CWaitObserver *pWaitObserver=NULL);	//获取数据长度

// 图片搜索
_EXPORT_NETCLIENT_SDK tuint32 NET_CLIENT_RequestSearchPic(tuint32 chId,tuint32 dwEventMask, tuint32 startTime, tuint32 endTime, tuint32 dwPageIndex, tuint32 dwPageSize, CWaitObserver* pWaitObserver, READ_RECORD_POSITION recordDataPosition); // 异步接口
_EXPORT_NETCLIENT_SDK tuint32 NET_CLIENT_RequestPicData(tuint32 chId, tuint32 time, char* pPicName, const int iLen,tuint32 dwImageIndex, tuint32 dwPageIndex, tuint32 dwPageSize, CWaitObserver* pWaitObserver, READ_RECORD_POSITION recordDataPosition); // 异步接口

//车牌搜索
_EXPORT_NETCLIENT_SDK tuint32 NET_CLIENT_RequestPlateMatchList(tuint32 chId, const char* szXML, tuint32 dwLen,  CWaitObserver* pWaitObserver); // 异步接口
_EXPORT_NETCLIENT_SDK tuint32 NET_CLIENT_RequestPlateMatchPicData(tuint32 chId, const char* szXML, tuint32 dwLen, CWaitObserver* pWaitObserver); // 异步接口


//根据通行记录索引查找图片数据
_EXPORT_NETCLIENT_SDK tuint32 NET_CLIENT_RequestPicDataByPmsPassRecordIndex(tuint32 chId, tuint32 nPmsPassRecordIndex, LPVOID waitLPara, CWaitObserver* pWaitObserver);

// 获取存储状态信息
_EXPORT_NETCLIENT_SDK tuint32 NET_CLIENT_RequestStorageInfo(tuint32 msuId, CWaitObserver* pWaitObserver); // 异步接口

// 获取电视墙信息
_EXPORT_NETCLIENT_SDK tuint32 NET_CLIENT_RequestGettingDecoderData(tuint32 decoderId, GUID wallGuid, LPVOID userPara,int userDataLen=0, void* userData=NULL, CWaitObserver *pWaitObserver=NULL);
_EXPORT_NETCLIENT_SDK tuint32 NET_CLIENT_RequestTvWallGettingConfig(tuint32 wallId,LPVOID cmdCode, int userDataLen=0, void* userData=NULL, CWaitObserver *pWaitObserver=NULL);
_EXPORT_NETCLIENT_SDK tuint32 NET_CLIENT_RequestTvWallSavingConfig(tuint32 wallId, LPVOID cmdCode, int userDataLen=0, void* userData=NULL, CWaitObserver *pWaitObserver=NULL);
_EXPORT_NETCLIENT_SDK tuint32 NET_CLIENT_RequestTvWallOption(tuint32 wallId,       LPVOID cmdCode, int userDataLen=0, void* userData=NULL, CWaitObserver *pWaitObserver=NULL);

// 获取智能分析服务信息
_EXPORT_NETCLIENT_SDK tuint32 NET_CLIENT_RequestSetIAEvent(tuint32 srvId, LPVOID cmdCode, int userDataLen=0, void* userData=NULL, LPVOID waitLPara = NULL, CWaitObserver *pWaitObserver=NULL);
_EXPORT_NETCLIENT_SDK tuint32 NET_CLIENT_RequestGetIAEvent(tuint32 srvId, LPVOID cmdCode, int userDataLen=0, void* userData=NULL, LPVOID waitLPara = NULL, CWaitObserver *pWaitObserver=NULL);

//测试邮箱
_EXPORT_NETCLIENT_SDK tuint32 NET_CLIENT_TestSmtp(tuint32 srvId, const char *pBuf, tuint32 dwBufLen, CBufferData* &pFrameData);

//电视墙回放控制
//_EXPORT_NETCLIENT_SDK bool NET_CLIENT_TVWallPlaybackCtrl(ECMS_TVW_PB_CTRL dwOP, tuint32 dwTVWallID, const BYTE* seiral, UCHAR ucOutIndex, UCHAR ucFrameIndex);
//_EXPORT_NETCLIENT_SDK tuint32 NET_CLIENT_TVWallPlaybackCtrl(ECMS_TVW_PB_CTRL dwOP, tuint32 dwTVWallID, const BYTE* seiral, UCHAR len, UCHAR ucOutIndex, UCHAR ucFrameIndex, LPVOID userPara, CWaitObserver* pWaitObserver=NULL);

_EXPORT_NETCLIENT_SDK void NET_CLIENT_AddFileStreamObserver(CStreamCaptureObserver *pStreamObserver);//文件流数据的观察者
_EXPORT_NETCLIENT_SDK void NET_CLIENT_DelFileStreamObserver(CStreamCaptureObserver *pStreamObserver);//文件流数据的观察者

_EXPORT_NETCLIENT_SDK tuint32 NET_CLIENT_DownFileStart(tuint32 dwDestID, const char *szFileName, LPVOID userPara);	//请求下载文件
_EXPORT_NETCLIENT_SDK bool NET_CLIENT_DownFileStop(tuint32 dwStreamID);					//请求停止下载文件

_EXPORT_NETCLIENT_SDK tuint32 NET_CLIENT_UploadFileStart(tuint32 dwDestID, const char *szFileType, const char *szAuthInfo, tuint32 authInfoLen, tuint32 dwFileLen, LPVOID userPara);	//请求上传文件
_EXPORT_NETCLIENT_SDK tuint32 NET_CLIENT_UploadFileData(tuint32 dwStreamID, const char *pBuf, tuint32 dwBufLen);	//上传文件数据,阻塞式
_EXPORT_NETCLIENT_SDK bool NET_CLIENT_UploadFileStop(tuint32 dwStreamID);						//请求停止上传文件

_EXPORT_NETCLIENT_SDK tuint32 NET_CLIENT_RequestTransport(tuint32 dwDestID, const char* szXML, tuint32 dwLen, CBufferData* &pFrameData);//透明数据传输，主要用于xml数据的请求和应答
_EXPORT_NETCLIENT_SDK tuint32 NET_CLIENT_RequestTransport(tuint32 dwDestID, const char* szXML, tuint32 dwLen, LPVOID userPara,CWaitObserver *pWaitObserver=NULL);//透明数据传输，主要用于xml数据的请求和应答

// 报警主机
_EXPORT_NETCLIENT_SDK bool NET_CLIENT_AlarmHostGetStateInfo(tuint32 alarmHostId);
_EXPORT_NETCLIENT_SDK tuint32 NET_CLIENT_AlarmHostControl(tuint32 subsystemOrZoneId, tuint32 controlType, CWaitObserver *pWaitObserver = NULL, tuint16 u16Channel = 0);

_EXPORT_NETCLIENT_SDK tuint32 NET_CLIENT_AcsGetDoorStateInfo(tuint32 jsuId, CWaitObserver* pObserver);
_EXPORT_NETCLIENT_SDK tuint32 NET_CLIENT_AcsGetDoorStateInfo(tuint32 jsuId, tuint32 doorID[64], CWaitObserver* pObserver);
_EXPORT_NETCLIENT_SDK tuint32 NET_CLIENT_AcsControl(tuint32 doorId, NET_PROTOCOL_CMD_DEF controlType, CWaitObserver* pObserver);
_EXPORT_NETCLIENT_SDK tuint32 NET_CLIENT_AcsRecord(tuint32 jsuId,tuint32 nBeginTime,tuint32 nEndTime, NET_PROTOCOL_CMD_DEF controlType, CWaitObserver* pObserver);

//键盘操作接口
_EXPORT_NETCLIENT_SDK bool NET_CLIENT_RequestKbTvWallOption(const tint8* pReqType, const tint8* pReqXml
                                                            , const tuint32& nReqLen, tint8*& pResXml
                                                            , tuint32& nResLen, CWaitObserver *pWaitObserver);

//前端抓图 本接口暂时只支持同步
_EXPORT_NETCLIENT_SDK bool NET_CLIENT_GetJPEGPicture(tuint32 chID,CBufferData* &pFrameData);

//测试连接编码设备
_EXPORT_NETCLIENT_SDK bool NET_CLIENT_SDK_TestLoginDev(tuint32 mduID/*转发服务器id*/,tuint32 devType,char *pDevIP,tuint16 devPort,char *pDevUser,char *pDevPwd,char *pUrl,CBufferData* &pFrameData);

_EXPORT_NETCLIENT_SDK void NET_CLIENT_AddRevCommonInfoObserver(CCommonInfoObserver *pObserver);//接收加密狗信息的观察者
_EXPORT_NETCLIENT_SDK void NET_CLIENT_DelRevCommonInfoObserver(CCommonInfoObserver *pObserver);//接收加密狗信息的观察者

_EXPORT_NETCLIENT_SDK bool NET_CLIENT_AddWaitForStreamCtrlResultObserver(CWaitObserver *pWaitObserver);//添加到视频流结果观察者
_EXPORT_NETCLIENT_SDK bool NET_CLIENT_DeleteWaitForStreamCtrlResultObserver(CWaitObserver *pWaitObserver);//删除视频流结果观察者

_EXPORT_NETCLIENT_SDK bool NET_CLIENT_GetLocalNodeTypeIsDecoder();//TRUE为解码器
_EXPORT_NETCLIENT_SDK tuint32 NET_CLIENT_GetPlatformType();//Enterprise_Type=0企业版,Standard_Type=1标准版,Lite_Type=2单机版
_EXPORT_NETCLIENT_SDK tuint32 NET_CLIENT_GetLocalNodeOtherType();//1为解码器 2为平台SDK

_EXPORT_NETCLIENT_SDK bool NET_CLIENT_RequestKeyFrame(tuint32 dwStreamID,bool waitResult);//实时流请求通道关键帧

//pms融合新增
//   道闸控制
_EXPORT_NETCLIENT_SDK tuint32 NET_CLIENT_BarrierControl(tuint32 dwDestID, tint32 operateType, GUID chnGUID, LPVOID userPara, CWaitObserver *pWaitObserver);
//   红门LED屏_设置显示内容 IP屏
_EXPORT_NETCLIENT_SDK tuint32 NET_CLIENT_LedScreenTextSet(tuint32 dwDestID, const char* text, tuint32 dwLen,bool bAudio, tuint32 nLine, GUID ledGUID, LPVOID userPara, CWaitObserver *pWaitObserver);
//   华安外接485屏_操作区
_EXPORT_NETCLIENT_SDK tuint32 NET_CLIENT_SerialLedScreenTextSet(tuint32 dwDestID, const unsigned char* text, tuint32 dwLen,bool bAudio, tuint32 nLine, GUID camGUID, LPVOID userPara, CWaitObserver *pWaitObserver);

_EXPORT_NETCLIENT_SDK tuint32 NET_CLIENT_SyncLowerCascadePlatformSrc(tuint32 dwDestID, LPVOID userPara, CWaitObserver *pWaitObserver);
//车牌识别的订阅
_EXPORT_NETCLIENT_SDK bool NET_CLIENT_SubVehcle(tuint32 chID,LPVOID userPara,const char* data,tuint32 ilen);
//车牌识别的退订
_EXPORT_NETCLIENT_SDK bool NET_CLIENT_unSubVehcle(tuint32 chID,LPVOID userPara,const char* data,tuint32 ilen);

_EXPORT_NETCLIENT_SDK bool NET_CLIENT_RequestBatchConfig(tuint32 dwDestID, const char* pSendBuf, tuint32 dwLen, CBufferData* &pFrameData);//透明数据传输，主要用于xml数据的请求和应答
_EXPORT_NETCLIENT_SDK tuint32 NET_CLIENT_RequestBatchConfig(tuint32 dwDestID, const char* pSendBuf, tuint32 dwLen, LPVOID userPara,CWaitObserver *pWaitObserver=NULL);//透明数据传输，主要用于xml数据的请求和应答

_EXPORT_NETCLIENT_SDK bool NET_CLIENT_RequestModifyDeviceIp(tuint32 dwDestID, const char* pSendBuf, tuint32 dwLen, CBufferData* &pFrameData);//透明数据传输，主要用于xml数据的请求和应答
_EXPORT_NETCLIENT_SDK tuint32 NET_CLIENT_RequestModifyDeviceIp(tuint32 dwDestID, const char* pSendBuf, tuint32 dwLen, LPVOID userPara,CWaitObserver *pWaitObserver=NULL);//透明数据传输，主要用于xml数据的请求和应答

//手动同步平台各个服务器和设备的时间 客户端下发命令给管理，管理再下发同步时间命令给各个服务器
_EXPORT_NETCLIENT_SDK void NET_CLIENT_SynPlatformTime();
//【INN24】【银行项目需求】
_EXPORT_NETCLIENT_SDK bool NET_CLIENT_GetDevAlarmChannel(tuint32 chID,tuint32 iAlarmType,CBufferData* &pFrameData,BYTE nTriggerType = 0/*0表示设备录像 1表示图片*/);

_EXPORT_NETCLIENT_SDK void NET_CLIENT_SynRSUTime(tuint32 dwTime);
// 设置 权限的指针（mac客户端使用）
_EXPORT_NETCLIENT_SDK void NET_CLIENT_SetAuthorizePtr(void *pAuthorizePtr);

_EXPORT_NETCLIENT_SDK tuint32 NET_CLIENT_PTZ_RequesDeletePreset(tuint32 chID, const char* szXmlPreset, const tuint32 nXmlLen);
//手动录像开始
_EXPORT_NETCLIENT_SDK tuint32 NET_CLIENT_RequestManualRecordStart(tuint32 dwDestID, const char* pSendBuf, tuint32 dwLen, LPVOID userPara,CWaitObserver *pWaitObserver =NULL );

//手动录像关闭
_EXPORT_NETCLIENT_SDK tuint32 NET_CLIENT_RequestManualRecordStop(tuint32 dwDestID, const char* pSendBuf, tuint32 dwLen, LPVOID userPara,CWaitObserver *pWaitObserver =NULL );
_EXPORT_NETCLIENT_SDK void NET_CLIENT_GetCustomInfo(CUSTOM_INFO_CLIENT *pCustomInfo);

//获取电视墙的备份数据
_EXPORT_NETCLIENT_SDK bool NET_CLIENT_GetTVWallsData(tuint32 tvwallID,CBufferData* &pFrameData);
_EXPORT_NETCLIENT_SDK tuint32 NET_CLIENT_GetTVWallsData(tuint32 tvwallID,char* pSendBuf, tuint32 dwLen, LPVOID userPara,CWaitObserver *pWaitObserver=NULL);

_EXPORT_NETCLIENT_SDK bool NET_CLIENT_SetTVWallsData(tuint32 dwDestID, const char* pSendBuf, tuint32 dwLen, CBufferData* &pFrameData);//透明数据传输，主要用于xml数据的请求和应答
_EXPORT_NETCLIENT_SDK tuint32 NET_CLIENT_SetTVWallsData(tuint32 dwDestID, const char* pSendBuf, tuint32 dwLen, LPVOID userPara,CWaitObserver *pWaitObserver=NULL);//透明数据传输，主要用于xml数据的请求和应答

_EXPORT_NETCLIENT_SDK tuint32 NET_CLIENT_CreateQueryPassRecordLog_ByPage(const CFGPACK_DEFINE::PMS_LOG_PASS_RECORD* pLogPassRecord,tuint32 dwStartTime, tuint32 dwEndTime,\
	tuint32 pageIndex/*第几页*/, tuint32 pageSize/*每页条数*/, CBufferData* &pFrameData);
_EXPORT_NETCLIENT_SDK tuint32 NET_CLIENT_CreateQueryPassRecordLog_ByPage(const CFGPACK_DEFINE::PMS_LOG_PASS_RECORD* pLogPassRecord,tuint32 dwStartTime, tuint32 dwEndTime,\
	tuint32 pageIndex/*第几页*/, tuint32 pageSize/*每页条数*/, LPVOID userPara, CWaitObserver *pWaitObserver=NULL);
_EXPORT_NETCLIENT_SDK void NET_CLIENT_DestroyQueryPassRecordLog_ByPage(tuint32 handle);
//通过二进制命令发送xml格式找管理服务器查询和编辑通行记录
_EXPORT_NETCLIENT_SDK tuint32 NET_CLIENT_XmlQueryPmsPassRecord(const char* pSendBuf, tuint32 dwLen, LPVOID userPara,CWaitObserver *pWaitObserver);//异步接口
_EXPORT_NETCLIENT_SDK tuint32 NET_CLIENT_XmlEditPmsPassRecord(const char* pSendBuf, tuint32 dwLen, LPVOID userPara,CWaitObserver *pWaitObserver);//异步接口
_EXPORT_NETCLIENT_SDK tuint32 NET_CLIENT_XmlEditPmsParkInfo(const char* pSendBuf, tuint32 dwLen, LPVOID userPara,CWaitObserver *pWaitObserver);//异步接口

#endif



