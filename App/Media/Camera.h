#ifndef _INCLUDE_MEDIA_CAMERA_H
#define _INCLUDE_MEDIA_CAMERA_H


#include "ExchangeAL/CameraExchange.h"
#include "ExchangeAL/Exchange.h"
#include "ExchangeAL/MediaExchange.h"
#include "ExchangeAL/ExchangeKind.h"
#include <Infra/Timer.h>
#include "Infra/Thread.h"


/*单光*/
typedef enum
{
	SINGLE_IRMODE_AUTO = 0, 	//自动
	SINGLE_IRMODE_CLOSE, 		//关夜视
	SINGLE_IRMODE_OPEN, 		//开夜视
} NIGHT_VISION_MODE_SINGLE_e;

/*双光*/
typedef enum
{
	DOUBLE_IRMODE_SMART = 0, 	//智能夜视
	DOUBLE_IRMODE_FULLCOLOR, 	//全彩夜视
	DOUBLE_IRMODE_IR, 			//红外夜视
} NIGHT_VISION_MODE_DOUBLE_e;

typedef enum{
	CAMERA_MODE_NONE = 0,
	CAMERA_MODE_DAY,
	CAMERA_MODE_NIGHT,
}CAMERA_MODE_E;

//摄像头配置相关
class CCamera : public CThread
{
public:
	static CCamera* instance();

	CCamera();
	~CCamera();
	
	void SetLightMode(bool bSingleLightMode);
    bool start();
    bool stop();
	
	void ThreadProc();
	
	int configure(int chn = 0, CameraParam* pNewConfig = NULL);    //初始化

	void onConfigCamera(const CConfigTable &config, int &ret);
	void onConfigFlightWarn(const CConfigTable &table, int &ret);

	void onConfigAdcIRCut(uint arg);
	void onCdsJudgeDayNight(uint arg);
	void onSensorJudgeDayNight(uint arg);
	
	void setMode(CAMERA_MODE_E mode, bool bIrLed = true, bool bFilterMdAlarm = true);
	void setMode_ScanQrcode(bool enable);
	//获取翻转参数
	int getRotateAttr();
	
	int DoDayNightMode(int mode);
	CAMERA_MODE_E getDayNightStatus();
	
	CAMERA_MODE_E CheckDayNightByCds(int cdsValue, CAMERA_MODE_E oldStatus);
	CAMERA_MODE_E CheckDayNightBySensor();
	CAMERA_MODE_E CheckDayNight();

	//APP手动控制白光灯
	void SetWhiteLed(bool enable, int worktime = -1);
	//报警抓图推送时调用，判断是否需要延时抓图
	bool SnapCheckNeedWait(uint64 *pu64AutoOpenLightTime_ms);
	
	//外部设置报警状态
	void SetAlarmTriggerStatus(bool bTrigger);
	
	//获取白光灯状态
	bool GetWhiteLedStatus();
	bool GetDayNightStatus(void);
	int GetLinkageLightTime(void);//获取联动控灯时长 add on 2025.01.15

	//控制红外灯
	int doIrLedCtrl(bool enable);
	//控制白光灯
	int doWhiteLedCtrlSwitch(bool enable);
	int doWhiteLedCtrlBrightness(int brightness);
	//控制白光灯 true-全透	false-截止
	int doIrcutCtrl(bool enable);
	int doMirrorFlipCtrl(bool mirror, bool flip);

private:
	bool m_bSingleLightMode; //true-单光 false-双光
	CMutex	m_mutex;
	CMutex	m_mutexCamCfg; 	//相机配置锁

	CameraParamAll m_configAll;
	FlightWarnConfig m_CfgFlightWarn; 	//白光灯配置

//	CameraParam m_stCameraParam;
	CTimer	m_ircutCfgTimer;
	CTimer	m_sensorJudgeDayNightTimer;
	CAMERA_MODE_E m_eDayNightStatus; 		//表示当前所处的状态 @see CAMERA_MODE_E
	CAMERA_MODE_E m_eNewDayNightStatus; 	//表示根据当前条件判断应该所处的状态，即需要切换到的状态 @see CAMERA_MODE_E
	CAMERA_MODE_E m_eSenJudgeDayNight;
	int m_iSmartIrLastValue;
	int m_iSmartIrSwitchTime;
	CTimer	m_cdsJudgeDayNightTimer;
	int m_avg_cds_value;
	
    bool m_bAutoDayNight; 		//单光时使用，是否自动夜视
    bool m_bManualOpenWhiteLed; //双光时使用，是否手动打开白光灯
    int m_iManualOpenWhiteLedWorktime; //双光时使用，手动打开白光灯时，白光灯亮灯时长，单位秒，<=0表示一直亮

	bool m_bCurrWhiteLedStatus; 		//记录当前白光灯的开关状态
	bool m_bCurrDayNightStatus;         //记录当前白天夜视状态
//	LightWarnConfig m_CfgLightWarn; 	//白光灯配置
	uint64 m_u64AutoOpenLightTime_ms; 	//记录联动开白光灯的时间，报警抓图推送时用来判断，避免抓到绿色的图片
	
	bool m_bAlarmTrigger; 	//报警触发状态，联动白光灯
	bool m_bAlarmTurnOnWihte; 	//双光时使用，是否联动打开白光灯
	int m_iAllowbAlarmTriggerTime; //允许报警触发的时间

	int m_iAllowGetDayNightStatusTime; //开关白光灯触发的停止获取日夜状态的时间
	int m_iForceNightStatusEndTime; 	//频繁切换触发的强制夜视状态结束时间
	bool m_bCheckDayNightStartTiming; 	//检测频繁切换开始计时
	
	bool m_bCurrIrcutStatus; 		//记录当前ircut的开关状态
	bool m_bScanQrcodeMode; //是否处于二维码扫描模式

	int m_iGetLvxCount; //sensor感光，获取亮度值计数
	
	int m_last_mode;

	int m_iCheckAntiFlickerMode; //1-弱抗闪 2-强抗闪
	int m_iCurrAntiFlickerMode; //1-弱抗闪 2-强抗闪
};

#define g_Camera (*CCamera::instance())
#endif //_INCLUDE_MEDIA_CAMERA_H
