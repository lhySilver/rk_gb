#ifndef __NVMS1000_RIGHT_DEFINE_
#define __NVMS1000_RIGHT_DEFINE_

#include <map>
#include <string>

namespace RIGHT_TYPE_DEF
{
	//监控点权限:云台控制、预览、回放、备份、录像、监控点配置
	const ULONGLONG MONITOR_SPOT_RIGHT_PTZ = 0x01;			//云台控制
	const ULONGLONG MONITOR_SPOT_RIGHT_LIVE = 0x02;			//预览
	const ULONGLONG MONITOR_SPOT_RIGHT_PLAYBACK = 0x04;		//回放
	const ULONGLONG MONITOR_SPOT_RIGHT_BACKUP = 0x08;		//备份
	const ULONGLONG MONITOR_SPOT_RIGHT_RECORD = 0x10;		//录像
	const ULONGLONG MONITOR_SPOT_RIGHT_CONFIG = 0x20;		//监控点配置

	const ULONGLONG MONITOR_SPOT_RIGHT_READ = MONITOR_SPOT_RIGHT_PTZ | MONITOR_SPOT_RIGHT_LIVE |\
											  MONITOR_SPOT_RIGHT_PLAYBACK | MONITOR_SPOT_RIGHT_BACKUP |\
											  MONITOR_SPOT_RIGHT_RECORD;
	const ULONGLONG MONITOR_SPOT_RIGHT_EDIT = MONITOR_SPOT_RIGHT_CONFIG;
	////////////

	//设备权限:与设备进行对讲、查看设备日志、设备配置
	const ULONGLONG DEVICE_RIGHT_TALKBACK = 0x40;			//对讲
	const ULONGLONG DEVICE_RIGHT_VIEW_LOG = 0x80;			//查看设备日志
	const ULONGLONG DEVICE_RIGHT_CONFIG = 0x100;			//设备配置

	const ULONGLONG DEVICE_RIGHT_READ = DEVICE_RIGHT_TALKBACK | DEVICE_RIGHT_VIEW_LOG;		//只读权限
	const ULONGLONG DEVICE_RIGHT_EDIT = DEVICE_RIGHT_CONFIG;//编辑权限
	///////////

	//电子地图:监控
	const ULONGLONG EMAP_RIGHT_LIVE = 0x01;					//监控
	/////////////////////

	//系统权限:广播、设备管理、排程管理、监控点管理、虚拟矩阵、解码设备管理、
	//搜索和导出本地日志、基本设置、设置电视墙输入、电视墙编辑、电子地图编辑
	const ULONGLONG SYSTEM_RIGHT_BROADCAST = 0x01;			//广播
	const ULONGLONG SYSTEM_RIGHT_DEVICE_MANAGE = 0x02;		//设备管理
	const ULONGLONG SYSTEM_RIGHT_SCHEDULE_MANAGE = 0x04;	//排程管理
	const ULONGLONG SYSTEM_RIGHT_MONITOR_SPOT_MANAGE = 0x08;//监控点管理
	const ULONGLONG SYSTEM_RIGHT_VIRTUAL_MATRIX = 0x10;		//虚拟矩阵
	const ULONGLONG SYSTEM_RIGHT_DECODE_DEVICE = 0x20;		//解码设备管理
	const ULONGLONG SYSTEM_RIGHT_LOG = 0x40;				//日志查看与导出
	const ULONGLONG SYSTEM_RIGHT_CONFIG = 0x80;				//基本设置
	const ULONGLONG SYSTEM_RIGHT_TVWALL_INPUT = 0x100;		//设置电视墙输入
	const ULONGLONG SYSTEM_RIGHT_TVWALL_EDIT = 0x200;		//电视墙编辑
	const ULONGLONG SYSTEM_RIGHT_EMAP_EDIT = 0x400;			//电子地图编辑
	const ULONGLONG SYSTEM_RIGHT_ALARM_MANAGE = 0x800;		//报警管理

	const ULONGLONG SYSTEM_RIGHT_READ = SYSTEM_RIGHT_BROADCAST | SYSTEM_RIGHT_LOG;
	const ULONGLONG SYSTEM_RIGHT_EDIT = SYSTEM_RIGHT_DEVICE_MANAGE | SYSTEM_RIGHT_SCHEDULE_MANAGE |\
										SYSTEM_RIGHT_MONITOR_SPOT_MANAGE | SYSTEM_RIGHT_VIRTUAL_MATRIX |\
										SYSTEM_RIGHT_DECODE_DEVICE | SYSTEM_RIGHT_CONFIG |\
										SYSTEM_RIGHT_TVWALL_INPUT | SYSTEM_RIGHT_TVWALL_INPUT |\
										SYSTEM_RIGHT_TVWALL_EDIT | SYSTEM_RIGHT_EMAP_EDIT |\
										SYSTEM_RIGHT_ALARM_MANAGE;




