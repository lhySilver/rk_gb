/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author        ： zgj
** Date           ：2016-1-18
** Name         ：
** Version       ：2.0
** Description ： DVR/NVR SDK接口定义
** Modify Record:
***************************************************************************************************
修改内容                                                     修改日期              版本                 修改人                                 备注
1、增加对DVR支持接口                                2016-1-18           V2.0                 zgj       
2、接口增加说明和分类
3、原NVR接口有部分函数名称变更
4、合并了1个接口到初始化函数
NVR详细接口变更内容如下：
VAC_ClearFrameBufferArea  去掉。 VAC_InputData改为VAC_InputAudioData。VAC_CapPlaybackJpeg变更为VAC_CaptureDisplayToJpeg
VAC_SetAudioFmt函数去掉，参数合并到VAC_InitialDevice。VAC_SetFrameBufAlpha接口去掉，VAC_SetHdmiAdaptive函数名称改为 VAC_SetOutputChangeCBFun

*****************************************************************************************************/
#ifndef  __TVT_SDK_H__
#define __TVT_SDK_H__

#ifdef __cplusplus
#if __cplusplus
#define EXTERN_C  extern "C"
#else
#define EXTERN_C  extern
#endif
#else
#define EXTERN_C  extern
#endif

/************************************************************************/
/*            常量      宏定义   枚举值     结构体                    BEGIN          */
/************************************************************************/
#define DRV_NOT_SUPPORT -100

typedef enum _VIDEO_STANDARD_
{
	VAC_NTSC = 0,
	VAC_PAL
}VIDEO_STANDARD;

typedef enum _AUDIO_FORMAT
{
	AUDIO_FORMAT_PCM,   //标准PCM可以直接播放数据
	AUDIO_FORMAT_ALAW,  //标准G711_A
	AUDIO_FORMAT_HI_ADPCM,//海斯ADPCM格式
	AUDIO_FORMAT_HI_ALAW,//海斯G711_A格式
	AUDIO_FORMAT_HK711,//海康711声音格式
	AUDIO_FORMAT_HK722,//海康722声音格式
	AUDIO_FORMAT_ULAW,  //标准G711_U
	AUDIO_FORMAT_HI_ULAW,   //海斯G711_U
    AUDIO_FORMAT_MP3,   //mp3
}AUDIO_FORMAT;

typedef enum _LED_TYPE_
{
    LED_BACKUP,
    LED_NET,
    LED_PLAY,
    LED_REC,
    LED_HDD,
    LED_TXRX,  //  rs485的方向键1 : 发送 0 : 接收
    LED_VIDEOLOSS,

	// 针对N128E的面板
	LED_LAN1,
	LED_LAN2,
	LED_ALARM,
	LED_RUN,

    LED_TYPE_BUTT
} LED_TYPE;

typedef enum _ALARM_IN_TYPE_
{
    ALARM_IN_UNVALID,
    ALARM_IN_VALID
}ALARM_IN_TYPE;

enum TVT_DRIVER_SUPPORT_RESOLUTION
{
	TVT_DRIVER_1920_1080=0x01,/////1080P
    TVT_DRIVER_3840_2160_P30=0x02,/////4K p30
	TVT_DRIVER_1280_1024=0x04,/////1280x1024
    TVT_DRIVER_1024_768=0x08,/////1024*768
	TVT_DRIVER_2560_1440=0x10,////2560*1440
    TVT_DRIVER_3840_2160_P60=0x20,/////4K p60
	TVT_DRIVER_SUPPORT_RESOLUTION_RANGE_END   
};

enum TVT_DRIVER_STATUS_CHANGE
{
	TVT_DRIVER_STATUS_CHANGE_RANGE_BEGIN,
	TVT_DRIVER_STATUS_VIDEO_MOTION=0x01,/////移动侦测
	TVT_DRIVER_STATUS_VIDEO_SHELTER=0x02,/////遮挡报警
	TVT_DRIVER_STATUS_VIDEO_TPYE=0x03,/////视频信号检测
	TVT_DRIVER_STATUS_ALARM=0x04,/////报警
    TVT_DRIVER_STATUS_SUPPORT_PIR=0x05,/////是否支持PIR， 1：支持， 0：不支持
    TVT_DRIVER_STATUS_PIR_ALARM=0x06,/////PIR是否有报警，1：有，0：无
	TVT_DRIVER_STATUS_CHANGE_RANGE_END   
};

typedef enum _IP_FRM_REF_TYPE
{
    I_SLICE = 0,                              /*the Idr frame at Base layer*/
	BP_SLICE_REFBYBASE,                          /*the P frame at Base layer, referenced by other frames at Base layer*/
    BP_SLICE_REFBYENHANCE,                       /*the P frame at Base layer, referenced by other frames at Enhance layer*/
    EP_SLICE_REFBYENHANCE,               		/*the P frame at Enhance layer, referenced by other frames at Enhance layer*/
    EP_SLICE_NOTFORREF,                       /*the P frame at Enhance layer ,not referenced*/
    P_SLICE_BUTT
} IP_FRM_REF_TYPE;

typedef enum _FISHEYE_MOUNT_DEVICE_TYPE
{
	FISHEYE_MOUNT_DESKTOP,
	FISHEYE_MOUNT_CEILING,
	FISHEYE_MOUNT_WALL,
	FISHEYE_MOUNT_BUTT
}FISHEYE_MOUNT_DEVICE_TYPE;

typedef enum _FISHEYE_ADJUST_MODE
{
	FISHEYE_ADJUST_MODE_180,
	FISHEYE_ADJUST_MODE_360,
	FISHEYE_ADJUST_MODE_NORMAL,
	FISHEYE_ADJUST_BYPASS,
	FISHEYE_ADJUST_MODE_MULTI_NORMAL,
	FISHEYE_ADJUST_MODE_MULTI_180,
	FISHEYE_ADJUST_MODE_MULTI_360,
    FISHEYE_ADJUST_MODE_MULTI_4PTZ,
    FISHEYE_ADJUST_MODE_MULTI_8PTZ,
	FISHEYE_ADJUST_MODE_BUTT
}FISHEYE_ADJUST_MODE;

typedef struct
{
    int mode;       //FISHEYE_ADJUST_MODE; 现在只有 180,360，normal，multi
    int horizon;    //中心点的水平位移，取值范围【0，255】，默认值128，小于128表示往左移，大于128表示往右移
    int vertical;   //中心点的垂直位移，取值范围【0，255】，默认值128，小于128表示往下移，大于128表示往上移
    int zoomh;      //水平放大倍数，取值范围【1，255】，默认值255，小于255表示放大
    int zoomv;      //垂直放大倍数，取值范围【1，255】，默认值255，小于255表示放大
    int inr;        //取值范围【1，255】，默认值128  现在暂时不使用
    int outr;       //取值范围【1，255】，默认值128  现在暂时不使用
}FISHEYE_PARA;


typedef union _vac_frame_extension_info
{
    struct _video_frame_ext_Info
    {
        unsigned char       byVersion;//////////////扩展信息版本
        unsigned char       byVideoFormat;/////视频制式  0 PAL   1为N制
        unsigned char       byIsKeyFrame;
        unsigned char       wReserver;///////////
        tuint32             dwVideoEncodeType;//////////////视频编码类型
        tuint16             swVideoWidth;//////////////视频宽度
        tuint16             swVideoHeight;/////////////高度
        tuint32             dwValidFrameLen;//////////////一帧数据长度，不包括帧头
    }video_frame_ext_Info;

    struct _audio_frame_ext_info
    {
        unsigned char       byVersion;//////////////扩展信息版本
        unsigned char       byChannels;//////////////声道数量
        tuint16             wAudioEncodeType;//////////////音频编码类型
        tuint16             wBitsPerSample; ////
        tuint16             wAvgBytesPerSec;//////采样平均位数
        tuint32             dwSamplesPerSec;//////////////采样率
        tuint32             dwValidFrameLen;//////////////一帧数据长度，不包括帧头
    }audio_frame_ext_info;

	struct
	{
		struct _video_frame_ext_Info     videoframeextinfo;
		IP_FRM_REF_TYPE refFrmType;
		tuint32             tm_seconds;
		tuint32             tm_microsecond;
	}venc_frame_Info;

	struct
	{
		struct _audio_frame_ext_info    audioframeextInfo;
		tuint32             tm_seconds;
		tuint32             tm_microsecond;
	}aenc_frame_info;

	struct
	{
		unsigned char       byVersion;//////////////扩展信息版本
		tuint16             swVideoWidth;//////////////视频宽度
		tuint16             swVideoHeight;/////////////高度
		tuint32             channel;
		tuint32             tm_seconds;
		tuint32             tm_microsecond;
		tuint32             dwValidFrameLen;
	}jpeg_frame_info;

    struct 
    {
    	 tuint32 resv[19];
    }resv_ext_info;
}VAC_FRAME_EXTENSION_INFO, *LPVAC_FRAME_EXTENSION_INFO;

