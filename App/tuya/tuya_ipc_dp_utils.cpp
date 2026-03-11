/*********************************************************************************
  *Copyright(C),2015-2020, 涂鸦科技 www.tuya.comm
  *FileName: tuya_ipc_dp_utils.c
  *
  * 文件描述：
  * 1. DP点工具API实现
  *
  * 本文件代码为工具代码，用户可以不用关心
  * 请勿随意修改该文件中的任何内容，如需修改请联系涂鸦产品经理！！
  *
**********************************************************************************/
#include "include/sdk_inc/tuya_ipc_api.h"
#include "include/tuya_ipc_dp_utils.h"
#include "include/tuya_ipc_dp_handler.h"
#include "include/sdk_inc/tuya_cloud_com_defs.h"
#include "include/sdk_inc/tuya_iot_config.h"
#include "include/sdk_inc/tuya_iot_com_api.h"

#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#include "Log/DebugDef.h"

#include "PAL/System.h"

#ifdef TUYA_DP_AI_FACE_DET

BOOL_T IPC_APP_get_ai_onoff(VOID);
VOID IPC_APP_set_ai_onoff(BOOL_T ai_on_off);
#endif

STATIC VOID respone_dp_value(BYTE_T dp_id, INT_T val);
STATIC VOID respone_dp_bool(BYTE_T dp_id, BOOL_T true_false);
STATIC VOID respone_dp_enum(BYTE_T dp_id, CHAR_T *p_val_enum);
STATIC VOID respone_dp_str(BYTE_T dp_id, CHAR_T *p_val_str);
STATIC VOID handle_DP_SD_STORAGE_ONLY_GET(IN TY_OBJ_DP_S *p_obj_dp);

//------------------------------------------
VOID IPC_APP_upload_all_status(VOID)
{
#ifdef TUYA_DP_SLEEP_MODE
    respone_dp_bool(TUYA_DP_SLEEP_MODE, IPC_APP_get_sleep_mode() );
#endif

#ifdef TUYA_DP_LIGHT
    respone_dp_bool(TUYA_DP_LIGHT, IPC_APP_get_light_onoff() );
#endif

#ifdef TUYA_DP_AI_FACE_DET
    respone_dp_bool(TUYA_DP_AI_FACE_DET, IPC_APP_get_ai_onoff() );
#endif


#ifdef TUYA_DP_FLIP
    respone_dp_bool(TUYA_DP_FLIP, IPC_APP_get_flip_onoff() );
#endif

#ifdef TUYA_DP_WATERMARK
    respone_dp_bool(TUYA_DP_WATERMARK, IPC_APP_get_watermark_onoff() );
#endif

#ifdef TUYA_DP_WDR
    respone_dp_bool(TUYA_DP_WDR, IPC_APP_get_wdr_onoff() );
#endif

#ifdef TUYA_DP_PIR_SWITCH1
    respone_dp_bool(TUYA_DP_PIR_SWITCH1, IPC_APP_get_PIR1_onoff() );
#endif

#ifdef TUYA_DP_PIR_SWITCH2
    respone_dp_bool(TUYA_DP_PIR_SWITCH2, IPC_APP_get_PIR2_onoff() );
#endif

#ifdef TUYA_DP_PIR_SWITCH3
    respone_dp_bool(TUYA_DP_PIR_SWITCH3, IPC_APP_get_PIR3_onoff() );
#endif

#ifdef TUYA_DP_NIGHT_MODE
    respone_dp_enum(TUYA_DP_NIGHT_MODE, IPC_APP_get_night_mode() );
#endif

#ifdef TUYA_DP_NIGHT_MODE_ANO
    respone_dp_enum(TUYA_DP_NIGHT_MODE_ANO, IPC_APP_get_night_mode_Ano() );
#endif

#ifdef TUYA_DP_ALARM_FUNCTION
    respone_dp_bool(TUYA_DP_ALARM_FUNCTION, IPC_APP_get_alarm_function_onoff() );
#endif

#ifdef TUYA_DP_MOTION_TIMER_SWITCH
respone_dp_bool(TUYA_DP_MOTION_TIMER_SWITCH, IPC_APP_get_motion_timer_switch() );
#endif

#ifdef TUYA_DP_MOTION_DETECT_TIMER
respone_dp_str(TUYA_DP_MOTION_DETECT_TIMER, IPC_APP_get_motion_detect_timer() );
#endif

#ifdef TUYA_DP_ALARM_SENSITIVITY
    respone_dp_enum(TUYA_DP_ALARM_SENSITIVITY, IPC_APP_get_alarm_sensitivity() );
#endif

#ifdef TUYA_DP_ALARM_ZONE_ENABLE
    respone_dp_bool(TUYA_DP_ALARM_ZONE_ENABLE, IPC_APP_get_alarm_zone_onoff() );
#endif

#ifdef TUYA_DP_ALARM_ZONE_DRAW
    respone_dp_str(TUYA_DP_ALARM_ZONE_DRAW, IPC_APP_get_alarm_zone_draw());
#endif

#ifdef TUYA_DP_ALARM_ZONE_DRAW_EX
    //respone_dp_str(TUYA_DP_ALARM_ZONE_DRAW_EX, IPC_APP_get_alarm_zone_draw_ex());
    char *text = IPC_APP_get_alarm_zone_draw_ex();
    int size = strlen(text)+1;

    printf("*************text=%s\n",text);
    printf("*************size=%d\n",size);

    dev_report_dp_raw_sync(NULL,TUYA_DP_ALARM_ZONE_DRAW_EX,(unsigned char *)text,size,1);
#endif

#ifdef TUYA_DP_SD_STATUS_ONLY_GET
    respone_dp_value(TUYA_DP_SD_STATUS_ONLY_GET, IPC_APP_get_sd_status() );
#endif

#ifdef TUYA_DP_SD_STORAGE_ONLY_GET
    handle_DP_SD_STORAGE_ONLY_GET(NULL);
#endif

#ifdef TUYA_DP_SD_RECORD_ENABLE
    respone_dp_bool(TUYA_DP_SD_RECORD_ENABLE, IPC_APP_get_sd_record_onoff() );
#endif

#ifdef TUYA_DP_SD_RECORD_MODE
    CHAR_T sd_mode[4];
    snprintf(sd_mode,4,"%d",IPC_APP_get_sd_record_mode());
    respone_dp_enum(TUYA_DP_SD_RECORD_MODE, sd_mode);
#endif


#ifdef TUYA_DP_SD_FORMAT_STATUS_ONLY_GET
    respone_dp_value(TUYA_DP_SD_FORMAT_STATUS_ONLY_GET, 0 );
#endif

#ifdef TUYA_DP_BLUB_SWITCH
    respone_dp_bool(TUYA_DP_BLUB_SWITCH, IPC_APP_get_blub_onoff() );
#endif

#ifdef TUYA_DP_POWERMODE
    IPC_APP_update_battery_status();
#endif

#ifdef TUYA_DP_MOTION_TRACK
    respone_dp_bool(TUYA_DP_MOTION_TRACK, IPC_APP_get_motion_track_onoff() );
#endif

#ifdef TUYA_DP_MOTION_INTERVAL
    CHAR_T interval_mode[4];
    snprintf(interval_mode,4,"%d",IPC_APP_get_motion_interval());
    respone_dp_enum(TUYA_DP_MOTION_INTERVAL, interval_mode);
#endif

#ifdef TUYA_DP_ANBAO_LIGHT
    respone_dp_bool(TUYA_DP_ANBAO_LIGHT, IPC_APP_get_anbao_light_onoff() );
#endif

#ifdef TUYA_DP_FLOODLIGHT_LIGHTNESS
    respone_dp_value(TUYA_DP_FLOODLIGHT_LIGHTNESS, IPC_APP_get_floodlight_lightness() );
#endif

#ifdef TUYA_DP_FLOODLIGHT_YELLOW
    respone_dp_value(TUYA_DP_FLOODLIGHT_YELLOW, IPC_APP_get_floodlight_yellow() );
#endif

#ifdef TUYA_DP_FLIGHT_WARN_SWITCH
     respone_dp_bool(TUYA_DP_FLIGHT_WARN_SWITCH, IPC_APP_get_flight_warn_switch() );
#endif

#ifdef TUYA_DP_FLIGHT_WARN_TIME
    respone_dp_value(TUYA_DP_FLIGHT_WARN_TIME, IPC_APP_get_flight_warn_time());
#endif

#ifdef TUYA_DP_MUTE_RECORD
    respone_dp_bool(TUYA_DP_MUTE_RECORD, IPC_APP_get_mute_record() );
#endif

#ifdef TUYA_DP_PIR_SWITCH_SENS
    CHAR_T pir_mode_sens[4];
    snprintf(pir_mode_sens,4,"%d",IPC_APP_get_pir_switch_sens());
    respone_dp_enum(TUYA_DP_PIR_SWITCH_SENS, pir_mode_sens);
#endif

#ifdef TUYA_DP_PIR_SWITCH_TIME
    CHAR_T pir_switch_time[4];
    snprintf(pir_switch_time,4,"%d",IPC_APP_get_pir_switch_time());
    respone_dp_enum(TUYA_DP_PIR_SWITCH_TIME, pir_switch_time);
#endif

#ifdef TUYA_DP_SET_LUX_LEVEL
    CHAR_T lux_level[4];
    snprintf(lux_level,4,"%d",IPC_APP_get_lux_level());
    respone_dp_enum(TUYA_DP_SET_LUX_LEVEL, lux_level);
#endif

#ifdef TUYA_DP_PIR_SWITCH
    CHAR_T pir_mode[4];
    snprintf(pir_mode,4,"%d",IPC_APP_get_pir_switch());
    respone_dp_enum(TUYA_DP_PIR_SWITCH, pir_mode);
#endif

#ifdef TUYA_DP_HUM_FILTER
    respone_dp_bool(TUYA_DP_HUM_FILTER, IPC_APP_get_human_filter() );
#endif

#ifdef TUYA_DP_SIREN_SWITCH
    respone_dp_bool(TUYA_DP_SIREN_SWITCH, IPC_APP_get_siren() );
#endif

#ifdef TUYA_DP_IPC_USAGE_MODE
    CHAR_T usage_mode[4];
    snprintf(usage_mode,4,"%d",IPC_APP_get_ipc_usage_mode());
    respone_dp_enum(TUYA_DP_IPC_USAGE_MODE, usage_mode);
#endif

#ifdef TUYA_DP_ANTI_FLICKER
    respone_dp_enum(TUYA_DP_ANTI_FLICKER, IPC_APP_get_anti_flicker());
#endif

#ifdef TUYA_DP_VOICE_ONOFF
    respone_dp_bool(TUYA_DP_VOICE_ONOFF, IPC_APP_get_voice_onoff() );
#endif

#ifdef TUYA_DP_VOICE_SENSITIVITY
	 CHAR_T change_sen[4];
    snprintf(change_sen,4,"%d",IPC_APP_get_voice_sensitivity());
    respone_dp_enum(TUYA_DP_VOICE_SENSITIVITY, change_sen);
#endif
#ifdef TUYA_DP_SIREN_LINKAGE
    respone_dp_bool(TUYA_DP_SIREN_LINKAGE, IPC_APP_get_siren_onoff() );
#endif

#ifdef TUYA_DP_SIREN_SOUND
	CHAR_T siren_sound[4];
	snprintf(siren_sound,4,"%d",IPC_APP_get_Siren_Sound());
    respone_dp_enum(TUYA_DP_SIREN_SOUND,siren_sound);
#endif

#ifdef TUYA_DP_SIREN_DURATION
    respone_dp_value(TUYA_DP_SIREN_DURATION,IPC_APP_get_Siren_Duration());
#endif

#ifdef TUYA_DP_SIREN_VOLUME
    respone_dp_value(TUYA_DP_SIREN_VOLUME,IPC_APP_get_Siren_Volume());
#endif

#ifdef TUYA_DP_IPC_VOLUME
    respone_dp_value(TUYA_DP_IPC_VOLUME,IPC_APP_get_Ipc_Volume());
#endif

#ifdef TUYA_DP_MEMORY_POINT
	IPC_APP_get_preset();
	//respone_dp_enum(TUYA_DP_CRUISE_STA, IPC_APP_get_cruise_sta() );
#endif
#ifdef TUYA_DP_ONVIF_SWITCH
	{
		BOOL_T bOnvifSwitch = FALSE;
		BOOL_T bPwdChanged = FALSE;
		bOnvifSwitch = IPC_APP_get_onvif_switch(&bPwdChanged);
	    respone_dp_bool(TUYA_DP_ONVIF_SWITCH, bOnvifSwitch );
		if( (TRUE == bOnvifSwitch) && (FALSE == bPwdChanged) )
		{
			IPC_APP_REPORT_ONVIF_PW_CHANGED(FALSE);
		}
	}
#endif
}

