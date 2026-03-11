
#ifndef __NVMS_DEFINE_
#define __NVMS_DEFINE_

/************************************************************************/
/* 2个或者2个以上模块需要用到的内容可以放到这里    
主要的模块群有： 网络模块， APP层，配置层， 录像模块*/
/************************************************************************/

#include "base_type_define.h"
#pragma pack(4)

extern char g_szSuperUser[64];//super user

enum PLATFORM_TYPE
{
	Enterprise_Type  = 0,//企业版
	Standard_Type	 = 1,//标准版
	Lite_Type = 2, //单机版
	Enterprise_Type_TYCO  = 3,//泰科企业版
	Standard_Type_TYCO	 = 4,//泰科标准版
	Lite_Type_TYCO = 5, //泰科单机版
	Ai_type = 6,//ai版本
	Professional_Type	 = 7,//专业版
};

enum SPECIAL_VERSION_CONTROL_TYPE
{
	SVCT_PARK		= 0,//园区		
	SVCT_COMMUNITY	= 1,//社区
	SVCT_PMS		= 2,//PMS
};

typedef struct 
{
	tuint32 nPlatformType;//平台版本类型 0-企业 1-标准 2-lite
	char szPlatformVersion[64];//平台版本   NVMS_VERSION
	tuint32 nChannelCount;//通道数量
	tuint32 length;         //本结构体长度
	tuint32 nDataLen;//二进制数据长度，不包含本结构体长度
	tuint32 nDevCount;//设备数量
	char szCheck[64]; //导入导出加密版本判断密码是否正确的校验值
	tuint32 nProtocolVer;
	char szRes[512-4-68];//预留 -4是用于nDevCount
}ST_NCFG_EXPORT_DATA_HEAD; 

namespace NVMS_DEFINE
{
	/************************************************************************/
	/*             此文件包括所有ECMS的公用结构体定义，后期进行分离的时候再界定其包括的内容    */
	/************************************************************************/
	////////////////////////////无效的节点ID
	const tuint32 INVALID_NODE_ID = 0;///////////0表示无效的节点ID，有效节点ID的起始值为1
	//////////////////////////////录像的最大分组数量
	const int MAX_PARTITION_GROUP_NUM = 5;

	//录像数据类型
	typedef enum _record_type
	{
		REC_TYPE_NULL	= 0x00,			//空
		REC_TYPE_MANUAL	= 0x01,			//手动录像
		REC_TYPE_SCHEDULE = 0x02,		//排程录像
		REC_TYPE_MOTION	= 0x04,			//移动侦测录像
		REC_TYPE_SENSOR	= 0x08,			//传感器录像
		REC_TYPE_GSENSOR = 0x10,		//GSENSOR, 车载设备
		REC_TYPE_SHELTER   = 0x20,		//遮挡报警
        REC_TYPE_OVERSPEED = 0x40,		//超速(GPS) 
        REC_TYPE_OVERBOUND = 0x80,		//越界(GPS)
		REC_TYPE_OSC = 0x100,           //物品看护(智能)
		REC_TYPE_AVD = 0x200,           //视频异常(智能)
		REC_TYPE_TRIPWIRE = 0x400,		//越界(智能)
		REC_TYPE_PERIMETER = 0x800,		//区域入侵(智能)
		REC_TYPE_VFD    = 0x1000,		//人脸识别(识别抓拍)
		REC_TYPE_POS	= 0x2000,		//POS
		REC_TYPE_VLOSS	= 0x4000,		//视频丢失
		REC_TYPE_CPC	= 0x8000,		//人数统计（笼统，具体有进入，离开，滞留）
		REC_TYPE_CDD	= 0x10000,		//人群密度 & 拥挤（拥挤为2020-6-18 新增）
		REC_TYPE_IPD	= 0x20000,		//人员入侵
		REC_TYPE_SUBSYSTEM = 0x40000,   //子系统
		REC_TYPE_ZONE	= 0x80000,		//防区
		REC_TYPE_ACSDOOR = 0x100000,	//门禁
		REC_TPYE_ACSDEVICE_ATTEND = 0x200000,//考勤
		REC_TYPE_FACE_MATCH = 0x400000, //人脸比对(比对，笼统，具体有白名单，黑名单)
		REC_TYPE_SMART_PLATE_MATCH = 0x800000, // 车牌比对
        
		REC_TYPE_FACE_EVENT = REC_TYPE_VFD | REC_TYPE_FACE_MATCH, // 人脸事件
		REC_TYPE_VIDEO_CHECK = REC_TYPE_AVD | REC_TYPE_VLOSS | REC_TYPE_SHELTER, //视频诊断
                                                                                 //REC_TYPE_TEMPERATURE, // 体温异常 （2020-6-18 新增， 并入行为分析）                                             //REC_TYPE_WEARMASK, // 戴口罩 （2020-6-18 新增, 并入行为分析）
		REC_TYPE_INTELLIGENT = REC_TYPE_OSC |  REC_TYPE_TRIPWIRE | REC_TYPE_PERIMETER | REC_TYPE_IPD | REC_TYPE_CDD , //行为分析
		REC_TYPE_ZONE_SUBSYSTEM = REC_TYPE_SUBSYSTEM | REC_TYPE_ZONE , //子系统防区
		REC_TYPE_ACSDOOR_ATTEND = REC_TYPE_ACSDOOR | REC_TPYE_ACSDEVICE_ATTEND , //门禁考勤
		REC_TYPE_ALL	= 0xFFFFFFF,/////所有的录像类型 
		//由于存储服务器给管理上报录像状态时，是将录像类型上报到管理，管理判断在此区间则认为录像中，存储录像失败时会把错误码上报过去，这样如果定义为8个F，错误码也在此区间，则永远都是判断成了录像中,所以改成7个F
	}RECORD_TYPE;

	//录像事件
	typedef struct _record_event_log
	{
		union
		{
			tuint32 nodeID_ul;	//事件对应的通道节点ID
			GUID nodeID_GUID;	//
			struct {tuint8 szIP[64];   tuint16  uPort;  tuint8 byChIndex; tuint8 byReserve;}nodeID_IP_Port;  //
		}nodeID;

		tuint32    type;	//事件类型：
		tuint32 dwStartTime;		//事件产生的开始时间   从1970 1 1号开始绝对时间的秒数
		tuint32 dwEndTime;			//事件的结束时间      从1970 1 1号开始绝对时间的秒数

		tuint32 dwDataLen;			//数据长度, MB为单位
	}RECORD_EVENT_LOG;


	//抓图类型类型
	typedef enum _tag_capture_picture_type
	{
		CAPTURE_TYPE_NULL = 0,
		CAPTURE_TYPE_MANNUEL,					//手动抓图
		CAPTURE_TYPE_ALARM,						//报警抓图
		CAPTURE_TYPE_PASS,						//行车抓图推送
	}CAPTURE_PICTURE_TYPE;

	//图片信息结构体
	typedef struct _tag_capture_picture_info
	{
		GUID ChGUID;				//通道GUID
		char szUserName[128];		//用户名
		FILETIME ftTime;			//图片时间
//		tuint32 byImgCounts;
// 		char szImgName[64];
// 		tuint32 byImgType;
		tuint8 byCapType;			//抓图类型,CAPTURE_PICTURE_TYPE
		unsigned char szPicType[3];	//图片类型,JPG,BMP,...  目前未使用
	}CAPTURE_PICTURE_INFO;

	enum ECMS_STREAM_TYPE
	{
		STREAM_NULL,

		LIVE_FIRST_STREAM,////现场主码流   请求和回复现场主码流的流类型
		LIVE_SECOND_STREAM,////现场子码流1  请求和回复现场子码流的流类型
		LIVE_SUB_STREAM_2,////现场子码流2  请求和回复现场子码流2的流类型
		LIVE_SUB_STREAM_3,////现场子码流3  请求和回复现场子码流3的流类型
		LIVE_SUB_STREAM_4,////现场子码流4  请求和回复现场子码流4的流类型

		LIVE_AUDIO_STREAM,/////现场音频流  回复现场音频的流类型

		PLAY_STREAM,///////回放流   请求和回复回放的码流类型

		TALKBACK_STREAM, ////对讲流   请求和回复对讲流的码流类型

		CMD_STREAM,	//命令流数据