typedef struct _video_osd_attrib
{
	tuint16	xPos;		//（1/10000）X坐标
	tuint16	yPos;		//（1/10000）Y坐标
	tuint16	cx;			//（1/10000）宽度
	tuint16	cy;			//（1/10000）高度
	unsigned char	*pData;		//数据
	tuint32	length;		//数据长度
	tuint32	FgAlpha;		///* foreground transparence */  for logo 透明度
}VIDEO_OSD_ATTRIB;

typedef struct _video_area_attrib
{
	unsigned short recv;
	unsigned short enable;	//是否有效

	unsigned short x;		//横坐标(1/10000)
	unsigned short y;		//纵坐标(1/10000)

	unsigned short cx;		//宽度(宽度和高度为零，表示无效）(1/10000)
	unsigned short cy;		//高度(宽度和高度为零，表示无效）(1/10000)
}VIDEO_AREA_ATTRIB;

//////////////AP  在使用这个结构体的时候 要根据最终情况确定， 640个字节太长了 
typedef struct _vac_motion_area_info
{
	unsigned short gridWidth;	//每行有多少个栈格
	unsigned short gridHeight;	//每列有多少个栈格
	unsigned char  pData[128];	//每个占格用一位表示
}VAC_MOTION_AREA_INFO;

typedef struct vac_video_encode_stream_attr
{
	int nResWidth;
	int nResHeight;
	int nCodeStream; // 码率，单位kbps
	int nCodeRateType; // 帧率
}VAC_VIDEO_ENCODE_STREAM_ATTR;

typedef struct vac_audio_encode_stream_attr
{
    AUDIO_FORMAT dwAudioEnFormat;
    tuint16 wChannels;
    tuint32 nSamplesPerSec;
    tuint32 nAvgBytesPerSec;
    tuint16 wBitsPerSample;
}VAC_AUDIO_ENCODE_STREAM_ATTR;

typedef struct _vac_keyframe_infomation
{
	int type;
	int value;
	int xpos;
	int ypos;
	int isDown;
}VAC_KEYFRAME_INFOMATION;

typedef struct
{
    int x;
    int y;
    int width;
    int height;
} VAC_VO_COND;


typedef struct _video_encode_param
{
	tuint32 width;			//分辨率
	tuint32 height;
	tuint32 rate;				//帧率(0~30)
	tuint32 encodeType;			//编码类型(0:CBR,1:VBR)
	tuint32 quality;			//画质(1~6),值越大，画质越好

	tuint32 minBitrate;			//码流下限（kbps）
	tuint32 maxBitrate;			//码流上限（kbps）

	tuint32 GOP;				//I帧间隔 
}VIDEO_ENCODE_PARAM;

////////////////////////////////////////////////////
typedef struct _chn_stream_caps
{
	tuint32	dwStreamIndex;	//第几路码流  下标从0开始， 0表示第一码流（最大）
	tuint32 	image_size;	
	char			strResolutionName[16];//分辨率名字
	tuint32 	image_width;
	tuint32 	image_height;
	tuint32	dwRate;			//支持帧率最大值
	char         bCBR;	/////支持CBR为true，不支持为False
	char         bVBR; ////支持VBR为true，不支持为false
	char		 videoLossState;		//0:no video loss; 1:video loss
	char 	      szRevser1;	
	tuint32 	supportDataBitRate[20];//支持的码率
	tuint32 	qualityForVBR;//VBR支持画质等级
	tuint32  supportEncodeType;////SDK_DECODE_CAP_H264  SDK_DECODE_CAP_H265
}CHN_STREAM_CAPS;

//////////////////////////////////////////////////////////////////////////
typedef enum _COAXIAL_TYPE_
{
	COAXIAL_TYPE_RANGE_BEGIN,
	COAXIAL_TYPE_NONE			= 0,
	COAXIAL_TYPE_TVI			= 0x01,
	COAXIAL_TYPE_AHD_960H		= 0x02,
	COAXIAL_TYPE_AHD_720P		= 0x04,
	COAXIAL_TYPE_AHD_1080P		= 0x08,
	COAXIAL_TYPE_CVI			= 0x10,
	COAXIAL_TYPE_RANGE_END,
}COAXIAL_TYPE;

/////////////////////////////////////////////////////
typedef enum _signal_type_
{
	SIGNAL_TYPE_RANGE_BEGIN,
	SIGNAL_TYPE_NONE = 0,
	SIGNAL_TYPE_AHD = 0x01,
	SIGNAL_TYPE_TVI = 0x02,
	SIGNAL_TYPE_AUTO = 0x04,
    SIGNAL_TYPE_CVI = 0x08,
	SIGNAL_TYPE_RANGE_END=0xFFFFFFFF,
}SIGNAL_TYPE;

/////////////////////////////////////////////////////
typedef struct _chn_caps
{
	int      nSupportSignalType;			//SIGNAL_TYPE
	int      nSupportCoaxialType;			//COAXIAL_TYPE
	char 	      szRevser[24];
}CHN_CAPS;

//////////////////////////////////////////////////////////////////////////
typedef enum corridor_mode
{
	corridor_mode_off = 0,
	corridor_mode_on,
	corridor_mode_end,
}CORRIDOR_MODE;

//////////////////////////////////////////////////////////////////////////
typedef struct _corridor_mode_param_
{
	unsigned char  nChipIndex;						
	unsigned char  nChannelIndex;						
	CORRIDOR_MODE  corridor_mode;					
}CORRIDOR_MODE_PARAM;

#define MISC_FLAS_IS_NVA 0x35EDC
#define MISC_FLAS_IS_NVN 0x3564B

// VAC_GetSDKParam   VAC_SetSDKParam
typedef enum _SDK_PARAM_
{
	VAC_SDK_PARAM_NONE = 0,
	VAC_GET_SDK_PARAM_DECODE_CAP,					// int, SDK_DECODE_CAP_H264....
	VAC_GET_SDK_PARAM_UI_SCALE_STATUS,				// int, 0 or 1
	VAC_GET_SDK_PARAM_VO_COND,						// VAC_VO_COND
	VAC_GET_SDK_PARAM_SIGNAL_TYPE,					//获取产品支持的信号类型，AHD + TVI
	VAC_GET_SDK_PARAM_BEGIN_ENCODE_INDEX,			//编码通道偏移索引值
	VAC_GET_SDK_PARAM_SUPPORT_EQ,					//是否支持EQ, 0不支持, 1支持
	VAC_GET_SDK_PARAM_PIP_SUPPORT,					//获取是否支持PIP
	VAC_GET_SDK_PARAM_3GNET_SUPPORT,				//获取是否支持3G/4G
	VAC_GET_SDK_PARAM_PSE_MOD_SUPPORT,				//电源管理模式支持情况，0x0-只支持自动(不支持界面设置)，0x1 -支持自动和手动(支持界面设置)
	VAC_GET_SDK_PARAM_GPIO_BORAD_VER,				//BORAD_VER:0-3
	VAC_GET_SDK_PARAM_PRODUCT_TYPE_A,				//是否-A机型: 0否(8路音频), 1是(16路音频)


	VAC_GET_SDK_PARAM_DVR_START = 60,				//0-60为DVR和NVR 公共部分，60-79为DVR特有段


	VAC_GET_SDK_PARAM_NVR_START = 80,				//0-60为DVR和NVR 公共部分，80-99为NVR特有段
	VAC_GET_SDK_NVA_PARAM,							//TOE


	VAC_SET_SDK_PARAM_BEGIN_ENCODE_INDEX = 100,
	VAC_SET_SDK_PARAM_LITE_MODE,      					//设置Lite模式，支持同时设置多个通道     LITE_MODE_PARAM指针数组，第二个参数为项数量
	VAC_SET_SDK_PARAM_CHANNEL_SIGNAL_TYPE,			//设置通道组信号类型，支持同时设置多个通道组，  CHANNEL_GROUP_SIGNAL_TYPE指针数组，第二个参数为项数量
	VAC_SET_SDK_PARAM_PRINT_LEVEL,					//设置打印等级0-3 ERROR-DEBUG	4 QUIET屏蔽驱动打印 
	VAC_SET_SDK_PARAM_ANALOG_TO_IP,					//设置模拟IP切换
	VAC_SET_SDK_PARAM_CORRIDOR_MODE,				//设置走廊模式
	VAC_SET_SDK_PARAM_DECODE_NUM,					//设置解码器个数
	VAC_SET_SDK_PARAM_KEYBOARD_TYPE,				//设置面板类型:光面板和普通面板
	VAC_SET_SDK_PARAM_BOND_MODE,					//加载卸载bonging驱动模块 加载:1  卸载:0
	VAC_SET_SDK_PARAM_4M_SUPPORT,					//设置-C 产品是否支持4M  1:支持4m 不支持pip  0:不支持4m，支持pip
	VAC_SET_SDK_PARAM_SAFE_CODE,					//传递安全码，struct SAFE_CODE_OPS
	VAC_SET_SDK_PARAM_UART,							//加密或者解密串口, 0:encrypt;other:decrypt

	VAC_SET_SDK_PARAM_DVR_START = 200,				//100-199为DVR和NVR 公共部分，200-299为DVR特有段


	VAC_SET_SDK_PARAM_NVR_START = 300,				//100-199为DVR和NVR 公共部分，300-399为NVR特有段
	VAC_SET_SDK_NVA_PARAM,							//TOE

	VAC_SDK_PARAM_RANGE_END,
}SDK_PARAM;