#ifdef TUYA_DP_DOOR_BELL
VOID IPC_APP_trigger_door_bell(VOID)
{
    AppInfo("door bell is triggered\r\n");

    BYTE_T wakeup_data_arr[10] = {0};
    UINT_T wakeup_data_len = 10;
    CHAR_T data[128] = {0};
    unsigned int wakeup_data = 0;
//<shang>    tuya_iot_get_wakeup_data(wakeup_data_arr, &wakeup_data_len);
    wakeup_data = (wakeup_data_arr[8] & 0xFF) |
                 ((wakeup_data_arr[7] << 8) & (0xFF00)) |
                 ((wakeup_data_arr[7] <<16) & (0xFF0000)) |
                 ((wakeup_data_arr[7] <<24) & (0xFF000000));

    TIME_T timeutc = 0;
    INT_T timezone = 0;
    tuya_ipc_get_service_time(&timeutc, &timezone);
    snprintf(data, 128, "{\"etype\":\"doorbell_press\",\"edata\":\"%x%d\"}", wakeup_data, (INT_T)timeutc);
    AppInfo("DoorBell PUSH:%s\r\n", data);
//<shang>    tuya_iot_send_custom_mqtt_msg(43, (BYTE_T*)data);


    UINT_T intval = time(NULL);
    CHAR_T strval[64] = {0};
    snprintf(strval, 64, "%d", intval);
    respone_dp_str(TUYA_DP_DOOR_BELL, strval);
}
#endif

#ifdef TUYA_DP_SD_FORMAT_STATUS_ONLY_GET
VOID IPC_APP_report_sd_format_status(INT_T status)
{
    respone_dp_value(TUYA_DP_SD_FORMAT_STATUS_ONLY_GET, status);
}
#endif

#ifdef TUYA_DP_ANBAO_LIGHT
VOID IPC_APP_report_anbao_light_status(BOOL_T status)
{
    AppWarning("IPC_APP_report_anbao_light_status status:%d\n\n",status);
    respone_dp_bool(TUYA_DP_ANBAO_LIGHT, status );
}
#endif

#ifdef TUYA_DP_SIREN_SWITCH
VOID IPC_APP_report_siren_status(BOOL_T status)
{
    AppWarning("IPC_APP_report_siren_status status:%d\n\n",status);
    respone_dp_bool(TUYA_DP_SIREN_SWITCH, status );
}
#endif

#ifdef TUYA_DP_SD_STATUS_ONLY_GET
VOID IPC_APP_report_sd_status_changed()
{
    respone_dp_value(TUYA_DP_SD_STATUS_ONLY_GET, IPC_APP_get_sd_status());
}
#endif

#ifdef TUYA_DP_SD_STORAGE_ONLY_GET
VOID IPC_APP_report_sd_storage()
{
    CHAR_T tmp_str[100] = {0};

    UINT_T total = 100;
    UINT_T used = 0;
    UINT_T empty = 100;
    IPC_APP_get_sd_storage(&total, &used, &empty);

    //"总容量|当前使用量|剩余容量"
    snprintf(tmp_str, 100, "%u|%u|%u", total, used, empty);
    respone_dp_str(TUYA_DP_SD_STORAGE_ONLY_GET, tmp_str);
}
#endif


#ifdef TUYA_DP_POWERMODE
VOID IPC_APP_update_battery_status(VOID)
{
    CHAR_T *power_mode = IPC_APP_get_power_mode();
    INT_T percent = IPC_APP_get_battery_percent();

    AppInfo("current power mode:%s\r\n", power_mode);
    respone_dp_enum(TUYA_DP_POWERMODE, power_mode);
    AppInfo("current battery percent:%d\r\n", percent);
    respone_dp_value(TUYA_DP_ELECTRICITY, percent);
}
#endif

//------------------------------------------
STATIC VOID respone_dp_value(BYTE_T dp_id, INT_T val)
{
    tuya_ipc_dp_report(NULL, dp_id,PROP_VALUE,&val,1);
}

STATIC VOID respone_dp_bool(BYTE_T dp_id, BOOL_T true_false)
{
    tuya_ipc_dp_report(NULL, dp_id,PROP_BOOL,&true_false,1);
}

STATIC VOID respone_dp_enum(BYTE_T dp_id, CHAR_T *p_val_enum)
{
    tuya_ipc_dp_report(NULL, dp_id,PROP_ENUM,p_val_enum,1);
}

STATIC VOID respone_dp_str(BYTE_T dp_id, CHAR_T *p_val_str)
{
    tuya_ipc_dp_report(NULL, dp_id,PROP_STR,p_val_str,1);
}

//------------------------------------------
STATIC BOOL_T check_dp_bool_invalid(IN TY_OBJ_DP_S *p_obj_dp)
{
    if(p_obj_dp == NULL)
    {
        AppErr("error! input is null \r\n");
        return -1;
    }

    if(p_obj_dp->type != PROP_BOOL)
    {
        AppErr("error! input is not bool %d \r\n", p_obj_dp->type);
        return -2;
    }

    if(p_obj_dp->value.dp_bool == 0)
    {
        return FALSE;
    }
    else if(p_obj_dp->value.dp_bool == 1)
    {
        return TRUE;
    }else
    {
        AppErr("Error!! type invalid %d \r\n", p_obj_dp->value.dp_bool);
        return -2;
    }
}

//------------------------------------------

#ifdef TUYA_DP_SLEEP_MODE
STATIC VOID handle_DP_SLEEP_MODE(IN TY_OBJ_DP_S *p_obj_dp)
{
    BOOL_T sleep_mode = check_dp_bool_invalid(p_obj_dp);

    IPC_APP_set_sleep_mode(sleep_mode);
    sleep_mode = IPC_APP_get_sleep_mode();

    respone_dp_bool(TUYA_DP_SLEEP_MODE, sleep_mode);
}
#endif

#ifdef TUYA_DP_LIGHT
STATIC VOID handle_DP_LIGHT(IN TY_OBJ_DP_S *p_obj_dp)
{
    BOOL_T light_on_off = check_dp_bool_invalid(p_obj_dp);

    IPC_APP_set_light_onoff(light_on_off);
    light_on_off = IPC_APP_get_light_onoff();

    respone_dp_bool(TUYA_DP_LIGHT, light_on_off);
}
#endif

#ifdef TUYA_DP_AI_FACE_DET
STATIC VOID handle_DP_AI_FACE_DET(IN TY_OBJ_DP_S *p_obj_dp)
{
    BOOL_T ai_on_off = check_dp_bool_invalid(p_obj_dp);

    IPC_APP_set_ai_onoff(ai_on_off);
    ai_on_off = IPC_APP_get_ai_onoff();

    respone_dp_bool(TUYA_DP_AI_FACE_DET, ai_on_off);
}
#endif


#ifdef TUYA_DP_FLIP
STATIC VOID handle_DP_FLIP(IN TY_OBJ_DP_S *p_obj_dp)
{
    BOOL_T flip_on_off = check_dp_bool_invalid(p_obj_dp);

    IPC_APP_set_flip_onoff(flip_on_off);
    flip_on_off = IPC_APP_get_flip_onoff();

    respone_dp_bool(TUYA_DP_FLIP, flip_on_off);
}
#endif

#ifdef TUYA_DP_WATERMARK
STATIC VOID handle_DP_WATERMARK(IN TY_OBJ_DP_S *p_obj_dp)
{
    BOOL_T watermark_on_off = check_dp_bool_invalid(p_obj_dp);

    IPC_APP_set_watermark_onoff(watermark_on_off);
    watermark_on_off = IPC_APP_get_watermark_onoff();

    respone_dp_bool(TUYA_DP_WATERMARK, watermark_on_off);
}
#endif

