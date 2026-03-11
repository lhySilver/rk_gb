#ifndef __BASICCMDPROCOBJECT_H__
#define __BASICCMDPROCOBJECT_H__

#include "NetCommSDK.h"
#include "CmdProcObject.h"

//基本命令处理对象

//登录命令处理对象基类

class _EXPORT_NETCOMM_CLASS CLoginCmdProcBase: public CCmdProcObject
{
public:
	   CLoginCmdProcBase(NET_PROTOCOL_CMD_DEF request) : CCmdProcObject(request, TRUE)
	   {

	   }

	   virtual ~CLoginCmdProcBase()
	   {

	   }
	   virtual CCmdProcObject*	  CreateSelf(){return new CLoginCmdProcBase(GetRequestCommand());};
	   virtual ECMS_NET_CTRL_FAIL ExecuteCommand(const CCmdProcParam* pParam);
	   virtual void	OnRequestCommand(const void *lpCmdData,size_t cmddatalen);
	   virtual void OnSendRequestCommand(const void *lpData,tuint32 datalen);
	   virtual void OnReplyCommandSuccess(const void *lpData,tuint32 datalen,bool hasreached);
	   virtual void OnReplyCommandFail(const void *lpData,tuint32 datalen,bool hasreached);
	   virtual void OnSendCommandSuccess(const void *lpData,tuint32 datalen);
	   virtual void OnSendCommandFail(const void *lpData,tuint32 datalen);
protected:
	   //更新节点信息
	   virtual bool OnLoginCheck(const ECMS_NET_LOGIN_INFO* pLoginInfo,GUID &nodeID,tuint32 &dwErr,ECMS_LOGIN_SUCCESS_INFO *pSuccessInfo);
	   virtual void ReplyLoginSuccess(const ECMS_NET_LOGIN_INFO* pLoginInfo,ECMS_LOGIN_SUCCESS_INFO *pSuccessInfo);
	   virtual void ReplyLoginFail(const ECMS_NET_LOGIN_INFO* pLoginInfo,tuint32 dwErr);
};

///////////////////////////////////////////////////////////////////
//状态信息命令处理
///////////////////////////////////////////////////////////////////

//处理对象
class _EXPORT_NETCOMM_CLASS CCmdProcStateConnect: public CCmdProcObject
{
public:
	explicit CCmdProcStateConnect(NET_PROTOCOL_CMD_DEF request) : CCmdProcObject(request,FALSE)
	{

	}

	virtual ~CCmdProcStateConnect()
	{

	}

	virtual ECMS_NET_CTRL_FAIL ExecuteCommand(const CCmdProcParam* pParam);
	virtual void OnRequestCommand(const void *lpCmdData,size_t cmddatalen);
	virtual CCmdProcObject*	   CreateSelf(){return new CCmdProcStateConnect(GetRequestCommand());};	
};



//报警状态
//报警状态处理对象
class _EXPORT_NETCOMM_CLASS CCmdProcStateAlarm: public CCmdProcObject
{
public:
	   explicit CCmdProcStateAlarm(NET_PROTOCOL_CMD_DEF request);
	   virtual ~CCmdProcStateAlarm() {}
	   virtual ECMS_NET_CTRL_FAIL ExecuteCommand(const CCmdProcParam* pParam);
	   virtual void OnRequestCommand(const void* pInBuffer, size_t len);
	   virtual CCmdProcObject*	   CreateSelf(){return new CCmdProcStateAlarm(GetRequestCommand());};
};


//录像状态
//录像状态处理对象
class _EXPORT_NETCOMM_CLASS CCmdProcStateRecord: public CCmdProcObject
{
public:
	   explicit CCmdProcStateRecord(NET_PROTOCOL_CMD_DEF request);
	   virtual ~CCmdProcStateRecord() {}
	   virtual ECMS_NET_CTRL_FAIL ExecuteCommand(const CCmdProcParam* pParam);
	   virtual void OnRequestCommand(const void* pInBuffer, size_t len);
	   virtual CCmdProcObject*	   CreateSelf(){return new CCmdProcStateRecord(GetRequestCommand());};
};

