/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : 
** Date         : 2013-8-27
** Name         : 
** Version      : 2.0
** Description  :
** Modify Record:

ECMS配置相关的协议
1、ECMS的配置信息在客户端要采取改变既产生新的配置内容，需要重新组织以后保存到服务器去。
**************************************************************************/
   
#ifndef CONFIGPACK_STRUCT_DEF_H
#define CONFIGPACK_STRUCT_DEF_H
#include "base_type_define.h"

namespace CFGPACK_DEFINE
{
#pragma pack(push, 4)			//结构体定义为4字节对齐,在gcc里只能是1,2,4

	const int MAX_IP_COUNT=8;
	const tuint32 NAME_VALID_LENGTH = 60;/////名称有效长度
	const tuint32 NAME_BUF_DEF_LENGTH = 256; //较长名称的长度
    const tuint32 APPEND_INFO_BUF_LENGTH = 4096;
	const tuint32 TIMERATE_INFO_BUF_LENGTH = 1024;

    const tuint32   MAX_UPGRADE_FILE_SIZE = 80*1024*1024;//升级包文件大小限制80M
    const tuint32   MAX_IMPORTCONFIGDATA_FILE_SIZE = 2*1024*1024;//导入配置数据文件大小限制2M

	/************************************************************************/
	/* 新的结构体重新定义                          2013-8-27                                           */
	/************************************************************************/
	typedef struct _binary_relation
	{
		GUID  nodeID;/////节点的ID
		tuint32  nodeType;/////节点类型
		GUID  childNodeID;//////子节点的ID
		tuint32 childNodeType;/////子节点的类型
	}BINARY_RELATION;/////二元关系   二元物理关系和二元逻辑关系

	//////////////////////////////////////////////////////////////////////////
	typedef BINARY_RELATION  BINARY_RELATION_REC_SCHEDULE;//////录像的排程关系的结构体

	typedef struct _binary_relation_task_schedule
	{
		GUID  nodeID;/////节点的ID
		tuint32  nodeType;/////节点类型
		GUID  childNodeID;//////子节点的ID
		tuint64 childNodeType;/////子节点的类型
	}BINARY_RELATION_TASK_SCHEDULE;//任务排程

	typedef struct _binary_relation_alarm_trigger_schedule
	{
		GUID  nodeID;/////节点的ID
		tuint32  nodeType;/////节点类型
		GUID  childNodeID;//////子节点的ID
		tuint64 childNodeType;/////报警类型参见NVMS_Define.h enum  ECMS_ALARM_TYPE
	}BINARY_RELATION_ALARM_TRIGGER_SCHEDULE;//报警联动排程

	//////////////////////////////////////////////////////////////////////////
	typedef struct _db_version_info
	{
		tuint32   High;/////高字节 
		tuint32   Low;/////低字节
		char      szDate[16];/////数据库日期
		char      szDescription[256];////数据库描述
		GUID     Identification;/////数据库的标识 
	}DB_VERSION_INFO;

	///////////报警输入的信息
	typedef struct _res_alarm_in_info
	{
		GUID		nodeID;//////
		char		name[208]; /////
		char		GB28181ID[24];
		char		UpperGB28181ID[24];//上联国标ID
		GUID        devNodeID;
		tuint32     nodeType;////报警输入的类型
		tbyte		index;///////
		unsigned char			   bEnable;//
		unsigned char              byReserve[2];////保留
		tuint32    alarCfgType; ///常开或常闭
	}RES_ALARM_IN_INFO, *LPRES_ALARM_IN_INFO;

	enum ALARM_IN_ALARM_CFG_TYPE
	{
		ALARM_IN_ALARM_CFG_TYPE_RANGE_BEGIN,
		KEEP_OPEN,  ////常开
		KEEP_CLOSE,	////常闭
		ALARM_IN_ALARM_CFG_TYPE_RANGE_END
	};		

	///////////报警输出的信息
	typedef struct _res_alarm_out_info
	{
		GUID		nodeID;///
		char		name[256]; ///
		GUID        devNodeID;
		tuint32     nodeType;////报警输出的类型
		tbyte		index;///
		unsigned char              byReserve[3];////保留
		tuint32     delayTime; //
	}RES_ALARM_OUT_INFO, *LPRES_ALARM_OUT_INFO;

	enum AREA_TYPE
	{
		AREA_TYPE_BEGIN,
		NORMAL_AREA,			//普通区域
		GB28181_AREA,				//国标区域
		AREA_TYPE_END
	};		

	///////////区域资源节点的信息
	typedef struct _res_area_info
	{
		GUID		nodeID;//////
		char			name[252-42]; /////
		char			GAT1400Id[21]; /////1400id（外部传人） 2020-7-18从name分出来
		char			UpperGAT1400ID[21]; /////1400 id(自配置)	2020-7-18从name分出来
		tuint32			   UpdateMask;////更新标识,标明更新哪些内容
		tuint32	areaType;	//AREA_TYPE
		GUID		gbID;	//对应下级网关的guid
		char			GB28181Id[22]; /////国标id（外部传人）
		char			UpperGB28181Id[22]; /////国标id(自配置)		
		char			byReserve[4];	//预留
	}RES_AREA_INFO, *LPRES_AREA_INFO;

	///////////////////////////////////////////////////通道资源的信息
	typedef struct _res_channel_info
	{
		GUID    nodeID;//////
		GUID    deviceNodeID;///////
		tuint32              MainStream;/////通道的主码流
		tuint32              FirstSubStream;///通道的子码流
		tuint32              RecStream;//通道的录像流
		tuint32              MobileStream;////通道的手机流
		char     name[256]; ///////
        char     szAppendInfo[APPEND_INFO_BUF_LENGTH]; //

		unsigned char Index;///////通道号码从0开始
		unsigned char              byStreamCount; /////通道支持的流的数量
		unsigned char              byFaceCaptureSource;//人脸捕捉源 0: 从设备抓拍， 1： 从设备比对， 2：从服务器抓拍
		unsigned char              byReserve[1];////保留,对齐
		tuint32              KeyboardIndex;//键盘编号 add by wangxin
		tuint32			   UpdateMask;////更新标识,标明更新哪些内容
		char				 GB28181ID[64];//下级平台通道国标ID
		char				 UpperGB28181ID[64];//上联国标ID
		char			GAT1400ID[21]; /////1400 id	2020-7-18从byReserver分出来
		char			UpperGAT1400ID[21]; /////1400 id(自配置)	2020-7-18从byReserver分出来
		char	byReserver[64-42];//保留
	}RES_CHANNEL_INFO, *LPRES_CHANNEL_INFO;

    ///////////////////////////////////////////////////键盘用到
    typedef struct _res_channel_info_lite
    {
        GUID    nodeID;//////
        GUID    deviceNodeID;///////
        //tuint32              MainStream;/////通道的主码流
        //tuint32              FirstSubStream;///通道的子码流
        //tuint32              RecStream;//通道的录像流
        //tuint32              MobileStream;////通道的手机流
        //char     name[256]; ///////
        //char     szAppendInfo[APPEND_INFO_BUF_LENGTH]; //
        unsigned char Index;///////通道号码从0开始
        unsigned char              byStreamCount; /////通道支持的流的数量
		unsigned char              byFaceCaptureSource;//人脸捕捉源 0表示从IPC 1表示从服务器
        unsigned char              byReserve[1];////保留,对齐
        tuint32              KeyboardIndex;//键盘编号 add by wangxin
        tuint32			   UpdateMask;////更新标识,标明更新哪些内容
        //char				 GB28181ID[64];//下级平台通道国标ID
        //char				 UpperGB28181ID[64];//上联国标ID
        //char	byReserver[64];//保留
    }RES_CHANNEL_INFO_Lite;

	///////////////////////////////////////////////////IP通道资源的信息
	typedef struct _res_ip_channel_info
	{
		GUID    nodeID;//////
		tuint32             MainStream;/////通道的主码流
		tuint32             FirstSubStream;///通道的子码流
		tuint32             RecStream;//通道的录像流
		tuint32             MobileStream;////通道的手机流
		char                 name[256]; ///////
        char                 szAppendInfo[APPEND_INFO_BUF_LENGTH]; //
		tuint32             manufacturer;////厂商
	
		tuint16                         Index;///////通道从0开始
		unsigned char              byStreamCount; /////通道支持的流的数量	
		unsigned char              byReserve;//保留,对齐
		tuint32			   UpdateMask;////更新标识,标明更新哪些内容
		tuint32                         streamUpperLimit;	//流上限

		char     ip[64];
		char     username[64];
		char     userPWD[64];
		tuint32  nodeProtocolType;/////节点的协议类型，例如海康的IPCamera，同为的IPCamera，大华的IPCamera

		tuint16  port;
		tuint8   sensorInNum;		//Sensor输入个数
		tuint8   alarmOutNum;		//报警输出个数
	}RES_IP_CHANNEL_INFO, *LPRES_IP_CHANNEL_INFO;

