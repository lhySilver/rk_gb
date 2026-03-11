
///////////////////////////NetCommSDK.h

#ifndef _NET_COMM_SDK_H
#define _NET_COMM_SDK_H

#ifdef WIN32 

#ifdef NET_COMM_SDK_EXPORT_API//动态库编译
#define _EXPORT_NETCOMM_CLASS __declspec(dllexport)
#define _EXPORT_NETCOMM_SDK  __declspec(dllexport)
#else
#ifdef NET_COMM_SDK_EXPORT_STATIC//静态库编译和使用
#define _EXPORT_NETCOMM_CLASS 
#define _EXPORT_NETCOMM_SDK 
#else
#define _EXPORT_NETCOMM_CLASS __declspec(dllimport)
#define _EXPORT_NETCOMM_SDK  __declspec(dllimport)
#endif
#endif

#else
#define _EXPORT_NETCOMM_CLASS __attribute__ ((visibility("default")))
#define _EXPORT_NETCOMM_SDK __attribute__ ((visibility("default")))
#endif

#include "NetProtocolProc.h"
#include "CmdProcObject.h"
#include "TypeCheck.h"

struct _EXPORT_NETCOMM_CLASS NET_NODE_INFO
{
	NET_NODE_INFO();
	~NET_NODE_INFO(void);
	NET_NODE_INFO(const NET_NODE_INFO& node);
	NET_NODE_INFO& operator = (const NET_NODE_INFO& node);

        bool     m_localNodeIsClient;//////
        tint32   m_hNetCommunication;//////////////网络通信的标识
        tuint32  m_localID;/////本地网络节点的ID，具有唯一性
		
		//网络协议版本号，用于不同版本下发不同的协议结构体
		tuint32 m_netprotrocolver;

	/////////////////////////////////
        char m_strName[256];
        char m_strUserName[64];
        char m_strPassword[64];

	///////////////////////////////////////////////目标节点的信息
	GUID m_remoteNodeID;////////////
	CTypeCheck::ECMS_NODE_TYPE_DEF m_remoteNodeType;//节点类型 NETNODE_TYPE
	char m_strRemoteIPAddr[64];///////得到远端的IP地址
	tuint16 m_remoteConnectedPort;///////////
	time_t  m_LoginTime;//////节点上线的时间
	bool m_bIsTestLogin;//测试登录节点，是测试登录节点的时候初始化buf开小一点

private:
	void Copy(const NET_NODE_INFO& node);
};

//////////////////////////////////////////////////////////////////////////
class CNetProtocolProc;
class _EXPORT_NETCOMM_CLASS CNetNode
{
	friend class CLoginCtrl;
	friend class CCmdProcGetConfigS;
	friend class CCmdProcUpdatePartitionInfoRSU;
	friend class CCmdProcSaveConfigS;
	friend class CCmdProcReady;
	friend class CCmdProcModifyPassWord_S;
	friend class CAppCommonCtrl;
	friend class CCmdProcObject;

public:
	//执行命令, 在连接对的上节点调用, 非阻塞方式，可以创建事件进行等待
	virtual bool ExecuteCommand(NET_PROTOCOL_CMD_DEF cmdType, const CCmdProcParam* pParam) = 0;

	//执行命令，阻塞方式，直接有结果后返回
	//如果不需要返回结果的情况下调用此函数（pParam->m_pCmdPara=NULL或pParam=NULL），返回值的含义是否已经把命令成功发送出去
	//如果需要返回结果的情况下调用此函数（pParam->m_pCmdPara!=NULL且pParam!=NULL），返回值的含义应答结果
	virtual bool ExecuteCommandWaitForResult(NET_PROTOCOL_CMD_DEF cmdType, const CCmdProcParam* pParam) = 0;

	virtual bool ExecuteCommandWithDestNode(NET_PROTOCOL_CMD_DEF cmdType,const GUID& destGuid, const CCmdProcParam* pParam) = 0;
	virtual bool ExecuteCommandWithDestNodeNoResult(NET_PROTOCOL_CMD_DEF cmdType,const GUID& destGuid, const CCmdProcParam* pParam) = 0;
	virtual bool ExecuteCommandTaskWithDestNode(NET_PROTOCOL_CMD_DEF cmdType, const GUID& DestNode,const GUID& taskId,const CCmdProcParam* pParam) = 0;
	//登陆登出
	virtual bool LogOut()=0;

	//现场流状态处理
	virtual void LiveStreamStop(const GUID &taskGUID) = 0;

	virtual bool LiveStreamStart(const GUID &taskGUID, const NET_STREAM_CTRL_INFO& streamCtrl) = 0;

