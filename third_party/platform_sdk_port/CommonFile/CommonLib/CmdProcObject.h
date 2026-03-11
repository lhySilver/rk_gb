
#ifndef __CMDPROC_OBJECT_H_
#define __CMDPROC_OBJECT_H_

#include <list>
#include "NetCommSDK.h"
#include "NetProtocolDefine_ForNVMS.h"
#include "MySingleLock.h"
#include "Interlocked.h"

using namespace NVMS_NET_PROTOCOL;

class CCmdProcParameter;

//////////////////////////////////////////////////////////////////////////
///////////网络失败的返回值
typedef enum _ecms_net_ctrl_fail_
{
	ECMS_NETERR_SUCCESS = 0,/////成功返回值
	//关于命令处理
	ECMS_CMDPROC_OBJECT_NONE,//找不到对于的命令处理对象
	ECMS_NETERR_DISCONNECT,//网络已断开,CmdProcMap执行命令不会返回此值，内部使用
	ECMS_NETERR_PARAMETER_ERROR,	//参数错误，具体命令解析命令参数错误时返回此值
	ECMS_CMDPROC_EXECUTECMD_PROC,//未处理ExecuteCommand函数

	ECMS_NETERR_UNKNOWN = 0xFFFFFFFF,/////未知错误返回值
}ECMS_NET_CTRL_FAIL;

class _EXPORT_NETCOMM_CLASS CCmdProcParameter
{
public:	
	////////构造函数
	CCmdProcParameter();

	//减少计数
	tint32	DecReference();
	//增加计数
	tint32	AddReference();

	void	CreateEventObject();
	void	DestroyEventObject();
	tuint32	WaitEvent(tuint32 timeout);
	void	SetEvent();
	const void*	GetEvent();

	void	CreateShareData(const void *pFrameData, int iBufLen);
	void	ReleaseShareData();
	const void*	GetShareData();
	const void* GetShareDataData();
	int GetShareDataLen();

protected:
	///////析构函数
	virtual ~CCmdProcParameter();

public:
	bool    bRetType; //结果包括成功或失败  成功为true失败为false
	tuint32   dwReserverID;////保留ID

private:
	mutable void* m_reference;
	void  *m_EventHandle;//没有信号的事件句柄，或者进来后程序主动设置一次没有信号，等回复来了设置有信号
	void  *m_pSharedData;//后面跟的数据的信息，外面取到数据以后会主动调用一次Release的，里面只负责创建就OK了
};

class _EXPORT_NETCOMM_CLASS CCmdProcParam
{
public:
	explicit CCmdProcParam(CCmdProcParameter* pCmdPara,const void *pCmdData=NULL,tuint32 cmdDataLen=0,bool autoRelease=TRUE);
	virtual ~CCmdProcParam();

	CCmdProcParameter* GetShareData();

	const CCmdProcParameter* GetShareData() const;
	const void *GetCmdData() const;
	tuint32 GetCmdDataLen() const;
	//减少计数
	tint32 DecReference();
	//增加计数
	tint32 AddReference();
protected:
	CCmdProcParameter* m_pCmdPara;
	//共享对象
	void *	   m_CmdData;
	tuint32			   m_CmdDataLen;
	bool		m_AutoRelease;
private:
	CCmdProcParam();
	friend class CCmdProcMap;
	friend class CNVMSNetNode;
	//创建执行命令的ID CCmdProcMap独有

	mutable void* m_reference;
};

class _EXPORT_NETCOMM_CLASS CCmdProcParamLogin : public CCmdProcParam
{
public:
    CCmdProcParamLogin(CCmdProcParameter* pShareData, const ECMS_NET_LOGIN_INFO* pLoginInfo);
    const ECMS_NET_LOGIN_INFO * m_pLoginInfo;
private:
};

//报警状态参数对象
class _EXPORT_NETCOMM_CLASS CCmProcParamAlarmInfo : public CCmdProcParam
{
public:
    CCmProcParamAlarmInfo(GUID nodeID,tuint32 nodeType,tuint32 alarmType,BYTE alarmIndex,BYTE alarmState,
        CCmdProcParameter* pShareData);
	void SetAlarmType(tuint32 alarmType);