	///////////////////////////////////////////////////设备资源的信息
 	typedef struct _res_device_info
 	{
 		GUID      nodeID;//////
 		char      name[256];////
 		char      ip[64];
 
 		char     username[64];
 		char     userPWD[64];
 		tuint32  nodeType;
 
 		tuint16  port;
 		tuint8   channelcount;
 		tuint8 sensorInNum;		//Sensor输入个数
 		tuint8 alarmOutNum;		//报警输出个数
		tuint8 defaultStream;	//设备默认码流 		
		unsigned char              byReserve1[2];////保留,对齐
		tuint32		   UpdateMask; //之前是unsigned char最多放到0xff大于0xff就变0了  
		unsigned char              byReserve[1];////保留

        char szAppendInfo[APPEND_INFO_BUF_LENGTH];
		unsigned char              byReserve2[3];////保留,对齐
		tuint32             manufacturer;////厂商
		GUID ChAreaGUID;		//其下通道所在的区域,当设备通道数量创建通道时使用
		GUID CreateUser;		//创建此设备的用户GUID,当设备通道数量创建通道时使用
		char     Create_Time_Str[24];

		char	url[96];
		char nodeGB28181ID[64];
		char upperNodeGB28181ID[64];
		GUID nodeCascadePlatformGUID;
		char nodeGB28181AppendInfo[1024];//原长度为4096 后面的字段都是从nodeGB28181AppendInfo分离出来
		char nodeModel[64];
		char nodeVersion[64];
		GUID nodeSopID;
		char			nodeGAT1400ID[21]; /////1400 id	2020-7-18从byReserver分出来
		char			upperNodeGAT1400ID[21]; /////1400 id(自配置) 2020-7-18从byReserver分出来
		char szRes[2928-42];
 	}RES_DEVICE_INFO, *LPRES_DEVICE_INFO;


	//键盘用到
    typedef struct _res_device_info_lite
    {
        GUID      nodeID;//////
    //	char      name[256];////
        //char      ip[64];

        //char     username[64];
        //char     userPWD[64];
        tuint32  nodeType;

        //tuint16  port;
        //tuint8   channelcount;
        //tuint8 sensorInNum;		//Sensor输入个数
        //tuint8 alarmOutNum;		//报警输出个数
        //tuint8 defaultStream;	//设备默认码流
        //unsigned char              byReserve1[2];////保留,对齐
        tuint32		   UpdateMask; //之前是unsigned char最多放到0xff大于0xff就变0了
    //	unsigned char              byReserve[1];////保留,对齐

    //	char szAppendInfo[APPEND_INFO_BUF_LENGTH];
    //	unsigned char              byReserve2[3];////保留,对齐
    //	tuint32             manufacturer;////厂商
    //	GUID ChAreaGUID;		//其下通道所在的区域,当设备通道数量创建通道时使用
    // 	GUID CreateUser;		//是谁创建了此设备,当设备通道数量创建通道时使用
    // 	char     Create_Time_Str[24];
    //
    // 	char	url[96];
    // 	char nodeGB28181ID[64];
    // 	char upperNodeGB28181ID[64];
    // 	GUID nodeCascadePlatformGUID;
    // 	char nodeGB28181AppendInfo[4096];
    }RES_DEVICE_INFO_Lite;

	/*解码器信息结构体,上面为编码设备，后续每种设备类型都新建一张表 add by wangxin*/
	typedef struct _res_decoder_info
	{
		GUID      nodeID;//////
		char      name[256];////
		char      ip[64];
		char     username[64];
		char     userPWD[64];
		tuint32  nodeType;
		tuint16  port;
		tuint8   outputcount;	
		tuint8   byReserve1;//保留,对齐
		tuint32		   UpdateMask; //之前是unsigned char最多放到0xff大于0xff就变0了  
		unsigned char              byReserve[1];////保留
		char szAppendInfo[APPEND_INFO_BUF_LENGTH];
		unsigned char              byReserve2[3];////保留,对齐
		tuint32             manufacturer;////厂商
		GUID CreateUser;		//是谁创建了此设备
	}RES_DECODER_INFO, *LPRES_DECODER_INFO;

	/*电视墙键盘信息结构体*/
	typedef struct _res_keyboard_info
	{
		GUID      nodeID;//////
		char      name[256];////
		char     username[64];
		char     userPWD[64];
		tuint32  nodeType;
		tuint32		   UpdateMask; //之前是unsigned char最多放到0xff大于0xff就变0了  
		unsigned char              byReserve[4];////保留,对齐
		GUID CreateUser;		//是谁创建了此设备
	}RES_KEYBOARD_INFO, *LPRES_KEYBOARD_INFO;

	/*电视墙墙信息结构体*/
	typedef struct _res_tvwall_info
	{
		GUID      nodeID;//////
		char      name[256];////
		char     description[256];
		tuint32  KeyboardIndex;
		tuint32		   UpdateMask; //之前是unsigned char最多放到0xff大于0xff就变0了  
		unsigned char              byReserve[4];////保留,对齐
		GUID CreateUser;		//是谁创建了此墙
	}RES_TVWALL_INFO, *LPRES_TVWALL_INFO;

	/*通道组信息结构体  文件下面_res_channel_group是以前的没有用到*/
	typedef struct _res_channelgroup_info
	{
		GUID      nodeID;//////
		char      name[256];////
		char     description[256];
		tuint32  KeyboardIndex;
		tuint32  DwellTime;
		tuint32		   UpdateMask; //之前是unsigned char最多放到0xff大于0xff就变0了  
		unsigned char              byReserve[4];////保留,对齐
		GUID CreateUser;		//是谁创建了此通道组
	}RES_CHANNELGROUP_INFO, *LPRES_CHANNELGROUP_INFO;

	/*通道与通道组信息结构体*/
	typedef struct _res_channel_group_relation_info
	{
		//GUID      nodeID;//////
		GUID      ChannelID;//////
		GUID      ChannelGroupID;//////
		tuint32  nodeIndexInGroup;
		tuint32		   UpdateMask; //之前是unsigned char最多放到0xff大于0xff就变0了  
		unsigned char              byReserve[4];////保留,对齐
	}RES_CHANNEL_GROUP_RELATION_INFO, *LPRES_CHANNEL_GROUP_RELATION_INFO;

	/*访客机信息结构体*/
	typedef struct _res_FK_info
	{
		GUID      nodeID;
		char      name[256];
		char      ip[64];
		tuint16   port;
		char      byReserve[2];//预留,对齐
		char      username[64];
		char      userPWD[64];
		tint32    nodeType;
		GUID      CreateUser;
		tuint32   UpdateMask; //之前是unsigned char最多放到0xff大于0xff就变0了
		char      res[252];//预留
	}RES_FK_INFO;

	/*访客机部门信息结构体*/
	typedef struct _res_FK_Department_info
	{
		GUID nodeID;
		GUID nodeDepartmentID;
		GUID nodeGroupID;
		GUID nodeReserved1;
		GUID nodeReserved2;
		GUID nodeReserved3;
		GUID nodeReserved4;
		tint32 nodeType;
		tuint32 DepartmentSyncID;
		tuint32 UpdateMask; //之前是unsigned char最多放到0xff大于0xff就变0了
		tuint32 nReserved1;
		tuint32 nReserved2;
		tuint32 nReserved3;
		tuint32 nReserved4;
		char cName[128];
		char cRemark[256];//预留
	}RES_FK_DEPARTMENT_INFO;

	/*报警主机信息结构体*/
	typedef struct _res_alarmhost_info
	{
		GUID      nodeID;//////
		char      name[256];////
		char      serial[64];//设备序列号
		char      ip[64];
		tuint16   port;
		char      byReserve[2];//预留,对齐
		char      username[64];
		char      userPWD[64];
		tuint32   subsystemcount;//子系统数量
		tuint32   zonecount;//防区数量
		tuint32   nodeType;//协议类型
		tuint32   hostModel;//主机型号
		GUID AreaGUID;		//所在的区域
		GUID CreateUser;		//是谁创建了此设备
		tuint32   alarmOutcount;//报警输出数量
		char      res[252];//预留
		tuint32		   UpdateMask; //之前是unsigned char最多放到0xff大于0xff就变0了  
	}RES_ALARMHOST_INFO, *LPRES_ALARMHOST_INFO;