typedef struct tvt_safe_code_ops
{
    char code[32];
    int len;
}SAFE_CODE_OPS;

typedef struct _lite_mode_param_
{
	unsigned char  byChnIndex;/////通道索引，下标按照统一个规定开始
	unsigned char  bySwitch;////// true表示设置为lite模式，false表示关闭lite模式
	unsigned char  byReserve[2];///保留
}LITE_MODE_PARAM;

typedef struct _channel_signal_type_
{
	int   nSignalType;///////
	unsigned char  szChnIndex;///通道索引，下标按照统一约定
	unsigned char  reserve[3];///保留
}CHANNEL_SIGNAL_TYPE;

typedef enum _SDK_DECODE_TYPE_
{
	SDK_DECODE_NONE		= 0,
	SDK_DECODE_CAP_H264	=0x01,
	SDK_DECODE_CAP_H265	=0x02,
	SDK_DECODE_BUTT
}SDK_DECODE_TYPE;

typedef enum _SDK_ENCODE_TYPE_
{
	SDK_ENCODE_NONE	= 0,
	SDK_ENCODE_H264	=0x01,
	SDK_ENCODE_H265	=0x02,
	SDK_ENCODE_BUTT
}SDK_ENCODE_TYPE;

#define VAC_VIDEO_ENCODE_TYPE_H264     0
#define VAC_VIDEO_ENCODE_TYPE_MGP4    1
#define VAC_VIDEO_ENCODE_TYPE_H265    3
#define VAC_VIDEO_ENCODE_TYPE_MJPG    4

//信号类型定义,用设置信号色彩默认值
typedef enum _video_type_def_color_
{
	VIDEO_TYPE_DEF_COLOR_BEGIN,
	VIDEO_TYPE_DEF_COLOR_AHD_720P25 = 0x01,
	VIDEO_TYPE_DEF_COLOR_AHD_720P30 = 0x02,
	VIDEO_TYPE_DEF_COLOR_AHD_1080P25 = 0x03,
	VIDEO_TYPE_DEF_COLOR_AHD_1080P30 = 0x04,
	VIDEO_TYPE_DEF_COLOR_AHD_3M = 0x05,
	VIDEO_TYPE_DEF_COLOR_AHD_5M = 0x06,	
	VIDEO_TYPE_DEF_COLOR_TVI_720P25 = 0x07,
	VIDEO_TYPE_DEF_COLOR_TVI_720P30 = 0x08,
	VIDEO_TYPE_DEF_COLOR_TVI_1080P25 = 0x09,
	VIDEO_TYPE_DEF_COLOR_TVI_1080P30 = 0x0a,	
	VIDEO_TYPE_DEF_COLOR_TVI_3M = 0x0b,
	VIDEO_TYPE_DEF_COLOR_TVI_5M = 0x0c,
	VIDEO_TYPE_DEF_COLOR_CVBS_PAL = 0x0d,
	VIDEO_TYPE_DEF_COLOR_CVBS_NTSC = 0x0e,
	VIDEO_TYPE_DEF_COLOR_END,
}VIDEO_TYPE_DEF_COLOR;

typedef struct _live_display_attrib
{
	unsigned int chnnIdx;	//通道索引号
	unsigned int nX;	//左上角横坐标
	unsigned int nY;	//左上角纵坐标
	unsigned int nWidth;		//显示区域宽度（像素）
	unsigned int nHeight;		//显示区域高度（像素）
	unsigned int nZIndex;		//显示优先级
}LIVE_DISPLAY_ATTRIB;


//POE  IP808  START

typedef enum _POE_STATE
{
	POE_NONE = 0,
	POE_AF,
	POE_AT,
	POE_BUTT
}POE_STA;

typedef struct _GET_POE_STATUS
{
	unsigned int num;   
	float poe_power[16];  // 功率值 浮点型
	POE_STA poe_status[16];
}GET_POE_STATUS;


typedef struct _set_poe_status
{
	unsigned int chn;  // 通道号
	POE_STA status;    // 开关状态
}set_poe_status;


typedef struct _ip808_set_poe
{
	unsigned int num;    //要设置总路数
	set_poe_status set_status[16];
}SET_POE_STATUS;

//POE  IP808  END


/***********************************************************************
CIPHER相关数据结构 begin
***********************************************************************/

#define CIPHER_MAX_NUM	8

typedef enum tvt_CIPHER_ALG_E
{
	TVT_CIPHER_ALG_DES			= 0x0,	/**< Data encryption standard (DES) algorithm */	 /**< CNcomment: DES算法 */
	TVT_CIPHER_ALG_3DES			= 0x1,	/**< 3DES algorithm */								 /**< CNcomment: 3DES算法 */
	TVT_CIPHER_ALG_AES			= 0x2,	/**< Advanced encryption standard (AES) algorithm */ /**< CNcomment: AES算法 */
	TVT_CIPHER_ALG_BUTT			= 0x3
}TVT_CIPHER_ALG_E;

/** Key length */
/** CNcomment: 密钥长度 */
typedef enum tvt_CIPHER_KEY_LENGTH_E
{
	TVT_CIPHER_KEY_AES_128BIT	= 0x0,	/**< 128-bit key for the AES algorithm */ /**< CNcomment:AES运算方式下采用128bit密钥长度 */
	TVT_CIPHER_KEY_AES_192BIT	= 0x1,	/**< 192-bit key for the AES algorithm */ /**< CNcomment:AES运算方式下采用192bit密钥长度 */
	TVT_CIPHER_KEY_AES_256BIT	= 0x2,	/**< 256-bit key for the AES algorithm */ /**< CNcomment:AES运算方式下采用256bit密钥长度 */
	TVT_CIPHER_KEY_DES_3KEY		= 0x2,	/**< Three keys for the DES algorithm */  /**< CNcomment:DES运算方式下采用3个key */
	TVT_CIPHER_KEY_DES_2KEY		= 0x3,	/**< Two keys for the DES algorithm */	  /**< CNcomment: DES运算方式下采用2个key  */
}TVT_CIPHER_KEY_LENGTH_E;

/** Cipher bit width */
/** CNcomment: 加密位宽 */
typedef enum tvt_CIPHER_BIT_WIDTH_E
{
	TVT_CIPHER_BIT_WIDTH_64BIT	= 0x0,	/**< 64-bit width */   /**< CNcomment:64bit位宽 */
	TVT_CIPHER_BIT_WIDTH_8BIT	= 0x1,	/**< 8-bit width */    /**< CNcomment:8bit位宽 */
	TVT_CIPHER_BIT_WIDTH_1BIT	= 0x2,	/**< 1-bit width */    /**< CNcomment:1bit位宽 */
	TVT_CIPHER_BIT_WIDTH_128BIT	= 0x3,	/**< 128-bit width */  /**< CNcomment:128bit位宽 */
}TVT_CIPHER_BIT_WIDTH_E;