		LIVE_STREAM_PIC,//现场图片流，主要用在黑莓手机上

		LIVE_STREAM_GPS,//实时行车数据

		PLAYBACK_STREAM_GPS,//历史行车数据

		LIVE_POS_STREAM,	//现场POS流

		STREAM_TYPE_NUM,	//流类型数量

		STREAM_END,	
	};	

	#define streamHEADERFLAG          MAKEFOURCC('S', 'H', 'F', 'L')

	typedef struct _stream_header_info 
	{	
		tuint32 swFlag;//////////////帧头标记	
		tuint16 swProductID;      //////////产品标识，标识不同公司的产品的码流	
		unsigned char byStreamType;          ////流类型  包括现场主视频流  现场子视频流  现场音频流   回放视音频流   对讲音频流流
		unsigned char byIsKeyFrame;           ///////音频流默认全当关键帧	
		GUID    nodeID;//////////////ID
		tuint32 dwBufferLen;///////////不包括本结构体后面缓冲区数据的长度，此长度是4的倍数  不仅仅包括帧数据还包括为了处理4字节对齐的数据
		/*共有数据由存储服务器录像的时候产生，由于从前端设备出去的流可能为0，
		所以不要用此时间控制播放，单位是PC上毫秒级的
		绝对时间，误差为15毫秒左右
		*/
		FILETIME   ftRecTime; //可以理解为到达存储服务器的时间，录像的时候产生的时间，跟前端设备没关系，主要用来做定位用

		/*此信息后面直接拿出去，另外在发送之前加到帧的外面*/
		tuint32 dwSendFrameIndex; ////发送的帧的索引值	
		tuint32 dwTaskID; ////回放的任务ID
	}STREAM_HEADER_INFO;

	const size_t NET_STREAM_HEADER_LENGTH = sizeof(STREAM_HEADER_INFO);

	/************************************************************************
	以下是同为设备的私有数据信息，封装在解码库里面，如果是解码卡需要转化为解码卡
	支持的数据信息
	************************************************************************/
	/////帧类型
	typedef enum _frame_type
	{
		VIDEO, ////视频数据
		AUDIO, ///音频数据
		CMD_FRAME,	//命令帧
		DIRECT_DISPLAY, //直接显示帧,不通过播放控制,用于回放
		PREDECOD,		//预解码,只解码不播放,用于回放,如果丢弃,后续无法解码
		POS_FRAME,		//POS帧类型
        MOTION_FRAME,   //Motion特征数据帧类型
        FACE_FRAME,       //人脸检测特征数据帧类型
        DISPOSABLE_FRAME,	//可丢弃帧,用于回放,如非流畅性需要,可以直接丢弃
	}FRAME_TYPE;

	typedef enum _IP_frame_type
	{
		VI_FRAME_TYPE = 128,	//VI帧类型
	}IP_FRAME_TYPE;
		
	//////////////帧头信息，每帧数据都有此标记
	typedef struct _frame_header_info 
	{
		unsigned char      byFrameType;/////帧类型      视频或音频
		unsigned char      byExtInfoLen; /////扩充信息长度  如果有扩充信息为sizeof(FRAME_EXTENSION_INFO),否则为0
        unsigned char      byIPFrameType;	//IP帧类型, 目前用于DVR+VI,定义见IP_FRAME_TYPE//
		unsigned char      byRes;			//保留位
		tuint32      dwRealFrameLen;/////////不包括任何结构体的有效帧数据的长度  
		FILETIME	ftDevTime;/////时间戳上的时间,也就是设备采集此帧的绝对时间，私有数据由前端设备填充
		FILETIME    ftECMSTime; ////在ECMS上的系统绝对时间不会随设备时间变化而变化，同时记录了准确的帧间隔
						//注意此时间对于现场是以设备所在转发服务器为基准的时间，对录像数据是以存储服务器为基准的时间
	}FRAME_HEADER_INFO, *LPFRAME_HEADER_INFO;

	const size_t FRAME_HEADER_INFO_LENGTH = sizeof(FRAME_HEADER_INFO);

	const tuint32 ECMS_VIDEO_ENCODE_TYPE_H264 = 	((tuint32)(BYTE)('H') | ((tuint32)(BYTE)('2') << 8) |   \
		((tuint32)(BYTE)('6') << 16) | ((tuint32)(BYTE)('4') << 24 ));
	const tuint32 ECMS_VIDEO_ENCODE_TYPE_H265 = 	((tuint32)(BYTE)('H') | ((tuint32)(BYTE)('E') << 8) |   \
		((tuint32)(BYTE)('V') << 16) | ((tuint32)(BYTE)('C') << 24 ));
	const tuint32 ECMS_VIDEO_ENCODE_TYPE_MJPEG = ((tuint32)(BYTE)('M') | ((tuint32)(BYTE)('J') << 8) |   \
		((tuint32)(BYTE)('P') << 16) | ((tuint32)(BYTE)('G') << 24 ));
	enum AUDIO_FORMAT
	{
		AUDIO_FORMAT_PCM,	//标准PCM可以直接播放数据
		AUDIO_FORMAT_ALAW,	//标准G711_A
		AUDIO_FORMAT_HI_ADPCM,//海斯ADPCM格式
		AUDIO_FORMAT_HI_ALAW,//海斯G711_A格式
		AUDIO_FORMAT_HK711,//海康711声音格式
		AUDIO_FORMAT_HK722,//海康722声音格式
		AUDIO_FORMAT_ULAW,	//标准G711_U
		AUDIO_FORMAT_HI_ULAW,	//海斯G711_U
	};

	/////帧特性,按位占用
	typedef enum _frame_propety
	{
		FRAME_PROPETY_NULL = 0,
		FRAME_PROPETY_FISHEYE_DESKTOP = 0x01,		//鱼眼桌面
		FRAME_PROPETY_FISHEYE_CEILING = 0x02,		//鱼眼顶装
		FRAME_PROPETY_FISHEYE_WALL = 0x04,			//鱼眼壁装
	}FRAME_PROPETY;

	////////////////////关键帧扩展信息，录像信息里面保留的
	typedef union _frame_extension_info 
	{
		struct 
		{
			unsigned char byVersion;//////////////扩展信息版本
			unsigned char byVideoFormat;/////视频制式  0 PAL   1为N制
			unsigned char      byFramePropety;	//帧特性, 按位占用, 最多8种,FRAME_PROPETY中定义
			tuint8             byReserver;///////////
			tuint32				dwVideoEncodeType;//////////////视频编码类型
			tuint16				swVideoWidth;//////////////视频宽度
			tuint16				swVideoHeight;/////////////高度
		}video_frame_ext_Info;

		struct 
		{
			unsigned char		byVersion;//////////////扩展信息版本
			unsigned char		byChannels;//////////////声道数量
			tuint16				wAudioEncodeType;//////////////音频编码类型
			tuint16				wBitsPerSample; ////
			tuint16				wAvgBytesPerSec;//////采样平均位数
			tuint32				dwSamplesPerSec;//////////////采样率
		}audio_frame_ext_info;

	}FRAME_EXTENSION_INFO, *LPFRAME_EXTENSION_INFO;

	const size_t FRAME_EXTENSION_INFO_LENGTH = sizeof(FRAME_EXTENSION_INFO);

	// 图片格式
	enum
	{
		IMG_TYPE_VALID = 0,
		IMG_TYPE_BMP = 1,
		IMG_TYPE_JPG = 2,
		IMG_TYPE_JPEG = IMG_TYPE_JPG,
		IMG_TYPE_PNG = 3,
	};