	/*报警主机子系统信息结构体*/
	typedef struct _res_subsystem_info
	{
		GUID      nodeID;//////
		char      name[256];////
		GUID DevGUID;		//所属设备
		tuint32   index;//子系统号
		char      res[256];//预留
		tuint32		   UpdateMask; //之前是unsigned char最多放到0xff大于0xff就变0了  
	}RES_SUBSYSTEM_INFO, *LPRES_SUBSYSTEM_INFO;

	/*报警主机防区信息结构体*/
	typedef struct _res_zone_info
	{
		GUID      nodeID;//////
		char      name[256];////
		GUID SubSystemGUID;		//所属子系统设备
		GUID AlarmHostGUID;		//所属设备 配置的时候可能不属于任何子系统，所以加上此字段
		tuint32   type;//探测器类型
		tuint32   index;//防区号
		char      res[252];//预留
		tuint32   imageType;//图片类型
		tuint32		   UpdateMask; //之前是unsigned char最多放到0xff大于0xff就变0了  
	}RES_ZONE_INFO, *LPRES_ZONE_INFO;


	///////////////////////////////////////////////////门禁考勤系统信息
	typedef struct _res_acssystem_info
	{
		GUID      nodeID;//////
		char      name[256];////
		char      ip[64];
		char     username[64];
		char     userPWD[64];
		tuint32  nodeType;
		tuint16  port;
		char      byReserve[2];//预留,对齐
		char      res[256];//预留 //KANTECH门禁系统存储的是密钥key，其他系统没有用到
		GUID CreateUser;		//是谁创建了此设备,当设备通道数量创建通道时使用

		tuint32		   UpdateMask; //之前是unsigned char最多放到0xff大于0xff就变0了  
	}RES_ACSSYSTEM_INFO, *LPRES_ACSSYSTEM_INFO;

	//门禁考勤系统设备表
	typedef struct _res_acsdevice_info
	{
		GUID      nodeID;//////
		char      name[256];////
		tuint32	  status;
		char     belongModule[64];
		char     devNodelName[256];
		char     nodeIdEx[64];
		char     nodeSerialNo[64];
		GUID AcsSystemID;		//所属门禁系统
		char      res[256];//预留

		tuint32		   UpdateMask; //之前是unsigned char最多放到0xff大于0xff就变0了  
	}RES_ACSDEVICE_INFO, *LPRES_ACSDEVICE_INFO;

	//门表
	typedef struct _res_acsdoor_info
	{
		GUID      nodeID;//////
		char      name[256];////
		GUID AcsDeviceID;		//所属门禁设备
		char     nodeIdEx[64];
		char      res[256];//预留

		tuint32		   UpdateMask; //之前是unsigned char最多放到0xff大于0xff就变0了  
	}RES_ACSDOOR_INFO, *LPRES_ACSDOOR_INFO;

	//读头表
	typedef struct _res_acsreader_info
	{
		GUID      nodeID;//////
		char      name[256];////
		GUID AcsDoorID;		//所属门
		char     nodeIdEx[64];
		char     nodeReaderNo[64];
		tuint32 nodeReaderState;
		char      res[256];//预留

		tuint32		   UpdateMask; //之前是unsigned char最多放到0xff大于0xff就变0了  
	}RES_ACSREADER_INFO, *LPRES_ACSREADER_INFO;

	/****************************************************************************/

	//级联平台
	typedef struct _res_cascade_platform_info
	{
		GUID      nodeID;//////
		char      name[256];////
		char      ip[64];
		char     username[64];
		char     userPWD[64];
		tuint32  nodeType;
		tuint16  port;
		char nodeGB28181ID[64];//国标编号
		GUID CreateUser;		//是谁创建了此设备,当设备通道数量创建通道时使用
		GUID nodeArea;//所属区域

		tuint32		   UpdateMask; //之前是unsigned char最多放到0xff大于0xff就变0了  
	}RES_CASCADEPLATFORM_INFO, *LPRES_CASCADEPLATFORM_INFO;

	/////////////////////////////////////////
	typedef RES_AREA_INFO  RES_SCHEDULE_INFO;

	/************************************************************************/
	/*         排程的时间段信息的类型定义      这个结构体需要把tbyte  Weekday改为tuint32的类型               */
	/************************************************************************/
	typedef struct _res_schedule_time_section_info
	{
		GUID         nodeID; ////
		tuint32		 CycleType;//循环类型  SCHEDULE_REPEAT_MODE
		tuint32      StartTime;///开始时间，只记录单位为秒
		tuint32      EndTime; ///结束时间，只记录单位为秒
		tbyte			 Weekday;//周几
		tbyte			 dwReserved[3];//保留
	}RES_SCHEDULE_TIME_SECTION_INFO, *LPRES_SCHEDULE_TIME_SECTION_INFO;

	enum SCHEDULE_REPEAT_MODE
	{
		SCHEDULE_REPEAT_WEEK,////按照周重复
	};

	enum {SUNDAY, MONDAY, TUESDAY, WEDNESDAY, THURSDAY, FRIDAY, SATURDAY};

	/************************************************************************/
	/*         时间段帧率信息                                                            */
	/************************************************************************/
	typedef struct _res_time_frameRate_info
	{
		GUID nodeID;
		char	name[256];
		char szTimeFps[TIMERATE_INFO_BUF_LENGTH];
	}RES_TIME_FRAMERATE_INFO, *LRES_TIME_FRAMERATE_INFO;

	/************************************************************************/
	/*         语音播报文件信息                                                            */
	/************************************************************************/
	typedef struct _res_common_file_info
	{
		GUID		nodeID;//////
		char		szName[256];
		char    szFileExtension[255]; ////对应文件的后缀
		unsigned char	szReserve[5]; ///保留
	}RES_COMMON_FILE_INFO, *LPRES_COMMON_FILE_INFO;//语音播报文件新增

	/************************************************************************/
	/*         服务器信息的类型定义                                                            */
	/************************************************************************/
	typedef char SERVER_IPADDR[100];
	typedef struct _res_server_info
	{
		GUID    nodeID;//////
		char      name[256];////
		char      ip[64];

		char     username[64];
		char     userPWD[64];
		tuint32  nodeType;

		SERVER_IPADDR InIP[MAX_IP_COUNT];
		SERVER_IPADDR OutIP[MAX_IP_COUNT];

		tuint16  port;
		unsigned char ipCout;
		unsigned char isBackupServer;//是热备服务（转发，存储）
			
		tuint32 isDefault;//是否是缺省创建的服务
		tuint32 UpdateMask;//变更MASK，在信息变更时指出哪些配置栏位发生了变化
		char GB28181ID[64];
		char GB28181Password[64];
	}RES_SERVER_INFO, *LPRES_SERVER_INFO;


	/************************************************************************/
	/*         用户信息的类型定义                                                            */
	/************************************************************************/
	typedef struct _res_user_info
	{
		GUID   nodeID;/////节点的ID
		GUID   nodeGroupID;/////节点的组ID
		
		char   nodeName[64];/////用户名称
		char  nodePWD[64];/////密码
		char   nodeLimitedMAC[24];/////限制的MAc地址
		char   nodeEmail[64];/////Email
		char   nodeTel[24];/////电话
		char   nodeDescription[256];/////描述

		tuint32 nodeType;/////类型USER_INFO_TYPE
		tuint32 nodeLevel;/////用户级别USER_LEVEL
		tuint32 nodePRI;/////优先级

		tuint8 nodeMUX;/////用户复用
		tuint8   nodeIsLimitedMAC;/////是否被限制了MAC地址
		tuint8   nodeEnableAUTH;/////是否启用了权限控制
		tuint8   nodeIsLocked;/////是否被锁定
		GUID nodeCreateID;//创建者  用户--存的是创建者的用户ID 角色--存的是创建者的角色ID
	}RES_USER_INFO, *LPRES_USER_INFO;

	enum USER_LEVEL
	{
		USER_LEVEL_RANGE_BEGIN,
		INVALIDATE_LEVEL,
		OPERATOR_LEVEL,////操作员
		ADMINISTRATOR_LEVEL,/////////管理员
		SUPER_ADMINISTRATOR_LEVEL, ////超级管理员， 只有一个就是默认的那个用户为这一种类型

		DEFAULT_GROUP, ////缺省的组
		USER_DEFINE_GROUP, /////用户创建的组
		USER_LEVEL_RANGE_END,
	};

	enum USER_INFO_TYPE
	{
		USER_GROUP,///////组
		USER,/////用户
	};

