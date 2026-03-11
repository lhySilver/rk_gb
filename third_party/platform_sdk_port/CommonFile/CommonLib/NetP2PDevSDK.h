
///////////////////////////NetP2PDevSDK.h

#ifndef _NET_P2PDEV_SDK_H
#define _NET_P2PDEV_SDK_H

#ifdef WIN32 

#ifdef NET_P2PDEV_SDK_EXPORT_API//动态库编译
#define _EXPORT_NETP2PDEV_SDK  __declspec(dllexport)
#else 
#ifdef NET_P2PDEV_SDK_EXPORT_STATIC//静态库编译和使用
#define _EXPORT_NETP2PDEV_SDK
#else
#define _EXPORT_NETP2PDEV_SDK  __declspec(dllimport)
#endif
#endif

#else
#define _EXPORT_NETP2PDEV_SDK __attribute__ ((visibility("default")))
#endif

#include "base_type_define.h"

enum NET_P2PDEV_ERROR_CODE
{
    NET_P2PDEV_ERROR_SUCCESS=0x0,
    NET_P2PDEV_ERROR_BASE=0x20000000, //用户错误都需要这个位为1
    NET_P2PDEV_ERROR_DUPLICATE_LISTEN,//重复Listen
    NET_P2PDEV_ERROR_OVERLOW,//资源有限
    NET_P2PDEV_ERROR_LISTEN_ERROR,//Listen错误
    NET_P2PDEV_ERROR_NOBUFS,  //没有足够的BUF
    NET_P2PDEV_ERROR_NET_DISCONNECT,//节点已断开
    NET_P2PDEV_ERROR_NET_STOPED,//节点已停止
};

typedef enum _tvt_product_type_p2p_
{
	TVT_PRODUCT_TYPE_NULL = 0,

	TVT_PRODUCT_TYPE_IPC = 1,			//IPC 
	TVT_PRODUCT_TYPE_NVR = 2,			//NVR
	TVT_PRODUCT_TYPE_DVR = 3,			//DVR	
	TVT_PRODUCT_TYPE_YITU_SUIT = 4,		//依图套装
	TVT_PRODUCT_TYPE_TVT_SUIT = 5,		//TVT套装
	TVT_PRODUCT_TYPE_YT_NVR = 7,		//依图NVR
	TVT_PRODUCT_TYPE_YT_NVMS = 8,		//NVMS
	TVT_PRODUCT_TYPE_MAX = 0xFFFF,	
};

class CP2PDevListenObserver
{
public:
    virtual ~CP2PDevListenObserver(){};
    virtual void OnP2PAcceptSocket(tint32 hNetCommunication,tuint16 listenPort,tuint32 userData) = 0;
};

class CNetP2PDevObserver
{
public:
    virtual ~CNetP2PDevObserver(){};
    virtual tuint32 OnNetP2PRecvData(tint32 hNetCommunication, const char* pBuffer, tuint32 len, LPVOID lpParameter) = 0;
};

//网络模块的初始化和销毁
_EXPORT_NETP2PDEV_SDK bool NET_P2PDEV_Initial();
_EXPORT_NETP2PDEV_SDK void NET_P2PDEV_Quit();

_EXPORT_NETP2PDEV_SDK bool NET_P2PDEV_Start(const char *serverIp, const tuint16 usPort, char *devSN, CP2PDevListenObserver *pAcceptObserver, tuint32 userData);
_EXPORT_NETP2PDEV_SDK int  NET_P2PDEV_Send(tint32 hNetCommunication, void* pData, int iLen, bool bBlock, int sendTimeout);//成功返回0， 失败返回未发送的长度
_EXPORT_NETP2PDEV_SDK void NET_P2PDEV_Stop(tint32 hNetCommunication);

//_EXPORT_NETP2PDEV_SDK tuint32 NET_P2PDEV_GetLastError(); //返回NET_P2PDEV_ERROR_CODE里面的值
//_EXPORT_NETP2PDEV_SDK bool NET_P2PDEV_CheckConnectState(tint32 hNetCommunication);

_EXPORT_NETP2PDEV_SDK bool NET_P2PDEV_RegisterNode( CNetP2PDevObserver* pNetP2PRecvDataObserver, LPVOID lpParameterRecv);

_EXPORT_NETP2PDEV_SDK void NET_P2PDEV_DisconnectAllSocketHandle();

_EXPORT_NETP2PDEV_SDK int NET_P2PDEV_Send_Immediate(tint32 hNetCommunication, const char *pBuf, tint32 nLen, tuint32 maxWaitMSec = 500);

_EXPORT_NETP2PDEV_SDK bool NET_ISP2PDEV_CONNECT(tint32 hNetCommunication);

_EXPORT_NETP2PDEV_SDK int  NET_P2PDEV_Send(tint32 hNetCommunication,void* pHeadInfo,int nHeadLen, void* pData, int iLen, bool bBlock, int sendTimeout);

_EXPORT_NETP2PDEV_SDK int  NET_P2PDEV_Recv_Immediate(tint32 hNetCommunication, void* pData, tint32 iLen,tuint32 waitTime = 0);
_EXPORT_NETP2PDEV_SDK bool NET_P2PDEV_SetPrikey(const char *pPrivateKeyBuf, tuint32 privateKeyLen);
_EXPORT_NETP2PDEV_SDK bool NET_P2PDEV_GETNATCONNECTSTATU();
#endif
