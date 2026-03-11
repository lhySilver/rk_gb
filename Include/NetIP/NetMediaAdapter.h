//使用说明:
//在这个策略库中的接口中CPacket 中不存在多帧的情况
//如果存在多帧，则由adapter 的掉用者在外围拆成多个
//CPacket 再调入，但是保留Cpacket 中帧不完整的情况，因
//为目前IPC  就满足了这个条件可以减少一次内存拷贝!

//修改接口以适应NVR的使用需求


#ifndef __NET_MEDIADAPTER_H__
#define __NET_MEDIADAPTER_H__

#include "Infra/Packet.h"
#include "Infra/Function.h"

#ifndef USE_OLD_PACKET
#define USE_OLD_PACKET
#endif

class INetMediaAdapter
{ 
public:
	enum Policy
	{
		AUTO,			//自适应
		QUALITY_DVR,	//画质优先(发送端丢帧)
		FLUENCY_DVR,	//流畅优先(切到辅码流)		
		QUALITY_IPC,	//画质优先(不切换主辅码流，编码器丢帧)
		FLUENCY_IPC,	//流畅优先(不切换主辅码流)	
		POLICY_NR,		//
	};

	enum AdapterOP
	{
		CLEAR,			//清发送队列
		ENCODE,			//修改编码器
		SWITCH,			//切换码流
		OP_NR,
	};
	
	typedef struct EncodeCfgInfo
	{
		int videoEnable;	//视频使能标志
		int audioEnable;	//音频使能标志
		int bitrate;		//
		int fps;			//
		int gop;			//单位为s  
		int rev[4];		
	}EncodeCfgInfoSt;	
	
	typedef struct AdapterCmd
	{
		AdapterOP 	op;			//enum AdapterOP
		int 			stream;
		int 			bitrate;
		int 			fps;
		int 			gop;
	}AdapterCmdSt;

	typedef struct NetStatistic	//网络状态统计
	{
		int 	sendSpeed;		//发送的速度kbps(对方确认收到)	
		int 	blocklevel;		//网络的阻塞程度[0-6]；
		int 	delaySize;		//发送在发送队列中的缓冲的数量(byte)
	}NetStatisticSt;
	
	//int callback(int objectID,AdapterCmdSt *pCmd);
	typedef TFunction2<int,int,AdapterCmdSt *>AdapterConfigCB;

public:
	static INetMediaAdapter* create(int objectID=0, Policy policy=QUALITY_IPC, AdapterConfigCB adapterConfigProc=NULL);
	virtual ~INetMediaAdapter(){};

	//媒体数据接口;在这个环节实现丢帧控制!!!
#ifdef USE_OLD_PACKET
	virtual int PutMediaData(CPacket *packet) = 0;
	virtual CPacket *GetMediaData() = 0;
#else
	virtual int PutMediaData(CPacket &packet) = 0;
	virtual CPacket GetMediaData() = 0;
#endif
	//告知Adapter  当前用户开启的通道码流信息(只在初始化时调用一次)
	//当回放等非实时监视类窄带控制的时候,传入参数-1 ，以通知策略
	//库区分对待传输策略
	virtual int SetMediaInfo(int channel,int stream,int bRemoteCh) = 0;	

	//告知Adapter  当前的网络状态
	virtual int SetNetStatistic(NetStatisticSt *stsc) = 0;
	
	//编码器的配置情况作为Adapter 的调整上限	
	//每次用户手动调整编码器有更新的时候调用
	static int SetEncodeConfig(int channel,int stream,EncodeCfgInfoSt *encCfg);

	//这是个编码器的Adapter  之后的配置值(动态调整的)	
	static int SetEncodeAdapter(int channel,int stream,EncodeCfgInfoSt *encCfg);

	//全局调试信息输出
	static int TriggerDebug(int flag);	

	//全局传输策略设置
	static int SetGobalPolicy(Policy policy);
};

#endif //__MEDIADAPTER_H__

