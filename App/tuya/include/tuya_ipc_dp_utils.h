/*********************************************************************************
  *Copyright(C),2015-2020, 涂鸦科技 www.tuya.comm
  *FileName:    tuya_ipc_dp_utils.h
  *
  * 请勿随意修改该文件中的任何内容，如需修改请联系涂鸦产品经理！！
  * 文件描述：
  * 1. DP点定义
  * 2. DP点工具API定义
**********************************************************************************/

#ifndef _TUYA_IPC_DP_UTILS_H
#define _TUYA_IPC_DP_UTILS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "sdk_inc/tuya_cloud_types.h"
#include "sdk_inc/tuya_ipc_api.h"
#include "sdk_inc/tuya_cloud_com_defs.h"

#include "sdk_inc/cJSON.h"

/* APP基本功能设置页面 */
#define TUYA_DP_SLEEP_MODE                 105         /* 休眠,BOOL类型,true休眠,false不休眠 */
#define TUYA_DP_LIGHT                      101         /* 状态指示灯,BOOL类型,true打开,false关闭 */
#define TUYA_DP_FLIP                       103         /* 录制画面翻转,BOOL类型,true反转,false正常 */
#define TUYA_DP_WATERMARK                  104         /* 视频水印,BOOL类型,true打开水印,false关闭水印 */
//#define TUYA_DP_WDR                        107         /* 宽动态范围模式,BOOL类型,true则打开宽动态范围模式,false则关闭宽动态范围 */
#define TUYA_DP_NIGHT_MODE                 108         /* 红外夜视功能,ENUM类型,0-自动 1-关 2-开 */
#define TUYA_DP_NIGHT_MODE_ANO             124
#define TUYA_DP_CALIBRATION                132         /* 摄像机校准,BOOL类型 */
/* APP移动侦测报警页面 */
#define TUYA_DP_ALARM_FUNCTION             134         /* 移动侦测报警功能开关,BOOL类型,true打开,false关闭 */
//#define TUYA_DP_MOTION_TIMER_SWITCH        135         /* 定时移动侦测功能开关,BOOL类型,true为定时移动侦测,false为全天移动侦测 */
//#define TUYA_DP_MOTION_DETECT_TIMER        114         /* 定时移动侦测时间设置,STR类型,"开始时间|结束时间" */
#define TUYA_DP_ALARM_SENSITIVITY          106         /* 移动侦测报警灵敏度,ENUM类型,0为低灵敏度,1为中灵敏度,2为高灵敏度*/
/* APP储存卡设置页面 */
#define TUYA_DP_SD_STATUS_ONLY_GET         110         /* SD卡状态,VALUE类型,1-正常,2-异常,3-空间不足,4-正在格式化,5-无SD卡 */
#define TUYA_DP_SD_STORAGE_ONLY_GET        109         /* SD卡容量,STR类型,"总容量|当前使用量|剩余容量",单位：kb */
#define TUYA_DP_SD_RECORD_ENABLE           150         /* SD卡录像功能使能,BOOL类型,true打开,false关闭 */
#define TUYA_DP_SD_RECORD_MODE             151         /* SD卡录像模式选择,ENUM类型,0-事件录像 1-连续录像 */
#define TUYA_DP_SD_UMOUNT                  112         /* 退出存储卡,BOOL类型,true为存储卡已退出,false为存储卡未退出 */
#define TUYA_DP_SD_FORMAT                  111         /* 格式化存储卡,BOOL类型 */
#define TUYA_DP_SD_FORMAT_STATUS_ONLY_GET  117         /* 格式化存储卡状态,VALUE类型,-2000:SD卡正在格式化,-2001:SD卡格式化异常,-2002:无SD卡,-2003:SD卡错误.正数为格式化进度 */
/* APP摄像头控制页面 */
#define TUYA_DP_PTZ_CONTROL                119         /* 云台转动开始,ENUM类型,0-上,1-右上,2-右,3-右下,4-下,5-左下,6-左,7-左上 4.0sdk云台转动开始,ENUM类型,0-右上,1-右,2-右下,3-下,4-左下,5-左,6-左上,7-上 */
#define TUYA_DP_PTZ_STOP                   116         /* 云台转动停止,BOOL类型 */
#define TUYA_DP_PTZ_CHECK                  132       /*PTZ self-check switch, 0 means off, 1 means on, default value is off*/
//#define TUYA_DP_TRACK_ENABLE               161        /*Move tracking switch, BOOL type, true means on, false means closed*/
#define TUYA_DP_EPTZ_STOP                  164
#define TUYA_DP_EPTZ_CONTROL               163        //ENUM类型 机械变焦, 0 减焦距 ，1 加焦距
#define TUYA_DP_HUM_FILTER                 170        /*Human filtering，BOOL type，,true means open，false means closed*/
#define TUYA_DP_PATROL_SWITCH              174       /*patrol switch，0 means closed  ，1 means open*/
#define TUYA_DP_PATROL_MODE                175       /*patrol path mode，0：automatically patrol  ，1：patrol according to presets*/
#define TUYA_DP_AI_FACE_DET                186
#define TUYA_DP_PATROL_TMODE               176       /*patrol time mode，0：patrol whole day，1：patrol timing */
#define TUYA_DP_PATROL_TIME                177        /*patrol time setting*/
//#define TUYA_DP_PRESET_SET                 178         /*Preset point operation，string type，type:1 add，type:2 delete.Different types are different from data strings*/
//#define TUYA_DP_PATROL_STATE               179         /*Patrol status query command，0:automatically patrol 1：patrol according to presets 2：not patrolling*/

