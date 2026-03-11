
///////////////////////////NetSocketSDK.h

#ifndef _NET_SOCKET_SDK_H
#define _NET_SOCKET_SDK_H

#ifdef WIN32 

#ifdef NET_SOCKET_SDK_EXPORT_API//动态库编译 
#define _EXPORT_NETSOCKET_SDK  __declspec(dllexport)
#else 
#ifdef NET_SOCKET_SDK_EXPORT_STATIC//静态库编译和使用
#define _EXPORT_NETSOCKET_SDK
#else
#define _EXPORT_NETSOCKET_SDK  __declspec(dllimport)
#endif
#endif

#else
#define _EXPORT_NETSOCKET_SDK __attribute__ ((visibility("default")))
#endif

#include "base_type_define.h"
#include "CustomContainer.h"

enum NET_SOCKET_ERROR_CODE
{
	NET_SOCKET_ERROR_SUCCESS=0x0,
	NET_SOCKET_ERROR_BASE=0x20000000, ////用户错误都需要这个位为1
	NET_SOCKET_ERROR_WSASTARTUP,//WSAStartup函数调用错误
	NET_SOCKET_ERROR_CREATE_EVENT,//WSACreateEvent错误
	NET_SOCKET_ERROR_CREATE_THREAD,//WSACreateEvent错误
	NET_SOCKET_ERROR_DUPLICATE_LISTEN,//重复Listen
	NET_SOCKET_ERROR_OVERLOW,//资源有限
	NET_SOCKET_ERROR_LISTEN_ERROR,//Listen错误
	NET_SOCKET_ERROR_LISTEN_EVENTSELECT,//WSAEventSelect错误
	NET_SOCKET_ERROR_INVLID_NODE,//无效的IOCP节点
	NET_SOCKET_ERROR_NOBUFS,  ///没有足够的BUF
	NET_SOCKET_ERROR_NET_DISCONNECT,//节点已断开
	NET_SOCKET_ERROR_NET_STOPED,//节点已停止
};

const size_t IOCP_PACKAGE_SIZE = 64*1024; //包最大长度
const int P2P_CONNECT_TIMEOUT = 3 * 60 * 1000; //p2p异步连接超时时间 3分钟 //?
const long P2P_MULTINET_SEND_BUF_SIZE = 1024 * 1024; //p2p，m_multiNetComm的发送缓冲区 //?

class CListenSocketObserver
{
public:
	virtual ~CListenSocketObserver(){};
	virtual void OnAcceptCreateSocket(tint32 hNetCommunication,tuint16 listenPort,tuint32 userData) = 0;
};

class CSocketDataObserver
{
public:
	virtual ~CSocketDataObserver(){};
    //pNeedLen为NULL时表示为IOCP模式，必须接收地方自己立即把数据保存起来
    virtual tuint32 OnSocketRecvData(tint32 hNetCommunication, const char* pBuffer, tuint32 len, tuint32 *pNeedLen,LPVOID lpParameter) = 0;
};

//网络模块的初始化和销毁
_EXPORT_NETSOCKET_SDK bool NET_SOCKET_Initial(tint32 nSendThreadCount, tint32 nRecvThreadCount, const char* p2pServerAddr=NULL, tuint32 iPort=8989);
_EXPORT_NETSOCKET_SDK void NET_SOCKET_Quit();
_EXPORT_NETSOCKET_SDK tuint32 NET_SOCKET_GetLastError(); //返回NET_SOCKET_ERROR_CODE里面的值

//Listen相关
_EXPORT_NETSOCKET_SDK void NET_SOCKET_SetP2PServerAddr(const char* p2pServerAddr, tuint32 iPort, bool isMainAddr=true, const char* p2pServerAddr2=NULL, tuint32 iPort2=7968);
_EXPORT_NETSOCKET_SDK bool NET_SOCKET_StartListen(tuint16 listenPort, CListenSocketObserver* pAcceptObserver,tuint32 UserData,bool startAcceptConnect);
_EXPORT_NETSOCKET_SDK bool NET_SOCKET_StartListenByP2P(tuint16 listenPort, CListenSocketObserver *pAcceptObserver, tuint32 userData, bool startAcceptConnect, const char *pStrSN, tuint32 devType, const char *devVer);

