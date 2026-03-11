
#ifndef _TYPE_CHECK_
#define _TYPE_CHECK_

#include "base_type_define.h"
/************************************************************************
通过给定的值去判断节点的类型，设备管理里面判断类型的可以去掉了 
可以避免很多地方用到这个枚举值以后需要修改的时候很多地方要改动
为了方便同时提供两种方式：定义对象时传入参数，然后判断多次，或直接使用静态函数直接判断的方式
************************************************************************/
class CTypeCheck
{
protected:
	/////////////// ////节点类型,本来要把客户端独立成一组，由于要让手机具有前后版本的兼容性
	enum _ecms_node_type_def_
	{
		NODE_TYPE_INVALID_NULL,
		
		NODE_TYPE_CLIENT_BEGIN = 0x1,//客户端开始范围，以用户为用户名资源登陆的
		NODE_TYPE_CLIENT_CONFIG=NODE_TYPE_CLIENT_BEGIN,//////////配置客户端
		NODE_TYPE_CLIENT_MONITOR,/////////监控客户端
		NODE_TYPE_CLIENT_IE, /////IE客户端
		NODE_TYPE_CLIENT_MOBILE,   //////手机客户端
		NODE_TYPE_CLIENT_TVWALL,//电视墙控制端
		NODE_TYPE_CLIENT_DECODER,//解码器客户端
		NODE_TYPE_CLIENT_1400OUT,//1400OUT客户端 1400下级网关
		NODE_TYPE_CLIENT_END = 0x1F,//客户端结束范围

		//NVMS5000里面存在的各种服务器
		NODE_TYPE_NVMS5000_SERVER_BEGIN=0x20,///认证服务器,
		NODE_TYPE_AUTHENTICATION_SERVER=NODE_TYPE_NVMS5000_SERVER_BEGIN,//////////
		NODE_TYPE_MEDIA_TRANSFER_SERVER,//////////媒体转发服务器
		NODE_TYPE_STORAGE_SERVER,//////////存储服务器
		NODE_TYPE_EMAP_SERVER,//////////电子地图服务器
		NODE_TYPE_ALARM_SERVER,//////////报警服务器		
		NODE_TYPE_TVWALL_SERVER,//////////电视墙服务器
		NODE_TYPE_GPS_SERVER,//GPS处理服务器
		NODE_TYPE_GSU_SERVER,//////////GPS存储服务器
		NODE_TYPE_JOIN_SERVER,//接入服务器
		NODE_TYPE_STATUS_SERVER,//状态服务器
		NODE_TYPE_ANALYSIS_SERVER,//智能分析服务器
		NODE_TYPE_CASCADE_GATEWAY_SERVER, //接入网关服务器
		NODE_TYPE_1400IN_SERVER, //1400上级网关服务器
		NODE_TYPE_APPLICATION_SERVER,//应用服务器，（app通过此应用服务器，再透传到其他服务）

		NODE_TYPE_NVMS5000_SERVER_END=0x7F,////

		//通用资源
		NODE_TYPE_COMMON_RES_BEGIN=0x80,///
        NODE_TYPE_CHANNEL,//通道
        NODE_TYPE_IP_CHANNEL,////IP通道  带IP地址，端口，通道索引，附加序号N9000特有
		NODE_TYPE_AREA,		//区域
		NODE_TYPE_WALL,  //墙，电视墙,不是服务
		NODE_TYPE_CHANNELGROUP,  //通道组
		NODE_TYPE_USERGROUP,//角色
		NODE_TYPE_SUBSYSTEM,//子系统
		NODE_TYPE_ZONE,//防区
		NODE_TYPE_HOT_SPOT,//热点
		NODE_TYPE_HOT_AREA,//热区
		NODE_TYPE_PARTITION_GROUP,////分区组
		NODE_TYPE_PARTITION,////分组
		NODE_TYPE_ACSDEVICE,//门禁设备
		NODE_TYPE_ACSDOOR,//门
		NODE_TYPE_ACSREADER,//门禁读头
		NODE_TYPE_CONFIG,
		NODE_TYPE_SOP,   //处警标准操作指导
		NODE_TYPE_PMSPARK,//停车场
		NODE_TYPE_PMSGATEWAY,//出入口
		NODE_TYPE_PMSLANE,//车道
		NODE_TYPE_PMSCHARGE_RULE,//收费规则
		NODE_TYPE_PMSFIXED_VEHICLE,//固定车辆
		NODE_TYPE_PMSTIME_SECTION_CHARGE_RULE,//自定义收费时间段
		NODE_TYPE_PMSBAG_PERIOD,//月卡包月收费表
		NODE_TYPE_PMSRECORDPASS, //通行记录
		NODE_TYPE_PMS_BACKLIST_VEHICLE,//黑名单车辆
		NODE_TYPE_ALARMTASK,  //告警任务
		NODE_TYPE_FK_DEPARTMENT_TYPE, //访客机部门
		NODE_TYPE_TIME_SECTION_FRAMERATE, //时间段帧率模板
		NODE_TYPE_CUSTOM,//自定义内容
		NODE_TYPE_PMS_VEHICLE_GROUP,//白名单车辆
		NODE_TYPE_ALBUM,//目标库 20200821改成人员分组了
		NODE_TYPE_ALARMOUTGROUP,  //通道组
		NODE_TYPE_EMAIL,//收件人地址
		NODE_TYPE_COMMON_RES_END=0xFF,///
		
		/////////////////支持的前端设备的类型定义
		NODE_TYPE_PERIPHERAL_DEVICE_BEGIN = 0x100, ////前端设备   前端设备的范围为 大于0x100 小于 0x1000的值
		NODE_TYPE_PERIPHERAL_ABSTRACT_DEVICE, //////抽象的前端设备类型  不是具体的设备类型 泛指设备
		PU_TVT_DVR,////////////////同为硬盘录像机	
		PU_TVT_IPCAMERA, ////////////////同为网络摄像机
		PU_TVT_MDVR,///同为车载DVR
		PU_HIKVISION_DVR, /////海康硬盘录像机
		PU_DAHUA_DVR,//大华DVR
		PU_HUAAN_IPC,//
		PU_JOIN_PROTOCOL_ONVIF,////ONVIF协议的类型，对于设备 协议类型和节点类型一致， 
		PU_JOIN_PROTOCOL_N9000,	//N9000作为接入设备的协议类型
		PU_TVT_N9000_DVR,//
		PU_ZENO_BOX,//智诺盒子类型
		PU_GB_DEVICE,//国标28181设备
		PROTOCOL_TYPE_ANALOG_CHANNEL,//////模拟通道虚拟成网络节点的协议类型
		PU_JOIN_PROTOCOL_RTSP,		//RTSP协议的类型
		PU_4G_IPC,	//4G IPC
		PU_THINKFORCE_PAD,		//熠知人脸平板
		PU_HUAAN_AUTOREPORT,		//华安主动上报,Pr车牌相机接入
		PU_YITU_BOX, //依图盒子
		PU_YITU_PAD, //依图人脸平板
		PU_PR_IPC,	//交通卡口IPC
        PU_SENSETIME_PAD_AUTOREPORT, //商汤平板，主动上报
        PU_EDGE_BOX, //边缘盒子
		NODE_TYPE_PERIPHERAL_DEVICE_END = 0x1FF,

		//解码设备
		NODE_TYPE_PERIPHERAL_DECODER_BEGIN = 0x200, ////解码设备   范围为 大于0x200 小于 0x2F0的值
		NODE_TYPE_PERIPHERAL_ABSTRACT_DECODER, //////抽象的解码设备类型  不是具体的设备类型 泛指设备
		NODE_TYPE_PERIPHERAL_DECODER_END = 0x2F0,