/** Cipher control parameters */
/** CNcomment:加密控制参数变更标志 */
typedef struct tvt_CIPHER_CTRL_CHANGE_FLAG_S
{
	unsigned int	 bit1IV:1;				/**< Initial Vector change or not */ /**< CNcomment:向量变更 */
	unsigned int	 bitsResv:31;			/**< Reserved */					 /**< CNcomment:保留 */
}TVT_CIPHER_CTRL_CHANGE_FLAG_S;

/** Structure of the cipher control information */
/** CNcomment:加密控制信息结构 */
typedef struct tvt_CIPHER_CTRL_S
{
	unsigned int u32Key[8];								/**< Key input */																									  /**< CNcomment:输入密钥 */
	unsigned int u32Reserve0[4];								/**< Initialization vector (IV) */																					  /**< CNcomment:初始向量 */
	TVT_CIPHER_ALG_E enAlg;						/**< Cipher algorithm */																							  /**< CNcomment:加密算法 */
	TVT_CIPHER_BIT_WIDTH_E enBitWidth;			/**< Bit width for encryption or decryption */																		  /**< CNcomment:加密或解密的位宽 */
	TVT_CIPHER_KEY_LENGTH_E enKeyLen;			/**< Key length */																									  /**< CNcomment:密钥长度 */
	unsigned int u32Reserve1; /**< control information exchange choices, we default all woulde be change except they have been in the choices */	  /**< CNcomment:控制信息变更选项，选项中没有标识的项默认全部变更 */
} TVT_CIPHER_CTRL_S;

/** Cipher data */
/** CNcomment:加解密数据 */
typedef struct tvt_CIPHER_DATA_S
{
    unsigned char *pu8SrcAddr;     /*源数据地址*/
    unsigned char *pu8DstAddr;    /*目标数据地址*/
    unsigned int u32ByteLength;     /*数据长度*/
} TVT_CIPHER_DATA_S;

/***********************************************************************
CIPHER相关数据结构 end
***********************************************************************/

typedef struct _tag_tvt_decode_engine
{
	unsigned short usWidth;			//宽最大值
	unsigned short usHeight;		//高最大值
	unsigned short usIndex;			//索引
	char szReserve[2];				//对齐保留位
}TVT_DECODE_ENGINE;


/////////////////////////////////uboot参数区  分为2段，第二段分为2部分（共享部分和AP部分）///////////////////////////////////
#define BLOCK_SIZE 32
#define EXT_FLASH_PARAM_AP_WEBLOGO_OFFSET	(0*BLOCK_SIZE)      /////////jpg -- 100*1024 size
#define EXT_FLASH_PARAM_AP_IPPROTOCOL_OFFSET	(3871*BLOCK_SIZE)      /////////xml -- 1024 size
#define EXT_FLASH_PARAM_AP1_OFFSET				(3903*BLOCK_SIZE)      /////////AP_PARAM_FIRST
#define EXT_FLASH_PARAM_AP2_OFFSET				(4031*BLOCK_SIZE)    ////////AP_PARAM_SECOND
#define EXT_FLASH_PARAM_AP_DRIVE_SHARED_OFFSET				(4047*BLOCK_SIZE)   ///////AP_PARAM_SECOND

/////////////////////////////////uboot中 第一段参数区AP信息/////////////////////////////////////////
typedef struct _ap_param_first
{
	tuint32 	    dwflag;		    //标志             生产测试软件写入
	tuint16  	wSize;		    //本结构体大小           生产测试软件写入
	tuint8       typeflag;       //是N9000还是生产测试软件     0表示N9000， 1表示生产测试软件，  首次生产测试软件写入           
	tuint8       betaflag;       //是否beta版本
	tuint8		uistyle[2];		//用户界面类型     1A  1B  1C   2A   2B   2C   常规效验项
	tuint8		pftype[4];	    //平台类型（包标识）常规效验项            生产测试软件写入
	tuint16		curcfgid;	 	    //配置文件版本号常规效验项
	tuint32		swid;		    ////软件版本号     大版本号在高位字节，  版本号的每一段一个字节
	tuint8       byReserveExceptionFlag;/////后备异常标识，当配置分区不可用时会使用此标识记录尝试次数，达到三次会对提示出厂并格式化配置分区
	tuint32     dwUpgradeTime; /////写升级日志时使用，仅限于常规升级，升级写入时间，启动写入日志后清0
	tuint16	    mincfgid;//开始兼容配置文件版本号常规效验项

	tuint8       packageContentFlag;  //包内容标识（必须效验项），UI标准包为0，客户特定OCX或语言定义为非标准包取值为1，标准包升级标准包，非标包升级非标包
    tuint8      dataEncryptVer; //0 不加密,1 aes 加密
    tuint8      reserve1[3];
    tuint8      HuaAnLicenseCode[32]; // face lib license code --- 24 bytes available
    tuint8      dataEncryptRWKey[16]; //本设备数据加、解密密码
    tuint8      dataEncryptRKey[16];  //其它设备磁盘数据解码密码
    tuint8      reserve2[32*4];//保留, 便于后续的扩展
} AP_PARAM_FIRST;

/////////////////////////////////uboot中 第二段参数区中AP参数信息/////////////////////////////////////////
typedef struct _ap_param_second
{
	unsigned char     byTimezone[32];		    //时区
	unsigned char     byModel[32];		    //原生产品型号产品型号如 TD-2708TS-PR，名字必须完全匹配
	unsigned char     byCustomModel[32];////客户定制产品型号
	tuint32                dwCustomID;//客户编码
	unsigned char     byExFactoryFlag; ////出厂标记，      1表示出厂， 其他表示未出厂
	unsigned char     byReserve1[3];       //保留4字节对齐
	tuint32          dwCustomType;    //客户类型 202:海外中性 208:国内中性
	tuint32          dwNewCustomID;     //客户编码
	unsigned char     byReserve3[8];       //保留
	unsigned char     byFactoryLanguage[8];//出厂语言
	unsigned char     bySN[16];//序列号
	unsigned char     byReserve2[16];//保留
	unsigned char     byProduceInfo[16];//生产日期和当日编号
	unsigned char     byPCBAVersionInfo[16];//PCBA版本信息
} AP_PARAM_SECOND;

/////////////////////////////////uboot中 第二段参数区中AP与驱动共享信息  END/////////////////////////////////////////
typedef struct _ap_drive_shared_param
{
	unsigned char	mac1[32];		//MAC地址1
	unsigned char	mac2[32];		//MAC地址2
	unsigned int	hrVersion;		//GPIO 硬件版本号
	unsigned char	byPanelType[8];	//面板类型
    tuint32         ih_time;	/* Image Creation Timestamp	*/
    unsigned char   byVideoFormat; //视频制式
    unsigned char	byReserve[3];//保留不可用
    unsigned char   bySecurityCode[8];//security code
    unsigned char   bySecurityCodeDefault[8];
    unsigned char	byReserve1[28];//保留不可用
} AP_DRIVE_SHARED_PARAM;

typedef enum _SDK_UPDATE_STATUS_TYPE_
{
	SDK_UPDATE_STATUS_TYPE_RANGE_BEGIN	= 0,
	SDK_STATUS_UPDATING	=0x01,
	SDK_STATUS_UPDATESUCCESS =0x02,
	SDK_STATUS_UPDATEFAIL =0x03,
	SDK_UPDATE_STATUS_TYPE_RANGE_END
}SDK_UPDATE_STATUS_TYPE;



//移动侦测特征数据描述
typedef struct _tvt_motion_describe
{
	tuint64 timeStamp;       //时间戳，与同一IPC所有码流的时间戳一致，同一模拟通道也类似。
	tuint16 compressType;    //压缩类型，目前暂时不使用，填0即可
	tuint8  widthDivideNum;  //从一帧提取出的特征数据对图像宽度的实际划分份数, <= 64.
	tuint8  heightDivideNum; //从一帧提取出的特征数据对图像高度的实际划分份数, <= 64.
    tuint32  date[128];      //数据区，按位存取，至多可64x64.
}TVT_MOTION_DESCRIBE;
/*关于数据区
1. 从形式上，数据区改成一维数组。
2. 从使用上，要将一维数组还原成二维的来使用。
3. 每行按32位对齐，从低位开始填充。
4. 如果划分份数是22x18，widthDivideNum=22， heightDivedeNum=18， 则每一行只需要32位即可，从低位开始填充，数据区填充方式如下：
5. 如果划分份数是36x24，widthDivideNum=36， heightDivedeNum=24， 则每一行需要2个32位，从低位开始填充，数据区填充方式如下：
*/

