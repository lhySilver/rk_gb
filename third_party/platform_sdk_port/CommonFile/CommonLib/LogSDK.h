
///////////////////////////NetCommSDK.h

#ifndef _LOG_SDK_H
#define _LOG_SDK_H

#ifdef WIN32
#ifdef LOG_SDK_EXPORT_API//动态库编译
#define _EXPORT_LOG_SDK  __declspec(dllexport)
#else
#ifdef LOG_SDK_EXPORT_STATIC//静态库
#define _EXPORT_LOG_SDK
#else//使用者编译
#define _EXPORT_LOG_SDK __declspec(dllimport)
#endif
#endif
#else
#define _EXPORT_LOG_SDK  __attribute__ ((visibility("default")))
#endif

#include "base_type_define.h"
#include <list>
#include<string>
#include "NetProtocolDefine_ForNVMS.h"
#include <map>

namespace ECMS_LOG_DEF
{
	const int LOG_USERNAME_LEN = 64;//log数据用来存储用户名的长度


	enum LOG_ERROR_CODE
	{
		LOG_ERROR_SUCCESS=0x0,
		LOG_ERROR_BASE=0x20000000, ////用户错误都需要这个位为1
	};

	/////////////报警日志子类型
	const tuint64 LOG_BASE_MASK = 0x10;//低四位保留

    //偏移量从 LOG_INFOR_RECORD_STATE 后面开始计算
    const tuint64 LOG_TYPE_BASE_MASK = (LOG_BASE_MASK << 45);

	const tuint64 LOG_ALARM_MOTION = LOG_BASE_MASK;  //移动侦测
	const tuint64 LOG_ALARM_SENSOR = (LOG_BASE_MASK << 1);  //传感器报警
	const tuint64 LOG_ALARM_ALARMOUTPUT = (LOG_BASE_MASK << 2); //报警输出端子报警
	const tuint64 LOG_ALARM_OCCLUSION = (LOG_BASE_MASK << 3); //遮挡侦测报警
	const tuint64 LOG_ALARM_VLOSS = (LOG_BASE_MASK << 4);	//视频丢失
    const tuint64 LOG_ALARM_INTELLIGENT = (LOG_BASE_MASK << 5);	//智能侦测报警
	//保留一位 (LOG_BASE_MASK << 5)
	//所有报警日志

	/////////////操作日志子类型
	const tuint64 LOG_OPERATE_BASE_MASK = (LOG_BASE_MASK << 6);

	const tuint64 LOG_OPERATE_RECORD_SPB = (LOG_OPERATE_BASE_MASK);//录像检索回放备份（检索search，回放playback，备份backup）
	const tuint64 LOG_OPERATE_MANUAL_RECORD = (LOG_OPERATE_BASE_MASK << 1);//手动录像（开始/停止手动录像）
	const tuint64 LOG_OPERATE_MANUAL_ALARM =(LOG_OPERATE_BASE_MASK << 2);//手动报警（手动触发报警/手动消除报警）
	const tuint64 LOG_OPERATE_SYSTEM_MAINTENANCE = (LOG_OPERATE_BASE_MASK << 3);//系统维护（升级，恢复出厂设置，备份和还原配置，查询和导出日志）
	const tuint64 LOG_OPERATE_PTZ_CONTROL = (LOG_OPERATE_BASE_MASK << 4);//云台控制（开始/停止巡航线，开始/停止轨迹，云台方向控制）
	const tuint64 LOG_OPERATE_AUDIO_TALK = (LOG_OPERATE_BASE_MASK << 5);//语音对讲（开启对讲，关闭对讲）
	const tuint64 LOG_OPERATE_SYSTEM_SCR = (LOG_OPERATE_BASE_MASK << 6);//开关机（开机start，关机close，重新启动restart）
	const tuint64 LOG_OPERATE_LOGIN_LOGOUT = (LOG_OPERATE_BASE_MASK << 7);//登入/登出
	const tuint64 LOG_OPERATE_SNAPSHOT_MSPB = (LOG_OPERATE_BASE_MASK << 8);//图片（手动抓拍manual snap，图片检索search，图片预览play，图片备份导出backup）
	const tuint64 LOG_OPERATE_FORMAT_HD = (LOG_OPERATE_BASE_MASK << 9);//格式化硬盘
    const tuint64 LOG_OPERATE_HDD_INSERT = (LOG_TYPE_BASE_MASK);//磁盘插入
    const tuint64 LOG_OPERATE_FEATURELIBRARY = (LOG_TYPE_BASE_MASK << 4);//特征库增加

	//所有操作日志
	const tuint64 LOG_TYPE_OPERATE_MASK = LOG_OPERATE_RECORD_SPB | LOG_OPERATE_MANUAL_RECORD | LOG_OPERATE_MANUAL_ALARM |
		LOG_OPERATE_SYSTEM_MAINTENANCE | LOG_OPERATE_PTZ_CONTROL | LOG_OPERATE_AUDIO_TALK
        | LOG_OPERATE_SYSTEM_SCR | LOG_OPERATE_LOGIN_LOGOUT | LOG_OPERATE_SNAPSHOT_MSPB | LOG_OPERATE_FORMAT_HD | LOG_OPERATE_HDD_INSERT
        | LOG_OPERATE_FEATURELIBRARY;

	/////////////设置日志子类型
	const tuint64 LOG_CONFIG_BASE_MASK = (LOG_OPERATE_BASE_MASK << 10);

	const tuint64 LOG_CONFIG_CHANNEL = (LOG_CONFIG_BASE_MASK);//通道参数（包括通道增删改，分组增删改，轮询配置，图像配置，云台参数）
	const tuint64 LOG_CONFIG_RECORD = (LOG_CONFIG_BASE_MASK << 1);//	录像参数
	const tuint64 LOG_CONFIG_ALARM = (LOG_CONFIG_BASE_MASK << 2);//报警参数
	const tuint64 LOG_CONFIG_DISK = (LOG_CONFIG_BASE_MASK << 3);//磁盘参数
	const tuint64 LOG_CONFIG_NETWORK = (LOG_CONFIG_BASE_MASK << 4);//网络参数
	const tuint64 LOG_CONFIG_SCHEDULE = (LOG_CONFIG_BASE_MASK << 5);//排程参数
	const tuint64 LOG_CONFIG_USER = (LOG_CONFIG_BASE_MASK << 6);//用户参数
	const tuint64 LOG_CONFIG_BASIC = (LOG_CONFIG_BASE_MASK << 7);//基本配置