		//显示设备
		NODE_TYPE_SCREEN_LED_BEGIN = 0x2F1, //显示设备   范围为 大于0x2F1 小于 0x2FF的值
		NODE_TYPE_SCREEN_LED_ABSTRACT_DEVICE, //抽象的显示设备类型  不是具体的设备类型 泛指设备
		NODE_TYPE_SCREEN_LED_IP_HONGMEN, //红门 IP LED显示屏
		NODE_TYPE_SCREEN_LED_CHEYIFU, //车易付 485 LED显示屏
        NODE_TYPE_SCREEN_LED_XCP10, //对接中东非洲 SO21南非大客户 的英文显示屏 型号：XCP10-OT16*64R-12
		NODE_TYPE_SCREEN_LED_END = 0x2FF,

		//
		NODE_TYPE_PERIPHERAL_ALARMIN_BEGIN = 0x300,		//报警输入,不是指火警、抢劫这些类型，而是指设备Sensor、还是数字报警输入设备等
		NODE_TYPE_PERIPHERAL_ABSTRACT_ALARMIN ,		//抽象的报警输入类型,不是具体的报警输入类型 泛指报警输入
		NODE_TYPE_ALARMIN_DEVICE_SENSOR, //报警输入
        NODE_TYPE_ALARMIN_LOCAL_SENSOR,//本地报警输入
		NODE_TYPE_PERIPHERAL_ALARMIN_END = 0x3FF,		//报警输入

		NODE_TYPE_PERIPHERAL_ALARMOUT_BEGIN = 0x400,		//报警输输出,
		NODE_TYPE_PERIPHERAL_ABSTRACT_ALARMOUT,		//抽象的报警输出类型,不是具体的报警输出类型 泛指报警输出
		NODE_TYPE_ALARMOUT_DEVICE_ALARMOUT, //报警输出
        NODE_TYPE_ALARMOUT_LOCAL_ALARMOUT, //本地报警输出
		NODE_TYPE_PERIPHERAL_ALARMOUT_END = 0x4FF,		//报警输出

		//支持的报警主机类型
		NODE_TYPE_PERIPHERAL_ALARMHOST_BEGIN = 0x500, ////报警主机设备   范围为 大于0x500 小于 0x5FF的值
		NODE_TYPE_PERIPHERAL_ABSTRACT_ALARMHOST, //////抽象的报警主机设备类型  不是具体的设备类型 泛指设备
		ALARMHOST_HIKMODULE, //海康网络模块
		NODE_TYPE_ALARMHOST_TYPE_XKM,	//恒通的报警主机（新科姆瓦斯主机使用）
		NODE_TYPE_ALARMHOST_TYPE_ALEAN,	//艾礼安报警主机
		NODE_TYPE_ALARMHOST_TYPE_DSC_PC4401,	//DSC报警主机
		NODE_TYPE_ALARMHOST_TYPE_PARADOX,    //Paradox报警主机
		NODE_TYPE_ALARMHOST_TYPE_BOSCH_7400,    //BOSCH_7400报警主机
		NODE_TYPE_PERIPHERAL_ALARMHOST_END = 0x5FF,

		//探测器类型
		NODE_TYPE_PERIPHERAL_ZONE_DETECTOR_BEGIN = 0x600, ////防区探测器类型   范围为 大于0x600 小于 0x6FF的值
		NODE_TYPE_PERIPHERAL_ABSTRACT_ZONE_DETECTOR, //////抽象的探测器类型 
		NODE_TYPE_PERIPHERAL_ZONE_DETECTOR_END = 0x6FF,

		//电子地图类型
		NODE_TYPE_EMAP_TYPE_BEGIN = 0x700,//电子地图类型
		NODE_TYPE_EMAP_TYPE_ABSTRACT ,
		NODE_TYPE_EMAP_TYPE_PICTURE ,
		NODE_TYPE_EMAP_TYPE_BAIDU ,
		NODE_TYPE_EMAP_TYPE_GOOGLE ,
		NODE_TYPE_EMAP_TYPE_GAODE ,
		NODE_TYPE_EMAP_TYPE_WAPIAN ,
		NODE_TYPE_EMAP_TYPE_END = 0x7FF,

		//第三方门禁报警平台系统类型
		NODE_TYPE_ACSSYSTEM_TYPE_BEGIN = 0x800,//
		NODE_TYPE_ACSSYSTEM_TYPE_ABSTRACT ,
		NODE_TYPE_ACSSYSTEM_TYPE_ZKTECO ,//中控智慧
		NODE_TYPE_ACSSYSTEM_TYPE_ZLCX  ,//中联创新
		NODE_TYPE_ACSSYSTEM_TYPE_SZWG  ,//深圳微耕
		NODE_TYPE_ACSSYSTEM_TYPE_KANTECH  ,//泰科定制的
		NODE_TYPE_ACSSYSTEM_TYPE_AOPU, //AOPU门禁  
		NODE_TYPE_ACSSYSTEM_TYPE_END = 0x8FF,

		//级联平台系统类型
		NODE_TYPE_CASCADE_PLATFORM_TYPE_BEGIN = 0x900,//
		NODE_TYPE_CASCADE_PLATFORM_TYPE_ABSTRACT ,
		NODE_TYPE_CASCADE_PLATFORM_TYPE_GB28181 ,//国标平台
		NODE_TYPE_CASCADE_PLATFORM_TYPE_END = 0x9FF,

		//访客机类型
		NODE_TYPE_FK_TYPE_BEGIN = 0x1000,
		NODE_TYPE_FK_TYPE_ABSTRACT,
		NODE_TYPE_FK_TYPE_QIANLIN, //钱林
		NODE_TYPE_FK_TYPE_END = 0x10FF,

		NODE_TYPE_NVMS_PRODUCT_BEGIN = 0x1200,//系统解决方案名称
		NODE_TYPE_NVMS_PRODUCT_NVMS1000,///NVMS1000通用单机版本集中视频控制中心
		NODE_TYPE_NVMS_PRODUCT_NVMS5000=0x1250,//NVMS5000通用分布式集中视频解决方案
		NODE_TYPE_NVMS_PRODUCT_NVMS8000=0x1280,//NVMS8000车载分布式集中视频解决方案
		NODE_TYPE_NVMS_PRODUCT_END = 0x12FF,//客户端结束范围

		NODE_TYPE_OTHER_BEGIN = 0x1300,
		NODE_TYPE_MEDIA_SERVER, //独立运行的流媒体服务器 
		NODE_TYPE_NET_KEYBOARD, //KEYBOARD  
		NODE_TYPE_OTHER_END = 0x13ff,

		NODE_TYPE_PCNVR_BEGIN = 0x1400,
		NODE_TYPE_PCNVR_9000, //PC式NVR
		NODE_TYPE_PCNVR_END = 0x14ff,

		NODE_TYPE_WIFIPROBE_TYPE_BEGIN = 0x1500,
		NODE_TYPE_WIFIPROBE_TYPE,
		NODE_TYPE_WIFIPROBE_TYPE_END = 0x15FF,

		NODE_TYPE_END,
	};

public:
	typedef enum _ecms_node_type_def_    ECMS_NODE_TYPE_DEF;


	class CPUTypeCheck
	{
	public:
		CPUTypeCheck();
		explicit CPUTypeCheck(tuint32 dwTypeValue);
		~CPUTypeCheck();
		static void GetRange(tuint32 &dwTypeValueStart,tuint32 &dwTypeValueEnd)
		{
			dwTypeValueStart=NODE_TYPE_PERIPHERAL_DEVICE_BEGIN;
			dwTypeValueEnd=NODE_TYPE_PERIPHERAL_DEVICE_END;
		}

		////////////是否为同为的抽象标准设备
		bool IsTWAbstractDevice()
		{
			return (m_dwTypeValue == GetTWAbstractDevice());
		}