    tuint32	m_nodeType;
    GUID	m_nodeID;////////指设备节点的ID
    tuint32 m_alarmType;//报警类型
	//从0开始的编号,由于设备内部不太好获得SensorID 通道ID等，所以在从设备出状态时会用到此变量
	//报警主机中用到的是子系统和防区，编号从1开始 add by wangxin
    unsigned char m_byIndexNum;
    unsigned char m_byisAlarm;///非0是报警状态，0是非报警状态

    ECMS_GPS_INFO m_GPSInfo;
	ECMS_ALARM_INFO m_AlarmInfo;//add by wangxin
};

//picture
// class _EXPORT_NETCOMM_CLASS CCmdProcParamPicture : public CCmdProcParam
// {
// public:
// 	CCmdProcParamPicture(GUID nodeID, tuint32 nodeType, BYTE alarmState, CCmdProcParameter* pShareData);
// 	tuint32	m_nodeType;
// 	GUID	m_nodeID; //指设备节点的ID
// 	unsigned char m_byisAlarm;///非0是报警状态，0是非报警状态
// 
// 	tuint32 m_passTime;//char m_szFileName[256];
// 	char m_szBuf[256*1024];//JPEG
// 	tuint32 m_bufLen;
// };


class _EXPORT_NETCOMM_CLASS CCmdProcParamUpdateProcessEvent : public CCmdProcParam
{
public:
	CCmdProcParamUpdateProcessEvent(GUID nodeID,CCmdProcParameter* pShareData);
	GUID	m_nodeChlID; //指通道节点的ID
	PROCESS_INFO m_processInfo;
};

//pass event
class _EXPORT_NETCOMM_CLASS CCmdProcParamPassevent : public CCmdProcParam
{
public:
	CCmdProcParamPassevent(GUID nodeID,CCmdProcParameter* pShareData);
	GUID	m_nodeChlID; //指通道节点的ID
	CFGPACK_DEFINE::PMS_LOG_PASS_RECORD m_passRecord;
	CFGPACK_DEFINE::PMS_LOG_PASS_RECORD_PICTURE m_passRecordPicture;
};

class _EXPORT_NETCOMM_CLASS CCmdProcParamNewOnlineChnUpdate : public CCmdProcParam
{
public:
	CCmdProcParamNewOnlineChnUpdate(GUID nodeID,CCmdProcParameter* pShareData);
	GUID	m_nodeChlID; //指通道节点的ID
	tuint32 nOnline;
};

class _EXPORT_NETCOMM_CLASS CCmdProcParamPasseventWithNoPicture : public CCmdProcParam
{
public:
	CCmdProcParamPasseventWithNoPicture(GUID nodeID,CCmdProcParameter* pShareData);
	GUID	m_nodeChlID; //指通道节点的ID
	CFGPACK_DEFINE::PMS_LOG_PASS_RECORD m_passRecord;
};

//区域入侵检测
class _EXPORT_NETCOMM_CLASS CCmdProcParamPermiterevent : public CCmdProcParam
{
public:
	CCmdProcParamPermiterevent(GUID nodeID,CCmdProcParameter* pShareData);
	GUID	m_nodeChlID; //指通道节点的ID
	CFGPACK_DEFINE::PERMITER_PICTURE m_sourcePic;	//原图和目标图暂时都只弄一个
	CFGPACK_DEFINE::PERMITER_PICTURE m_DstPic;
};


class _EXPORT_NETCOMM_CLASS CCmdProcParamTrajectRect : public CCmdProcParam
{
public:
	CCmdProcParamTrajectRect(GUID nodeID,CCmdProcParameter* pShareData);
	GUID	m_nodeChlID; //指节点的ID
	CFGPACK_DEFINE::TRAIECT_RECT m_trajectRect;
};

class _EXPORT_NETCOMM_CLASS CCmdProcParamIPCPushSmartData : public CCmdProcParam
{
public:
    CCmdProcParamIPCPushSmartData(CCmdProcParameter* pShareData, const char*  pCfgBuf,  int CfgBufLen);
};

class _EXPORT_NETCOMM_CLASS CCmdProcParamFrNvrPushSmartData : public CCmdProcParam
{
public:
	CCmdProcParamFrNvrPushSmartData(CCmdProcParameter* pShareData, const char*  pCfgBuf, int CfgBufLen);
};