#ifdef TUYA_DP_WDR
STATIC VOID handle_DP_WDR(IN TY_OBJ_DP_S *p_obj_dp)
{
    BOOL_T wdr_on_off = check_dp_bool_invalid(p_obj_dp);

    IPC_APP_set_wdr_onoff(wdr_on_off);
    wdr_on_off = IPC_APP_get_wdr_onoff();

    respone_dp_bool(TUYA_DP_WDR, wdr_on_off);
}
#endif

#ifdef TUYA_DP_NIGHT_MODE
STATIC VOID handle_DP_NIGHT_MODE(IN TY_OBJ_DP_S *p_obj_dp)
{
    if( (p_obj_dp == NULL) || (p_obj_dp->type != PROP_ENUM) )
    {
        AppErr("Error!! type invalid %d \r\n", p_obj_dp->type);
        return;
    }
    CHAR_T tmp_str[2] = {0};
    tmp_str[0] = '0' + p_obj_dp->value.dp_enum;

    IPC_APP_set_night_mode(tmp_str);
    CHAR_T *p_night_mode = IPC_APP_get_night_mode();

    respone_dp_enum(TUYA_DP_NIGHT_MODE, p_night_mode);
}
#endif

#ifdef TUYA_DP_NIGHT_MODE_ANO
STATIC VOID handle_DP_NIGHT_MODE_ANO(IN TY_OBJ_DP_S *p_obj_dp)
{
    if( (p_obj_dp == NULL) || (p_obj_dp->type != PROP_ENUM) )
    {
        AppErr("Error!! type invalid %d \r\n", p_obj_dp->type);
        return;
    }
    CHAR_T tmp_str[2] = {0};
    tmp_str[0] = '0' + p_obj_dp->value.dp_enum;

    IPC_APP_set_night_mode_Ano(tmp_str);
    CHAR_T *p_night_mode = IPC_APP_get_night_mode_Ano();

    respone_dp_enum(TUYA_DP_NIGHT_MODE_ANO, p_night_mode);
}
#endif


#ifdef TUYA_DP_CALIBRATION
STATIC VOID handle_DP_CALIBRATION(IN TY_OBJ_DP_S *p_obj_dp)
{
    IPC_APP_start_calibration();
    respone_dp_bool(TUYA_DP_CALIBRATION, TRUE);
}
#endif

#ifdef TUYA_DP_ALARM_FUNCTION
STATIC VOID handle_DP_ALARM_FUNCTION(IN TY_OBJ_DP_S *p_obj_dp)
{
    BOOL_T alarm_on_off = check_dp_bool_invalid(p_obj_dp);

    IPC_APP_set_alarm_function_onoff(alarm_on_off);
    alarm_on_off = IPC_APP_get_alarm_function_onoff();

    respone_dp_bool(TUYA_DP_ALARM_FUNCTION, alarm_on_off);
}
#endif

#ifdef TUYA_DP_MOTION_TIMER_SWITCH
STATIC VOID handle_DP_MOTION_TIMER_SWITCH(IN TY_OBJ_DP_S *p_obj_dp)
{
    BOOL_T switch_on_off = check_dp_bool_invalid(p_obj_dp);

    IPC_APP_set_motion_timer_switch(switch_on_off);
    switch_on_off = IPC_APP_get_motion_timer_switch();

    respone_dp_bool(TUYA_DP_MOTION_TIMER_SWITCH, switch_on_off);
}
#endif

#ifdef TUYA_DP_MOTION_DETECT_TIMER
STATIC VOID handle_DP_MOTION_DETECT_TIMER(IN TY_OBJ_DP_S *p_obj_dp)
{
    if( (p_obj_dp == NULL) || (p_obj_dp->type != PROP_STR) )
    {
        AppErr("Error!! type invalid %d \r\n", p_obj_dp->type);
        return;
    }

    CHAR_T *p_timer = p_obj_dp->value.dp_str;

    IPC_APP_set_motion_detect_timer(p_timer);
    p_timer = IPC_APP_get_motion_detect_timer();

    respone_dp_str(TUYA_DP_MOTION_DETECT_TIMER, p_timer);
}
#endif

#ifdef TUYA_DP_ALARM_SENSITIVITY
STATIC VOID handle_DP_ALARM_SENSITIVITY(IN TY_OBJ_DP_S *p_obj_dp)
{
    if( (p_obj_dp == NULL) || (p_obj_dp->type != PROP_ENUM) )
    {
        AppErr("Error!! type invalid %d \r\n", p_obj_dp->type);
        return;
    }

    CHAR_T tmp_str[2] = {0};
    tmp_str[0] = '0' + p_obj_dp->value.dp_enum;

    IPC_APP_set_alarm_sensitivity(tmp_str);
    CHAR_T *p_sensitivity = IPC_APP_get_alarm_sensitivity();

    respone_dp_enum(TUYA_DP_ALARM_SENSITIVITY, p_sensitivity);
}
#endif

#ifdef TUYA_DP_ALARM_ZONE_ENABLE
STATIC VOID handle_DP_ALARM_ZONE_ENABLE(IN TY_OBJ_DP_S *p_dp_json)
{
    if(p_dp_json == NULL )
    {
        AppErr("Error!! type invalid %p \r\n", p_dp_json);
        return;
    }
    BOOL_T alarm_zone_enable = check_dp_bool_invalid(p_dp_json);
    IPC_APP_set_alarm_zone_onoff(alarm_zone_enable);
    respone_dp_bool(TUYA_DP_ALARM_ZONE_ENABLE, IPC_APP_get_alarm_zone_onoff());
}
#endif

#ifdef TUYA_DP_ALARM_ZONE_DRAW
STATIC VOID handle_DP_ALARM_ZONE_DRAW(IN TY_OBJ_DP_S *p_dp_json)
{
    if(p_dp_json == NULL )
    {
        AppErr("Error!! type invalid\r\n");
        return;
    }
    IPC_APP_set_alarm_zone_draw((cJSON *)(p_dp_json->value.dp_str));
    respone_dp_str(TUYA_DP_ALARM_ZONE_DRAW, IPC_APP_get_alarm_zone_draw());
}
#endif

#ifdef TUYA_DP_ALARM_ZONE_DRAW_EX
STATIC VOID handle_DP_ALARM_ZONE_DRAW_EX(IN TY_OBJ_DP_S *p_dp_json)
{
    if(p_dp_json == NULL )
    {
        printf("Error!! type invalid\r\n");
        return;
    }
	printf("***********set_zone_draw_ex**********\n");
    IPC_APP_set_alarm_zone_draw_ex((cJSON *)(p_dp_json));
    respone_dp_str(TUYA_DP_ALARM_ZONE_DRAW_EX, IPC_APP_get_alarm_zone_draw_ex());
}
#endif

#ifdef TUYA_DP_SD_STATUS_ONLY_GET
STATIC VOID handle_DP_SD_STATUS_ONLY_GET(IN TY_OBJ_DP_S *p_obj_dp)
{
    INT_T sd_status = IPC_APP_get_sd_status();

    respone_dp_value(TUYA_DP_SD_STATUS_ONLY_GET, sd_status);
}
#endif

#ifdef TUYA_DP_SD_STORAGE_ONLY_GET
STATIC VOID handle_DP_SD_STORAGE_ONLY_GET(IN TY_OBJ_DP_S *p_obj_dp)
{
    CHAR_T tmp_str[100] = {0};

    UINT_T total = 100;
    UINT_T used = 0;
    UINT_T empty = 100;
    IPC_APP_get_sd_storage(&total, &used, &empty);

    //"总容量|当前使用量|剩余容量"
    snprintf(tmp_str, 100, "%u|%u|%u", total, used, empty);
    respone_dp_str(TUYA_DP_SD_STORAGE_ONLY_GET, tmp_str);
}
#endif

#ifdef TUYA_DP_SD_RECORD_ENABLE
STATIC VOID handle_DP_SD_RECORD_ENABLE(IN TY_OBJ_DP_S *p_obj_dp)
{
    BOOL_T sd_record_on_off = check_dp_bool_invalid(p_obj_dp);

    IPC_APP_set_sd_record_onoff(sd_record_on_off);
    sd_record_on_off = IPC_APP_get_sd_record_onoff();

    respone_dp_bool(TUYA_DP_SD_RECORD_ENABLE, sd_record_on_off);
}
#endif

#ifdef TUYA_DP_SD_RECORD_MODE
STATIC VOID handle_DP_SD_RECORD_MODE(IN TY_OBJ_DP_S *p_obj_dp)
{
    if( (p_obj_dp == NULL) || (p_obj_dp->type != PROP_ENUM) )
    {
        AppErr("Error!! type invalid %d \r\n", p_obj_dp->type);
        return;
    }

    IPC_APP_set_sd_record_mode(p_obj_dp->value.dp_enum);
    UINT_T mode = IPC_APP_get_sd_record_mode();
    CHAR_T sMode[2];
    snprintf(sMode,2,"%d",mode);
    respone_dp_enum(TUYA_DP_SD_RECORD_MODE,sMode);
}
#endif

#ifdef TUYA_DP_SD_UMOUNT
STATIC VOID handle_DP_SD_UMOUNT(IN TY_OBJ_DP_S *p_obj_dp)
{
    BOOL_T umount_result = IPC_APP_unmount_sd_card();
    respone_dp_bool(TUYA_DP_SD_UMOUNT, umount_result);
}
#endif

#ifdef TUYA_DP_SD_FORMAT
STATIC VOID handle_DP_SD_FORMAT(IN TY_OBJ_DP_S *p_obj_dp)
{
    IPC_APP_format_sd_card();
    respone_dp_bool(TUYA_DP_SD_FORMAT, TRUE);
}
#endif

#ifdef TUYA_DP_SD_FORMAT_STATUS_ONLY_GET
STATIC VOID handle_DP_SD_FORMAT_STATUS_ONLY_GET(IN TY_OBJ_DP_S *p_obj_dp)
{
    INT_T progress = IPC_APP_get_sd_format_status();
    respone_dp_value(TUYA_DP_SD_FORMAT_STATUS_ONLY_GET, progress);
}
#endif