//为了避免死锁,把WORD listenPort去掉了,否则在有两个Listen时,如果先开始一个接受连接
//第一个Listen的连接马上就进来了向外通知,而第二个刚刚调用NET_SOCKET_StartAcceptConnect,向外和向内就出现了死锁的可能
_EXPORT_NETSOCKET_SDK bool NET_SOCKET_StartAcceptConnect();
_EXPORT_NETSOCKET_SDK void NET_SOCKET_StopListen(tuint16 listenPort);

//Connect相关
_EXPORT_NETSOCKET_SDK tint32 NET_SOCKET_AddConnect(tuint32 localIP,const char* pStrServerIP,tuint16 netPort);
_EXPORT_NETSOCKET_SDK tint32 NET_SOCKET_AddConnectByP2P(tuint32 localIP,const char* pStrSn,tuint16 netPort);
_EXPORT_NETSOCKET_SDK tint32 NET_SOCKET_AddConnectByP2P2(tuint32 localIP,const char* pStrSn,tuint16 netPort);
_EXPORT_NETSOCKET_SDK void NET_SOCKET_DelConnect(tint32 hNetCommunication);

/////返回true的时候链接结果有效，  result大于0表示链接成功， 小于0表示链接失败，等于0表示正在链接中
//注意：建立连接的时候采用NET_SOCKET_PopConnectResult判断是否建立连接，连接上后，不能用这个函数决定连接是否有效，要用NET_SOCKET_CheckConnectState
_EXPORT_NETSOCKET_SDK bool NET_SOCKET_PopConnectResult(tint32 hNetCommunication,tint32 &result, tuint32 maxWaitMSec = 0);

//Send Recv相关
_EXPORT_NETSOCKET_SDK bool NET_SOCKET_RegisterNode(tint32 hNetCommunication, CSocketDataObserver* pSocketDataObserver,LPVOID lpParameterRecv,
													 tint32 RecvBufCount,tint32 SendBufCount);
_EXPORT_NETSOCKET_SDK bool NET_SOCKET_Start(tint32 hNetCommunication);
_EXPORT_NETSOCKET_SDK bool NET_SOCKET_CheckConnectState(tint32 hNetCommunication);
_EXPORT_NETSOCKET_SDK int  NET_SOCKET_Send(tint32 hNetCommunication, const char *pHeadInfo,size_t headlen, const char* pBuffer=NULL, size_t len=0,CChildPairContainer<BYTE *, tint32> *pContent=NULL, tuint32 dwTimeout = INFINITE);//成功返回0， 失败返回未发送的长度
_EXPORT_NETSOCKET_SDK void NET_SOCKET_Stop(tint32 hNetCommunication);
_EXPORT_NETSOCKET_SDK void NET_SOCKET_UnRegisterNode(tint32 hNetCommunication);
_EXPORT_NETSOCKET_SDK void NET_SOCKET_DestroyHNetCommunication(tint32 hNetCommunication);

_EXPORT_NETSOCKET_SDK int NET_SOCKET_Recv_Immediate(tint32 hNetCommunication, char *pBuf, tint32 nLen, bool bPeek, tuint32 maxWaitMSec = 500);//500是为了保持和原来的无参数时接收40次兼容
_EXPORT_NETSOCKET_SDK int NET_SOCKET_Recv_Immediate(tint32 hNetCommunication, char *pBuf, tint32 nLen, bool *pIsMakeError);		//缓冲区内数据接收完立即返回
_EXPORT_NETSOCKET_SDK int NET_SOCKET_Send_Immediate(tint32 hNetCommunication, const char *pBuf, tint32 nLen, tuint32 maxWaitMSec = 500);//500是为了保持和原来的无参数时接收40次兼容
_EXPORT_NETSOCKET_SDK int NET_SOCKET_Send_Immediate(tint32 hNetCommunication, const char *pBuf, tint32 nLen, bool& bErr);

