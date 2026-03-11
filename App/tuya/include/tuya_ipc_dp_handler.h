/*********************************************************************************
  *Copyright(C),2015-2020, 涂鸦科技 www.tuya.comm
  *FileName: tuya_ipc_dp_handler.h
  *
  * 文件描述：
  * 1. DP点设置和获取函数API定义，具体内容请参考.c文件中的代码注释
**********************************************************************************/

#ifndef _TUYA_IPC_DP_HANDLER_H
#define _TUYA_IPC_DP_HANDLER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "sdk_inc/tuya_cloud_types.h"
#include "tuya_ipc_dp_utils.h"
#include "sdk_inc/tuya_ipc_ptz.h"

#ifdef TUYA_DP_SLEEP_MODE
VOID IPC_APP_set_sleep_mode(BOOL_T sleep_mode);
BOOL_T IPC_APP_get_sleep_mode(VOID);
#endif

#ifdef TUYA_DP_LIGHT
VOID IPC_APP_set_light_onoff(BOOL_T light_on_off);
BOOL_T IPC_APP_get_light_onoff(VOID);
#endif

#ifdef TUYA_DP_FLIP
VOID IPC_APP_set_flip_onoff(BOOL_T flip_on_off);
BOOL_T IPC_APP_get_flip_onoff(VOID);
#endif

#ifdef TUYA_DP_WATERMARK
VOID IPC_APP_set_watermark_onoff(BOOL_T watermark_on_off);
BOOL_T IPC_APP_get_watermark_onoff(VOID);
#endif

#ifdef TUYA_DP_WDR
VOID IPC_APP_set_wdr_onoff(BOOL_T wdr_on_off);
BOOL_T IPC_APP_get_wdr_onoff(VOID);
#endif

#ifdef TUYA_DP_NIGHT_MODE
VOID IPC_APP_set_night_mode(CHAR_T *p_night_mode);
CHAR_T *IPC_APP_get_night_mode(VOID);
#endif

#ifdef TUYA_DP_NIGHT_MODE_ANO
VOID IPC_APP_set_night_mode_Ano(CHAR_T *p_night_mode);
CHAR_T *IPC_APP_get_night_mode_Ano(VOID);
#endif

#ifdef TUYA_DP_CALIBRATION
VOID IPC_APP_start_calibration(VOID);
#endif

#ifdef TUYA_DP_ALARM_FUNCTION
VOID IPC_APP_set_alarm_function_onoff(BOOL_T alarm_on_off);
BOOL_T IPC_APP_get_alarm_function_onoff(VOID);
#endif

#ifdef TUYA_DP_MOTION_TIMER_SWITCH
VOID IPC_APP_set_motion_timer_switch(BOOL_T switch_on_off);
BOOL_T IPC_APP_get_motion_timer_switch(VOID);
#endif

#ifdef TUYA_DP_MOTION_DETECT_TIMER
VOID IPC_APP_set_motion_detect_timer(CHAR_T *p_timer);
CHAR_T *IPC_APP_get_motion_detect_timer(VOID);
#endif

#ifdef TUYA_DP_ALARM_SENSITIVITY
VOID IPC_APP_set_alarm_sensitivity(CHAR_T *p_sensitivity);
CHAR_T *IPC_APP_get_alarm_sensitivity(VOID);
#endif

#ifdef TUYA_DP_ALARM_ZONE_DRAW
VOID IPC_APP_set_alarm_zone_draw(cJSON * p_alarm_zone);
char * IPC_APP_get_alarm_zone_draw(VOID);
#endif

#ifdef TUYA_DP_ALARM_ZONE_DRAW_EX
VOID IPC_APP_set_alarm_zone_draw_ex(cJSON * p_alarm_zone);
char * IPC_APP_get_alarm_zone_draw_ex(VOID);
#endif

#ifdef TUYA_DP_ALARM_ZONE_ENABLE
VOID IPC_APP_set_alarm_zone_onoff(BOOL_T alarm_zone_on_off);
BOOL_T IPC_APP_get_alarm_zone_onoff(VOID);
#endif

//#ifdef TUYA_DP_ALARM_INTERVAL
//VOID IPC_APP_set_alarm_interval(CHAR_T *p_interval);
//CHAR_T *IPC_APP_get_alarm_interval(VOID);
//#endif

#ifdef TUYA_DP_SD_STATUS_ONLY_GET
INT_T IPC_APP_get_sd_status(VOID);
#endif

#ifdef TUYA_DP_SD_STORAGE_ONLY_GET
VOID IPC_APP_get_sd_storage(UINT_T *p_total, UINT_T *p_used, UINT_T *p_empty);
#endif

#ifdef TUYA_DP_SD_RECORD_ENABLE
VOID IPC_APP_set_sd_record_onoff(BOOL_T sd_record_on_off);
BOOL_T IPC_APP_get_sd_record_onoff(VOID);
#endif

#ifdef TUYA_DP_SD_RECORD_MODE
VOID IPC_APP_set_sd_record_mode(UINT_T sd_record_mode);
UINT_T IPC_APP_get_sd_record_mode(VOID);
#endif