#ifdef TUYA_DP_PTZ_CONTROL
STATIC VOID handle_DP_PTZ_CONTROL(IN TY_OBJ_DP_S *p_obj_dp)
{
    if( (p_obj_dp == NULL) || (p_obj_dp->type != PROP_ENUM) )
    {
        AppErr("Error!! type invalid %d \r\n", p_obj_dp->type);
        return;
    }

    //dp 119 format: {"range":["1","2","3","4","5","6","7","0"],"type":"enum"}
    UINT_T dp_directions[8] = {0,1,2,3,4,5,6,7};
    UINT_T direction = dp_directions[p_obj_dp->value.dp_enum];
    CHAR_T tmp_str[2] = {0};
    snprintf(tmp_str,2,"%d",direction);    
    IPC_APP_ptz_start_move(tmp_str);
    respone_dp_enum(TUYA_DP_PTZ_CONTROL,tmp_str);
}
#endif

#ifdef TUYA_DP_PTZ_STOP
STATIC VOID handle_DP_PTZ_STOP(IN TY_OBJ_DP_S *p_obj_dp)
{
    IPC_APP_ptz_stop_move();
    respone_dp_bool(TUYA_DP_PTZ_STOP, TRUE);
}
#endif

#ifdef TUYA_DP_PTZ_CHECK
STATIC VOID handle_DP_PTZ_CHECK(IN TY_OBJ_DP_S *p_obj_dp)
{
    IPC_APP_ptz_check();
    respone_dp_bool(TUYA_DP_PTZ_CHECK, TRUE);
}
#endif

#ifdef TUYA_DP_EPTZ_CONTROL
STATIC VOID handle_DP_EPTZ_CONTROL(IN TY_OBJ_DP_S *p_obj_dp)
{
    if( (p_obj_dp == NULL) || (p_obj_dp->type != PROP_ENUM) )
    {
        AppErr("Error!! type invalid %d \r\n", p_obj_dp->type);
        return;
    }

    //dp 119 format: {"range":["0","1"],"type":"enum"}
    UINT_T dp_directions[2] = {0,1};
    UINT_T direction = dp_directions[p_obj_dp->value.dp_enum];
    CHAR_T tmp_str[2] = {0};
    snprintf(tmp_str,2,"%d",direction);    
    IPC_APP_eptz_start_move(tmp_str);
    respone_dp_enum(TUYA_DP_EPTZ_CONTROL,tmp_str);
}
#endif

#ifdef TUYA_DP_EPTZ_STOP
STATIC VOID handle_DP_EPTZ_STOP(IN TY_OBJ_DP_S *p_obj_dp)
{
    IPC_APP_eptz_stop_move();
    respone_dp_bool(TUYA_DP_EPTZ_STOP, TRUE);
}
#endif

#ifdef TUYA_DP_TRACK_ENABLE
STATIC VOID handle_DP_TRACK_ENABLE(IN TY_OBJ_DP_S *p_obj_dp)
{
    BOOL_T track_enable = check_dp_bool_invalid(p_obj_dp);

    IPC_APP_track_enable(track_enable);

    respone_dp_bool(TUYA_DP_TRACK_ENABLE, track_enable);
}

#endif

#ifdef TUYA_DP_LINK_MOVE_ACTION
STATIC VOID handle_DP_LINK_MOVE(IN TY_OBJ_DP_S *p_obj_dp)
{
    if( (p_obj_dp == NULL) || (p_obj_dp->type != PROP_ENUM) )
    {
        printf("Error!! type invalid %d \r\n", p_obj_dp->type);
        return;
    }

    CHAR_T tmp_str[2] = {0};
    int bind_move = 0;
    bind_move = p_obj_dp->value.dp_enum;
    tmp_str[0] = '0' + p_obj_dp->value.dp_enum;

    IPC_APP_set_link_move(bind_move);
    respone_dp_enum(TUYA_DP_LINK_MOVE_ACTION, tmp_str);
}
#endif

#ifdef TUYA_DP_LINK_MOVE_SET
STATIC VOID handle_DP_LINK_MOVE_SET(IN TY_OBJ_DP_S *p_obj_dp)
{
    if( (p_obj_dp == NULL) || (p_obj_dp->type != PROP_ENUM) )
    {
        printf("Error!! type invalid %d \r\n", p_obj_dp->type);
        return;
    }

    CHAR_T tmp_str[2] = {0};
    int bind_move = 0;
    bind_move = p_obj_dp->value.dp_enum;
    tmp_str[0] = '0' + p_obj_dp->value.dp_enum;

    IPC_APP_set_link_pos(bind_move);
    respone_dp_enum(TUYA_DP_LINK_MOVE_SET, tmp_str);
}
#endif


#ifdef TUYA_DP_HUM_FILTER
STATIC VOID handle_DP_HUM_FILTER(IN TY_OBJ_DP_S *p_obj_dp)
{
    BOOL_T hum_filter = check_dp_bool_invalid(p_obj_dp);

    IPC_APP_set_human_filter(hum_filter);
	hum_filter = IPC_APP_get_human_filter();
 
    respone_dp_bool(TUYA_DP_HUM_FILTER, hum_filter);
}
#endif

#ifdef TUYA_DP_PATROL_MODE
STATIC VOID handle_DP_patrol_mode(IN TY_OBJ_DP_S *p_obj_dp)
{
    if( (p_obj_dp == NULL) || (p_obj_dp->type != PROP_ENUM) )
    {
        printf("Error!! type invalid %d \r\n", p_obj_dp->type);
        return;
    }
    IPC_APP_set_patrol_mode(p_obj_dp->value.dp_enum);
    CHAR_T sMode[2];
    snprintf(sMode,2,"%d",p_obj_dp->value.dp_enum);

    respone_dp_enum(TUYA_DP_PATROL_MODE,sMode);
}


#endif

#ifdef TUYA_DP_PATROL_SWITCH
STATIC VOID handle_DP_patrol_switch(IN TY_OBJ_DP_S *p_obj_dp)
{
    BOOL_T patrol_mode = check_dp_bool_invalid(p_obj_dp);

    IPC_APP_set_patrol_switch(patrol_mode);
 
    respone_dp_bool(TUYA_DP_PATROL_SWITCH, patrol_mode);
}
#endif

#ifdef TUYA_DP_PATROL_TMODE
STATIC VOID handle_DP_patrol_tmode(IN TY_OBJ_DP_S *p_obj_dp)
{
    if( (p_obj_dp == NULL) || (p_obj_dp->type != PROP_ENUM) )
    {
        printf("Error!! type invalid %d \r\n", p_obj_dp->type);
        return;
    }
    IPC_APP_set_patrol_tmode(p_obj_dp->value.dp_enum);
    CHAR_T sMode[2];
    snprintf(sMode,2,"%d",p_obj_dp->value.dp_enum);
    respone_dp_enum(TUYA_DP_PATROL_TMODE,sMode);
}


#endif

#ifdef TUYA_DP_PATROL_TIME
STATIC VOID handle_DP_patrol_time(IN TY_OBJ_DP_S *p_dp_json)
{
    printf("---%s---\n",p_dp_json->value.dp_str);
    IPC_APP_set_patrol_time((cJSON *)(p_dp_json->value.dp_str));
    return ;
}
#endif
#ifdef TUYA_DP_PATROL_STATE
STATIC VOID handle_DP_patrol_state(IN TY_OBJ_DP_S *p_dp_json)
{
    int patrol_state = 0;
    //printf("---get_patrol_state\n");
    IPC_APP_patrol_state(&patrol_state);
    printf("---get_patrol_state:%d\n",patrol_state);

    CHAR_T sd_mode[4];
    snprintf(sd_mode,4,"%d",patrol_state);
    respone_dp_enum(TUYA_DP_PATROL_STATE, sd_mode);
    return ;
}
#endif

#ifdef TUYA_DP_PRESET_SET
STATIC VOID handle_DP_SET_PRESET(IN TY_OBJ_DP_S *p_dp_json)
{
  
    IPC_APP_set_preset((cJSON *)(p_dp_json->value.dp_str));
    return;
}
#endif


#ifdef TUYA_DP_DOOR_BELL
STATIC VOID handle_DP_DOOR_BELL(IN TY_OBJ_DP_S *p_obj_dp)
{
    AppErr("error! door bell can only trigged by IPC side.\r\n");
    respone_dp_str(TUYA_DP_DOOR_BELL, (char*)"-1");
}
#endif

#ifdef TUYA_DP_BLUB_SWITCH
STATIC VOID handle_DP_BLUB_SWITCH(IN TY_OBJ_DP_S *p_obj_dp)
{
    BOOL_T blub_on_off = check_dp_bool_invalid(p_obj_dp);

    IPC_APP_set_blub_onoff(blub_on_off);
    blub_on_off = IPC_APP_get_blub_onoff();

    respone_dp_bool(TUYA_DP_BLUB_SWITCH, blub_on_off);
}
#endif

#ifdef TUYA_DP_ELECTRICITY
STATIC VOID handle_DP_ELECTRICITY(IN TY_OBJ_DP_S *p_obj_dp)
{
    INT_T percent = IPC_APP_get_battery_percent();
    AppWarning("current battery percent:%d\r\n", percent);
    respone_dp_value(TUYA_DP_ELECTRICITY, percent);
}
#endif

#ifdef TUYA_DP_POWERMODE
STATIC VOID handle_DP_POWERMODE(IN TY_OBJ_DP_S *p_obj_dp)
{
    CHAR_T *power_mode = IPC_APP_get_power_mode();
    AppWarning("current power mode:%s\r\n", power_mode);
    respone_dp_enum(TUYA_DP_POWERMODE, power_mode);
}
#endif

#ifdef TUYA_DP_LOWELECTRIC
STATIC VOID handle_DP_LOWELECTRIC(IN TY_OBJ_DP_S *p_obj_dp)
{
    if( (p_obj_dp == NULL) || (p_obj_dp->type != PROP_VALUE) )
    {
        AppErr("Error!! type invalid %d \r\n", p_obj_dp->type);
        return;
    }
    respone_dp_value(TUYA_DP_LOWELECTRIC, p_obj_dp->value.dp_value);
}
#endif