		////////////是否为同为的IPC
		bool IsTWIPC()
		{
			return (m_dwTypeValue == GetTWIPCTypeValue());
		}

		////////////是否为同为的DVR
		bool IsTWDVR()
		{
			return (m_dwTypeValue == GetTWDVRTypeValue());
		}
		bool IsHuaAnIPC()
		{
			return (m_dwTypeValue == GetHUAANIPCTypeValue());
		}

		bool IsTWMobileDVR()
		{
			return (m_dwTypeValue == GetTWMobileDVRTypeValue());
		}

		///////////////是否为海康的DVR
		bool IsHiKVisionDVR()
		{
			return (m_dwTypeValue == GetHiKVisionDVRTypeValue());
		}
		///////////////是否为大华的DVR
		bool IsDaHuaDVR()
		{
			return (m_dwTypeValue == GetDaHuaDVRTypeValue());
		}

        bool IsZenoBox()
        {
            return (m_dwTypeValue == GetZenoBoxTypeValue());
        }

		bool IsN9000NVR(void)
		{
			return (m_dwTypeValue == GetN9000NVRTypeValue());
		}

		bool IsN9000DVR(void)
		{
			return (m_dwTypeValue == GetN9000DVRTypeValue());
		}

		//SDK 2.0 
		bool IsONVIF(void)
		{
			return (m_dwTypeValue == GetONVIFTypeValue());
		}

		bool IsGB(void)
		{
			return (m_dwTypeValue == GetGBDeviceTypeValue());
		}

		bool IsRTSP(void)
		{
			return (m_dwTypeValue == GetRTSPTypeValue());
		}

		bool Is4GIPC()
		{
			return (m_dwTypeValue == Get4GIPCTypeValue());
		}

		bool IsHuaAnAutoReportIPC()
		{
			return (m_dwTypeValue == GetHuaAnAutoReportTypeValue());
		}

		bool IsThinkForcePad()
		{
			return (m_dwTypeValue == GetThinkForceTypeValue());
		}

		bool IsYITU()
		{
			return (m_dwTypeValue == GetYITUTypeValue());
		}

		bool IsYITUPad()
		{
			return (m_dwTypeValue == GetYITUPadTypeValue());
		}

		bool IsAOPU()
		{
			return (m_dwTypeValue == GetAOPUTypeValue());
		}

		bool IsPrIPC()
		{
			return (m_dwTypeValue == GetPrIPCTypeValue());
		}

		bool IsSensetimePadAutoReport()
		{
			return (m_dwTypeValue == GetSenseTimePadAutoReportTypeValue());
		}

		bool IsEdgeBox()
		{
			return (m_dwTypeValue == GetEdgeBoxTypeValue());
		}
		//////////////////////////获取支持的设备的类型
		static tuint32 GetTWAbstractDevice() ;
		static tuint32 GetTWIPCTypeValue() ;
		static tuint32 GetTWDVRTypeValue() ;
		static tuint32 GetHUAANIPCTypeValue() ;
		static tuint32 GetTWMobileDVRTypeValue() ;
		static tuint32 GetHiKVisionDVRTypeValue();
		static tuint32 GetDaHuaDVRTypeValue();
		static tuint32 GetN9000NVRTypeValue();
		static tuint32 GetONVIFTypeValue();
		static tuint32 GetN9000DVRTypeValue();
		static tuint32 GetAnalogChannelTypeValue();
		static tuint32 GetZenoBoxTypeValue();
		static tuint32 GetGBDeviceTypeValue();
		static tuint32 GetRTSPTypeValue();
		static tuint32 Get4GIPCTypeValue();
		static tuint32 GetThinkForceTypeValue();
		static tuint32 GetHuaAnAutoReportTypeValue();
        static tuint32 GetSenseTimePadAutoReportTypeValue();
		static tuint32 GetYITUTypeValue();
		static tuint32 GetYITUPadTypeValue();
		static tuint32 GetAOPUTypeValue();
        static tuint32 GetEdgeBoxTypeValue();
		//卡扣IPC
		static tuint32 GetPrIPCTypeValue();
		////////////申明为友缘类
		friend class CTypeCheck;
	protected:
		tuint32 m_dwTypeValue;
	};

	class CDecoderTypeCheck
	{
	public:
		CDecoderTypeCheck();
		explicit CDecoderTypeCheck(tuint32 dwTypeValue);
		~CDecoderTypeCheck();
		static void GetRange(tuint32 &dwTypeValueStart,tuint32 &dwTypeValueEnd)
		{
			dwTypeValueStart=NODE_TYPE_PERIPHERAL_DECODER_BEGIN;
			dwTypeValueEnd=NODE_TYPE_PERIPHERAL_DECODER_END;
		}


		////////////申明为友缘类
		friend class CTypeCheck;
	protected:
		tuint32 m_dwTypeValue;
	};
	
	class CAlarmHostTypeCheck
	{
	public:
		CAlarmHostTypeCheck();
		explicit CAlarmHostTypeCheck(tuint32 dwTypeValue);
		~CAlarmHostTypeCheck();
		static void GetRange(tuint32 &dwTypeValueStart,tuint32 &dwTypeValueEnd)
		{
			dwTypeValueStart=NODE_TYPE_PERIPHERAL_ALARMHOST_BEGIN;
			dwTypeValueEnd=NODE_TYPE_PERIPHERAL_ALARMHOST_END;
		}

		////////////申明为友缘类
		friend class CTypeCheck;

		////////////是否为海康网络模块
		bool IsHKNetModel()
		{
			return (m_dwTypeValue == GetHIKNetModuleTypeValue());
		}

		bool IsXKMNetModel()
		{
			return (m_dwTypeValue == GetXKMAlarmHostTypeValue());
		}

		bool IsAleanNetModel()
		{
			return (m_dwTypeValue == GetAleanAlarmHostTypeValue());
		}

		bool IsDscPC4401NetModel()
		{
			return (m_dwTypeValue == GetDscPC4401AlarmHostTypeValue());
		}

		bool IsParadoxNetModel()
		{
			return (m_dwTypeValue == GetParadoxAlarmHostTypeValue());
		}

		bool IsBosch7400NetModel()
		{
			return (m_dwTypeValue == GetBosch7400AlarmHostTypeValue());
		}
		
		static tuint32 GetHIKNetModuleTypeValue();
		static tuint32 GetXKMAlarmHostTypeValue();
		static tuint32 GetAleanAlarmHostTypeValue();
		static tuint32 GetDscPC4401AlarmHostTypeValue();
		static tuint32 GetParadoxAlarmHostTypeValue();
		static tuint32 GetBosch7400AlarmHostTypeValue();
		
	protected:
		tuint32 m_dwTypeValue;
	};

	class CAlarmInTypeCheck
	{
	public:
		CAlarmInTypeCheck();
		explicit CAlarmInTypeCheck(tuint32 dwTypeValue);
		~CAlarmInTypeCheck();
		static void GetRange(tuint32 &dwTypeValueStart,tuint32 &dwTypeValueEnd)
		{
			dwTypeValueStart=NODE_TYPE_PERIPHERAL_ALARMIN_BEGIN;
			dwTypeValueEnd=NODE_TYPE_PERIPHERAL_ALARMIN_END;
		}
		////是否为设备带的Sensor
		bool IsDevSensor()
		{
			return (m_dwTypeValue == GetDevSensorTypeValue());
		}
        bool IsLocalAlarmIn()
        {
            return m_dwTypeValue == GetLocalAlarmInTypeValue() ? true : false;
        }

		//////////////////////////获取支持的设备的类型
		static _ecms_node_type_def_ GetDevSensorTypeValue(){return NODE_TYPE_ALARMIN_DEVICE_SENSOR;} ;
        static _ecms_node_type_def_ GetLocalAlarmInTypeValue(){return NODE_TYPE_ALARMIN_LOCAL_SENSOR;} ;