/************************************************************************/
/*            常量      宏定义   枚举值     结构体                   END          */
/************************************************************************/

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/************************************************************************/
/*            回调函数                   BEGIN          */
/************************************************************************/
/////插入显示器时，驱动识别显示器支持分辨率的回调函数
typedef void (*OutputChangeCBFun)(int nChipIndex, int nOutputIndex, unsigned int dwSupport, void *pUserParam);	//dwSupport: TVT_DRIVER_SUPPORT_RESOLUTION定义支持的分辨率

////////
typedef void (*StatusChangeCBFun)(int nChipIndex, enum TVT_DRIVER_STATUS_CHANGE StatusType, unsigned long long ullStatusValue, void *pParamEx, void *pUserParam);	

////////
typedef void (*JpegEncCallback)(void* pInParam, VAC_FRAME_EXTENSION_INFO* pJpegInfo, char* pJpegAddr );

////////现场或回放抓图回调:bRetCode 0:成功-1失败 nJpegLen数据长度
typedef void (*CaptureDisPlayCallback)( int bRetCode, int nJpegLen, void* pInParam );

typedef void (*UpdateStatusCBFun)( SDK_UPDATE_STATUS_TYPE updateStatus, int progress, void *pUserParam);

////////获取报警盒子离、在线回调:bstatus 0:离线1:在线
typedef void (*ExtendAlarmDevCallback)( int bstatus, void* pUserParam );

/************************************************************************/
/*            回调函数                   END          */
/************************************************************************/

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/************************************************************************/
/*           接口                   BEGIN          */
/************************************************************************/

/***********************************************************************************************系统接口**********************************************************************************************************************/
/************************************************************************
**功能说明：设置音频格式和制式, 启动时一次性设置编码和解码音频格式，制式
**返回值：
************************************************************************/
// 初始化 和 释放资源接口
EXTERN_C int VAC_InitialDevice(VIDEO_STANDARD standardType,  AUDIO_FORMAT AudioFMT);
EXTERN_C int VAC_ReleaseDevice();

//产品加密相关接口
EXTERN_C int VAC_GetDefaultProductID( int* p_chnn_count );
EXTERN_C int VAC_EncryptChipCheck( int * p_chnn_count );
EXTERN_C bool VAC_SetProductID( int productID, int chnn_count  );

/************************************************************************
**函数名称：VAC_SetStatusChangeCBFun
**功能说明：设置通道状态改变回调函数，状态包括：信号遮挡、移动侦测、信号状态改变
**返回值：
************************************************************************/
EXTERN_C int VAC_SetStatusChangeCBFun( StatusChangeCBFun pStatusChangeCBFun, void *pUserParam );
/************************************************************************
**函数名称：VAC_SetDevStatusChangeCBFun
**功能说明：设置设备状态改变回调函数，状态包括：Sensor状态改变
**返回值：
************************************************************************/
EXTERN_C int VAC_SetDevStatusChangeCBFun( StatusChangeCBFun pStatusChangeCBFun, void *pUserParam );

/////设置HDMI输出插入显示器时的回调函数
EXTERN_C void VAC_SetOutputChangeCBFun(OutputChangeCBFun pOutputChangeCBFun, void *pUserParam);
/////系统支持分辨率集合
EXTERN_C void VAC_GetSupportResolutionSet(int nOutputIndex, unsigned int* p_Resolution_supportSet);
/////设置输出分辨率
EXTERN_C int VAC_SetOutputResolution(int nChipIndex, int nOutputIndex, int ulWidth, int ulHeight, TVT_DRIVER_SUPPORT_RESOLUTION enIntSync);

/////设置图像输出分割模式
EXTERN_C bool VAC_SetImageDisplaySplitMode(int nChipIndex, int nOutputIndex, int nSplitMode);

/************************************************************************
**函数名称：VAC_SetFbColorkeyValue
**功能说明：窗口透明色，顶层内容可以透过窗口（现在顶层内容仅指视频）
**返回值：
************************************************************************/
EXTERN_C int VAC_SetFbColorkeyValue( int color_key );

/////看门狗控制
EXTERN_C bool VAC_WatchDogCtrl(bool bEnable);
EXTERN_C void VAC_FoodDog();

/////面板LED控制
EXTERN_C bool VAC_LEDShowCtrl(LED_TYPE led_type, bool bOpen );
EXTERN_C bool VAC_LEDShowVideoStatus(LED_TYPE led_type, bool bOpen, int chnn );
EXTERN_C bool VAC_LEDVideoLossEffective(void);
EXTERN_C bool VAC_LEDShowHDDStatus( int index, bool bOpen );
EXTERN_C bool VAC_LEDHDDEffective(void);
EXTERN_C bool VAC_PowerOffDev(void);

/////继电器+蜂鸣器控制
EXTERN_C bool VAC_BuzzerCtrl(bool bOpen, int nIntervalMS);
EXTERN_C bool VAC_GetSensorStatus(int *pStatusValue, int iAlarmNum);
EXTERN_C bool VAC_AlarmOutCtrl(int nAlarmOutIndex, bool bAlarm);

/////前面板+遥控器获取键值
EXTERN_C int VAC_GetKeyboardValue( VAC_KEYFRAME_INFOMATION* p_key_info );

/////Flash参数设置
EXTERN_C int VAC_SetFlashParam(char* param_value, int param_len, int offset);
EXTERN_C int VAC_GetFlashParam(char* param_value, int param_len, int offset);

/////获取版本信息
EXTERN_C bool VAC_GetKernelVersion( char* p_version_info, int len );
EXTERN_C bool VAC_GetMCUVersion( char* p_version_info, int len );

/////设置Uboot引导信息
EXTERN_C int VAC_SetBootImageName(char* boot_image );

/************************************************************************
**功能说明：485串口和同轴读写 
**函数参数：Buf+波特率+类型
**返回值：
************************************************************************/
/**
* 读同轴控制信息
* @param[in] bitrate 300~115200 串口波特率；0: 同轴COC 控制
* @param[in] chn bitrate为0 时是同轴控制通道。bitrate大于0时无实际意义
* @param[in] data  串口或同轴发送数据指针
* @param[in] len  串口或PTZ发送数据data 长度
* @return 0 成功；-1 失败
* @note 目前bitrate没有使用，此接口只有控制同轴功能
* @see VAC_CocAndComRead
*/ 
EXTERN_C int VAC_CocRead(int chn, int* data, int *p_len);
EXTERN_C int VAC_ComRead(char* data, int *p_len);

/**
* 控制同轴或者写PTZ 串口控制数据
* @param[in] bitrate 300,1200,2400,4800,9600,19200,38400,57600,115200 串口波特率；0: 同轴COC 控制
* @param[in] chn bitrate为0 时是同轴控制通道。bitrate大于0时无实际意义
* @param[in] data  串口或同轴发送数据指针
* @param[in] len  串口或PTZ发送数据data 长度
* @return 0 成功；-1 失败
* @note bitrate 为0是控制同轴，大于0时是控制PTZ云台
* @see VAC_CocAndComRead
*/ 
EXTERN_C int VAC_CocWrite(int chn, int* data, int len);
EXTERN_C int VAC_ComWrite(int bitrate, char* data, int len);

//系统+RTC时间AP设定后通知驱动或者响应界面设置时间时直接调用驱动接口设定时间。                 
//驱动或者AP任何一个设定时间均可，原则：系统+RTC时间一起设定        RTC时间不用脚本设置
/************************************************************************
**功能说明：VAC_SetSystemTime,VAC_GetRTCTime已经废除,时间设置参考VAC_SyncRTCTime的接口说明
************************************************************************/
//EXTERN_C int VAC_SetSystemTime(int nYear, int nMonth, int nDay, int nHour, int nMinuter, int nSecond);////////////
//EXTERN_C int VAC_GetRTCTime(int *nYear, int *nMonth, int *nDay, int *nHour, int *nMinuter, int *nSecond);////////////

/************************************************************************
**函数名称：VAC_SyncRTCTime
**功能说明：同步RTC时间
**参数说明:   bSystemTime, 是否同步编码系统时间
**返回值：0:成功 -1 失败
**第一次启动(start.sh 启动脚本)、手动更改时间  这2个逻辑会调用 hwclock
**系统退出,已及运行每隔12小时会调用一次 VAC_SyncRTCTime
************************************************************************/
EXTERN_C int VAC_SyncRTCTime(bool bSystemTime);

/************************************************************************
**功能说明：获取SDK参数   通用接口
**返回值：
************************************************************************/
EXTERN_C int VAC_GetSDKParam(int paramNum, ...);