#ifdef TUYA_DP_MOTION_TRACK
STATIC VOID handle_DP_MOTION_TRACK(IN TY_OBJ_DP_S *p_obj_dp)
{
    BOOL_T motion_track_on_off = check_dp_bool_invalid(p_obj_dp);

    IPC_APP_set_motion_track_onoff(motion_track_on_off);
    motion_track_on_off = IPC_APP_get_motion_track_onoff();

    respone_dp_bool(TUYA_DP_MOTION_TRACK, motion_track_on_off);
}
#endif

#ifdef TUYA_DP_MOTION_INTERVAL
STATIC VOID handle_DP_MOTION_INTERVAL(IN TY_OBJ_DP_S *p_obj_dp)
{
    if( (p_obj_dp == NULL) || (p_obj_dp->type != PROP_ENUM) )
    {
        AppErr("Error!! type invalid %d \r\n", p_obj_dp->type);
        return;
    }

    IPC_APP_set_motion_interval(p_obj_dp->value.dp_enum);
    UINT_T mode = IPC_APP_get_motion_interval();
    CHAR_T sMode[2];
    snprintf(sMode,2,"%d",mode);
    respone_dp_enum(TUYA_DP_MOTION_INTERVAL,sMode);
}
#endif

/////安保灯 add by ale
#ifdef TUYA_DP_ANBAO_LIGHT
STATIC VOID handle_DP_ANBAO_LIGHT(IN TY_OBJ_DP_S *p_obj_dp)
{
    BOOL_T light_on_off = check_dp_bool_invalid(p_obj_dp);

    IPC_APP_set_anbao_light_onoff(light_on_off);
    light_on_off = IPC_APP_get_anbao_light_onoff();

    respone_dp_bool(TUYA_DP_ANBAO_LIGHT, light_on_off);
}
#endif

#ifdef TUYA_DP_FLOODLIGHT_LIGHTNESS
STATIC VOID handle_DP_FLOODLIGHT_LIGHTNESS(IN TY_OBJ_DP_S *p_obj_dp)
{
    if( (p_obj_dp == NULL) || (p_obj_dp->type != PROP_VALUE) )
    {
        AppErr("Error!! type invalid %d \r\n", p_obj_dp->type);
        return;
    }
    IPC_APP_set_floodlight_lightness(p_obj_dp->value.dp_value);
    INT_T lightness = IPC_APP_get_floodlight_lightness();
    respone_dp_value(TUYA_DP_FLOODLIGHT_LIGHTNESS, lightness);
}
#endif


#ifdef TUYA_DP_FLOODLIGHT_YELLOW
STATIC VOID handle_DP_FLOODLIGHT_YELLOW(IN TY_OBJ_DP_S *p_obj_dp)
{
    if( (p_obj_dp == NULL) || (p_obj_dp->type != PROP_VALUE) )
    {
        AppErr("Error!! type invalid %d \r\n", p_obj_dp->type);
        return;
    }
    IPC_APP_set_floodlight_yellow(p_obj_dp->value.dp_value);
    INT_T lightness = IPC_APP_get_floodlight_yellow();
    respone_dp_value(TUYA_DP_FLOODLIGHT_YELLOW, lightness);
}
#endif


#ifdef TUYA_DP_FLIGHT_WARN_SWITCH
STATIC VOID handle_DP_FLIGHT_WARN_SWITCH(IN TY_OBJ_DP_S *p_obj_dp)
{
    BOOL_T pir_switch_on_off = check_dp_bool_invalid(p_obj_dp);

    IPC_APP_set_flight_warn_switch(pir_switch_on_off);
    pir_switch_on_off = IPC_APP_get_flight_warn_switch();

    respone_dp_bool(TUYA_DP_FLIGHT_WARN_SWITCH, pir_switch_on_off);
}
#endif

#ifdef TUYA_DP_FLIGHT_WARN_TIME
STATIC VOID handle_DP_PIR_ALARM_TIME(IN TY_OBJ_DP_S *p_obj_dp)
{
    if( (p_obj_dp == NULL) || (p_obj_dp->type != PROP_VALUE) )
    {
        AppErr("Error!! type invalid %d \r\n", p_obj_dp->type);
        return;
    }
    IPC_APP_set_flight_warn_time(p_obj_dp->value.dp_value);
    INT_T mode = IPC_APP_get_flight_warn_time();
    respone_dp_value(TUYA_DP_FLIGHT_WARN_TIME, mode);
}
#endif

#ifdef TUYA_DP_MUTE_RECORD
STATIC VOID handle_DP_MUTE_RECORD(IN TY_OBJ_DP_S *p_obj_dp)
{
    BOOL_T mute_record_on_off = check_dp_bool_invalid(p_obj_dp);

    IPC_APP_set_mute_record(mute_record_on_off);
    mute_record_on_off = IPC_APP_get_mute_record();

    respone_dp_bool(TUYA_DP_MUTE_RECORD, mute_record_on_off);
}
#endif
#ifdef TUYA_DP_PIR_SWITCH1
STATIC VOID handle_DP_PIR_SWITCH1(IN TY_OBJ_DP_S *p_obj_dp)
{
    BOOL_T pir_on_off = check_dp_bool_invalid(p_obj_dp);

    IPC_APP_set_PIR1_onoff(pir_on_off);
    pir_on_off = IPC_APP_get_PIR1_onoff();

    respone_dp_bool(TUYA_DP_PIR_SWITCH1, pir_on_off);
}
#endif

#ifdef TUYA_DP_PIR_SWITCH2
STATIC VOID handle_DP_PIR_SWITCH2(IN TY_OBJ_DP_S *p_obj_dp)
{
    BOOL_T pir_on_off = check_dp_bool_invalid(p_obj_dp);

    IPC_APP_set_PIR2_onoff(pir_on_off);
    pir_on_off = IPC_APP_get_PIR2_onoff();

    respone_dp_bool(TUYA_DP_PIR_SWITCH2, pir_on_off);
}
#endif

#ifdef TUYA_DP_PIR_SWITCH3
STATIC VOID handle_DP_PIR_SWITCH3(IN TY_OBJ_DP_S *p_obj_dp)
{
    BOOL_T pir_on_off = check_dp_bool_invalid(p_obj_dp);

    IPC_APP_set_PIR3_onoff(pir_on_off);
    pir_on_off = IPC_APP_get_PIR3_onoff();

    respone_dp_bool(TUYA_DP_PIR_SWITCH3, pir_on_off);
}
#endif
#ifdef TUYA_DP_PIR_SWITCH_SENS
STATIC VOID handle_DP_PIR_SWITCH_SENS(IN TY_OBJ_DP_S *p_obj_dp)
{
    if( (p_obj_dp == NULL) || (p_obj_dp->type != PROP_ENUM) )
    {
        AppErr("Error!! type invalid %d \r\n", p_obj_dp->type);
        return;
    }
    IPC_APP_set_pir_switch_sens(p_obj_dp->value.dp_enum);
    UINT_T mode = IPC_APP_get_pir_switch_sens();
    CHAR_T sMode[2];
    snprintf(sMode,2,"%d",mode);
    respone_dp_enum(TUYA_DP_PIR_SWITCH_SENS,sMode);	
}
#endif

#ifdef TUYA_DP_PIR_SWITCH_TIME
STATIC VOID handle_DP_PIR_SWITCH_TIME(IN TY_OBJ_DP_S *p_obj_dp)
{
    if( (p_obj_dp == NULL) || (p_obj_dp->type != PROP_ENUM) )
    {
        AppErr("Error!! type invalid %d \r\n", p_obj_dp->type);
        return;
    }
    IPC_APP_set_pir_switch_time(p_obj_dp->value.dp_enum);
    UINT_T switch_time = IPC_APP_get_pir_switch_time();
    CHAR_T stime[2];
    snprintf(stime,2,"%d",switch_time);
    respone_dp_enum(TUYA_DP_PIR_SWITCH_TIME,stime);	
}
#endif

#ifdef TUYA_DP_SET_LUX_LEVEL
STATIC VOID handle_DP_LUX_LEVEL(IN TY_OBJ_DP_S *p_obj_dp)
{
    if( (p_obj_dp == NULL) || (p_obj_dp->type != PROP_ENUM) )
    {
        AppErr("Error!! type invalid %d \r\n", p_obj_dp->type);
        return;
    }
    IPC_APP_set_lux_level(p_obj_dp->value.dp_enum);
    UINT_T mode = IPC_APP_get_lux_level();
    CHAR_T sMode[2];
    snprintf(sMode,2,"%d",mode);
    respone_dp_enum(TUYA_DP_SET_LUX_LEVEL,sMode);	
}
#endif

#ifdef TUYA_DP_PIR_SWITCH
STATIC VOID handle_DP_PIR_SWITCH(IN TY_OBJ_DP_S *p_obj_dp)
{
    if( (p_obj_dp == NULL) || (p_obj_dp->type != PROP_ENUM) )
    {
        AppErr("Error!! type invalid %d \r\n", p_obj_dp->type);
        return;
    }
    IPC_APP_set_pir_switch(p_obj_dp->value.dp_enum);
    UINT_T mode = IPC_APP_get_pir_switch();
    CHAR_T sMode[2];
    snprintf(sMode,2,"%d",mode);
    respone_dp_enum(TUYA_DP_PIR_SWITCH,sMode);	
}
#endif

#ifdef TUYA_DP_SIREN_SWITCH
STATIC VOID handle_DP_SIREN_SWITCH(IN TY_OBJ_DP_S *p_obj_dp)
{
    BOOL_T siren_switch = check_dp_bool_invalid(p_obj_dp);

    IPC_APP_set_siren(siren_switch);
    siren_switch = IPC_APP_get_siren();

    respone_dp_bool(TUYA_DP_SIREN_SWITCH, siren_switch);
}
#endif

#ifdef TUYA_DP_IPC_USAGE_MODE
STATIC VOID handle_DP_IPC_USAGE_MODE(IN TY_OBJ_DP_S *p_obj_dp)
{
    if( (p_obj_dp == NULL) || (p_obj_dp->type != PROP_ENUM) )
    {
        AppErr("Error!! type invalid %d \r\n", p_obj_dp->type);
        return;
    }
    IPC_APP_set_ipc_usage_mode(p_obj_dp->value.dp_enum);
    UINT_T mode = IPC_APP_get_ipc_usage_mode();
    CHAR_T sMode[2];
    snprintf(sMode,2,"%d",mode);
    respone_dp_enum(TUYA_DP_IPC_USAGE_MODE,sMode);	
}
#endif