/*Motion detection area setting page*/
#define TUYA_DP_ALARM_ZONE_ENABLE          168        /* Detection area switch, BOOL type, true means on, false means closed*/
#define TUYA_DP_ALARM_ZONE_DRAW            169        /* Detection area setting, STR type，example:{"num":1, "region0":{"x":30,"y":40,"xlen":50,"ylen":60}}  */
#define TUYA_DP_ALARM_ZONE_DRAW_EX         238     /* Detection area setting, STR type，example:[{"ispoly":0,"points":[20,35,40,35,40,75,20,75]},{"ispoly":0,"points":[46,36,66,36,66,76,46,76]}]  */

/* special DP point in IPC */
#define TUYA_DP_DOOR_BELL                  136         /* Doorbell call, STR type, "current timestamp"*/
//#define TUYA_DP_BLUB_SWITCH                138         /* Light control switch, BOOL type, true menas open, false means closed*/
//#define TUYA_DP_SOUND_DETECT               139         /* Decibel detection switch,BOOL type,true means open,false means closed */
//#define TUYA_DP_SOUND_SENSITIVITY          140         /* Decibel detection sensitivity, ENUM type, 0 means low sensitivity, 1 means high sensitivity */
//#define TUYA_DP_SOUND_ALARM                141         /* Decibel alarm channel, STR type, "current timestamp" */
#define TUYA_DP_TEMPERATURE                142         /* Temperature measurement, VALUE type,[0-50] */
#define TUYA_DP_HUMIDITY                   143         /* Humidity measurement, VALUE type,[0-100] */
//#define TUYA_DP_ELECTRICITY                145         /* Battery power percentage, VALUE type[0-100] */
//#define TUYA_DP_POWERMODE                  146         /* Power supply mode, ENUM type, "0" is the battery power supply state, "1" is the plug-in power supply state (or battery charging state) */
#define TUYA_DP_LOWELECTRIC                147         /* Low battery alarm threshold, VALUE type */
#define TUYA_DP_DOOR_STATUS                149         /* Doorbell status notification, BOOL type, true means open, false means closed  */

#define TUYA_DP_MOTION_DETECTION_ALARM     115         /* 移动侦测消息报警 */
#define TUYA_DP_DOOR_BELL_SNAP             154         /* 门铃推送截图提示 */
#define TUYA_DP_ONVIF_SWITCH               241         /* Onvif开关,BOOL类型,true打开,false关闭 */
#define TUYA_DP_ONVIF_PW_CHANGED           242        /* 是否修改Onvif密码,只上报,BOOL类型,首次开启Onvif功能,上报0弹出修改密码界面,更改密码后上报1关闭界面 */
#define TUYA_DP_ONVIF_IP_ADDR              243         /* Onvif IP 上报,只上报,STR类型,如192.168.0.100 */
#define TUYA_DP_ONVIF_CHANGE_PWD           244         /* 修改Onvif密码,STR类型, */


/* APP人体追踪页面 */ 	///< add by shang 2018.06.01
#define TUYA_DP_MOTION_TRACK               161         /* 人体追踪功能开关,BOOL类型,true打开,false关闭 */

/*ptz联动dp点*/
//#define TUYA_DP_LINK_MOVE_ACTION          190         /* ptz联动dp点*/
#define TUYA_DP_LINK_MOVE_SET              199         /* ptz联动dp点*/

#define TUYA_DP_MOTION_INTERVAL            133         /* 移动侦测报警间隔 ENUM类型 1 3 5 单位分钟*/
#define TUYA_DP_ANBAO_LIGHT                138         /* 安保灯, 灯 BOOL类型,true打开,false关闭 */
#define TUYA_DP_FLOODLIGHT_LIGHTNESS       158         /* 安保灯, 灯光亮度 数值型,1-100 */
//#define TUYA_DP_FLOODLIGHT_YELLOW          231         /* 安保灯，黄灯亮度，数值型0-1000 */

#define TUYA_DP_FLIGHT_WARN_SWITCH         172         /* 告警联动灯设置 BOOL类型 true 打开，false关闭*/
#define TUYA_DP_FLIGHT_WARN_TIME           173         /* 告警联动亮灯持续时间 ENUM类型,5-600 sec*/
#define TUYA_DP_MUTE_RECORD                197         /* 静音录像功能开关,BOOL类型,true打开,false关闭 */