#ifdef TUYA_DP_SD_UMOUNT
BOOL_T IPC_APP_unmount_sd_card(VOID);
#endif

#ifdef TUYA_DP_SD_FORMAT
VOID IPC_APP_format_sd_card(VOID);
#endif

#ifdef TUYA_DP_SD_FORMAT_STATUS_ONLY_GET
INT_T IPC_APP_get_sd_format_status(VOID);
#endif

#ifdef TUYA_DP_PTZ_CONTROL
VOID IPC_APP_ptz_start_move(CHAR_T *p_direction);
#endif

#ifdef TUYA_DP_PTZ_STOP
VOID IPC_APP_ptz_stop_move(VOID);
#endif

#ifdef TUYA_DP_PTZ_CHECK
void IPC_APP_ptz_check(VOID);
#endif

#ifdef TUYA_DP_EPTZ_CONTROL
VOID IPC_APP_eptz_start_move(CHAR_T *p_direction);
#endif

#ifdef TUYA_DP_EPTZ_STOP
VOID IPC_APP_eptz_stop_move(VOID);
#endif

#ifdef TUYA_DP_TRACK_ENABLE
void IPC_APP_track_enable(BOOL_T track_enable);

BOOL_T IPC_APP_get_track_enable(void);

#endif

#ifdef TUYA_DP_LINK_MOVE_ACTION
VOID IPC_APP_set_link_move(INT_T bind_seq);
#endif

#ifdef TUYA_DP_LINK_MOVE_SET
VOID IPC_APP_set_link_pos(INT_T bind_seq);
#endif


#ifdef TUYA_DP_HUM_FILTER
void IPC_APP_set_human_filter(BOOL_T filter_enable);
BOOL_T IPC_APP_get_human_filter();
#endif

#ifdef TUYA_DP_PATROL_MODE
void IPC_APP_set_patrol_mode(BOOL_T patrol_mode);
char IPC_APP_get_patrol_mode(void);

#endif

#ifdef TUYA_DP_PATROL_SWITCH
void IPC_APP_set_patrol_switch(BOOL_T patrol_switch);

BOOL_T IPC_APP_get_patrol_switch(void);

void IPC_APP_ptz_preset_reset(S_PRESET_CFG *preset_cfg);

#endif

#ifdef TUYA_DP_PATROL_TMODE
void IPC_APP_set_patrol_tmode(BOOL_T patrol_tmode);

char IPC_APP_get_patrol_tmode(void);
#endif

#ifdef TUYA_DP_PATROL_TIME
void IPC_APP_set_patrol_time(cJSON * p_patrol_time);
#endif

#ifdef TUYA_DP_PRESET_SET
void IPC_APP_set_preset(cJSON * p_preset_param);

#endif

#ifdef TUYA_DP_PATROL_STATE
void IPC_APP_patrol_state(int *patrol_state);
#endif


#ifdef TUYA_DP_BLUB_SWITCH
VOID IPC_APP_set_blub_onoff(BOOL_T blub_on_off);
BOOL_T IPC_APP_get_blub_onoff(VOID);
#endif

#ifdef TUYA_DP_ELECTRICITY
INT_T IPC_APP_get_battery_percent(VOID);
#endif

#ifdef TUYA_DP_POWERMODE
CHAR_T *IPC_APP_get_power_mode(VOID);
#endif

#ifdef TUYA_DP_MOTION_TRACK
VOID IPC_APP_set_motion_track_onoff(BOOL_T motion_track_on_off);
BOOL_T IPC_APP_get_motion_track_onoff(VOID);
#endif

#ifdef TUYA_DP_MOTION_INTERVAL
VOID IPC_APP_set_motion_interval(UINT_T interval);
UINT_T IPC_APP_get_motion_interval(VOID);
#endif

/////安保灯 add by ale
#ifdef TUYA_DP_ANBAO_LIGHT
VOID IPC_APP_set_anbao_light_onoff(BOOL_T light_on_off);
BOOL_T IPC_APP_get_anbao_light_onoff(VOID);
#endif

#ifdef TUYA_DP_FLOODLIGHT_LIGHTNESS
VOID IPC_APP_set_floodlight_lightness(INT_T lightness);
INT_T IPC_APP_get_floodlight_lightness(VOID);
#endif

#ifdef TUYA_DP_FLOODLIGHT_YELLOW
VOID IPC_APP_set_floodlight_yellow(INT_T lightness);
INT_T IPC_APP_get_floodlight_yellow(VOID);
#endif

#ifdef TUYA_DP_FLIGHT_WARN_SWITCH
VOID IPC_APP_set_flight_warn_switch(BOOL_T pir_switch_onoff);
BOOL_T IPC_APP_get_flight_warn_switch(VOID);
#endif

#ifdef TUYA_DP_FLIGHT_WARN_TIME
VOID IPC_APP_set_flight_warn_time(UINT_T atime);
UINT_T IPC_APP_get_flight_warn_time(VOID);
#endif