/************************************************************************
**功能说明：设置SDK参数   通用接口
**返回值：
************************************************************************/
EXTERN_C int VAC_SetSDKParam(int paramNum, ...);


/************************************************************************
**功能说明：开启或关闭SPOT功能，仅对支持SPOT功能产品有效
**输入参数：true表示开启Spot，false表示关闭Spot
**返回值：
************************************************************************/
EXTERN_C int VAC_SpotCtrl(bool bEnable);

/************************************************************************
**功能说明：改变SPOT输出的通道
**返回值：
************************************************************************/
EXTERN_C int VAC_ChangeSpotChannel(int nChannel);

/***********************************************************************************音视频编码************************************************************************************************************/
EXTERN_C int VAC_SetMainStreamEncodeParam( int nChipIndex, int nChannelIndex, VIDEO_ENCODE_PARAM* pEncodeParam );
EXTERN_C int VAC_SetSubStreamEncodeParam( int nChipIndex, int nChannelIndex, VIDEO_ENCODE_PARAM* pEncodeParam );
EXTERN_C int VAC_SetAuxStreamEncodeParam( int nChipIndex, int nChannelIndex, VIDEO_ENCODE_PARAM* pEncodeParam );

/************************************************************************
**功能说明：打开主码流编码
**返回值：成功返回大于等于0， 失败返回-1
************************************************************************/
EXTERN_C int VAC_EncOpenForMainStream( int nChipIndex, int nChannelIndex );   /////打开主码流编码
EXTERN_C void VAC_EncCloseForMainStream(  int hEnc ); /////关闭主码流编码

EXTERN_C int VAC_EncOpenForSubStream( int nChipIndex, int nChannelIndex );/////打开子码流编码
EXTERN_C void VAC_EncCloseForSubStream(  int hEnc );/////关闭子码流编码

EXTERN_C int VAC_EncOpenForAuxStream( int nChipIndex, int nChannelIndex );/////打开第三码流编码
EXTERN_C void VAC_EncCloseForAuxStream(  int hEnc );/////关闭第三码流编码

/////音频编码
EXTERN_C int VAC_EncOpenForAudioStream( int nChipIndex, int nChannelIndex );/////打开通道音频编码
EXTERN_C void VAC_EncCloseForAudioStream(  int hEnc );/////关闭通道音频编码

EXTERN_C int VAC_EncSelect(int nChipIndex, int *pHasDataFlag, int nCount, int nMsecTimeout);
EXTERN_C int VAC_EncGetDataLength( int hEnc );/////获取通道码流数据长度
EXTERN_C int VAC_EncGetData(  int hEnc,char *pOutBuf, int nBufLen, VAC_FRAME_EXTENSION_INFO* pstFrameExtInfo );////获取数据

EXTERN_C int VAC_RequestMainStreamKeyframe( int nChipIndex, int nChannelIndex );//////////立即产生关键帧
EXTERN_C int VAC_RequestSubStreamKeyframe( int nChipIndex, int nChannelIndex );//////////立即产生关键帧
EXTERN_C int VAC_RequestAuxStreamKeyframe( int nChipIndex, int nChannelIndex );//////////立即产生关键帧

/********************回放接口********************/
EXTERN_C int VAC_PlaybackStop(void);
EXTERN_C int VAC_PlaybackStart( unsigned long ulChannlInfo );
EXTERN_C int VAC_PlaybackRestart(unsigned long ulChannlInfo);
EXTERN_C int VAC_DropEncodeFPS(void);
EXTERN_C int VAC_RestoreEncodeFPS(void);

/***********************************************************************************音视频解码************************************************************************************************************/
EXTERN_C int VAC_InputAudioData(int nChipIndex, int nChannelIndex,  char *pBuf, int nBufSize);
EXTERN_C int VAC_InputVideoData(int nChipIndex, int nChannelIndex,  VAC_FRAME_EXTENSION_INFO* p_frm_ext, char *pBuf, int nBufSize);

//音频抓取
EXTERN_C int VAC_EncodeChannelOpenForAudio(int nChipIndex,int nChannelIndex, VAC_AUDIO_ENCODE_STREAM_ATTR *pencodeStreamAttr);		//打开音频抓取
EXTERN_C int VAC_EnChannelCloseForAudio(int nChipIndex, int nChannelIndex);										//关闭音频抓取
EXTERN_C int VAC_GetCaptureEnAudioStream(int nChipIndex, int nChannelIndex, char *pOutBuf, int nBufLen);		//返回编码后的有效数据长度




/////////////////H264/265 解码YUV后直接输出显示的接口群   BEGIN
/***********************************************************************************音频输出+视频显示************************************************************************************************************/
EXTERN_C int VAC_CreateDecodeEngine(int nChipIndex, TVT_DECODE_ENGINE *pDecodeEngine, int nNumber);		//创建解码引擎
EXTERN_C int VAC_DestroyDecodeEngine(int nChipIndex, TVT_DECODE_ENGINE *pDecodeEngine, int nNumber);	//销毁解码引擎

EXTERN_C int VAC_OpenDisplayChannel(int nChipIndex, int nOutputIndex, int nChannelIndex, int nX, int nY, int nWidth, int nHeight, int nZIndex);
EXTERN_C int VAC_CloseDisplayChannel(int nChipIndex, int nOutputIndex, int nChannelIndex);
EXTERN_C int VAC_Zoom(int nChipIndex, int nOutputIndex, int nChannelIndex, int nX, int nY, int nWidth, int nHeight);

EXTERN_C int VAC_OpenDisplayChannelBAT(int nChipIndex, int nOutputIndex, LIVE_DISPLAY_ATTRIB* nDispPlayAttrib, int chnNum);
EXTERN_C int VAC_CloseDisplayChannelBAT(int nChipIndex, int nOutputIndex, int *nChannelIndex, int chnNum);
/////////////////H264/265 解码YUV后直接输出显示的接口群   END


//打开/关闭画中画
EXTERN_C int VAC_OpenPIPChannel(int nChipIndex, int nOutputIndex, int nChannelIndex, int nX, int nY, int nWidth, int nHeight);
EXTERN_C int VAC_ClosePIPChannel(int nChipIndex, int nOutputIndex, int nChannelIndex);

// 打开鱼眼校正通道
EXTERN_C int VAC_OpenFisheyeAdjustChannel(int nChipIndex, int nOutputIndex, int nChannelIndex, int nX, int nY, int nWidth, int nHeight );
EXTERN_C int VAC_CloseFisheyeAdjustChannel(int nChipIndex, int nOutputIndex, int nChannelIndex);
EXTERN_C int VAC_SetFisheyeMode( int fisheye_type, const FISHEYE_PARA *fish_para, int cnt);


EXTERN_C int VAC_OpenChlAudioOutput(int nChipIndex, int nChannelIndex);
EXTERN_C int VAC_CloseChlAudio(int nChipIndex, int nChannelIndex);

/***********************************************************************************音视频播放控制***************************************************************************************************************************/
/************************************************************************
**函数名称：VAC_SetChnnVdecFrmSpeed
**功能说明：设置播放的帧率
**返回值：
************************************************************************/
EXTERN_C int VAC_SetChnnVdecFrmSpeed(int nChipIndex, int nChannelIndex , int frm_sec );

/************************************************************************
**函数名称：VAC_GetChnnVdecLeftFrmCnt
**功能说明：获取剩余帧数量
**返回值：
************************************************************************/
EXTERN_C int VAC_GetChnnVdecLeftFrmCnt(int nChipIndex, int nChannelIndex );

/************************************************************************
**函数名称：VAC_SetPlaybackPauseState
**功能说明：设置暂停或播放状态
**参数： bPause为0-播放，1-暂停
**返回值：
************************************************************************/
EXTERN_C int VAC_SetPlaybackPauseState(int nChipIndex, int nChannelIndex, bool bPause);
EXTERN_C int VAC_SetAudioVolumn( int nChipIndex, int nChannelIndex, int vol );		//vol: 0-100