	//所有设置日志
	const tuint64 LOG_TYPE_CONFIG_MASK = LOG_CONFIG_CHANNEL | LOG_CONFIG_RECORD | LOG_CONFIG_ALARM | LOG_CONFIG_DISK | LOG_CONFIG_NETWORK
		| LOG_CONFIG_SCHEDULE | LOG_CONFIG_USER | LOG_CONFIG_BASIC;

	/////////////异常日志子类型
	const tuint64 LOG_EXCEPTION_BASE_MASK = (LOG_CONFIG_BASE_MASK << 8);

	const tuint64 LOG_EXCEPTION_UNLAWFUL_ACCESS = LOG_EXCEPTION_BASE_MASK;//非法访问
	const tuint64 LOG_EXCEPTION_DISK_FULL = (LOG_EXCEPTION_BASE_MASK << 1);//磁盘满
	const tuint64 LOG_EXCEPTION_DISK_IO_ERROR = (LOG_EXCEPTION_BASE_MASK << 2);//磁盘I/O错误
	const tuint64 LOG_EXCEPTION_IP_COLLISION = (LOG_EXCEPTION_BASE_MASK << 3);//IP冲突
	const tuint64 LOG_EXCEPTION_INTERNET_DISCONNECT = (LOG_EXCEPTION_BASE_MASK << 4);//网络断开
	const tuint64 LOG_EXCEPTION_IPC_DISCONNECT = (LOG_EXCEPTION_BASE_MASK << 5);//IPC未连接
	const tuint64 LOG_EXCEPTION_ABNORMAL_SHUTDOWN = (LOG_EXCEPTION_BASE_MASK << 6);//系统异常关机
	const tuint64 LOG_EXCEPTION_ABNORMAL_RAID_SUBHEALTH = (LOG_EXCEPTION_BASE_MASK << 7);//阵列亚健康
	const tuint64 LOG_EXCEPTION_ABNORMAL_RAID_UNAVAILABLE = (LOG_EXCEPTION_BASE_MASK << 8);//阵列不可用
	const tuint64 LOG_EXCEPTION_NO_DISK = (LOG_EXCEPTION_BASE_MASK << 9);			//无磁盘
    const tuint64 LOG_EXCEPTION_VIDEO_LOSS = (LOG_EXCEPTION_BASE_MASK << 10);			//视频丢失
    const tuint64 LOG_EXCEPTION_SIGNAL_SHELTER = (LOG_EXCEPTION_BASE_MASK << 11);			//信号遮挡
    const tuint64 LOG_EXCEPTION_ABNORMAL_RAID_HOT_EXCEPTION = (LOG_TYPE_BASE_MASK << 3);			//阵列热备异常
    const tuint64 LOG_EXCEPTION_NAT_TRAVERSAL_ABNORMAL = (LOG_TYPE_BASE_MASK << 2);   //NAT网络穿透
    const tuint64 LOG_EXCEPTION_HDD_PULL_OUT = (LOG_TYPE_BASE_MASK << 1);			//磁盘拨出
    const tuint64 LOG_EXCEPTION_DISCARD_EXTRACT_TASK  = (LOG_TYPE_BASE_MASK << 5); //丢弃人脸提取任务
    const tuint64 LOG_ALARM_FACE_MATCH = (LOG_TYPE_BASE_MASK << 6);//人脸比对报警
	const tuint64 LOG_EXCEPTION_ALARM_SERVER_OFFLINE = (LOG_TYPE_BASE_MASK << 7);			//报警服务器掉线 
    const tuint64 LOG_ALARM_COMBINED = (LOG_TYPE_BASE_MASK << 8);//组合报警
    const tuint64 LOG_TYPE_ALARM_MASK = LOG_ALARM_MOTION | LOG_ALARM_SENSOR | LOG_ALARM_ALARMOUTPUT | LOG_ALARM_OCCLUSION | LOG_ALARM_VLOSS | LOG_ALARM_INTELLIGENT | LOG_ALARM_COMBINED | LOG_ALARM_FACE_MATCH;

	//所有异常日志
	const tuint64 LOG_TYPE_EXCEPTION_MASK = LOG_EXCEPTION_UNLAWFUL_ACCESS | LOG_EXCEPTION_DISK_FULL | LOG_EXCEPTION_DISK_IO_ERROR
		| LOG_EXCEPTION_IP_COLLISION | LOG_EXCEPTION_INTERNET_DISCONNECT | LOG_EXCEPTION_IPC_DISCONNECT
		| LOG_EXCEPTION_ABNORMAL_SHUTDOWN | LOG_EXCEPTION_ABNORMAL_RAID_SUBHEALTH | LOG_EXCEPTION_ABNORMAL_RAID_UNAVAILABLE 
        | LOG_EXCEPTION_NO_DISK | LOG_EXCEPTION_VIDEO_LOSS | LOG_EXCEPTION_SIGNAL_SHELTER | LOG_EXCEPTION_HDD_PULL_OUT | LOG_EXCEPTION_NAT_TRAVERSAL_ABNORMAL | LOG_EXCEPTION_ALARM_SERVER_OFFLINE
        | LOG_EXCEPTION_DISCARD_EXTRACT_TASK;

	/////////////信息日志子类型
    const tuint64 LOG_INFOR_BASE_MASK = (LOG_EXCEPTION_BASE_MASK << 12);