	/************************************************************************/
	/*  定义报警输入和输出的资源    在很多地方使用这个枚举值的时候定义的变量都是BYTE的所以这里
	要小心定义*/
	/************************************************************************/
	//写入日志的时候也是用的这个类型,包括报警,操作,设置,信息,每加完一个枚举值要在EventDictionary中加入相应翻译
	enum  ECMS_ALARM_TYPE
	{
		ECMS_ALARM_TYPE_RANGE_BEGIN,
		//监控点报警类型
        ECMS_ALARM_TYPE_CHANNEL_BEGIN = 0x01,
		ECMS_ALARM_TYPE_MOTION = ECMS_ALARM_TYPE_CHANNEL_BEGIN,/////移动侦测报警输入
		ECMS_ALARM_TYPE_SENSOR,/////传感器报警输入
		ECMS_ALARM_TYPE_VLOSS,////视频丢失报警输入
        ECMS_ALARM_TYPE_FRONT_OFFLINE, //////前端设备掉线报警
		ECMS_ALARM_TYPE_OSC,////物品看护侦测报警 
		ECMS_ALARM_TYPE_AVD,////视频异常侦测报警 
		ECMS_ALARM_TYPE_PEA_TRIPWIRE, ////越界侦测报警 
		ECMS_ALARM_TYPE_PEA_PERIMETER, ////区域入侵侦测报警 
		ECMS_ALARM_TYPE_PEA, ////越界侦测与区域入侵侦测
		ECMS_ALARM_TYPE_VFD,//人脸检测报警 //这里往上是和N9K合的
		ECMS_ALARM_TYPE_CPC,//人数统计报警，笼统的（设备暂时上报的没有做区分，所以只能采用笼统这个）
		ECMS_ALARM_TYPE_ENTER_AREA_OVER_LINE,//人数统计： 1、进入人数超过指定阈值， EnterAreaOverLine
		ECMS_ALARM_TYPE_LEAVE_AREA_OVER_LINE,//人数统计：2、离开人数超过指定阈值LeaveAreaOverLine
		ECMS_ALARM_TYPE_STAY_AREA_OVER_LINE,//人数统计：3、滞留人数超过指定阈值， StayAreaOverLine
		ECMS_ALARM_TYPE_TARGET_IN_BLACKLIST,//人脸布控： 1、黑名单匹配，TargetInBlackList
		ECMS_ALARM_TYPE_TARGET_UNDEFINED,//人脸布控：2、白名单不匹配， IllegalTarget 或者 TargetUndefined（I开头容易和l混淆）
		// ECMS_ALARM_TYPE_TARGET_UNDEFINED 需要修改为 ECMS_ALARM_TYPE_TARGET_NOT_IN_WHITELIST
		ECMS_ALARM_TYPE_CDD,//人群密度检测报警
		ECMS_ALARM_TYPE_IPD,//人员入侵侦测报警
		ECMS_ALARM_TYPE_CHANNEL_OFFLINE,//监控点下线报警
		ECMS_ALARM_TYPE_FACE_MATCH, //人脸比对报警
		ECMS_ALARM_TYPE_BLACKLIST_CAR_PASS, //黑名单车辆通行报警
		ECMS_ALARM_TYPE_AOIENTRY,//进入区域
		ECMS_ALARM_TYPE_AOILEAVE,//离开区域
		ECMS_ALARM_TYPE_SENCE_CHANGE,   // 场景变更（视频遮挡）
		ECMS_ALARM_TYPE_CAR_PASS_NOBLACKLIST,//白名单车辆的通行报警
		ECMS_ALARM_TYPE_CAR_PASS_TEMPCAR,//临时车辆的通行报警
        // 新增捷尚报警类型，由于类型个数超过预留，把后面不用的车载相关报警类型注释，20180518，wangjiaying
        //ECMS_ALARM_TYPE_FOLLOW = 0x2A,        // 人员尾随
        //ECMS_ALARM_TYPE_MASK_COVER,    // 口罩遮挡
        //ECMS_ALARM_TYPE_GLASS_COVER,   // 墨镜遮挡
        //ECMS_ALARM_TYPE_MULTI_FACE,    // 多人脸检测
        //ECMS_ALARM_TYPE_HELMATE_COVER, // 头盔遮挡
        //ECMS_ALARM_TYPE_CLEAR_FACE,    // 清晰人脸
        //ECMS_ALARM_TYPE_FIGHT,         // 打砸
        //ECMS_ALARM_TYPE_SMASH,         // 打架
        //ECMS_ALARM_TYPE_OBJECT_STAY,   // 物体滞留
        //ECMS_ALARM_TYPE_OBJECT_TAKEN,  // 目标拿走
        //ECMS_ALARM_TYPE_STAY_LONG,     // 取款超时
        //ECMS_ALARM_TYPE_HUMAN_IN,      // 人体进入
        //ECMS_ALARM_TYPE_HUMAN_OUT,     // 人体离开
        ECMS_ALARM_TYPE_TEMP_CAR_PARKING_OVERTIME,//临时车辆超时停放报警
		ECMS_ALARM_TYPE_INDUSTRY_THERMALEX_HIGH_WARNING,//工业相机高温预警
		ECMS_ALARM_TYPE_INDUSTRY_THERMALEX_HIGH_ALARM,//工业相机高温报警
        ECMS_ALARM_TYPE_STRANGER_MATCHING,          // 人脸布控：陌生人匹配
        ECMS_ALARM_TYPE_BLACKLIST_MATCHING,         // 人脸布控：黑名单匹配
		ECMS_ALARM_TYPE_INDUSTRY_THERMALEX_LOW_WARNING,//工业相机低温预警
		ECMS_ALARM_TYPE_INDUSTRY_THERMALEX_LOW_ALARM,//工业相机低温报警
        ECMS_ALARM_TYPE_CHANNEL_END,

		////和车载有关的报警
        //ECMS_ALARM_TYPE_GPS_BEGIN = 0x21,
		//ECMS_ALARM_TYPE_GPS_SPEED_OVER= ECMS_ALARM_TYPE_GPS_BEGIN,//超速
		//ECMS_ALARM_TYPE_GPS_CROSS_BOADER,//越界
		//ECMS_ALARM_TYPE_GPS_TEMPERATURE_OVER,//温度报警
		//ECMS_ALARM_TYPE_GPS_GSENSOR_X,//GSENSOR报警
		//ECMS_ALARM_TYPE_GPS_GSENSOR_Y,
		//ECMS_ALARM_TYPE_GPS_GSENSOR_Z,
        //ECMS_ALARM_TYPE_GPS_END,

		//设备报警
        ECMS_ALARM_TYPE_DEVICE_BEGIN = 0x41,
		NVMS_ALARM_TYPE_EXCEPTION = ECMS_ALARM_TYPE_DEVICE_BEGIN,
		NVMS_ALARM_TYPE_IP_CONFLICT,   /////IP地址冲突
		NVMS_ALARM_TYPE_DISK_IO_ERROR, /////磁盘IO错误
		NVMS_ALARM_TYPE_DISK_FULL,	   /////磁盘满
		NVMS_ALARM_TYPE_RAID_SUBHEALTH, //阵列亚健康
		NVMS_ALARM_TYPE_RAID_UNAVAILABLE, //阵列不可用 
		NVMS_ALARM_TYPE_ILLEIGAL_ACCESS,  /////非法访问
        NVMS_ALARM_TYPE_NET_DISCONNECT,  /////网络断开
        NVMS_ALARM_TYPE_NO_DISK,		////盘组下没有磁盘
        NVMS_ALARM_TYPE_SIGNAL_SHELTER, //信号遮挡
        NVMS_ALARM_TYPE_HDD_PULL_OUT, //前面板硬盘拔出
		ECMS_ALARM_TYPE_DEVICE_OFFLINE,//设备下线报警
		NVMS_ALARM_TYPE_RAID_HOT_ERROR,//热备错误
		NVMS_ALARM_TYPE_RAID_EXCEPTION,//阵列异常
        ECMS_ALARM_TYPE_DEVICE_END,

		NVMS_ALARM_TYPE_ALARM_OUT = 0x51,  /////报警输出的类型，报警输出也有状态需要管理

		//针对报警主机
		/////第三方告警服务器上报的告警，子系统：布防/撤防           防区：告警/消除告警， 旁路/旁路恢复
		/////第三方告警服务器反控操作，  子系统：布防/撤防/消除告警  防区：旁路，旁路恢复

        ECMS_ALARM_TYPE_SUBSYSTEM_BEGIN = 0x61,
		ECMS_ALARM_TYPE_ARM = ECMS_ALARM_TYPE_SUBSYSTEM_BEGIN, /////布防 子系统
		ECMS_ALARM_TYPE_DISARM,				/////撤防 子系统
		ECMS_ALARM_TYPE_CLEAR_ALARM,	/////消警 子系统
        ECMS_ALARM_TYPE_SUBSYSTEM_END = ECMS_ALARM_TYPE_CLEAR_ALARM,
        ECMS_ALARM_TYPE_ZONE_BEGIN,
		ECMS_ALARM_TYPE_BYPASS = ECMS_ALARM_TYPE_ZONE_BEGIN, /////旁路 防区
 		ECMS_ALARM_TYPE_BYPASSRES,			/////旁路恢复 防区
		ECMS_ALARM_TYPE_ZONE_ALARM,			/////防区报警 防区
		ECMS_ALARM_TYPE_ZONE_ALARMRES,			/////防区报警恢复 防区
		ECMS_ALARM_TYPE_ALARMHOST_OFFLINE,			/////报警主机下线
        ECMS_ALARM_TYPE_ZONE_END,