		////////////申明为友缘类
		friend class CTypeCheck;
	protected:
		tuint32 m_dwTypeValue;
	};


	class CAlarmOutTypeCheck
	{
	public:
		CAlarmOutTypeCheck();
		explicit CAlarmOutTypeCheck(tuint32 dwTypeValue);
		~CAlarmOutTypeCheck();
		static void GetRange(tuint32 &dwTypeValueStart,tuint32 &dwTypeValueEnd)
		{
			dwTypeValueStart=NODE_TYPE_PERIPHERAL_ALARMOUT_BEGIN;
			dwTypeValueEnd=NODE_TYPE_PERIPHERAL_ALARMOUT_END;
		}
        ////
		bool IsDevAlarmOut()
		{
			return (m_dwTypeValue == GetDevAlarmOutTypeValue());
		}
        bool IsLocalAlarmOut()
        {
            return m_dwTypeValue == GetLocalAlarmOutTypeValue() ? true : false;
        }

        //////////////////////////
		static _ecms_node_type_def_ GetDevAlarmOutTypeValue(){return NODE_TYPE_ALARMOUT_DEVICE_ALARMOUT;} ;
        static _ecms_node_type_def_ GetLocalAlarmOutTypeValue(){return NODE_TYPE_ALARMOUT_LOCAL_ALARMOUT;} ;

		////////////申明为友缘类
		friend class CTypeCheck;
	protected:
		tuint32 m_dwTypeValue;
	};


    class CProtocolTypeCheck
    {
        ////////////申明为友元类
        friend class CTypeCheck;
    public:
        CProtocolTypeCheck();
        explicit CProtocolTypeCheck(tuint32 dwProtocolTypeValue);
        ~CProtocolTypeCheck();

        ////////////同为的IPC  SDK
        bool IsTWIPC_SDK()
        {
            return (m_dwProtocolTypeValue == GetTWIPC_SDK_ProtocolTypeValue());
        }

        ////////////同为的DVR  SDK
        bool IsTWDVR_SDK()
        {
            return (m_dwProtocolTypeValue == GetTWDVR_SDK_Protocol_TypeValue());
        }
        bool IsHuaAnIPC_SDK()
        {
            return (m_dwProtocolTypeValue == GetHuaAnIPC_SDK_Protocol_TypeValue());
        }

        bool IsTWMobileDVR_SDK()
        {
            return (m_dwProtocolTypeValue == GetTWMobileDVR_SDK_Protocol_TypeValue());
        }

        ///////////////海康SDK
        bool IsHiKVision_SDK()
        {
            return (m_dwProtocolTypeValue == GetHiKVision_SDK_Protocol_TypeValue());
        }
        ///////////////大华 SDK
        bool IsDaHua_SDK()
        {
            return (m_dwProtocolTypeValue == GetDaHua_SDK_Protocol_TypeValue());
        }

        ///////////////智诺 SDK
        bool IsZeno_SDK()
        {
            return (m_dwProtocolTypeValue == GetZeno_SDK_Protocol_TypeValue());
        }

		///////////////ONVIF协议
		bool IsONVIF()
		{
			return (m_dwProtocolTypeValue == GetONVIF_Protocol_TypeValue());
		}

		bool IsN9000NVR(void)
		{
			return (m_dwProtocolTypeValue == GetN9000NVR_Protocol_TypeValue());
		}

		bool IsN9000DVR(void)
		{
			return (m_dwProtocolTypeValue == GetN9000DVR_Protocol_TypeValue());
		}

		bool IsAnalogChannel(void)
		{
			return (m_dwProtocolTypeValue == GetAnalogChannel_Protocol_TypeValue());
		}

		bool IsGBDevice(void)
		{
			return (m_dwProtocolTypeValue == GetGB_Protocol_TypeValue());
		}

		bool IsRTSP(void)
		{
			return (m_dwProtocolTypeValue == GetRTSP_Protocol_TypeValue());
		}

		bool Is4GIPC_SDK(void)
		{
			return (m_dwProtocolTypeValue == Get4GIPC_SDK_Protocol_TypeValue());
		}
		
		bool IsHuaAnAutoReport_SDK(void)
		{
			return (m_dwProtocolTypeValue == GetHuaAnAutoReport_SDK_Protocol_TypeValue());
		}
        
        bool IsSenseTimePadAutoReport_SDK(void)
        {
            return (m_dwProtocolTypeValue == GetSenseTimePadAutoReport_SDK_Protocol_TypeValue());
        }

		bool IsThinkForce_SDK()
		{
			return (m_dwProtocolTypeValue == GetThinkForce_SDK_Protocol_TypeValue());
		}

		bool IsYITU()
		{
			return (m_dwProtocolTypeValue == GetYITU_Protocol_TypeValue());
		}

        bool IsYITUPad()
        {
            return (m_dwProtocolTypeValue == GetYITUPad_Protocol_TypeValue());
        }

		bool IsAOPU()
		{
			return (m_dwProtocolTypeValue == GetAOPU_Protocol_TypeValue());
		}

		bool IsPR_IPC_SDK(void)
		{
			return (m_dwProtocolTypeValue == GetPrIPC_SDK_Protocol_TypeValue());
		}
		
		bool IsEdgeBox()
		{
			return (m_dwProtocolTypeValue == GetEdgeBox_Protocol_TypeValue());
		}
        //////////////////////////
        static tuint32 GetTWIPC_SDK_ProtocolTypeValue() ;
        static tuint32 GetTWDVR_SDK_Protocol_TypeValue() ;
        static tuint32 GetHuaAnIPC_SDK_Protocol_TypeValue() ;
        static tuint32 GetTWMobileDVR_SDK_Protocol_TypeValue() ;

        static tuint32 GetHiKVision_SDK_Protocol_TypeValue();
        static tuint32 GetDaHua_SDK_Protocol_TypeValue();
		static tuint32 GetZeno_SDK_Protocol_TypeValue();
        static tuint32 GetThinkForce_SDK_Protocol_TypeValue();

        static tuint32 GetONVIF_Protocol_TypeValue();
		static tuint32 GetN9000NVR_Protocol_TypeValue(void);
		static tuint32 GetN9000DVR_Protocol_TypeValue(void);

		static tuint32 GetAnalogChannel_Protocol_TypeValue(void);
		static tuint32 GetGB_Protocol_TypeValue(void);
		static tuint32 GetRTSP_Protocol_TypeValue();
		static tuint32 Get4GIPC_SDK_Protocol_TypeValue();
		static tuint32 GetHuaAnAutoReport_SDK_Protocol_TypeValue();
        static tuint32 GetSenseTimePadAutoReport_SDK_Protocol_TypeValue();
		static tuint32 GetYITU_Protocol_TypeValue();
        static tuint32 GetYITUPad_Protocol_TypeValue();
		static tuint32 GetAOPU_Protocol_TypeValue();
		static tuint32 GetPrIPC_SDK_Protocol_TypeValue();
        static tuint32 GetEdgeBox_Protocol_TypeValue();
    protected:
        tuint32 m_dwProtocolTypeValue;
    };

	class CEmapFileTypeCheck
	{
		////////////申明为友元类
		friend class CTypeCheck;
	public:
		CEmapFileTypeCheck();
		explicit CEmapFileTypeCheck(tuint32 dwEmapFileTypeValue);
		~CEmapFileTypeCheck();

		////////////静态矢量图
		bool IsPicture_Emap()
		{
			return (m_dwTypeValue == GetPictue_EmapTypeValue());
		}

		////////////百度
		bool IsBaidu_Emap()
		{
			return (m_dwTypeValue == GetBaidu_EmapTypeValue());
		}

		bool IsGoogle_Emap()
		{
			return (m_dwTypeValue == GetGoogle_EmapTypeValue());
		}

