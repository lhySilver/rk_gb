#ifndef __PRODUCE_H__
#define __PRODUCE_H__

#include "Common.h"
#include "../HttpServer/HttpServer.h"

#include "event2/http.h"
#include "event2/http_struct.h"
#include "event2/event.h"
#include "event2/buffer.h"
#include "event2/buffer_compat.h"
#include "event2/keyvalq_struct.h"

#define DEVICE_NAME "DG_Tets_Tuya"

// STRUCT_OSD_TYPE OsdType[] = { 	{IOTYPE_PRODUCT_TYPE,"ProductType"},
// 								{IOTYPE_CLOUD_PLATFORM,"CloudPlatform"},
// 								{IOTYPE_VERSION,"Version"},
// 								{IOTYPE_MAC,"MAC"},
// 								{IOTYPE_PID,"PID"},
// 								{IOTYPE_UID,"UID"},
// 								{IOTYPE_WIFI_INFO,"WifiInfo"},
// 								{IOTYPE_PRODUCT_TYPE,"ProductType"}	};

class CProduce : public CThread
{
public:
	PATTERN_SINGLETON_DECLARE(CProduce);

	typedef enum 
	{
		OSD_PRODUCT_TYPE		= 0x0330,
		OSD_CLOUD_PLATFORM		= 0x0331,
		OSD_VERSION				= 0x0340,
		OSD_MAC					= 0x0341,
		OSD_PID					= 0x0344,
		OSD_UID					= 0x0345,
		OSD_WIFI_INFO			= 0x0312,
		OSD_USER_MAX
	}ENUM_OSD_MSGTYPE;

	typedef enum 
	{
		CMD_RESET_IPC               = 1003,
		CMD_SPEAKER                 = 1011,
		CMD_WHITE_LAMP              = 1012,
		CMD_COLOR_TEM_LAMP          = 1013,
		CMD_INDICATOR_LAMP          = 1014,
		CMD_RESET_BUTTON            = 1015,
		CMD_PTZ_UP                  = 1016,
		CMD_PTZ_DOWN                = 1017,
		CMD_PTZ_LEFT                = 1018,
		CMD_PTZ_RIGHT               = 1019,
		CMD_MIC                     = 1020,
		CMD_DAY_NIGHT               = 1021,
		CMD_PIR1                = 1022,
		CMD_PIR2                = 1023,
		CMD_PIR3                = 1024,
		CMD_USER_MAX
	}ENUM_CMD_MSGTYPE;

	typedef enum 
	{
		HAND_CMD_CHANGELENS         = 8003,
		HAND_CMD_PTZ                = 8005,
	}ENUM_HAND_CMD_MSGTYPE;

	// typdef struct osd
	// {
	// 	int index;
	// 	char name[50];
	// }STRUCT_OSD_TYPE;

	/// 线程执行体
	void ThreadProc();

	int init();
	/// 开启
	bool start();
	/// 停止
	bool stop();

	///开启标定
	bool startcal();
	bool stopcal();

	void onAppEvent(std::string code, int index, appEventAction action, const EventHandler *param, Json::Value *data);

	CHttpServer::OPTIONS opt;
	int m_processRun;
	int m_processExit;
};

#define g_ProducdeHandle (*CProduce::instance())

#endif // __PRODUCE_H__