#ifdef TUYA_DP_MUTE_RECORD
VOID IPC_APP_set_mute_record(BOOL_T mute_recordt_on_off);
BOOL_T IPC_APP_get_mute_record(VOID);
#endif
#ifdef TUYA_DP_PIR_SWITCH1
VOID IPC_APP_set_PIR1_onoff(BOOL_T pir_on_off);
BOOL_T IPC_APP_get_PIR1_onoff(VOID);
#endif

#ifdef TUYA_DP_PIR_SWITCH2
VOID IPC_APP_set_PIR2_onoff(BOOL_T pir_on_off);
BOOL_T IPC_APP_get_PIR2_onoff(VOID);
#endif

#ifdef TUYA_DP_PIR_SWITCH3
VOID IPC_APP_set_PIR3_onoff(BOOL_T pir_on_off);
BOOL_T IPC_APP_get_PIR3_onoff(VOID);
#endif

#ifdef TUYA_DP_PIR_SWITCH_SENS
VOID IPC_APP_set_pir_switch_sens(UINT_T mode);
UINT_T IPC_APP_get_pir_switch_sens(VOID);
#endif

#ifdef TUYA_DP_PIR_SWITCH_TIME
VOID IPC_APP_set_pir_switch_time(UINT_T atime);
UINT_T IPC_APP_get_pir_switch_time(VOID);
#endif

#ifdef TUYA_DP_SET_LUX_LEVEL
VOID IPC_APP_set_lux_level(UINT_T mode);
UINT_T IPC_APP_get_lux_level(VOID);
#endif

#ifdef TUYA_DP_PIR_SWITCH
VOID IPC_APP_set_pir_switch(UINT_T mode);
UINT_T IPC_APP_get_pir_switch(VOID);
#endif

#ifdef TUYA_DP_SIREN_SWITCH
VOID IPC_APP_set_siren(BOOL_T siren_switch);
BOOL_T IPC_APP_get_siren(VOID);
#endif

#ifdef TUYA_DP_IPC_USAGE_MODE
VOID IPC_APP_set_ipc_usage_mode(UINT_T mode);
UINT_T IPC_APP_get_ipc_usage_mode(VOID);
#endif

#ifdef TUYA_DP_VOICE_ONOFF
VOID IPC_APP_set_voice_onoff(BOOL_T alarm_onoff);
BOOL_T IPC_APP_get_voice_onoff(VOID);
#endif

#ifdef TUYA_DP_VOICE_SENSITIVITY
VOID IPC_APP_set_voice_sensitivity(CHAR_T *cf);
INT_T IPC_APP_get_voice_sensitivity(VOID);
#endif
#ifdef TUYA_DP_SIREN_LINKAGE
VOID IPC_APP_set_siren_onoff(BOOL_T siren_on_off);
BOOL_T IPC_APP_get_siren_onoff(VOID);
#endif

#ifdef TUYA_DP_SIREN_SOUND
VOID IPC_APP_set_Siren_Sound(CHAR_T *list);
INT_T IPC_APP_get_Siren_Sound(VOID);
#endif

#ifdef TUYA_DP_SIREN_DURATION
VOID IPC_APP_set_Siren_Duration(INT_T value);
INT_T IPC_APP_get_Siren_Duration(VOID);
#endif

#ifdef TUYA_DP_SIREN_VOLUME
VOID IPC_APP_set_Siren_Volume(INT_T value);
INT_T IPC_APP_get_Siren_Volume(VOID);
#endif

#ifdef TUYA_DP_IPC_VOLUME
VOID IPC_APP_set_Ipc_Volume(INT_T value);
INT_T IPC_APP_get_Ipc_Volume(VOID);
#endif

#ifdef TUYA_DP_MEMORY_POINT
VOID IPC_APP_set_PRESET(CHAR_T *p_preset);
CHAR_T *IPC_APP_get_cruise_sta(VOID);
void IPC_APP_set_PRESET_ACTION(CHAR_T *p_action);
VOID IPC_APP_get_preset(void);
#endif
#ifdef TUYA_DP_RESET_IPC
VOID IPC_APP_set_reset_ipc(BOOL_T reset_ipc);
#endif

#ifdef TUYA_DP_ONVIF_SWITCH
VOID IPC_APP_set_onvif_switch(BOOL_T onoff);
BOOL_T IPC_APP_get_onvif_switch(BOOL_T *pbPwdChanged);
#endif

#ifdef TUYA_DP_ONVIF_CHANGE_PWD
INT_T IPC_APP_onvif_change_pwd(CHAR_T * p_onvif_pwd);
#endif

#ifdef TUYA_DP_ANTI_FLICKER
VOID IPC_APP_set_anti_flicker(CHAR_T *p_anti_flicker);
CHAR_T *IPC_APP_get_anti_flicker(VOID);
#endif


#ifdef __cplusplus
}
#endif

#endif  /*_TUYA_IPC_DP_HANDLER_H*/
