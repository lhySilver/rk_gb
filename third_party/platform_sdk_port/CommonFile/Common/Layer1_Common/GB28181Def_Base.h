#ifndef __GBT28181_DEF_BASE_H__
#define __GBT28181_DEF_BASE_H__


#pragma once
#include <string>

using namespace std;

#define UM_ALARM			(WM_USER + 0x8005)	//报警
#define UM_CONNECTSTATE		(WM_USER + 0x8016)	//通道在线状态
#define UM_STREAMSTOP		(WM_USER + 0x8030)	//流结束
//视频流消息
#define UM_LIVESTREAM		(WM_USER + 0x8011)
//历史回放消息
#define UM_PLAYBACK			(WM_USER + 0x8012)	
#define UM_PLAYBACKRUN		(WM_USER + 0x8013)
#define UM_PLAYBACKPAUSE	(WM_USER + 0x8014)
#define UM_PLAYBACKSTOP		(WM_USER + 0x8015)
//与注册服务器连接断开
#define UM_DELETEALLCHANNEL	(WM_USER + 0x8017)
//上级联网单元重新启动
#define UM_ADDALLCHANNEL	(WM_USER + 0x8018)
//删除视频流
#define UM_DELETELIVESTREAM	(WM_USER + 0x8019)
//删除历史流
#define UM_DELETEPLAYBACK	(WM_USER + 0x8020)
//对讲流消息
#define UM_TALKSTREAM		(WM_USER + 0x8021)
//删除对讲流
#define UM_DELETETALKSTREAM	(WM_USER + 0x8022)
//目录检测发送
#define CATALOGTIME		200

#define WAITTIME		50				//SIP事件监听时间间隔
#define LIVETIME		(90 * 1000)		//保活时间
#define HEARTBEATTIME	(60 * 1000)		//心跳间隔

//XML配置
struct tagIP
{
	string strIP;
};

struct tagSip
{
	string strUser;
	string strPwd;
	string strPort;
};

struct tagRtp
{
	string strPort;
	string min_port;
	string max_port;
};

struct tagRtcp
{
	string strPort;
};

struct tagUpperNetUnit
{
	string strUser;
	string strHost;
	string strPort;
};

struct tagEcms
{
	string strUser;
	string strPwd;
	string strIP;
	string strPort;
};

struct tagCode
{
	string strArea;		//中心编码（8位）
	string strStreet;	//行业编码（2位）
	string strPoint;	//类型编码（3位）
	string strDevice;	//序号（7位）
};

struct tagStream
{
	string strStream;
	bool frame_extraction;
};

struct tagPayload
{
	string strVideo;
	string strAudio;
};

struct tagProtocol
{
        string  proco;
};

struct tagPublic
{
	string  ip;
	string  use;
};

struct tagAlarmSubSwitch
{
	string  alarmsubSwitch;
};

struct tagXmlConfig 
{
	tagIP ip;
	tagSip sip;
	tagRtp rtp;
	tagRtcp rtcp;
	tagUpperNetUnit upper;
	tagEcms ecms;
	tagCode code;
	tagStream stream;
	tagPayload payload;
	tagPublic   public_param; //公网配置
	tagAlarmSubSwitch alarm_subscribe_switch;

	std::string encoding;
	std::string need_send_device_node;
	tagProtocol protocol;
	int regist_expire;
	int regist_repeat;
	bool bNeedModifyAreaGBID;
	bool enable_notify;

	tagXmlConfig()
	{
		ip.strIP = "127.0.0.1";

		sip.strUser = "44030500042030900000";
		sip.strPwd = "12345678";
		sip.strPort = "5061";

		rtp.strPort = "2000";
		rtp.min_port = "6000";
		rtp.max_port = "20000";

		rtcp.strPort = "554";

		upper.strUser = "00000000042030000001";
		upper.strHost = "127.0.0.1";
		upper.strPort = "5060";

		ecms.strUser = "test1";
		ecms.strPwd = "test1";
		ecms.strIP = "127.0.0.1";
		ecms.strPort = "6003";

		code.strArea = "44030500";
		code.strStreet = "04";
		code.strPoint = "203";
		code.strDevice = "0900000";

		stream.strStream = "TS";
		stream.frame_extraction = true;
		payload.strVideo = "96";
		payload.strAudio = "0";
		protocol.proco = "UDP";

		public_param.ip = "127.0.0.1";
		public_param.use = "false"; 

		encoding = "utf-8";
		need_send_device_node = "true";

		regist_expire = 3600000;
		regist_repeat = 10000;

		bNeedModifyAreaGBID = false;
		enable_notify = false;
	}
};
#endif//__GBT28181_DEF_BASE_H__