	const tuint64 LOG_INFOR_SCHEDULE_RECORD = LOG_INFOR_BASE_MASK;//排程录像开启/关闭
	const tuint64 LOG_INFOR_SCHEDULE_SNAP = (LOG_INFOR_BASE_MASK << 1);//定时抓图
	const tuint64 LOG_INFOR_DISK = (LOG_INFOR_BASE_MASK << 2);//硬盘信息
	const tuint64 LOG_INFOR_NETWORK = (LOG_INFOR_BASE_MASK << 3);//网络状态
	const tuint64 LOG_INFOR_SYSTEM_BASE = (LOG_INFOR_BASE_MASK << 4);//系统基本信息
	const tuint64 LOG_INFOR_SYSTEM_RUN = (LOG_INFOR_BASE_MASK << 5);//系统运行状态
	const tuint64 LOG_INFOR_CHANNEL_STATE = (LOG_INFOR_BASE_MASK << 6);//通道状态
	const tuint64 LOG_INFOR_ALARM_STATE = (LOG_INFOR_BASE_MASK << 7);//报警状态
	const tuint64 LOG_INFOR_RECORD_STATE = (LOG_INFOR_BASE_MASK << 8);//录像状态
	//所有信息日志
	const tuint64 LOG_TYPE_INFORMATION = LOG_INFOR_SCHEDULE_RECORD | LOG_INFOR_SCHEDULE_SNAP | LOG_INFOR_DISK
		| LOG_INFOR_NETWORK | LOG_INFOR_SYSTEM_BASE | LOG_INFOR_SYSTEM_RUN
		| LOG_INFOR_CHANNEL_STATE | LOG_INFOR_ALARM_STATE | LOG_INFOR_RECORD_STATE;

	//日志主类型
	//全部，报警，操作，设置，异常，信息
	const tuint64 LOG_TYPE_ALL_MASK = LOG_TYPE_ALARM_MASK | LOG_TYPE_OPERATE_MASK | LOG_TYPE_CONFIG_MASK | LOG_TYPE_EXCEPTION_MASK | LOG_TYPE_INFORMATION;
	/**********状态日志 由于表不同，所以不用考虑是否与上面定义的重复***********/ 
	const tuint64 LOG_STATE_MASK = 0x10;//低四位保留
	const tuint64 LOG_STATE_SERVER_ONLINE = LOG_STATE_MASK <<1;  //服务器上线
	const tuint64 LOG_STATE_SERVER_OFFLINE = LOG_STATE_MASK <<2;  //服务器下线
	//所有服务器状态日志
	const tuint64 LOG_STATE_SERVER = LOG_STATE_SERVER_ONLINE | LOG_STATE_SERVER_OFFLINE; 

	const tuint64 LOG_STATE_PU_ONLINE = LOG_STATE_MASK <<3;
	const tuint64 LOG_STATE_PU_OFFLINE = LOG_STATE_MASK <<4;
	//所有编码器状态日志
	const tuint64 LOG_STATE_PU = LOG_STATE_PU_ONLINE | LOG_STATE_PU_OFFLINE; 

	const tuint64 LOG_STATE_DECODER_ONLINE = LOG_STATE_MASK <<5;
	const tuint64 LOG_STATE_DECODER_OFFLINE = LOG_STATE_MASK <<6; 
	//所有解码器状态日志
	const tuint64 LOG_STATE_DECODER = LOG_STATE_DECODER_ONLINE | LOG_STATE_DECODER_OFFLINE; 

	const tuint64 LOG_STATE_ALARMHOST_ONLINE = LOG_STATE_MASK <<7;
	const tuint64 LOG_STATE_ALARMHOST_OFFLINE = LOG_STATE_MASK <<8; 
	//所有报警主机状态日志
	const tuint64 LOG_STATE_ALARMHOST = LOG_STATE_ALARMHOST_ONLINE | LOG_STATE_ALARMHOST_OFFLINE; 

	const tuint64 LOG_STATE_CHLRECORD_RECORDING = LOG_STATE_MASK <<9;//开始录像
	const tuint64 LOG_STATE_CHLRECORD_NORECORD = LOG_STATE_MASK <<10; //停止录像
	//所有通道录像状态日志
	const tuint64 LOG_STATE_CHLRECORD = LOG_STATE_CHLRECORD_RECORDING | LOG_STATE_CHLRECORD_NORECORD; 

	const tuint64 LOG_STATE_CHANNEL_ONLINE = LOG_STATE_MASK <<11;
	const tuint64 LOG_STATE_CHANNEL_OFFLINE = LOG_STATE_MASK <<12;
	//所有通道上下状态日志
	const tuint64 LOG_STATE_CHANNEL = LOG_STATE_CHANNEL_ONLINE | LOG_STATE_CHANNEL_OFFLINE; 

	//CS客户端
	const tuint64 LOG_STATE_MCU_ONLINE = LOG_STATE_MASK <<13;
	const tuint64 LOG_STATE_MCU_OFFLINE = LOG_STATE_MASK <<14;
	//所有客户端上下状态日志
	const tuint64 LOG_STATE_MCU = LOG_STATE_MCU_ONLINE | LOG_STATE_MCU_OFFLINE; 

	//门禁系统
	const tuint64 LOG_STATE_ACSSYSTEM_ONLINE = LOG_STATE_MASK <<15;
	const tuint64 LOG_STATE_ACSSYSTEM_OFFLINE = LOG_STATE_MASK <<16;
	//所有门禁系统上下状态日志
	const tuint64 LOG_STATE_ACSSYSTEM = LOG_STATE_ACSSYSTEM_ONLINE | LOG_STATE_ACSSYSTEM_OFFLINE; 

	//所有状态日志
	const tuint64 LOG_STATE_ALL = LOG_STATE_SERVER | LOG_STATE_PU | LOG_STATE_DECODER | LOG_STATE_ALARMHOST | LOG_STATE_CHLRECORD | LOG_STATE_CHANNEL | LOG_STATE_ACSSYSTEM /*| LOG_STATE_MCU*/; 
	/**********状态日志***********/

	//原N1000中的类型
	////////////视频丢失报警, 属于报警日志
	//const tuint64 LOG_TYPE_VLOSS = /*0x100*/0x01;