		//针对门禁系统报警
		ECMS_ALARM_TYPE_ACSSYSTEM_BEGIN = 0x81,
		ECMS_ALARM_TYPE_ACSDOOR_ALARM,//门禁事件报警
		ECMS_ALARM_TYPE_ACSDEVICE_ATTEND_ALARM,//考勤事件报警
		ECMS_ALARM_TYPE_ACSSYSTEM_OFFLINE,			//门禁系统下线
		ECMS_ALARM_TYPE_ACSSYSTEM_END,

		//针对服务器报警
		ECMS_ALARM_TYPE_SERVER_BEGIN = 0x91,
		ECMS_ALARM_TYPE_SERVER_OFFLINE,//服务器下线报警
		ECMS_ALARM_TYPE_RAID_NOUSE,//无可用阵列
		ECMS_ALARM_TYPE_RAID_EXCEPTION,//阵列异常
		ECMS_ALARM_TYPE_RAID_REBUILD,//阵列重建
		ECMS_ALARM_TYPE_RAID_DEGRADE,//阵列降级Degrade
		ECMS_ALARM_TYPE_NO_DISK,//无存储磁盘
		ECMS_ALARM_TYPE_NO_PARTION,//无分区
		ECMS_ALARM_TYPE_OPENFILE_ERR,//打开文件失败
		ECMS_ALARM_TYPE_ROUTING_CHECK_OFF_LINE,//服务器巡检不在线
		ECMS_ALARM_TYPE_RAID_DISKBAD,
		ECMS_ALARM_TYPE_SERVER_END,

		//针对解码设备报警
		ECMS_ALARM_TYPE_DECODER_BEGIN = 0xA1,
		ECMS_ALARM_TYPE_DECODER_OFFLINE,//解码设备下线报警

		//针对智能告警（智能告警也属于监控点告警）
		ECMS_ALARM_TYPE_INTELLIGENT_BEGIN = 0xB1,
		ECMS_ALARM_TYPE_VQD_DARK,	          		//亮度异常过暗
		ECMS_ALARM_TYPE_VQD_LIGHT,                  //亮度异常过亮
		ECMS_ALARM_TYPE_VQD_BLUR,                   //清晰度异常
		ECMS_ALARM_TYPE_VQD_NOISE,                  //雪花噪声异常
		ECMS_ALARM_TYPE_VQD_STRIATION,              //条纹噪声异常
		ECMS_ALARM_TYPE_VQD_JITTER,                 //抖动异常
		ECMS_ALARM_TYPE_VQD_FROZEN,                 //冻结异常
		ECMS_ALARM_TYPE_VQD_COVER,                  //遮挡异常
		ECMS_ALARM_TYPE_VQD_LOSS,                   //信号缺失异常
		ECMS_ALARM_TYPE_VQD_SCENE,                  //场景变换异常
		ECMS_ALARM_TYPE_VQD_CAMMOVE,                //相机移动异常
		ECMS_ALARM_TYPE_VQD_PTZERR,                 //ptz控制异常
		ECMS_ALARM_TYPE_VQD_UNBALANCE,              //偏色异常
		ECMS_ALARM_TYPE_SSB_TRAIL,            		//尾随取款
		ECMS_ALARM_TYPE_SSB_HOVER,                  //人员徘徊
		ECMS_ALARM_TYPE_SSB_FELL,                   //人员倒地
		ECMS_ALARM_TYPE_SSB_GATHER,                 //多人聚集
		ECMS_ALARM_TYPE_SSB_REMNANTS,               //遗留物
		ECMS_ALARM_TYPE_SSB_FIGHT,                  //行为过激
		ECMS_ALARM_TYPE_SSB_INVADE,                 //加钞间入侵
		ECMS_ALARM_TYPE_SSB_SUSPICIOUS_TRAIL,       //疑似尾随
		ECMS_ALARM_TYPE_SSB_HIJACK,                 //人员挟持
		ECMS_ALARM_TYPE_SSB_HUMAN_NUMBER,           //加钞间人数异常
		ECMS_ALARM_TYPE_ATM_PANELPASTE,       		//检测区异物
		ECMS_ALARM_TYPE_ATM_KEYBOARD,               //键盘区异常
		ECMS_ALARM_TYPE_ATM_CARDPORT,               //插卡口异常
		ECMS_ALARM_TYPE_ATM_KEYMASK,                //防护罩异常
		ECMS_ALARM_TYPE_ATM_ENTER,                  //人员进入
		ECMS_ALARM_TYPE_ATM_LEAVE,                  //人员离开
		ECMS_ALARM_TYPE_ATM_STAY,	                //人员长时间操作
		ECMS_ALARM_TYPE_ATM_FACECAPTURE,            //ATM人脸捕获
		ECMS_ALARM_TYPE_ATM_FACEABNORMAL,           //ATM异常人脸
		ECMS_ALARM_TYPE_ATM_SMASH,                  //舱内打砸
		ECMS_ALARM_TYPE_ATM_TANKDOWN,               //舱内人员倒地
		ECMS_ALARM_TYPE_ATM_BAGGAGE,                //舱内遗留物
		ECMS_ALARM_TYPE_ATM_GATHER,                 //舱内尾随
		ECMS_ALARM_TYPE_ATM_MULTIFACE,              //多人脸检测
		ECMS_ALARM_TYPE_ATM_FIGHT,				  	//舱内打架
		ECMS_ALARM_TYPE_ATM_HIJACK,                 //舱内劫持
		ECMS_ALARM_TYPE_ATM_INVADE,				  	//舱内强行推入
		ECMS_ALARM_TYPE_ATM_FIRE,                   //舱内火焰检测
		ECMS_ALARM_TYPE_ATM_FAKEKEYBOARD,           //假键盘
		ECMS_ALARM_TYPE_ATM_FAKECARDREADER,         //假读卡器
		ECMS_ALARM_TYPE_ATM_PHONEFRAUD,             //电话诈骗
		ECMS_ALARM_TYPE_SUSPECTED_FEVER,             //疑似发热Suspected fever
		ECMS_ALARM_TYPE_UNMASK,             //未戴口罩
		/*ECMS_ALARM_TYPE_RANGE_END = 0xFF,////不能超过这个值  否则会出错*/