//不要轻易改变此类，因为NetComm里面也用到了，改变后要同时编译
class _EXPORT_NETCOMM_CLASS CCmdProcParamConfig : public CCmdProcParam
{
public:
    CCmdProcParamConfig(CCmdProcParameter* pShareData, const char*  pCfgBuf,  int CfgBufLen) ;
    GUID  m_DestNodeID;
	//tuint32 m_nChlIndex;//监控点索引
private:
};

class _EXPORT_NETCOMM_CLASS CCmdProcAnsWithMduParam : public CCmdProcParam
{
public:
	CCmdProcAnsWithMduParam(CCmdProcParameter* pShareData, const char*  pCfgBuf, int CfgBufLen);

	Ans_Mdu_CMD_INFO	m_cmdDataInfo;
	GUID				m_DestNodeID;
};

class _EXPORT_NETCOMM_CLASS CCmdProcVehcleWithMduParam : public CCmdProcParam
{
public:
	CCmdProcVehcleWithMduParam(CCmdProcParameter* pShareData, const char*  pCfgBuf, int CfgBufLen);

	CMD_VEHCLETOPIC	m_cmdDataInfo;
	GUID			m_DestNodeID;
};

class _EXPORT_NETCOMM_CLASS CCmdProcMsuWithMduParam : public CCmdProcParam
{
public:
	CCmdProcMsuWithMduParam(CCmdProcParameter* pShareData, const char*  pCfgBuf, int CfgBufLen);

	Mdu_To_Msu_CMD_INFO	m_cmdDataInfo;
	GUID			m_DestNodeID;
};

class _EXPORT_NETCOMM_CLASS CCmdProcMduWithFrNvrParam : public CCmdProcParam
{
public:
	CCmdProcMduWithFrNvrParam(CCmdProcParameter* pShareData, const char*  pCfgBuf, int CfgBufLen);

	Mdu_FrNvr_CMD_INFO	m_cmdDataInfo;
	GUID				m_destNodeGuid;
};

//设备录像状态参数对象
class _EXPORT_NETCOMM_CLASS CCmProcParamDevRecordInfo : public CCmdProcParam
{
public:
    CCmProcParamDevRecordInfo(GUID nodeID,tuint32 recordState,CCmdProcParameter* pShareData);
    GUID	m_nodeID;
    tuint32 m_RecordState;
};

class _EXPORT_NETCOMM_CLASS CCmdProcParamPlayStreamTWDevice : public CCmdProcParam
{
public:
    CCmdProcParamPlayStreamTWDevice(GUID &taskGUID, GUID destID,GUID routeGUID,const void *pBuf,tuint32 bufLen,CCmdProcParameter* pShareData,bool autoRelease=TRUE) :
      CCmdProcParam(pShareData,pBuf,bufLen,autoRelease),m_TaskGUID(taskGUID),m_RouteGUID(routeGUID),m_DestID(destID),m_Errorcode(0)
      {
          m_Errorcode = 0;
      }
      GUID m_TaskGUID;
      GUID m_RouteGUID;
      GUID m_DestID;
      tuint32 m_Errorcode;
private:
};


/************************************************************************/
/*          CCmdProcParamAudioStream,对讲音频数据                     */
/*  目前只有客户端发送音频到设备才会用到，从设备发送音频过来的是设备内部定义的，借用回放的*/
/************************************************************************/
class _EXPORT_NETCOMM_CLASS CCmdProcParamAudioStream : public CCmdProcParam
{
public:
    CCmdProcParamAudioStream(const void *pCmdData,tuint32 cmdDataLen) : \
        CCmdProcParam(NULL, pCmdData, cmdDataLen)
    {
        m_destNodeID = GUID_NULL;
    }
    GUID m_destNodeID;	//设备节点ID
};

class _EXPORT_NETCOMM_CLASS CCmdProcParamSynRSUTime : public CCmdProcParam
{
public:
	CCmdProcParamSynRSUTime(CCmdProcParameter* pShareData, const char*  pCfgBuf, int CfgBufLen);
};

class _EXPORT_NETCOMM_CLASS CCmdProcParamManualRecord : public CCmdProcParam
{
public:
	CCmdProcParamManualRecord(CCmdProcParameter* pShareData, const char*  pCfgBuf,  int CfgBufLen) ;
	GUID  m_DestNodeID;
};