#ifdef TUYA_DP_ANTI_FLICKER
STATIC VOID handle_DP_ANTI_FLICKER(IN TY_OBJ_DP_S *p_obj_dp)
{
    if( (p_obj_dp == NULL) || (p_obj_dp->type != PROP_ENUM) )
    {
        AppErr("Error!! type invalid %d \r\n", p_obj_dp->type);
        return;
    }
    CHAR_T tmp_str[2] = {0};
    tmp_str[0] = '0' + p_obj_dp->value.dp_enum;

    IPC_APP_set_anti_flicker(tmp_str);
    CHAR_T *p_sim_mode = IPC_APP_get_anti_flicker();

    respone_dp_enum(TUYA_DP_ANTI_FLICKER, p_sim_mode);
}
#endif


#ifdef TUYA_DP_VOICE_ONOFF
STATIC VOID handle_DP_VOICE_ONOFF(IN TY_OBJ_DP_S *p_obj_dp)
{
    BOOL_T high_alarm = check_dp_bool_invalid(p_obj_dp);

    IPC_APP_set_voice_onoff(high_alarm);
    high_alarm = IPC_APP_get_voice_onoff();

    respone_dp_bool(TUYA_DP_VOICE_ONOFF, high_alarm);
}
#endif


#ifdef TUYA_DP_VOICE_SENSITIVITY
STATIC VOID handle_DP_VOICE_SEN(IN TY_OBJ_DP_S *p_obj_dp)
{
    if( (p_obj_dp == NULL) || (p_obj_dp->type != PROP_ENUM) )
    {
        AppErr("Error!! type invalid %d \r\n", p_obj_dp->type);
        return;
    }
	CHAR_T tmp_str[2] = {0};
    tmp_str[0] = '0' + p_obj_dp->value.dp_enum;
    IPC_APP_set_voice_sensitivity(tmp_str);
    CHAR_T sMode[4];
    snprintf(sMode,4,"%d",IPC_APP_get_voice_sensitivity());
    respone_dp_enum(TUYA_DP_VOICE_SENSITIVITY,sMode);
}
#endif

#ifdef TUYA_DP_SIREN_LINKAGE
STATIC VOID handle_DP_SIREN_LINKAGE(IN TY_OBJ_DP_S *p_obj_dp)
{
    BOOL_T siren_on_off = check_dp_bool_invalid(p_obj_dp);

    IPC_APP_set_siren_onoff(siren_on_off);
    siren_on_off = IPC_APP_get_siren_onoff();

    respone_dp_bool(TUYA_DP_SIREN_LINKAGE, siren_on_off);
}
#endif

#ifdef TUYA_DP_SIREN_SOUND
STATIC VOID handle_DP_SIREN_SOUND(IN TY_OBJ_DP_S *p_obj_dp)
{
	if( (p_obj_dp == NULL) || (p_obj_dp->type != PROP_ENUM) )
    {
        AppErr("Error!! type invalid %d \r\n", p_obj_dp->type);
        return;
    }
	CHAR_T tmp_str[2] = {0};
	tmp_str[0] = '0' + p_obj_dp->value.dp_enum;
    IPC_APP_set_Siren_Sound(tmp_str);
    CHAR_T sMode[4] = {0};
	snprintf(sMode,4,"%d",IPC_APP_get_Siren_Sound());
    respone_dp_enum(TUYA_DP_SIREN_SOUND, sMode);
}
#endif

#ifdef TUYA_DP_SIREN_DURATION
STATIC VOID handle_DP_SIREN_DURATION(IN TY_OBJ_DP_S *p_obj_dp)
{
	if( (p_obj_dp == NULL) || (p_obj_dp->type != PROP_VALUE) )
    {
        AppErr("Error!! type invalid %d \r\n", p_obj_dp->type);
        return;
    }
  	INT_T Duration = p_obj_dp->value.dp_value;
	IPC_APP_set_Siren_Duration(Duration);
	Duration = IPC_APP_get_Siren_Duration();
    respone_dp_value(TUYA_DP_SIREN_DURATION, Duration);
}
#endif

#ifdef TUYA_DP_SIREN_VOLUME
STATIC VOID handle_DP_SIREN_VOLUME(IN TY_OBJ_DP_S *p_obj_dp)
{
	if( (p_obj_dp == NULL) || (p_obj_dp->type != PROP_VALUE) )
    {
        AppErr("Error!! type invalid %d \r\n", p_obj_dp->type);
        return;
    }
  	INT_T volume = p_obj_dp->value.dp_value;
	IPC_APP_set_Siren_Volume(volume);
	volume = IPC_APP_get_Siren_Volume();
    respone_dp_value(TUYA_DP_SIREN_VOLUME, volume);
}
#endif

#ifdef TUYA_DP_IPC_VOLUME
STATIC VOID handle_DP_IPC_VOLUME(IN TY_OBJ_DP_S *p_obj_dp)
{
	if( (p_obj_dp == NULL) || (p_obj_dp->type != PROP_VALUE) )
    {
        AppErr("Error!! type invalid %d \r\n", p_obj_dp->type);
        return;
    }
  	INT_T volume = p_obj_dp->value.dp_value;
	IPC_APP_set_Ipc_Volume(volume);
	volume = IPC_APP_get_Ipc_Volume();
    respone_dp_value(TUYA_DP_IPC_VOLUME, volume);
}
#endif

#ifdef TUYA_DP_MEMORY_POINT
STATIC VOID handle_DP_PRESET_POINT(IN TY_OBJ_DP_S *p_obj_dp)
{
    if( (p_obj_dp == NULL) || (p_obj_dp->type != PROP_STR) )
    {
        AppErr("Error!! type invalid %d \r\n", p_obj_dp->type);
        return;
    }

    CHAR_T *p_preset = p_obj_dp->value.dp_str;

    IPC_APP_set_PRESET(p_preset);
   // p_preset = IPC_APP_get_PRESET();

   // respone_dp_str(TUYA_DP_MEMORY_POINT, p_preset);
}
#endif

#ifdef TUYA_DP_POINT_ACTION
STATIC VOID handle_DP_PRESET_ACTION(IN TY_OBJ_DP_S *p_obj_dp)
{
    if( (p_obj_dp == NULL) || (p_obj_dp->type != PROP_ENUM) )
    {
        AppErr("Error!! type invalid %d \r\n", p_obj_dp->type);
        return;
    }
    CHAR_T tmp_str[2] = {0};
    tmp_str[0] = '0' + p_obj_dp->value.dp_enum;

    IPC_APP_set_PRESET_ACTION(tmp_str);
}
#endif

#ifdef TUYA_DP_RESET_IPC
STATIC VOID handle_DP_RESET(IN TY_OBJ_DP_S *p_obj_dp)
{
    BOOL_T reset_ipc = check_dp_bool_invalid(p_obj_dp);

	respone_dp_bool(TUYA_DP_RESET_IPC, reset_ipc);
	
    IPC_APP_set_reset_ipc(reset_ipc);
}
#endif

#ifdef TUYA_DP_ONVIF_SWITCH
STATIC VOID handle_DP_ONVIF_SWITCH(IN TY_OBJ_DP_S *p_obj_dp)
{
    BOOL_T bOnvifSwitch = check_dp_bool_invalid(p_obj_dp);
	BOOL_T bPwdChanged = FALSE;

    IPC_APP_set_onvif_switch(bOnvifSwitch);
    bOnvifSwitch = IPC_APP_get_onvif_switch(&bPwdChanged);

    respone_dp_bool(TUYA_DP_ONVIF_SWITCH, bOnvifSwitch);

	if( (TRUE == bOnvifSwitch) && (FALSE == bPwdChanged) )
	{
		IPC_APP_REPORT_ONVIF_PW_CHANGED(FALSE);
	}
}
#endif

#ifdef TUYA_DP_ONVIF_CHANGE_PWD
STATIC VOID handle_DP_ONVIF_CHANGE_PWD(IN TY_OBJ_DP_S *p_obj_dp)
{
    if( (p_obj_dp == NULL) || (p_obj_dp->type != PROP_STR) )
    {
        AppErr("Error!! type invalid %d \r\n", p_obj_dp->type);
        return;
    }

    CHAR_T *p_pwd = p_obj_dp->value.dp_str;
    INT_T ret = IPC_APP_onvif_change_pwd(p_pwd);
	//printf("change onvif pwd. ret : %d\n", ret);
	
    CHAR_T str[64] = {0};
	if( 0 == ret )
	{
		snprintf(str, 64, "{\"res\":\"ok\"}");
	}
	else
	{
		snprintf(str, 64, "{\"res\":\"failed\", \"errcode\":%d}", abs(ret));
	}
    respone_dp_str(TUYA_DP_ONVIF_CHANGE_PWD, str);

	if( 0 == ret )
		IPC_APP_REPORT_ONVIF_PW_CHANGED(TRUE);
}
#endif
STATIC VOID handle_DP_RESERVED(IN TY_OBJ_DP_S *p_obj_dp)
{
    AppErr("error! not implememt yet.\r\n");
}


typedef VOID (*TUYA_DP_HANDLER)(IN TY_OBJ_DP_S *p_obj_dp);
typedef struct
{
    BYTE_T dp_id;
    TUYA_DP_HANDLER handler;
}TUYA_DP_INFO_S;