		bool IsGaode_Emap()
		{
			return (m_dwTypeValue == GetGaode_EmapTypeValue());
		}

		bool IsWapian_Emap()
		{
			return (m_dwTypeValue == GetWapian_EmapTypeValue());
		}
		//////////////////////////
		static tuint32 GetPictue_EmapTypeValue() ;
		static tuint32 GetBaidu_EmapTypeValue() ;
		static tuint32 GetGoogle_EmapTypeValue() ;
		static tuint32 GetGaode_EmapTypeValue() ;
		static tuint32 GetWapian_EmapTypeValue();

	protected:
		tuint32 m_dwTypeValue;
	};



	class CAcsSystemTypeCheck
	{
	public:
		CAcsSystemTypeCheck();
		explicit CAcsSystemTypeCheck(tuint32 dwTypeValue);
		~CAcsSystemTypeCheck();
		static void GetRange(tuint32 &dwTypeValueStart,tuint32 &dwTypeValueEnd)
		{
			dwTypeValueStart=NODE_TYPE_ACSSYSTEM_TYPE_BEGIN;
			dwTypeValueEnd=NODE_TYPE_ACSSYSTEM_TYPE_END;
		}

		////////////是否为的抽象第三方门禁考勤系统
		bool IsAbstractAcsSystem()
		{
			return (m_dwTypeValue == GetAbstractAcsSystem());
		}


		////////////是否为中控智慧平台
		bool IsZktEcoAcsSystem()
		{
			return (m_dwTypeValue == GetZktEcoAcsSystemTypeValue());
		}
		////////////是否为中联创新平台
		bool IsZLCXAcsSystem()
		{
			return (m_dwTypeValue == GetZLCXAcsSystemTypeValue());
		}
		////////////是否为深圳微耕平台
		bool IsSZWGAcsSystem()
		{
			return (m_dwTypeValue == GetSZWGAcsSystemTypeValue());
		}
		////////////是否为KANTECH
		bool IsKANTECHAcsSystem()
		{
			return (m_dwTypeValue == GetKANTECHAcsSystemTypeValue());
		}
		////////////是否为AOPU
		bool IsAOPUAcsSystem()
		{
			return (m_dwTypeValue == GetAOPUAcsSystemTypeValue());
		}

		//////////////////////////获取支持的设备的类型
		static tuint32 GetAbstractAcsSystem() ;
		static tuint32 GetZktEcoAcsSystemTypeValue() ;
		static tuint32 GetZLCXAcsSystemTypeValue();
		static tuint32 GetSZWGAcsSystemTypeValue();
		static tuint32 GetKANTECHAcsSystemTypeValue();
		static tuint32 GetAOPUAcsSystemTypeValue();
		////////////申明为友缘类
		friend class CTypeCheck;
	protected:
		tuint32 m_dwTypeValue;
	};

	class CFkTypeCheck
	{
	public:
		CFkTypeCheck();
		explicit CFkTypeCheck(tuint32 dwTypeValue);
		~CFkTypeCheck();
		static void GetRange(tuint32 &dwTypeValueStart,tuint32 &dwTypeValueEnd)
		{
			dwTypeValueStart=NODE_TYPE_FK_TYPE_BEGIN;
			dwTypeValueEnd=NODE_TYPE_FK_TYPE_END;
		}

		////////////是否为钱林访客机
		bool IsQianLinFk()
		{
			return (m_dwTypeValue == GetQianLinValue());
		}


		//////////////////////////获取支持的设备的类型
		static tuint32 GetQianLinValue() ;
		static tuint32 GetDepartmentValue();

		//申明为友缘类
		friend class CTypeCheck;
	protected:
		tuint32 m_dwTypeValue;

	};

	//级联平台
	class CCascadePlatformTypeCheck
	{
	public:
		CCascadePlatformTypeCheck();
		explicit CCascadePlatformTypeCheck(tuint32 dwTypeValue);
		~CCascadePlatformTypeCheck();
		static void GetRange(tuint32 &dwTypeValueStart,tuint32 &dwTypeValueEnd)
		{
			dwTypeValueStart=NODE_TYPE_CASCADE_PLATFORM_TYPE_BEGIN;
			dwTypeValueEnd=NODE_TYPE_CASCADE_PLATFORM_TYPE_END;
		}

		////////////是否为的抽象第三方门禁考勤系统
		bool IsAbstractCascadePlatform()
		{
			return (m_dwTypeValue == GetAbstractCascadePlatform());
		}


		////////////是否为中控智慧平台
		bool IsGB28181CascadePlatform()
		{
			return (m_dwTypeValue == GetGB28181CascadePlatformTypeValue());
		}

		//////////////////////////获取支持的设备的类型
		static tuint32 GetAbstractCascadePlatform() ;
		static tuint32 GetGB28181CascadePlatformTypeValue() ;

		////////////申明为友缘类
		friend class CTypeCheck;
	protected:
		tuint32 m_dwTypeValue;
	};



	class CPmsLedTypeCheck
	{
	public:
		CPmsLedTypeCheck();
		explicit CPmsLedTypeCheck(tuint32 dwTypeValue);
		~CPmsLedTypeCheck();
		static void GetRange(tuint32 &dwTypeValueStart,tuint32 &dwTypeValueEnd)
		{
			dwTypeValueStart=NODE_TYPE_SCREEN_LED_BEGIN;
			dwTypeValueEnd=NODE_TYPE_SCREEN_LED_END;
		}

		////////////是否为的抽象
		bool IsAbstractPmsLed()
		{
			return (m_dwTypeValue == GetPmsLedAbstractTypeValue());
		}


		////////////是否为IP红门屏幕
		bool IsIPHongMenLed()
		{
			return (m_dwTypeValue == GetIPHongMenLedTypeValue());
		}
		////////////是否为车易付485屏
		bool IsCheYiFuLed()
		{
			return (m_dwTypeValue == GetCheYiFuTypeValue());
		}
        ////////////是否为XCP10型号LED屏
        bool IsXCP10Led()
        {
            return (m_dwTypeValue == GetXCP10LedTypeValue());
        }
		//////////////////////////获取支持的设备的类型
		static tuint32 GetAbstractPmsLed() ;
		static tuint32 GetIPHongMenLedTypeValue();
		static tuint32 GetCheYiFuTypeValue();
        static tuint32 GetXCP10LedTypeValue();

		////////////申明为友缘类
		friend class CTypeCheck;
	protected:
		tuint32 m_dwTypeValue;
	};


	class CWifiProbeTypeCheck
	{
		friend class CTypeCheck;

	public:
		CWifiProbeTypeCheck();
		explicit CWifiProbeTypeCheck(tuint32 dwTypeValue);
		~CWifiProbeTypeCheck();

	public:
		static void GetRange(tuint32 &dwTypeValueStart,tuint32 &dwTypeValueEnd)
		{
			dwTypeValueStart=NODE_TYPE_WIFIPROBE_TYPE_BEGIN;
			dwTypeValueEnd=NODE_TYPE_WIFIPROBE_TYPE_END;
		}

		static tuint32 GetWifiProbeValue();

	protected:
		tuint32 m_dwTypeValue;
	};



	explicit CTypeCheck(tuint32 dwType);
	~CTypeCheck(void);/////////没有必要定义为虚函数  

	//是否为媒体存储单元
	bool IsMSU(void) const;

	//是否为GPS存储单元
	bool IsGSU(void) const;

	//是否为媒体转发单元
	bool IsMDU(void) const;

	//是否为独立流媒体服务器
	bool IsMediaServer(void) const;
	//是否为独立流媒体服务器
	bool IsPCNVR9000(void) const;

	//是否为注册服务器单元
	bool IsRSU(void) const;

	//是否为电子地图单元
	bool IsEMU(void) const;