	//回放流状态处理
	virtual bool PlayStreamStart(const GUID &taskGUID) = 0;

	//清除通道状态，停止现场流时，将要调用
	virtual void PlayStreamStop(const GUID &taskGUID) = 0;

	//获取节点ID
	virtual GUID GetGUID() const = 0;// 
	virtual tuint32 GetID()  const = 0;// 

	virtual void UpdateID(tuint32 id)  = 0;//设置网络节点的int型ID，保持与本地节点一致，初始化时可能值为0或值不对，比如刚连接管理服务器时管理的节点

	//获得节点类型
	virtual CTypeCheck::ECMS_NODE_TYPE_DEF GetNodeType() const =0;

	//获取节点信息
	virtual NET_NODE_INFO GetInfoClass() const = 0;
	//获取登录结构体
	virtual ECMS_NET_LOGIN_INFO GetLoginStruct() const = 0;

	//得到通道节点，只有设备才会被调用
	virtual int GetChNodeIDVecotr(GUID pChID[])=0;
	virtual int GetChAlarmInNodeIDVecotr(GUID pChAlarmIn[])=0;
	virtual int GetChAlarmOutNodeIDVecotr(GUID pChAlarmOut[])=0;
	//设置通道节点，只有设备才会被调用
	virtual void SetChNodeIDVecotr(GUID *pChID,tuint32 nCount)=0;//nCount表示pChID有多少个GUID
	virtual void SetChAlarmInNodeIDVecotr(GUID *pChID,tuint32 nCount)=0;
	virtual void SetChAlarmOutNodeIDVecotr(GUID *pChID,tuint32 nCount)=0;

	//////////////////////////////////////////////////////////////////////////
	virtual bool NodeIsClient() const =0;

	//节点的状态信息
	virtual bool IsConnected() const =0;
	virtual bool IsLogined() const =0;
	virtual bool IsLogouted() const =0;
	virtual tuint32 GetReadyParam(GUID &CfgGUID) = 0;
	virtual bool SetReadyParam(tuint32 readyValue, const GUID &CfgGUID) = 0;

	virtual GUID GetCurrAppNodeID()=0;
	virtual tuint32 GetCurrAppNodeType()=0;

	virtual bool GetPeerIP(sockaddr_in &addrInfo)=0;
	virtual bool GetLocalIP(sockaddr_in &addrInfo)=0;

	virtual bool SendRawData(const void* pBuffer, size_t len) = 0;

	//释放节点
	virtual tint32 DecReference(int nID = 0) = 0;

	//增加共享次数
	virtual tint32 AddReference(int nID = 0) = 0;

	virtual bool GetProductInfo(char *szProductInfo, tuint32 nBufLen) = 0;

	virtual void SetProductInfo(const char *szProductInfo) = 0;

	virtual int getLoginNonce() = 0;
	virtual void setLoginNonce(int loginNonce) = 0;

	virtual bool IsBackup() = 0;

	virtual void SetBackup(bool bBackup) = 0;

	virtual bool GetAESKey(char *szAESKey, tuint32 nBufLen) = 0;
	virtual void SetAESKey(const char *szAESKey,int len) = 0;

	virtual void SetIsDecoder(bool ) = 0;
	virtual bool GetIsDecoder() = 0;

	//更新节点信息
	virtual bool UpdateNodeInfo(const NET_NODE_INFO& info) = 0;

protected:
	CNetNode(void);
	virtual ~CNetNode(void) = 0;
};

/************************************************************************/
/*                                                                      */
/************************************************************************/
#include "StateObservable.h"
#include "NetNodeIterator.h"
#include "BasicCmdProcObject.h"

enum NET_COMM_ERROR_CODE
{
        NET_COMM_ERROR_SUCCESS=0x0,
        NET_COMM_ERROR_BASE=0x20000000, ////用户错误都需要这个位为1
};

class CRouteCheckBase
{
public:
	CRouteCheckBase(void){};
	virtual ~CRouteCheckBase(void){};

public:
        virtual GUID CheckRoute(GUID sourceID, GUID destID, GUID first, GUID curNode, NVMS_NET_PROTOCOL::ROUTE_DIRECT_DEF routeDirect)=0;
};

//控制本地软件和其它节点之间的连接使用的Buf大小，返回的是单位为8K的BUF数量  数据发送是否使用公共线程　
class CNetBufferCtrlBase
{
public:
	CNetBufferCtrlBase(void){};
	virtual ~CNetBufferCtrlBase(void){};

