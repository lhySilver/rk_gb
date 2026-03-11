/*********************************************************************************
  *Copyright(C),2015-2020, 涂鸦科技 www.tuya.comm
  *FileName: tuya_ipc_dp_handler.c
  *
  * 文件描述：
  * 1. DP点设置和获取函数API实现
  *
  * 开发者工作：
  * 1. 本地配置的获取与更新。
  * 2. 设置本地IPC属性，比如画面翻转，时间水印等。开发者可以根据IPC的硬件特性选择实现；
  *    如果有特性不支持，函数留空即可。
  *
**********************************************************************************/
#include "include/tuya_ipc_dp_utils.h"
#include "include/tuya_ipc_dp_handler.h"
#include "include/sdk_inc/tuya_ipc_stream_storage.h"
#include "include/sdk_inc/tuya_devos_netlink.h"
#include "include/sdk_inc/app_agent.h"
#include "include/sdk_inc/tuya_os_adapter.h"
#include "include/sdk_inc/tuya_hal_wifi.h"
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>


#include "Tuya.h"

extern int iAllowMotorTrackTime; 	//允许移动追踪时间点

extern int pb_handler_g;



unsigned char bStartPrivateMode; 		//是否开启隐私模式


#ifdef TUYA_DP_SLEEP_MODE
//STATIC BOOL_T s_sleep_mode;
PtzPosition_s pstPtzPosition_sleep;

VOID IPC_APP_set_sleep_mode(BOOL_T sleep_mode)
{
    AppWarning("set sleep_mode:%d \r\n", sleep_mode);
    //TODO
    /* 休眠,BOOL类型,true休眠,false不休眠 */
    
	bool re;
	Json::Value table;
	//int status = 0;
	re = g_configManager.getConfig(getConfigName(CFG_PRIVATE), table);
	AppWarning("get CFG_PRIVATE config %s\n", re ? "succ" : "failed");
	AppWarning("CFG_PRIVATE status %d\n",table["Status"].asInt());
	//status = table["Status"].asInt();
	if (sleep_mode)
	{
		g_Alarm.SetAllowMotionDetTime(time(NULL)+10);
		table["Status"] = 1;
		bStartPrivateMode = 1;
		if(ProductCof_g.private_motorstatus == 1)
		{
			g_PtzHandle.GetPosition(&pstPtzPosition_sleep);
			g_PtzHandle.DoMotorMove_private(PTZ_UP);
		}
		else if(ProductCof_g.private_motorstatus == 2)
		{
			g_PtzHandle.GetPosition(&pstPtzPosition_sleep);
			g_PtzHandle.DoMotorMove_private(PTZ_DOWN);
		}
	}
    else
    {
		g_Alarm.SetAllowMotionDetTime(time(NULL)+10);
        table["Status"] = 0;
		if(ProductCof_g.private_motorstatus != 0)
			g_PtzHandle.GotoPosition(pstPtzPosition_sleep.x,pstPtzPosition_sleep.y);
		bStartPrivateMode = 0;
    }
    
    re = ((g_configManager.setConfig(getConfigName(CFG_PRIVATE), table, 0, IConfigManager::applyOK)) & IConfigManager::applyFileError)==0?true:false;
    AppWarning("set CFG_PRIVATE config %s\n", re ? "succ" : "failed");
//	s_sleep_mode = sleep_mode;
}

