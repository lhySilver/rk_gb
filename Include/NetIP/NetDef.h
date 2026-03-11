#ifndef _NETIP_H
#define _NETIP_H

#include "Infra/Mutex.h"
#include "Types/Types.h"
#include "NetIP/MsgDef.h"

#ifdef WIN32
#include "TPBase/TPSocket/TPSocket.h"
#elif defined(LINUX)
#include <time.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#endif

#define NETIPASSERT(x) do{ if(!x) return;}while (0);
#define ASSERTRET(x) do{ if(!x) return -1;}while (0);

typedef struct netipmsg
{
	DVRIP_MSG_HEAD_T MsgHead;
//	uchar pData[0]; 	//这里的pData并不占用任何空间，只是为了方便获取消息头部后面紧跟的数据
	uchar *pData;
}DVRIP_MSG_T;

typedef struct msginfo
{
	DVRIP_MSG_HEAD_T DvrIpMsg;
	void *pData;
	uint nLen;
}MSG_INFO_T;

/*
* 为了减少内存拷贝的次数, 构建了该结构体
* MsgHead.DataLen = CPACKET的大小,而不是MEDIA_MSG_T结构体的大小
* pMediaData: 为CPACKET的地址
* iMediaPos: 已经发送的媒体数据大小
*
*/
typedef struct media_msg_t
{
	DVRIP_MSG_HEAD_T	MsgHead;
	void				*pObjct;
	char				*pMediaData;
	int					iMediaPos;
}MEDIA_MSG_T;

typedef struct _NET_MEDIA_T_
{
	void *pObjct;
	char *pMediaData;
	int	iMediaPos;
	int	iDataLen;
}NET_MEDIA_T;

typedef struct tagStreamHead
{
	char cFlag[4];  // 码流标志 I帧：00 00 01 FC P帧：00 00 01 FD A帧：00 00 01 FA
	char cType;		// 0: h264
	char cFrameNum; // PAL: 0~25 NTSC: 0~30
	char cWidth;	// 实际宽度x8
	char cHeight;	// 实际高度x8
	DateTime dateTime;	//时间戳
	int iDataLen;	// 数据长度，不包括头部
}STREAM_HEAD_INFO;

typedef struct tagNodeHead
{
	char *pBuf;
	int iBufLen;
	int iPos;
	int iFrames;  /*此包数据对应的帧数*/
	int iAudio;
	tagNodeHead *pQPre;	/*!< 用于标识节点在队列中的位置 */
	tagNodeHead *pQNext; /*!< 用于标识节点在队列中的位置 */
	tagNodeHead *pPPre;	/*!< 用于标识节点在节点池中的位置 */
	tagNodeHead *pPNext; /*!< 用于标识节点在节点池中的位置 */
}NODE_HEAD_T;

typedef struct tagNodePool
{
	void			*pPoolAddress;	/*!< 节点池的内存起始地址 */
	uint			iPoolSize;		/*!< 申请的内存大小 */
	NODE_HEAD_T		*pHead;			/*!< 节点头 */
	NODE_HEAD_T		*pCur;			/*!< 当前节点 */
	NODE_HEAD_T		*pTail;			/*!< 节点尾 */
	uint			nTotal;			/*!< 总节点个数 */
	uint			nUsed;			/*!< 已经使用的节点个数 */
	//CMutex			Mutex;			/*!< 同步锁 */
}NODE_POOL;

