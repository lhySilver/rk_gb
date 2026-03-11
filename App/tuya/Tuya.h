#ifndef __TUYA_H__
#define __TUYA_H__

#include "include/tuya_ipc_common.h"
#include "include/tuya_ipc_dp_handler.h"
#include "include/tuya_ipc_dp_utils.h"
#include "include/tuya_ipc_media_handler.h"
#include "include/tuya_ipc_motion_detect.h"
#include "include/tuya_ipc_system_control.h"


#include "include/sdk_inc/cJSON.h"
#include "include/sdk_inc/tuya_cloud_com_defs.h"
#include "include/sdk_inc/tuya_cloud_error_code.h"
#include "include/sdk_inc/tuya_cloud_types.h"
#include "include/sdk_inc/tuya_cloud_wifi_defs.h"
#include "include/sdk_inc/tuya_iot_config.h"
#include "include/sdk_inc/tuya_ipc_api.h"
#include "include/sdk_inc/tuya_ipc_cloud_storage.h"
#include "include/sdk_inc/tuya_ipc_media.h"
#include "include/sdk_inc/tuya_ipc_p2p.h"
#include "include/sdk_inc/tuya_ipc_stream_storage.h"
#include "include/sdk_inc/tuya_ring_buffer.h"
#include "include/sdk_inc/uni_network.h"
#include "include/tuya_ipc_motion_detect.h"
#include "include/sdk_inc/tuya_ipc_ai_detect_storage.h"
#include "include/sdk_inc/tuya_cloud_base_defs.h"

#include <map>
#include <string>

#include "Tuya_common.h"

class CTuya : public CThread
{
public:
	
	PATTERN_SINGLETON_DECLARE(CTuya);

	/// 线程执行体
	void ThreadProc();

	int init();
	/// 开启
	bool start();

	/// 停止
	bool stop();

	bool IsTuyaSdkStarted();
	
	void onAppEvent(std::string code, int index, appEventAction action, const EventHandler *param, Json::Value* data);

public:
	//获取设备注册激活状态
	bool GetRegisterStatus();
	//
	int ReportAnbaoLightSwich(int onoff);
	int ReportSirenSwitch(int onoff);
	int ReportOnvifIp(char *pIpAddr);
	int EventNotification(int status,int type);
	bool IsSyncUTCTime();
private:
	bool m_bTuyaSdkStarted;
	
	CTimer m_timerPushAlarm;
	CTimer m_timerPushAlarmEnd;

	bool m_bIsUpdate;
};

#define g_TuyaHandle (*CTuya::instance())



#endif// __TUYA_H__