class CNetNode;
//////////////////////////////////////////////////////////////////////////
class _EXPORT_NETCOMM_CLASS CCmdProcObject
{
protected:
	explicit CCmdProcObject(NET_PROTOCOL_CMD_DEF request, bool hasReply);

	virtual ~CCmdProcObject();

public://以下为继承类应该实现的接口
	friend class CCmdProcMap;
	//执行命令,应用层调用，每个命令自己实现具体方法,只有参数错误返回FALSE，否则返回TRUE，通过事件获得结果
	virtual ECMS_NET_CTRL_FAIL ExecuteCommand(const CCmdProcParam* pParam) {ASSERT(FALSE);return ECMS_CMDPROC_EXECUTECMD_PROC;}

	//收到命令请求到达目的地后此函数会自动被调用,具体命令必须自己处理此函数
	virtual void	OnRequestCommand(const void *lpCmdData,size_t cmddatalen){ ;}

	//收到需要转发的命令请求后如果没有到达目的地此函数会自动被调用, 具体命令必须自己处理此函数，返回true代表继续往前发，否则到此为止，自己处理返回
	virtual bool	OnForwardRequestCommand(const void *lpCmdData,size_t cmddatalen){return true;}

	//请求向下级节点转发时会经过此函数，大部分情况下不需要处理此函数
	virtual void	OnSendRequestCommand(const void *lpData,tuint32 datalen);

	//应答到达源后，这两个函数会被调用,有应答的命令必须自己处理此函数
	virtual void OnReplyCommandSuccess(const void *lpData,tuint32 datalen,bool hasreached){;}
	virtual void OnReplyCommandFail(const void *lpData,tuint32 datalen,bool hasreached){;}

	//应答通过下级节点继续转发时会调用以下两个函数，只有转发指令需要接受这两个函数
	//默认不做任何处理，如果需要继承类自己实现
	virtual void OnSendCommandSuccess(const void *lpData,tuint32 datalen){;}
	virtual void OnSendCommandFail(const void *lpData,tuint32 datalen){;}
	/*
	//注意此处并没有给请求继续转发时的处理机会，一般命令在收到请求端处理就可以了
	*/
	//生成一份自己的拷贝
	virtual CCmdProcObject*	   CreateSelf() =0;

	//返回命令类型，在命令具有开始和停止的概念时返回是停止还是结束命令
	virtual TASKSTATE_TYPE GetTaskState(tuint32 dwCmd) const { return TASKSTATE_TYPE_NULL; }

	//只有任务型命令的Start才需要处理
	virtual NET_PROTOCOL_CMD_DEF GetStopCommand() const { return ECMS_CMD_NULL; }
	virtual NET_PROTOCOL_CMD_DEF GetCompleteCommand() const { return ECMS_CMD_NULL; }
	virtual CCmdProcParam* GetStopCommandParam(ECMS_PACKCMD_EXTEND_TASK *pExtTask) const { return NULL; }
	virtual CCmdProcParam* GetCompleteCommandParam(ECMS_PACKCMD_EXTEND_TASK *pExtTask) const { return NULL; }

public://以下只有命令处理模块调用的接口

	//要转发请求命令，在转发过程中使用此接口
	virtual bool	RequestCommandForward(const void *pCmdBuf,tuint32 cmdBufLen);
	//要转发应答命令，收到应答命令后，需要转发的直接调用下节点的此接口
	virtual bool	ReplyCommandForward(const void  *pCmdBuf,tuint32 cmdBufLen);
	//收到请求命令后此函数会自动被调用
	virtual bool				OnRequestCommandProc(const void *lpCmdData,tuint32 cmddatalen,bool hasreached,bool bNotSupport=false);//bNotSupport是否是不支持的命令
	//收到应答命令后
	virtual void 				OnReplyCommandProc(const void *lpCmdData,tuint32 cmddatalen,bool hasreached);
	//响应断开连接
	virtual bool				NetDisconnectNeedReply();//网络断开时是否需要模拟应答
	virtual tint32				GetReplyCmdLen(const ECMS_NET_REPLY_RESULT *lpReplyResult);
	virtual tint32				GetReplyCmdBuf(const ECMS_NET_REPLY_RESULT *lpReplyResult,BYTE *pCmdBuf);
	virtual tint32				GetReplyCmdBuf(const ECMS_NET_REPLY_RESULT *lpReplyResult,BYTE *pCmdBuf,tuint32 nCmd);

public://以下为继承类需要使用的接口