	//请求现场预览的日志信息, 属于操作日志
	const tuint64 LOG_TYPE_LIVE_BASE	= /*0x800*/0x8000000000;
	const tuint64 LOG_TYPE_LIVE_START	= LOG_TYPE_LIVE_BASE | 0x1;			//打开预览
	const tuint64 LOG_TYPE_LIVE_CLOSE	= LOG_TYPE_LIVE_BASE | 0x2;			//关闭预览
	const tuint64 LOG_TYPE_LIVE_DWELL_GROUP		= LOG_TYPE_LIVE_BASE | 0x5;	//开始组轮询，只记录在注册服务器
	const tuint64 LOG_TYPE_LIVE_DWELL_GROUP_STOP	= LOG_TYPE_LIVE_BASE | 0x6;	//停止组轮询，只记录在注册服务器
	const tuint64 LOG_TYPE_LIVE_DWELL_CHANNEL		= LOG_TYPE_LIVE_BASE | 0x7;    //开始通道轮询，只记录在注册服务器
	const tuint64 LOG_TYPE_LIVE_DWELL_CHANNEL_STOP= LOG_TYPE_LIVE_BASE | 0x8;    //停止通道轮询，只记录在注册服务器
	const tuint64 LOG_TYPE_LIVE_END	= LOG_TYPE_LIVE_BASE | 0xF;

	////////回放的日志信息,属于操作日志
	const tuint64   LOG_TYPE_PLAYBACK_BASE = /*0x1000*/LOG_OPERATE_RECORD_SPB;
	const tuint64   LOG_TYPE_RECDATA_PLAYBACK_MSU			= LOG_OPERATE_RECORD_SPB | 0x1;	//请求系统回放成功
	const tuint64   LOG_TYPE_RECDATA_PLAYBACK_STOP_MSU	= LOG_OPERATE_RECORD_SPB | 0x2;	//系统回放结束,在回放自己结束后也要记录一次
	const tuint64   LOG_TYPE_RECDATA_REQUEST_EVENT_MSU	= LOG_OPERATE_RECORD_SPB | 0x3;	//请求系统事件成功
	const tuint64   LOG_TYPE_RECDATA_PLAYBACK_DEVICE= LOG_OPERATE_RECORD_SPB | 0x4;	//请求设备回放成功
	const tuint64   LOG_TYPE_RECDATA_PLAYBACK_STOP_DEVICE	= LOG_OPERATE_RECORD_SPB | 0x5;	//设备回放结束,在回放自己结束后也要记录一次
	const tuint64   LOG_TYPE_RECDATA_REQUEST_EVENT_DEVICE	= LOG_OPERATE_RECORD_SPB | 0x6;	//请求设备事件成功（设备）
	const tuint64   LOG_TYPE_RECDATA_BACKUP_DEVICE		= LOG_OPERATE_RECORD_SPB | 0x7;	//录像数据备份（设备）
	const tuint64   LOG_TYPE_PRECDATA_BACKUP_STOP_DEVICE = LOG_OPERATE_RECORD_SPB | 0x8;	//录像数据备份停止（设备）
	const tuint64   LOG_TYPE_RECDATA_BACKUP_MSU		= LOG_OPERATE_RECORD_SPB | 0x9;	//录像数据备份(存储服务器)
	const tuint64   LOG_TYPE_PRECDATA_BACKUP_STOP_MSU = LOG_OPERATE_RECORD_SPB | 0xA;	//录像数据备份停止（存储服务器）
	const tuint64   LOG_TYPE_PLAYBACK_END	= LOG_OPERATE_RECORD_SPB | 0xF;

	//云台操作, 属于操作日志
	const tuint64 LOG_TYPE_PTZ_BASE		= /*0x2000*/LOG_OPERATE_PTZ_CONTROL;
	const tuint64 LOG_TYPE_PTZ_CRUISE		= LOG_OPERATE_PTZ_CONTROL | 0x1;		//打开巡航线
	const tuint64 LOG_TYPE_PTZ_CRUISE_END = LOG_OPERATE_PTZ_CONTROL | 0x2;		//停止巡航线
	const tuint64 LOG_TYPE_PTZ_PRESET		= LOG_OPERATE_PTZ_CONTROL | 0x3;		//打开预置点
	const tuint64 LOG_TYPE_PTZ_CTRL_DIRECTION		= LOG_OPERATE_PTZ_CONTROL | 0x4;		//云台方向控制
	const tuint64 LOG_TYPE_PTZ_TRACK_START= LOG_OPERATE_PTZ_CONTROL | 0x5;		//云台轨迹开始
	const tuint64 LOG_TYPE_PTZ_TRACK_END	= LOG_OPERATE_PTZ_CONTROL | 0x6;		//云台轨迹结束
	const tuint64 LOG_TYPE_PTZ_SCAN		= LOG_OPERATE_PTZ_CONTROL | 0x7;		//开始自动扫描
	const tuint64 LOG_TYPE_PTZ_SCAN_END	= LOG_OPERATE_PTZ_CONTROL | 0x8;		//停止自动扫描
	const tuint64 LOG_TYPE_PTZ_CTRL = LOG_OPERATE_PTZ_CONTROL | 0x9; ///云台控制    抽象一点的动作  不具体
	const tuint64 LOG_TYPE_PTZ_END		= LOG_OPERATE_PTZ_CONTROL | 0xF;

	//对讲和广播, 属于操作日志
	const tuint64 LOG_TYPE_TALKBACK_BASE			= /*0x10000*/LOG_OPERATE_AUDIO_TALK;
	const tuint64 LOG_TYPE_TALKBACK				= LOG_OPERATE_AUDIO_TALK | 0x1;	//对讲
	const tuint64 LOG_TYPE_TALKBACK_STOP			= LOG_OPERATE_AUDIO_TALK | 0x2;	//对讲结束
	const tuint64 LOG_TYPE_SOUND_RECORD			= LOG_OPERATE_AUDIO_TALK | 0x3;	//对讲录音
	const tuint64 LOG_TYPE_SOUND_RECORD_STOP		= LOG_OPERATE_AUDIO_TALK | 0x4;	//对讲录音结束
	const tuint64 LOG_TYPE_BROADCAST				= LOG_OPERATE_AUDIO_TALK | 0x5;	//广播，只记录在注册服务器
	const tuint64 LOG_TYPE_BROADCAST_STOP			= LOG_OPERATE_AUDIO_TALK | 0x6;	//广播结束，只记录在注册服务器
	const tuint64 LOG_TYPE_TALKBACK_END			= LOG_OPERATE_AUDIO_TALK | 0xF;

