#ifndef _MSGDEF_H
#define _MSGDEF_H

#if defined(WIN32)
#pragma pack()
#endif

#include "Types/Types.h"


#define MSG_VERSION		(0x01)
#define MSG_HEADFLAG	(0xFF)

#define MSG_HEAD_LEN   (sizeof(DVRIP_MSG_HEAD_T))
#define MAX_MSG_DATA_LEN	(64*1024)


typedef struct netipmsg_h_dh            //dahua
{
	uchar		HeadFlag;				/* head flag = 0xFF */
	uchar		Version;				/* version  */
	uchar		Reserved1;				/* reserved1 */
	uchar		Reserved2;				/* reserved2 */
	uint		SID;					/* session ID */
	uint		Seq;					/* sequence number */
	union
	{
		struct
		{
			uchar	TotalPacket;		/* total packet */
			uchar	CurPacket;			/* current packet */
		}c;
		
		struct
		{
			uchar	Channel;			/* channel */
			uchar	EndFlag;			/* end flag */
		}m;
	};
	ushort		MsgId;			        /* mesage id */
	uint		DataLen;			    /* data len */
}DVRIP_MSG_HEAD_T_DH;

typedef struct netipmsg_h
{
	uchar		HeadFlag;				/* head flag = 0xFF */
	uchar		Reserved0;				/* reserved0 */
	uchar		Reserved1;				/* reserved1 */
	uchar		Reserved2;				/* reserved2 */
	uchar		Version;				/* version */
	uchar		Reserved3;				/* reserved3 */
	uchar		Reserved4;				/* reserved4 */
	uchar		Reserved5;				/* reserved5 */
	uint		SID;					/* session ID */
	uint		Seq;					/* sequence number */
	ushort		MsgId;					/* mesage id */
	union
	{
		struct
		{
			uchar	TotalPacket;		/* total packet */
			uchar	CurPacket;			/* current packet */
		}c;
		
		struct
		{
			uchar	Channel;			/* channel */
			uchar	EndFlag;			/* end flag */
		}m;
	};
	uint		DataLen;				/* data len */
}DVRIP_MSG_HEAD_T;

#if defined(WIN32)
#pragma warning (disable : 4200)
#endif

typedef enum ret_code
{
	OK = 100,
	UNKNOWNERROR = 101,		/// 未知错误
	NOTSUPPORT = 102,		/// 版本不支持
	NOTVALID = 103,			/// 非法请求
	LOGINED = 104,			/// 该用户已经登录
	UNLOGINED = 105,		/// 该用户未登录
	USERORPWDERROR = 106,	/// 用户名密码错误
	NOPOWER = 107,			/// 无权限
	TIMEOUT = 108,			/// 超时
	NOTFOUND = 109,			/// 查找失败，没有找到对应文件
	FOUND = 110,			/// 查找成功，返回全部文件
	FOUNDPART = 111,		/// 查找成功，返回部分文件
	USEREXIST = 112,		/// 该用户已经存在
	USENOTEXIST = 113,		/// 该用户不存在
	GROUPEXIST = 114,		/// 该用户组已经存在
	GROUPNOTEXIST = 115,	/// 该用户组不存在
	NEEDREBOOT = 150,		/// 成功，设备需要重启
	NETIP_ERR_MSGFORMATERR = 117,	///< 消息格式错误
	NETIP_ERR_NOPTZPROTOCOL = 118,	///< 未设置云台协议
	NETIP_ERR_NOFILEFOUND = 119,	///< 没有查询到文件
	CONFIG_NOT_ENABLE = 120,	    ///< 配置为启用
	MEDIA_CHN_NOTCONNECT = 121,			//数字通道未连接
	NATCONNET_REACHED_MAX=122,//Nat视频链接达到最大，不允许新的Nat视频链接
	TCPCONNET_REACHED_MAX=123,//Tcp视频链接达到最大，不允许新的Tcp视频链接
}RET_CODE;