typedef struct tagNetQueue
{
	NODE_HEAD_T  	   *pHead;	/// 发送队列的头部
	NODE_HEAD_T  	   *pTail;		/// 发送队列的尾部
	
	uint	iTotalSize;				/// 队列中的数据总长度
	
	/// 以下参数都是用于网络数据发送策略调整
	uint		adapterTime;			/// 记录上一次进行策略调整的时间
	int		iCurBitRate;			/// 当前码率
	int		iCurGOP;				/// 当前GOP
	int		iCurFramePolicy;		/// 当前丢帧策略,见QUALITY_POLICY
	int		iCurSendPFrame;		/// 当前已经发送P帧个数, -1表示下个I帧之前的P帧都丢掉
	int		iNeedSendPFrame;	/// 需要发送P帧个数, -1表示下个I帧之前的数据都要发送
	int		iCurBitRatePolicy;		/// 当前的降码率策略
	int		iExtramResolution;	/// 辅码流的配置分辨率
	int		iExtramBitRate;		/// 辅码流的配置码率
	int		nExtramFPS;			/// 辅码流的配置帧率
	int		iChangeToMainCnt;	/// 累计连续需要切换回主码流的次数，超过5次，切换主码流, 0:表示当前已经是主码流了
	int		iCurAutoPolicy;		/// 当前的自动调节策略	
	int		iCurAutoLevel;		/// 当前的自动调节策略等级

	bool		bExtramStream;		/// 辅码流是否打开标志
	bool		bExtramAudioEnable;	/// 辅码流音频是否打开标志			
	bool    	bFullFrame;             	/// 当前是否发送了完整的一帧数据	

	uint		iPackageCnt;			///队列中package的数量主要处理安帧发送的情况
	uint		iKeyFrameIntv;		///等待下一个I帧
	uint		iFrameCnt;			///两个I帧之间的帧数，用来更新iKeyFrameIntv
	uint 		iTotalSendVideoSize; ///发送的视频数据
	uint		iTotalSendAudioSize; ///发送的音频数据
	uint		iTotalSendFrames;
//geyijun@20120110 

	uint		iConfigBitRate;		//用户配置的码率(作为调整的上限)	
	int			iConfigFPS;			//用户配置的帧率(作为调整的上限)
	int			iCurFPS;			//当前动态的帧率 小于0表示丢I帧的间隔
	uint		ilastPackageCnt;	
	uint		iSendIFramDiv;		//丢帧分频值0: 不进行丢帧  1:每I  帧都发 2: 两I  帧只发一个
	uint		ilastSendIFramDiv;
	uint		iCurSendIFram;
	int			iOldFPS[3];
	uint		iOldFPSIndex;
	int 		iPolicyStart;
	uint		ID;
} NET_QUEUE;

typedef enum quality_policy
{
	PFRAME_NONE = 0,	/// 不丢帧
	PFRAME_1_20,
	PFRAME_2_20,
	PFRAME_3_20,
	PFRAME_4_20,
	PFRAME_5_20,
	PFRAME_6_20,
	PFRAME_7_20,
	PFRAME_8_20,
	PFRAME_9_20,
	PFRAME_10_20,
	PFRAME_11_20,
	PFRAME_12_20,
	PFRAME_13_20,
	PFRAME_14_20,
	PFRAME_15_20,
	PFRAME_16_20,
	PFRAME_17_20,
	PFRAME_18_20,
	PFRAME_19_20,
	PFRMAE_ALL,		/// 所有P帧
	IFRAME_ALL,			/// 所有帧，包括I帧
	QUALITY_NUM
}QUALITY_POLICY;

typedef enum fluency_policy
{
	FLUENCY_RET_MAIN = -1,	/// 切换回主码流
	FLUENCY_NONE = 0,		/// 正常，不处理
	FLUENCY_EXTRAM_100,		/// 切换到辅码流
	FLUENCY_EXTRAM_90,		/// 切换到辅码流, 码率为设置值的80%
	FLUENCY_EXTRAM_80,		/// 切换到辅码流, 码率为设置值的80%
	FLUENCY_EXTRAM_70,		/// 切换到辅码流, 码率为设置值的70%
	FLUENCY_EXTRAM_60,		/// 切换到辅码流, 码率为设置值的60%
	FLUENCY_EXTRAM_50,		/// 切换到辅码流, 码率为设置值的50%
	FLUENCY_EXTRAM_40,		/// 切换到辅码流, 码率为设置值的40%
	FLUENCY_EXTRAM_30,		/// 切换到辅码流, 码率为设置值的30%
	FLUENCY_EXTRAM_20,		/// 切换到辅码流, 码率为设置值的20%
	FLUENCY_NUM
}FLUENCY_POLICY;