		ECMS_ALARM_TYPE_KANTECHDOOR_ALARM_BEGIN = 0x100,  //泰科KANTECH门禁报警新增,对应ENUM字典从201-232之间的报警
		ECMS_ALARM_TYPE_KANTECHDOOR_ALARM_SECOND_ENTRANCE,  
		ECMS_ALARM_TYPE_KANTECHDOOR_ALARM_UNLOCK,
		ECMS_ALARM_TYPE_KANTECHDOOR_ALARM_GRANTED,
		ECMS_ALARM_TYPE_KANTECHDOOR_ALARM_CARD_UNKNOWN,
		ECMS_ALARM_TYPE_KANTECHDOOR_ALARM_BADCARD,
		ECMS_ALARM_TYPE_KANTECHDOOR_ALARM_CARD_LOSTSTOLEN,
		ECMS_ALARM_TYPE_KANTECHDOOR_ALARM_CARD_EXPIRED,
		ECMS_ALARM_TYPE_KANTECHDOOR_ALARM_CARD_PENDING,
		ECMS_ALARM_TYPE_KANTECHDOOR_ALARM_CARD_COUNT,
		ECMS_ALARM_TYPE_KANTECHDOOR_ALARM_SCHEDULE_EXCEPTION,
		ECMS_ALARM_TYPE_KANTECHDOOR_ALARM_BAD_ACCESS,
		ECMS_ALARM_TYPE_KANTECHDOOR_ALARM_SUPLEVEL_REQUIRED,
		ECMS_ALARM_TYPE_KANTECHDOOR_ALARM_PASSBACKBAD_LOCATION,
		ECMS_ALARM_TYPE_KANTECHDOOR_ALARM_DUAL_CUSTODY,
		ECMS_ALARM_TYPE_KANTECHDOOR_ALARM_NUMBER_AREA,
		ECMS_ALARM_TYPE_KANTECHDOOR_ALARM_ALARMSYSTEM_ARMED,
		ECMS_ALARM_TYPE_KANTECHDOOR_ALARM_AREA_NOTCLEAR,
		ECMS_ALARM_TYPE_KANTECHDOOR_ALARM_INTERLOCK_ACTIVE,
		ECMS_ALARM_TYPE_KANTECHDOOR_ALARM_DOORMANUALLY_DISABLED,
		ECMS_ALARM_TYPE_KANTECHDOOR_ALARM_NOTENOUGH_CARDS,
		ECMS_ALARM_TYPE_KANTECHDOOR_ALARM_CARD_BUSY,
		ECMS_ALARM_TYPE_KANTECHDOOR_ALARM_MINPASSBACK_DELAY,
		ECMS_ALARM_TYPE_KANTECHDOOR_ALARM_PASSBACK_BADLOCATION,
		ECMS_ALARM_TYPE_KANTECHDOOR_ALARM_AREA,
		ECMS_ALARM_TYPE_KANTECHDOOR_ALARM_WAIT_APPROVAL,
		ECMS_ALARM_TYPE_KANTECHDOOR_ALARM_READER_LOCKED,
		ECMS_ALARM_TYPE_KANTECHDOOR_ALARM_SECOND_CARD,
		ECMS_ALARM_TYPE_KANTECHDOOR_ALARM_DOOR_ARMED,
		ECMS_ALARM_TYPE_KANTECHDOOR_ALARM_ACCESS_DENIED,
		ECMS_ALARM_TYPE_KANTECHDOOR_ALARM_DOOR_FORCED_OPEN,
		ECMS_ALARM_TYPE_KANTECHDOOR_ALARM_DOOR_FORCED_OPEN_RESTORED,
		ECMS_ALARM_TYPE_KANTECHDOOR_ALARM_PREALARM_DOOR_OPEN_TOOLONG,
		ECMS_ALARM_TYPE_KANTECHDOOR_ALARM_DOOR_OPEN_TOOLONG,
		ECMS_ALARM_TYPE_KANTECHDOOR_ALARM_END,

        //针对任务报警
        ECMS_ALARM_TYPE_ALARM_TASK_BEGIN = 0x200,
        ECMS_ALARM_TYPE_ALARM_TASK_LEAVE_POST,//离岗报警
		ECMS_ALARM_TYPE_ALARM_TASK_ENTER_HIGH_COUNT,//阈值报警
        ECMS_ALARM_TYPE_ALARM_TASK_END,
		
		ECMS_ALARM_TYPE_RANGE_END
	};
	//操作日志类型
	enum  ECMS_OPERATE_TYPE
	{
		/**************************************/
		ECMS_OPERATE_TYPE_RANGE_BEGIN = 0x1000,//用于写日志
		/**************************************/


		ECMS_OPERATE_TYPE_RECORD_SPB,//录像检索回放备份（检索search，回放playback，备份backup）
		ECMS_OPERATE_TYPE_MANUAL_RECORD,//手动录像（开始/停止手动录像）
		ECMS_OPERATE_TYPE_MANUAL_ALARM,//手动报警（手动触发报警/手动消除报警）
		ECMS_OPERATE_TYPE_SYSTEM_MAINTENANCE,//系统维护（升级，恢复出厂设置，备份和还原配置，查询和导出日志）
		ECMS_OPERATE_TYPE_PTZ_CONTROL,//云台控制（开始/停止巡航线，开始/停止轨迹，云台方向控制）
		ECMS_OPERATE_TYPE_AUDIO_TALK,//语音对讲（开启对讲，关闭对讲）
		ECMS_OPERATE_TYPE_SYSTEM_SCR,//开关机（开机start，关机close，重新启动restart）
		ECMS_OPERATE_TYPE_LOGIN_LOGOUT,//登入/登出
		ECMS_OPERATE_TYPE_SNAPSHOT_MSPB,//图片（手动抓拍manual snap，图片检索search，图片预览play，图片备份导出backup）
		ECMS_OPERATE_TYPE_CAPTURE_MACHINE,//抓拍机（添加，删除，修改）
		ECMS_OPERATE_TYPE_LED,//LED显示（添加，删除，修改,显示）
		ECMS_OPERATE_TYPE_BARRIER_GATE,//道闸（开）
		ECMS_OPERATE_TYPE_OPENDOOR_BYHAND,//门禁操作日志：手动开门操作
		ECMS_OPERATE_TYPE_SECURITYQUESTION,//密保问题操作日志：验证
		ECMS_OPERATE_TYPE_PREVIEW,// 预览视频
		ECMS_OPERATE_TYPE_PLAYBACK,// 录像回放
		ECMS_OPERATE_TYPE_RANGE_END=0x1FFF,
	};

	//设置日志类型
	enum  ECMS_CONFIG_TYPE
	{
		/**************************************/
		ECMS_CONFIG_TYPE_RANGE_BEGIN = 0x2000,//操作类型+1,用于写日志
		/**************************************/

		ECMS_CONFIG_TYPE_CHANNEL,//通道参数（包括通道增删改，分组增删改，轮询配置，图像配置，云台参数）
		ECMS_CONFIG_TYPE_RECORD,//录像参数
		ECMS_CONFIG_TYPE_ALARM,//报警参数
		ECMS_CONFIG_TYPE_DISK,//磁盘参数
		ECMS_CONFIG_TYPE_NETWORK,//网络参数
		ECMS_CONFIG_TYPE_SCHEDULE,//排程参数
		ECMS_CONFIG_TYPE_USER,//用户参数
		ECMS_CONFIG_TYPE_BASIC,//基本配置
		ECMS_CONFIG_TYPE_SERICUTYQUESTION, //密保问题

		ECMS_CONFIG_TYPE_RANGE_END = 0x2FFF,
	};

	//信息日志类型
	enum  ECMS_INFO_TYPE
	{
		/**************************************/
		ECMS_INFO_TYPE_RANGE_BEGIN = 0x3000,//配置类型+1,用于写日志
		/**************************************/

		ECMS_INFO_TYPE_SCHEDULE_RECORD,//排程录像开启/关闭
		ECMS_INFO_TYPE_SCHEDULE_SNAP,//定时抓图
		ECMS_INFO_TYPE_DISK,//硬盘信息
		ECMS_INFO_TYPE_NETWORK,//网络状态
		ECMS_INFO_TYPE_SYSTEM_BASE,//系统基本信息
		ECMS_INFO_TYPE_SYSTEM_RUN,//系统运行状态
		ECMS_INFO_TYPE_CHANNEL_STATE,//通道状态
		ECMS_INFO_TYPE_ALARM_STATE,//报警状态
		ECMS_INFO_TYPE_RECORD_STATE,//录像状态

		ECMS_INFO_TYPE_RANGE_END = 0x3FFF,
	};

	//异常日志类型
	enum  ECMS_EXCEPTION_TYPE
	{
		/**************************************/
		ECMS_EXCEPTION_TYPE_RANGE_BEGIN = 0x4000,//信息类型+1,用于写日志
		/**************************************/

		ECMS_EXCEPTION_TYPE_DISK_FULL,//磁盘满
		ECMS_EXCEPTION_TYPE_RAID_UNAVAILABLE,//无可用阵列
		ECMS_EXCEPTION_TYPE_NOT_DISK,//无磁盘
		ECMS_EXCEPTION_TYPE_NOT_PARTION,//无分区
        ECMS_EXCEPTION_TYPE_RAID_EXCEPTION,//阵列异常
        ECMS_EXCEPTION_TYPE_RAID_REBUILD,//阵列重建
        ECMS_EXCEPTION_TYPE_RAID_DEGRADE,//阵列降级
        ECMS_EXCEPTION_TYPE_RAID_NORMAL,//阵列恢复正常
        ECMS_EXCEPTION_TYPE_RAID_CREATE, //创建阵列
        ECMS_EXCEPTION_TYPE_RAID_DEL,//删除阵列
        ECMS_EXCEPTION_TYPE_RAIDDISK_ADD, //存储阵列-磁盘加入
        ECMS_EXCEPTION_TYPE_RAIDDISK_REMOVED, //存储阵列-磁盘退出
        ECMS_EXCEPTION_TYPE_RAIDDISK_SUBHEALTH, //存储阵列-磁盘亚健康
        ECMS_EXCEPTION_TYPE_RAIDDISK_BAD, //存储阵列-磁盘异常