STATIC TUYA_DP_INFO_S s_dp_table[] =
{
#ifdef TUYA_DP_SLEEP_MODE
    {TUYA_DP_SLEEP_MODE,            handle_DP_SLEEP_MODE},
#endif
#ifdef TUYA_DP_LIGHT
    {TUYA_DP_LIGHT,                 handle_DP_LIGHT},
#endif
#ifdef TUYA_DP_AI_FACE_DET
    {TUYA_DP_AI_FACE_DET,           handle_DP_AI_FACE_DET},
#endif
#ifdef TUYA_DP_PIR_SWITCH1
    {TUYA_DP_PIR_SWITCH1,           handle_DP_PIR_SWITCH1},
#endif

#ifdef TUYA_DP_PIR_SWITCH2
    {TUYA_DP_PIR_SWITCH2,           handle_DP_PIR_SWITCH2},
#endif

#ifdef TUYA_DP_PIR_SWITCH3
    {TUYA_DP_PIR_SWITCH3,           handle_DP_PIR_SWITCH3},
#endif

#ifdef TUYA_DP_PIR_SWITCH_SENS
    {TUYA_DP_PIR_SWITCH_SENS,      handle_DP_PIR_SWITCH_SENS},
#endif

#ifdef TUYA_DP_PIR_SWITCH_TIME
    {TUYA_DP_PIR_SWITCH_TIME,      handle_DP_PIR_SWITCH_TIME},
#endif

#ifdef TUYA_DP_SET_LUX_LEVEL
    {TUYA_DP_SET_LUX_LEVEL,        handle_DP_LUX_LEVEL},
#endif

#ifdef TUYA_DP_FLIP
    {TUYA_DP_FLIP,                  handle_DP_FLIP},
#endif
#ifdef TUYA_DP_WATERMARK
    {TUYA_DP_WATERMARK,             handle_DP_WATERMARK},
#endif
#ifdef TUYA_DP_WDR
    {TUYA_DP_WDR,                   handle_DP_WDR},
#endif
#ifdef TUYA_DP_NIGHT_MODE
    {TUYA_DP_NIGHT_MODE,            handle_DP_NIGHT_MODE},
#endif
#ifdef TUYA_DP_NIGHT_MODE_ANO
    {TUYA_DP_NIGHT_MODE_ANO,            handle_DP_NIGHT_MODE_ANO},
#endif
#ifdef TUYA_DP_CALIBRATION
    {TUYA_DP_CALIBRATION,           handle_DP_CALIBRATION},
#endif
#ifdef TUYA_DP_ALARM_FUNCTION
    {TUYA_DP_ALARM_FUNCTION,        handle_DP_ALARM_FUNCTION},
#endif
#ifdef TUYA_DP_MOTION_TIMER_SWITCH
	{TUYA_DP_MOTION_TIMER_SWITCH,	handle_DP_MOTION_TIMER_SWITCH},
#endif
#ifdef TUYA_DP_MOTION_DETECT_TIMER
	{TUYA_DP_MOTION_DETECT_TIMER,	handle_DP_MOTION_DETECT_TIMER},
#endif
#ifdef TUYA_DP_ALARM_SENSITIVITY
    {TUYA_DP_ALARM_SENSITIVITY,     handle_DP_ALARM_SENSITIVITY},
#endif
//#ifdef TUYA_DP_ALARM_INTERVAL
//    {TUYA_DP_ALARM_INTERVAL,        handle_DP_ALARM_INTERVAL},
//#endif
#ifdef TUYA_DP_ALARM_ZONE_ENABLE
    {TUYA_DP_ALARM_ZONE_ENABLE,     handle_DP_ALARM_ZONE_ENABLE},
#endif

#ifdef TUYA_DP_ALARM_ZONE_DRAW
    {TUYA_DP_ALARM_ZONE_DRAW,     handle_DP_ALARM_ZONE_DRAW},
#endif

#ifdef TUYA_DP_ALARM_ZONE_DRAW_EX
    {TUYA_DP_ALARM_ZONE_DRAW_EX,     handle_DP_ALARM_ZONE_DRAW_EX},
#endif

#ifdef TUYA_DP_SD_STATUS_ONLY_GET
    {TUYA_DP_SD_STATUS_ONLY_GET,    handle_DP_SD_STATUS_ONLY_GET},
#endif
#ifdef TUYA_DP_SD_STORAGE_ONLY_GET
    {TUYA_DP_SD_STORAGE_ONLY_GET,   handle_DP_SD_STORAGE_ONLY_GET},
#endif
#ifdef TUYA_DP_SD_RECORD_ENABLE
    {TUYA_DP_SD_RECORD_ENABLE,      handle_DP_SD_RECORD_ENABLE},
#endif
#ifdef TUYA_DP_SD_RECORD_MODE
    {TUYA_DP_SD_RECORD_MODE,        handle_DP_SD_RECORD_MODE},
#endif
#ifdef TUYA_DP_SD_UMOUNT
    {TUYA_DP_SD_UMOUNT,             handle_DP_SD_UMOUNT},
#endif
#ifdef TUYA_DP_SD_FORMAT
    {TUYA_DP_SD_FORMAT,             handle_DP_SD_FORMAT},
#endif
#ifdef TUYA_DP_SD_FORMAT_STATUS_ONLY_GET
    {TUYA_DP_SD_FORMAT_STATUS_ONLY_GET, handle_DP_SD_FORMAT_STATUS_ONLY_GET},
#endif
#ifdef TUYA_DP_PTZ_CONTROL
    {TUYA_DP_PTZ_CONTROL,           handle_DP_PTZ_CONTROL},
#endif
#ifdef TUYA_DP_PTZ_STOP
    {TUYA_DP_PTZ_STOP,              handle_DP_PTZ_STOP},
#endif
#ifdef TUYA_DP_PTZ_CHECK
    {TUYA_DP_PTZ_CHECK,              handle_DP_PTZ_CHECK},
#endif
#ifdef TUYA_DP_EPTZ_CONTROL
    {TUYA_DP_EPTZ_CONTROL,           handle_DP_EPTZ_CONTROL},
#endif
#ifdef TUYA_DP_EPTZ_STOP
    {TUYA_DP_EPTZ_STOP,              handle_DP_EPTZ_STOP},
#endif
#ifdef TUYA_DP_TRACK_ENABLE
    {TUYA_DP_TRACK_ENABLE,           handle_DP_TRACK_ENABLE},
#endif
#ifdef TUYA_DP_HUM_FILTER
    {TUYA_DP_HUM_FILTER,             handle_DP_HUM_FILTER},
#endif
#ifdef TUYA_DP_PATROL_MODE
    {TUYA_DP_PATROL_MODE,            handle_DP_patrol_mode},
#endif
#ifdef TUYA_DP_PATROL_SWITCH
    {TUYA_DP_PATROL_SWITCH,          handle_DP_patrol_switch},
#endif
#ifdef TUYA_DP_PATROL_TMODE
    {TUYA_DP_PATROL_TMODE,           handle_DP_patrol_tmode},
#endif
#ifdef TUYA_DP_PATROL_TIME
    {TUYA_DP_PATROL_TIME,           handle_DP_patrol_time},
#endif
#ifdef TUYA_DP_RESET_IPC
    {TUYA_DP_RESET_IPC,             handle_DP_RESET},
#endif

#ifdef TUYA_DP_PATROL_STATE
    {TUYA_DP_PATROL_STATE,           handle_DP_patrol_state},
#endif

#ifdef TUYA_DP_PRESET_SET
    {TUYA_DP_PRESET_SET,              handle_DP_SET_PRESET},
#endif

#ifdef TUYA_DP_LINK_MOVE_ACTION
    {TUYA_DP_LINK_MOVE_ACTION,    handle_DP_LINK_MOVE},
#endif
#ifdef TUYA_DP_LINK_MOVE_SET
    {TUYA_DP_LINK_MOVE_SET,    handle_DP_LINK_MOVE_SET},
#endif

#ifdef TUYA_DP_DOOR_BELL
    {TUYA_DP_DOOR_BELL,             handle_DP_DOOR_BELL},
#endif
#ifdef TUYA_DP_BLUB_SWITCH
    {TUYA_DP_BLUB_SWITCH,           handle_DP_BLUB_SWITCH},
#endif
#ifdef TUYA_DP_SOUND_DETECT
    {TUYA_DP_SOUND_DETECT,          handle_DP_RESERVED},
#endif
#ifdef TUYA_DP_SOUND_SENSITIVITY
    {TUYA_DP_SOUND_SENSITIVITY,     handle_DP_RESERVED},
#endif
#ifdef TUYA_DP_SOUND_ALARM
    {TUYA_DP_SOUND_ALARM,           handle_DP_RESERVED},
#endif
#ifdef TUYA_DP_TEMPERATURE
    {TUYA_DP_TEMPERATURE,           handle_DP_RESERVED},
#endif
#ifdef TUYA_DP_HUMIDITY
    {TUYA_DP_HUMIDITY,              handle_DP_RESERVED},
#endif
#ifdef TUYA_DP_ELECTRICITY
    {TUYA_DP_ELECTRICITY,           handle_DP_ELECTRICITY},
#endif
#ifdef TUYA_DP_POWERMODE
    {TUYA_DP_POWERMODE,             handle_DP_POWERMODE},
#endif
#ifdef TUYA_DP_LOWELECTRIC
    {TUYA_DP_LOWELECTRIC,           handle_DP_LOWELECTRIC},
#endif
#ifdef TUYA_DP_MOTION_TRACK
    {TUYA_DP_MOTION_TRACK,          handle_DP_MOTION_TRACK},
#endif
#ifdef TUYA_DP_MOTION_INTERVAL
     {TUYA_DP_MOTION_INTERVAL,      handle_DP_MOTION_INTERVAL},
#endif
#ifdef TUYA_DP_ANBAO_LIGHT
    {TUYA_DP_ANBAO_LIGHT,           handle_DP_ANBAO_LIGHT},
#endif
#ifdef TUYA_DP_FLOODLIGHT_LIGHTNESS
    {TUYA_DP_FLOODLIGHT_LIGHTNESS,           handle_DP_FLOODLIGHT_LIGHTNESS},
#endif
#ifdef TUYA_DP_FLOODLIGHT_YELLOW
    {TUYA_DP_FLOODLIGHT_YELLOW,           handle_DP_FLOODLIGHT_YELLOW},
#endif
#ifdef TUYA_DP_FLIGHT_WARN_SWITCH
    {TUYA_DP_FLIGHT_WARN_SWITCH,     handle_DP_FLIGHT_WARN_SWITCH},
#endif
#ifdef TUYA_DP_FLIGHT_WARN_TIME
     {TUYA_DP_FLIGHT_WARN_TIME,       handle_DP_PIR_ALARM_TIME},
#endif
#ifdef TUYA_DP_MUTE_RECORD
    {TUYA_DP_MUTE_RECORD,           handle_DP_MUTE_RECORD},
#endif
#ifdef TUYA_DP_PIR_SWITCH
     {TUYA_DP_PIR_SWITCH,       handle_DP_PIR_SWITCH},
#endif
#ifdef TUYA_DP_SIREN_SWITCH
    {TUYA_DP_SIREN_SWITCH,           handle_DP_SIREN_SWITCH},
#endif
#ifdef TUYA_DP_IPC_USAGE_MODE
     {TUYA_DP_IPC_USAGE_MODE,       handle_DP_IPC_USAGE_MODE},
#endif
#ifdef TUYA_DP_VOICE_ONOFF
	{TUYA_DP_VOICE_ONOFF,			handle_DP_VOICE_ONOFF},
#endif
#ifdef TUYA_DP_ANTI_FLICKER
    {TUYA_DP_ANTI_FLICKER,            handle_DP_ANTI_FLICKER},
#endif
#ifdef TUYA_DP_VOICE_SENSITIVITY
	{TUYA_DP_VOICE_SENSITIVITY,			handle_DP_VOICE_SEN},
#endif

#ifdef TUYA_DP_SIREN_LINKAGE
    {TUYA_DP_SIREN_LINKAGE,      handle_DP_SIREN_LINKAGE},
#endif

#ifdef TUYA_DP_SIREN_SOUND
    {TUYA_DP_SIREN_SOUND,      handle_DP_SIREN_SOUND},
#endif

#ifdef TUYA_DP_SIREN_DURATION
    {TUYA_DP_SIREN_DURATION,      handle_DP_SIREN_DURATION},
#endif

#ifdef TUYA_DP_SIREN_VOLUME
    {TUYA_DP_SIREN_VOLUME,      handle_DP_SIREN_VOLUME},
#endif

#ifdef TUYA_DP_IPC_VOLUME
    {TUYA_DP_IPC_VOLUME,      handle_DP_IPC_VOLUME},
#endif

#ifdef TUYA_DP_MEMORY_POINT
	{TUYA_DP_MEMORY_POINT,	handle_DP_PRESET_POINT},
#endif
#ifdef TUYA_DP_POINT_ACTION
	{TUYA_DP_POINT_ACTION,	handle_DP_PRESET_ACTION},
#endif

#ifdef TUYA_DP_ONVIF_SWITCH
    {TUYA_DP_ONVIF_SWITCH, 		handle_DP_ONVIF_SWITCH},
#endif
#ifdef TUYA_DP_ONVIF_CHANGE_PWD
    {TUYA_DP_ONVIF_CHANGE_PWD, 		handle_DP_ONVIF_CHANGE_PWD},
#endif
};