/***********************************************************************************JPG编码+解码***************************************************************************************************************************/
/************************************************************************
**函数名称：VAC_CaptureDisplayToJpeg
**功能说明：抓取正在显示的视频图片
**参数： img_qty: 0高-5低
**返回值：
************************************************************************/
EXTERN_C int VAC_CaptureDisplayToJpeg( int nChipIndex, int nOutputIndex, int nChannelIndex, int img_qty, unsigned char* p_img_buf, int buf_len, int* jpeg_len );//
/************************************************************************
**函数名称：VAC_SetCaptureDisplayCallback
**功能说明：设置现场或回放的回调函数，实现异步处理
**此回答返回抓图是否成功及成功后的数据长度
**参数： 
**返回值：
************************************************************************/
EXTERN_C int VAC_SetCaptureDisplayCallback( CaptureDisPlayCallback pCaptureCallback, void* pInParam );




/////////////////H264/265 解码YUV后，编码成JPG输出的接口群           BEING

//解码H.264视频为JPG图片    非阻塞
EXTERN_C int VAC_OpenVideoDecodeToJPG(int nChipIndex, int nChannelIndex, int img_qty);
EXTERN_C int VAC_CloseVideoDecodeToJPG(int nChipIndex,int nChannelIndex);
EXTERN_C int VAC_GetJPGStream(int nChipIndex,int nChannelIndex, char *pOutBuf, int nOutBufLen, char *userParam);		//返回数据有效长度
EXTERN_C int VAC_InputDataForJPG(int nChipIndex, int nChannelIndex,  VAC_FRAME_EXTENSION_INFO* p_frm_ext, char *pBuf, tuint32 nBufSize, int nResWidth, int nResHeight, char *userParam);

/////    阻塞
EXTERN_C int VAC_DecodeToJPG(VAC_FRAME_EXTENSION_INFO* p_frm_ext, char *pInBuf, tuint32 nInBufLen, int nResWidth, int nResHeight, int nImagQty, char *pOutBuf, tuint32 nOutBufLen);
/////////////////H264/265 解码YUV后，编码成JPG输出的接口群           END

/////////////////DVR直接原始码流编码成JPG输出的接口群           BEGIN
EXTERN_C int VAC_SetJpegEncParam( int nChipIndex, int nChannelIndex, int nSnapFrames, int nSnapTimes, int nImagQty, int nVideoWidth, int nVideoHeight );/////仅DVR有效
/************************************************************************
**函数名称：VAC_SetJpegCallback
**功能说明：设置JPG编码的回调函数
**参数： img_qty: 0高-5低
**返回值：
************************************************************************/
EXTERN_C int VAC_SetJpegEncCallback( JpegEncCallback pJpegEncCallback, void* pInParam );////////////DVR有效

/////////////////DVR直接原始码流编码成JPG输出的接口群           END

/***********************************************************************************通道设置***************************************************************************************************************************/
EXTERN_C int VAC_SetVideoBrightness( int nChipIndex, int nChannelIndex, int nValue );//////设置图像色彩
EXTERN_C int VAC_SetVideoHue( int nChipIndex, int nChannelIndex, int nValue );/////设置图像色彩
EXTERN_C int VAC_SetVideoContrast( int nChipIndex, int nChannelIndex, int nValue );/////设置图像色彩
EXTERN_C int VAC_SetVideoSaturation( int nChipIndex, int nChannelIndex, int nValue );/////设置图像色彩
EXTERN_C int VAC_SetVideoSharpness( int nChipIndex, int nChannelIndex, int nValue );/////设置图像锐度
EXTERN_C int VAC_SetShiftImage( int nChipIndex, int nChannelIndex, int nValue );		//设置图像偏移
EXTERN_C int VAC_SetVideoEQ( int nChipIndex, int nChannelIndex, int nValue );/////设置EQ值,0:自动, 其余为1~6

/////////////通道OSD
EXTERN_C int VAC_SetVideoCaptionOSD( int nChipIndex, int nChannelIndex, int nOsdIndex, VIDEO_OSD_ATTRIB* pOsdAttrib );////////
EXTERN_C int VAC_CleanVideoCaptionOSD( int nChipIndex, int nChannelIndex, int nOsdIndex );////////

///////////通道时间戳
EXTERN_C int VAC_SetVideoTimestampOSD( int nChipIndex, int nChannelIndex, VIDEO_OSD_ATTRIB* pOsdAttrib );////////
EXTERN_C int VAC_CleanVideoTimestampOSD( int nChipIndex, int nChannelIndex );//////// 

////////////通道水印
EXTERN_C int VAC_SetVideoWaterMark( int nChipIndex, int nChannelIndex, unsigned char* user_info, int user_len );
EXTERN_C int VAC_CleanVideoWaterMask( int nChipIndex, int nChannelIndex );

/////////////////////区域隐藏
EXTERN_C int VAC_SetVideoCoverArea( int nChipIndex, int nChannelIndex, VIDEO_AREA_ATTRIB* pCoverArea, int areaNum, unsigned long color );//////区域隐藏
EXTERN_C int VAC_CleanVideoCoverArea( int nChipIndex, int nChannelIndex, int nCoverIndex );///////

EXTERN_C int VAC_SetVideoMotionArea( int nChipIndex, int nChannelIndex, VAC_MOTION_AREA_INFO* pMotionArea );//////移动侦测
EXTERN_C int VAC_SetVideoMotionSensite( int nChipIndex, int nChannelIndex, int nSensite );//////移动侦测

//////////////信号遮盖
EXTERN_C int VAC_SetVideoShelterArea( int nChipIndex, int nChannelIndex, VIDEO_AREA_ATTRIB* pShelterArea, int areaNum );////////遮盖   
EXTERN_C int VAC_SetVideoShelterSensite( int nChipIndex, int nChannelIndex, int nSensite );/////信号遮挡    灵敏度
EXTERN_C int VAC_CleanVideoShelterArea( int nChipIndex, int nChannelIndex );////////遮盖    

//////////获取通道能力集
////////////pChnStreamCaps为NULL的时候   pItemCounts返回数量

/************************************************************************
**函数名称：VAC_GetChnStreamCaps
**功能说明：获取通道能力集
**参数： pChnStreamCaps为NULL的时候   pItemCounts返回需要CHN_STREAM_CAPS的数量， pChnStreamCaps不为NULL的时候   pItemCounts返回有效数量
**返回值：
************************************************************************/
EXTERN_C int VAC_GetChnStreamCaps( int nChipIndex, int nChannelIndex, CHN_STREAM_CAPS* pChnStreamCaps, int *pItemCounts);////
EXTERN_C int VAC_GetChnCaps( int nChipIndex, int nChannelIndex, CHN_CAPS *pChnCaps );//////
EXTERN_C int VAC_GetChnCoaxialType( int nChipIndex, int nChannelIndex, int *pCoaxialType );//////

//////////设置鼠标位置
EXTERN_C int VAC_SetMousePosition( int dx, int dy );
//////////设置鼠标样式;len为固定长度4096
EXTERN_C int VAC_SetMouseShape( char *pdata, int len );

/************************************************************************
**函数名称：VAC_SetDefaultColor
**功能说明：设置信号色彩默认值
**参数：videoType  类型为VIDEO_TYPE_DEF_COLOR
**返回值:0  成功
************************************************************************/
EXTERN_C int VAC_SetDefaultColor(int videoType, int bright, int hue, int sat, int contrast);

/************************************************************************
**函数名称: VAC_ResetChip
**功能说明: 复位芯片, 立即重启
**参数: 无
**返回值: 无
************************************************************************/
EXTERN_C void VAC_ResetChip(void);

/************************************************************************
**函数名称: VAC_GetChnMotionDescribe
**功能说明: 获取基于Motion侦测的特征数据
**参数: nChannelIndex 通道号偏移量
**返回值: 0表示无特征数据，>0表示有特征数据，-1表示输入参数非法
************************************************************************/
EXTERN_C int VAC_GetChnMotionDescribe(int nChannelIndex, TVT_MOTION_DESCRIBE *pMdDate);


/************************************************************************
**函数名称：VAC_FBInitForUpdate
**功能说明：升级时初始化Directfb
**参数：
**返回值:0  成功-1  失败
************************************************************************/
EXTERN_C int VAC_FBInitForUpdate();

/************************************************************************
**函数名称：VAC_SetUpdateStatusCBFun
**功能说明：设置升级状态回调函数
**参数：
**返回值:0  成功-1  失败
************************************************************************/
EXTERN_C int VAC_SetUpdateStatusCBFun(UpdateStatusCBFun pUpdateStatusCBFun, void* pInParam);

/************************************************************************
**函数名称：VAC_SetDevStatusExtendAlarmCBFun
**功能说明：设置报警扩展盒回调函数
**参数：
**返回值:0  成功-1  失败
************************************************************************/
EXTERN_C int VAC_SetDevStatusExtendAlarmCBFun( ExtendAlarmDevCallback pStatusExtendAlarmCBFunc, void* pUserParam );


