//V0.01 
//USE_OLD_PACKET : 
//当使用老的Packet 时需要在Makefile 中定义改宏,发送完成后显性release()。

//V0.02 
//增加清除发送队列的接口
//修改拆包协议部分接口定

//V0.03 
//修改连接的优先级设置接口
//因为连接优先级数据，并不是全局排列的，而是成组的。
//如一个主链和对应该连接的附属连接之间才有优先级。
//而该附属连接和另外的主连接是没有优先级的。
//例如:   两个客户端登录并申请码流，这样共有2组连接
//(socketA+socketa)和(socketB+socketb)；只有socketA中无数据才会发送socketa
//但是socketA 不对socketb  构成影响。

//V0.04 
//取消send  函数中的优先级设置，因为优先级通常在两个不同
//的socket  之间，而同一个socket  同时是不区分两次send  的优先级的
//不然容易产生乱序的数据。所以在此取消相关接口。

//V0.05 
//增加主动断开客户端连接的接口

//V0.06 
//增加UDP 可靠传输分包协议

//V0.06 
//增加获取穿越的连接状态接口!

//V0.06 
//增加增加用来设置内部队列缓冲的接口

//V0.07 
//重新整理接口，1> 去掉多余的RTP  相关接口2>心跳中增加超时设定
//并提交Client  相关接口，以支持主动注册操作!

//V0.08 
//对锁的使用做优化以使Send   函数和heartbeat  之间的公共临界区更加的小

//V0.09 
//每一个Server独立一个线程；并去掉HeartBeat()接口。
//引入NotePool进行全局内存限制。

#ifndef __TRANSPORT_SERVER__
#define __TRANSPORT_SERVER__

#include 	"Infra/Packet.h"

#include	"TransportDef.h"

#ifndef	USE_OLD_PACKET
#define USE_OLD_PACKET
#endif

//服务器端对象
class ITransportServer
{
public:
	// 此处创建的对象需要使用者进行释放
	static ITransportServer* create(int objectID=0,transport_type type=TRANSPORT_TCP,TransportCallbackProc sendProc=NULL,TransportCallbackProc recvProc=NULL,TransportCallbackProc connProc=NULL,TransportCallbackProc closeProc=NULL);

	virtual ~ITransportServer(){};
public:
	//设定客户端连接的超时时间
	virtual int SetTimeOut(int timeout) = 0;	//单位为s

	//设置连接的优先级,  高优先级的连接发送完成后，才发低优先级的连接
	virtual int SetPriority(int clientID,int groupID,priority_level priority=PRIORITY_HIGH) = 0;

	//指定本地的监听Ip  和port	
	virtual int Listen(char *ip, int port) = 0;	

	//异步数据发送；
	//内存直接拷贝  ，不需要调用者指针的有效性	
	//由clientID  指明数据要发送给哪个客户端			
	//tail : 应对媒体数据发送时MsgHdr和数据内容分开的情况(RTP拆包)
	virtual int Send(char *buffer, int size, int clientID,int tail=1) = 0;

#ifdef USE_OLD_PACKET
	virtual int Send(CPacket *packet, int clientID,int tail=1) = 0;
#else
	virtual int Send(CBuffer &buffer, int clientID,int tail=1) = 0;

	virtual int Send(CPacket &pakcet, int clientID,int tail=1) = 0;
#endif

	//同步数据发送；
	virtual int SendDirect(char *buffer, int size, int clientID,int bSplite=0) = 0;

	virtual int RecvDirect(char *buffer, int size,int clientID,int timeout=0/*ms*/) = 0;

	//核心驱动源	
	virtual int Heartbeat() = 0;	

	virtual int GetStatisticInfo(int clientID,transport_statistic *info) = 0;

	virtual int GetStatisticRtpInfo(int clientID,transport_statistic_rtp*info) = 0;

	//清除发送队列(提供窄带控制使用)
	virtual int ClearQueue(int clientID) = 0;	

	//下面三个函数仅在传输类型为UDP|VVNET  时需要调用
	//设置拆分协议
	virtual int SetSplitProtocal(split_protocal protocal) = 0;

	//设置拆分单元的大小(默认为1400)	
	virtual int SetSplitUnit(int unit) = 0;	

	//使用RTP  拆包协议的时候有效(ssrc = ch<<8 + stream)
	virtual int SetSSRC(int clientID,int ssrc) = 0;

	//下面这个函数仅在传输类型为VVNET  时需要调用
	//设置注册到VVNat  服务器时需要的参数
	virtual int SetVVNatParam(char *vvnatSvrIp, int vvnatSvrPort,char *uuid) = 0;

	//设置附加信息
	virtual int SetVVNatExInfo(char *pInfo, int length) = 0;

	//主动断开客户端的连接。。。	
	virtual int KickoutClient(int clientID) = 0;		

	//增加调试接口;  当clientID=-1 的时候全显示
	virtual int Dump(int clientID) = 0;	
};

#endif	//__TRANSPORT_SERVER__

