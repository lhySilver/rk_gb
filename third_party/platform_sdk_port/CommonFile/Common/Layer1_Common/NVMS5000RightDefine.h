#pragma once
#include<string>
namespace RIGHT_TYPE_DEF_FOR_N5000
{
	
	//区域权限（监控点相关）:云台控制、预览、回放、备份、录像、监控点配置
	const ULONGLONG AREA_RIGHT_PTZ = 0x01;			//云台控制
	const ULONGLONG AREA_RIGHT_LIVE = 0x02;			//预览
	const ULONGLONG AREA_RIGHT_PLAYBACK = 0x04;		//回放
	const ULONGLONG AREA_RIGHT_BACKUP = 0x08;		//备份
	const ULONGLONG AREA_RIGHT_RECORD = 0x10;		//录像
	const ULONGLONG AREA_RIGHT_CHANNEL_CONFIG = 0x20;		//监控点配置
	//区域权限（设备相关）:与设备进行对讲、查看设备日志、设备配置
	const ULONGLONG AREA_RIGHT_TALKBACK = 0x40;			//对讲
	const ULONGLONG AREA_RIGHT_VIEW_LOG = 0x80;			//查看设备日志
	const ULONGLONG AREA_RIGHT_DEVICE_CONFIG = 0x100;			//设备配置
	const ULONGLONG AREA_RIGHT_SOUND = 0x200;			//视频声音

	const ULONGLONG AREA_RIGHT_READ = AREA_RIGHT_PTZ | AREA_RIGHT_LIVE |\
		AREA_RIGHT_PLAYBACK | AREA_RIGHT_BACKUP |\
		AREA_RIGHT_RECORD|AREA_RIGHT_CHANNEL_CONFIG|\
		AREA_RIGHT_TALKBACK|AREA_RIGHT_VIEW_LOG|AREA_RIGHT_DEVICE_CONFIG|AREA_RIGHT_SOUND;

	//电视墙子项的权限配置
	const ULONGLONG TVWALL_RIGHT_ITEM = 0x01;	

	//目标库子项的权限配置
	const ULONGLONG ALBUM_RIGHT_ADDTARGET = 0x01;	
	const ULONGLONG ALBUM_RIGHT_EDITTARGET = 0x02;
	const ULONGLONG ALBUM_RIGHT_DELTARGET = 0x04;
	const ULONGLONG ALBUM_RIGHT_QUERYTARGET = 0x08;

	/*系统配置权限：资源管理配置、服务器管理配置、录像管理配置、报警管理配置、用户和权限管理配置、电子地图配置、电视墙配置、系统设置配置*/
	/*系统操作权限：系统维护备份还原数据库、搜索和导出本地日志、电子地图操作、电视墙操作、目标库操作*/
	//资源管理配置
	const ULONGLONG SYSTEM_RIGHT_RESOURCE_CONFIG = 0x01;
	//服务器管理配置
	const ULONGLONG SYSTEM_RIGHT_SERVER_CONFIG = 0x02;
	//录像管理配置
	const ULONGLONG SYSTEM_RIGHT_REC_CONFIG = 0x04;
	//报警管理配置
	const ULONGLONG SYSTEM_RIGHT_ALARM_CONFIG = 0x08;	
	//用户和权限管理配置
	const ULONGLONG SYSTEM_RIGHT_USERRIGHT_CONFIG = 0x0010;
	//电子地图配置
	const ULONGLONG SYSTEM_RIGHT_EMAP_CONFIG = 0x0020;
	//电视墙配置
	const ULONGLONG SYSTEM_RIGHT_TVWALL_CONFIG = 0x0040;
	//系统设置配置
	const ULONGLONG SYSTEM_RIGHT_SYSTEM_CONFIG = 0x0080;
	/*系统操作权限*/
	//系统维护备份还原数据库
	const ULONGLONG SYSTEM_RIGHT_SYSTEM_BACKUP_STORE = 0x0100;	
	//搜索和导出本地日志
	const ULONGLONG SYSTEM_RIGHT_LOG = 0x0200;	
	//电子地图操作
	const ULONGLONG SYSTEM_RIGHT_EMAP_OPERATE = 0x0400;
	//电视墙操作
	const ULONGLONG SYSTEM_RIGHT_TVWALL_OPERATE = 0x0800;

	//新增权限必须是往后顺延
	//停车场设置配置
	const ULONGLONG SYSTEM_RIGHT_PMS_CONFIG = 0x1000;

	//new add 20190729 by zcy
	const ULONGLONG SYSTEM_RIGHT_FACEDEPLOY_CONFIG = 0x2000;
	const ULONGLONG SYSTEM_RIGHT_FACEATTEND_CONFIG = 0x4000;
	const ULONGLONG SYSTEM_RIGHT_GUEST_CONFIG = 0x8000;
	const ULONGLONG SYSTEM_RIGHT_CPC_CONFIG = 0x10000;

	const ULONGLONG SYSTEM_RIGHT_OATTEND_CONFIG = 0x20000;
	const ULONGLONG SYSTEM_RIGHT_ACCESS_CONFIG = 0x40000;
	
	const ULONGLONG SYSTEM_RIGHT_ALBUM_OPERATE = 0x80000;
	const ULONGLONG SYSTEM_RIGHT_THERMAL_IMAGE = 0x100000;

	//添加通道组操作
	const ULONGLONG SYSTEM_RIGHT_CHLGROUP_ADD_OPERATE = 0x200000;
	//删除通道组操作
	const ULONGLONG SYSTEM_RIGHT_CHLGROUP_DEL_OPERATE = 0x400000;

