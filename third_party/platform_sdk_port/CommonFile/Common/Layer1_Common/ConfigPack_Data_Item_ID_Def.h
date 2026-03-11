/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : zgj
** Date         : 2012-10-09
** Name         : 
** Version      : 2.0
** Description  :
** Modify Record:

1、ConfigPack的数据信息在客户端要采取改变既产生新的，需要重新组织以后保存到服务器去。
**************************************************************************/
#ifndef CONFIGPACK_DATA_ITEM_ID_DEF_H
#define CONFIGPACK_DATA_ITEM_ID_DEF_H
#include "base_type_define.h"

namespace CFGPACK_DEFINE
{
	//一般情况下不要改变此值，具体是否要更改讨论后确定
	const unsigned long		NCFG_VERSION					=	0x03;//////N9000不使用这个值
#pragma pack(push, 4)			//结构体定义为4字节对齐,在gcc里只能是1,2,4

	/************************************************************************/
	/* 数据项格式为*/
	/*    
	--NCFG_BLOCK_HEAD--
	-------------------
	--NCFG_ITEM_HEAD1--
	--NCFG_ITEM_HEAD2--
	--NCFG_ITEM_HEAD3--
	.
	.
	.
	--NCFG_ITEM_HEAD N--
	--------------------
	项数据信息
	*/
	/************************************************************************/
	/////////////数据项ID定义
	typedef enum data_item_id
	{
		////////////////////////
		DATA_ITEM_REPLY_FAIL = 0x10,/////////////回复失败的结构体 ECMS_NET_REPLY_RESULT

		/////////////系统基本配置的杂项
		DATA_ITEM_SYSTEM_BASE=0x100,
		DATA_ITEM_RSU_INI,
		DATA_ITEM_SYSTEM_PACKET,
		DATA_ITEM_EMAP_PACKET,
        DATA_ITEM_EXPORT_FILE_SECTION_ID_RANGE_BEGIN,
        DATA_ITEM_EXPORT_FILE_SECTION_ID_RANGE_END = DATA_ITEM_EXPORT_FILE_SECTION_ID_RANGE_BEGIN+10,

		//////////////节点相关的信息
		DATA_ITEM_RESOURCE_BASE = 0x200,
		DATA_ITEM_DEVICE_INFO,  /////设备节点的信息  NCFG_DEVICE_INFO
		DATA_ITEM_CH_INFO,   //////通道节点的信息  NCFG_DEVICE_CH_INFO
		DATA_ITEM_IP_CHANNEL_INFO,		//IP通道节点的信息
		DATA_ITEM_SCHEDULE_TIME_INFO,/////排程的时间信息   NCFG_SCHEDULE_TIME_SECTION
		DATA_ITEM_SCHEDULE_NAME_INFO,/////排程的名字信息  NCFG_SCHEDULE_NAME
		DATA_ITEM_AREA_INFO,  /////区域信息   ECMS_CFG_DEF::NCFG_AREA_INFO  更改和删除
		DATA_ITEM_SERVER_INFO, /////系统中的服务器信息  New   2013.9.3
		DATA_ITEM_ALARM_OUT_INFO, /////系统中的报警输出的信息  New  2013.9.3
		DATA_ITEM_ALARM_IN_INFO, /////系统中的报警输入的信息  New  2013.9.3
		DATA_ITEM_BINARY_RELATION_LOGIC,////2元逻辑关系  New  2013.9.3
		DATA_ITEM_BINARY_RELATION_LOGIC_FOR_EMAP,////2元逻辑关系  New  2013.9.3
		DATA_ITEM_BINARY_RELATION_PHYSICS,//2元物理关系  New  2013.9.3	
		DATA_ITEM_BINARY_RELATION_PHYSICS_FOR_EMAP,//2元物理关系  New  2013.9.3 
		DATA_ITEM_TERNARY_RELATION_PHYSICS,//3元物理关系  New  2013.9.3			
		DATA_ITEM_BINARY_RELATION_PHYSICS_FOR_MPR,//Media Play Resource  2元物理关系  New  2013.9.3
		DATA_ITEM_DECODER_INFO,  ////解码器节点信息add by wangxin
		DATA_ITEM_WALL_INFO,  ////电视墙节点信息add by wangxin DATA_ITEM_TVWALL_INFO被占用了
		DATA_ITEM_CHANNELGROUP_INFO,//通道组信息add by wangxin
		DATA_ITEM_CHANNEL_GROUP_RELATION_INFO,//通道与通道组关系信息add by wangxin
		DATA_ITEM_ALARMHOST_INFO,//报警主机节点信息add by wangxin
		DATA_ITEM_SUBSYSTEM_INFO,//子系统节点信息add by wangxin
		DATA_ITEM_ZONE_INFO,//防区节点信息add by wangxin
		DATA_ITEM_ACSSYSTEM_INFO,//第三方门禁考勤系统
		DATA_ITEM_ACSDEVICE_INFO,//门禁考勤设备
		DATA_ITEM_ACSDOOR_INFO,//门禁考勤门
		DATA_ITEM_ACSREADER_INFO,//门禁考勤读头
		DATA_ITEM_TASK_SCHEDULE,//各项任务排程信息
		DATA_ITEM_ALARM_TRIGGER_SCHEDULE,//报警联动排程信息
		DATA_ITEM_FIXED_CAR,//固定车
		DATA_ITEM_GATEWAY_INFO,//出入口
		DATA_ITEM_LANE_INFO,//车道
		DATA_ITEM_CHARGE_RULE_INFO,//收费规则
		DATA_ITEM_BAG_PERIOD_RULE_INFO,//包期规则
		DATA_ITEM_PARK_INFO,//停车场信息
		DATA_ITEM_TIME_SECTION_CHARGE_RULE_INFO,//时间段收费规则信息
		DATA_ITEM_LOG_OPERATOR_RECORD_INFO,//操作记录信息
		DATA_ITEM_LED_INFO,//LED屏
		DATA_ITEM_BR_LANE_DEVICE,//车道和屏或者摄像机的关系
		DATA_ITEM_CASCADE_PLATFORM_INFO,//级联平台
		DATA_ITEM_PASS_RECORD_INFO,	//通行记录
		DATA_ITEM_BLACKLIST_CAR,//黑名单车辆
		DATA_ITEM_CONFIG_INFO,//系统配置数据
		DATA_ITEM_SOP_INFO,//处警标准操作指导信息add by zcy
		DATA_ITEM_SOP_ACTION_INFO,//处警标准操与处警动作关系信息add by zcy
		DATA_ITEM_ALARMTASK_INFO,    //告警任务组信息
		DATA_ITEM_ALARMTASK_RELATION_INFO,   //告警任务信息与告警任务组关系
		DATA_ITEM_FK_INFO,//访客机 
		DATA_ITEM_FK_DEPARTMENT_INFO,//访客机 
		DATA_ITEM_CUSTOM_INFO,//自定义内容
		DATA_ITEM_VEHICLE_GROUP,//车辆组
		DATA_ITEM_ALBUM_INFO,//目标库
		DATA_ITEM_WIFIPROBE_INFO,//wifi探针
		DATA_ITEM_WIFIPROBE_RECORD_INFO,
		DATA_ITEM_ALARMOUTGROUP_INFO,//报警输出组信息add by zcy
		DATA_ITEM_ALARMOUT_GROUP_RELATION_INFO,//报警输出通道与报警输出组关系信息add by zcy
		DATA_ITEM_EMAIL_INFO,//邮件信息
		DATA_ITEM_LOCAL_ALARM_IN_INFO,//本地报警输入
		DATA_ITEM_LOCAL_ALARM_OUT_INFO,//本地报警输出

		//////////////存储服务器分区相关的信息
		DATA_ITEM_MSU_PARTITION_BASE = 0x300,
		DATA_ITEM_MSU_PARTITION_STATE,//////存储服务器的分区状态信息,NET_PARTITION_INFO
		DATA_ITEM_STORAGE_MEDIA_GROUPS, ////多个存储介质组信息
		DATA_ITEM_RECORD_SCHEDULE, ///////排程录像信息
		DATA_ITEM_RECORD_TIME_SECTION_FPS, ///////时间段帧率录像信息

		DATA_ITEM_EMAP_INFO_BASE = 0x400,
		DATA_ITEM_EMAP_FILE_INFO,//电子地图的文件信息
		DATA_ITEM_EMAP_HOT_SPOT_AREA_INFO, //热点热区信息
		DATA_ITEM_EMAP_HOT_SPOT_EVENT,	//热点事件信息
		DATA_ITEM_EMAP_INFO_END = 0x4FF,

		DATA_ITEM_USER_BASE=0x500,//////////用户及用户权限的信息
		DATA_ITEM_USER_INFO,/////用户信息   对应的结构体USER_INFO
		DATA_ITEM_USER_RIGHT,/////////用户权限  NODE_ACCESS_RIGHTS

		DATA_ITEM_ALARM_BASE = 0x600, ///////////报警 与报警排程的信息
		DATA_ITEM_ALARM_TRIGGER,///////////报警联动的信息


		DATA_ITEM_TVWALL_BASE = 0x700,
		DATA_ITEM_TVWALL_INFO,				//电视墙服务器信息
		DATA_ITEM_TVWALL_OUTPUT,		//电视墙显示输出通道信息
		DATA_ITEM_TVWALL_FRAME,			//电视墙输出通道的画面的信息
		DATA_ITEM_TVWALL_LIVE,				//电视墙的现场任务配置信息
		DATA_ITEM_TVWALL_PLAN,			//电视墙计划任务配置信息
		DATA_ITEM_TVWALL_RELATE,			//电视墙画面和任务关系信息
		DATA_ITEM_TVWALL_USED , ///////////电视墙服务器的资源使用情况
		DATA_ITEM_TVWALL_PLAIN,	/////电视墙计划任务配置信息
		DATA_ITEM_OUTPUT,	 /// 
		DATA_ITEM_OUTPUT_FRAME,	 ///

		DATA_ITEM_LOG_BASE=0x800,
		DATA_ITEM_LOG_INFO,////日志信息		///////////////只读信息
		DATA_ITEM_LOG_SEARCH_INFO,		//日志查询信息


		DATA_ITEM_LOGIN_BASE = 0x900,
		DATA_ITEM_LOGIN_INFO,    /////////////ECMS_NET_LOGIN_INFO
		DATA_ITEM_LOGIN_SUCCESS,  ///////////登陆成功的结构体 ECMS_LOGIN_SUCCESS_INFO

		DATA_ITEM_DEVICE_LOCAL_CFG_BASE = 0xA00,
		DATA_ITEM_DEVICE_LOCAL_CFG_INFO,  /////////设备本地的配置信息   格式为ＸＭＬ


		DATA_ITEM_DWELL_CHANNEL = 0x10000,		//通道轮询信息
		DATA_ITEM_DWELL_GROUP,					//组轮询信息
		DATA_ITEM_LIVE_INFO,					//现场预览信息
		DATA_ITEM_CHANNEL_GROUP,				//通道组信息
		DATA_ITEM_ITEM_STATE,					//预览页状态
		DATA_ITEM_PLAY_BACK_INFO,				//回放页
		DATA_ITEM_ALARM_INFO,					//报警页
		DATA_ITEM_ALARM_CONFIG_INFO,			//报警联动配置信息
		DATA_ITEM_DWELL_SCHEME_GROUP,		    //组轮询方案信息
		DATA_ITEM_EMAP_INFO,                  //电子地图信息
		DATA_ITEM_STATE_INFO,                // 状态信息
		DATA_ITEM_CHANNEL_GROUP_RELATE,			//通道组中的关系信息，通道和通道组的关系信息，只需要支持增加和删除
		DATA_ITEM_CHANNEL_GROUP_MPR,			//通道组信息MPR专用
		DATA_ITEM_CHANNEL_PROGRAM_RELATE_MPR,	//通道与方案关系


		DATA_ITEM_BASEVALUE = 0x20000, /////////下面是本地的简单的信息保存用的ID
        DATA_ITEM_SNAP_FRAME_COUNT,/////抓取帧的数量
		DATA_ITEM_VIDEO_DISPLAY_PARAM, /////视频显示参数设置    包括 宽   高  是否显示标题
		DATA_ITEM_PATH_INFO, /////
		DATA_ITEM_ALARM_EMAP_PARAM, ///////报警电子地图的参数
		DATA_ITEM_EMAP_VIEW_SET_INFO, /////电子地图视图的设置信息
		DATA_ITEM_ALARM_PREVIEW_PARAM, //////  ALARM_PREVIEW_PARAM
		DATA_ITEM_LIVE_PREVIEW_PARAM,
		DATA_ITEM_DWELL_ITEM_INFO, //////轮询信息
		DATA_ITEM_DWELL_RELATE_INFO, /////轮询项的关系信息   ***********************************
		DATA_ITEM_PAGE_INFO, //////页面的相关信息
		DATA_ITEM_PAGE_INFO_EXTENSION,///////现场页信息的扩展 ***********************
        DATA_ITEM_DEFAULT_CH_STREAM_INFO, ////通道流的信息，ＮＶＭＳ１０００，　NVMS1200本地配置文件保存
        DATA_ITEM_SERVER_MAINTAIN_SET_INFO,	//// 服务器启动和维护设置
		DATA_ITEM_LOCAL_LOG_MAINTAIN_INFO,	//// 本地日志维护
        DATA_ITEM_ALARM_MANUAL_TRIGGER_INFO,	//// 手动触发报警设置
		DATA_ITEM_ALARM_LIST_INFO,
		DATA_ITEM_DISK_ARRAYS,                  //////磁盘阵列    对应XML的信息
		DATA_ITEM_IP,  ///////////IP信息   对应XML的信息
		DATA_ITEM_NET_PORT,  ///////////端口信息   对应XML的信息
		DATA_ITEM_DDNS,/////DDNS信息  对应XML的信息
		DATA_ITEM_UPNP,/////UPnP信息  对应XML的信息
		DATA_ITEM_FTP,/////FTP信息  对应XML的信息
		DATA_ITEM_PPPOE,/////PPPOE信息  对应XML的信息
		DATA_ITEM_P2P,//P2P信息  对应XML的信息

		DATA_ITEM_TIME, ////时间信息
		DATA_ITEM_SYSTEM_INFO, //////系统信息
		DATA_ITEM_NEW_RESOURCE_DEFAULT_RIGHT_SET_INFO,//////

        ///////////////////////////注意：   后面的定义的本地的信息，如果不涉及到动态数量变化的信息，进行使用xml，不在使用结构体了
        ////原来使用结构体的会逐渐被xml信息替代
        DATA_ITEM_RECORD_DISTRIBUTE_INFO,/////录像相关的一些分散参数的设置
		DATA_ITEM_ALARM_OUTPUT_DISTRIBUTE_INFO, //报警输出的一些分散参数的设置
		DATA_ITEM_EMAIL, //Email信息   对应XML的信息
		DATA_ITEM_WEB_LISTEN_PORT,		////Web 监听端口信息
		DATA_ITEM_FUNCTION_CUSTOMIZATION_INFO, ///系统定制功能选项
		DATA_ITEM_PRESET,		//预置点信息
		DATA_ITEM_CRUISE,		//巡航线信息
		DATA_ITEM_BLACK_WHITE_LIST, //黑白名单
		DATA_ITEM_DEV_DEFAULT_PASSWORD, //厂商默认密码

		DATA_ITEM_VOICEBROADCAST_FILE = 0x40000,///////////语音播报文件

	}DATA_ITEM_ID;