		ECMS_EXCEPTION_TYPE_RANGE_END= 0x4FFF,
	};

	enum ECMS_STATU_TYPE
	{
		/**************************************/
		ECMS_INFO_STATUS_RANGE_BEGIN = 0x5000,//信息类型+1,用于写日志
		/**************************************/

		ECMS_INFO_STATUS_SERVER_ONLINE,//服务器上线
		ECMS_INFO_STATUS_SERVER_OFFLINE,//服务器下线
		ECMS_INFO_STATUS_PU_ONLINE,//编码设备上线
		ECMS_INFO_STATUS_PU_OFFLINE,//编码设备下线
		ECMS_INFO_STATUS_DECODER_ONLINE,//解码设备上线
		ECMS_INFO_STATUS_DECODER_OFFLINE,//解码设备下线
		ECMS_INFO_STATUS_ALARMHOST_ONLINE,//报警主机上线
		ECMS_INFO_STATUS_ALARMHOST_OFFLINE,//报警主机下线
		ECMS_INFO_STATUS_RECORD_ON, //正在录像
		ECMS_INFO_STATUS_RECORD_OFF,//未录像
		ECMS_INFO_STATUS_CHANNEL_ONLINE, //监控点上线
		ECMS_INFO_STATUS_CHANNEL_OFFLINE, //监控点下线

		ECMS_INFO_STATUS_RANGE_END= 0x5FFF,
	};
		/**各项任务类型**********************************************************************/
	enum  ECMS_TASK_TYPE
	{
		ECMS_TASK_TYPE_RANGE_BEGIN = 0x10000,//定义这么大是可能以后把报警联动排程也放到这张表了，提取兼容
		
		ECMS_TASK_TYPE_VFD=0x10001,//人脸检测

		ECMS_TASK_TYPE_SECURITYALARM = 0x10002,  //告警任务 ：离岗任务

		ECMS_TASK_TYPE_CPC = 0x10003,  //人数统计

		ECMS_TASK_TYPE_THRESHOLD = 0x10004,  //告警任务 ：阈值任务

		ECMS_TASKM_TYPE_RANGE_END = 0xFFFFFFFF
	};

	//中控智慧门禁事件类型编号
	enum  ECMS_ALARM_TYPE_ZKTECO
	{
		ECMS_ALARM_TYPE_ACS_Normal_verification_open=0,//正常验证开门 
		ECMS_ALARM_TYPE_ACS_Verification_in_normally_open_time=1,//常开时间段内验证  
		ECMS_ALARM_TYPE_ACS_First_person_open=2,//首人开门  
		ECMS_ALARM_TYPE_ACS_Many_people_open=3,//多人开门  
		ECMS_ALARM_TYPE_ACS_Emergency_password_open=4,//紧急状态密码开门  
		ECMS_ALARM_TYPE_ACS_Open_at_normal_opening_time=5,//常开时间段开门  
		ECMS_ALARM_TYPE_ACS_Trigger_link_event=6,//触发联动事件  
		ECMS_ALARM_TYPE_ACS_Cancel_alarm=7,//取消报警  
		ECMS_ALARM_TYPE_ACS_Remote_open_door=8,//远程开门  
		ECMS_ALARM_TYPE_ACS_Remote_close_door=9,//远程关门  
		ECMS_ALARM_TYPE_ACS_Disable_normal_opening_time_of_the_day=10,//禁用当天常开时间段  
		ECMS_ALARM_TYPE_ACS_Enable_normal_opening_time_of_the_day=11,//启用当天常开时间段  
		ECMS_ALARM_TYPE_ACS_Remote_open_auxiliary_output=12,//远程打开辅助输出  
		ECMS_ALARM_TYPE_ACS_Remote_close_auxiliary_output=13,//远程关闭辅助输出  
		ECMS_ALARM_TYPE_ACS_Operate_interval_too_short=20,//操作间隔太短  
		ECMS_ALARM_TYPE_ACS_Open_door_verification_in_non_valid_time=21,//门非有效时间段验证开门  
		ECMS_ALARM_TYPE_ACS_Non_illegal_time=22,//非法时间段  
		ECMS_ALARM_TYPE_ACS_Illegal_access=23,//非法访问   
		ECMS_ALARM_TYPE_ACS_Anti_submarine=24,//反潜   
		ECMS_ALARM_TYPE_ACS_Interlock=25,//互锁   
		ECMS_ALARM_TYPE_ACS_Multi_person_authentication_waiting=26,//多人验证等待   
		ECMS_ALARM_TYPE_ACS_Unregistered_person=27,//人未登记   
		ECMS_ALARM_TYPE_ACS_Door_open_timeout=28,//门开超时   
		ECMS_ALARM_TYPE_ACS_Man_passed_term_of_validity=29,//人已过有效期   
		ECMS_ALARM_TYPE_ACS_Password_error=30,//密码错误   
		ECMS_ALARM_TYPE_ACS_Non_effective_time_interval_of_door=36,//门非有效时间段(按出门按钮)   
		ECMS_ALARM_TYPE_ACS_Normally_open_time_can_not_be_closed=37,//常开时间段无法关门   
		ECMS_ALARM_TYPE_ACS_Blacklist=39,//黑名单   
		ECMS_ALARM_TYPE_ACS_Validation_error=41,//验证方式错误   
		ECMS_ALARM_TYPE_ACS_Format_error=42,//韦根格式错误   
		ECMS_ALARM_TYPE_ACS_Background_validation_failed=44,//后台验证失败   
		ECMS_ALARM_TYPE_ACS_Background_validation_timeout=45,//后台验证超时   
		ECMS_ALARM_TYPE_ACS_Multi_person_validation_failed=48,//多人验证失败  
		ECMS_ALARM_TYPE_ACS_The_door_is_locked=63,//门已锁定   
		ECMS_ALARM_TYPE_ACS_Eexit_button_operate_not_in_effective_time=64,//出门按钮不在有效时间段内操作   
		ECMS_ALARM_TYPE_ACS_Auxiliary_input_operate_not_in_effective_time=65,//辅助输入不在有效时间段内操作   
		ECMS_ALARM_TYPE_ACS_Anti_demolition_alarm=100,//防拆报警   
		ECMS_ALARM_TYPE_ACS_Stress_open_door_alarm=101,//胁迫开门报警   
		ECMS_ALARM_TYPE_ACS_Door_accidentally_Opened=102,//门被意外打开   
		ECMS_ALARM_TYPE_ACS_Unable_connect_server=105,//无法连接服务器   
		ECMS_ALARM_TYPE_ACS_Power_off=106,//市电掉电   
		ECMS_ALARM_TYPE_ACS_Battery_power_failure=107,//电池掉电   
		ECMS_ALARM_TYPE_ACS_Door_is_open=200,//门已打开   
		ECMS_ALARM_TYPE_ACS_Door_is_close=201,//门已关闭   
		ECMS_ALARM_TYPE_ACS_Open_door_with_button=202,//出门按钮开门   
		ECMS_ALARM_TYPE_ACS_End_of_normally_open_time=204,//常开时间段结束   
		ECMS_ALARM_TYPE_ACS_Remote_open_door_normally_open=205,//远程开门常开   
		ECMS_ALARM_TYPE_ACS_Device_startup=206,//设备启动   
		ECMS_ALARM_TYPE_ACS_Super_users_open_door=208,//超级用户开门   
		ECMS_ALARM_TYPE_ACS_Trigger_button=209,//触发出门按钮(被锁定)    
		ECMS_ALARM_TYPE_ACS_Successful_connection_server=214,//成功连接服务器    
		ECMS_ALARM_TYPE_ACS_Auxiliary_input_point_disconnect=220,//辅助输入点断开    
		ECMS_ALARM_TYPE_ACS_Auxiliary_input_point_short_circuit=221,//辅助输入点短路    
		ECMS_ALARM_TYPE_ACS_Background_verification_successful=222,//后台验证成功    
		ECMS_ALARM_TYPE_ACS_Background_verification=223,//后台验证    
		ECMS_ALARM_TYPE_ACS_Ring_doorbell=224,//按门铃   
		ECMS_ALARM_TYPE_ACS_Auxiliary_output_timing_normally_open=229,//辅助输出定时常开    
		ECMS_ALARM_TYPE_ACS_Auxiliary_output_timing_closed_normally_open=230,//辅助输出定时关闭常开   
		ECMS_ALARM_TYPE_ACS_Verify_through=232,//验证通过    
		ECMS_ALARM_TYPE_ACS_Remote_locking=233,//远程锁定    
		ECMS_ALARM_TYPE_ACS_Remote_unlock=234,//远程解锁    
		ECMS_ALARM_TYPE_ACS_Trigger_global_linkage=300,//触发全局联动    
		ECMS_ALARM_TYPE_ACS_Global_antisubmarine_login=500,//全局反潜(逻辑)    
		ECMS_ALARM_TYPE_ACS_Global_interlock=503,//全局互锁   
		ECMS_ALARM_TYPE_ACS_Global_antisubmarine_timing=505,//全局反潜(定时)     
		ECMS_ALARM_TYPE_ACS_Global_antisubmarine_timing_logic=506,//全局反潜(定时逻辑)     
		ECMS_ALARM_TYPE_ACS_Background_verification_failed_by_notregistered=509,//后台验证失败(人未登记)     
		ECMS_ALARM_TYPE_ACS_Background_verification_failed_by_Data_anomaly=510,//后台验证失败(数据异常)     
		ECMS_ALARM_TYPE_ACS_Connection_disconnect=700,//连接断开  


	};