OPERATE_RET IPC_APP_handle_dp_cmd_objs(IN CONST TY_RECV_OBJ_DP_S *dp_rev)
{
    TY_OBJ_DP_S *dp_data = (TY_OBJ_DP_S *)(dp_rev->dps);
    UINT_T cnt = dp_rev->dps_cnt;
    INT_T table_idx = 0;
    INT_T table_count = ( sizeof(s_dp_table) / sizeof(s_dp_table[0]) );
    INT_T index = 0;
    printf("dp_data->dpid=%d\n",dp_data->dpid);
    for(index = 0; index < cnt; index++)
    {
        TY_OBJ_DP_S *p_dp_obj = dp_data + index;

        for(table_idx = 0; table_idx < table_count; table_idx++)
        {
            if(s_dp_table[table_idx].dp_id == p_dp_obj->dpid)
            {
                s_dp_table[table_idx].handler(p_dp_obj);
                break;
            }
        }
    }
    return 0;
}
OPERATE_RET IPC_APP_handle_raw_dp_cmd_objs(IN CONST TY_RECV_RAW_DP_S *dp)
{
    printf("*************>p_dp_obj->dpid=%d\n",dp->dpid);
    printf("*************>p_dp_obj->data=%s\n",dp->data);
    // INT_T table_idx = 0;
    // INT_T table_count = ( sizeof(s_dp_table) / sizeof(s_dp_table[0]) );
    // for(table_idx = 0; table_idx < table_count; table_idx++)
    // {
    //     if(s_dp_table[table_idx].dp_id == dp->dpid)
    //     {
    //         s_dp_table[table_idx].handler(dp->data);
    //         break;
    //     }
    // }
    IPC_APP_set_alarm_zone_draw_ex((cJSON *)(dp->data));
    char *text = IPC_APP_get_alarm_zone_draw_ex();
    int size = strlen(text)+1;

    printf("*************text=%s\n",text);
    printf("*************size=%d\n",size);

    dev_report_dp_raw_sync(NULL,TUYA_DP_ALARM_ZONE_DRAW_EX,(unsigned char *)text,size,1);
    //tuya_ipc_dp_report
    //respone_dp_str(TUYA_DP_ALARM_ZONE_DRAW_EX, IPC_APP_get_alarm_zone_draw_ex());
    return 0;
}
OPERATE_RET IPC_APP_handle_dp_query_objs(TY_DP_QUERY_S *dp_query)
{
    INT_T table_idx = 0;
    INT_T table_count = ( sizeof(s_dp_table) / sizeof(s_dp_table[0]) );
    INT_T index = 0;
    for(index = 0; index < dp_query->cnt; index++)
    {
        for(table_idx = 0; table_idx < table_count; table_idx++)
        {
            if(s_dp_table[table_idx].dp_id == dp_query->dpid[index])
            {
                s_dp_table[table_idx].handler(NULL);
                break;
            }
        }
    }
    return 0;
}

/*  以下demo接口已废弃。发送移动告警和门铃消息，请参考tuya_ipc_api.h中的tuya_ipc_notify_motion_detect和tuya_ipc_notify_door_bell_press

OPERATE_RET IPC_APP_Send_Motion_Alarm_From_Buffer(CHAR_T *data, UINT_T size, NOTIFICATION_CONTENT_TYPE_E type)
{
    OPERATE_RET ret = OPRT_OK;
    INT_T try_count = 3;
    INT_T count = 1;
    VOID *message = NULL;
    INT_T message_size = 0;
#ifdef TUYA_DP_ALARM_FUNCTION
    if(IPC_APP_get_alarm_function_onoff() != TRUE)
    {
        printf("motion alarm upload not enabled.skip \r\n");
        return OPRT_COM_ERROR;
    }
#endif

    printf("Send Motion Alarm. size:%d type:%d\r\n", size, type);
    message_size = tuya_ipc_notification_message_malloc(count, &message);
    if((message_size == 0)||(message == NULL))
    {
        printf("tuya_ipc_notification_message_malloc failed\n");
        return OPRT_COM_ERROR;
    }

    memset(message, 0, message_size);
    while (try_count != 0)
    {
        ret = tuya_ipc_notification_content_upload_from_buffer(type,data,size,message);
        if(ret != OPRT_OK)
        {
            try_count --;
            continue;
        }
        break;
    }
    if(ret == OPRT_OK)
    {
        ret = tuya_ipc_notification_message_upload(TUYA_DP_MOTION_DETECTION_ALARM, message, 5);
    }

    tuya_ipc_notification_message_free(message);

    return ret;
}

OPERATE_RET IPC_APP_Send_Motion_Alarm(CHAR_T *p_abs_file, NOTIFICATION_CONTENT_TYPE_E file_type)
{
#ifdef TUYA_DP_ALARM_FUNCTION
    if(IPC_APP_get_alarm_function_onoff() != TRUE)
    {
        printf("motion alarm upload not enabled.skip \r\n");
        return OPRT_COM_ERROR;
    }
#endif

    OPERATE_RET ret = OPRT_OK;
    INT_T try_count = 3;
    INT_T count = 1;
    VOID *message = NULL;
    INT_T size = 0;

    printf("Send Motion Alarm. type:%d File:%s\r\n", file_type, p_abs_file);

    size = tuya_ipc_notification_message_malloc(count, &message);
    if((size == 0)||(message == NULL))
    {
        printf("tuya_ipc_notification_message_malloc failed\n");
        return OPRT_COM_ERROR;
    }

    memset(message, 0, size);
    while (try_count != 0)
    {
        ret = tuya_ipc_notification_content_upload_from_file(p_abs_file, file_type, message);
        if(ret != OPRT_OK)
        {
            try_count --;
            continue;
        }
        break;
    }
    if(ret == OPRT_OK)
    {
        ret = tuya_ipc_notification_message_upload(TUYA_DP_MOTION_DETECTION_ALARM, message, 5);
    }

    tuya_ipc_notification_message_free(message);

    return ret;
}

OPERATE_RET IPC_APP_Send_DoorBell_Snap(CHAR_T *p_snap_file, NOTIFICATION_CONTENT_TYPE_E file_type)
{
    OPERATE_RET ret = OPRT_OK;
    INT_T try_count = 3;
    INT_T count = 1;
    VOID *message = NULL;
    INT_T size = 0;

    printf("Send DoorBell Snap. type:%d File:%s\r\n", file_type, p_snap_file);
    size = tuya_ipc_notification_message_malloc(count, &message);
    if((size == 0)||(message == NULL))
    {
        printf("tuya_ipc_notification_message_malloc failed\n");
        return OPRT_COM_ERROR;
    }

    memset(message, 0, size);
    while (try_count != 0)
    {
        ret = tuya_ipc_notification_content_upload_from_file(p_snap_file, file_type, message);
        if(ret != OPRT_OK)
        {
            try_count --;
            continue;
        }
        break;
    }
    if(ret == OPRT_OK)
    {
        ret = tuya_ipc_snapshot_message_upload(TUYA_DP_DOOR_BELL_SNAP, message, 5);
    }

    tuya_ipc_notification_message_free(message);

    return ret;
}

*/

VOID IPC_APP_REPORT_ONVIF_PW_CHANGED(IN BOOL_T bChanged)
{
#ifdef TUYA_DP_ONVIF_PW_CHANGED
    respone_dp_bool(TUYA_DP_ONVIF_PW_CHANGED, bChanged);
#endif
}

VOID IPC_APP_REPORT_ONVIF_IP_ADDR(IN CHAR_T *pIpAddr)
{
#ifdef TUYA_DP_ONVIF_IP_ADDR
    respone_dp_str(TUYA_DP_ONVIF_IP_ADDR, pIpAddr);
#endif
}