	//////////////////////////////////////////////////////
	typedef struct _res_authority_info 
	{
		GUID nodeID; ///用户和组ID
		GUID otherRESID;/////节点认证服务器、转发服务器、设备节点、通道节点、报警联动组的ID
		tuint64  ullAUTHMask;//一个位代表一个权限
	}RES_AUTH_INFO, *LPRES_AUTH_INFO;

	//////////////////////////////////
	typedef struct _ternary_relation
	{
		GUID  nodeID;/////节点的ID
		GUID  childNodeID;//////子节点的ID
		GUID  contentNodeID;
	}TERNARY_RELATION;/////三元关系  

	//////////////////////////////////报警联动的结构体
	typedef struct  _res_alarm_trigger_info
	{
		GUID         nodeID; //////项的唯一标识，从1开始

		GUID			sourceNodeID;////
		tuint32         alarmType;///

		GUID            destNodeID;//报警联动节点
		tuint32         triggerType;//报警联动类型
		char         triggerInfoEx[1024]; /////联动信息的扩展参数
	}RES_ALARM_TRIGGER_INFO;

	//////////////////////////////////报警联动的结构体扩展，协议版本为6时使用这个结构体
	typedef struct  _res_alarm_trigger_info_ex
	{
		GUID         nodeID; //////项的唯一标识，从1开始

		GUID			sourceNodeID;////
		tuint32         alarmType;///

		GUID            destNodeID;//报警联动节点
		tuint32         triggerType;//报警联动类型
		char         triggerInfoEx[255]; /////联动信息的扩展参数，长度由1024减少为255，防止联动信息太多导致数据过长
	}RES_ALARM_TRIGGER_INFO_EX;
	
	///////////存储介质分组信息
	const int MAX_STORAGE_MEDIA_GROUP_COUNTS = 5;////最多支持5个
	typedef struct _res_storage_media_group 
	{
		GUID	 nodeID;////的ID
		char     name[256];

		unsigned char       byType;////STORAGE_MEDIA_GROUP_TYPE
		unsigned char       byReserve[3]; ////保留
	}RES_STORAGE_MEIDA_GROUP;

	//分区组类型
	enum STORAGE_MEDIA_GROUP_TYPE
	{
		STORAGE_MEDIA_GROUP_TYPE_NORMAL=0x00,//普通分区组
		STORAGE_MEDIA_GROUP_TYPE_DEFAULT=0x01,//默认分区组
		STORAGE_MEDIA_GROUP_TYPE_BUFFER=0x02,//缓存分区组
		STORAGE_MEDIA_GROUP_TYPE_BACKUP=0x04,//备用分区组
	};

  
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	enum NODE_CHANGE_MASK
	{
		NODE_CHANGE_MASK_NAME		=0x01,//节点名字
		NODE_CHANGE_MASK_IP_PORT	=0x02,//节点网络连接信息 IP和端口、RTSP的Url
		NODE_CHANGE_MASK_USER_INFO	=0x04,//节点用户信息 用户名、密码
		NODE_CHANGE_MASK_NO			=0x08,//修改通道序号
		NODE_CHANGE_MASK_ADDITIONAL_INFO = 0x10,	//节点附加信息    热区热点的资源ID和类型
		NODE_CHANGE_MASK_TYPE		= 0x20,		//节点具体类型 设备协议类型、门禁系统节点类型
		NODE_CHANGE_MASK_PROPERTY	= 0x40,		//节点属性 通道数量、输入数量、输出数量、子系统数量、防区数量  电子地图初始化X坐标、Y坐标、放大倍数 热区热点X坐标、Y坐标、放大倍数 ，通道组表示轮循时间,目标库表示预留字段Res1和Res2
		NODE_CHANGE_MASK_OTHER_INFO =0x80,//节点其它信息   报警主机、子系统、防区、门禁系统表表示预留/电子地图文件扩展名/热区热点文本颜色信息和图标信息/级联平台为国标ID/服务器为国标ID和密码/国标设备为国标ID、所属平台、国标附加信息
		NODE_CHANGE_MASK_AREA       =0x100,//节点区域 设备节点用到//add 
		NODE_CHANGE_MASK_KEYBOARDINDEX       =0x200,//键盘编号 通道，电视墙,通道组用到//add 
		NODE_CHANGE_MASK_DESCRIPTION       =0x400,//节点描述 电视墙、通道组用到//add 
		NODE_CHANGE_MASK_SERIAL			=0x800,//序列号，报警主机用到
		NODE_CHANGE_MASK_HOSTMODEL			=0x1000,//主机型号，报警主机用到
		NODE_CHANGE_MASK_SUBSYSTEM			=0x2000,//子系统，防区用到
		NODE_CHANGE_MASK_ATTRIBUTE			=0x4000,//用途属性，电子地图用到,修改编码设备的厂家信息也用到,修改通道的人脸抓拍源也用到
		NODE_CHANGE_MASK_SCHEDULE        =0x8000,   //告警任务排程ID  告警任务用到
		NODE_CHANGE_MASK_ALARMTYPE       = 0x10000,   //报警类型  告警任务用到
		NODE_CHANGE_MASK_TRIGGERTIME     = 0x20000,   //触发报警时间  告警任务用到
 		NODE_CHANGE_MASK_SOPID			=0x40000,//SOPID编码设备用到
		NODE_CHANGE_MASK_UPPER_GB28181ID	=0x80000,//设置UpperGBID用到
		NODE_CHANGE_MASK_GB28181INFO	=0x100000,//设置GBINFO用到
		NODE_CHANGE_MASK_TARGETTYPE 	=0x200000,//设置目标类型（人车非）
		NODE_CHANGE_MASK_MAXCOUNT	    =0x400000,//设置最大保有量阈值
		NODE_CHANGE_MASK_PARENT	    =0x800000,//设置父节点
		NODE_CHANGE_MASK_UPPER_GAT1400ID	=0x1000000,//设置UpperGAT1400ID用到
		NODE_CHANGE_MASK_GAT1400ID	=0x2000000,//设置GAT1400ID用到 1400设备报过来的id
		NODE_CHANGE_MASK_ALL		=0xffffffff,//全部节点信息//modif
	};

	///////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	///////////电子地图资源节点的信息
	typedef struct _res_emap_file_info
	{
		GUID	nodeID;//////
		char	name[256]; /////
		tuint32 nodeType;//参见TypeCheck.h _ecms_node_type_def_ 0x702静态图 0x703百度 0x704谷歌 0x705高德 0x706瓦片
		char    szFileExtension[255]; ////对应文件的后缀BMP，或者JPG等
		char    szDefaultXPos[255]; ////初始化x坐标
		char    szDefaultYPos[255]; ////初始化y坐标
		char	byReserve[3];//预留,对齐
		tuint32 defaultZoomRate;//初始化放大级别
		tuint32 attribute;//属性
		tuint32	UpdateMask;	//更新标识,标明更新哪些内容

		unsigned char	szReserve[4]; ///保留
	}RES_EMAP_FILE_INFO, *LPRES_EMAP_FILE_INFO;

	////////////////电子地图热区、热点的信息
	typedef struct _res_emap_hotspot_hotarea_info
	{	
		GUID	nodeID; ////ID
		char	name[256]; ////
		tuint32 nodeType;//0-热区 1-热点
		GUID	resourceNodeID;
		tuint32	resourceNodeType;
		char			xPos[32];///// X坐标
		char			yPos[32];//// Y坐标
		tuint32			zoomRate;//放大级别
		char			textColor[32];	//文本的颜色
		unsigned char	byImageIndex; ////图标索引
		unsigned char	byReserve[3];//预留,对齐
		char			posInfo[672];//String类型存储的位置信息
		tuint32	UpdateMask;	//更新标识,标明更新哪些内容
		unsigned char	szReserve[4]; ///保留
	}RES_EMAP_HOTSPOT_HOTAREA_INFO;

	////////////////热点 对应报警信息的过滤器
	typedef struct _hotspot_include_type_filter
	{	
		GUID nodeID; ////热点的ID
		tuint32 includeType;/////关注的报警类型
	}HOTSPOT_INCLUDE_TYPE_FILTER;


	//////////////输出的信息
	typedef struct _res_output_Info_
	{
		GUID nodeGUID;			//本节点的ID，也是输出通道的GUID
		char szName[64];			//输出通道名称
		GUID decodeDevGUID;		    //所属服务器的GUID
		unsigned char ucIndex;		//输出的索引值
		unsigned char ucModel;		//分割模式
		unsigned char ucAlarm;		//此画面1是0否为报警绑定输出
		unsigned char szOutType;			//定义一组枚举值，按照位标识输出类型
		unsigned char szResver[4];
	}RES_OUTPUT_INFO;  