	//应用层实现ExecuteCommand命令后最终调用这两个函数执行命令
	virtual ECMS_NET_CTRL_FAIL  ExecuteCommand(const void *lpData,tuint32 datalen,const CCmdProcParameter *pEventSharedData);
	virtual ECMS_NET_CTRL_FAIL  ExecuteCommand(const void *lpData,tuint32 datalen,GUID destID,const CCmdProcParameter *pEventSharedData);
	virtual ECMS_NET_CTRL_FAIL  ExecuteCommand(const void *lpData,tuint32 datalen,GUID destID,GUID &taskguid,const CCmdProcParameter *pEventSharedData);
	ECMS_NET_CTRL_FAIL  ExecuteCommandBase(const void *lpData,tuint32 datalen,GUID destID,const CCmdProcParameter *pEventSharedData);
	ECMS_NET_CTRL_FAIL  ExecuteCommandTask(const void *lpData,tuint32 datalen,GUID destID,const GUID &taskguid,const CCmdProcParameter *pEventSharedData);
	//应用程序处理完请求后调用以下两个函数作应答
	virtual bool				ReplyCommandSuccess(const void *lpData,tuint32 datalen);
	//专门为透传命令所提供，目的是在于，当是透传的时候，命令号依然是之前的命令号,如智能命令，就传智能命令
	virtual bool				ReplyCommandSuccessByTransPort(const void *lpData,tuint32 datalen,tuint32 nCmd);
	virtual bool				ReplyCommandFail(const ECMS_NET_REPLY_RESULT *lpReplyResult);
	//回复指定错误命令，用于透传。
	virtual bool				ReplyCommandFail(const ECMS_NET_REPLY_RESULT *lpReplyResult,tuint32 nCmd);
public:		
	//查询接口
	tuint32	GetCommandID();

	//得到任务ID，在处理请求时会用到
	GUID	GetTaskGUID() const; 
	//得到源节点ID，在处理请求时会用到
	GUID	GetSrcID();
	GUID GetRouteGUID();
	////返回命令处理类型
	NET_PROTOCOL_CMD_DEF GetRequestCommand() const;
	tuint32 GetReplyCommandSuccess() const ;
	tuint32 GetReplyCommandFail() const ;
	bool HasReply();
	void SetExeCmdID(tuint32 cmdID);
	//设置所有者
	void SetOwner(void* pOwner) ;
	//获得所有者
	void* GetOwner();

	//获取命令处理类型
	CMDPROC_TYPE  GetCmdProcType(tuint32 procType) const;

	//减少计数
	tint32 DecReference(int index=0);

	//增加计数
	tint32 AddReference(int index=0);
protected:
	void Destroy();
	bool Initial(void* pNetDispath, CNetNode* pCurNetNode);
	virtual bool ReplySuccess(const void *lpData,tuint32 datalen,bool isDevice);
	virtual bool ReplyFail(const ECMS_NET_REPLY_RESULT *lpReplyResult,bool isDevice);
	virtual bool ReplyFail(const ECMS_NET_REPLY_RESULT *lpReplyResult,bool isDevice,tuint32 nCmd);
	CNetNode* GetNetNode() ;

	//获得转发对象
	void* GetNetDispatch();

	//设置命令处理回复事件
	bool SetNetCmdProcEvent(const CCmdProcParameter *pEventSharedData);
	void ClearNetCmdProcEvent();

	//设置命令处理回复事件
	bool SetNetCmdProcEvent(const void* pBuffer, size_t len,bool bRetType);
	bool SetNetCmdProcEventLocalNetDisconnect();

	//发送命令
	bool SendCommandEx(const void* pBuffer , size_t len);

