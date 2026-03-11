/*********************************************************************************
  *Copyright(C),2017, 涂鸦科技 www.tuya.comm
  *FileName:    tuya_ipc_mgr_handler.h
  *
  * 文件描述：
  * 1. 固件函数API定义，具体内容请参考.c文件中的代码注释
**********************************************************************************/

#ifndef _TUYA_IPC_SYSTEM_CONTROL_H
#define _TUYA_IPC_SYSTEM_CONTROL_H

#ifdef __cplusplus
extern "C" {
#endif

#include "sdk_inc/tuya_cloud_com_defs.h"
#include "sdk_inc/tuya_ipc_media.h"

typedef enum
{
    IPC_BOOTUP_FINISH,
    IPC_START_WIFI_CFG,
    IPC_REV_WIFI_CFG,
    IPC_CONNECTING_WIFI,
    IPC_MQTT_ONLINE,
    IPC_RESET_SUCCESS,
}IPC_APP_NOTIFY_EVENT_E;

/* 更新本地时间 */
OPERATE_RET IPC_APP_Sync_Utc_Time(VOID);
VOID IPC_APP_Show_OSD_Time(VOID);

VOID IPC_APP_Reset_System_CB(GW_RESET_TYPE_E reboot_type);

VOID IPC_APP_Restart_Process_CB(VOID);

VOID IPC_APP_Upgrade_Firmware_CB(VOID);

VOID IPC_APP_Notify_LED_Sound_Status_CB(IPC_APP_NOTIFY_EVENT_E notify_event);

BOOL_T tuya_get_sniffer_enable();


#ifdef __cplusplus
}
#endif

#endif  /*_TUYA_IPC_SYSTEM_CONTROL_H*/