typedef enum auto_policy
{
	AUTO_NONE = 0,			/// 正常，不处理
	AUTO_PFRAME_1_10,		/// 丢帧主码流设定帧率的1/10
	AUTO_PFRAME_2_10,		/// 丢帧主码流设定帧率的2/10
	AUTO_PFRAME_3_10,		/// 丢帧主码流设定帧率的3/10
	AUTO_PFRAME_4_10,		/// 丢帧主码流设定帧率的4/10
	AUTO_PFRAME_5_10,		/// 丢帧主码流设定帧率的5/10
	AUTO_PFRAME_6_10,		/// 丢帧主码流设定帧率的6/10
	AUTO_EXTRAM_100,		/// 切换到辅码流
	AUTO_EXTRAM_100_1_10,	/// 丢帧辅码流设定帧率的1/10
	AUTO_EXTRAM_100_2_10,	/// 丢帧辅码流设定帧率的2/10
	AUTO_EXTRAM_100_3_10,	/// 丢帧辅码流设定帧率的3/10
	AUTO_EXTRAM_100_4_10,	/// 丢帧辅码流设定帧率的4/10
	AUTO_EXTRAM_100_5_10,	/// 丢帧辅码流设定帧率的5/10
	AUTO_EXTRAM_100_6_10,	/// 丢帧辅码流设定帧率的6/10
	AUTO_EXTRAM_70,			/// 切换到辅码流, 码率为设置值的70%
	AUTO_EXTRAM_70_1_10,	/// 丢帧设定帧率的1/10
	AUTO_EXTRAM_70_2_10,	/// 丢帧设定帧率的2/10
	AUTO_EXTRAM_70_3_10,	/// 丢帧设定帧率的3/10
	AUTO_EXTRAM_70_4_10,	/// 丢帧设定帧率的4/10
	AUTO_EXTRAM_70_5_10,	/// 丢帧设定帧率的5/10
	AUTO_EXTRAM_70_6_10,	/// 丢帧设定帧率的6/10
	AUTO_EXTRAM_50,			/// 切换到辅码流, 码率为设置值的50%
	AUTO_EXTRAM_50_1_10,	/// 丢帧设定帧率的1/10
	AUTO_EXTRAM_50_2_10,	/// 丢帧设定帧率的2/10
	AUTO_EXTRAM_50_3_10,	/// 丢帧设定帧率的3/10
	AUTO_EXTRAM_50_4_10,	/// 丢帧设定帧率的4/10
	AUTO_EXTRAM_50_5_10,	/// 丢帧设定帧率的5/10
	AUTO_EXTRAM_50_6_10,	/// 丢帧设定帧率的6/10
	AUTO_EXTRAM_30,			/// 切换到辅码流, 码率为设置值的30%
	AUTO_EXTRAM_30_1_10,	/// 丢帧设定帧率的1/10
	AUTO_EXTRAM_30_2_10,	/// 丢帧设定帧率的2/10
	AUTO_EXTRAM_30_3_10,	/// 丢帧设定帧率的3/10
	AUTO_EXTRAM_30_4_10,	/// 丢帧设定帧率的4/10
	AUTO_EXTRAM_30_5_10,	/// 丢帧设定帧率的5/10
	AUTO_EXTRAM_30_6_10,	/// 丢帧设定帧率的6/10
	AUTO_EXTRAM_20,			/// 切换到辅码流, 码率为设置值的20%
	AUTO_EXTRAM_20_1_10,	/// 丢帧设定帧率的1/10
	AUTO_EXTRAM_20_2_10,	/// 丢帧设定帧率的2/10
	AUTO_EXTRAM_20_3_10,	/// 丢帧设定帧率的3/10
	AUTO_EXTRAM_20_4_10,	/// 丢帧设定帧率的4/10
	AUTO_EXTRAM_20_5_10,	/// 丢帧设定帧率的5/10
	AUTO_EXTRAM_20_6_10,	/// 丢帧设定帧率的6/10
	AUTO_NUM
}AUTO_POLICY;

typedef enum adapter_mini_frame_num
{
	QUALITY_MIN_FRAME_NUM = 6,		/// 画质优先时最小帧率
	FLUENCY_MIN_FRAME_NUM = 20,		/// 流畅优先时最小帧率
}ADAPTER_MINI_FRAME_NUM;

typedef enum connect_type
{
	NONE_CONNECT		= 0,
	MAIN_CONNECT		= 1,
	CLIENT_CONNECT		= 2,
	MONITOR_CONNECT		= 3,
	PLAY_CONNECT		= 4,
	TALK_CONNECT		= 5,
}CONNECT_TYPE;

#endif //_NETIP_H



