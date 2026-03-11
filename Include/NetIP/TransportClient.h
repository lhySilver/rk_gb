//仿照server 端实现client  端代码

//同步server 端的优化操作

#ifndef __TRANSPORT_CLIENT__
#define __TRANSPORT_CLIENT__

#include "Infra/Packet.h"

#include "TransportDef.h"

#ifndef	USE_OLD_PACKET
#define USE_OLD_PACKET
#endif

class ITransportClient
{
public:
	//设置VVNat  服务器端的参数，所有客户端共用一个服务接口!!!
	static int SetVVNatParam(char *vvnatSvrIp,int vvnatSvrPort);

	//获得nat 穿越的服务器连接状态
	static int GetNatRegisterStatus();
	
public:
	static ITransportClient* create(int objectID=0,transport_type type=TRANSPORT_TCP,TransportCallbackProc sendProc=NULL,TransportCallbackProc recvProc=NULL,TransportCallbackProc connProc=NULL,TransportCallbackProc closeProc=NULL);

	virtual ~ITransportClient(){};
public:
	//直接绑定远程客户端，如果是UDP  则也是虚拟绑定
	//如果是VVNat  协议仅用remoteIp  指出要连接的目标端uuid；
	virtual int Connect(char *remoteIp, int remotePort,char *localIp=NULL,int localPort=0) = 0;
	
	virtual int Send(char *buffer, int size,int tail=1) = 0;
#ifdef USE_OLD_PACKET
	virtual int Send(CPacket *packet,int tail=1) = 0;
#else
	virtual int Send(CBuffer &buffer,int tail=1) = 0;

	virtual int Send(CPacket &pakcet,int tail=1) = 0;
#endif
	virtual int SendDirect(char *buffer, int size,int bSplite) = 0;
	
	virtual int RecvDirect(char *buffer, int size,int timeout=0/*ms*/) = 0;

	virtual int GetStatisticInfo(transport_statistic *info) = 0;

	//清除发送队列		
	virtual int ClearQueue() = 0;	

	//设置拆分协议()
	virtual int SetSplitProtocal(split_protocal protocal) = 0;

	//设置拆分单元的大小(默认为1400)	
	virtual int SetSplitUnit(int unit) = 0;	

	//获得附加信息
	virtual int GetVVNatExInfo(char *uuid,char *pInfo, int length,char* ipaddr= NULL) = 0;
	
	//客户端主动断开连接。
	virtual int Disconnect() = 0;

	//增加设置内部缓冲区大小的限定
	virtual int SetQueueLimit(int queueSize,int queueLength) = 0;	

	//增加调试接口
	virtual int Dump() = 0;	
	
};

#endif//__TRANSPORT_CLIENT__