_EXPORT_NETSOCKET_SDK bool NET_SOCKET_GetLocalIP(tint32 hNetCommunication, sockaddr_in &addrInfo);

_EXPORT_NETSOCKET_SDK bool NET_SOCKET_GetRemoteIP(tint32 hNetCommunication, sockaddr_in &addrInfo);



//UDP系列函数

/*
  *serverPort:Call bind if it's greater than 0, otherwise not to bind which mean it's a client or the user want to bind it later.
  */
_EXPORT_NETSOCKET_SDK tint32 NET_SOCKET_CreateUDPNetCommunication( const char *pIfName = "eth0", tuint32 serverPort = 0, bool bNonBlock = true);
_EXPORT_NETSOCKET_SDK bool NET_SOCKET_UDPSend(tint32 hNetCommunication,tuint32 serverIP,tuint16 serverport, const void *pSendBuf, tuint32 sendLen,tuint32 sendTimes=1);
_EXPORT_NETSOCKET_SDK tuint32 NET_SOCKET_UDPRecv(tint32 hNetCommunication,tuint32 &serverIP,tuint16 &serverport,void *RecvBuf, tuint32 RecBufLen, tuint32 timeout);
_EXPORT_NETSOCKET_SDK tuint32 NET_SOCKET_UDPRecvSelect(tint32 *phNetCommunication,tint32 count, tuint32 timeout,bool *pRecvFlag);
_EXPORT_NETSOCKET_SDK void NET_SOCKET_DestroyUDPNetCommunication(tint32 hNetCommunication);	   

_EXPORT_NETSOCKET_SDK bool NET_SOCKET_Inet_ntoa(tuint32 dwIP, char *buf);

/*
  *Describe:
  *It's a complete communication process which is used in the simple client communcation, e.g. http in dns module.
  *It include serverl steps as follows:
  *1.create the socket and connect to the server.
  *2.query the result with a max wait time set by maxWaitMSec
  *3.send the data pointed by pSendBuffer.
  *4.recvive the data to buffer pointed by pRecvBuffer.
  *Return value:
  *0:fail, no data sended. >0:the number of sended data. <0:not define.
  */
_EXPORT_NETSOCKET_SDK tint32 NET_SOCKET_SimpleClientComm(const char* pServerAddr, tuint16 serverPort, const char* pSendBuffer, tuint32 sendLen, char* pRecvBuffer, tuint32 recvLen,const char *pEndStr, tuint32 maxWaitMSec = 5000);
_EXPORT_NETSOCKET_SDK tint32 NET_SOCKET_SimpleClientComm(const char* pServerAddr, tuint16 serverPort, const char* pSendBuffer, tuint32 sendLen, char* pRecvBuffer, tuint32 recvLen,const char **ppEndStr, tuint32 nStrCnt, tuint32 maxWaitMSec = 5000);

_EXPORT_NETSOCKET_SDK bool NET_SOCKET_StartHttpServerListen(tuint16 listenPort, CSocketDataObserver* pAcceptObserver, tuint32 userData);
_EXPORT_NETSOCKET_SDK void NET_SOCKET_StopHttpServerListen(tuint16 listenPort);
_EXPORT_NETSOCKET_SDK void NET_SOCKET_SETP2PACCEPT(tint32 hNetCommunication,tuint16 listenPort,tuint32 userData);

_EXPORT_NETSOCKET_SDK tint32 NET_SOCKETSetSSL_fd(void *ssl, tint32 hNetCommunication);

#endif