	enum OPERATOR_TYPE
	{
        OPERATOR_TYPE_RANGE_BEGIN,
		NORMAL_ITEM, /////////////没有进行操作的项
		CREATE_ITEM, /////////////新建的项
		DELETE_ITEM, /////////////删除的项
		CHANGE_ITEM,  /////////////更改的项
        OPERATOR_TYPE_RANGE_END,
	};

	typedef struct _ncfg_block_head
	{
		tuint32	biSize;		//本结构体长度
		tuint32	ItemNum;	//元素数目
		tuint32	netcfgver;	//服务器端定义的版本号，固定传输NCFG_VERSION，客户端根据需要动态判断
	}NCFG_BLOCK_HEAD;

	const size_t NCFG_BLOCK_HEAD_LENGTH = sizeof(NCFG_BLOCK_HEAD);


	typedef struct _ncfg_item_head
	{
		tuint32	itemID;		//ID
		tuint16	num;		//元素数目	
		tuint16 operatorType;///操作类型   ，OPERATOR_TYPE操作类型包括添加、删除、修改
		tuint32	subLen;		//每个元素的长度
		tuint32 	len;		//数据总长度
		GUID destNodeID;//操作对象目标ID,当为无效节点时表示没有具体目标 改成GUID用于网络传输
		//tuint32	destNodeID; //操作对象目标ID,当为无效节点时表示没有具体目标
	}NCFG_ITEM_HEAD;

	const size_t NCFG_ITEM_HEAD_LENGTH = sizeof(NCFG_ITEM_HEAD);


#pragma pack(pop)
};





#endif //CONFIGPACK_DATA_ITEM_ID_DEF_H