	//配置客户端操作日志，可能存在于电视墙服务器和注册服务器两个地方
	const tuint64 LOG_TYPE_CONFIGCLIENT_BASE		=/*0x8000*/0x40000000000;
	const tuint64 LOG_TYPE_CONFIGCLIENT_RSU_SAVE	=LOG_TYPE_CONFIGCLIENT_BASE | 0x1;/////////保存非电子地图的配置信息
	const tuint64 LOG_TYPE_CONFIGCLIENT_EMAP_SAVE	=LOG_TYPE_CONFIGCLIENT_BASE | 0x2;////////保存电子地图的配置信息
	const tuint64 LOG_TYPE_CONFIGCLIENT_END		=LOG_TYPE_CONFIGCLIENT_BASE | 0xF;

	//设备参数设置, 属于操作日志
	const tuint64 LOG_TYPE_DEVICE_CONFIG_BASE		= /*0x4000*/0x80000000000;
	const tuint64 LOG_TYPE_DEVICE_CONFIG_REBOOT	= LOG_TYPE_DEVICE_CONFIG_BASE | 0x1;/////成功重启设备
	const tuint64 LOG_TYPE_DEVICE_CONFIG_RESTORE	= LOG_TYPE_DEVICE_CONFIG_BASE | 0x2;//////成功恢复缺省配置
	const tuint64 LOG_TYPE_DEVICE_CONFIG_SAVE	    = LOG_TYPE_DEVICE_CONFIG_BASE | 0x3;/////保存配置
	const tuint64 LOG_TYPE_DEVICE_CONFIG_IMPORT = LOG_TYPE_DEVICE_CONFIG_BASE | 0x4;////导入配置数据
	const tuint64 LOG_TYPE_DEVICE_CONFIG_EXPORT = LOG_TYPE_DEVICE_CONFIG_BASE | 0x5; ////导出配置数据
	const tuint64 LOG_TYPE_DEVICE_CONFIG_END		= LOG_TYPE_DEVICE_CONFIG_BASE | 0xF;

	//日志查询, 属于操作日志
	const tuint64 LOG_TYPE_LOG_SEARCH_BASE = /*0x400*/0x100000000000;
	const tuint64 LOG_TYPE_LOG_SEARCH			= LOG_TYPE_LOG_SEARCH_BASE | 0x1; //察看系统日志操作，只记录在注册服务器
	const tuint64 LOG_TYPE_LOG_DEVICE_SEARCH	= LOG_TYPE_LOG_SEARCH_BASE | 0x2;		//查看设备日志
	const tuint64 LOG_TYPE_EXPORT_LOG			= LOG_TYPE_LOG_SEARCH_BASE | 0x3;
	const tuint64 LOG_TYPE_LOG_SEARCH_END = LOG_TYPE_LOG_SEARCH_BASE | 0xF;

	//服务日志, 属于系统日志
	const tuint64   LOG_TYPE_SERVICE_BASE   = /*0x20*/0x200000000000;
	const tuint64   LOG_TYPE_SYSTEM_START		= LOG_TYPE_SERVICE_BASE | 0x1;	//系统启动
	const tuint64   LOG_TYPE_SYSTEM_SHUTDOWN	= LOG_TYPE_SERVICE_BASE | 0x2;	//系统关闭
	const tuint64   LOG_TYPE_SYSTEM_SHUTDOWN_ABNORMAL = LOG_TYPE_SERVICE_BASE | 0x3;	//系统异常关闭，一般属于服务器异常断电导致
	const tuint64   LOG_TYPE_SYSTEM_RESTART		= LOG_TYPE_SERVICE_BASE | 0x4;	//系统重启动，目前只有注册服务器导入配置一个可能
	const tuint64   LOG_TYPE_SERVICE_START		= LOG_TYPE_SERVICE_BASE | 0x5;	//服务人为点击开始
	const tuint64   LOG_TYPE_SERVICE_STOP			= LOG_TYPE_SERVICE_BASE | 0x6;	//服务人为停止（点击了停止服务按钮）
	const tuint64   LOG_TYPE_SERVICE_START_SUCCESS	= LOG_TYPE_SERVICE_BASE | 0x7;	//服务开启成功（指登陆服务器成功）
	const tuint64   LOG_TYPE_SERVICE_START_FAIL		= LOG_TYPE_SERVICE_BASE | 0x8;	//服务开启失败（自动登陆因为要反复登陆会记录大量信息，所以此时只记录用户名密码错误的情况）
	const tuint64   LOG_TYPE_SERVICE_STOP_ABNORMAL	= LOG_TYPE_SERVICE_BASE | 0x9;	//服务异常停止（和注册服务器断开）
	const tuint64   LOG_TYPE_SERVICE_START_ABNORMAL	= LOG_TYPE_SERVICE_BASE | 0xA;	//自动重连成功
	const tuint64   LOG_TYPE_SERVICE_END	   = LOG_TYPE_SERVICE_BASE | 0xF;

	//登陆日志,属于系统日志
	const tuint64   LOG_TYPE_LOGIN_BASE = /*0x10*/LOG_OPERATE_LOGIN_LOGOUT;
	const tuint64   LOG_TYPE_LOGIN_SUCCESS	= LOG_OPERATE_LOGIN_LOGOUT | 0x1;		//登陆成功
	const tuint64   LOG_TYPE_LOGOUT			= LOG_OPERATE_LOGIN_LOGOUT | 0x2;		//注销
	const tuint64   LOG_TYPE_LOGIN_END		= LOG_OPERATE_LOGIN_LOGOUT | 0xF;

	//////////设备上线、掉线，属于系统日志
	const tuint64 LOG_TYPE_DEVICE_CONNECT_BASE = /*0x40000*/0x400000000000;
	const tuint64 LOG_TYPE_DEVICE_ONLINE = LOG_TYPE_DEVICE_CONNECT_BASE | 0x1;
	const tuint64 LOG_TYPE_DEVICE_OFFLINE = LOG_TYPE_DEVICE_CONNECT_BASE | 0x2;
	const tuint64 LOG_TYPE_DEVICE_CONNECT_END = LOG_TYPE_DEVICE_CONNECT_BASE | 0xF;