	/************************************************************************/
	/* 参数说明：
	localIsConnected :    TRUE表示连接方向为远程节点连接本节点，　否则为本节点连接远程节点，
	remoteNodeType　：　　远程节点的类型（表示与本节点有直接网络连接的节点的类型）
    ************************************************************************/
        virtual tuint32 GetSendBufferSize(tuint32 remoteNodeType, bool localNodeIsClient)=0;//发送BUF
        virtual tuint32 GetRecvBufferSize(tuint32 remoteNodeType, bool localNodeIsClient)=0;//接受BUF
        virtual bool IsUsingCommonSendThread(tuint32 remoteNodeType, bool localNodeIsClient) = 0;//////是否使用公共发送线程发送Buffer
};

//状态转发控制，即节点连接状态、录像状态、设备录像状态、报警状态等根据对方节点类型判断是否需要转发过去
class CStateDispatchCtrlBase
{
public:
	CStateDispatchCtrlBase(void){};
	virtual ~CStateDispatchCtrlBase(void){};

	/************************************************************************/
	/* 参数说明：
	isLocalNodeState：　TRUE表示本地直连节点，　FALSE表示为网络上转发过来的节点
	localNodeIsClient :   TRUE表示连接方向为本节点连接远程节点，否则为远程节点连接本节点
	remoteNodeType　：　　远程节点的类型（表示与本节点有直接网络连接的节点的类型）
	statenodetype　：　　状态节点的类型
    ************************************************************************/
        virtual bool ConnectStateNeed(bool isLocalNodeState, bool localNodeIsClient, tuint32 remoteNodeType, tuint32 statenodetype)=0;
        virtual bool RecordStateNeed(bool isLocalNodeState, bool localNodeIsClient, tuint32 remoteNodeType, tuint32 statenodetype)=0;
        virtual bool DevRecStateNeed(bool isLocalNodeState, bool localNodeIsClient, tuint32 remoteNodeType, tuint32 statenodetype)=0;
        virtual bool AlarmStateNeed(bool isLocalNodeState, bool localNodeIsClient, tuint32 remoteNodeType, tuint32 statenodetype)=0;
		virtual bool AlarmStateNeedEx(bool isLocalNodeState, bool localNodeIsClient, tuint32 remoteNodeType, tuint32 statenodetype, BYTE State, ALARM_INDEX alarmIndex)
		{
			return AlarmStateNeed(isLocalNodeState, localNodeIsClient, remoteNodeType, statenodetype);
		}

	//是否是测试连接节点，测试连接节点不用发送状态
	virtual bool IsTestLoginGUID(GUID nodeID){return false;};
};

//网络节点状态本地维护
class CNetNodeStateObserver
{
public:
	CNetNodeStateObserver(void){};
	virtual ~CNetNodeStateObserver(void){};

public:
        virtual void Logout(CNetNode *pNetNode)=0;//节点登出的通知
};

//现场流观察者
class CLiveStreamObserver
{
public:
    CLiveStreamObserver(void){};
    virtual ~CLiveStreamObserver(void){};

public:
    virtual void LiveStreamCapture(const char *pBuf, size_t iLen)=0;
};


//网络模块的初始化和销毁
_EXPORT_NETCOMM_SDK bool NET_COMM_Initial(tuint32 localNodeType, CNetBufferCtrlBase *pNetBufCtrl, CStateDispatchCtrlBase *pStateCtrl);
_EXPORT_NETCOMM_SDK void NET_COMM_Quit();
_EXPORT_NETCOMM_SDK bool NET_COMM_Start();
_EXPORT_NETCOMM_SDK void NET_COMM_Stop();
_EXPORT_NETCOMM_SDK tuint32 NET_COMM_GetLastError(); //返回NET_SOCKET_ERROR_CODE里面的值

_EXPORT_NETCOMM_SDK bool NET_COMM_RegistCmdProcObject(tuint32 remoteType, bool localNodeIsClient, CCmdProcObject* pCmdProcObject);

//此函数必须和NET_COMM_Initial在同一个时机调用，不然会出错
_EXPORT_NETCOMM_SDK bool NET_COMM_RegistDevProtocolProcObject(tuint32 devType, CNetProtocolProc* pDevProtocolProc);
_EXPORT_NETCOMM_SDK bool NET_COMM_AddNetNode(const NET_NODE_INFO& info, GUID chID[], int chIDSize, GUID chAlarmIn[], int chAlarmInSize, GUID chAlarmIOut[],
												int chAlarmOutSize, CNetNode** pOutNetNode  = NULL, bool bEncrypt = false, int loginNonce = 0);
_EXPORT_NETCOMM_SDK bool NET_COMM_DelNetNode(CNetNode* pNetNode);