	//是否为报警转发单元
	bool IsADU(void) const;

	//是否为智能分析单元
	bool IsANS(void) const;

	/////////是否为级联网关单元
	bool IsCGU(void) const;

	/////////是否为1400上级网关单元
	bool Is1400IN(void) const;
	/////////是否为应用服务器单元
	bool IsAPPServer(void) const;
	//是否为GPS警情分析单元
	bool IsGPSServer(void) const;

	//是否电视墙服务器    这是服务不是墙，使用时要注意
	bool IsTVW(void) const;

	//是否为墙
	bool IsWall(void) const;

	//是否为通道组
	bool IsChannelGroup(void) const; //通道组

	//是否为告警任务组
	bool IsAlarmTaskGroup(void) const;  //告警任务

	//是否为处警标准操作指导
	bool IsSop(void) const; //处警标准操作指导
	////是否为收件人信息
	bool IsEmail(void) const;
	
	//是否接入服务器
	bool IsJSU(void) const;

	//是否状态服务器
	bool IsSTU(void) const;

	//是否为N5000的服务器
	bool IsN5000Server() const;

	/////////是否为前端设备单元
	bool IsPU(CPUTypeCheck *pPUTypeCheck = NULL) const;
	/////////是否为解码设备单元 解码器
	bool IsDecoder(CDecoderTypeCheck *pDecoderTypeCheck = NULL) const;
	/////////是否为目标库
	bool IsAlbum(void) const;
	/////////是否为报警主机单元
	bool IsAlarmHost(CAlarmHostTypeCheck *pTypeCheck = NULL) const;
	/////////是否为报警输入
	bool IsAlarmIn(CAlarmInTypeCheck *pAlarmInTypeCheck = NULL) const;
	/////////是否为报警输出
	bool IsAlarmOut(CAlarmOutTypeCheck *pAlarmOutTypeCheck = NULL) const;
	/////////是否为门禁考勤报警系统
	bool IsAcsSystem(CAcsSystemTypeCheck *pTypeCheck = NULL) const;
	/////////是否为访客系统
	bool IsFkSystem(CFkTypeCheck *pTypeCheck = NULL) const;
	//是否为级联平台
	bool IsCascadePlatform(void) const;

	//是否为门禁设备
	bool IsAcsDevice(void) const;
	//是否为门
	bool IsAcsDoor(void) const;
	//是否为读头
	bool IsAcsReader(void) const;

	/////////是否为通道单元
	bool IsChannel(void) const;

	bool IsIPChannel(void) const;

	//是否为子系统
	bool IsSubSystem(void) const;
	//是否为防区
	bool IsZone(void) const;

	/////////是否区域
	bool IsArea(void) const;

	bool IsClient(void) const;

	/////////是否为Monitor单元
	bool IsMCU(void) const;
	/////////是否为网络键盘单元
	bool IsNKB(void) const;

	/////////是否为配置客户端单元
	bool IsCCU(void) const;

	//是否为解码器客户端
	bool IsDCU(void) const;

	/////////是否为手机客户端单元
	bool IsMobile(void) const;

	/////////是否为1400OUT客户端单元
	bool Is1400Out(void) const;

	/////////是否为IE客户端单元
	bool IsIEClient(void) const;
	bool IsTVWClient(void) const;

	bool IsNVMS1000(void) const;

	bool IsNVMS5000(void) const;

	//是否是角色
	bool IsUserGroup(void) const;

	//是否是电子地图
	bool IsEmapFile(void) const;
	//是否是热区
	bool IsHotArea(void) const;
	//是否是热点
	bool IsHotSpot(void) const;
	//是否是时间段帧率模板
	bool IsTimeSecRate(void) const;
	//是否为分区组
	bool IsPartitionGroup(void) const;

	//是否为分组
	bool IsPartition(void) const;
	//是否为系统配置
	bool IsConfig(void) const;
	bool IsCustom(void) const;
	//是否为Wifi探针
	bool IsWifiProbe()const;

	//pms
	bool IsPmsPark(void) const;//停车场
	bool IsPmsGateway(void) const;//出入口
	bool IsPmsLane(void) const;//车道
	bool IsPmsLed(void) const;//显示屏
	bool IsPmsChargeRule(void) const;//收费规则
	bool IsPmsPassRecord(void) const; //通行记录
	bool IsPmsFixedVehicle(void) const;//固定车辆
	bool IsPmsTimeSectionChargeRule(void) const;//自定义收费时间段
	bool IsPmsBagPeriod(void) const;//月卡包月收费
	bool IsPmsVehicleGroup(void) const;//车辆组
	//是否为报警输出组
	bool IsAlarmOutGroup(void) const; //报警输出组
    bool IsLocalAlarmIn() const;
    bool IsLocalAlarmOut() const;

	//得到无效节点类型值
	inline static _ecms_node_type_def_ GetInvalidTypeValue(){return NODE_TYPE_INVALID_NULL;} ;

	//////////////获取存储服务器的类型值
	static _ecms_node_type_def_ GetMSUTypeValue() {return NODE_TYPE_STORAGE_SERVER; };

	//////////////获取GPS存储服务器的类型值
	static _ecms_node_type_def_ GetGSUTypeValue() {return NODE_TYPE_GSU_SERVER; };

	//////////////获取转发服务器的类型值
	static  _ecms_node_type_def_ GetMDUTypeValue() {return NODE_TYPE_MEDIA_TRANSFER_SERVER; };

	//////////////获取独立流媒体服务器的类型值
	static  _ecms_node_type_def_ GetMediaServerTypeValue() {return NODE_TYPE_MEDIA_SERVER; };

	//////////////获取独立流媒体服务器的类型值
	static  _ecms_node_type_def_ GetPCNVR9000TypeValue() {return NODE_TYPE_PCNVR_9000; };

	//////////////获取网络键盘的类型值
	static _ecms_node_type_def_ GetNetkeyboardTypeValue() {return NODE_TYPE_NET_KEYBOARD;};

	//////////////获取认证服务器的类型值
	static _ecms_node_type_def_ GetRSUTypeValue(){return NODE_TYPE_AUTHENTICATION_SERVER; } ;

	//////////////获取报警服务器的类型值
	static _ecms_node_type_def_ GetADUTypeValue(){return NODE_TYPE_ALARM_SERVER;  } ;

	//////////////获取智能分析服务器的类型值
	static _ecms_node_type_def_ GetANSTypeValue(){return NODE_TYPE_ANALYSIS_SERVER;  } ;

//////////////获取级联网关服务器的类型值
	static _ecms_node_type_def_ GetCGUTypeValue(){return NODE_TYPE_CASCADE_GATEWAY_SERVER;  } ;
	
	//////////////获取1400上级网关服务器的类型值
	static _ecms_node_type_def_ Get1400INTypeValue(){return NODE_TYPE_1400IN_SERVER;  } ;

	//////////////获取GPS警情分析服务器的类型值
	static _ecms_node_type_def_ GetGPSServerTypeValue(){return NODE_TYPE_GPS_SERVER;  } ;

	//////////////获取电视墙服务器的类型值
	static _ecms_node_type_def_ GetTVWTypeValue(){return NODE_TYPE_TVWALL_SERVER; } ;

	//////////////获取电视墙(墙，不是服务)的类型值
	static _ecms_node_type_def_ GetWallTypeValue(){return NODE_TYPE_WALL; } ;

	static _ecms_node_type_def_ GetApplicationTypeValue(){return NODE_TYPE_APPLICATION_SERVER;};

	//////////////获取Monitor客户端的类型值
	static _ecms_node_type_def_ GetMCUTypeValue(){return NODE_TYPE_CLIENT_MONITOR; } ;

	//////////////获取解码器客户端的类型值
	static _ecms_node_type_def_ GetDecoderClientTypeValue(){return NODE_TYPE_CLIENT_DECODER; } ;