	////////////输出中某一个分屏的信息
	typedef struct _res_output_frame_info
	{
		GUID nodeGUID;				//本节点的ID 
		char szName[64];			    //
		GUID outputGUID;				//所属输出的GUID
		unsigned char ucIndex;			//此画面的索引
		unsigned char ucAlarm;			//此画面1是0否为报警绑定画面
		unsigned char resver[2];		//保留字节
	}RES_OUTPUT_FRAME_INFO;

	////////////通道组
	typedef struct _res_channel_group
	{
		GUID nodeGUID;				//本节点的ID 
		char szName[64];			//
		tuint32 dwellTime_S;	    //
		unsigned char ucGroup;		//1:通道组,其他:方案
		unsigned char resver[3];	//保留字节
	}RES_CHANNEL_GROUP; 

	/***********方案的内容*************************************/
 	typedef struct _res_channel_group_item
 	{
 		GUID         ownerNodeID;        //
 		GUID         chlGUID;            //
 		tbyte        bySplitScreenIndex; //
 		tbyte		 byPageIndex;        //
 		tbyte		 byReserved[2];      //
 	}RES_CHANNEL_GROUP_ITEM, *LPRES_CHANNEL_GROUP_ITEM;


    ////写入文件末尾的长度固定为64个字节
    typedef struct _pkg_check_info /////////notice   must 64 bytes,
    {
        char version[4];    //版本号
        char structLength[4];   //本结构体长度
        char productId[8];      //
        char   pkgId[4]; //
        char   fileAuthId[32]; //FILE_AUTH_INDENTIFY
        char szReserve[12]; ///////为了凑齐64字节而加的，
    }PKG_CHECK_INFO;


    ////写入文件末尾的长度固定为64个字节
    typedef struct _export_config_data_head /////////notice   must 64 bytes,
    {
        tuint32 confVersion;    //导出的配置数据版本号
        tuint16 structLength;   //本结构体长度
        tuint16 maxChlNum;      //接入通道数
        char    hardwareVer[8]; //硬件版本号
        tuint16 alarminNum;     //报警输入数
        tuint16 alarmoutNum;    //报警输出数
        tuint32 chipID;         //加密芯片中的数字，可以唯一标示一个产品
        char chIdentify[32];	//软件唯一标识符
        tuint32 fileSize;
        char szReserve[4]; ///////为了凑齐64字节而加的，
    }NCFG_EXPORT_CONFIG_DATA_HEAD;

	///////////////////////////////////////////////////配置信息
	typedef struct _res_config_info
	{
		GUID nodeID;
		tuint32 configKey;/////配置类型
		char     configValue[2048]; //
	}RES_CONFIG_INFO, *LPRES_CONFIG_INFO;

	typedef struct _res_custom_info
	{
		GUID nodeID;
		tuint32 customIdx;//自定义索引
		char    customContent[128]; //自定义内容
	}RES_CUSTOM_INFO, *LPRES_CUSTOM_INFO;

	/*PMS相关*/
	typedef struct _binary_user_device
	{
		GUID  userGUID;
		char  userName[64];
		GUID  devGUID;
		char  devName[64];
	}BINARY_USER_DEVICE;//二元关系   二元物理关系和二元逻辑关系

	typedef struct _binary_lane_device
	{
		GUID  laneGUID;
		GUID  devGUID;
		tuint32  devType;
	}BINARY_LANE_DEVICE;//二元关系   二元物理关系和二元逻辑关系

	typedef struct _pms_fixed_vehicle_info /////////notice   must 64 bytes,
	{
		GUID nodeID;			//固定车的ID
		tuint32 nodeType;		//
		char licensePlate[64];  //车牌号
		tuint64 parkRight;		//
		tuint32 parkType;		//
		tuint32 vehicleType;	//
		char vehicleColor[64];	//
		char ownerName[64];		//
		char ownerPhone[32];	//
		tuint32 startTime;		//
		tuint32 endTime;		//
		tuint32 Surplus;		//
		char szMemo[256-16-4];	//
		GUID groupGUID;			//车辆组GUID
		tuint32	UpdateMask;	//更新标识,标明更新哪些内容
		tint32 targetId;//人员ID
	}PMS_FIXED_VEHICLE_INFO;

	typedef struct _pms_blacklist_vehicle_info
	{
		GUID nodeID;			//黑名单车的ID
		tuint32 nodeType;		//
		char licensePlate[64];  //车牌号
		tuint32 vehicleType;	//
		char vehicleColor[64];	//
		char ownerName[64];		//
		char ownerPhone[32];	//
		tuint32 enableBlock;	//
		char szMemo[256];		//
		tuint32	UpdateMask;	//更新标识,标明更新哪些内容
	}PMS_BLACKLIST_VEHICLE_INFO;

	typedef struct _pms_vehicle_group_info
	{
		GUID nodeID;			//车辆组GUID
		tuint32 nodeType;		//
		char groupName[256];	//组名称
		tuint32 groupAttribute;	//组属性：0月租车 1黑名单 2其它
		GUID scheduleGUID;		//排程GUID
		char szMemo[256];		//
		tuint32	UpdateMask;		//更新标识,标明更新哪些内容
	}PMS_VEHICLE_GROUP_INFO;

	typedef struct _pms_log_pass_record //
	{
		tuint32 nIndex;			//唯一标识
		char licensePlate[64];  //车牌号
		char licensePicPath[64];//图片路径 暂时填充的是监控点GUID
		tuint32 passTime;		//utc时间
		char parkGUID[64];		//停车场GUID
		char gatewayGUID[64];	//出入口GUID
		char laneGUID[64];		//车道GUID
		tuint32 laneType;       //车道类型1入口 2免费出口 3收费出口
		tuint32 vehicleType;	//车辆类型1小型车 2大型车 3其它
		char userGUID[64];		//客户端用户GUID
		tuint32 passMode;		//1自动通行2手动放行3未通行
		tuint32 carColor;		//0-蓝色 1-黑色 2-黄色 3-白色 4-绿色 5-红色 6-灰色 7-紫色
        tuint32 surveillance;   //停车类型 0月租车 1黑名单 2其它
		char szMemo[256-4];		//
		tuint32 parkTime;		//停车时长（秒）
		tuint32 outPark;		//当为入场记录时，该字段1表示已有对应的出场记录,0表示无对应记录，当为出场记录时，该字段无效
	}PMS_LOG_PASS_RECORD;