//设备录像状态
//设备录像状态处理对象
class _EXPORT_NETCOMM_CLASS CCmdProcStateDevRecord: public CCmdProcObject
{
public:
	   explicit CCmdProcStateDevRecord(NET_PROTOCOL_CMD_DEF request);
	   virtual ~CCmdProcStateDevRecord() {}
	   virtual ECMS_NET_CTRL_FAIL ExecuteCommand(const CCmdProcParam* pParam);
	   virtual void OnRequestCommand(const void* pInBuffer, size_t len);
	   virtual CCmdProcObject*	   CreateSelf(){return new CCmdProcStateDevRecord(GetRequestCommand());};
};

///////声明处理路由信息的命令处理对象
class _EXPORT_NETCOMM_CLASS CCmdProcRouteInfo: public CCmdProcObject
{
public:
	explicit CCmdProcRouteInfo(NET_PROTOCOL_CMD_DEF request);
	virtual ~CCmdProcRouteInfo() {}
	virtual ECMS_NET_CTRL_FAIL ExecuteCommand(const CCmdProcParam* pParam);
	virtual void OnRequestCommand(const void* pInBuffer, size_t len);
	virtual void OnReplyCommandSuccess(const void *lpData,tuint32 datalen,bool hasreached);
	virtual void OnReplyCommandFail(const void *lpData,tuint32 datalen,bool hasreached);
	virtual CCmdProcObject*	   CreateSelf(){return new CCmdProcRouteInfo(GetRequestCommand());};
private:
	ROUTE_CHECK_INFO m_RouteInfo;
};
///////声明请求刷新路由信息的命令处理对象
class _EXPORT_NETCOMM_CLASS CCmdRefreshRouteInfo: public CCmdProcObject
{
public:
	   explicit CCmdRefreshRouteInfo(NET_PROTOCOL_CMD_DEF request);
	   virtual ~CCmdRefreshRouteInfo() {}
	   virtual ECMS_NET_CTRL_FAIL ExecuteCommand(const CCmdProcParam* pParam);
	   virtual void OnRequestCommand(const void* pInBuffer, size_t len);
	   virtual CCmdProcObject*	   CreateSelf(){return new CCmdRefreshRouteInfo(GetRequestCommand());};
};

///////声明进入配置的命令处理对象
class _EXPORT_NETCOMM_CLASS CCmdProcEnterConfig: public CCmdProcObject
{
public:
	   explicit CCmdProcEnterConfig(NET_PROTOCOL_CMD_DEF request);
	   virtual ~CCmdProcEnterConfig() {}
	   virtual ECMS_NET_CTRL_FAIL ExecuteCommand(const CCmdProcParam* pParam);
	   virtual void OnRequestCommand(const void* pInBuffer, size_t len);
	   virtual void OnReplyCommandSuccess(const void *lpData,tuint32 datalen,bool hasreached);
	   virtual void OnReplyCommandFail(const void *lpData,tuint32 datalen,bool hasreached);
	   virtual CCmdProcObject*	   CreateSelf(){return new CCmdProcEnterConfig(GetRequestCommand());};
};

///////声明离开配置的命令处理对象
class _EXPORT_NETCOMM_CLASS CCmdProcExitConfig: public CCmdProcObject
{
public:
	explicit CCmdProcExitConfig(NET_PROTOCOL_CMD_DEF request);
	virtual ~CCmdProcExitConfig() {}
	virtual ECMS_NET_CTRL_FAIL ExecuteCommand(const CCmdProcParam* pParam);
	virtual void OnRequestCommand(const void* pInBuffer, size_t len);
	virtual void OnReplyCommandSuccess(const void *lpData,tuint32 datalen,bool hasreached);
	virtual void OnReplyCommandFail(const void *lpData,tuint32 datalen,bool hasreached);
	virtual CCmdProcObject*	   CreateSelf(){return new CCmdProcExitConfig(GetRequestCommand());};
};

#endif 