	//////////////获取目标库的类型值
	static _ecms_node_type_def_ GetAlbumTypeValue(){return NODE_TYPE_ALBUM; } ;

	//////////////获取配置客户端的类型值
	static _ecms_node_type_def_ GetCCUTypeValue(){return NODE_TYPE_CLIENT_CONFIG;  } ;

	//////////////获取1400OUT客户端的类型值
	static _ecms_node_type_def_ Get1400OUTTypeValue(){return NODE_TYPE_CLIENT_1400OUT; } ;

	//////////////获取通道的类型值
	static _ecms_node_type_def_ GetChnnTypeValue(){return NODE_TYPE_CHANNEL;} ;

	//////////////获取子系统的类型值
	static _ecms_node_type_def_ GetSubSystemTypeValue(){return NODE_TYPE_SUBSYSTEM;} ;

	//////////////获取防区的类型值
	static _ecms_node_type_def_ GetZoneTypeValue(){return NODE_TYPE_ZONE;} ;

	//////////////获取门禁设备的类型值
	static _ecms_node_type_def_ GetAcsDeviceTypeValue(){return NODE_TYPE_ACSDEVICE;} ;
	//////////////获取门的类型值
	static _ecms_node_type_def_ GetAcsDoorTypeValue(){return NODE_TYPE_ACSDOOR;} ;
	//////////////获取读头的类型值
	static _ecms_node_type_def_ GetAcsReaderTypeValue(){return NODE_TYPE_ACSREADER;} ;
	//////////////获取系统配置的类型值
	static _ecms_node_type_def_ GetConfigTypeValue(){return NODE_TYPE_CONFIG;} ;
	//////////////获取电子地图的类型值
	static _ecms_node_type_def_ GetEmapAbstractTypeValue(){return NODE_TYPE_EMAP_TYPE_ABSTRACT;} ;
	//////////////获取热区的类型值
	static _ecms_node_type_def_ GetHotAreaTypeValue(){return NODE_TYPE_HOT_AREA;} ;
	//////////////获取热点的类型值
	static _ecms_node_type_def_ GetHotSpotTypeValue(){return NODE_TYPE_HOT_SPOT;} ;


	//获取IP通道的类型值
	static _ecms_node_type_def_ GetIPChannelTypeValue(){return NODE_TYPE_IP_CHANNEL;} ;

	//////////////获取电子地图服务器的类型值
	static _ecms_node_type_def_ GetEMUTypeValue(void){return NODE_TYPE_EMAP_SERVER;  } ;

	//获取手机客户端的类型值
	static _ecms_node_type_def_ GetMobileTypeValue(void){return NODE_TYPE_CLIENT_MOBILE;  };

	//获取IE客户端的类型值
	static _ecms_node_type_def_ GetIEClientTypeValue(void){return NODE_TYPE_CLIENT_IE;  } ;
	//获取电视墙客户端的类型值
	static _ecms_node_type_def_ GetTVWallClientTypeValue(void){return NODE_TYPE_CLIENT_TVWALL;  } ;

	//获取区域的类型值
	static _ecms_node_type_def_ GetAreaTypeValue(void) {return NODE_TYPE_AREA;};

	//获取接入服务器的类型值
	static _ecms_node_type_def_ GetJSUTypeValue(void){return NODE_TYPE_JOIN_SERVER;};

	//获取状态服务器的类型值
	static _ecms_node_type_def_ GetSTUTypeValue(void){return NODE_TYPE_STATUS_SERVER;};

	//获取服务器的起始类型值
	static _ecms_node_type_def_ GetServerTypeBeginValue(void){return NODE_TYPE_NVMS5000_SERVER_BEGIN;};

	//获取PU类型值  此PU类型值代表抽象的设备类型  非具体设备
	static _ecms_node_type_def_ GetPUAbstractTypeValue(void){return NODE_TYPE_PERIPHERAL_ABSTRACT_DEVICE;};
	//获取AlarmIn类型值  此AlarmIn类型值代表抽象的设备类型  非具体
	static _ecms_node_type_def_ GetAlarmInAbstractTypeValue(void){return NODE_TYPE_PERIPHERAL_ABSTRACT_ALARMIN;};
	//获取AlarmOut类型值  此AlarmOut类型值代表抽象的设备类型  非具体
	static _ecms_node_type_def_ GetAlarmOutAbstractTypeValue(void){return NODE_TYPE_PERIPHERAL_ABSTRACT_ALARMOUT;};
	//获取防区探测器类型值  此抽象的探测器类型  非具体
	static _ecms_node_type_def_ GetZoneDetectorAbstractTypeValue(void){return NODE_TYPE_PERIPHERAL_ABSTRACT_ZONE_DETECTOR;};
	//获取门禁考勤类型值  此代表抽象的门禁考勤系统 非具体
	static _ecms_node_type_def_ GetAcsSystemAbstractTypeValue(void){return NODE_TYPE_ACSSYSTEM_TYPE_ABSTRACT;};

	//获取访客类型值 非具体
	static _ecms_node_type_def_ GetFkAbstractTypeValue(void){return NODE_TYPE_FK_TYPE_ABSTRACT;};

	static _ecms_node_type_def_ GetNVMS1000TypeValue(void){return NODE_TYPE_NVMS_PRODUCT_NVMS1000;};

	static _ecms_node_type_def_ GetNVMS5000TypeValue(void){return NODE_TYPE_NVMS_PRODUCT_NVMS5000;};

	//获取DECODER类型值  此DECODER类型值代表抽象的解码设备类型  
	static _ecms_node_type_def_ GetDECODERAbstractTypeValue(void){return NODE_TYPE_PERIPHERAL_ABSTRACT_DECODER;};
	//获取ALARMHOST类型值  报警主机值代表抽象的设备类型  非具体设备
	static _ecms_node_type_def_ GetAlarmHostAbstractTypeValue(void){return NODE_TYPE_PERIPHERAL_ABSTRACT_ALARMHOST;};
	//获取通道组类型值
	static _ecms_node_type_def_ GetChannelGroupValue(void){return NODE_TYPE_CHANNELGROUP;};

	//获取报警输出组类型值
	static _ecms_node_type_def_ GetAlarmOutGroupValue(void){return NODE_TYPE_ALARMOUTGROUP;};

	//获取处警标准操作类型值
	static _ecms_node_type_def_ GetSopValue(void){return NODE_TYPE_SOP;};

	//获取邮箱类型值
	static _ecms_node_type_def_ GetEmailTypeValue(void){return NODE_TYPE_EMAIL;};
	

	//获取角色类型值
	static _ecms_node_type_def_ GetUserGroupTypeValue(void){return NODE_TYPE_USERGROUP;};
	//////////////获取分区组类型值
	static _ecms_node_type_def_ GetPartitionGroupTypeValue() {return NODE_TYPE_PARTITION_GROUP; }
	//////////////获取分区类型值
	static _ecms_node_type_def_ GetPartitionTypeValue() {return NODE_TYPE_PARTITION; }

	//获取告警任务组类型值
	static _ecms_node_type_def_ GetAlarmTaskTypeValue(void){return NODE_TYPE_ALARMTASK; };