	//设备异常
	const tuint64 LOG_TYPE_EXCEPTION_BASE			= /*0x20000*/0x800000000000;
	const tuint64 LOG_TYPE_IP_CONFLICT			= LOG_TYPE_EXCEPTION_BASE | 0x1; 	//网络地址冲突 LOG_EXCEPTION_IP_COLLISION
	const tuint64 LOG_TYPE_NETWORK_ERR			= LOG_TYPE_EXCEPTION_BASE | 0x2;	//网络异常
	const tuint64 LOG_TYPE_DDNS_ERR				= LOG_TYPE_EXCEPTION_BASE | 0x3;	//DDNS错误
	const tuint64 LOG_TYPE_DISK_IO_ERR			= LOG_TYPE_EXCEPTION_BASE | 0x4;	//磁盘读写错误
	const tuint64 LOG_TYPE_UNKNOWN_OFF			= LOG_TYPE_EXCEPTION_BASE | 0x5;	//异常断电
	const tuint64 LOG_TYE_DISK_WARNING			= LOG_TYPE_EXCEPTION_BASE | 0x6;	//磁盘衰减
	const tuint64 LOG_TYE_DISK_DISCONNECT			= LOG_TYPE_EXCEPTION_BASE | 0x7;	//磁盘掉线
	const tuint64 LOG_TYPE_UNKNOWN_ERR			= LOG_TYPE_EXCEPTION_BASE | 0x8;	//未知错误
	const tuint64 LOG_TYPE_EXCEPTION_END			= LOG_TYPE_EXCEPTION_BASE | 0xF;

	//所有系统日志类型标志
	const tuint64   LOG_TYPE_SYSTEM_MASK = LOG_TYPE_LOGIN_BASE | LOG_TYPE_SERVICE_BASE | LOG_TYPE_DEVICE_CONNECT_BASE;

	//所有操作类型标志
	const tuint64   LOG_TYPE_OPERATOR_MASK = LOG_TYPE_LOG_SEARCH_BASE|LOG_TYPE_PLAYBACK_BASE|LOG_TYPE_LIVE_BASE|LOG_TYPE_PTZ_BASE|LOG_TYPE_DEVICE_CONFIG_BASE|LOG_TYPE_CONFIGCLIENT_BASE|LOG_TYPE_TALKBACK_BASE;

	//用户相关日志标志,目前相关的有操作日志和登陆日志
	const tuint64   LOG_TYPE_ABOUT_USER_MASK = LOG_TYPE_LOGIN_BASE|LOG_TYPE_OPERATOR_MASK;
	//原N1000中的类型

#pragma pack(push, 4)

	typedef struct _alarm_log_info 
	{
		_alarm_log_info():dwLogID(0),dwLogKey(0),dwOccurTimeS(0), dwNodeType(0)
			,byIndexNum(0) , dwIP(0), blobSize(0), pBlobData(NULL){
				memset(byReserve, 0, 3* sizeof(unsigned char));
				memset(content, 0, MAX_LOG_CONTENT_LEN* sizeof(char));
				memset(szAlarmTrigger, 0, MAX_LOG_ALARMTRIGGER_LEN* sizeof(char));
		}
		tuint64 dwLogID;////确定Log类型
		tuint64 dwLogKey;
		tuint32 dwOccurTimeS; ///发生时间, 单位为秒

		tuint32 dwNodeType;//节点类型,在登陆日志和操作日志里面表示登陆的客户端类型：CC/IE/MC等
		GUID NodeGUID;		//节点GUID
		unsigned char byIndexNum;//从0开始的编号，在操作LOG中表示成功失败
		unsigned char byReserve[3];//保留

		GUID UserGUID;
		tuint32  dwIP;/////IP地址
		char   szUser[LOG_USERNAME_LEN*4];///用户  操作日志，配置日志存的用户名，报警日志存的报警节点的名称
		char content[MAX_LOG_CONTENT_LEN];////详细信息
		char szAlarmTrigger[MAX_LOG_ALARMTRIGGER_LEN]; //报警联动信息
        tuint32 blobSize;
        tuint64 pBlobData;//之前是指针
		tuint32 dwOverTimeS; ///结束时间, 单位为秒
		BYTE AlarmState;
	}ALARM_LOG_INFO;

	///////////////////////////////////////////////////////
	typedef ALARM_LOG_INFO OPERATOR_LOG_INFO;

	typedef struct _abnormal_log_info 
	{
		tuint32 dwOccurTimeS; ///发生时间, 单位为秒
		GUID NodeGUID;	//日志发生的节点GUID
		tuint32 ServerType;//发生异常的服务器ID，搜索后填充
		tuint32 lineNum;//在原文件中的行号
		tuint32 versionNO;//执行档版本号
		char  Servername[LOG_USERNAME_LEN];///发生事情的服务器名称，搜索后填充
		char  FileName[LOG_USERNAME_LEN];///原文件名
		char  DebugInfo[256];//填充的Debug信息
	}ABNORMAL_LOG_INFO;
	

	typedef struct _state_log_info 
	{
		tuint64 dwLogID;////确定Log类型
		tuint32 dwNodeType;//节点类型,在登陆日志和操作日志里面表示登陆的客户端类型：CC/IE/MC等
		GUID NodeGUID;		//节点GUID
		char szName[256];//节点名称
		char szArea[1024];//区域
		//区域
		tuint32 dwOccurTimeS; ///发生时间, 单位为秒
		char content[512];////详细信息
		unsigned char byReserve[3];//保留
		_state_log_info()
		{
			dwLogID = 0;
			dwNodeType = 0;
			NodeGUID = GUID_NULL;
			dwOccurTimeS = 0;
			memset(szName,0,sizeof(szName));
			memset(szArea,0,sizeof(szArea));
			memset(content,0,sizeof(content));
			memset(byReserve,0,sizeof(byReserve));
		}
	}STATE_LOG_INFO;


#pragma pack(pop)
}