#define TUYA_DP_PIR_SWITCH                 152         /* PIR开关及灵敏度, ENUM类型 */
#define TUYA_DP_SIREN_SWITCH               159         /* 警笛开关, BOOL类型 */

#define TUYA_DP_IPC_USAGE_MODE             234         /* 推送配置, 枚举型 [0,1]*/

#define TUYA_DP_VOICE_ONOFF                139         /*声音侦测报警开关*/
#define TUYA_DP_VOICE_SENSITIVITY          140         /*声音侦测灵敏度*/

#define TUYA_DP_SIREN_LINKAGE              120         /*联动蜂鸣器*/
#define TUYA_DP_SIREN_SOUND                125         /*报警音*/
#define TUYA_DP_SIREN_DURATION             194         /*蜂鸣器时长*/
#define TUYA_DP_SIREN_VOLUME               195//160         /*蜂鸣器音量*/
#define TUYA_DP_IPC_VOLUME                 160         /*蜂鸣器音量*/
// #define TUYA_DP_MEMORY_POINT               178         /*收藏点*/
// #define TUYA_DP_POINT_ACTION               190         /*收藏点联动*/
// #define TUYA_DP_CRUISE_STA                 179         /*巡航状态*/
#define TUYA_DP_RESET_IPC                  162         /* 远程重启摄像机 */

#define TUYA_DP_ANTI_FLICKER               188         /*抗闪烁*/


#define TUYA_DP_PIR_SWITCH1                181         /*PIR1开关*/
#define TUYA_DP_PIR_SWITCH2                182         /*PIR2开关*/
#define TUYA_DP_PIR_SWITCH3                183         /*PIR3开关*/
#define TUYA_DP_PIR_SWITCH_SENS            184         /* PIR灵敏度, ENUM类型 */
#define TUYA_DP_PIR_SWITCH_TIME            233         /* PIR亮灯时长 单位分钟, ENUM类型 */

//#define TUYA_DP_SET_LUX_LEVEL              231         /*Set_lux_level Enum Value: 0, 1, 2 0=Low，1=Middle（默认），2=High

/* 上报本地所有DP点最新状态 */
VOID IPC_APP_upload_all_status(VOID);

#ifdef TUYA_DP_DOOR_BELL
/* 门铃产品形态下，当按下门铃后推送通知到APP */
VOID IPC_APP_trigger_door_bell(VOID);
#endif

#ifdef TUYA_DP_SD_FORMAT_STATUS_ONLY_GET
/* SD卡格式化进度上报 */
VOID IPC_APP_report_sd_format_status(INT_T status);
#endif

#ifdef TUYA_DP_SD_STATUS_ONLY_GET
/* 当SD卡发生变化时(插拔)，调用此API通知APP */
VOID IPC_APP_report_sd_status_changed();
#endif

#ifdef TUYA_DP_SD_STORAGE_ONLY_GET
/* 格式化后，调用此API上报存储容量 */
VOID IPC_APP_report_sd_storage();
#endif


#ifdef TUYA_DP_POWERMODE
/* 电源产品形态下，电池状态发生变动时，调用此API通知APP */
VOID IPC_APP_update_battery_status(VOID);
#endif

#ifdef TUYA_DP_MOTION_DETECTION_ALARM
/* 当发生移动侦测事件时，调用该API推送报警图片到APP */
OPERATE_RET IPC_APP_Send_Motion_Alarm_From_Buffer(CHAR_T *data, UINT_T size, NOTIFICATION_CONTENT_TYPE_E type);
OPERATE_RET IPC_APP_Send_Motion_Alarm(CHAR_T *p_abs_file, NOTIFICATION_CONTENT_TYPE_E file_type);
#endif

#ifdef TUYA_DP_DOOR_BELL_SNAP
/* 门铃产品形态下，当按下门铃后，门铃抓拍一张图片，调用该API推送图片到APP */
OPERATE_RET IPC_APP_Send_DoorBell_Snap(CHAR_T *p_snap_file, NOTIFICATION_CONTENT_TYPE_E file_type);
#endif

/* 工具API */
OPERATE_RET IPC_APP_handle_dp_cmd_objs(IN CONST TY_RECV_OBJ_DP_S *dp_rev);

OPERATE_RET IPC_APP_handle_raw_dp_cmd_objs(IN CONST TY_RECV_RAW_DP_S *dp_rev);

/* 工具API */
OPERATE_RET IPC_APP_handle_dp_query_objs(TY_DP_QUERY_S *dp_query);

void MCUSETSENSITIVITY(int ilevel,int net_status,int mode=0 ); //mode 0 ,mode 1 仅仅上传
VOID IPC_APP_REPORT_ONVIF_PW_CHANGED(IN BOOL_T bChanged);
VOID IPC_APP_REPORT_ONVIF_IP_ADDR(IN CHAR_T *pIpAddr);

#ifdef __cplusplus
}
#endif

#endif  /*_TUYA_IPC_DP_UTILS_H*/