	//获取停车场类型值
	static _ecms_node_type_def_ GetPmsParkTypeValue() {return NODE_TYPE_PMSPARK; }
	//获取出入口类型值
	static _ecms_node_type_def_ GetPmsGatewayTypeValue() {return NODE_TYPE_PMSGATEWAY; }
	//获取车道类型值
	static _ecms_node_type_def_ GetPmsLaneTypeValue() {return NODE_TYPE_PMSLANE; }
	//获取显示屏类型值
	static _ecms_node_type_def_ GetPmsLedAbstractTypeValue() {return NODE_TYPE_SCREEN_LED_ABSTRACT_DEVICE; }
	//获取收费规则类型值
	static _ecms_node_type_def_ GetPmsChargeRuleTypeValue() {return NODE_TYPE_PMSCHARGE_RULE; }
	//获取通行记录类型值
	static _ecms_node_type_def_ GetPmsPassRecordTypeValue() {return NODE_TYPE_PMSRECORDPASS; }	
	//获取固定车辆类型值
	static _ecms_node_type_def_ GetPmsFixedVehicleTypeValue() {return NODE_TYPE_PMSFIXED_VEHICLE; }
	//获取黑名单车辆类型值
	static _ecms_node_type_def_ GetPmsBlacklistVehicleTypeValue() {return NODE_TYPE_PMS_BACKLIST_VEHICLE; }
	//获取车辆组类型值
	static _ecms_node_type_def_ GetPmsVehicleGroupTypeValue() {return NODE_TYPE_PMS_VEHICLE_GROUP; }
	//获取自定义收费时间段类型值
	static _ecms_node_type_def_ GetPmsTimeSectionChargeRuleTypeValue() {return NODE_TYPE_PMSTIME_SECTION_CHARGE_RULE; }
	//获取月卡包月收费类型值
	static _ecms_node_type_def_ GetPmsBagPeriodTypeValue() {return NODE_TYPE_PMSBAG_PERIOD; }
	//获取时间段帧率模板
	static _ecms_node_type_def_ GetTimeSecRateTypeValue() {return NODE_TYPE_TIME_SECTION_FRAMERATE; }
	//级联平台类型值
	static _ecms_node_type_def_ GetCascadePlatformTypeValue() {return NODE_TYPE_CASCADE_PLATFORM_TYPE_ABSTRACT; }

	//自定义类型
	static _ecms_node_type_def_ GetCustomTypeValue() {return NODE_TYPE_CUSTOM; }

	//获取Wifi探针类型值
	static _ecms_node_type_def_ GeWifiProbeTypeValue() {return NODE_TYPE_WIFIPROBE_TYPE; }

    static _ecms_node_type_def_ GetLocalAlarmInTypeValue() {return NODE_TYPE_ALARMIN_LOCAL_SENSOR; }
    static _ecms_node_type_def_ GetLocalAlarmOutTypeValue() {return NODE_TYPE_ALARMOUT_LOCAL_ALARMOUT; }

	static bool IsInvalidNodeType_s(tuint32 nodeType);
	static bool IsMSU_s(tuint32 nodeType); //存储服务器
	static bool IsGSU_s(tuint32 nodeType); //GPS存储服务器
	static bool IsMDU_s(tuint32 nodeType); //媒体转发服务器
	static bool IsMediaServer_s(tuint32 nodeType); //媒体转发服务器
	static bool IsPCNVR9000_s(tuint32 nodeType); //NVR
	static bool IsRSU_s(tuint32 nodeType); //认证服务器
	static bool IsADU_s(tuint32 nodeType); //报警服务器
	static bool IsANS_s(tuint32 nodeType); //智能分析服务器
	static bool IsAPPS_s(tuint32 nodeType); //应用服务器
	static bool IsCGU_s(tuint32 nodeType);//级联网关
	static bool IsGPSServer_s(tuint32 nodeType); //GPS警情服务器
	static bool IsTVW_s(tuint32 nodeType); //电视墙服务器,这是服务不是墙，使用时要注意
	static bool IsSTU_s(tuint32 nodeType); //状态服务器
	static bool IsJSU_s(tuint32 nodeType); //媒体转发服务器
	static bool IsMCU_s(tuint32 nodeType); //监控客户端
	static bool IsDCU_s(tuint32 nodeType); //解码器客户端
	static bool IsNKB_s(tuint32 nodeType); //网络键盘客户端
	static bool IsCCU_s(tuint32 nodeType); //配置客户端
	static bool Is1400Out_s(tuint32 nodeType); //1400Out
	static bool Is1400In_s(tuint32 nodeType); //1400In
	static bool IsPU_s(tuint32 nodeType, CPUTypeCheck *pPUTypeCheck = NULL); //前端设备
	static bool IsDecoder_s(tuint32 nodeType, CDecoderTypeCheck *pDecoderTypeCheck = NULL);//解码设备 解码器
	static bool IsAlarmIn_s(tuint32 nodeType, CAlarmInTypeCheck *pAlarmInTypeCheck = NULL); //报警输入
	static bool IsAlarmOut_s(tuint32 nodeType, CAlarmOutTypeCheck *pAlarmOutTypeCheck = NULL); //报警输出
	static bool IsWall_s(tuint32 nodeType); //电视墙,墙
	static bool IsChannelGroup_s(tuint32 nodeType); //通道组
	static bool IsAlbum_s(tuint32 nodeType);//目标库
	static bool IsSop_s(tuint32 nodeType); //处警标准操作指导
	static bool IsEmail_s(tuint32 nodeType);//是否为收件人信息
	static bool IsAlarmHost_s(tuint32 nodeType, CAlarmHostTypeCheck *pTypeCheck = NULL);//报警主机
	static bool IsAcsSystem_s(tuint32 nodeType, CAcsSystemTypeCheck *pTypeCheck = NULL); //第三方门禁考勤系统
	static bool IsCascadePlatform_s(tuint32 nodeType);//级联平台
	static bool IsAlarmTask_s(tuint32 nodeType);//告警任务
	static bool IsClient(tuint32 nodeType);//客户端
	static bool IsIEClient_s(tuint32 nodeType);//IE客户端
	static bool IsMobile_s(tuint32 nodeType); //手机客户端
	static bool IsTVWallClient_s(tuint32 nodeType); //
	static bool IsChannel_s(tuint32 nodeType); //通道
	static bool IsIPChannel_s(tuint32 nodeType);//IP通道
	static bool IsEMU_s(tuint32 nodeType); //电子地图服务器
	static bool IsArea_s(tuint32 nodeType); //区域
	static bool IsNVMS1000_s(tuint32 nodeType);
	static bool IsNVMS5000_s(tuint32 nodeType);
	static bool IsUserGroup_s(tuint32 nodeType);
	static bool IsSubSystem_s(tuint32 nodeType);//子系统
	static bool IsZone_s(tuint32 nodeType);//防区
	static bool IsEmapFile_s(tuint32 nodeType);
	static bool IsHotArea_s(tuint32 nodeType);
	static bool IsHotSpot_s(tuint32 nodeType);
	static bool IsPartitionGroup_s(tuint32 nodeType); //分区组
	static bool IsPartition_s(tuint32 nodeType); //分区
	static bool IsAcsDevice_s(tuint32 nodeType); 
	static bool IsAcsDoor_s(tuint32 nodeType);
	static bool IsAcsReader_s(tuint32 nodeType);
	static bool IsConfig_s(tuint32 nodeType);
	static bool IsCustom_s(tuint32 nodeType);
	static bool IsN5000Server_s(tuint32 nodeType);
	static bool IsTimeSecRate_s(tuint32 nodeType);
	static bool IsAlarmOutGroup_s(tuint32 nodeType); //报警输出组

	//pms
	static bool IsPmsPark_s(tuint32 nodeType);//停车场
	static bool IsPmsGateway_s(tuint32 nodeType);//出入口
	static bool IsPmsLane_s(tuint32 nodeType);//车道
	static bool IsPmsLed_s(tuint32 nodeType);//显示屏
	static bool IsPmsChargeRule_s(tuint32 nodeType);//收费规则
	static bool IsPmsFixedVehicle_s(tuint32 nodeType);//固定车辆
	static bool IsPmsTimeSectionChargeRule_s(tuint32 nodeType);//自定义收费时间段
	static bool IsPmsBagPeriod_s(tuint32 nodeType);//月卡包月收费
protected:
	tuint32   m_dwType;
};

#endif