_EXPORT_LOG_SDK bool LOG_Initial(const char *szSysLogFile, const char *szAbNormalLogFile,const char *szPmsLogFile, bool bCache = false, bool AsyncWrite = false,const std::string strDBAddress="127.0.0.1",tuint32 nDBPort=3306, const std::string strUserID="root", const std::string strPSW="123456");

_EXPORT_LOG_SDK void LOG_Quit(void);

_EXPORT_LOG_SDK tuint32 LOG_GetLastError(void);

//dwLogID:日志类型, dwNodeID:节点ID, dwNodeType:节点类型, byAlarmIndex:报警源索引
_EXPORT_LOG_SDK bool LOG_AddAlarmLogRecord(tuint64 dwLogID, const GUID &NodeGUID,const char *pNodeName, tuint32 dwNodeType, const char *pContent,  const char *pBlobData, int blobSize,tuint32 OccurTime,tuint64 dwLogKey/*接收到的报警utc时间*/, const char * pAlarmTrigger, BYTE AlarmState);

//dwLogID:日志类型, dwUserID:用户ID, dwOperateNodeID:所操作节点的ID
_EXPORT_LOG_SDK bool LOG_AddOperateLogRecord(tuint64 dwLogID, const GUID &UserGUID, const char *szUserName, tuint32 clientType, const GUID &OperateNodeGUID, tuint32 dwOcrTime,  const char *pBlobData, int blobSize,  const char *pContent);

//配置日志
_EXPORT_LOG_SDK bool LOG_AddConfigLogRecord(tuint64 dwLogID, const GUID &UserGUID, const char *szUserName, tuint32 clientType, const GUID &OperateNodeGUID, const char *pContent);

//异常日志
_EXPORT_LOG_SDK bool LOG_AddExceptionLogRecord(tuint64 dwLogID, const GUID &NodeGUID, const char *pContent, tuint32 time=0);

//信息日志
_EXPORT_LOG_SDK bool LOG_AddInforLogRecord(tuint64 dwLogID, const GUID &UserGUID, const char *szUserName, tuint32 clientType, const GUID &OperateNodeGUID, const char *pContent);

//strText:异常描述, fileName=CString(__FILE__):记录文件名称, line=__LINE__:记录文件行号,version发生异常模块的版本号
_EXPORT_LOG_SDK bool LOG_AddAbnormalLogRecord(const char *szText, const char *szFileName, tuint32 line, tuint32 version);

_EXPORT_LOG_SDK tuint32 LOG_CreateQueryRecord(tuint64 dwLogTypeMask, tuint32 dwStartTime, tuint32 dwEndTime, const GUID &UserGUID);

_EXPORT_LOG_SDK tuint32 LOG_GetRecordTotalCount(tuint32 handle);

//从第startIndex条开始取count条日志，startIndex从1开始
_EXPORT_LOG_SDK tuint32 LOG_GetRecord(tuint32 handle, tuint32 startIndex, tuint32 count, char *&pBuf);

_EXPORT_LOG_SDK bool LOG_DestroyQueryRecord(tuint32 handle);

_EXPORT_LOG_SDK int LOG_QueryAbnormalRecord(tuint32 dwStartTime, tuint32 dwEndTime, const char *pFilePath);

//删除dwTime这个时间点之前的日志
_EXPORT_LOG_SDK bool LOG_RemoveHistory(tuint32 dwTime);

//状态日志
//写入状态日志
_EXPORT_LOG_SDK bool LOG_AddStateLog(tuint64 dwLogID,tuint32 nodeType, const GUID &NodeGUID,const char *pNodeName,const char *pNodeArea,tuint32 dwOcrTim, const char *pContent);
//查询状态日志
_EXPORT_LOG_SDK tuint32 LOG_CreateQueryStateLog(tuint64 dwLogTypeMask, tuint32 dwStartTime, tuint32 dwEndTime);
_EXPORT_LOG_SDK tuint32 LOG_GetStateLogTotalCount(tuint32 handle);
_EXPORT_LOG_SDK tuint32 LOG_GetStateLog(tuint32 handle, tuint32 startIndex, tuint32 count, char *&pBuf);
_EXPORT_LOG_SDK bool LOG_DestroyQueryStateLog(tuint32 handle);
//清除状态日志
_EXPORT_LOG_SDK bool LOG_RemoveHistoryStateLog(tuint32 dwTime);

//分页查询报警操作配置等日志 返回值为查询句柄销毁用
_EXPORT_LOG_SDK tuint32 LOG_CreateQueryRecord_ByPage(const char * dwLogTypeMask, tuint32 dwStartTime, tuint32 dwEndTime, const GUID &UserGUID,\
	tuint32 pageIndex/*第几页*/, tuint32 pageSize/*每页条数*/, char *&pBuf , double &totalCount, bool isExportLog);
_EXPORT_LOG_SDK bool LOG_DestroyQueryRecord_ByPage(tuint32 handle);

//用于精确查询报警日志 返回值为查询句柄销毁用
_EXPORT_LOG_SDK tuint32 LOG_CreateQueryAlarmRecord_ByPageAndNode(const char* pLogTypeMask, const char* pLogNodeId, tuint32 dwStartTime, tuint32 dwEndTime, const GUID &UserGUID, tuint32 pageIndex/*第几页*/, tuint32 pageSize/*每页条数*/, char *&pBuf, double &totalCount, bool isExportLog);
_EXPORT_LOG_SDK bool LOG_DestroyQueryAlarmRecord_ByPageAndNode(tuint32 handle);
//分页查询状态日志
_EXPORT_LOG_SDK tuint32 LOG_CreateQueryStateLog_ByPage(tuint64 dwLogTypeMask, tuint32 dwStartTime, tuint32 dwEndTime,\
	tuint32 pageIndex/*第几页*/, tuint32 pageSize/*每页条数*/, char *&pBuf,double &totalCount);
_EXPORT_LOG_SDK bool LOG_DestroyQueryStateLog_ByPage(tuint32 handle);