/************************************************************************
**函数名称：VAC_Update
**功能说明：设备软件升级
**参数：fileAddr  升级文件地址，len  升级文件长度, force_upgrade 是否强制升级
**返回值:0  成功-1  失败
************************************************************************/
EXTERN_C int VAC_Update(unsigned char *fileAddr, int len, bool force_upgrade);

/************************************************************************/
/*           接口                            */
/************************************************************************/
typedef enum _GET_DATE_TYPE
{
    DATE_YUV,
    DATE_Y,
    DATE_RGB
}GET_DATE_TYPE;

typedef struct _yuv_decoder_param
{
	unsigned int vdeWidth;  // 创建解码器宽
	unsigned int vdeHeight; // 创建解码器高
	GET_DATE_TYPE dateConverType;
}YUV_DECODER_PARAM;

typedef struct _yuv_date_param
{
	char *pBuf;               // 数据指针
	unsigned int nBufSize;    // 数据长度
	unsigned int yuvdeWidth;  // 获取yuv 宽
	unsigned int yuvHeight;   // 获取yuv 高
	char *userParam;          //用户私有指针，方便功能扩展
}YUV_DATE_PARAM;



/////////////////////////////////////////////////////////////////////////////
typedef enum _image_format_type_
{
    IMAGE_FORMAT_TYPE_RANGE_BEGIN,
    IMAGE_FORMAT_TYPE_JPG,
    IMAGE_FORMAT_TYPE_YUV,
    IMAGE_FORMAT_TYPE_264,
    IMAGE_FORMAT_TYPE_265,
    IMAGE_FORMAT_TYPE_RGB,
    IMAGE_FORMAT_TYPE_RANGE_END
}IMAGE_FORMAT_TYPE;

typedef struct _a2b_format_convert_param
{
	unsigned int vdeWidth;  // 创建解码器宽
	unsigned int vdeHeight; // 创建解码器高
    //IMAGE_FORMAT_TYPE    sourcedateformat;
	IMAGE_FORMAT_TYPE    destdateformat;
}A2B_FORMAT_CONVERT_PARAM;


typedef struct _extend_date_param
{
	char *pBuf;               // 数据指针
	unsigned int nBufSize;    // 数据长度
	unsigned int destWidth;  // 
	unsigned int destHeight;   // 	
	char *userParam;          //用户私有指针
}EXTEND_DATE_PARAM;

/////////////////格式转换接口群         BEGIN
/*
20181013前 JPG ——》RGB
20181013后
JPG ——》YUV或RGB
264 265——》YUV
YUV——》JPG
*/

/*
使用流程
1.打开解码引擎，设置解码引擎长宽和数据yuv类型
2.输入JPEG数据，并调用 VAC_GetYUVStream 查询获取yuv数据，此过程可循环调用。
3.不使用时，销毁通道。
*/
//typedef enum _JPG_RGB_DECODEID
//{
//    JPG_RGB_DECODEID_FIRST = 0,
//    JPG_RGB_DECODEID_SECOND,
//    JPG_RGB_DECODEID_THIRD
//}JPG_RGB_DECODEID;


typedef enum _data_convert_id_
{
    DATA_CONVERT_ID_JPG2RGB_0 = 0x0,
    DATA_CONVERT_ID_JPG2RGB_1,
    DATA_CONVERT_ID_JPG2RGB_2,
    DATA_CONVERT_ID_26X2YUV_0 = 0x10,
    DATA_CONVERT_ID_26X2YUV_1,
    DATA_CONVERT_ID_26X2YUV_2,
    DATA_CONVERT_ID_26X2YUV_3,
    DATA_CONVERT_ID_YUV2JPG_0 = 0x20,
    DATA_CONVERT_ID_YUV2JPG_1
}DATA_CONVERT_ID;


typedef struct _enc_dest_info
{
    int 	nEncType;	// 1:jpeg;
    int 	nImagQty; //编码数据质量 0-5， 0最好，5最差

    union
    {
        struct
        {
            char	*pDataBuf;//jpeg数据缓冲区
            int 	nBufLen;	//jpeg数据缓冲区长度
            int 	ndataLen; //返回的jpeg数据的实际长度
        }jpeg_enc_info;
    }enc;

}ENC_DEST_INFO;

typedef struct raw_data_info{
	/*! 图像格式。 */
	unsigned int u32Width;
	unsigned int u32Height;
    unsigned int enField;

	unsigned int enPixelFormat;  //PIXEL_FORMAT_E
	unsigned int	u32Stride[3];
	union
    {
        struct
        {
            unsigned long long	u64PhyAddr[3];
			unsigned long long  u64VirAddr[3];
        }st64;
        struct
        {
            unsigned int  u32PhyAddr[3];
			unsigned int  u32VirAddr[3];
        }st32;
    }Addr;


	short   s16OffsetTop;        /* top offset of show area */
	short   s16OffsetBottom;    /* bottom offset of show area */
	short   s16OffsetLeft;        /* left offset of show area */
	short   s16OffsetRight;        /* right offset of show area */

	unsigned long long    u64PTS;
}RAW_DATA_INFO;

typedef struct _rect_s
{
    int 	s32StartX;
    int 	s32StartY;
    unsigned int	u32Width;
    unsigned int 	u32Height;
}TVT_RECT;

EXTERN_C int VAC_OpenVideoDecodeToYUV(int nChipIndex, YUV_DECODER_PARAM *yuv_decoder_param, int nDecodeID);
EXTERN_C int VAC_CloseVideoDecodeToYUV(int nChipIndex, int nDecodeID);
EXTERN_C int VAC_InputDataForYUV(int nChipIndex, int nChannelIndex, VAC_FRAME_EXTENSION_INFO* p_frm_ext, YUV_DATE_PARAM* p_yuv_param);
EXTERN_C int VAC_GetYUVStream(int nChipIndex, char *pOutBuf, int nOutBufLen, char *userParam);

EXTERN_C int VAC_CreateA2BFormatConvert(int nChipIndex, DATA_CONVERT_ID enConvertID, A2B_FORMAT_CONVERT_PARAM *pParam);
EXTERN_C int VAC_DestroyA2BFormatConvert(int nChipIndex, DATA_CONVERT_ID enConvertID);
EXTERN_C int VAC_InputEncDataToDec(int nChipIndex, DATA_CONVERT_ID enConvertID, VAC_FRAME_EXTENSION_INFO* p_frm_ext, EXTEND_DATE_PARAM* pParam);
EXTERN_C int VAC_InputSrcDataToEnc(int nChipIndex, DATA_CONVERT_ID enConvertID, VAC_FRAME_EXTENSION_INFO* p_frm_ext, EXTEND_DATE_PARAM* pParam);
EXTERN_C int VAC_GetConvertResult(int nChipIndex, DATA_CONVERT_ID enConvertID, RAW_DATA_INFO *pstRawInfo);
EXTERN_C int VAC_RawDataToEnc_Block(int nChipIndex, RAW_DATA_INFO* pRawInfo, TVT_RECT *pRect, ENC_DEST_INFO* pEncDestInfo);

/************************************************************************
**函数名称：VAC_GetGUIDVersion
**功能说明：版本信息获取  UBOOT(1字节)/文件系统(1字节)/内核(1字节)/KO(2字节)/SDK(2字节)
**参数：p_version_info  版本信息地址，len  版本信息长度
**返回值:0  成功-1  失败
************************************************************************/
EXTERN_C bool VAC_GetGUIDVersion( char* p_version_info, int len );

/************************************************************************/


// POE 状态设置
EXTERN_C int VAC_GetPoeStatus(GET_POE_STATUS *poe_status);
EXTERN_C int VAC_SetPoeStatus(SET_POE_STATUS *poe_set_status);

EXTERN_C int VAC_CipherGetMem(unsigned int u32CipherID, unsigned char **pu8SrcAddr, unsigned char **pu8DstAddr, int nSize);
EXTERN_C int VAC_CipherEncrypt(unsigned int nCipherID, TVT_CIPHER_CTRL_S *pstCipherConfig, TVT_CIPHER_DATA_S *pstDataInfo);
EXTERN_C int VAC_CipherDecrypt(unsigned int nCipherID, TVT_CIPHER_CTRL_S *pstCipherConfig, TVT_CIPHER_DATA_S *pstDataInfo);

/************************************************************************/


/*           接口                   END          */
/************************************************************************/

#endif/////////////