	typedef struct _log_visitor_record //【访客记录表】
	{
		char Log_Index[64];
		char Device_Location[255]; 
		char Device_Identity[64]; 
		char Device_Meta[255]; 
		char Id_Num[64]; 
		char Id_Name[255]; 
		tuint32 Id_Sex; 
		char Id_Folk[64]; 
		tuint32 Id_Birth; 
		char Id_Address[255];
		char Id_Depart[255]; 
		char Id_ValidTime[64]; 

		//char Id_Photo[512*1024]; 
		//char Verify_FaceImage[512*1024];
		//char Verify_SceneImage[512*1024];
        tuint32 lenPhoto;			//表示图片数据长度
		tuint64 Id_Photo;			//表示图片数据
		
        tuint32 lenFaceImage;
		tuint64 Verify_FaceImage;
		
        tuint32 lenSceneImage; 
		tuint64 Verify_SceneImage; 

		char Verify_Similarity[64];
		tuint32 Verify_SamePerson; 
		tuint32 IsRemind; 
		char Staff_ID[64]; 
		char Staff_BelongToDBId[64];
		char Staff_Name[255]; 
		char Staff_Phone[64]; 
		char Handler_ID_Entrance[64]; 
		char Handler_Name_Entrance[255]; 
		char Handler_ID_Exit[64]; 
		char Handler_Name_Exit[255]; 
		tuint32 Visit_StartTime;
		tuint32 Visit_ExpectedEndTime;
		tuint32 Visit_ActualEndTime; 
		char ReservedField[255];
		char Visitor_Num[64]; 
		char Visitor_Phone[64]; 
		tuint32 IsAllowedToVisit; 
		char AccessAuthority[255];
		char Notes[255];
		char Reserved1[255];
		char Reserved2[255];

        tuint32 getSize() {
            tuint32 len = 0;
            len += sizeof(Log_Index);
            len += sizeof(Device_Location);
            len += sizeof(Device_Identity);
            len += sizeof(Device_Meta);
            len += sizeof(Id_Num);
            len += sizeof(Id_Name);
            len += sizeof(Id_Sex);
            len += sizeof(Id_Folk);
            len += sizeof(Id_Birth);
            len += sizeof(Id_Address);
            len += sizeof(Id_Depart);
            len += sizeof(Id_ValidTime);
            len += sizeof(lenPhoto);
            len += lenPhoto;
            len += sizeof(lenFaceImage);
            len += lenFaceImage;
            len += sizeof(lenSceneImage);
            len += lenSceneImage;
            len += sizeof(Verify_Similarity);
            len += sizeof(Verify_SamePerson);
            len += sizeof(IsRemind);
            len += sizeof(Staff_ID);
            len += sizeof(Staff_BelongToDBId);
            len += sizeof(Staff_Name);
            len += sizeof(Staff_Phone);
            len += sizeof(Handler_ID_Entrance);
            len += sizeof(Handler_Name_Entrance);
            len += sizeof(Handler_ID_Exit);
            len += sizeof(Handler_Name_Exit);
            len += sizeof(Visit_StartTime);
            len += sizeof(Visit_ExpectedEndTime);
            len += sizeof(Visit_ActualEndTime);
            len += sizeof(ReservedField);
            len += sizeof(Visitor_Num);
            len += sizeof(Visitor_Phone);
            len += sizeof(IsAllowedToVisit);
            len += sizeof(AccessAuthority);
            len += sizeof(Notes);
            len += sizeof(Reserved1);
            len += sizeof(Reserved2);
            return len;
        }

        tuint8* getData() {
            tuint32 len = getSize();
            tuint8* data = new tuint8[len];
            tuint8* buf = data;

            memcpy(buf, &Log_Index, sizeof(Log_Index));
            buf += sizeof(Log_Index);

            memcpy(buf, &Device_Location, sizeof(Device_Location));
            buf += sizeof(Device_Location);

            memcpy(buf, &Device_Identity, sizeof(Device_Identity));
            buf += sizeof(Device_Identity);

            memcpy(buf, &Device_Meta, sizeof(Device_Meta));
            buf += sizeof(Device_Meta);

            memcpy(buf, &Id_Num, sizeof(Id_Num));
            buf += sizeof(Id_Num);

            memcpy(buf, &Id_Name, sizeof(Id_Name));
            buf += sizeof(Id_Name);

            memcpy(buf, &Id_Sex, sizeof(Id_Sex));
            buf += sizeof(Id_Sex);

            memcpy(buf, &Id_Folk, sizeof(Id_Folk));
            buf += sizeof(Id_Folk);

            memcpy(buf, &Id_Birth, sizeof(Id_Birth));
            buf += sizeof(Id_Birth);

            memcpy(buf, &Id_Address, sizeof(Id_Address));
            buf += sizeof(Id_Address);

            memcpy(buf, &Id_Depart, sizeof(Id_Depart));
            buf += sizeof(Id_Depart);

            memcpy(buf, &Id_ValidTime, sizeof(Id_ValidTime));
            buf += sizeof(Id_ValidTime);

            memcpy(buf, &lenPhoto, sizeof(lenPhoto));
            buf += sizeof(lenPhoto);
            memcpy(buf, (void*)Id_Photo, lenPhoto);
            buf += lenPhoto;

            memcpy(buf, &lenFaceImage, sizeof(lenFaceImage));
            buf += sizeof(lenFaceImage);
            memcpy(buf, (void*)Verify_FaceImage, lenFaceImage);
            buf += lenFaceImage;
            
            memcpy(buf, &lenSceneImage, sizeof(lenSceneImage));
            buf += sizeof(lenSceneImage);
            memcpy(buf, (void*)Verify_SceneImage, lenSceneImage);
            buf += lenSceneImage;
            
            memcpy(buf, &Verify_Similarity, sizeof(Verify_Similarity));
            buf += sizeof(Verify_Similarity);

            memcpy(buf, &Verify_SamePerson, sizeof(Verify_SamePerson));
            buf += sizeof(Verify_SamePerson);

            memcpy(buf, &IsRemind, sizeof(IsRemind));
            buf += sizeof(IsRemind);

            memcpy(buf, &Staff_ID, sizeof(Staff_ID));
            buf += sizeof(Staff_ID);

            memcpy(buf, &Staff_BelongToDBId, sizeof(Staff_BelongToDBId));
            buf += sizeof(Staff_BelongToDBId);

            memcpy(buf, &Staff_Name, sizeof(Staff_Name));
            buf += sizeof(Staff_Name);

            memcpy(buf, &Staff_Phone, sizeof(Staff_Phone));
            buf += sizeof(Staff_Phone);

            memcpy(buf, &Handler_ID_Entrance, sizeof(Handler_ID_Entrance));
            buf += sizeof(Handler_ID_Entrance);

            memcpy(buf, &Handler_Name_Entrance, sizeof(Handler_Name_Entrance));
            buf += sizeof(Handler_Name_Entrance);

            memcpy(buf, &Handler_ID_Exit, sizeof(Handler_ID_Exit));
            buf += sizeof(Handler_ID_Exit);

            memcpy(buf, &Handler_Name_Exit, sizeof(Handler_Name_Exit));
            buf += sizeof(Handler_Name_Exit);

            memcpy(buf, &Visit_StartTime, sizeof(Visit_StartTime));
            buf += sizeof(Visit_StartTime);

            memcpy(buf, &Visit_ExpectedEndTime, sizeof(Visit_ExpectedEndTime));
            buf += sizeof(Visit_ExpectedEndTime);

            memcpy(buf, &Visit_ActualEndTime, sizeof(Visit_ActualEndTime));
            buf += sizeof(Visit_ActualEndTime);

            memcpy(buf, &ReservedField, sizeof(ReservedField));
            buf += sizeof(ReservedField);

            memcpy(buf, &Visitor_Num, sizeof(Visitor_Num));
            buf += sizeof(Visitor_Num);

            memcpy(buf, &Visitor_Phone, sizeof(Visitor_Phone));
            buf += sizeof(Visitor_Phone);

            memcpy(buf, &IsAllowedToVisit, sizeof(IsAllowedToVisit));
            buf += sizeof(IsAllowedToVisit);

            memcpy(buf, &AccessAuthority, sizeof(AccessAuthority));
            buf += sizeof(AccessAuthority);

            memcpy(buf, &Notes, sizeof(Notes));
            buf += sizeof(Notes);

            memcpy(buf, &Reserved1, sizeof(Reserved1));
            buf += sizeof(Reserved1);

            memcpy(buf, &Reserved2, sizeof(Reserved2));
            buf += sizeof(Reserved2);
            return data;
        }

        bool setData(const tuint8* pData, const tint32& len)
        {
            tuint8* data = const_cast<tuint8*>(pData);
            tuint8* buf = data;

            memcpy(Log_Index, buf, sizeof(Log_Index));
            buf += sizeof(Log_Index);

            memcpy(Device_Location, buf, sizeof(Device_Location));
            buf += sizeof(Device_Location);

            memcpy(Device_Identity, buf, sizeof(Device_Identity));
            buf += sizeof(Device_Identity);
            
            memcpy(Device_Meta, buf, sizeof(Device_Meta));
            buf += sizeof(Device_Meta);

            memcpy(Id_Num, buf, sizeof(Id_Num));
            buf += sizeof(Id_Num);

            memcpy(Id_Name, buf, sizeof(Id_Name));
            buf += sizeof(Id_Name);

            memcpy(&Id_Sex, buf, sizeof(Id_Sex));
            buf += sizeof(Id_Sex);

            memcpy(Id_Folk, buf, sizeof(Id_Folk));
            buf += sizeof(Id_Folk);

            memcpy(&Id_Birth, buf, sizeof(Id_Birth));
            buf += sizeof(Id_Birth);

            memcpy(Id_Address, buf, sizeof(Id_Address));
            buf += sizeof(Id_Address);

            memcpy(Id_Depart, buf, sizeof(Id_Depart));
            buf += sizeof(Id_Depart);

            memcpy(Id_ValidTime, buf, sizeof(Id_ValidTime));
            buf += sizeof(Id_ValidTime);

            memcpy(&lenPhoto, buf, sizeof(lenPhoto));
            buf += sizeof(lenPhoto);

            void* pPhoto = new char[lenPhoto];
            memcpy(pPhoto, buf, lenPhoto);
            Id_Photo = (tuint64)pPhoto;
            buf += lenPhoto;

            memcpy(&lenFaceImage, buf, sizeof(lenFaceImage));
            buf += sizeof(lenFaceImage);

            void* pFace = new char[lenFaceImage];
            memcpy(pFace, buf, lenFaceImage);
            Verify_FaceImage = (tuint64)pFace;
            buf += lenFaceImage;

            memcpy(&lenSceneImage, buf, sizeof(lenSceneImage));
            buf += sizeof(lenSceneImage);

            void* pScene = new char[lenSceneImage];
            memcpy(pScene, buf, lenSceneImage);
            Verify_SceneImage = (tuint64)pScene;
            buf += lenSceneImage;
            

            memcpy(Verify_Similarity, buf, sizeof(Verify_Similarity));
            buf += sizeof(Verify_Similarity);

            memcpy(&Verify_SamePerson, buf, sizeof(Verify_SamePerson));
            buf += sizeof(Verify_SamePerson);

            memcpy(&IsRemind, buf, sizeof(IsRemind));
            buf += sizeof(IsRemind);

            memcpy(Staff_ID, buf, sizeof(Staff_ID));
            buf += sizeof(Staff_ID);

            memcpy(Staff_BelongToDBId, buf, sizeof(Staff_BelongToDBId));
            buf += sizeof(Staff_BelongToDBId);

            memcpy(Staff_Name, buf, sizeof(Staff_Name));
            buf += sizeof(Staff_Name);

            memcpy(Staff_Phone, buf, sizeof(Staff_Phone));
            buf += sizeof(Staff_Phone);

            memcpy(Handler_ID_Entrance, buf, sizeof(Handler_ID_Entrance));
            buf += sizeof(Handler_ID_Entrance);

            memcpy(Handler_Name_Entrance, buf, sizeof(Handler_Name_Entrance));
            buf += sizeof(Handler_Name_Entrance);

            memcpy(Handler_ID_Exit, buf, sizeof(Handler_ID_Exit));
            buf += sizeof(Handler_ID_Exit);

            memcpy(Handler_Name_Exit, buf, sizeof(Handler_Name_Exit));
            buf += sizeof(Handler_Name_Exit);

            memcpy(&Visit_StartTime, buf, sizeof(Visit_StartTime));
            buf += sizeof(Visit_StartTime);

            memcpy(&Visit_ExpectedEndTime, buf, sizeof(Visit_ExpectedEndTime));
            buf += sizeof(Visit_ExpectedEndTime);

            memcpy(&Visit_ActualEndTime, buf, sizeof(Visit_ActualEndTime));
            buf += sizeof(Visit_ActualEndTime);

            memcpy(ReservedField, buf, sizeof(ReservedField));
            buf += sizeof(ReservedField);

            memcpy(Visitor_Num, buf, sizeof(Visitor_Num));
            buf += sizeof(Visitor_Num);

            memcpy(Visitor_Phone, buf, sizeof(Visitor_Phone));
            buf += sizeof(Visitor_Phone);

            memcpy(&IsAllowedToVisit, buf, sizeof(IsAllowedToVisit));
            buf += sizeof(IsAllowedToVisit);

            memcpy(AccessAuthority, buf, sizeof(AccessAuthority));
            buf += sizeof(AccessAuthority);

            memcpy(Notes, buf, sizeof(Notes));
            buf += sizeof(Notes);

            memcpy(Reserved1, buf, sizeof(Reserved1));
            buf += sizeof(Reserved1);

            memcpy(Reserved2, buf, sizeof(Reserved2));
            buf += sizeof(Reserved2);
            return true;
        }

        void ReleaseData() {
            if (Id_Photo != 0) {
                delete[] (void*)Id_Photo;
            }
            if (Verify_FaceImage != 0) {
                delete[] (void*)Verify_FaceImage;
            }
            if (Verify_SceneImage != 0) {
                delete[] (void*)Verify_SceneImage;
            }
        }

	}LOG_VISITOR_RECORD;