	const ULONGLONG SYSTEM_RIGHT_ORGANIZATION = 0x800000;//组织管理模块权限

	//系统权限
	const ULONGLONG SYSTEM_RIGHT_EDIT = SYSTEM_RIGHT_RESOURCE_CONFIG | SYSTEM_RIGHT_SERVER_CONFIG |\
		SYSTEM_RIGHT_REC_CONFIG | SYSTEM_RIGHT_ALARM_CONFIG |\
		SYSTEM_RIGHT_USERRIGHT_CONFIG | SYSTEM_RIGHT_EMAP_CONFIG |\
		SYSTEM_RIGHT_TVWALL_CONFIG | SYSTEM_RIGHT_SYSTEM_CONFIG| SYSTEM_RIGHT_PMS_CONFIG|\
		SYSTEM_RIGHT_FACEDEPLOY_CONFIG | SYSTEM_RIGHT_FACEATTEND_CONFIG | SYSTEM_RIGHT_GUEST_CONFIG |\
		SYSTEM_RIGHT_CPC_CONFIG| SYSTEM_RIGHT_OATTEND_CONFIG | SYSTEM_RIGHT_ACCESS_CONFIG | SYSTEM_RIGHT_THERMAL_IMAGE|\
		SYSTEM_RIGHT_ORGANIZATION;

	//操作权限
	const ULONGLONG SYSTEM_RIGHT_READ = SYSTEM_RIGHT_LOG | SYSTEM_RIGHT_SYSTEM_BACKUP_STORE |\
		SYSTEM_RIGHT_EMAP_OPERATE | SYSTEM_RIGHT_TVWALL_OPERATE | SYSTEM_RIGHT_ALBUM_OPERATE |\
		SYSTEM_RIGHT_CHLGROUP_ADD_OPERATE | SYSTEM_RIGHT_CHLGROUP_DEL_OPERATE;


	// 区域权限定义***********************************************************************************************
	static const std::map<std::string, tuint64>::value_type right_value_Init_Data_ForN5000[] =
	{
		//区域权限:云台控制、预览、回放、备份、录像、监控点配置、与设备进行对讲、查看设备日志、设备配置
		std::map<std::string, tuint64>::value_type("crc", RIGHT_TYPE_DEF_FOR_N5000::AREA_RIGHT_CHANNEL_CONFIG),
		std::map<std::string, tuint64>::value_type("drc", RIGHT_TYPE_DEF_FOR_N5000::AREA_RIGHT_DEVICE_CONFIG),
		std::map<std::string, tuint64>::value_type("dt", RIGHT_TYPE_DEF_FOR_N5000::AREA_RIGHT_TALKBACK),
		std::map<std::string, tuint64>::value_type("vdl", RIGHT_TYPE_DEF_FOR_N5000::AREA_RIGHT_VIEW_LOG),
		std::map<std::string, tuint64>::value_type("ptz", RIGHT_TYPE_DEF_FOR_N5000::AREA_RIGHT_PTZ),
		std::map<std::string, tuint64>::value_type("lp", RIGHT_TYPE_DEF_FOR_N5000::AREA_RIGHT_LIVE),
		std::map<std::string, tuint64>::value_type("spr", RIGHT_TYPE_DEF_FOR_N5000::AREA_RIGHT_PLAYBACK),
		std::map<std::string, tuint64>::value_type("bk", RIGHT_TYPE_DEF_FOR_N5000::AREA_RIGHT_BACKUP),
		std::map<std::string, tuint64>::value_type("rec", RIGHT_TYPE_DEF_FOR_N5000::AREA_RIGHT_RECORD),
		std::map<std::string, tuint64>::value_type("sd", RIGHT_TYPE_DEF_FOR_N5000::AREA_RIGHT_SOUND),

	};

	static const std::map<std::string,tuint64>::value_type right_value_InitTvwall_Data[] =
	{
		std::map<std::string, tuint64>::value_type("tvitem", RIGHT_TYPE_DEF_FOR_N5000::TVWALL_RIGHT_ITEM),
	};

	//目标库子权限定义
	static const std::map<std::string, tuint64>::value_type right_value_InitAlbumData[] =
	{
		std::map<std::string, tuint64>::value_type("addtarget", RIGHT_TYPE_DEF_FOR_N5000::ALBUM_RIGHT_ADDTARGET),
		std::map<std::string, tuint64>::value_type("edittarget", RIGHT_TYPE_DEF_FOR_N5000::ALBUM_RIGHT_EDITTARGET),
		std::map<std::string, tuint64>::value_type("deltarget", RIGHT_TYPE_DEF_FOR_N5000::ALBUM_RIGHT_DELTARGET),
		std::map<std::string, tuint64>::value_type("querytarget", RIGHT_TYPE_DEF_FOR_N5000::ALBUM_RIGHT_QUERYTARGET),

	};

	//////////////////////////////////////////////////////////////////////////
	static const std::map<std::string, tuint64> g_NodeRightForN5000(right_value_Init_Data_ForN5000, right_value_Init_Data_ForN5000+_countof(right_value_Init_Data_ForN5000));  
	static const std::map<std::string, tuint64> g_NodeTvWallRightForN5000(right_value_InitTvwall_Data, right_value_InitTvwall_Data+_countof(right_value_InitTvwall_Data));  
	static const std::map<std::string, tuint64> g_NodeAlbumRightForN5000(right_value_InitAlbumData, right_value_InitAlbumData+_countof(right_value_InitAlbumData));  
}