	//报警联动类型
	enum ALARM_TRIGGER_TYPE
	{
		TRIGGER_INVALID = 0,
		TRIGGER_CH_REC,///////////联动录像  dwTriggerInfo1标识通道号的ID
		TRIGGER_PREVIEW,///////////联动预览   dwTriggerInfo1表示联动预览的通道号ID	   弹出视频
		TRIGGER_AUDIO,/////联动声音的时候 dwTriggerInfo1  dwTriggerInfo2无意义		 蜂鸣器或 播放声音文件
		TRIGGER_ALARM_OUT,   ////联动报警输出
		TRIGGER_PTZ_RESET,///////////联动云台预置点  dwTriggerInfo1 表示联动云台的的节点ID，dwTriggerInfo2标识预置点号 
		TRIGGER_SNAP_PICTURE,///////////联动抓图  dwTriggerInfo1表示抓图的张数
		TRIGGER_SNAP_PICTURE_TO_FTP,///////////联动抓图到FTP  dwTriggerInfo1表示抓图的张数
		TRIGGER_EMAP,///////////联动电子地图 dwTriggerInfo1  dwTriggerInfo2无意义
		TRIGGER_TVWALL_PREVIEW,//////联动电视墙预览
		TRIGGER_EMAIL,
		TRIGGER_POPUP_MESSAGE,////弹出消息框
		TRIGGER_CH_REC_TO_FTP,///////////联动录像到FTP
		TRIGGER_DEV_AUDIO,///////////联动设备语音播报
		TRIGGER_ACS_DOOR,///////////联动开门
		TRIGGER_SOP,//联动sop
		ALARM_TRIGGER_SWITCH=0xFE ,//联动的开关，二进制表示,写成FE(254)是并不是限定死了，只是想让数据库中看起来大一点
		TRIGGER_END,
	};

	//报警联动开关
	typedef enum _trigger_switch
	{
		TRIGGER_SWITCH_INVALID	= 0x00,			//空
		TRIGGER_SWITCH_CH_REC	= 0x01,			//联动录像开关
		TRIGGER_SWITCH_PREVIEW = 0x02,		//联动预览开关
		TRIGGER_SWITCH_ALARM_OUT	= 0x04,			//联动报警输出开关
		TRIGGER_SWITCH_PTZ_RESET	= 0x08,			//联动云台预置点开关
		TRIGGER_SWITCH_TVWALL_PREVIEW = 0x10,		//联动联动电视墙开关
		TRIGGER_SWITCH_AUDIO = 0x20,		//联动声音开关
		TRIGGER_SWITCH_SNAPSHOT = 0x40,		//联动前端抓图开关
		TRIGGER_SWITCH_DEV_AUDIO = 0x80,		//联动前端语音播报开关
		TRIGGER_SWITCH_ACS_DOOR = 0x100,		//联动开门开关
		TRIGGER_SWITCH_EMAIL = 0x200,		//联动发邮件
		TRIGGER_SWITCH_SOP = 0x400,		//联动sop
		TRIGGER_SWITCH_ALL	= 0xFFFF,/////所有的声音开关
	}TRIGGER_SWITCH;

    typedef enum _smart_push
    {
        ECMS_PUSH_SMART_STATE,  // IPC返回的智能事件状态  SMART_EVENT_STATE
        ECMS_PUSH_VFD_DATA,     // IPC推送的人脸检测数据  SmartVFDData
		ECMS_PUSH_CPC_DATA,		// IPC推送的人数统计数据  SMART_CPC_DATA

        ECMS_PUSH_VFD_RULE,     // IPC推送人脸检测功能开关及检测区域， 无效，网页更改之后暂时不会推送

		ECMS_PUSH_NVR_FACE_MATCH_ALARM,	// 人脸比对NVR推送人脸匹配告警	FrNvrFaceMatchData
		ECMS_PUSH_VFD_MATCH_RESULT,		// IPC推送的人脸比对结果			FrIPCFaceMatchData

		ECMS_PUSH_MDU_SUB_RESULT,		// 转发服务器向智能分析服务器推送的订阅状态信息
		
        ECMS_PUSH_VEHICLE_INFO,			// 车牌IPC适配库 向 转发 推送车牌信息

        ECMS_PUSH_FEBRILE_PERSON_INFO,  // 推送疑似发热人员信息
		ECMS_PUSH_TRAJECT_RECT_INFO,//目标框信息

		ECMS_PUSH_PEA_TARGET,  // IPC推送的区域入侵和绊线 PEATarget
		ECMS_PUSH_PLATE_TARGET, // IPC推送的车牌 PlateTarget
		ECMS_PUSH_INDUSTRIA_FEBRILE_INFO,  // 推送工业测温信息

    }SMART_PUSH;

	// 转发服务向IPC 订阅事件结构体
	enum CEMS_TOPIC_TYPE {
		VFD_TOPIC = 0,	//人脸抓拍事件
		CPC_TOPIC = 1,	//人数统计事件
		VFD_CMP_TOPIC = 2,	//人脸比对事件
		VEHICLE_TOPIC = 3,	// 车牌识别订阅主题
		PASSLINECOUNT_TOPIC = 4,	// 过线统计，包括人/车/非机动车
		PEA_TOPIC = 5,	// 伴线跟入侵
		AOIENTRY_TOPIC = 6,	// 进入
		AOILEAVE_TOPIC = 7,	// 离开
		PRE_PERMITER_TOPIC = 8,	//区域入侵
		VFD_CMP_FAILED_TOPIC = 9,	//人脸比对失败事件
		INDUSTRIA_FEBRILE_TOPIC = 10,	// 工业测温订阅
	};

	typedef struct _ecms_smart_topic_subber {
		GUID		chnlGuid;		// 通道GUID
		tuint32		nTopicCnt;
		tuint32		szTopicType[32];	// 订阅事件集, @enum CEMS_TOPIC_TYPE
	}ECMS_SMART_TOPIC_SUBBER;

	// 订阅返回
	typedef struct _ecms_smart_topic_subber_ret {
		char szServerAddr[256];
	}ECMS_SMART_TOPIC_SUBBER_RET;

	// 退订
	typedef struct _ecms_smart_topic_unsubber {
		ECMS_SMART_TOPIC_SUBBER_RET	sSubAddr;
		ECMS_SMART_TOPIC_SUBBER		sSubInfo;
	}ECMS_SMART_TOPIC_UN_SUBBER;