	typedef struct _pms_log_pass_record_picture //
	{
		tuint32 passTime;		//秒
		tuint32 nMilliseconds;//秒后面的毫秒数
		char m_szBuf[256*1024];//JPEG
		tuint32 m_bufLen;
	}PMS_LOG_PASS_RECORD_PICTURE;

	//////////PMS图片索引的返回结构体
	typedef struct _pms_passevent_index_info
	{
		tuint32 nPassRecordIndex;//通行记录索引编号
		tuint32 nIsPictureServer;//是否使用图片服务器
		tuint32 nPicSerPort;//图片服务器的端口
		unsigned char byReserve[4];//保留
	}PMS_PASSEVENT_INDEX_INFO;

	typedef struct _pms_log_pass_charge //
	{
		tuint32 nIndex;			//
		char userName[64];		//
		char licensePlate[64];  //车牌号
		tuint32 enterTime;		//
		tuint32 outTime;		//
		tuint32 vehicleType;	//
		tuint32 chargeType;		//
		tuint32 chargeSum;		//
		char chargeRuleName[64];//
		char reduceRuleName[64];//
		tuint32 reduceSum;		//
		char parkName[64];
		char gatewayName[64];	//
		tuint32 abnormalPass;	//
		char szMemo[256];		//
	}PMS_LOG_PASS_CHARGE;

	typedef struct _pms_log_account_charge //
	{
		tuint32 nIndex;			//
		char userName[64];		//
		char licensePlate[64];  //车牌号
		tuint32 chargeTime;		//
		tuint32 chargeType;		//
		tuint32 chargeSum;		//
		tuint32 extendTime;		//
		char szMemo[256];		//
	}PMS_LOG_ACCOUNT_CHARGE;

// 	enum PARK_INFO_OPERATE_TYPE{
// 		PARK_INFO_OPERATE_TYPE_NULL=0,
// 		PARK_INFO_OPERATE_TYPE_CREATE_ITEM,
// 		PARK_INFO_OPERATE_TYPE_CHANGE_ITEM,
// 		PARK_INFO_OPERATE_TYPE_DELETE_ITEM,
// 		PARK_INFO_OPERATE_TYPE_INCREASE_REMAINPARKSAPCE,
// 		PARK_INFO_OPERATE_TYPE_DECREASE_REMAINPARKSAPCE,
// 		PARK_INFO_OPERATE_TYPE_END,
// 	};
	typedef struct _pms_park_info //
	{
		GUID nodeID;
		char parkName[64];		//
		tuint32 enable;			//
		GUID parentParkID;
		tuint32 gatewayNum;		//
		tuint32 parkingSpace;
		tuint32 remainParkingSpace;
		char szMemo[256];		//
		tuint32 index;
		GUID parkChargeRuleGUID;
		//PARK_INFO_OPERATE_TYPE type;
		tuint32	UpdateMask;	//更新标识,标明更新哪些内容
	}PMS_PARK_INFO;

	typedef struct _pms_gateway_info //
	{
		GUID nodeID;
		char gateName[64];		//
		tuint32 nIndex;			//
		GUID parkGUID;
		tuint32 laneNum;		//
		tuint32 enable;			//
		char szMemo[256];		//
		tuint32	UpdateMask;	//更新标识,标明更新哪些内容
	}PMS_GATEWAY_INFO;

	typedef struct _pms_lane_info //
	{
		GUID nodeID;
		char laneName[64];		//
		tuint32 nIndex;			//
		tuint32 enable;			//
		tuint32 tempCarIn;		//
		tuint32 laneType;		//
		tuint32 autoReleaseRule;//
		GUID gateGUID;			//
		char szMemo[256];		//
		tuint32	UpdateMask;	//更新标识,标明更新哪些内容
	}PMS_LANE_INFO;

	typedef struct _pms_led_info //
	{
		GUID      nodeID;//////
		char      name[64];////
		char      ip[64];
		tuint32  nodeType;
		tuint16  port;
 		char szAppendInfo[256];
		tuint32	UpdateMask;	//更新标识,标明更新哪些内容
	}PMS_LED_INFO;

	//自定义收费规则：时间段收费信息
	typedef struct _pms_time_section_charge_rule_info
	{
		GUID nodeID;
		GUID parentRuleID;
		char startTime[8]; //"00:00:00"格式		
		tuint32 chargeUnitTime;//计费单位时间
		char endTime[8];
		tuint32 amount;		//单位时间内的费用
		tuint32 minChargeTime;//最小计费时间
		tuint32 maxChargeAmount;//总时间内的封顶金额
		tuint32	UpdateMask;	//更新标识,标明更新哪些内容
	}PMS_TIME_SECTION_CHARGE_RULE_INFO;	//收费规则信息