	//填充命令头信息,此函数只可能在执行命令里面使用
	__inline void FillECMDPack(ECMS_PACKCMD &packCMD, tuint32 dwDataLen, EXTEND_CMD_TYPE bExtenddInfoType = EXTEND_CMD_NULL);
	virtual bool SendExecuteCommand(ECMS_PACKCMD &packCmd,ECMS_PACKCMD_EXTEND *pExtCmd,
		tuint32 extCmdLen,const void *lpData,tuint32 datalen);
	//命令处理对象的容器
	BYTE	*m_pCmdBuf;//处理请求到应答之间时做的缓存，这要求处理请求时对象必须复制
	tuint32	m_BufLen;//BUF长度
	tuint32 m_replySuccessCmd;
	tuint32 m_replyFailCmd;
	NET_PROTOCOL_CMD_DEF m_requestCmd;
	tuint32 m_cmdID;
	GUID m_SrcNodeID;
	GUID m_DstNodeID;
	bool  m_IsExeCmd;//此命令是执行命令时生成的
private:
	void SetDeviceCommandInfo(ECMS_PACKCMD &packCmd,ECMS_PACKCMD_EXTEND *pExtCmd, \
		tuint32 extCmdLen,const void *lpData,tuint32 datalen);

	virtual bool SendExeCommand(ECMS_PACKCMD &packCmd,ECMS_PACKCMD_EXTEND *pExtCmd, \
		tuint32 extCmdLen,const void *lpData,tuint32 datalen,bool isDevice);

	CNetNode* m_pCurNetNode;//当前网络节点
	void *m_MemberValInfo;
	GUID  m_TaskGUID;
};

//////////////////////////////////////////////////////////////////////////
#ifndef DECLARE_CMDPROC_OBJECT
#define DECLARE_CMDPROC_OBJECT(className) \
class className : public CCmdProcObject\
{\
public:\
	explicit className(NET_PROTOCOL_CMD_DEF request);\
	virtual ~className() {}\
	virtual ECMS_NET_CTRL_FAIL ExecuteCommand(const CCmdProcParam* pParam);\
	virtual void OnRequestCommand(const void* pInBuffer, size_t len);\
	virtual void OnReplyCommandSuccess(const void *lpData,tuint32 datalen,bool hasreached);\
	virtual void OnReplyCommandFail(const void *lpData,tuint32 datalen,bool hasreached);\
	virtual CCmdProcObject*	   CreateSelf(){return new className(GetRequestCommand());};\
};
#endif

#ifndef DECLARE_CMDPROC_OBJECTEX
#define DECLARE_CMDPROC_OBJECTEX(className) \
class className : public CCmdProcObject\
{\
public:\
	explicit className(NET_PROTOCOL_CMD_DEF request);\
	virtual ~className() {}\
	virtual ECMS_NET_CTRL_FAIL ExecuteCommand(const CCmdProcParam* pParam);\
	virtual void OnRequestCommand(const void* pInBuffer, size_t len);\
	virtual bool OnForwardRequestCommand(const void *lpCmdData,size_t cmddatalen);\
	virtual void OnSendCommandSuccess(const void *lpData,tuint32 datalen);\
	virtual void OnSendCommandFail(const void *lpData,tuint32 datalen);\
	virtual CCmdProcObject*	   CreateSelf(){return new className(GetRequestCommand());};\
	virtual TASKSTATE_TYPE GetTaskState(tuint32 dwCmd) const;\
	virtual NET_PROTOCOL_CMD_DEF GetStopCommand() const;\
	virtual NET_PROTOCOL_CMD_DEF GetCompleteCommand() const;\
	virtual CCmdProcParam* GetStopCommandParam(ECMS_PACKCMD_EXTEND_TASK *pExtTask) const;\
	virtual CCmdProcParam* GetCompleteCommandParam(ECMS_PACKCMD_EXTEND_TASK *pExtTask) const;\
};
#endif

#ifndef DECLARE_CMDPROC_OBJECTDEVICE
#define DECLARE_CMDPROC_OBJECTDEVICE(className) \
class className : public CCmdProcObject\
{\
public:\
	explicit className(NET_PROTOCOL_CMD_DEF request);\
	virtual ~className() {}\
	virtual ECMS_NET_CTRL_FAIL ExecuteCommand(const CCmdProcParam* pParam);\
	virtual void OnSendRequestCommand(const void *lpData,tuint32 datalen);\
	virtual CCmdProcObject*	   CreateSelf(){return new className(GetRequestCommand());};\
};
#endif

#endif //__CMDPROC_OBJECT_H_