_EXPORT_LOG_SDK tuint32 LOG_QueryPassRecord_ByPage(const CFGPACK_DEFINE::PMS_LOG_PASS_RECORD *pLogPassRecord,tuint32 dwStartTime, tuint32 dwEndTime,const GUID &UserGUID,\
	tuint32 pageIndex/*第几页*/, tuint32 pageSize/*每页条数*/, char *&pBuf,double &totalCount);
_EXPORT_LOG_SDK bool LOG_DestroyPassRecordLog_ByPage(tuint32 handle);
_EXPORT_LOG_SDK tuint32 LOG_QueryPassRecordList_ByPage(const CFGPACK_DEFINE::PMS_LOG_PASS_RECORD *pLogPassRecord,std::list<std::string>& laneGUIDList,tuint32 dwStartTime, tuint32 dwEndTime,\
	tuint32 pageIndex/*第几页*/, tuint32 pageSize/*每页条数*/, char *&pBuf,double &totalCount);
_EXPORT_LOG_SDK bool LOG_CountPassRecord(NVMS_NET_PROTOCOL::PMS_INFO &pmsInfo);
_EXPORT_LOG_SDK void LOG_QueryPassRecord_OverTime(const tuint32 tempParkingTimeLimit, std::list<CFGPACK_DEFINE::PMS_LOG_PASS_RECORD> &resultList);

_EXPORT_LOG_SDK bool LOG_QueryPassCharge_ByPage(const CFGPACK_DEFINE::PMS_LOG_PASS_CHARGE *pLogPassCharge, tuint32 dwStartTime, tuint32 dwEndTime, const GUID &UserGUID, tuint32 pageIndex/*第几页*/, tuint32 pageSize/*每页条数*/, char *&pBuf, double &totalCount);
_EXPORT_LOG_SDK bool LOG_DestroyPassChargeLog_ByPage(tuint32 handle);

_EXPORT_LOG_SDK bool LOG_QueryAccountCharge_ByPage(const CFGPACK_DEFINE::PMS_LOG_ACCOUNT_CHARGE *pLogAccountCharge, tuint32 dwStartTime, tuint32 dwEndTime, const GUID &UserGUID, tuint32 pageIndex/*第几页*/, tuint32 pageSize/*每页条数*/, char *&pBuf, double &totalCount);
_EXPORT_LOG_SDK bool LOG_DestroyAccountChargeLog_ByPage(tuint32 handle);

_EXPORT_LOG_SDK tuint32 LOG_PmsAddPassRecord(const CFGPACK_DEFINE::PMS_LOG_PASS_RECORD *pLogPassRecord);
_EXPORT_LOG_SDK bool LOG_PmsSavePassRecord(CFGPACK_DEFINE::PMS_LOG_PASS_RECORD *pPassRecord);
_EXPORT_LOG_SDK bool LOG_PmsDelPassRecord(tuint32 nIndex);
_EXPORT_LOG_SDK bool LOG_PmsQueryPassRecordByIndex(tuint32 nIndex,CFGPACK_DEFINE::PMS_LOG_PASS_RECORD &result);

// 仿照 PassRecord 类 建的 VisitorRecord 类  
/*****************************************/
_EXPORT_LOG_SDK tuint32 LOG_FuzzyQueryVisitorRecord_ByPage(tuint32 dwStartTime, tuint32 dwEndTime, std::string dwKeyValue, \
	std::string sexType, std::string allowedVisitType, std::string verifyType, std::string remindType, const GUID &UserGUID, \
	tuint32 pageIndex/*第几页*/, tuint32 pageSize/*每页条数*/, char *&pBuf,double &totalCount);

_EXPORT_LOG_SDK tuint32 LOG_QueryVisitorRecord_ByPage(const CFGPACK_DEFINE::LOG_VISITOR_RECORD *pLogVisitorRecord,const GUID &UserGUID,\
	tuint32 pageIndex/*第几页*/, tuint32 pageSize/*每页条数*/, char *&pBuf,double &totalCount);

_EXPORT_LOG_SDK bool LOG_DestroyVisitorRecordLog_ByPage(tuint32 handle);

_EXPORT_LOG_SDK std::string LOG_AddVisitorRecord(const CFGPACK_DEFINE::LOG_VISITOR_RECORD *pLogVisitorRecord);
_EXPORT_LOG_SDK bool LOG_ModifyVisitorRecord(const CFGPACK_DEFINE::LOG_VISITOR_RECORD *pVisitorRecord);
_EXPORT_LOG_SDK bool LOG_DelVisitorRecord(const std::string strIndex);
_EXPORT_LOG_SDK bool LOG_QueryExpectedEndTime(std::map<std::string, tuint32> &tempMap);
// 用于数据迁移
_EXPORT_LOG_SDK void LOG_QueryAllVisitorRecord(std::list<CFGPACK_DEFINE::LOG_VISITOR_RECORD> &resultList, tuint32& count);
_EXPORT_LOG_SDK bool LOG_DeleteRecordAndImg(const std::string strIndex);
/*****************************************/

_EXPORT_LOG_SDK bool LOG_PmsAddAccountCharge(const CFGPACK_DEFINE::PMS_LOG_ACCOUNT_CHARGE *accountCharge);
_EXPORT_LOG_SDK bool LOG_PmsAddPassCharge(const CFGPACK_DEFINE::PMS_LOG_PASS_CHARGE *logPassCharge);
_EXPORT_LOG_SDK tuint32 LOG_QueryContent(tuint64 dwLogKey, tuint32 iOccurTime,char *&pOutBuf,tuint32 nOutLen);
_EXPORT_LOG_SDK tuint32 LOG_EditContent(tuint64 dwLogKey, tuint32 iOccurTime,char *pContent,tuint32 nContent);
//数据库完整性校验
_EXPORT_LOG_SDK bool LOG_RunLogIntegrityCheck();

_EXPORT_LOG_SDK tuint32 LOG_QueryLogWithSop(tuint32 dwStartTime, tuint32 dwEndTime, std::list<ECMS_LOG_DEF::OPERATOR_LOG_INFO> &LogList);
//删除旧日志
_EXPORT_LOG_SDK bool LOG_RemoveOldData();

//查询日志表个数
_EXPORT_LOG_SDK void LOG_QueryTableCounts(std::list<std::string>& logTableList);
#endif