	typedef struct _pms_charge_rule_info /////////notice
	{
		GUID nodeID;			//收费规则的ID
		tuint32 nodeType;		//
		tuint32 matchTempCar;//是否匹配临时车
		char ruleName[64];  //收费规则名称
		tuint32 parkType;		//
		tuint32 vehicleType;		//
		tuint32 chargeType;	//
		tuint32 dateType;	//
		float chargeAmount;
		tuint32 freeParkMinute;//minute
		tuint32 dayMaxAmount;
		tuint32 firstSectionMinute;
		tuint32 firstSectionAmount;
		tuint32 intervalMinute;		//
		tuint32 intervalAmount;		//
		char szMemo[256];		//
		tuint32	UpdateMask;	//更新标识,标明更新哪些内容
	}PMS_CHARGE_RULE_INFO;

	//包期规则
	typedef struct _pms_bag_period_rule_info
	{
		GUID nodeID;
		char ruleName[64];//包期规则名称
		tuint32 amount; //金额
		tuint32 bagTimeType;
		tuint32 bagTime;
		char description[256];//描述
		tuint32	UpdateMask;	//更新标识,标明更新哪些内容
	}PMS_BAG_PERIOD_RULE_INFO;
	//通行记录
	typedef struct _pms_pass_record_info
	{
		GUID	nodeID;///
		tuint32 logIndex;
		char licensePlate[64];
		tuint32 passtime;
		char parkName[64];
		char gatewayName[64];
		char laneName[64];
		tuint32 laneType;
		tuint32 vehicleType;
		char userName[64];
		tuint32 passMode;
		tuint32 carColor;
		tuint32 surveilllance;
		char nodename[64];
		tuint32 outPark;    //0表示没有出场，1表示出场
		char szMemo[256];
		tuint32	UpdateMask;	//更新标识,标明更新哪些内容
	}PMS_PASS_RECORD_INFO;
	/*PMS相关*/

	enum TARGET_TYPE{	
		PERSON = 0x01,
		CAR = 0x02,
		MOTOR = 0x04,
	};

	/*告警任务信息结构体*/
	typedef struct _res_alarmtask_info
	{
		GUID      nodeID;//////
		char      name[256];////
		tuint32   alarmType;//枚举ECMS_TASK_TYPE
		GUID      scheduleID;
		tuint32   triggerTime;
		char     description[256];
		tuint32		   UpdateMask; //之前是unsigned char最多放到0xff大于0xff就变0了  
		unsigned char              byReserve[4];////保留,对齐
		tuint32 targetType;//目标类型人车非 对应TARGET_TYPE
		tint32			maxCount;			// 最大保有量阈值
	}RES_ALARMTASK_INFO, *LPRES_ALARMTASK_INFO;

	/*告警任务与告警任务信息结构体*/
	typedef struct _res_alarmtask_relation_info
	{
		//GUID      nodeID;//////
		GUID      ResourceID;//////
		GUID      AlarmTaskID;//////
		tuint32		   UpdateMask; //之前是unsigned char最多放到0xff大于0xff就变0了  
		unsigned char              byReserve[4];////保留,对齐
	}RES_ALARMTASK_RELATION_INFO, *LPRES_ALARMTASK_RELATION_INFO;


	/*邮件信息结构体*/
	typedef struct _res_email_info
	{
		GUID      nodeID;//////
		char     addr[64];//邮箱地址
		char      name[64];//收发件人名称
		tuint32     type;//发件人1.收件人0
		char     senderInfo[512];//发件人服务地址端口之类的信息
		tuint32		   UpdateMask; 
	}RES_EMAIL_INFO, *LPRES_EMAIL_INFO;

	/*sop信息结构体  */
	typedef struct _res_sop_info
	{
		GUID      nodeID;//////
		char      name[256];////
		tuint32		   UpdateMask; //之前是unsigned char最多放到0xff大于0xff就变0了  
		unsigned char              byReserve[4];////保留,对齐
	}RES_SOP_INFO, *LPRES_SOP_INFO;

	/*SOP与SOP Action信息结构体*/
	typedef struct _res_sop_action_info
	{
		//GUID      nodeID;//////
		GUID      SopActionID;//////
		char      name[256];////
		tuint32  sopActionIndexInSop;
		GUID      SopID;//////
		tuint32		   UpdateMask; //之前是unsigned char最多放到0xff大于0xff就变0了  
		unsigned char              byReserve[4];////保留,对齐
	}RES_SOP_ACTION_INFO, *LPRES_SOP_ACTION_INFO;


	typedef struct _permiter_picture //
	{
		tuint32 picTime;		//秒
		tuint32 nMilliseconds;//秒后面的毫秒数
		char m_szBuf[256*1024];//JPEG
		tuint32 m_bufLen;
	}PERMITER_PICTURE;

	typedef struct
	{
		tint32 X1;   // 左上角x坐标
		tint32 Y1;   // 左上角y坐标
		tint32 X2;   // 右下角x坐标
		tint32 Y2;   // 右下角y坐标
	}RECT;

	typedef struct _traject_Rect //
	{	
		GUID	nodeChlID; //指通道节点的ID
		tuint32		count;             // 个数
		RECT		rect[32];        // 目标矩形框
		unsigned char              byReserve[128];////保留,对齐

	}TRAIECT_RECT;
	
	/*管理数据库存储的目标库信息表 */
	typedef struct _res_album_info
	{
		GUID      nodeID;//////
		char      name[256];////
		GUID      ANSServerGUID;//智能分析服务器的GUID
		GUID      FatherID;//////父分组
		char      szRes1[48];////
		char      szRes2[64];////
		tuint32		   UpdateMask; //之前是unsigned char最多放到0xff大于0xff就变0了  
	}RES_ALBUM_INFO, *LPRES_ALBUM_INFO;

	/*报警输出组信息结构体  文件下面_res_alarmout_group是以前的没有用到*/
	typedef struct _res_alarmoutgroup_info
	{
		GUID      nodeID;//////
		char      name[256];////
		char     description[256];
		tuint32		   UpdateMask; //之前是unsigned char最多放到0xff大于0xff就变0了  
		unsigned char              byReserve[4];////保留,对齐
		GUID CreateUser;		//是谁创建了此报警输出组
	}RES_ALARMOUTGROUP_INFO, *LPRES_ALARMOUTGROUP_INFO;

	/*报警输出通道与报警输出组信息结构体*/
	typedef struct _res_alarmout_group_relation_info
	{
		//GUID      nodeID;//////
		GUID      AlarmOutID;//////
		GUID      AlarmOutGroupID;//////
		tuint32  nodeIndexInGroup;
		tuint32		   UpdateMask; //之前是unsigned char最多放到0xff大于0xff就变0了  
		unsigned char              byReserve[4];////保留,对齐
	}RES_ALARMOUT_GROUP_RELATION_INFO, *LPRES_ALARMOUT_GROUP_RELATION_INFO;

	/*wifi探针*/
	typedef struct _res_WifiProbe_info
	{
		GUID nodeID;
		tint32 nodeType;
		tuint32 UpdateMask; //之前是unsigned char最多放到0xff大于0xff就变0了
		char cDevName[128];
		char cDevID[128];
		char cDevAddr[128];
		char cIP[64];//预留
		tuint32 nPort;
		char cUserName[64];
		char cPassword[64];
		char cRes[256];
	}RES_WIFIPROBE_INFO;

	typedef struct _res_WifiProbe_Record_info
	{
		GUID nodeID;
		tint32 nodeType;
		tuint32 UpdateMask; //之前是unsigned char最多放到0xff大于0xff就变0了
		char cRecordID[64];
		char cDevName[256];
		char cAddr[256];
		char cDevID[64];//设备编码
		char cWifiMAc[64];//wifi 的mac地址
		char cTime[64];//时间
		char cLon[64];
		char cLat[64];
		char cSSID[64];
		char cLeaveTime[64];
	}RES_WIFIPROBE_RECORD_INFO;

    ///////////报警输出的信息
    typedef struct _res_local_alarm_out_info
    {
        GUID		nodeID;///
        char		name[256]; ///
        GUID        devNodeID;
        tuint32     nodeType;////报警输出的类型
        tbyte		index;///
        unsigned char              byReserve[3];////保留
        tuint32     delayTime; //
    }RES_LOCAL_ALARM_OUT_INFO;

    ///////////报警输出的信息
    typedef struct _res_local_alarm_in_info
    {
        GUID		nodeID;///
        char		name[256]; ///
        GUID        devNodeID;
        tuint32     nodeType;////报警输出的类型
        tbyte		index;///
        unsigned char              byReserve[3];////保留
        tuint32     delayTime; //
    }RES_LOCAL_ALARM_IN_INFO;

#pragma pack(pop)  


};

#endif ///CONFIGPACK_STRUCT_DEF_H








