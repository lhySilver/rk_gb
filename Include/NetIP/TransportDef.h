#ifndef __TRANSPORT_DEF_H__
#define __TRANSPORT_DEF_H__

#include "Infra/Function.h"

#define MAX_PRIORITY_GROUP		(128)

typedef enum priority_level
{
	PRIORITY_NORMAL = 0,
	PRIORITY_HIGH,
	PRIORITY_LOW,
	PRIORITY_NUM,
}priority_level;

typedef enum transport_type
{
	TRANSPORT_TCP = 0,
	TRANSPORT_UDP,	
	TRANSPORT_VVNAT,	//连接VVeye穿越库!
}transport_type;		

typedef enum split_protocal
{
	TRANSPORT_AUTO = 0,	//由协议栈去拆分
	TRANSPORT_RTP,			//采用RTP  协议
	TRANSPORT_UDPSAFE,	//采用UDP 可靠传输分包协议
}split_protocal;

typedef struct transport_statistic
{
	unsigned int interval;			//统计间隔xxxx ms
	unsigned int send_total;		//发送的数据量(历史数据)
	unsigned int recv_total;			//收到的数据量(历史数据)
	unsigned int send_size;			//发送的数据量(统计间隔内)
	unsigned int recv_size;			//收到的数据量(统计间隔内)
	unsigned int put_size;			//上层调用send_函数尝试要发送的数据量(统计间隔内)
	unsigned int queuesize;			//发送队列数据长度!!!
	unsigned int queuenum;		//发送队列节点数!!!
}transport_statistic;

typedef struct transport_statistic_rtp
{
	int	calcInterval;		//统计间隔(ms)
	int	recvSize;			//收到的数据量(byte)	
	int	shouldPacket;	//理论应该收到的数据报数据
	int	realPacket;		//实际收到的数据包的数据
	int	validPacket;		//收到数据包中有效部分的个数
	int	ssrc;			//媒体源编号= 通道号* 码流总数 + 码流类型
}transport_statistic_rtp;

//int IOcallback(int objID,int clientID,const char* data,int len);	//数据收发
//int DCcallback(int objID,int clientID,char* ip,int port);			//断开关闭
typedef TFunction4<int, int, int,char *,int> TransportCallbackProc;	

#endif	//__TRANSPORT_DEF_H__