BOOL_T IPC_APP_get_sleep_mode(VOID)
{
//    BOOL_T sleep_mode = s_sleep_mode;
//    AppWarning("curr sleep_mode:%d \r\n", sleep_mode);
//    return sleep_mode;
        
    bool re;
    Json::Value table;
    int status = 0;
    re = g_configManager.getConfig(getConfigName(CFG_PRIVATE), table);
    AppWarning("get CFG_PRIVATE config %s\n", re ? "succ" : "failed");
    AppWarning("CFG_PRIVATE status %d\n",table["Status"].asInt());
    status = table["Status"].asInt();
    if (1 == status)
    {        
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}
#endif

//------------------------------------------

#ifdef TUYA_DP_LIGHT
VOID IPC_APP_set_light_onoff(BOOL_T light_on_off)
{
	AppWarning("set light_on_off:%d \r\n", light_on_off);
    //TODO
    /* 状态指示灯,BOOL类型,true打开,false关闭 */

	bool ret;
	Json::Value table;
	ret = g_configManager.getConfig(getConfigName(CFG_LIGHT), table);
	AppWarning("get (CFG_LIGHT) config %s\n", ret ? "succ" : "failed");
	AppWarning("(CFG_LIGHT) status %d\n",table["Light"].asInt());
	
	table["Light"] = light_on_off ? 1 : 0;
	ret = ((g_configManager.setConfig(getConfigName((CFG_LIGHT)), table, 0, IConfigManager::applyOK)) & IConfigManager::applyFileError)==0?true:false;
	AppWarning("set (CFG_LIGHT) config %s\n", ret ? "succ" : "failed");
}

BOOL_T IPC_APP_get_light_onoff(VOID)
{
    BOOL_T light_on_off;
	
	Json::Value table;
	g_configManager.getConfig(getConfigName(CFG_LIGHT), table);
	
	light_on_off = table["Light"].asInt();
    AppWarning("curr light_on_off:%d \r\n", light_on_off);
	
    return light_on_off;
}
#endif


#ifdef TUYA_DP_AI_FACE_DET
STATIC BOOL_T s_ai_on_off;
VOID IPC_APP_set_ai_onoff(BOOL_T ai_on_off)
{
    AppWarning("set ai_on_off:%d \r\n", ai_on_off);
    //TODO
    /* ai,BOOL类型,true打开,false关闭 */
    
	bool re;
	Json::Value table;
	//int status = 0;
	re = g_configManager.getConfig(getConfigName(CFG_AIONOFF), table);
	AppWarning("get CFG_AIONOFF config %s\n", re ? "succ" : "failed");
	AppWarning("CFG_AIONOFF status %d\n",table["Status"].asInt());
	//status = table["Status"].asInt();
	if (ai_on_off)
	{
		table["Status"] = 1;
	}
    else
    {
        table["Status"] = 0;
    }
    
    re = ((g_configManager.setConfig(getConfigName(CFG_AIONOFF), table, 0, IConfigManager::applyOK)) & IConfigManager::applyFileError)==0?true:false;
    AppWarning("set CFG_AIONOFF config %s\n", re ? "succ" : "failed");
	s_ai_on_off = ai_on_off;
}

BOOL_T IPC_APP_get_ai_onoff(VOID)
{
//    BOOL_T ai_on_off = s_ai_on_off;
//    AppWarning("curr ai_on_off:%d \r\n", ai_on_off);

    
	bool re;
	Json::Value table;
	int status = 0;
	re = g_configManager.getConfig(getConfigName(CFG_AIONOFF), table);
	AppWarning("get CFG_AIONOFF config %s\n", re ? "succ" : "failed");
	AppWarning("CFG_AIONOFF status %d\n",table["Status"].asInt());
	status = table["Status"].asInt();
	if (1 == status)
	{
        return TRUE;
	}
    else
    {
        return FALSE;
    }
//    return ai_on_off;
}
#endif


//------------------------------------------

#ifdef TUYA_DP_FLIP
VOID IPC_APP_set_flip_onoff(BOOL_T flip_on_off)
{
    AppWarning("set flip_on_off:%d \r\n", flip_on_off);
    //TODO
    /* 录制画面翻转,BOOL类型,true反转,false正常 */

	CConfigTable table;
	CameraParamAll cpa;
	memset(&cpa, 0, sizeof(cpa));
	g_configManager.getConfig(getConfigName(CFG_CAMERA_PARAM), table);
	TExchangeAL<CameraParamAll>::getConfigV2(table, cpa, 1);

	if( flip_on_off )//翻转
	{
		if( RA_NONE == cpa.vCameraParamAll[0].rotateAttr )
		{
			g_Alarm.SetAllowMotionDetTime(time(NULL)+10);
			
			cpa.vCameraParamAll[0].rotateAttr = RA_180;//RA_180;
			TExchangeAL<CameraParamAll>::setConfigV2(cpa, table, 1);
			g_configManager.setConfig(getConfigName(CFG_CAMERA_PARAM), table,0, IConfigManager::applyOK);
		}
	}
	else //正常
	{
		if( RA_180 == cpa.vCameraParamAll[0].rotateAttr )
		{
			g_Alarm.SetAllowMotionDetTime(time(NULL)+10);

			cpa.vCameraParamAll[0].rotateAttr = RA_NONE;
			TExchangeAL<CameraParamAll>::setConfigV2(cpa, table, 1);
			g_configManager.setConfig(getConfigName(CFG_CAMERA_PARAM), table,0, IConfigManager::applyOK);
		}
	}
}

BOOL_T IPC_APP_get_flip_onoff(VOID)
{
    AppWarning("IPC_APP_get_flip_onoff \r\n");
    //TODO 		获取 视频翻转
    
    BOOL_T flip_on_off = FALSE;
	CConfigTable table;
	CameraParamAll cpa;
	memset(&cpa, 0, sizeof(cpa));
	g_configManager.getConfig(getConfigName(CFG_CAMERA_PARAM), table);
	TExchangeAL<CameraParamAll>::getConfigV2(table, cpa, 1);

	if( RA_NONE == cpa.vCameraParamAll[0].rotateAttr )
		flip_on_off = FALSE;
	else
		flip_on_off = TRUE;
    AppInfo("curr flip_on_off:%d \r\n", flip_on_off);
    return flip_on_off;
}
#endif

//------------------------------------------

#ifdef TUYA_DP_WATERMARK
VOID IPC_APP_set_watermark_onoff(BOOL_T watermark_on_off)
{
    AppWarning("set watermark_on_off:%d \r\n", watermark_on_off);
    //TODO
    /* 视频水印,BOOL类型,true打开水印,false关闭水印 */
	
	CConfigTable table;
	CameraParamAll cpa;
	memset(&cpa, 0, sizeof(cpa));
	g_configManager.getConfig(getConfigName(CFG_CAMERA_PARAM), table);
	TExchangeAL<CameraParamAll>::getConfigV2(table, cpa, 1);
	
	if( cpa.vCameraParamAll[0].osdSwitch != watermark_on_off )
	{
		cpa.vCameraParamAll[0].osdSwitch = watermark_on_off;
		TExchangeAL<CameraParamAll>::setConfigV2(cpa, table, 1);
		g_configManager.setConfig(getConfigName(CFG_CAMERA_PARAM), table,0, IConfigManager::applyOK);
	}
}

BOOL_T IPC_APP_get_watermark_onoff(VOID)
{
    AppWarning("IPC_APP_get_watermark_onoff \r\n");
    //TODO 		获取 视频水印
    BOOL_T watermark_on_off;

	CConfigTable table;
	CameraParamAll cpa;
	memset(&cpa, 0, sizeof(cpa));
	g_configManager.getConfig(getConfigName(CFG_CAMERA_PARAM), table);
	TExchangeAL<CameraParamAll>::getConfigV2(table, cpa, 1);

	watermark_on_off = cpa.vCameraParamAll[0].osdSwitch;

	AppWarning("curr watermark_on_off:%d \r\n", watermark_on_off);

	return watermark_on_off;	
}
#endif

//------------------------------------------

#ifdef TUYA_DP_WDR
VOID IPC_APP_set_wdr_onoff(BOOL_T wdr_on_off)
{
    AppWarning("set wdr_on_off:%d \r\n", wdr_on_off);
    //TODO
    /* 宽动态范围模式,BOOL类型,true则打开宽动态范围模式,false则关闭宽动态范围 */

	//<shang>
	#if 0
	CConfigTable table;
	CameraParamAll cpa;
	memset(&cpa, 0, sizeof(cpa));
	g_configManager.getConfig(getConfigName(CFG_CAMERA_PARAM), table);
	TExchangeAL<CameraParamAll>::getConfigV2(table, cpa, 1);
	
	if( cpa.vCameraParamAll[0].wdrSwitch != wdr_on_off )
	{
		cpa.vCameraParamAll[0].wdrSwitch = wdr_on_off;
		TExchangeAL<CameraParamAll>::setConfigV2(cpa, table, 1);
		g_configManager.setConfig(getConfigName(CFG_CAMERA_PARAM), table,0, IConfigManager::applyOK);
	}
	#endif
}

BOOL_T IPC_APP_get_wdr_onoff(VOID)
{
    AppWarning("IPC_APP_get_wdr_onoff \r\n");
    BOOL_T wdr_on_off;

	//<shang>
	#if 0
	CConfigTable table;
	CameraParamAll cpa;
	memset(&cpa, 0, sizeof(cpa));
	g_configManager.getConfig(getConfigName(CFG_CAMERA_PARAM), table);
	TExchangeAL<CameraParamAll>::getConfigV2(table, cpa, 1);

	wdr_on_off = cpa.vCameraParamAll[0].wdrSwitch;

    AppWarning("curr s_wdr_on_off:%d \r\n", wdr_on_off);
	#else
	wdr_on_off = false;
	#endif
    return wdr_on_off;
}
#endif

//------------------------------------------

#ifdef TUYA_DP_NIGHT_MODE
STATIC CHAR_T s_night_mode[4] = {0};//for demo
VOID IPC_APP_set_night_mode(CHAR_T *p_night_mode)
{//0-自动 1-关 2-开
    AppWarning("set night_mode:%s \r\n", p_night_mode);
    //TODO
    /* 红外夜视功能,ENUM类型,0-自动 1-关 2-开 */
	g_Alarm.SetAllowMotionDetTime(time(NULL)+10);
	unsigned int newMode;

	CConfigTable table;
	CameraParamAll cpa;
	memset(&cpa, 0, sizeof(cpa));
	g_configManager.getConfig(getConfigName(CFG_CAMERA_PARAM), table);
	TExchangeAL<CameraParamAll>::getConfigV2(table, cpa, 1);

    if(strcmp("0", p_night_mode) == 0)
		newMode = SINGLE_IRMODE_AUTO;
	else if(strcmp("1", p_night_mode) == 0)
		newMode = SINGLE_IRMODE_CLOSE;
	else if(strcmp("2", p_night_mode) == 0)
		newMode = SINGLE_IRMODE_OPEN;
	else
		return;
	
	if( cpa.vCameraParamAll[0].nightVisionMode != newMode )
	{
		cpa.vCameraParamAll[0].nightVisionMode = newMode;
		TExchangeAL<CameraParamAll>::setConfigV2(cpa, table, 1);
		g_configManager.setConfig(getConfigName(CFG_CAMERA_PARAM), table,0, IConfigManager::applyOK);
	}
}

CHAR_T *IPC_APP_get_night_mode(VOID)
{
    AppWarning("IPC_APP_get_night_mode \r\n");
	
	CConfigTable table;
	CameraParamAll cpa;
	memset(&cpa, 0, sizeof(cpa));
	g_configManager.getConfig(getConfigName(CFG_CAMERA_PARAM), table);
	TExchangeAL<CameraParamAll>::getConfigV2(table, cpa, 1);

	unsigned int mode;
    if (SINGLE_IRMODE_AUTO == cpa.vCameraParamAll[0].nightVisionMode)
		mode = 0;
	else if (SINGLE_IRMODE_CLOSE == cpa.vCameraParamAll[0].nightVisionMode)
		mode = 1;
	else if (SINGLE_IRMODE_OPEN == cpa.vCameraParamAll[0].nightVisionMode)
		mode = 2;

	snprintf(s_night_mode, sizeof(s_night_mode), "%d", mode);

    AppInfo("curr s_night_mode:%s \r\n", s_night_mode );

    return  s_night_mode;
}
#endif

#ifdef TUYA_DP_NIGHT_MODE_ANO
STATIC CHAR_T s_night_mode1[4] = {0};//for demo
VOID IPC_APP_set_night_mode_Ano(CHAR_T *p_night_mode)
{//0-自动 1-关 2-开
    AppWarning("set night_mode:%s \r\n", p_night_mode);
    //TODO
    /* 红外夜视功能,ENUM类型,0-自动 1-开 2-关 */
	g_Alarm.SetAllowMotionDetTime(time(NULL)+10);
	unsigned int newMode;

	CConfigTable table;
	CameraParamAll cpa;
	memset(&cpa, 0, sizeof(cpa));
	g_configManager.getConfig(getConfigName(CFG_CAMERA_PARAM), table);
	TExchangeAL<CameraParamAll>::getConfigV2(table, cpa, 1);

    if(strcmp("0", p_night_mode) == 0)
		newMode = DOUBLE_IRMODE_SMART;
	else if(strcmp("1", p_night_mode) == 0)
		newMode = DOUBLE_IRMODE_IR;
	else if(strcmp("2", p_night_mode) == 0)
		newMode = DOUBLE_IRMODE_FULLCOLOR;
	else
		return;
	
	if( cpa.vCameraParamAll[0].nightVisionMode != newMode )
	{
		cpa.vCameraParamAll[0].nightVisionMode = newMode;
		TExchangeAL<CameraParamAll>::setConfigV2(cpa, table, 1);
		g_configManager.setConfig(getConfigName(CFG_CAMERA_PARAM), table,0, IConfigManager::applyOK);
	}
}

CHAR_T *IPC_APP_get_night_mode_Ano(VOID)
{
    AppWarning("IPC_APP_get_night_mode \r\n");
	
	CConfigTable table;
	CameraParamAll cpa;
	memset(&cpa, 0, sizeof(cpa));
	g_configManager.getConfig(getConfigName(CFG_CAMERA_PARAM), table);
	TExchangeAL<CameraParamAll>::getConfigV2(table, cpa, 1);

	unsigned int mode;
    if (DOUBLE_IRMODE_SMART == cpa.vCameraParamAll[0].nightVisionMode)
		mode = 0;
	else if (DOUBLE_IRMODE_IR == cpa.vCameraParamAll[0].nightVisionMode)
		mode = 1;
	else if (DOUBLE_IRMODE_FULLCOLOR == cpa.vCameraParamAll[0].nightVisionMode)
		mode = 2;

	snprintf(s_night_mode, sizeof(s_night_mode), "%d", mode);

    AppInfo("curr s_night_mode:%s \r\n", s_night_mode );

    return  s_night_mode;
}
#endif




//------------------------------------------

#ifdef TUYA_DP_CALIBRATION
VOID IPC_APP_start_calibration(VOID)
{
    AppWarning("start calibration\r\n");
    //非阻塞执行
    //TODO
	g_PtzHandle.Calibration();
    /* 摄像机校准 */
}
#endif

//------------------------------------------

#ifdef TUYA_DP_ALARM_FUNCTION
VOID IPC_APP_set_alarm_function_onoff(BOOL_T alarm_on_off)
{
    //TODO 		设置  移动侦测开关
    AppWarning("set alarm_on_off:%d \r\n", alarm_on_off);
    /* 移动侦测报警功能开关,BOOL类型,true打开,false关闭。
     * 这里SDK已经将该功能整合了，开发者只要做好本地配置的设置和属性即可。 */

	CConfigTable table;
	MotionDetectConfigAll MotionCfg;
	
	g_configManager.getConfig(getConfigName(CFG_MOTIONDETECT), table);
	TExchangeAL<MotionDetectConfigAll>::getConfigV2(table, MotionCfg, 1);
	if( MotionCfg.vMotionDetectAll[0].bEnable != alarm_on_off )
	{
		MotionCfg.vMotionDetectAll[0].bEnable = alarm_on_off;
        TExchangeAL<MotionDetectConfigAll>::setConfigV2(MotionCfg, table, 1);
		g_configManager.setConfig(getConfigName(CFG_MOTIONDETECT), table, 0, IConfigManager::applyOK);
	}
}

BOOL_T IPC_APP_get_alarm_function_onoff(VOID)
{
	//TODO		获取  移动侦测开关
	CConfigTable table;
	MotionDetectConfigAll MotionCfg;

	g_configManager.getConfig(getConfigName(CFG_MOTIONDETECT), table);
	TExchangeAL<MotionDetectConfigAll>::getConfigV2(table, MotionCfg, 1);

	AppWarning("curr alarm_on_off:%d \r\n", MotionCfg.vMotionDetectAll[0].bEnable);
	return MotionCfg.vMotionDetectAll[0].bEnable;
}
#endif

//------------------------------------------

#ifdef TUYA_DP_MOTION_TIMER_SWITCH
VOID IPC_APP_set_motion_timer_switch(BOOL_T switch_on_off)
{
    AppWarning("set motion_timer_switch_on_off:%d \r\n", switch_on_off);

    //TODO 		设置  移动侦测定时开关
	
	CConfigTable table;
	MotionDetectConfigAll MotionCfg;
	
	g_configManager.getConfig(getConfigName(CFG_MOTIONDETECT), table);
	TExchangeAL<MotionDetectConfigAll>::getConfigV2(table, MotionCfg, 1);
	if( switch_on_off ) 	//打开
	{
		if( MotionCfg.vMotionDetectAll[0].iTimerCount <= 0 )
		{
			MotionCfg.vMotionDetectAll[0].iTimerCount = 1;
			TExchangeAL<MotionDetectConfigAll>::setConfigV2(MotionCfg, table, 1);
			
			g_configManager.setConfig(getConfigName(CFG_MOTIONDETECT), table, 0, IConfigManager::applyOK);
		}
	}
	else 					//关闭
	{
		if( MotionCfg.vMotionDetectAll[0].iTimerCount > 0 )
		{
			MotionCfg.vMotionDetectAll[0].iTimerCount = 0;
			TExchangeAL<MotionDetectConfigAll>::setConfigV2(MotionCfg, table, 1);
			
			g_configManager.setConfig(getConfigName(CFG_MOTIONDETECT), table, 0, IConfigManager::applyOK);
		}
	}	
}

BOOL_T IPC_APP_get_motion_timer_switch(VOID)
{
    BOOL_T switch_on_off = FALSE;

    //TODO 		获取  移动侦测定时开关	
	CConfigTable table;
	MotionDetectConfigAll MotionCfg;
	
	g_configManager.getConfig(getConfigName(CFG_MOTIONDETECT), table);
	TExchangeAL<MotionDetectConfigAll>::getConfigV2(table, MotionCfg, 1);

	if( MotionCfg.vMotionDetectAll[0].iTimerCount > 0 )
		switch_on_off = TRUE;
	else
		switch_on_off = FALSE;
    AppWarning("curr motion_timer_switch_on_off:%d \r\n", switch_on_off);
    return switch_on_off;
}
#endif

#ifdef TUYA_DP_ALARM_ZONE_ENABLE
static BOOL_T s_alarm_zone_on_off;

VOID IPC_APP_set_alarm_zone_onoff(BOOL_T alarm_zone_on_off)
{
    /* Motion detection area setting switch,BOOL type,true means on,false is off*/
    printf("set alarm_zone_onoff:%d \r\n", alarm_zone_on_off);
    //s_alarm_zone_on_off = alarm_zone_on_off;

    
    CConfigTable table;
    MotionDetectConfigAll MotionCfg;

    g_configManager.getConfig(getConfigName(CFG_MOTIONDETECT), table);
    TExchangeAL<MotionDetectConfigAll>::getConfigV2(table, MotionCfg, 1);
    if( MotionCfg.vMotionDetectAll[0].bRegionEnable != alarm_zone_on_off )
    {
        MotionCfg.vMotionDetectAll[0].bRegionEnable = alarm_zone_on_off;
        TExchangeAL<MotionDetectConfigAll>::setConfigV2(MotionCfg, table, 1);
        g_configManager.setConfig(getConfigName(CFG_MOTIONDETECT), table, 0, IConfigManager::applyOK);
    }

}

BOOL_T IPC_APP_get_alarm_zone_onoff()
{
//    BOOL_T alarm_zone_on_off = s_alarm_zone_on_off;//__tuya_app_read_INT("alarm_zone_on_off");
//    printf("curr alarm_on_off:%d \r\n", alarm_zone_on_off);
//    return alarm_zone_on_off;

    
	CConfigTable table;
	MotionDetectConfigAll MotionCfg;

	g_configManager.getConfig(getConfigName(CFG_MOTIONDETECT), table);
	TExchangeAL<MotionDetectConfigAll>::getConfigV2(table, MotionCfg, 1);

	AppWarning("curr motion_area:%d \r\n", MotionCfg.vMotionDetectAll[0].bRegionEnable);
    return MotionCfg.vMotionDetectAll[0].bRegionEnable;
}
#endif


#ifdef TUYA_DP_ALARM_ZONE_DRAW

#define MAX_ALARM_ZONE_NUM      (6)     //Supports the maximum number of detection areas
//Detection area structure
typedef struct{
    char pointX;    //Starting point x  [0-100]
    char pointY;    //Starting point Y  [0-100]
    char width;     //width    [0-100]
    char height;    //height    [0-100]
}ALARM_ZONE_T;

typedef struct{
    int iZoneNum;   //Number of detection areas
    ALARM_ZONE_T alarmZone[MAX_ALARM_ZONE_NUM];
}ALARM_ZONE_INFO_T;


VOID IPC_APP_set_alarm_zone_draw(cJSON * p_alarm_zone)
{
	printf("IPC_APP_set_alarm_zone_draw\n");
    if (NULL == p_alarm_zone){
        return ;
    }
#if 0
    /*demo code*/
    /*Motion detection area setting switch*/
    printf("%s %d set alarm_zone_set:%s \r\n",__FUNCTION__,__LINE__, (char *)p_alarm_zone);
    ALARM_ZONE_INFO_T strAlarmZoneInfo;
    INT_T i = 0;
    cJSON * pJson = cJSON_Parse((CHAR_T *)p_alarm_zone);

    if (NULL == pJson){
        printf("%s %d step error\n",__FUNCTION__,__LINE__);
        //free(pResult);
        return;
    }
    cJSON * tmp = cJSON_GetObjectItem(pJson, "num");
    if (NULL == tmp){
        printf("%s %d step error\n",__FUNCTION__,__LINE__);
        cJSON_Delete(pJson);
        //free(pResult);
        return ;
    }
    memset(&strAlarmZoneInfo, 0x00, sizeof(ALARM_ZONE_INFO_T));
    strAlarmZoneInfo.iZoneNum = tmp->valueint;
    printf("%s %d step num[%d]\n",__FUNCTION__,__LINE__,strAlarmZoneInfo.iZoneNum);
    if (strAlarmZoneInfo.iZoneNum > MAX_ALARM_ZONE_NUM){
        printf("#####error zone num too big[%d]\n",strAlarmZoneInfo.iZoneNum);
        cJSON_Delete(pJson);
        //free(pResult);
        return ;
    }
    for (i = 0; i < strAlarmZoneInfo.iZoneNum; i++){
        char region[12] = {0};
        cJSON * cJSONRegion = NULL;
        snprintf(region, 12, "region%d",i);
        cJSONRegion = cJSON_GetObjectItem(pJson, region);
        if (NULL == cJSONRegion){
            printf("#####[%s][%d]error\n",__FUNCTION__,__LINE__);
            cJSON_Delete(pJson);
            //free(pResult);
            return;
        }
        strAlarmZoneInfo.alarmZone[i].pointX = cJSON_GetObjectItem(cJSONRegion, "x")->valueint;
        strAlarmZoneInfo.alarmZone[i].pointY = cJSON_GetObjectItem(cJSONRegion, "y")->valueint;
        strAlarmZoneInfo.alarmZone[i].width = cJSON_GetObjectItem(cJSONRegion,  "xlen")->valueint;
        strAlarmZoneInfo.alarmZone[i].height = cJSON_GetObjectItem(cJSONRegion, "ylen")->valueint;
        printf("#####[%s][%d][%d,%d,%d,%d]\n",__FUNCTION__,__LINE__,strAlarmZoneInfo.alarmZone[i].pointX,\
            strAlarmZoneInfo.alarmZone[i].pointY,strAlarmZoneInfo.alarmZone[i].width,strAlarmZoneInfo.alarmZone[i].height);
    }
    cJSON_Delete(pJson);
    //free(pResult);
#else
    CHAR_T * p_area = (CHAR_T*)p_alarm_zone;
    printf("set motion_area:%s \r\n", p_area);
    //{"num":1,"region0":{"x":0,"xlen":100,"y":0,"ylen":100}} 
    CHAR_T motion_area[256];
    int x=0,xlen=0,y=0,ylen=0;
	strncpy(motion_area,p_area,256);
    AppWarning("set motion_area:%s \r\n", motion_area);

    std::string strValue = motion_area;
    Json::Reader reader;
    Json::Value value;
    Json::Value item;

    if (reader.parse(strValue, value))
    {
        item = value["region0"];
        x = item["x"].asInt();
        xlen = item["xlen"].asInt();
        y = item["y"].asInt();
        ylen = item["ylen"].asInt();
    }
    
    printf("x=%d,xlen=%d,y=%d,ylen=%d\n",x,xlen,y,ylen);
    
    uint val = (x << 24) | (xlen << 16) | (y << 8) | (ylen);
    printf("val=%u\n",val);
    CConfigTable table;
    MotionDetectConfigAll MotionCfg;

    g_configManager.getConfig(getConfigName(CFG_MOTIONDETECT), table);
    TExchangeAL<MotionDetectConfigAll>::getConfigV2(table, MotionCfg, 1);
    if( MotionCfg.vMotionDetectAll[0].mRegion[0] != val )
    {
        MotionCfg.vMotionDetectAll[0].mRegion[0] = val;
        TExchangeAL<MotionDetectConfigAll>::setConfigV2(MotionCfg, table, 1);
        g_configManager.setConfig(getConfigName(CFG_MOTIONDETECT), table, 0, IConfigManager::applyOK);
    }
#endif
    return ;
}
static char s_alarm_zone[256] = {0};
char * IPC_APP_get_alarm_zone_draw()
{
    
	CConfigTable table;
	MotionDetectConfigAll MotionCfg;

	g_configManager.getConfig(getConfigName(CFG_MOTIONDETECT), table);
	TExchangeAL<MotionDetectConfigAll>::getConfigV2(table, MotionCfg, 1);

	AppWarning("curr motion_area:%d \r\n", MotionCfg.vMotionDetectAll[0].mRegion[0]);
     //{"num":1,"region0":{"x":0,"xlen":100,"y":0,"ylen":100}} 
    //Json::Value root;
    //Json::Value item;
    //item["x"] = (MotionCfg.vMotionDetectAll[0].mRegion[0] >> 24) & 0x000000FF;
    //item["xlen"] = (MotionCfg.vMotionDetectAll[0].mRegion[0] >> 16) & 0x000000FF;
    //item["y"] = (MotionCfg.vMotionDetectAll[0].mRegion[0] >> 8) & 0x000000FF;
    //item["ylen"] = MotionCfg.vMotionDetectAll[0].mRegion[0] & 0x000000FF;


    //root["num"] = 1;
    //root["region0"] = item;
    //root.toStyledString();
    //std::string out = root.toStyledString();
    //AppWarning("out:%s \r\n", out.c_str());
	//return (CHAR_T*)out.c_str();
	
    /*demo code*/
    int i;
    ALARM_ZONE_INFO_T strAlarmZoneInfo;

    memset(&strAlarmZoneInfo, 0x00, sizeof(ALARM_ZONE_INFO_T));
    strAlarmZoneInfo.iZoneNum = 1;
    strAlarmZoneInfo.alarmZone[0].pointX = (MotionCfg.vMotionDetectAll[0].mRegion[0] >> 24) & 0x000000FF;
    strAlarmZoneInfo.alarmZone[0].pointY  = (MotionCfg.vMotionDetectAll[0].mRegion[0] >> 8) & 0x000000FF;
    strAlarmZoneInfo.alarmZone[0].width = (MotionCfg.vMotionDetectAll[0].mRegion[0] >> 16) & 0x000000FF;
    strAlarmZoneInfo.alarmZone[0].height = MotionCfg.vMotionDetectAll[0].mRegion[0] & 0x000000FF;
    //tycam_kv_db_read(BASIC_IPC_ALARM_ZONE_SET,&strAlarmZoneInfo);
    /*get param of alarmzoneInfo yourself*/
    memset(s_alarm_zone, 0x00, 256);
    if (strAlarmZoneInfo.iZoneNum > MAX_ALARM_ZONE_NUM){
        printf("[%s] [%d ]get iZoneNum[%d] error",__FUNCTION__,__LINE__,strAlarmZoneInfo.iZoneNum);
        return s_alarm_zone;
    }
    for (i = 0; i < strAlarmZoneInfo.iZoneNum; i++){
        char region[64] = {0};
        //{"169":"{\"num\":1,\"region0\":{\"x\":0,\"y\":0,\"xlen\":50,\"ylen\":50}}"}
        if (0 == i){
            snprintf(s_alarm_zone, 256,"{\\\"num\\\":%d",strAlarmZoneInfo.iZoneNum);
        }
        snprintf(region, 64, ",\\\"region%d\\\":{\\\"x\\\":%d,\\\"y\\\":%d,\\\"xlen\\\":%d,\\\"ylen\\\":%d}",i,strAlarmZoneInfo.alarmZone[i].pointX,\
            strAlarmZoneInfo.alarmZone[i].pointY,strAlarmZoneInfo.alarmZone[i].width,strAlarmZoneInfo.alarmZone[i].height);
        strcat(s_alarm_zone, region);
        if(i == (strAlarmZoneInfo.iZoneNum - 1)){
            strcat(s_alarm_zone, "}");
        }
    }
    printf("[%s][%d] alarm zone[%s]\n",__FUNCTION__,__LINE__,s_alarm_zone);
    return s_alarm_zone;
}
#endif

#ifdef TUYA_DP_ALARM_ZONE_DRAW_EX

#define MAX_ALARM_ZONE_NUM_EX      (4)     //Supports the maximum number of detection areas
//Detection area structure

typedef struct{
    char point[8];//4个点
}ALARM_ZONE_T_EX;

typedef struct{
    int iZoneNum;   //Number of detection areas
    ALARM_ZONE_T_EX alarmZone[MAX_ALARM_ZONE_NUM_EX];
}ALARM_ZONE_INFO_T_EX;


VOID IPC_APP_set_alarm_zone_draw_ex(cJSON * p_alarm_zone)
{
    if (NULL == p_alarm_zone){
        return ;
    }

  //  printf("%s %d set alarm_zone_set:%s \r\n",__FUNCTION__,__LINE__, (char *)p_alarm_zone);
    ALARM_ZONE_INFO_T_EX strAlarmZoneInfo;

    cJSON * pJson = cJSON_Parse((CHAR_T *)p_alarm_zone);

    if (NULL == pJson){
        printf("%s %d step error\n",__FUNCTION__,__LINE__);
        //free(pResult);
        return;
    }
	memset(&strAlarmZoneInfo, 0x00, sizeof(ALARM_ZONE_INFO_T_EX));
    strAlarmZoneInfo.iZoneNum = cJSON_GetArraySize(pJson);

	

  //  printf("%s %d step num[%d]\n",__FUNCTION__,__LINE__,strAlarmZoneInfo.iZoneNum);
    if (strAlarmZoneInfo.iZoneNum > MAX_ALARM_ZONE_NUM_EX)
	{
        printf("#####error zone num too big[%d]\n",strAlarmZoneInfo.iZoneNum);
        return ;
    }
    for(int i = 0; i < strAlarmZoneInfo.iZoneNum; ++i)
	{
		cJSON *oneJsonObj = cJSON_GetArrayItem(pJson, i);
		if (NULL == oneJsonObj)
		{
			cJSON_Delete(pJson);
			return;
		}
		// cJSON * ispoly = cJSON_GetObjectItem(oneJsonObj, "ispoly");
		// printf("ispoly->valueint %d\n", ispoly->valueint);

		cJSON *pSubObj = cJSON_GetObjectItem(oneJsonObj, "points");
		if (NULL == pSubObj)
		{
			cJSON_Delete(pJson);
			return;
		}
		//assert(pSubObj);
		for(int j = 0; j < cJSON_GetArraySize(pSubObj); ++j)
		{
			cJSON *oneJsonObj = cJSON_GetArrayItem(pSubObj, j);
			if (NULL == oneJsonObj)
			{
				cJSON_Delete(pJson);
				return;
			}
			printf("%d\n", oneJsonObj->valueint);

			strAlarmZoneInfo.alarmZone[i].point[j] = oneJsonObj->valueint;
		}
	}
	cJSON_Delete(pJson);
    //free(pResult);



	uint val = 0;
	int x,xlen,y,ylen;
	int k = 0;

	CConfigTable table;
    MotionDetectConfigAll MotionCfg;

    g_configManager.getConfig(getConfigName(CFG_MOTIONDETECT), table);
    TExchangeAL<MotionDetectConfigAll>::getConfigV2(table, MotionCfg, 1);
	memset( MotionCfg.vMotionDetectAll[0].mRegion,0,sizeof(MotionCfg.vMotionDetectAll[0].mRegion));

	for(k = 0; k < strAlarmZoneInfo.iZoneNum; ++k)
	{
		x = strAlarmZoneInfo.alarmZone[k].point[0]; //x1
		y = strAlarmZoneInfo.alarmZone[k].point[1]; //y1
		xlen = strAlarmZoneInfo.alarmZone[k].point[2]-strAlarmZoneInfo.alarmZone[k].point[0]; //x2-x1
		ylen = strAlarmZoneInfo.alarmZone[k].point[7]-strAlarmZoneInfo.alarmZone[k].point[1]; //y8-y1
		val = (x << 24) | (xlen << 16) | (y << 8) | (ylen);
		MotionCfg.vMotionDetectAll[0].mRegion[k] = val;
	}

	TExchangeAL<MotionDetectConfigAll>::setConfigV2(MotionCfg, table, 1);
    g_configManager.setConfig(getConfigName(CFG_MOTIONDETECT), table, 0, IConfigManager::applyOK);

    return ;
}
static char s_alarm_zone_ex[256] = {0};
char * IPC_APP_get_alarm_zone_draw_ex()
{
    
	CConfigTable table;
	MotionDetectConfigAll MotionCfg;

	g_configManager.getConfig(getConfigName(CFG_MOTIONDETECT), table);
	TExchangeAL<MotionDetectConfigAll>::getConfigV2(table, MotionCfg, 1);

	AppWarning("curr motion_area:%d \r\n", MotionCfg.vMotionDetectAll[0].mRegion[0]);
     
	//[{"ispoly":0,"points":[20,35,40,35,40,75,20,75]},{"ispoly":0,"points":[46,36,66,36,66,76,46,76]}]
    
    int i;
	int x,xlen,y,ylen;
	int j = 0;
    ALARM_ZONE_INFO_T_EX strAlarmZoneInfo;

    memset(&strAlarmZoneInfo, 0x00, sizeof(ALARM_ZONE_INFO_T_EX));
    strAlarmZoneInfo.iZoneNum = MAX_ALARM_ZONE_NUM_EX;
	for (i = 0; i < MAX_ALARM_ZONE_NUM_EX; i++)
	{

		x = (MotionCfg.vMotionDetectAll[0].mRegion[i] >> 24) & 0x000000FF;
		y  = (MotionCfg.vMotionDetectAll[0].mRegion[i] >> 8) & 0x000000FF;
		xlen = (MotionCfg.vMotionDetectAll[0].mRegion[i] >> 16) & 0x000000FF;
		ylen = MotionCfg.vMotionDetectAll[0].mRegion[i] & 0x000000FF;

		if ( (0==xlen) || (0==ylen) )
		{
			continue;
		}
		j++;
		strAlarmZoneInfo.alarmZone[i].point[0] = x;
		strAlarmZoneInfo.alarmZone[i].point[1] = y;
		strAlarmZoneInfo.alarmZone[i].point[2] = x+xlen;
		strAlarmZoneInfo.alarmZone[i].point[3] = y;
		strAlarmZoneInfo.alarmZone[i].point[4] = x+xlen;
		strAlarmZoneInfo.alarmZone[i].point[5] = y +ylen;
		strAlarmZoneInfo.alarmZone[i].point[6] = x;
		strAlarmZoneInfo.alarmZone[i].point[7] = y +ylen;
	}
    strAlarmZoneInfo.iZoneNum = j;

	//[{"ispoly":0,"points":[20,35,40,35,40,75,20,75]},{"ispoly":0,"points":[46,36,66,36,66,76,46,76]}]

	#if 1
	char *msg = NULL;
	memset(s_alarm_zone_ex, 0x00, 256);
	cJSON *root = cJSON_CreateArray(); 
	if (NULL == root)
	{
		printf("cJSON_CreateArray error [%s][%d] alarm zone=%s\n",__FUNCTION__,__LINE__,s_alarm_zone_ex);
		return s_alarm_zone_ex;
	}
	
	for (i = 0; i < strAlarmZoneInfo.iZoneNum; i++)
	{
		cJSON *sub = cJSON_CreateObject();

		cJSON_AddNumberToObject(sub,"ispoly",0);

		cJSON *jsonArray = cJSON_CreateArray(); 
		for (j = 0; j < 8; j++)
		{
			cJSON_AddItemToArray(jsonArray, cJSON_CreateNumber(strAlarmZoneInfo.alarmZone[i].point[j]));
		}
		cJSON_AddItemToObject(sub, "points", jsonArray);
		cJSON_AddItemToArray(root,sub);
	}
	
	msg = cJSON_PrintUnformatted(root);

	printf("msg:%s\n",msg);
	strncpy(s_alarm_zone_ex,msg,sizeof(s_alarm_zone_ex));
	printf("msg:%s\n",s_alarm_zone_ex);

	if(root)
	{
		cJSON_Delete(root);
	}
	printf("[%s][%d] alarm zone=%s\n",__FUNCTION__,__LINE__,s_alarm_zone_ex);
    return s_alarm_zone_ex;

	#endif


	#if 0
	//[{"ispoly":0,"points":[20,35,40,35,40,75,20,75]},{"ispoly":0,"points":[46,36,66,36,66,76,46,76]}]
    memset(s_alarm_zone_ex, 0x00, 256);
    if (strAlarmZoneInfo.iZoneNum > MAX_ALARM_ZONE_NUM_EX){
        printf("[%s] [%d ]get iZoneNum[%d] error",__FUNCTION__,__LINE__,strAlarmZoneInfo.iZoneNum);
        return s_alarm_zone_ex;
    }
    for (i = 0; i < strAlarmZoneInfo.iZoneNum; i++){
		char ispoly[64] = {0};
        char region[64] = {0};
        
        if (0 == i){
			snprintf(ispoly, 256,"[{\\\"ispoly\\\":%d",0);
			strcat(s_alarm_zone_ex, ispoly);
        }
		else
		{
			snprintf(ispoly, 256,",{\\\"ispoly\\\":%d",0);
			strcat(s_alarm_zone_ex, ispoly);
		}
        snprintf(region, 64, ",\\\"points\\\":[%d,%d,%d,%d,%d,%d,%d,%d]}",strAlarmZoneInfo.alarmZone[i].point[0],\
            strAlarmZoneInfo.alarmZone[i].point[1],\
			strAlarmZoneInfo.alarmZone[i].point[2],\
			strAlarmZoneInfo.alarmZone[i].point[3],\
			strAlarmZoneInfo.alarmZone[i].point[4],\
			strAlarmZoneInfo.alarmZone[i].point[5],\
			strAlarmZoneInfo.alarmZone[i].point[6],\
			strAlarmZoneInfo.alarmZone[i].point[7]);
        strcat(s_alarm_zone_ex, region);

        if(i != (strAlarmZoneInfo.iZoneNum - 1)){
            strcat(s_alarm_zone_ex, "]");
        }
    }
    printf("[%s][%d] alarm zone=%s\n",__FUNCTION__,__LINE__,s_alarm_zone_ex);
    return s_alarm_zone_ex;

	#endif
}
#endif

#ifdef TUYA_DP_MOTION_DETECT_TIMER
STATIC CHAR_T s_detect_timer[15] = {0};//for demo
VOID IPC_APP_set_motion_detect_timer(CHAR_T *p_timer)
{
    AppWarning("set motion_detect_timer:%s \r\n", p_timer);
    //"开始时间|结束时间"

    //TODO 		设置  移动侦测定时时间
    char *p = NULL;
    char *p2 = NULL;
	char buf[64] = {0};
	snprintf(buf, 64, p_timer);
	MotionDetectTimePeriod TimePeriod;
	
	p2 = buf;
	p = NULL;
	if( p = strchr(buf, ':') )
	{
		*p = '\0';
		TimePeriod.begin.hour = atoi(p2);
	}
	else
	{
		AppInfo("---------------------------------1\n");
		return;
	}
	
	p2 = ++p;
	p = NULL;
	if( p = strchr(p2, '|') )
	{
		*p = '\0';
		TimePeriod.begin.minute = atoi(p2);
	}
	else
	{
		AppInfo("---------------------------------2\n");
		return;
	}
	
	p2 = ++p;
	p = NULL;
	if( p = strchr(p2, ':') )
	{
		*p = '\0';
		TimePeriod.end.hour = atoi(p2);
	}
	else
	{
		AppInfo("---------------------------------3\n");
		return;
	}
	
	p2 = ++p;
	if('\0' != p2)
	{
		TimePeriod.end.minute = atoi(p2);
	}
	else
	{
		AppInfo("---------------------------------4\n");
		return;
	}
	
	CConfigTable table;
	MotionDetectConfigAll MotionCfg;
	
	g_configManager.getConfig(getConfigName(CFG_MOTIONDETECT), table);
	TExchangeAL<MotionDetectConfigAll>::getConfigV2(table, MotionCfg, 1);

	MotionCfg.vMotionDetectAll[0].vTimePeriod[0] = TimePeriod;	
    TExchangeAL<MotionDetectConfigAll>::setConfigV2(MotionCfg, table, 1);
	g_configManager.setConfig(getConfigName(CFG_MOTIONDETECT), table, 0, IConfigManager::applyOK);
}

CHAR_T *IPC_APP_get_motion_detect_timer(VOID)
{
    //TODO 		获取  移动侦测定时时间
    CHAR_T *p_timer = s_detect_timer;

	CConfigTable table;
	MotionDetectConfigAll MotionCfg;
	
	g_configManager.getConfig(getConfigName(CFG_MOTIONDETECT), table);
	TExchangeAL<MotionDetectConfigAll>::getConfigV2(table, MotionCfg, 1);

	MotionDetectTimePeriod *pTimePeriod = &MotionCfg.vMotionDetectAll[0].vTimePeriod[0];
	snprintf(p_timer, 15, "%02d:%02d|%02d:%02d", pTimePeriod->begin.hour, pTimePeriod->begin.minute, pTimePeriod->end.hour, pTimePeriod->end.minute);

    AppWarning("curr motion_detect_timer:%s \r\n", p_timer );

    return p_timer;
}
#endif

//------------------------------------------

#ifdef TUYA_DP_ALARM_SENSITIVITY
VOID IPC_APP_set_alarm_sensitivity(CHAR_T *p_sensitivity)
{
    AppWarning("set alarm_sensitivity:%s \r\n", p_sensitivity);
    //TODO
    /* 移动侦测报警灵敏度,ENUM类型,0为低灵敏度,1为中灵敏度,2为高灵敏度*/

    //TODO 		设置  移动侦测灵敏度
    int sensitivity = -1;
    if(strcmp("0", p_sensitivity) == 0)
    {
        sensitivity = 0;
    }
    if(strcmp("1", p_sensitivity) == 0)
    {
        sensitivity = 1;
    }
    if(strcmp("2", p_sensitivity) == 0)
    {
        sensitivity = 2;
    }

	if( (0 != sensitivity) && (1 != sensitivity) && (2 != sensitivity) )
		return ;
	
	CConfigTable table;
	MotionDetectConfigAll MotionCfg;
	
	g_configManager.getConfig(getConfigName(CFG_MOTIONDETECT), table);
	TExchangeAL<MotionDetectConfigAll>::getConfigV2(table, MotionCfg, 1);

	if( sensitivity != MotionCfg.vMotionDetectAll[0].iLevel )
	{
		MotionCfg.vMotionDetectAll[0].iLevel = sensitivity;
		TExchangeAL<MotionDetectConfigAll>::setConfigV2(MotionCfg, table, 1);
		g_configManager.setConfig(getConfigName(CFG_MOTIONDETECT), table, 0, IConfigManager::applyOK);
	}
}

CHAR_T *IPC_APP_get_alarm_sensitivity(VOID)
{
    CHAR_T* sensitivity_arr[] = {(char*)"0", (char*)"1", (char*)"2"};
    INT_T curr_index = 0;
    //0为低灵敏度,1为中灵敏度,2为高灵敏度

    //TODO 		获取  移动侦测灵敏度	
	CConfigTable table;
	MotionDetectConfigAll MotionCfg;
	
	g_configManager.getConfig(getConfigName(CFG_MOTIONDETECT), table);
	TExchangeAL<MotionDetectConfigAll>::getConfigV2(table, MotionCfg, 1);

	curr_index = MotionCfg.vMotionDetectAll[0].iLevel;
	if( curr_index > 2 )
		curr_index = 2;
	else if( curr_index < 0 )
		curr_index = 0;

    AppWarning("curr alarm_sensitivity:%s \r\n", sensitivity_arr[curr_index] );
    return  sensitivity_arr[curr_index] ;
}
#endif

//------------------------------------------

//#ifdef TUYA_DP_ALARM_INTERVAL
//STATIC CHAR_T s_alarm_interval[4] = {0};//for demo
//VOID IPC_APP_set_alarm_interval(CHAR_T *p_interval)
//{
//    printf("set alarm_interval:%s \r\n", p_interval);
//    //TODO
//    /* 移动侦测报警间歇,单位为分钟,ENUM类型,"1","5","10","30","60" */

//    __tuya_app_write_STR("tuya_alarm_interval", p_interval);
//}

//CHAR_T *IPC_APP_get_alarm_interval(VOID)
//{
//    //移动侦测报警间歇,单位为分钟,ENUM类型,"1","5","10","30","60"
//    __tuya_app_read_STR("tuya_alarm_interval", s_alarm_interval, 4);
//    printf("curr alarm_intervaly:%s \r\n", s_alarm_interval);
//    return s_alarm_interval;
//}
//#endif

//------------------------------------------


#ifdef TUYA_DP_SD_STATUS_ONLY_GET
INT_T IPC_APP_get_sd_status(VOID)
{
    INT_T sd_status = 1;
    /* SD卡状态,VALUE类型,1-正常,2-异常,3-空间不足,4-正在格式化,5-无SD卡 */
    /* 开发者需要返回本地SD卡状态 */
	
	//TODO		获取  SD卡状态

	int iDiskStatus = DISK_STATUS_UNKNOWN;
	iDiskStatus = g_StorageManager->GetDiskState();
	
	AppWarning("iDiskStatus = %d\n", iDiskStatus);


	if( DISK_STATUS_NORMAL == iDiskStatus)
		sd_status = 1;
	else if( DISK_STATUS_ERROR == iDiskStatus)
		sd_status = 2;
	else if(DISK_STATUS_FORMATING == iDiskStatus)
		sd_status = 4;
	else if( (DISK_STATUS_NO_DISK == iDiskStatus) || (DISK_STATUS_UNKNOWN == iDiskStatus) || (DISK_STATUS_NOT_MOUNT == iDiskStatus)  )
		sd_status = 5;

	AppWarning("curr sd_status:%d \r\n", sd_status);

	return sd_status;
}
#endif

//------------------------------------------

#ifdef TUYA_DP_SD_STORAGE_ONLY_GET
VOID IPC_APP_get_sd_storage(UINT_T *p_total, UINT_T *p_used, UINT_T *p_empty)
{//单位：kb
    //TODO 		获取  SD卡内存大小
    /* 开发者需要返回本地SD卡容量 */
	
	int iDiskStatus = DISK_STATUS_UNKNOWN;
	iDiskStatus = g_StorageManager->GetDiskState();
	//SD 卡正常
	if( DISK_STATUS_NORMAL == iDiskStatus )
	{					
		unsigned long long ullDiskSize = 0;
		unsigned long long ullDiskUsedSize = 0;
		unsigned long long ullDiskFreeSize = 0;
		g_StorageManager->GetDiskcapacity(&ullDiskSize, &ullDiskUsedSize, &ullDiskFreeSize);
		ullDiskSize /= 1024;
		ullDiskUsedSize /= 1024;
		ullDiskFreeSize /= 1024;
		
		AppInfo("ullDiskTotalSize=%llu\n", ullDiskSize);
		AppInfo("ullDiskUsedSize=%llu\n", ullDiskUsedSize);
		AppInfo("ullDiskFreeSize=%llu\n", ullDiskFreeSize);
	
		*p_total = ullDiskSize;
		*p_empty = ullDiskFreeSize;
		*p_used = ullDiskUsedSize;
	}
	else
	{
		*p_total = 0;
		*p_empty = 0;
		*p_used = 0;
	}
	
    AppWarning("curr sd total:%u used:%u empty:%u \r\n", *p_total, *p_used, *p_empty);
}
#endif

//------------------------------------------

#ifdef TUYA_DP_SD_RECORD_ENABLE
VOID IPC_APP_set_sd_record_onoff(BOOL_T sd_record_on_off)
{
    AppWarning("set sd_record_on_off:%d \r\n", sd_record_on_off);
    /* SD卡录像功能使能,BOOL类型,true打开,false关闭。
     * 这里SDK已经将该功能整合了，开发者只要做好本地配置的设置和属性即可。 */
	
	CConfigTable tableRecord;
	RecordConfigAll cCfgRecord;
	g_configManager.getConfig(getConfigName(CFG_RECORD),tableRecord);
	TExchangeAL<RecordConfigAll>::getConfigV2(tableRecord, cCfgRecord,1);
	
	if (cCfgRecord.vRecordConfigAll[0].bRecordEnable != sd_record_on_off)
	{
		cCfgRecord.vRecordConfigAll[0].bRecordEnable = sd_record_on_off;
	}
	
	CConfigTable table;
	TExchangeAL<RecordConfigAll>::setConfigV2(cCfgRecord, table, 1);
	g_configManager.setConfig(getConfigName(CFG_RECORD), table,0, IConfigManager::applyOK);

	if(sd_record_on_off)	//打开录像时，因为APP不会获取，所以强制上报
	{	
		CHAR_T sd_mode[4];
        if (RECORD_MODE_ALARM == cCfgRecord.vRecordConfigAll[0].iRecordMode)
		    snprintf(sd_mode,4,"%d",0);
        else if (RECORD_MODE_FULLTIME == cCfgRecord.vRecordConfigAll[0].iRecordMode)
            snprintf(sd_mode,4,"%d",1);
		tuya_ipc_dp_report(NULL, TUYA_DP_SD_RECORD_MODE, PROP_ENUM, sd_mode, 1);
	}
}

BOOL_T IPC_APP_get_sd_record_onoff(VOID)
{
	//TODO		获取  录像开关
			
	CConfigTable tableRecord;
	RecordConfigAll cCfgRecord;
	g_configManager.getConfig(getConfigName(CFG_RECORD),tableRecord);
	TExchangeAL<RecordConfigAll>::getConfigV2(tableRecord, cCfgRecord,1);
	
	AppWarning("config.iRecordMode 	= %d\n", cCfgRecord.vRecordConfigAll[0].bRecordEnable);
	return cCfgRecord.vRecordConfigAll[0].bRecordEnable;
}
#endif

//------------------------------------------

#ifdef TUYA_DP_SD_RECORD_MODE
VOID IPC_APP_set_sd_record_mode(UINT_T sd_record_mode)
{
    AppWarning("set sd_record_mode:%d \r\n", sd_record_mode);
    /* SD卡录像模式选择,UINT_T类型,0-事件录像 1-连续录像。
     * 这里SDK已经将该功能整合了，开发者只要做好本地配置的设置和属性即可。 */
	//TODO		设置  录像模式

	struct RecordConfig config = {0};
	
	CConfigTable tableRecord;
	RecordConfigAll cCfgRecord;
	g_configManager.getConfig(getConfigName(CFG_RECORD),tableRecord);
	TExchangeAL<RecordConfigAll>::getConfigV2(tableRecord, cCfgRecord,1);
	config.iRecordMode = cCfgRecord.vRecordConfigAll[0].iRecordMode;
	//RECORD_MODE_CLOSED,	///< 关闭录像
	//RECORD_MODE_MANUAL,	///< 手动录像
	//RECORD_MODE_CONFIG,	///< 按配置录像
	//RECORD_MODE_FULLTIME, ///< 全天录像
	//RECORD_MODE_ALARM,		///< 报警录像
	//RECORD_MODE_FULLTIME_ALARM,///< 全天录像 and 报警录像


    if( 0 == sd_record_mode )
    {
		 config.iRecordMode = RECORD_MODE_ALARM;
    }
	else if( 1 == sd_record_mode )
    {
		 config.iRecordMode = RECORD_MODE_FULLTIME;
    }
	else //默认全天录像
    {
        AppErr("Error, should not happen\r\n");
//		config.iRecordMode = RECORD_MODE_FULLTIME;
		return ; 	//无效参数, 直接返回
    }
	
	//RecordConfigAll cCfgRecord;
	CConfigTable table;
	cCfgRecord.vRecordConfigAll[0].iRecordMode = config.iRecordMode;
	TExchangeAL<RecordConfigAll>::setConfigV2(cCfgRecord, table, 1);
	g_configManager.setConfig(getConfigName(CFG_RECORD), table,0, IConfigManager::applyOK);
}

UINT_T IPC_APP_get_sd_record_mode(VOID)
{
    //TODO 		获取  录像模式

    struct RecordConfig config = {0};
			
	CConfigTable tableRecord;
	RecordConfigAll cCfgRecord;
	g_configManager.getConfig(getConfigName(CFG_RECORD),tableRecord);
	TExchangeAL<RecordConfigAll>::getConfigV2(tableRecord, cCfgRecord,1);
	config.iRecordMode = cCfgRecord.vRecordConfigAll[0].iRecordMode;
	
	AppWarning("config.iRecordMode		= %d\n", config.iRecordMode);
	switch(config.iRecordMode)
	{
		case RECORD_MODE_FULLTIME:
			return 1;
		case RECORD_MODE_ALARM:
			return 0;
	}

	return 0xABCD; 	//返回无效结果
}

#endif

//------------------------------------------

#ifdef TUYA_DP_SD_UMOUNT
BOOL_T IPC_APP_unmount_sd_card(VOID)
{
    BOOL_T umount_ok = TRUE;

    //TODO
    /* 退出存储卡 */

    AppErr("unmount result:%d \r\n", umount_ok);
    return umount_ok;
}
#endif

//------------------------------------------

#ifdef TUYA_DP_SD_FORMAT
/* -2000:SD卡正在格式化,-2001:SD卡格式化异常,-2002:无SD卡,-2003:SD卡错误.正数为格式化进度 */
STATIC INT_T s_sd_format_progress = 0;
void *thread_sd_format(void *arg)
{
	int iDiskStatus = DISK_STATUS_UNKNOWN;

    /* 先通知APP，进度0% */
    s_sd_format_progress = 0;
    IPC_APP_report_sd_format_status(s_sd_format_progress);

	g_StorageManager->DiskFmt(false);
	
	int count = 50;
	while(count-- > 0)
	{
		iDiskStatus = g_StorageManager->GetDiskState();
		if( iDiskStatus == DISK_STATUS_FORMATING )
		{
			printf("start format. count=%d\n", count);
			int val = 4;
			tuya_ipc_dp_report(NULL, 110,PROP_VALUE,&val,1);
			break;
		}
		usleep(100000); 	// 100ms
	}
	/* 先通知APP，进度0% */
    s_sd_format_progress = 0;
    IPC_APP_report_sd_format_status(s_sd_format_progress);
	
	if(count > 0)
	{
		while(1)
		{
			iDiskStatus = g_StorageManager->GetDiskState();
			if ( iDiskStatus != DISK_STATUS_FORMATING )
			{
				printf("finish format.\n");
				break;
			}

			int curFmtProgress = g_StorageManager->GetFormatProgress();
			if( curFmtProgress != s_sd_format_progress )
			{
				s_sd_format_progress = curFmtProgress;
				printf("report_sd_format_status %d\n",s_sd_format_progress);
				IPC_APP_report_sd_format_status(s_sd_format_progress);
			}
			
			sleep(1);
		}
	}
	if( iDiskStatus == DISK_STATUS_NORMAL )
	{
		s_sd_format_progress = 100;
		IPC_APP_report_sd_format_status(s_sd_format_progress);
	}
	else
	{
		s_sd_format_progress = -2001;
		IPC_APP_report_sd_format_status(s_sd_format_progress);
	}
	
    pthread_exit(0);
}

VOID IPC_APP_format_sd_card(VOID)
{
    AppWarning("start to format sd_card \r\n");
    /* SD卡格式化。
     * 这里SDK已经完成了部分代码的编写，开发者只需要实现格式化操作即可。 */
	//TODO		格式化SD卡

	bool ret;
	Json::Value table;

	ret = g_configManager.getConfig(getConfigName(CFG_SDF), table);
	AppWarning("get cfg_sdf config %s\n", ret ? "succ" : "failed");
	AppWarning("sdcard status %d\n",table["Status"].asInt());

	table["Status"] = 1;
	ret = ((g_configManager.setConfig(getConfigName(CFG_SDF), table, 0, IConfigManager::applyOK)) & IConfigManager::applyFileError)==0?true:false;
	AppWarning("set cfg_sdf config %s\n", ret ? "succ" : "failed");

	
	//语音提示
	/*CAudioPrompt::AudioFileParm audioFile;
	audioFile.strFileName = AUDIO_FILE_SD_FORMATING;
	audioFile.type = 0;
	g_AudioPrompt.aoPlay(audioFile);*/

    pthread_t sd_format_thread;
    pthread_create(&sd_format_thread, NULL, thread_sd_format, NULL);
    pthread_detach(sd_format_thread);
}
#endif

#ifdef TUYA_DP_SD_FORMAT_STATUS_ONLY_GET
INT_T IPC_APP_get_sd_format_status(VOID)
{
	printf("get_sd_format_status %d\n",s_sd_format_progress);
	static int get_status_sd = 0;
	static unsigned char sd_status = 0;
	if(s_sd_format_progress == 90)
	{
		get_status_sd = 1;
	}
	if(s_sd_format_progress == 100)
	{
		if(get_status_sd == 0)
		{
			sd_status ++;
			if(sd_status == 4)
			{
				sd_status = 0;
				get_status_sd = 1;
			}
			return 90;
		}
		else
		{
			get_status_sd = 0;
		}
	}
	return s_sd_format_progress;
}
#endif

//------------------------------------------

#ifdef TUYA_DP_PTZ_CONTROL
VOID IPC_APP_ptz_start_move(CHAR_T *p_direction)
{
    AppWarning("ptz start move:%s \r\n", p_direction);
    //TODO
    /* 0-上,1-右上,2-右,3-右下,4-下,5-左下,6-左,7-左上 */
	
    int tuya_motor_dir = atoi(p_direction);
    AppWarning("----------------------------->ptz start move:%d \r\n", tuya_motor_dir);

	
	g_Alarm.SetAllowMotionDetTime(time(NULL)+20); 		/// +20以防停止云台的消息丢失

	PTZDirection_e dir;
	switch (tuya_motor_dir)
	{
		case 0:
			dir = PTZ_UP;
			break;
		case 4:
			dir = PTZ_DOWN;
			break;
		case 6:
			dir = PTZ_LEFT;
			break;
		case 2:
			dir = PTZ_RIGHT;
			break;
		default :
			return;
	}
	g_PtzHandle.DoMotorMove(dir);
}
#endif

#ifdef TUYA_DP_PTZ_STOP
VOID IPC_APP_ptz_stop_move(VOID)
{
    AppWarning("----------------------------->ptz stop move \r\n");
    //TODO
    /* 云台转动停止 */
	
	g_PtzHandle.DoMotorStop();
	AppWarning("-------------------------ptz stop---------\n");
	
	g_Alarm.SetAllowMotionDetTime(time(NULL)+10); 	///< 10s后才允许移动侦测报警
	iAllowMotorTrackTime = time(NULL) + 5;
}
#endif

#ifdef TUYA_DP_PTZ_CHECK
void IPC_APP_ptz_check(VOID)
{
    printf("ptz check \r\n");
}
#endif

#ifdef TUYA_DP_EPTZ_CONTROL
int s_scale = 1;
VOID IPC_APP_eptz_start_move(CHAR_T *p_direction)
{
    printf("ptz start move:%s \r\n", p_direction);
    //TODO
    int dir = atoi(p_direction);
    printf("----------------------------->eptz start move:%d \r\n", dir);

	if (0 == dir)
	{
		s_scale --;
		if (s_scale < 1)
			s_scale = 1;
	}
	else if (1 == dir)
	{
		s_scale ++;
		if (s_scale > 9)
			s_scale = 9;
	}

	CaptureSetEptz(s_scale);
}
#endif

#ifdef TUYA_DP_EPTZ_STOP
VOID IPC_APP_eptz_stop_move(VOID)
{
    printf("----------------------------->eptz stop move \r\n");
    //TODO

}
#endif

#ifdef TUYA_DP_TRACK_ENABLE
void IPC_APP_track_enable(BOOL_T track_enable)
{
    printf("track_enable %d\r\n",track_enable);
}

BOOL_T IPC_APP_get_track_enable(void)
{
    char track_enable = 0;
    //the value you get yourself
    return (BOOL_T)track_enable;
}

#endif

#ifdef TUYA_DP_HUM_FILTER
void IPC_APP_set_human_filter(BOOL_T filter_enable)
{
    AppWarning("set filter_enable %d\r\n",filter_enable);

	// CConfigTable table;
	// MotionTrackConfig MotionTrackCfg;
	
	// g_configManager.getConfig(getConfigName(CFG_MOTIONTRACK), table);
	// TExchangeAL<MotionTrackConfig>::getConfig(table, MotionTrackCfg);

	// MotionTrackCfg.bPersonEnable = filter_enable;
	// TExchangeAL<MotionTrackConfig>::setConfig(MotionTrackCfg, table);
	// g_configManager.setConfig(getConfigName(CFG_MOTIONTRACK), table, 0, IConfigManager::applyOK);


	CConfigTable table;
	MotionDetectConfigAll MotionCfg;
	
	g_configManager.getConfig(getConfigName(CFG_MOTIONDETECT), table);
	TExchangeAL<MotionDetectConfigAll>::getConfigV2(table, MotionCfg, 1);
	if( MotionCfg.vMotionDetectAll[0].bPersonFilterEnable != filter_enable )
	{
		MotionCfg.vMotionDetectAll[0].bPersonFilterEnable = filter_enable;
        TExchangeAL<MotionDetectConfigAll>::setConfigV2(MotionCfg, table, 1);
		g_configManager.setConfig(getConfigName(CFG_MOTIONDETECT), table, 0, IConfigManager::applyOK);
	}

    return;
}

BOOL_T IPC_APP_get_human_filter()
{
	// BOOL_T filter_enable;
	// CConfigTable table;
	// MotionTrackConfig MotionTrackCfg;
	
	// g_configManager.getConfig(getConfigName(CFG_MOTIONTRACK), table);
	// TExchangeAL<MotionTrackConfig>::getConfig(table, MotionTrackCfg);

	CConfigTable table;
	MotionDetectConfigAll MotionCfg;

	g_configManager.getConfig(getConfigName(CFG_MOTIONDETECT), table);
	TExchangeAL<MotionDetectConfigAll>::getConfigV2(table, MotionCfg, 1);

	AppWarning("curr filter_enable:%d \r\n", MotionCfg.vMotionDetectAll[0].bPersonFilterEnable);
	return MotionCfg.vMotionDetectAll[0].bPersonFilterEnable;
}
#endif

#ifdef TUYA_DP_PATROL_MODE
void IPC_APP_set_patrol_mode(BOOL_T patrol_mode)
{
    printf("patrol_mode %d\r\n",patrol_mode);
    return;
}

char IPC_APP_get_patrol_mode(void)
{
    char patrol_mode = 0;
    //the value you get yourself
    return patrol_mode;
}

#endif

#ifdef TUYA_DP_PATROL_SWITCH
void IPC_APP_set_patrol_switch(BOOL_T patrol_switch)
{
    printf("patrol_switch %d\r\n",patrol_switch);
    return;
}

BOOL_T IPC_APP_get_patrol_switch(void)
{
    char patrol_switch = 0;
    //the value you get yourself
    return (BOOL_T)patrol_switch;
}

void IPC_APP_ptz_preset_reset(S_PRESET_CFG *preset_cfg)
{
    /*Synchronize data from server*/
    return;
}

#endif

#ifdef TUYA_DP_PATROL_TMODE
void IPC_APP_set_patrol_tmode(BOOL_T patrol_tmode)
{
    printf("patrol_tmode %d\r\n",patrol_tmode);
    return;
}

char IPC_APP_get_patrol_tmode(void)
{
    char patrol_tmode = 0;
    //the value you get yourself
    return patrol_tmode;
}

#endif

#ifdef TUYA_DP_PATROL_TIME
void IPC_APP_set_patrol_time(cJSON * p_patrol_time)
{
    //set your patrol_time
    /*

    cJSON * pJson = cJSON_Parse((CHAR_T *)p_patrol_time);
    if (NULL == pJson){
        TYWARN("----error---\n");

        return -1;
    }
    cJSON* t_start = cJSON_GetObjectItem(pJson, "t_start");
    cJSON* t_end = cJSON_GetObjectItem(pJson, "t_end");
    if ((NULL == t_start) || (NULL == t_end)){
        TYWARN("----t_start---\n");
        cJSON_Delete(pJson);
        return -1;
    }
    TYDEBUG("stare%s--end:%s\n", t_start->valuestring,t_end->valuestring);

    */
    return;
}

#endif

#ifdef TUYA_DP_PRESET_SET
void IPC_APP_set_preset(cJSON * p_preset_param)
{
    //preset add ,preset del, preset go
#if 0
    cJSON * pJson = cJSON_Parse((CHAR_T *)p_preset_param);
    if (NULL == pJson){
        TYERROR("null preset set input");
        return -1;
    }
    cJSON* type = cJSON_GetObjectItem(pJson, "type");
    cJSON* data = cJSON_GetObjectItem(pJson, "data");
    if ((NULL == type) || (NULL == data)){
        TYERROR("invalid preset set input");
        return -1;
    }
   

    TYDEBUG("preset set type: %d",type->valueint);
    //1:add preset point 2:delete preset point 3:call preset point
    if(type->valueint == 1)
    {
        char pic_buffer[PRESET_PIC_MAX_SIZE] = {0};  
        int pic_size = sizeof(pic_buffer);  /*Image to be shown*/
        S_PRESET_POSITION preset_pos;
        char respond_add[128] = {0}; 
        /*mpId is 1,2,3,4,5,6，The server will generate a set of its own preset point number information based on the mpid.*/
        preset_pos.mpId = 1;   
        preset_pos.ptz.pan = 100; /*horizontal position*/
        preset_pos.ptz.tilt = 60;/*vertical position*/
        cJSON* name = cJSON_GetObjectItem(data, "name");
        int name_len = 0;
        int error_num = 0;
        
        if(name == NULL)
        {
            TYERROR("name is null\n");
            return -1;
        }
        name_len = strlen(name->valuestring);
        name_len = (name_len > 30)?30:name_len;
        memcpy(&preset_pos.name,name->valuestring,(name_len));
        preset_pos.name[name_len] = '\0';
        error_num = tuya_ipc_preset_add(&preset_pos);

        snprintf(respond_add,128,"{\\\"type\\\":%d,\\\"data\\\":{\\\"error\\\":%d}}",type->valueint,error_num);

        tuya_ipc_dp_report(NULL, TUYA_DP_PRESET_SET,PROP_STR,respond_add,1);

        //tuya_ipc_preset_add_pic(pic_buffer,pic_size); /*if you need show pic ,you should set this api*/
    }
    else if(type->valueint == 2)
    {
        cJSON* num = cJSON_GetObjectItem(data, "num"); //can delete one or more
        cJSON* sets = cJSON_GetObjectItem(data, "sets");
        char respond_del[128] = {0}; 
        cJSON* del_data;
        int del_num = num->valueint;
        for(i = 0; i < del_num; i++)
        {
            del_data = cJSON_GetArrayItem(sets,i);
            cJSON* devId = cJSON_GetObjectItem(del_data, "devId");  /*devid is the preset point number registered in the server*/
            cJSON* mpId = cJSON_GetObjectItem(del_data, "mpId");  /*mpid is the preset point number managed on the device*/
            if((NULL == devId) || (NULL == mpId))
            {
                printf("devid or mpid is error\n");
                return -1;
            }
            del_preset.seq = atoi(mpId->valuestring);
            
            printf("%d---%s\n",del_preset.seq,devId->valuestring);

            error_num = (int)time(NULL);

            tuya_ipc_preset_del(devId->valuestring);
            
            snprintf(respond_add,128,"{\\\"type\\\":%d,\\\"data\\\":{\\\"error\\\":%d}}",type->valueint,error_num);
        }
    }
    else if(type->valueint == 3)
    {
        cJSON* mpId = cJSON_GetObjectItem(data, "mpId");

        preset_seq = atoi(mpId->valuestring);
        //get your seq pos and go there
    }
#endif
    return;
}

#endif

#ifdef TUYA_DP_PATROL_STATE
void IPC_APP_patrol_state(int *patrol_state)
{
    //printf("patrol_state %d\r\n",atoi(patrol_state));
    //return your patrol_state
    return;
}

#endif


#ifdef TUYA_DP_LINK_MOVE_SET
VOID IPC_APP_set_link_pos(INT_T bind_seq)
{
    /*set the link pos*/
    printf("IPC_APP_set_bind_pos:%d \r\n", bind_seq);
    /*demo
    step1: get the current position
    step2: save the position to flash
    */
    return;
}
#endif


#ifdef TUYA_DP_LINK_MOVE_ACTION
VOID IPC_APP_set_link_move(INT_T bind_seq)
{
    /*move to the link pos*/
    printf("IPC_APP_set_bind_move:%d \r\n", bind_seq);
    /*demo
     step1: get the position base seq
     step2: go to the target position
    */
    return;
}
#endif

//------------------------------------------

#ifdef TUYA_DP_BLUB_SWITCH
STATIC BOOL_T s_blub_on_off = TRUE;//for demo
VOID IPC_APP_set_blub_onoff(BOOL_T blub_on_off)
{
    AppWarning("set blub_on_off:%d \r\n", blub_on_off);
    //TODO
    /* 特殊灯光控制开关,BOOL类型,true打开,false关闭 */

    s_blub_on_off = blub_on_off;
}

BOOL_T IPC_APP_get_blub_onoff(VOID)
{
    AppWarning("curr blub_on_off:%d \r\n", s_blub_on_off);
    return s_blub_on_off;
}
#endif

//------------------------------------------

#ifdef TUYA_DP_ELECTRICITY
INT_T IPC_APP_get_battery_percent(VOID)
{
    //TODO
    /* 电池电量(百分比),VALUE类型,[0-100] */

    return 80;
}
#endif

#ifdef TUYA_DP_POWERMODE
CHAR_T *IPC_APP_get_power_mode(VOID)
{
    //TODO
    /* 供电方式,ENUM类型,"0"为电池供电状态,"1"为插电供电状态(或电池充电状态) */

    return "1";
}
#endif

//------------------------------------------

#ifdef TUYA_DP_MOTION_TRACK
VOID IPC_APP_set_motion_track_onoff(BOOL_T motion_track_on_off)
{
    //TODO 		设置  人体追踪开关
    AppWarning("----------------------set motion track : %s---------------------------\n\n", motion_track_on_off?"true":"false");
	
	CConfigTable table;
	MotionTrackConfig MotionTrackCfg;
	
	g_configManager.getConfig(getConfigName(CFG_MOTIONTRACK), table);
	TExchangeAL<MotionTrackConfig>::getConfig(table, MotionTrackCfg);

	MotionTrackCfg.bEnable = motion_track_on_off;
	TExchangeAL<MotionTrackConfig>::setConfig(MotionTrackCfg, table);
	g_configManager.setConfig(getConfigName(CFG_MOTIONTRACK), table, 0, IConfigManager::applyOK);

}
BOOL_T IPC_APP_get_motion_track_onoff(VOID)
{
    //TODO 		获取  人体追踪开关
	CConfigTable table;
	MotionTrackConfig MotionTrackCfg;
	
	g_configManager.getConfig(getConfigName(CFG_MOTIONTRACK), table);
	TExchangeAL<MotionTrackConfig>::getConfig(table, MotionTrackCfg);

    AppWarning("----------------------get motion track : %s---------------------------\n\n", MotionTrackCfg.bEnable?"true":"false");
	
	return MotionTrackCfg.bEnable;
}
#endif

//------------------------------------------

#ifdef TUYA_DP_MOTION_INTERVAL
VOID IPC_APP_set_motion_interval (UINT_T interval )
{
    AppWarning("set motion interval :%d \r\n", interval );
    /* 0-1min,1-3min,2-5min */
	//TODO
    int valitmer = 0;
	
    if (0 == interval)
    {
        valitmer = 60;
    }
    else if (1 == interval)
    {
        valitmer = 60*3;
    }
    else if (2 == interval)
    {
        valitmer = 60*5;
    }
    else
    {
        AppErr("set motion interval :%d error!\r\n", interval );
        return;
    }

    
	CConfigTable table;
	MotionDetectConfigAll MotionCfg;
	
	g_configManager.getConfig(getConfigName(CFG_MOTIONDETECT), table);
	TExchangeAL<MotionDetectConfigAll>::getConfigV2(table, MotionCfg, 1);
	if( MotionCfg.vMotionDetectAll[0].iInterval != valitmer )
	{
		MotionCfg.vMotionDetectAll[0].iInterval = valitmer;
        TExchangeAL<MotionDetectConfigAll>::setConfigV2(MotionCfg, table, 1);
		g_configManager.setConfig(getConfigName(CFG_MOTIONDETECT), table, 0, IConfigManager::applyOK);
	}
}
UINT_T IPC_APP_get_motion_interval (VOID)
{
    //TODO
    UINT_T interval;
	CConfigTable table;
	MotionDetectConfigAll MotionCfg;

	g_configManager.getConfig(getConfigName(CFG_MOTIONDETECT), table);
	TExchangeAL<MotionDetectConfigAll>::getConfigV2(table, MotionCfg, 1);

	AppWarning("curr  motion interval:%d \r\n", MotionCfg.vMotionDetectAll[0].iInterval);
    if (60 == MotionCfg.vMotionDetectAll[0].iInterval)
    {
        interval = 0;
    }
    else if (60*3 == MotionCfg.vMotionDetectAll[0].iInterval)
    {
        interval = 1;
    }
    else if (60*5 == MotionCfg.vMotionDetectAll[0].iInterval)
    {
        interval = 2;
    }
	return interval;
}
#endif

#ifdef TUYA_DP_RESET_IPC
VOID IPC_APP_set_reset_ipc(BOOL_T reset_ipc)
{
    AppWarning("restart ipc :%d \r\n", reset_ipc);
	if(reset_ipc == true)
	{
		AbnormalRestart();
	}
}
#endif

//------------------------------------------
/////安保灯 add by ale
#define LIGHT_LOW 20
#ifdef TUYA_DP_ANBAO_LIGHT
VOID IPC_APP_set_anbao_light_onoff(BOOL_T light_on_off)
{
    printf("set anbao light_on_off:%d \r\n", light_on_off);
    //TODO
    /* 状态指示灯,BOOL类型,true打开,false关闭 */
	
	CConfigTable table;
	FlightWarnConfig CfgFlightWarn;
	
	g_configManager.getConfig(getConfigName(CFG_FLIGHT_WARN), table);
	TExchangeAL<FlightWarnConfig>::getConfig(table,CfgFlightWarn);
    printf("get old anbao bManualEnable:%d \r\n", CfgFlightWarn.bManualEnable);
	
	g_Camera.SetWhiteLed(light_on_off, CfgFlightWarn.iDuration);
	
    if (CfgFlightWarn.bManualEnable != light_on_off)
    {
		CfgFlightWarn.bManualEnable = light_on_off;

		TExchangeAL<FlightWarnConfig>::setConfig(CfgFlightWarn, table);
		g_configManager.setConfig(getConfigName(CFG_FLIGHT_WARN), table, 0, IConfigManager::applyOK);
    }

//	g_AnBaoLightManager.SetAnBaoLightStatus(light_on_off);
}
BOOL_T IPC_APP_get_anbao_light_onoff(VOID)
{
	#if 0
	CConfigTable table;
	FlightWarnConfig CfgFlightWarn;
	g_configManager.getConfig(getConfigName(CFG_FLIGHT_WARN), table);
	TExchangeAL<FlightWarnConfig>::getConfig(table,CfgFlightWarn);
	
    printf("curr anbao_light_on_off:%d \r\n", CfgFlightWarn.bManualEnable);
	
    return CfgFlightWarn.bManualEnable;
	#else
	//返回灯真实的开关状态
	bool enable = g_Camera.GetWhiteLedStatus();
    AppWarning("curr anbao_light_on_off:%d \r\n", enable);
    return enable;
	#endif
}
#endif

//------------------------------------------

#ifdef TUYA_DP_FLOODLIGHT_LIGHTNESS
VOID IPC_APP_set_floodlight_lightness(INT_T lightness)
{
    printf("set floodlight_lightness:%d \r\n", lightness);
    //TODO
    /* 白光灯亮度,数值型,1-100 */

	CConfigTable table;
	FlightWarnConfig CfgFlightWarn;
	
	g_configManager.getConfig(getConfigName(CFG_FLIGHT_WARN), table);
	TExchangeAL<FlightWarnConfig>::getConfig(table,CfgFlightWarn);
    
    if (CfgFlightWarn.iLuminance != lightness)
    {
		CfgFlightWarn.iLuminance = lightness;
		
		TExchangeAL<FlightWarnConfig>::setConfig(CfgFlightWarn, table);
		g_configManager.setConfig(getConfigName(CFG_FLIGHT_WARN), table, 0, IConfigManager::applyOK);

//		g_AnBaoLightManager.SetAnBaoLightlightness();
    }
}
INT_T IPC_APP_get_floodlight_lightness(VOID)
{
	CConfigTable table;
	FlightWarnConfig CfgFlightWarn;
	g_configManager.getConfig(getConfigName(CFG_FLIGHT_WARN), table);
	TExchangeAL<FlightWarnConfig>::getConfig(table,CfgFlightWarn);
	
    AppWarning("get floodlight_lightness:%d \r\n", CfgFlightWarn.iLuminance);
	
    return CfgFlightWarn.iLuminance;
}
#endif

#ifdef TUYA_DP_FLOODLIGHT_YELLOW
VOID IPC_APP_set_floodlight_yellow(INT_T lightness)
{
    AppWarning("set floodlight_yellow:%d \r\n", lightness);
    //TODO
    /* 黄光灯亮度,数值型,1-100 */

	CConfigTable table;
	FlightWarnConfig CfgFlightWarn;
	
	g_configManager.getConfig(getConfigName(CFG_FLIGHT_WARN), table);
	TExchangeAL<FlightWarnConfig>::getConfig(table,CfgFlightWarn);
    
    if (CfgFlightWarn.iLuminance_yellow != lightness)
    {
		CfgFlightWarn.iLuminance_yellow = lightness;
		if(CfgFlightWarn.iLuminance_yellow < 20)
		{
		//	if((CfgFlightWarn.bManualEnable == 1) && (check_light_status !=1))
			{
			//	check_light_status = 1;
				SystemSetIncandescentLampSwitch(0);
			}
			if(CfgFlightWarn.iLuminance <= LIGHT_LOW)
			{
				SystemSetIncandescentLampLumi(LIGHT_LOW,CfgFlightWarn.iLuminance_yellow);
				SystemSetIncandescentLampLumi_yellow(LIGHT_LOW,1000-CfgFlightWarn.iLuminance_yellow);
			}
			else
			{
				SystemSetIncandescentLampLumi(CfgFlightWarn.iLuminance,CfgFlightWarn.iLuminance_yellow);
				SystemSetIncandescentLampLumi_yellow(CfgFlightWarn.iLuminance,1000-CfgFlightWarn.iLuminance_yellow);
			}
		}
		else if((CfgFlightWarn.iLuminance_yellow >=20) && (CfgFlightWarn.iLuminance_yellow <970))
		{
			//if((CfgFlightWarn.bManualEnable == 1) && (check_light_status != 2))
			{
				//check_light_status = 2;
				SystemSetIncandescentLampSwitch(1);
				SystemSetIncandescentLampSwitch_yellow(1);
			}
			if(CfgFlightWarn.iLuminance <= LIGHT_LOW)
			{
				SystemSetIncandescentLampLumi(LIGHT_LOW,CfgFlightWarn.iLuminance_yellow);
				SystemSetIncandescentLampLumi_yellow(LIGHT_LOW,1000-CfgFlightWarn.iLuminance_yellow);
			}
			else
			{
				SystemSetIncandescentLampLumi(CfgFlightWarn.iLuminance,CfgFlightWarn.iLuminance_yellow);
				SystemSetIncandescentLampLumi_yellow(CfgFlightWarn.iLuminance,1000-CfgFlightWarn.iLuminance_yellow);
			}
		}
		else
		{
			//if((CfgFlightWarn.bManualEnable == 1) && (check_light_status != 3))
			{
				//check_light_status = 3;
				SystemSetIncandescentLampSwitch_yellow(0);
			}
			if(CfgFlightWarn.iLuminance <= LIGHT_LOW)
			{
				SystemSetIncandescentLampLumi(LIGHT_LOW,CfgFlightWarn.iLuminance_yellow);
				SystemSetIncandescentLampLumi_yellow(LIGHT_LOW,1000-CfgFlightWarn.iLuminance_yellow);
			}
			else
			{
				SystemSetIncandescentLampLumi(CfgFlightWarn.iLuminance,CfgFlightWarn.iLuminance_yellow);
				SystemSetIncandescentLampLumi_yellow(CfgFlightWarn.iLuminance,1000-CfgFlightWarn.iLuminance_yellow);
			}
		}
		TExchangeAL<FlightWarnConfig>::setConfig(CfgFlightWarn, table);
		g_configManager.setConfig(getConfigName(CFG_FLIGHT_WARN), table, 0, IConfigManager::applyOK);
    }
}
INT_T IPC_APP_get_floodlight_yellow(VOID)
{
	CConfigTable table;
	FlightWarnConfig CfgFlightWarn;
	g_configManager.getConfig(getConfigName(CFG_FLIGHT_WARN), table);
	TExchangeAL<FlightWarnConfig>::getConfig(table,CfgFlightWarn);
	
    AppWarning("get floodlight_yellow:%d \r\n", CfgFlightWarn.iLuminance_yellow);
	
    return CfgFlightWarn.iLuminance_yellow;
}
#endif

//------------------------------------------

#ifdef TUYA_DP_FLIGHT_WARN_SWITCH
VOID IPC_APP_set_flight_warn_switch(BOOL_T pir_switch_on_off)
{
    AppWarning("set flight_warn_switch:%d \r\n", pir_switch_on_off);
    //TODO
    /* 状态指示灯,BOOL类型,true打开,false关闭 */
	CConfigTable table;
	FlightWarnConfig CfgFlightWarn;
	
	g_configManager.getConfig(getConfigName(CFG_FLIGHT_WARN), table);
	TExchangeAL<FlightWarnConfig>::getConfig(table,CfgFlightWarn);
    
    if (CfgFlightWarn.bEnable != pir_switch_on_off)
    {
	    CfgFlightWarn.bEnable = pir_switch_on_off;

		TExchangeAL<FlightWarnConfig>::setConfig(CfgFlightWarn, table);
		g_configManager.setConfig(getConfigName(CFG_FLIGHT_WARN), table, 0, IConfigManager::applyOK);
    }
}
BOOL_T IPC_APP_get_flight_warn_switch(VOID)
{
	CConfigTable table;
	FlightWarnConfig CfgFlightWarn;
	g_configManager.getConfig(getConfigName(CFG_FLIGHT_WARN), table);
	TExchangeAL<FlightWarnConfig>::getConfig(table,CfgFlightWarn);
    AppWarning("curr flight_warn_switch:%d \r\n", CfgFlightWarn.bEnable);
    
    return CfgFlightWarn.bEnable;
}
#endif

//------------------------------------------
#ifdef TUYA_DP_FLIGHT_WARN_TIME
VOID IPC_APP_set_flight_warn_time(UINT_T atime )
{
    AppWarning("set flight_warn_time :%d \r\n", atime );
    /* 5-600 sec */
	//TODO

	CConfigTable table;
	FlightWarnConfig CfgFlightWarn;
	
	g_configManager.getConfig(getConfigName(CFG_FLIGHT_WARN), table);
	TExchangeAL<FlightWarnConfig>::getConfig(table,CfgFlightWarn);
    
    if (CfgFlightWarn.iDuration != atime)
    {
	    CfgFlightWarn.iDuration = atime;

		TExchangeAL<FlightWarnConfig>::setConfig(CfgFlightWarn, table);
		g_configManager.setConfig(getConfigName(CFG_FLIGHT_WARN), table, 0, IConfigManager::applyOK);
    }
}
UINT_T IPC_APP_get_flight_warn_time (VOID)
{
    /* 5-600 sec */
    //TODO
    
	CConfigTable table;
	FlightWarnConfig CfgFlightWarn;
	
	g_configManager.getConfig(getConfigName(CFG_FLIGHT_WARN), table);
	TExchangeAL<FlightWarnConfig>::getConfig(table,CfgFlightWarn);
    AppWarning("get flight_warn_time:%d \r\n", CfgFlightWarn.iDuration);
	
    return CfgFlightWarn.iDuration;
}
#endif

//------------------------------------------

#ifdef TUYA_DP_MUTE_RECORD
VOID IPC_APP_set_mute_record(BOOL_T mute_record_on_off)
{
    AppWarning("set mute_record_on_off:%d \r\n", mute_record_on_off);
    //TODO
    /* 静音录像功能,BOOL类型,true打开,false关闭 */


	bool bMuteRecord;
	
	CConfigTable tableRecord;
	RecordConfigAll cCfgRecord;
	g_configManager.getConfig(getConfigName(CFG_RECORD),tableRecord);
	TExchangeAL<RecordConfigAll>::getConfigV2(tableRecord, cCfgRecord,1);
	bMuteRecord = cCfgRecord.vRecordConfigAll[0].bMuteRecord;	

	if( bMuteRecord != mute_record_on_off )
	{
		 bMuteRecord = mute_record_on_off;
		 
		 cCfgRecord.vRecordConfigAll[0].bMuteRecord = bMuteRecord;
		 TExchangeAL<RecordConfigAll>::setConfigV2(cCfgRecord, tableRecord, 1);
		 g_configManager.setConfig(getConfigName(CFG_RECORD), tableRecord, 0, IConfigManager::applyOK);
	}    
}
BOOL_T IPC_APP_get_mute_record(VOID)
{
	bool bMuteRecord;
	CConfigTable tableRecord;
	RecordConfigAll cCfgRecord;
	g_configManager.getConfig(getConfigName(CFG_RECORD),tableRecord);
	TExchangeAL<RecordConfigAll>::getConfigV2(tableRecord, cCfgRecord,1);
	bMuteRecord = cCfgRecord.vRecordConfigAll[0].bMuteRecord;	
    AppWarning("curr mute_record_on_off:%d \r\n", bMuteRecord);
    return bMuteRecord;
}
#endif

//------------------------------------------
#ifdef TUYA_DP_PIR_SWITCH1
VOID IPC_APP_set_PIR1_onoff(BOOL_T pir_on_off)
{
    printf("1set pir_on_off:%d \r\n", pir_on_off);
    //TODO
    CConfigTable table;
	PirConfig PirCfg;
	
	g_configManager.getConfig(getConfigName(CFG_PIR), table);
	TExchangeAL<PirConfig>::getConfig(table,PirCfg);
    if (PirCfg.bpir1 != pir_on_off)
    {
    	PirCfg.bpir1 = pir_on_off;
    	TExchangeAL<PirConfig>::setConfig(PirCfg, table);
    	g_configManager.setConfig(getConfigName(CFG_PIR), table, 0, IConfigManager::applyOK);
    }
}

BOOL_T IPC_APP_get_PIR1_onoff(VOID)
{
 	CConfigTable table;
	PirConfig PirCfg;
	
	g_configManager.getConfig(getConfigName(CFG_PIR), table);
	TExchangeAL<PirConfig>::getConfig(table, PirCfg);
	
    AppWarning("get pir1 :%d \r\n", PirCfg.bpir1);
    return PirCfg.bpir1;
}
#endif

#ifdef TUYA_DP_PIR_SWITCH2
VOID IPC_APP_set_PIR2_onoff(BOOL_T pir_on_off)
{
    printf("2set pir_on_off:%d \r\n", pir_on_off);
    CConfigTable table;
	PirConfig PirCfg;
	
	g_configManager.getConfig(getConfigName(CFG_PIR), table);
	TExchangeAL<PirConfig>::getConfig(table,PirCfg);
    if (PirCfg.bpir2 != pir_on_off)
    {
    	PirCfg.bpir2 = pir_on_off;
    	TExchangeAL<PirConfig>::setConfig(PirCfg, table);
    	g_configManager.setConfig(getConfigName(CFG_PIR), table, 0, IConfigManager::applyOK);
    }
}

BOOL_T IPC_APP_get_PIR2_onoff(VOID)
{
	CConfigTable table;
	PirConfig PirCfg;
	
	g_configManager.getConfig(getConfigName(CFG_PIR), table);
	TExchangeAL<PirConfig>::getConfig(table, PirCfg);
	
    AppWarning("get pir2 :%d \r\n", PirCfg.bpir2);
    return PirCfg.bpir2;
}
#endif


#ifdef TUYA_DP_PIR_SWITCH3
VOID IPC_APP_set_PIR3_onoff(BOOL_T pir_on_off)
{
    printf("3set pir_on_off:%d \r\n", pir_on_off);
    //TODO
    CConfigTable table;
	PirConfig PirCfg;
	
	g_configManager.getConfig(getConfigName(CFG_PIR), table);
	TExchangeAL<PirConfig>::getConfig(table,PirCfg);
    if (PirCfg.bpir3 != pir_on_off)
    {
		PirCfg.bpir3 = pir_on_off;
		TExchangeAL<PirConfig>::setConfig(PirCfg, table);
		g_configManager.setConfig(getConfigName(CFG_PIR), table, 0, IConfigManager::applyOK);
    }
}

BOOL_T IPC_APP_get_PIR3_onoff(VOID)
{
	CConfigTable table;
	PirConfig PirCfg;
	
	g_configManager.getConfig(getConfigName(CFG_PIR), table);
	TExchangeAL<PirConfig>::getConfig(table, PirCfg);
	
    AppWarning("get pir3 :%d \r\n", PirCfg.bpir3);
    return PirCfg.bpir3;
}
#endif

#ifdef TUYA_DP_PIR_SWITCH_SENS
VOID IPC_APP_set_pir_switch_sens(UINT_T mode)
{
    //TODO
    /* pir 灵敏度,ENUM类型,0-灵敏度低 1-灵敏度中 2-灵敏度高*/
    AppWarning("set pir mode:%d \r\n", mode);
    
    if( mode > 3 )
		return;
	
	CConfigTable table;
	PirConfig PirCfg;
	
	g_configManager.getConfig(getConfigName(CFG_PIR), table);
	TExchangeAL<PirConfig>::getConfig(table,PirCfg);
    if (PirCfg.iSensitivity != mode)
    {
		PirCfg.iSensitivity = mode;
		TExchangeAL<PirConfig>::setConfig(PirCfg, table);
		g_configManager.setConfig(getConfigName(CFG_PIR), table, 0, IConfigManager::applyOK);
    }
}

UINT_T IPC_APP_get_pir_switch_sens(VOID)
{
	CConfigTable table;
	PirConfig PirCfg;
	
	g_configManager.getConfig(getConfigName(CFG_PIR), table);
	TExchangeAL<PirConfig>::getConfig(table, PirCfg);
	
    AppWarning("get pir mode:%d \r\n", PirCfg.iSensitivity);
    return PirCfg.iSensitivity;
}
#endif

#ifdef TUYA_DP_PIR_SWITCH_TIME
VOID IPC_APP_set_pir_switch_time(UINT_T atime)
{
    //TODO
    /* pir 亮灯时长,0-1分钟 1-5分钟 2-10分钟*/
    printf("set pir time:%d \r\n", atime);
    
    if( atime > 3 )
		return;
	
	CConfigTable table;
	PirConfig PirCfg;
	
	g_configManager.getConfig(getConfigName(CFG_PIR), table);
	TExchangeAL<PirConfig>::getConfig(table,PirCfg);
    if (PirCfg.iPirLightOnTime != atime)
    {
		PirCfg.iPirLightOnTime = atime;
		TExchangeAL<PirConfig>::setConfig(PirCfg, table);
		g_configManager.setConfig(getConfigName(CFG_PIR), table, 0, IConfigManager::applyOK);
    }
}

UINT_T IPC_APP_get_pir_switch_time(VOID)
{
	CConfigTable table;
	PirConfig PirCfg;
	
	g_configManager.getConfig(getConfigName(CFG_PIR), table);
	TExchangeAL<PirConfig>::getConfig(table, PirCfg);
	
    printf("get pir time:%d \r\n", PirCfg.iPirLightOnTime);
    return PirCfg.iPirLightOnTime;
}
#endif


#ifdef TUYA_DP_SET_LUX_LEVEL
VOID IPC_APP_set_lux_level(UINT_T mode)
{
    //TODO
    /* Enum Value: 0, 1, 2 0=Low，1=Middle（默认），2=High*/
    AppWarning("set lux level  mode:%d \r\n", mode);
    
    if( mode > 3 )
		return;
	
	CConfigTable table;
	LuxConfig LuxCfg;
	
	g_configManager.getConfig(getConfigName(CFG_LUX), table);
	TExchangeAL<LuxConfig>::getConfig(table,LuxCfg);
    if (LuxCfg.mode != mode)
    {
    	LuxCfg.mode = mode;
    	TExchangeAL<LuxConfig>::setConfig(LuxCfg, table);
    	g_configManager.setConfig(getConfigName(CFG_LUX), table, 0, IConfigManager::applyOK);
    }
}

UINT_T IPC_APP_get_lux_level(VOID)
{
	CConfigTable table;
	LuxConfig LuxCfg;
	
	g_configManager.getConfig(getConfigName(CFG_LUX), table);
	TExchangeAL<LuxConfig>::getConfig(table, LuxCfg);
	
    AppWarning("get lux level mode:%d \r\n", LuxCfg.mode);
    return LuxCfg.mode;
}
#endif

#ifdef TUYA_DP_PIR_SWITCH
VOID IPC_APP_set_pir_switch(UINT_T mode)
{
    //TODO
    /* pir 模式,ENUM类型,0-关闭 1-灵敏度低 2-灵敏度中 3-灵敏度高 4-开启*/
    AppWarning("set pir mode:%d \r\n", mode);
    
    if( mode > 3 )
		return;
	
	CConfigTable table;
	PirConfig PirCfg;
	
	g_configManager.getConfig(getConfigName(CFG_PIR), table);
	TExchangeAL<PirConfig>::getConfig(table,PirCfg);
    if (PirCfg.iSensitivity != mode)
    {
    	PirCfg.iSensitivity = mode;
    	TExchangeAL<PirConfig>::setConfig(PirCfg, table);
    	g_configManager.setConfig(getConfigName(CFG_PIR), table, 0, IConfigManager::applyOK);
    }
}

UINT_T IPC_APP_get_pir_switch(VOID)
{
	CConfigTable table;
	PirConfig PirCfg;
	
	g_configManager.getConfig(getConfigName(CFG_PIR), table);
	TExchangeAL<PirConfig>::getConfig(table, PirCfg);
	
    AppWarning("get pir mode:%d \r\n", PirCfg.iSensitivity);
    return PirCfg.iSensitivity;
}
#endif

//------------------------------------------

#ifdef TUYA_DP_SIREN_SWITCH
BOOL_T s_siren_switch = FALSE;
VOID IPC_APP_set_siren(BOOL_T siren_switch)
{
    AppWarning("set set_siren:%d \r\n", siren_switch);
    //TODO
    /* 警笛开关,BOOL类型,true打开,false关闭 */
	s_siren_switch = siren_switch;
	g_Siren.SetSirenStatus(siren_switch);
}
BOOL_T IPC_APP_get_siren(VOID)
{
    AppWarning("curr get_siren:%d \r\n", s_siren_switch);	
    return s_siren_switch;
}
#endif

//------------------------------------------

#ifdef TUYA_DP_IPC_USAGE_MODE
VOID IPC_APP_set_ipc_usage_mode(UINT_T mode)
{
    //TODO
    /* 推送模式,ENUM类型,0-仅文字 1-文字和图片 */
    AppWarning("set usage mode:%d \r\n", mode);
    
    if( (1 != mode) && (2 != mode) )
		return;
	
	int iMode;
	CConfigTable table;
	
	g_configManager.getConfig(getConfigName(CFG_USAGE_MODE), table);
	iMode = table["Mode"].asInt();

    if (iMode != mode)
    {
    	table["Mode"] = mode;
    	g_configManager.setConfig(getConfigName(CFG_USAGE_MODE), table, 0, IConfigManager::applyOK);
    }
}

UINT_T IPC_APP_get_ipc_usage_mode(VOID)
{
	int iMode;
	CConfigTable table;
	
	g_configManager.getConfig(getConfigName(CFG_USAGE_MODE), table);
	iMode = table["Mode"].asInt();
	
    AppWarning("get usage mode:%d \r\n", iMode);
    return iMode;
}
#endif

#ifdef TUYA_DP_VOICE_ONOFF
//STATIC BOOL_T high_alarm_onoff = 0;
VOID IPC_APP_set_voice_onoff(BOOL_T alarm_onoff)
{
	printf("set voice onoff %d\n",alarm_onoff);
	// CConfigTable table;
	// VoiceConfig Voicfg;
	
	// g_configManager.getConfig(getConfigName(CFG_VOICE), table);
	// TExchangeAL<VoiceConfig>::getConfig(table,Voicfg);
    
    // Voicfg.bVoiceonoff = alarm_onoff;

	// TExchangeAL<VoiceConfig>::setConfig(Voicfg, table);
	// g_configManager.setConfig(getConfigName(CFG_VOICE), table, 0, IConfigManager::applyOK);
}
BOOL_T IPC_APP_get_voice_onoff(VOID)
{
	// printf("get voice onoff\n");
	// BOOL_T voice = 0;
	// CConfigTable table;
	// VoiceConfig Voicfg;
	// g_configManager.getConfig(getConfigName(CFG_VOICE), table);
	// TExchangeAL<VoiceConfig>::getConfig(table,Voicfg);
	// if (0==Voicfg.bVoiceonoff)
    //     voice = 0;
    // else
    //     voice = 1;
	return 0;
}
#endif

#ifdef TUYA_DP_VOICE_SENSITIVITY
VOID IPC_APP_set_voice_sensitivity(CHAR_T *cf)
{
	// int mode = atoi(cf);
	// CConfigTable table;
	// VoiceConfig Voicfg;
	
	// g_configManager.getConfig(getConfigName(CFG_VOICE), table);
	// TExchangeAL<VoiceConfig>::getConfig(table,Voicfg);
	// Voicfg.IVoicesentivity = mode;
	// printf("set sensitivity is %d\n",Voicfg.IVoicesentivity);
	// TExchangeAL<VoiceConfig>::setConfig(Voicfg, table);
	// g_configManager.setConfig(getConfigName(CFG_VOICE), table, 0, IConfigManager::applyOK);
}
INT_T IPC_APP_get_voice_sensitivity(VOID)
{
	// CConfigTable table;
	// VoiceConfig Voicfg;
	// g_configManager.getConfig(getConfigName(CFG_VOICE), table);
	// TExchangeAL<VoiceConfig>::getConfig(table,Voicfg);
	// printf("get Tem_cf %d\n",Voicfg.IVoicesentivity);
	// return Voicfg.IVoicesentivity;

	return 0;
}

#endif
#ifdef TUYA_DP_SIREN_LINKAGE
VOID IPC_APP_set_siren_onoff(BOOL_T siren_on_off)
{
	CConfigTable table;
	SirenConfig SirenCfg;
	
	g_configManager.getConfig(getConfigName(CFG_SIREN), table);
	TExchangeAL<SirenConfig>::getConfig(table,SirenCfg);
    AppWarning("get sirenswitch:%d \r\n", SirenCfg.autosiren);
	SirenCfg.autosiren = siren_on_off;
	TExchangeAL<SirenConfig>::setConfig(SirenCfg, table);
	g_configManager.setConfig(getConfigName(CFG_SIREN), table, 0, IConfigManager::applyOK);
}

BOOL_T IPC_APP_get_siren_onoff(VOID)
{
    CConfigTable table;
	SirenConfig SirenCfg;
	BOOL_T siren_onoff = 0;
	g_configManager.getConfig(getConfigName(CFG_SIREN), table);
	TExchangeAL<SirenConfig>::getConfig(table,SirenCfg);
	if(SirenCfg.autosiren == 1)
	{
		siren_onoff = 1;
	}
	else
	{
		siren_onoff = 0;
	}
	return siren_onoff;
}
#endif

#ifdef TUYA_DP_SIREN_SOUND
VOID IPC_APP_set_Siren_Sound(CHAR_T *list)
{
	int mode = atoi(list);
	CConfigTable table;
	SirenConfig SirenCfg;
	
	g_configManager.getConfig(getConfigName(CFG_SIREN), table);
	TExchangeAL<SirenConfig>::getConfig(table,SirenCfg);
    AppWarning("get sirensound:%d \r\n", SirenCfg.sirenlist);
	SirenCfg.sirenlist = mode;
	TExchangeAL<SirenConfig>::setConfig(SirenCfg, table);
	g_configManager.setConfig(getConfigName(CFG_SIREN), table, 0, IConfigManager::applyOK);
}
INT_T IPC_APP_get_Siren_Sound(VOID)
{
	CConfigTable table;
	SirenConfig SirenCfg;
	g_configManager.getConfig(getConfigName(CFG_SIREN), table);
	TExchangeAL<SirenConfig>::getConfig(table,SirenCfg);
//	printf("get sirenlist %d\n",SirenCfg.sirenlist);
	return SirenCfg.sirenlist;
}

#endif

#ifdef TUYA_DP_SIREN_DURATION
VOID IPC_APP_set_Siren_Duration(INT_T value)
{
	CConfigTable table;
	SirenConfig SirenCfg;
	
	g_configManager.getConfig(getConfigName(CFG_SIREN), table);
	TExchangeAL<SirenConfig>::getConfig(table,SirenCfg);
    AppWarning("get sirenduration:%d \r\n", SirenCfg.sirenduration);
	SirenCfg.sirenduration = value;
	TExchangeAL<SirenConfig>::setConfig(SirenCfg, table);
	g_configManager.setConfig(getConfigName(CFG_SIREN), table, 0, IConfigManager::applyOK);
}

INT_T IPC_APP_get_Siren_Duration(VOID)
{
	CConfigTable table;
	SirenConfig SirenCfg;
	g_configManager.getConfig(getConfigName(CFG_SIREN), table);
	TExchangeAL<SirenConfig>::getConfig(table,SirenCfg);
//	printf("get sirenduration %d\n",SirenCfg.sirenduration);
	return SirenCfg.sirenduration;
}
#endif

#ifdef TUYA_DP_SIREN_VOLUME
VOID IPC_APP_set_Siren_Volume(INT_T value)
{
	CConfigTable table;
	SirenConfig SirenCfg;
	
	g_configManager.getConfig(getConfigName(CFG_SIREN), table);
	TExchangeAL<SirenConfig>::getConfig(table,SirenCfg);
    AppWarning("get sirenvolume:%d \r\n", SirenCfg.sirenvolume);
	SirenCfg.sirenvolume = value;
	TExchangeAL<SirenConfig>::setConfig(SirenCfg, table);
	g_configManager.setConfig(getConfigName(CFG_SIREN), table, 0, IConfigManager::applyOK);
	//AudioSetVolume(0,0,value);
}

INT_T IPC_APP_get_Siren_Volume(VOID)
{
	CConfigTable table;
	SirenConfig SirenCfg;
	g_configManager.getConfig(getConfigName(CFG_SIREN), table);
	TExchangeAL<SirenConfig>::getConfig(table,SirenCfg);
	AppWarning("get sirenvolume %d\n",SirenCfg.sirenvolume);
	return SirenCfg.sirenvolume;
}
#endif

#ifdef TUYA_DP_IPC_VOLUME
VOID IPC_APP_set_Ipc_Volume(INT_T value)
{
	CConfigTable table;
	IpcVolConfig IpcVolCfg;
	
	g_configManager.getConfig(getConfigName(CFG_IPC_VOL), table);
	TExchangeAL<IpcVolConfig>::getConfig(table,IpcVolCfg);
    AppWarning("get ipcvolume:%d \r\n", IpcVolCfg.ipcvolume);
	IpcVolCfg.ipcvolume = value;
	TExchangeAL<IpcVolConfig>::setConfig(IpcVolCfg, table);
	g_configManager.setConfig(getConfigName(CFG_IPC_VOL), table, 0, IConfigManager::applyOK);
	//AudioSetVolume(0,0,value);
}

INT_T IPC_APP_get_Ipc_Volume(VOID)
{
	CConfigTable table;
	IpcVolConfig IpcVolCfg;
	g_configManager.getConfig(getConfigName(CFG_IPC_VOL), table);
	TExchangeAL<IpcVolConfig>::getConfig(table,IpcVolCfg);
//	printf("get sirenvolume %d\n",SirenCfg.sirenvolume);
	return IpcVolCfg.ipcvolume;
}
#endif

#ifdef TUYA_DP_MEMORY_POINT
//STATIC CHAR_T s_detect_timer[15] = {0};//for demo
S_PRESET_CFG preset_cfg;
void get_motion_snapshot(char *snap_addr, int *snap_size);

char snap_addr1[100*1024] = {0}; //Snapshot maximum size is 100KB
int snap_size1 = 0;

int error_num = 1;
int tmp = 0;
VOID IPC_APP_set_PRESET(CHAR_T *p_preset)
{
    AppWarning("set_PRESET:%s \r\n", p_preset);
    //"开始时间|结束时间"	
    int ret = 0;
    struct motor_get_status stMotorMessage_preset;
	Json::Reader reader;
    Json::Value value;
	
	if( false == reader.parse(p_preset, value) )
    {
		AppErr("parse json failed.\n");
		return;
	}
    CHAR_T s_respond_add[128] = {0};
	CHAR_T respond_del[128] = {0};
	unsigned char set_preset[4] = {0};
	unsigned char compore_buf[6] = {0};
    printf("type is %d\n",(p_preset[8]-'0'));
    if(value["type"].asInt() == 1)
    {
		printf("preset_cfg num is %d\n",preset_cfg.num);
		for(int i = 0;i<preset_cfg.num;i++)
		{
			printf("preset_cfg.position[%d].mpId is %d\n",i,preset_cfg.position[i].mpId);
			if(preset_cfg.position[i].mpId != 0)
			{
				compore_buf[preset_cfg.position[i].mpId-1] = 1;
			}
		}
		for(int k=0;k<6;k++)
		{
			if(compore_buf[k] == 0)
			{
				preset_cfg.position[preset_cfg.num].mpId = k+1;
				break;
			}
		}
		ret = MotorGetStatus(&stMotorMessage_preset);
		if(ret != 0)
		{
			printf("get motor status error\n");
			return;
		}
		memset(preset_cfg.position[preset_cfg.num].name,0,32);
		memcpy(preset_cfg.position[preset_cfg.num].name,value["data"]["name"].asString().c_str(),value["data"]["name"].asString().length());
	
		preset_cfg.position[preset_cfg.num].ptz.pan = stMotorMessage_preset.fhx_cur_step;
		preset_cfg.position[preset_cfg.num].ptz.tilt = stMotorMessage_preset.fhy_cur_step;
		error_num = tuya_ipc_preset_add(&preset_cfg.position[preset_cfg.num]);
		snprintf(s_respond_add,128,"{\"type\":%d,\"data\":{\"error\":%d}}",1,error_num);
		tuya_ipc_dp_report(NULL,TUYA_DP_MEMORY_POINT,PROP_STR,s_respond_add,1);
		get_motion_snapshot(snap_addr1,&snap_size1);
		tuya_ipc_preset_add_pic(snap_addr1,snap_size1);
		
		
		snprintf(s_respond_add,128,"{\"type\":%d,\"data\":{\"seq\":%d,\"pan\":%d,\"tilt\": %d,\"zoom\": 0 }}",1,error_num,stMotorMessage_preset.fhx_cur_step,stMotorMessage_preset.fhy_cur_step);
		printf("string is %s\n",s_respond_add);
		tuya_ipc_dp_report(NULL,TUYA_DP_MEMORY_POINT,PROP_STR,s_respond_add,1);
	}
	else if(value["type"].asInt() == 2)
	{
		printf("mpId is %d\n",p_preset[61]-'0');
		printf("devId is %s\n",preset_cfg.position[p_preset[61]-'0'-1].id);
		for(int m=0;m<6;m++)
		{
			if(preset_cfg.position[m].mpId == (p_preset[61]-'0'))
			{
				printf("mpId dele is %d\n",preset_cfg.position[m].mpId);
				tuya_ipc_preset_del(preset_cfg.position[m].id);
				preset_cfg.position[m].mpId = 0;
				break;
			}
		}
		if(tmp == 0)
		{
			tmp = 1;
		}
		else
		{
			tmp = 0;
		}
		snprintf(respond_del,128,"{\"type\":%d,\"data\":{\"error\":%d}}",2,tmp);
		tuya_ipc_dp_report(NULL,TUYA_DP_MEMORY_POINT,PROP_STR,respond_del,1);
	}
	else if(value["type"].asInt() == 3)
	{
		printf("turn ptz is %d\n",p_preset[26]-'0');
		for(int y=0;y<preset_cfg.num;y++)
		{
			if((p_preset[26]-'0') == preset_cfg.position[y].mpId)
			{
				g_PtzHandle.GotoPosition(preset_cfg.position[y].ptz.pan,preset_cfg.position[y].ptz.tilt);
			}
		}
	}

	if((p_preset[8]-'0') != 3)
	{
		IPC_APP_get_preset();
	}
	
}

VOID IPC_APP_get_preset(void)
{
	tuya_ipc_preset_get(&preset_cfg);
	printf("preset sum is %d\n",preset_cfg.num);
	for(int i=0;i<preset_cfg.num;i++)
	{
		printf("*****preset sum mpid is %d x is %d  y is %d\n",preset_cfg.position[i].mpId,preset_cfg.position[i].ptz.pan,preset_cfg.position[i].ptz.tilt);
	}
}

CHAR_T *IPC_APP_get_cruise_sta(VOID)
{
    CHAR_T* cruise_arr[] = {(char*)"0", (char*)"1", (char*)"2"};
    return  cruise_arr[2] ;
}

void IPC_APP_set_PRESET_ACTION(CHAR_T *p_action)
{
	printf("preset action is %d\n",atoi(p_action)+1);
	for(int y=0;y<preset_cfg.num;y++)
	{
		if((atoi(p_action)+1) == preset_cfg.position[y].mpId)
		{
			g_PtzHandle.GotoPosition(preset_cfg.position[y].ptz.pan,preset_cfg.position[y].ptz.tilt);
		}
	}
	
}

#endif

#ifdef TUYA_DP_ONVIF_SWITCH
VOID IPC_APP_set_onvif_switch(BOOL_T onoff)
{
    AppWarning("set onvif switch: %d \r\n", onoff);
    //printf("set onvif switch: %d \r\n", onoff);
	
	CConfigTable table;
	OnvifConf_S stOnvifCfg;
	g_configManager.getConfig(getConfigName(CFG_ONVIF), table);	
	TExchangeAL<OnvifConf_S>::getConfig(table, stOnvifCfg);

	if(stOnvifCfg.Enable != onoff)
	{
		stOnvifCfg.Enable = onoff;
		
		TExchangeAL<OnvifConf_S>::setConfig(stOnvifCfg, table);
		g_configManager.setConfig(getConfigName(CFG_ONVIF), table,0, IConfigManager::applyOK);
	}	
}

BOOL_T IPC_APP_get_onvif_switch(BOOL_T *pbPwdChanged)
{
	CConfigTable table;
	OnvifConf_S stOnvifCfg;
	g_configManager.getConfig(getConfigName(CFG_ONVIF), table);	
	TExchangeAL<OnvifConf_S>::getConfig(table, stOnvifCfg);

	if( pbPwdChanged )
	{
		if(stOnvifCfg.Password == "12345678")
			*pbPwdChanged = FALSE;
		else
			*pbPwdChanged = TRUE;
	}

	return stOnvifCfg.Enable;
}
#endif

#ifdef TUYA_DP_ONVIF_CHANGE_PWD
INT_T IPC_APP_onvif_change_pwd(CHAR_T * p_onvif_pwd)
{
    AppWarning("onvif change pwd: %s \r\n", p_onvif_pwd);
    //printf("onvif change pwd: %s \r\n", p_onvif_pwd);

    //{"oldpwd":"","newpwd":""} 

    Json::Reader reader;
    Json::Value value;
    Json::Value item;
		
    if( false == reader.parse(p_onvif_pwd, value) )
    {
		AppErr("parse json failed.\n");
		return -1;
	}

	if( false == value.isMember("oldpwd") || false == value["oldpwd"].isString() || 
		false == value.isMember("newpwd") || false == value["newpwd"].isString() )
	{
		AppErr("json have no \"oldpwd\"/\"newpwd\" or is not string.\n");
		return -1;
	}
	
	CConfigTable table;
	OnvifConf_S stOnvifCfg;
	g_configManager.getConfig(getConfigName(CFG_ONVIF), table);	
	TExchangeAL<OnvifConf_S>::getConfig(table, stOnvifCfg);

	if((stOnvifCfg.FirstEnable == false) || (value["oldpwd"].asString() == stOnvifCfg.Password))
	{
		if( value["oldpwd"].asString() != stOnvifCfg.Password )
		{
			AppErr("oldpwd error.\n");
			return -1;
		}
	}

	if( value["newpwd"].asString() == stOnvifCfg.Password )
	{
		AppErr("newpwd and oldpwd is same.\n");
		return -2;
	}
	
	if( value["newpwd"].asString().length() > 32 )
	{
		AppErr("newpwd len[%d] is more than 32.\n", value["newpwd"].asString().length());
		return -2;
	}
	if((stOnvifCfg.FirstEnable == true) && (value["oldpwd"].asString() == stOnvifCfg.Password))
	{
		stOnvifCfg.FirstEnable = false;
	}
	else if(stOnvifCfg.FirstEnable == true)
	{
		stOnvifCfg.FirstEnable = false;
		stOnvifCfg.Enable = true;
		tuya_ipc_dp_report(NULL, TUYA_DP_ONVIF_SWITCH,PROP_BOOL,&stOnvifCfg.Enable,1);
	}
	stOnvifCfg.Password = value["newpwd"].asString();
	TExchangeAL<OnvifConf_S>::setConfig(stOnvifCfg, table);
	g_configManager.setConfig(getConfigName(CFG_ONVIF), table,0, IConfigManager::applyOK);
	
	return 0;
}
#endif

#ifdef TUYA_DP_ANTI_FLICKER
STATIC CHAR_T s_anti_flicker[4] = {0};//for demo
VOID IPC_APP_set_anti_flicker(CHAR_T *p_anti_flicker)
{
	//防闪烁 0-50hz 1-60hz
	unsigned int newMode = 0;
	CConfigTable table;
	CameraParamAll cpa;
	memset(&cpa, 0, sizeof(cpa));
	g_configManager.getConfig(getConfigName(CFG_CAMERA_PARAM), table);
	TExchangeAL<CameraParamAll>::getConfigV2(table, cpa, 1);
	g_Alarm.SetAllowMotionDetTime(time(NULL)+10);
	
	if(strcmp("0", p_anti_flicker) == 0)
		newMode = 0;
	else if(strcmp("1", p_anti_flicker) == 0)
		newMode = 1;
	else if(strcmp("2", p_anti_flicker) == 0)
		newMode = 2;
	else
		return;
	printf("**********anti_flicker*******is %u\n",newMode);
	
	cpa.vCameraParamAll[0].iAntiFlicker = newMode;
	TExchangeAL<CameraParamAll>::setConfigV2(cpa, table, 1);
	g_configManager.setConfig(getConfigName(CFG_CAMERA_PARAM), table,0, IConfigManager::applyOK);
	
}

CHAR_T *IPC_APP_get_anti_flicker(VOID)
{
	CConfigTable table;
	CameraParamAll cpa;
	memset(&cpa, 0, sizeof(cpa));
	g_configManager.getConfig(getConfigName(CFG_CAMERA_PARAM), table);
	TExchangeAL<CameraParamAll>::getConfigV2(table, cpa, 1);
	snprintf(s_anti_flicker, sizeof(s_anti_flicker), "%d", cpa.vCameraParamAll[0].iAntiFlicker);
	
	printf("curr s_anti_flicker:%s \r\n", s_anti_flicker );
	return s_anti_flicker;
}
#endif