	//////////////////////////////////////////////////////////////////////////
	//语音播报文件结构体
	struct st_EncodeDataInfo{
		char    type[4]; // tvt,hk,dh
		tuint32 subType; //G711A ...
		tuint32 subLen;  //每个音频包的长度
		tuint32 len;	//总长度
		tuint64 pos;	//数据位置
		st_EncodeDataInfo()
		{
			memset(type, 0, sizeof(type));
			subType = 0;
			subLen = 0;
			len = 0;
			pos = 0;
		}
		st_EncodeDataInfo(const char* szType, const tuint32 iSubType)
		{
			memcpy(type, szType, sizeof(type));
			subType = iSubType;
			subLen = 0;
			len = 0;
			pos = 0;
		}
	};

	enum CONFIG_INFO_TYPE
	{
		DEVICE_PASSWORD=1,//添加设备时的默认密码
		DEVICE_CALIBRATION_TIME=2,//设备校时Calibration time
		ALARM_FREQUENCY,     //同类型报警频率
		ALARM_IN_SCHEDULE,     //排程外的报警不显示
		VIEW_USE_THIRD_STREAM,   //联动预览使用第三码流(或子碼流)
		LOG_SAVE_DAYS,     //日志保存天数（如果为0  则默认750天）
		Modify_AREA_GBID,//开关：是否可以修改区域GBID 20200718将管理服务器配置文件中是否自定义国标配置扩展在这个xml中
		DEVICE_CALIBRATION_TIMEZONE, //设备校时是否同步时区
		PASSWORD_SECURITY_QUESTION,     //密码重置的密保问题保存
		TVTPAD_FEBRILE_USE_VFD,//同为热成像平板是采用哪种方式做测温0为人脸比对，1为人脸检测
		LOGO_TEXT,//客户端左上角的logo文字
		OFFLINE_TIME, //判断存储和转发是否是故障离线（用于离线热备）
        LANG_OPTION,                //13 阿拉伯语 和 默认0（显示正常阿拉伯数字） xml字段：languageOption   //对应枚举类型：LANG_OPTION_VALUE
        TEMP_VEHICLE_SCHEDULE,      //14 临时车辆的排程 tempVehicleSchedule  默认排程：关闭
        TEMP_PARKING_TIME_LIMIT,    //15 临时车辆的停车时间限制，超出这个时长，定时发送告警信息   默认：24h  单位：h  24  xml字段：tempParkingTimeLimit
		LOGIN_FAILED_LOCK,          //登录失败锁定
		TVWALL_ALARM_PREVIEW_ISCLOSE, //17报警联动上墙是否自动关闭。
	};

    enum LANG_OPTION_VALUE
    {
        LANG_OPTION_DEFAULT=0,      //默认0（显示正常阿拉伯数字）
        LANG_OPTION_ARABIC=1,
    };

	const char szEncrypKey[] = "Tvt123456QazWsx";   //固定加密key
	const char szEncryptHead[] = "###_";   //确认是否进行了数据加密(已被szEncryptHead_new代替，v2.1.1 200305及以后版本不再使用)
	const char szProtocol[] = "defineXY";//客制数据协议字段
	const char szP2PServer[] = "definePPS";//客制数据P2P服务器
	const char szP2PPort[] = "definePPP";//客制数据P2P端口
	const char szP2PServer_sub[] = "defineSubPPS";//客制数据P2P服务器
	const char szP2PPort_sub[] = "defineSubPPP";//客制数据P2P端口
	const char szP2PServer20[] = "definePPS20";//客制数据P2P2.0服务器
	const char szP2PPort20[] = "definePPP20";//客制数据P2P2.0端口
	const char szTVTManufacturer[] = "defineSCCS";//客户定制tvt设备的生产厂商，非定制tvt设备显示onvif
	const char szTVTManufacturer_nvr[] = "defineNVRSCCS";//客户定制tvt设备的生产厂商，非定制tvt设备显示onvif,针对nvr
	
	const char szDefaultAdminPassword[] = "defineMRMM";//客制数据第一次登录时默认密码
	const char szDefaultAdminName[] = "defineMRYHM";//客制数据第一次登录时默认用户名

	const char szStandardName[] = "defineBZXYYHM";//接入标准协议用户名
	const char szHKName[] = "defineHKXYYHM";//接入Hikvision协议用户名
	const char szDHName[] = "defineDHXYYHM";//接入Dahua协议用户名
	const char szOnvifName[] = "defineONVIFXYYHM";//接入ONVIF协议用户名
	const char szHuaAnName[] = "defineHAXYYHM";//接入HuaAn协议用户名
	const char szATMName[] = "defineATMXYYHM";//接入TVT-ATM协议用户名
	
	const char szStandardPwd[] = "defineBZXYMM";//接入标准协议密码
	const char szHKPwd[] = "defineHKXYMM";//接入Hikvision协议密码
	const char szDHPwd[] = "defineDHXYMM";//接入Dahua协议密码
	const char szOnvifPwd[] = "defineONVIFXYMM";//接入ONVIF协议密码
	const char szHuaAnPwd[] = "defineHAXYMM";//接入HuaAn协议密码
	const char szATMPwd[] = "defineATMXYMM";//接入TVT-ATM协议密码
	const char szMCU_MaxCount[] = "defineMCUJRS";//客户端最大接入数
	const char szCustomFile[] = "Product.dat";

	const char szMD5EncryptHead[] = "***_";   //确认是否加密为MD5
	const char szEncryptHead_new[] = "@@@_";   //确认是否进行了数据加密
	const char szEncryptHead_PlanBk[] = "PLAN_";   //确认是否是每日定时备份数据进行了数据加密

    const char szMYSQLPassword[] = "BpHHaA4QghCJsJgs";   //mysql数据库root用户的密码，用于linux，windows可以在安装mysql的时候改动，因为初始密码过于简单，容易被黑客攻击

	const char szRSUServerUserName[] = "Management Server" ;//管理服务用户名

	const char szGB28181AreaNo[] = "216";//国标28181中区域的类型编码
	const char szGB28181DevNo[] = "113";//国标28181中设备的类型编码
	const char szGB28181ChnNo[] = "131";//国标28181中通道的类型编码
	const char szGAT1400AreaNo[] = "216";//国标1400中区域的类型编码
	const char szGAT1400DevNo[] = "113";//国标1400中设备的类型编码
	const char szGAT1400ChnNo[] = "119";//国标1400中通道的类型编码
	enum EM_CUSTOM_FIELD
	{
		ECMS_CUSTOM_FIELD1 = 1,
		ECMS_CUSTOM_FIELD2,
		ECMS_CUSTOM_FIELD3,
		ECMS_CUSTOM_FIELD4,
		ECMS_CUSTOM_FIELD5,
		ECMS_CUSTOM_FIELD6,
		ECMS_CUSTOM_FIELD7,
		ECMS_CUSTOM_FIELD8,
		ECMS_CUSTOM_FIELD9,
		ECMS_CUSTOM_FIELD10,
	};

	enum CHN_CROSSLINE_TYPE
	{
		CHN_CROSSLINE_TYPE_NOCONFIG = 0,//未配置
		CHN_CROSSLINE_TYPE_ENTER,//进入
		CHN_CROSSLINE_TYPE_EXIT,//离开
		CHN_CROSSLINE_TYPE_NONEED,//支持过线统计的ipc不需要配置
	};

	//编辑监控点信息枚举
	enum EDIT_CHANNEL_INFO_TYPE
	{
		EDITCHN_KEYBOARD_INDEX = 0,
		EDITCHN_FACE_CAPTURE_SOURCE = 1,
		EDITCHN_FACE_ATTEND_TYPE = 2,
		EDITCHN_UPPERGBID = 3,
		EDITCHN_ISENABLE_CROSSLINE = 4,
		EDITCHN_INTELLIGENT_SCENE = 5,
	};

    /// TLV结构信息，用于组帧时存储人脸跟踪框和温度等信息
    //TLV头结构
    typedef struct _tag_tlv_buffer_description
    {
        //        tuint32 dwFlag;     //TLV头标志 TvtTlvHEADERFLAG
        tuint8  byType;     //数据类型，由应用方定义，见定义 ADDITIONAL_DATA_TYPE
        tuint8  byVersion;  //数据版本，由应用方定义，不同的数据类型有各自的数据版本
        tuint16 usNumber;   //元素数量，最多655355个
        tuint32 dwSize;     //单个元素的大小，总长度=usNumber*dwSize

    }TLV_BUFFER_DESC;

};//////end   namespace NVMS_DEFINE

#pragma pack()

#endif //__NVMS_DEFINE_