#define CTRL_MSG(v) (10000 + v)

//消息类型定义
typedef enum ctrl_msg
{
//C1: 登录,登出,保活
#ifdef _NETIP_PROTOCOL_SPECIAL
	LOGIN_REQ	= CTRL_MSG(999),
#else
	LOGIN_REQ	= CTRL_MSG(1000),
#endif
	LOGIN_RSP	= CTRL_MSG(1001),
	LOGOUT_REQ = CTRL_MSG(1002),
	LOGOUT_RSP = CTRL_MSG(1003),
	FORCELOGOUT_REQ = CTRL_MSG(1004),
	FORCELOGOUT_RSP	= CTRL_MSG(1005),
	KEEPALIVE_REQ	= CTRL_MSG(1006),
	KEEPALIVE_RSP	= CTRL_MSG(1007),

	LOGIN_DAS_REQ	= CTRL_MSG(1008),
	LOGIN_DAS_RSP	= CTRL_MSG(1009),
//C2: 系统信息,存储信息
	SYSINFO_REQ	= CTRL_MSG(1020),
	SYSINFO_RSP	= CTRL_MSG(1021),

//C3: 编码配置, 遮挡,叠加,图像颜色
//C4: 录像设置
//C5: 动检,遮挡,视频丢失,外部告警,网络告警,存储告警
//C6: 通用网络配置,网络服务配置
//C7: 串口配置
//C8: 云台配置,预置点,巡航
//C9: 本地轮巡,TV调解,视频输入,输出,音频输入
//C10: 通用配置,本地化配置
//C11: 自动维护
	CONFIG_SET = CTRL_MSG(1040),
	CONFIG_SET_RSP = CTRL_MSG(1041),
	CONFIG_GET = CTRL_MSG(1042),
	CONFIG_GET_RSP = CTRL_MSG(1043),
	DEFAULT_CONFIG_GET = CTRL_MSG(1044),
	DEFAULT_CONFIG_GET_RSP = CTRL_MSG(1045),
	CONFIG_CHANNELTILE_SET = CTRL_MSG(1046),
	CONFIG_CHANNELTILE_SET_RSP = CTRL_MSG(1047),
	CONFIG_CHANNELTILE_GET = CTRL_MSG(1048),
	CONFIG_CHANNELTILE_GET_RSP = CTRL_MSG(1049),
	
	CONFIG_CHANNELTILE_DOT_SET = CTRL_MSG(1050),
	CONFIG_CHANNELTILE_DOT_SET_RSP = CTRL_MSG(1051),

	//系统调试
	SYSTEM_DEBUG_REQ = CTRL_MSG(1052),
	SYSTEM_DEBUG_RSP = CTRL_MSG(1053),
	CAMERA_DEBUG_REQ = CTRL_MSG(1054),
	CAMERA_DEBUG_RSP = CTRL_MSG(1055),

//C12: 能力级查询
	ABILITY_GET = CTRL_MSG(1360),
	ABILITY_GET_RSP = CTRL_MSG(1361),

//C13: 云台控制
	PTZ_REQ	= CTRL_MSG(1400),
	PTZ_RSP = CTRL_MSG(1401),

//C14: 监视控制
	MONITOR_REQ	= CTRL_MSG(1410),
	MONITOR_RSP	= CTRL_MSG(1411),
	MONITOR_DATA = CTRL_MSG(1412),
	MONITOR_CLAIM = CTRL_MSG(1413),
	MONITOR_CLAIM_RSP = CTRL_MSG(1414),

//C15: 回放控制
	PLAY_REQ = CTRL_MSG(1420),
	PLAY_RSP = CTRL_MSG(1421),
	PLAY_DATA = CTRL_MSG(1422),
	PLAY_EOF = CTRL_MSG(1423),
	PLAY_CLAIM = CTRL_MSG(1424),
	PLAY_CLAIM_RSP = CTRL_MSG(1425),
	DOWNLOAD_DATA = CTRL_MSG(1426),

//C16: 语音对讲控制
	TALK_REQ = CTRL_MSG(1430),
	TALK_RSP = CTRL_MSG(1431),
	TALK_CU_PU_DATA = CTRL_MSG(1432),
	TALK_PU_CU_DATA = CTRL_MSG(1433),
	TALK_CLAIM = CTRL_MSG(1434),
	TALK_CLAIM_RSP = CTRL_MSG(1435),

//C17: 文件查询包括录像文件,图片文件,日志文件
	FILESEARCH_REQ = CTRL_MSG(1440),
	FILESEARCH_RSP = CTRL_MSG(1441),
	LOGSEARCH_REQ = CTRL_MSG(1442),
	LOGSEARCH_RSP = CTRL_MSG(1443),
	FILESEARCH_BYTIME_REQ = CTRL_MSG(1444),
	FILESEARCH_BYTIME_RSP = CTRL_MSG(1445),

//C18: 系统管理
	SYSMANAGER_REQ = CTRL_MSG(1450),
	SYSMANAGER_RSP = CTRL_MSG(1451),

// 系统时间和录像模式查询
	TIMEQUERY_REQ = CTRL_MSG(1452),
	TIMEQUERY_RSP = CTRL_MSG(1453),

//C19: 硬盘管理
	DSIKMANAGER_REQ = CTRL_MSG(1460),
	DSIKMANAGER_RSP	= CTRL_MSG(1461),

//C20: 用户管理
	FULLAUTHORITYLIST_GET = CTRL_MSG(1470),
	FULLAUTHORITYLIST_GET_RSP = CTRL_MSG(1471),
	USERS_GET	= CTRL_MSG(1472),
	USERS_GET_RSP = CTRL_MSG(1473),
	GROUPS_GET = CTRL_MSG(1474),
	GROUPS_GET_RSP = CTRL_MSG(1475),
	ADDGROUP_REQ = CTRL_MSG(1476),
	ADDGROUP_RSP = CTRL_MSG(1477),
	MODIFYGROUP_REQ = CTRL_MSG(1478),
	MODIFYGROUP_RSP = CTRL_MSG(1479),
	DELETEGROUP_REQ = CTRL_MSG(1480),
	DELETEGROUP_RSP = CTRL_MSG(1481),
	ADDUSER_REQ = CTRL_MSG(1482),
	ADDUSER_RSP = CTRL_MSG(1483),
	MODIFYUSER_REQ = CTRL_MSG(1484),
	MODIFYUSER_RSP = CTRL_MSG(1485),
	DELETEUSER_REQ = CTRL_MSG(1486),
	DELETEUSER_RSP = CTRL_MSG(1487),
	MODIFYPASSWORD_REQ = CTRL_MSG(1488),
	MODIFYPASSWORD_RSP = CTRL_MSG(1489),

//C21: 布警/撤警,告警上报
	GUARD_REQ = CTRL_MSG(1500),
	GUARD_RSP = CTRL_MSG(1501),
	UNGUARD_REQ	= CTRL_MSG(1502),
	UNGUARD_RSP = CTRL_MSG(1503),
	ALARM_REQ	= CTRL_MSG(1504),
	ALARM_RSP	= CTRL_MSG(1505),
	NET_ALARM_REQ = CTRL_MSG(1506),
	NET_ALARM_RSP = CTRL_MSG(1507),
	ALARMCENTER_MSG_REQ	= CTRL_MSG(1508),
	
//C22: 系统升级控制
	UPGRADE_REQ = CTRL_MSG(1520),
	UPGRADE_RSP	 = CTRL_MSG(1521),
	UPGRADE_DATA = CTRL_MSG(1522),
	UPGRADE_DATA_RSP = CTRL_MSG(1523),
	UPGRADE_PROGRESS = CTRL_MSG(1524),
	UPGRADE_INFO_REQ = CTRL_MSG(1525),
	UPGRADE_INFO_RSQ = CTRL_MSG(1526),

//C23: 设备自动搜索
	IPSEARCH_REQ = CTRL_MSG(1530),
	IPSEARCH_RSP = CTRL_MSG(1531),
	IP_SET_REQ = CTRL_MSG(1532),
	IP_SET_RSP = CTRL_MSG(1533),

//C24: 系统信息导入导出
	CONFIG_IMPORT_REQ = CTRL_MSG(1540),
	CONFIG_IMPORT_RSP = CTRL_MSG(1541),
	CONFIG_EXPORT_REQ = CTRL_MSG(1542),
	CONFIG_EXPORT_RSP = CTRL_MSG(1543),
	LOG_EXPORT_REQ = CTRL_MSG(1544),
	LOG_EXPORT_RSP = CTRL_MSG(1545),

//C25: 网络键盘
	NET_KEYBOARD_REQ = CTRL_MSG(1550),
	NET_KEYBOARD_RSP = CTRL_MSG(1551),

//  网络抓拍
	NET_SNAP_REQ = CTRL_MSG(1560),
	NET_SNAP_RSP = CTRL_MSG(1561),

	SET_IFRAME_REQ = CTRL_MSG(1562),
	SET_IFRAME_RSP = CTRL_MSG(1563),

//  透明串口
	RS232_READ_REQ = CTRL_MSG(1570),
	RS232_READ_RSP = CTRL_MSG(1571),
	RS232_WRITE_REQ = CTRL_MSG(1572),
	RS232_WRITE_RSP = CTRL_MSG(1573),
	RS485_READ_REQ = CTRL_MSG(1574),
	RS485_READ_RSP = CTRL_MSG(1575),
	RS485_WRITE_REQ = CTRL_MSG(1576),
	RS485_WRITE_RSP = CTRL_MSG(1577),

	TRANSPARENT_COMM_REQ = CTRL_MSG(1578),
	TRANSPARENT_COMM_RSP = CTRL_MSG(1579),
	RS485_TRANSPARENT_DATA_REQ = CTRL_MSG(1580),
	RS485_TRANSPARENT_DATA_RSP = CTRL_MSG(1581),
	RS232_TRANSPARENT_DATA_REQ = CTRL_MSG(1582),
	RS232_TRANSPARENT_DATA_RSP = CTRL_MSG(1583),

//网络登录时间同步
	SYNC_TIME_REQ = CTRL_MSG(1590),
	SYNC_TIME_RSP = CTRL_MSG(1591),

//获取图片
	PHOTO_GET_REQ = CTRL_MSG(1600),
	PHOTO_GET_RSP = CTRL_MSG(1601),

//上传数据
	UPLOAD_DATA_START_REQ = CTRL_MSG(1610),
	UPLOAD_DATA_START_RSP = CTRL_MSG(1611),
	UPLOAD_DATA_STOP_REQ = CTRL_MSG(1612),
	UPLOAD_DATA_STOP_RSP = CTRL_MSG(1613),
/*1614 - 1629预留MSG，区分上传数据类型*/
	VEHICLE_INFO_REQ = CTRL_MSG(1614),
	VEHICLE_INFO_RSP = CTRL_MSG(1615),

//人数统计数据
	CPC_DATA_SEARCH_REQ = CTRL_MSG(1630),
	CPC_DATA_SEARCH_RSP = CTRL_MSG(1631),
	CPC_DATA_CLEAR_REQ = CTRL_MSG(1632),
	CPC_DATA_CLEAR_RSP = CTRL_MSG(1633),
	
}CMSG_TYPE;

#define KeepAliveCircle 20

enum NetOprResult
{
	oprOK = 0, 	/*操作ok，什么也没返回*/
	oprError,	/*操作失败*/
	oprRespData,/*操作ok，返回回复消息*/
	oprResustData,/*操作ok，返回接收结果*/
};

#endif //_MSGDEF_H