	// 权限定义***********************************************************************************************
	static const std::map<std::string, tuint64>::value_type RightValue[] =
	{
		//监控点权限:云台控制、预览、回放、备份、录像、监控点配置
		std::map<std::string, tuint64>::value_type("ptz", RIGHT_TYPE_DEF::MONITOR_SPOT_RIGHT_PTZ),
		std::map<std::string, tuint64>::value_type("preview", RIGHT_TYPE_DEF::MONITOR_SPOT_RIGHT_LIVE),
		std::map<std::string, tuint64>::value_type("playback", RIGHT_TYPE_DEF::MONITOR_SPOT_RIGHT_PLAYBACK),
		std::map<std::string, tuint64>::value_type("backup", RIGHT_TYPE_DEF::MONITOR_SPOT_RIGHT_BACKUP),
		std::map<std::string, tuint64>::value_type("rec", RIGHT_TYPE_DEF::MONITOR_SPOT_RIGHT_RECORD),
		std::map<std::string, tuint64>::value_type("chCfg", RIGHT_TYPE_DEF::MONITOR_SPOT_RIGHT_CONFIG),

		// 设备权限:与设备进行对讲、查看设备日志、设备配置
		std::map<std::string, tuint64>::value_type("talkback", RIGHT_TYPE_DEF::DEVICE_RIGHT_TALKBACK),
		std::map<std::string, tuint64>::value_type("devLogView", RIGHT_TYPE_DEF::DEVICE_RIGHT_VIEW_LOG),
		std::map<std::string, tuint64>::value_type("devCfg", RIGHT_TYPE_DEF::DEVICE_RIGHT_CONFIG),

		// 电子地图:监控
		std::map<std::string, tuint64>::value_type("emapLive", RIGHT_TYPE_DEF::EMAP_RIGHT_LIVE),

		// 系统权限:广播、设备管理、排程管理、监控点管理、虚拟矩阵、解码设备管理、搜索和导出本地日志
		// 、基本设置、设置电视墙输入、电视墙编辑、电子地图编辑
		std::map<std::string, tuint64>::value_type("broadcast", RIGHT_TYPE_DEF::SYSTEM_RIGHT_BROADCAST),
		std::map<std::string, tuint64>::value_type("devMgr", RIGHT_TYPE_DEF::SYSTEM_RIGHT_DEVICE_MANAGE),
		std::map<std::string, tuint64>::value_type("scheduleMgr", RIGHT_TYPE_DEF::SYSTEM_RIGHT_SCHEDULE_MANAGE),
		std::map<std::string, tuint64>::value_type("areaAndCHMgr", RIGHT_TYPE_DEF::SYSTEM_RIGHT_MONITOR_SPOT_MANAGE),
		std::map<std::string, tuint64>::value_type("virtualMatrix", RIGHT_TYPE_DEF::SYSTEM_RIGHT_VIRTUAL_MATRIX),
		std::map<std::string, tuint64>::value_type("decodeMgr", RIGHT_TYPE_DEF::SYSTEM_RIGHT_DECODE_DEVICE),
		std::map<std::string, tuint64>::value_type("localLog", RIGHT_TYPE_DEF::SYSTEM_RIGHT_LOG),
		std::map<std::string, tuint64>::value_type("basicCfg", RIGHT_TYPE_DEF::SYSTEM_RIGHT_CONFIG),
		std::map<std::string, tuint64>::value_type("tvwallInput", RIGHT_TYPE_DEF::SYSTEM_RIGHT_TVWALL_INPUT),
		std::map<std::string, tuint64>::value_type("tvwallEdit", RIGHT_TYPE_DEF::SYSTEM_RIGHT_TVWALL_EDIT),
		std::map<std::string, tuint64>::value_type("emapEdit", RIGHT_TYPE_DEF::SYSTEM_RIGHT_EMAP_EDIT),
		std::map<std::string, tuint64>::value_type("alarmMgr", RIGHT_TYPE_DEF::SYSTEM_RIGHT_ALARM_MANAGE),
	};

	static const std::map<std::string, tuint64> RightValueMap(RightValue, RightValue+22);


}

#endif //__NVMS1000_RIGHT_DEFINE_