//注册路由处理方法，只有注册服务器才需要注册
_EXPORT_NETCOMM_SDK bool NET_COMM_RegistRouteCheck(CRouteCheckBase *pRouteCheck);

//////////////////////////////////////////////////////////////////////////
//登出状态Logout
_EXPORT_NETCOMM_SDK void NET_COMM_AddNetNodeStateObserver(CNetNodeStateObserver *pObServer);
_EXPORT_NETCOMM_SDK void NET_COMM_DelNetNodeStateObserver(CNetNodeStateObserver *pObServer);  

_EXPORT_NETCOMM_SDK void NET_COMM_AddLiveStreamObserver(CLiveStreamObserver *pObServer);
_EXPORT_NETCOMM_SDK void NET_COMM_DelLiveStreamObserver(CLiveStreamObserver *pObServer);

//得到目前应用程序登陆成功后节点ID
_EXPORT_NETCOMM_SDK GUID NET_COMM_GetCurrentAppNodeID();
_EXPORT_NETCOMM_SDK tuint32 NET_COMM_GetCurrentAppNodeType();
_EXPORT_NETCOMM_SDK bool  NET_COMM_SetCurrentAppNodeID(GUID localNodeID);//

_EXPORT_NETCOMM_SDK void NET_COMM_AddConnectStateObserver(CConnectStateObserver *pObServer);
_EXPORT_NETCOMM_SDK void NET_COMM_DelConnectStateObserver(CConnectStateObserver *pObServer);
_EXPORT_NETCOMM_SDK bool NET_COMM_GetConnectState(GUID nodeID);

//此函数只有从设备收到通道上下线状态时才会被调用
_EXPORT_NETCOMM_SDK void NET_COMM_SetConnectState(GUID mduID, tuint32 nodeType, GUID nodeID, bool bConnect);

_EXPORT_NETCOMM_SDK void NET_COMM_AddAlarmStateObserver(CAlarmStateObserver *pObServer);
_EXPORT_NETCOMM_SDK void NET_COMM_DelAlarmStateObserver(CAlarmStateObserver *pObServer);
_EXPORT_NETCOMM_SDK BYTE NET_COMM_GetAlarmState(GUID nodeID, tuint32 alarmType);

//此函数只有从设备收到报警状态时才会被调用
_EXPORT_NETCOMM_SDK void NET_COMM_SetAlarmState(GUID mduID, tuint32 nodeType, GUID nodeID, tuint32 alarmType, BYTE alarmState, ECMS_ALARM_INFO AlarmInfo);

_EXPORT_NETCOMM_SDK void NET_COMM_AddRecordStateObserver(CRecordStateObserver *pObServer);
_EXPORT_NETCOMM_SDK void NET_COMM_DelRecordStateObserver(CRecordStateObserver *pObServer);
_EXPORT_NETCOMM_SDK void NET_COMM_SetRecordState(GUID chID, tuint32 recState);//由于录像状态时由存储服务器自己的模块产生的, 所以有此接口
_EXPORT_NETCOMM_SDK tuint32 NET_COMM_GetRecordState(GUID storageID, GUID chID);

_EXPORT_NETCOMM_SDK void NET_COMM_AddDevRecordStateObserver(CDevRecordStateObserver *pObServer);
_EXPORT_NETCOMM_SDK void NET_COMM_DelDevRecordStateObserver(CDevRecordStateObserver *pObServer);
_EXPORT_NETCOMM_SDK tuint32 NET_COMM_GetDevRecordState(GUID chID);

//此函数只有从设备收到设备通道录像状态时才会被调用
_EXPORT_NETCOMM_SDK void NET_COMM_SetDevRecordState(GUID mduID, tuint32 nodeType, GUID chID, tuint32 recordState);

//此函数只有从设备收到设备通道现场数据时才会被调用
_EXPORT_NETCOMM_SDK void NET_COMM_AddLiveData(const char *pData, tuint32 datalen, GUID &devNetNodeGUID);

_EXPORT_NETCOMM_SDK bool NET_COMM_SetTaskItemData(const GUID &chGUID, tuint32 itemData);
_EXPORT_NETCOMM_SDK tuint32 NET_COMM_GetTaskItemData(const GUID &chGUID);

//门禁状态主动推送
_EXPORT_NETCOMM_SDK void NET_COMM_AddSystemDoorStateObserver(CSystemDoorStateObserver *pObServer);
_EXPORT_NETCOMM_SDK void NET_COMM_DelSystemDoorStateObserver(CSystemDoorStateObserver *pObServer);
_EXPORT_NETCOMM_SDK void NET_COMM_SetSystemDoorState(const void* pData,tuint32 datalen);

#endif








