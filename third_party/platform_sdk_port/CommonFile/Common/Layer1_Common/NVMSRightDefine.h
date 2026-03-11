#ifndef __NVMS_RIGHT_DEFINE_H_
#define __NVMS_RIGHT_DEFINE_H_

#include <string>
#include <map>
//设备用的权限，N5000的权限放在头文件NVMS5000RightDefine.h中
namespace RIGHT_TYPE_DEF
{
	//////////////////////////系统权限
	//本地通道管理、远程通道管理、远程登录、磁盘管理、对讲、报警管理、网络、录像配置和管理、排程管理 、 本地系统配置与维护 、 远程系统配置与维护
	const ULONGLONG SYS_AUTH_MONITOR_SPOT_MANAGER_LOCAL = 0x02;
	const ULONGLONG SYS_AUTH_MONITOR_SPOT_MANAGER_REMOTE = 0x04;
	const ULONGLONG SYS_AUTH_DISK_MANAGER = 0x08;
	const ULONGLONG SYS_AUTH_TALKBACK = 0x10;
	const ULONGLONG SYS_AUTH_ALARM_MANAGER = 0x20;
	const ULONGLONG SYS_AUTH_NET = 0x40;
	const ULONGLONG SYS_AUTH_RECORD_CONFIG = 0x80;
	const ULONGLONG SYS_AUTH_REMOTE_LOGIN = 0x100;
	const ULONGLONG SYS_AUTH_SCHEDULE_MANAGER = 0x200;
	const ULONGLONG SYS_AUTH_SYSTEM_CONFIG_MAINTENANCE_LOCAL = 0x400;////本地系统配置与维护：日志搜索，系统基本配置，系统备份和还原
	const ULONGLONG SYS_AUTH_SYSTEM_CONFIG_MAINTENANCE_REMOTE = 0x800;////远程系统配置与维护：日志搜索，系统基本配置，系统备份和还原

	
	//监控点权限：本地预览、远程预览、本地检索及回放、远程回放、本地备份、远程备份、本地云台控制、远程云台控制、远程手动录像。
	const ULONGLONG MONITOR_SPOT_AUTH_PREVIEW_LOCAL = 0x1000;			//	本地预览
	const ULONGLONG MONITOR_SPOT_AUTH_PREVIEW_REMOTE = 0x2000;			//通过网络远程预览
	const ULONGLONG MONITOR_SPOT_AUTH_SEARCH_AND_PLAYBACK_LOCAL = 0x4000;		//本地搜索和回放
	const ULONGLONG MONITOR_SPOT_AUTH_SEARCH_AND_PLAYBACK_REMOTE = 0x8000;		//通过网络搜索和回放
	const ULONGLONG MONITOR_SPOT_AUTH_BACKUP_LOCAL = 0x10000;		//本地备份管理
	const ULONGLONG MONITOR_SPOT_AUTH_BACKUP_REMOTE = 0x20000;		//通过网络进行备份管理
	const ULONGLONG MONITOR_SPOT_AUTH_PTZ_CTRL_LOCAL = 0x40000;		//本地PTZ控制
	const ULONGLONG MONITOR_SPOT_AUTH_PTZ_CTRL_REMOTE = 0x80000;		//通过网络进行PTZ控制
	const ULONGLONG MONITOR_SPOT_AUTH_MANUAL_RECORD_REMOTE = 0x100000;		//通过网络进行手动录像控制


// 	内置3个权限组
// 		管理员：所有权限
// 		高级：
// 		系统：日志搜索、本地通道管理、远程通道管理、远程登录、对讲
// 		通道：本地预览、远程预览、本地检索及回放、远程回放、本地备份、远程备份、本地云台控制、远程云台控制、远程手动录像
// 
// 		一般：
// 		系统：对讲
// 		通道：本地预览
	const ULONGLONG AUTH_GROUP_ADMIN = 0xFFFFFFFFFFFFFFFF;
	
	const ULONGLONG AUTH_GROUP_ADVANCED = SYS_AUTH_SYSTEM_CONFIG_MAINTENANCE_LOCAL | SYS_AUTH_SYSTEM_CONFIG_MAINTENANCE_REMOTE | SYS_AUTH_MONITOR_SPOT_MANAGER_LOCAL | SYS_AUTH_MONITOR_SPOT_MANAGER_REMOTE | \
		SYS_AUTH_REMOTE_LOGIN | SYS_AUTH_TALKBACK | MONITOR_SPOT_AUTH_PREVIEW_LOCAL | MONITOR_SPOT_AUTH_PREVIEW_REMOTE | \
		MONITOR_SPOT_AUTH_SEARCH_AND_PLAYBACK_LOCAL | MONITOR_SPOT_AUTH_SEARCH_AND_PLAYBACK_REMOTE | MONITOR_SPOT_AUTH_BACKUP_LOCAL | \
		MONITOR_SPOT_AUTH_BACKUP_REMOTE | MONITOR_SPOT_AUTH_PTZ_CTRL_LOCAL | MONITOR_SPOT_AUTH_PTZ_CTRL_REMOTE | MONITOR_SPOT_AUTH_MANUAL_RECORD_REMOTE;	
	
	const ULONGLONG AUTH_GROUP_GENERAL = MONITOR_SPOT_AUTH_PREVIEW_LOCAL | SYS_AUTH_TALKBACK; 


	// 通道权限定义***********************************************************************************************
 	static const std::map<std::string, tuint64>::value_type right_value_Init_Data[] =
 	{
 	//监控点权限：本地预览、远程预览、本地检索及回放、远程回放、本地备份、远程备份、本地云台控制、远程云台控制、远程手动录像。
 		std::map<std::string, tuint64>::value_type("_lp", RIGHT_TYPE_DEF::MONITOR_SPOT_AUTH_PREVIEW_LOCAL),
 		std::map<std::string, tuint64>::value_type("@lp", RIGHT_TYPE_DEF::MONITOR_SPOT_AUTH_PREVIEW_REMOTE),
 		std::map<std::string, tuint64>::value_type("_spr", RIGHT_TYPE_DEF::MONITOR_SPOT_AUTH_SEARCH_AND_PLAYBACK_LOCAL),
 		std::map<std::string, tuint64>::value_type("@spr", RIGHT_TYPE_DEF::MONITOR_SPOT_AUTH_SEARCH_AND_PLAYBACK_REMOTE),
 		std::map<std::string, tuint64>::value_type("_bk", RIGHT_TYPE_DEF::MONITOR_SPOT_AUTH_BACKUP_LOCAL),
 		std::map<std::string, tuint64>::value_type("@bk", RIGHT_TYPE_DEF::MONITOR_SPOT_AUTH_BACKUP_REMOTE),
 
 
		std::map<std::string, tuint64>::value_type("_ptz", RIGHT_TYPE_DEF::MONITOR_SPOT_AUTH_PTZ_CTRL_LOCAL),
		std::map<std::string, tuint64>::value_type("@ptz", RIGHT_TYPE_DEF::MONITOR_SPOT_AUTH_PTZ_CTRL_REMOTE),

		std::map<std::string, tuint64>::value_type("@mr", RIGHT_TYPE_DEF::MONITOR_SPOT_AUTH_MANUAL_RECORD_REMOTE),
 	};
 
 	//////////////////////////////////////////////////////////////////////////
 	static const std::map<std::string, tuint64> g_NodeRight(right_value_Init_Data, right_value_Init_Data+_countof(right_value_Init_Data));  


}

#endif //__NVMS_RIGHT_DEFINE_H_


