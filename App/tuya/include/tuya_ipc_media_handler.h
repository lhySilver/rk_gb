/*********************************************************************************
  *Copyright(C),2015-2020, 涂鸦科技 www.tuya.comm
  *FileName:    tuya_ipc_media_demo.h
**********************************************************************************/


#ifndef _TUYA_IPC_MEDIA_HANDLER_H
#define _TUYA_IPC_MEDIA_HANDLER_H

#include "sdk_inc/tuya_ring_buffer.h"
#include "sdk_inc/tuya_cloud_types.h"


#ifdef __cplusplus
extern "C" {
#endif



VOID TUYA_APP_Enable_Speaker_CB(BOOL_T enabled);

VOID TUYA_APP_Rev_Audio_CB(IN CONST MEDIA_FRAME_S *p_audio_frame,
                           TUYA_AUDIO_SAMPLE_E audio_sample,
                           TUYA_AUDIO_DATABITS_E audio_databits,
                           TUYA_AUDIO_CHANNEL_E audio_channel);


/* 工具API */
VOID IPC_APP_Set_Media_Info(VOID);
/* 工具API */
OPERATE_RET TUYA_APP_Init_Ring_Buffer(VOID);
/* 工具API */
OPERATE_RET TUYA_APP_Init_Stream_Storage(IN CONST CHAR_T *p_sd_base_path);
/* 将采集到的音视频裸数据送入SDK */
OPERATE_RET TUYA_APP_Put_Frame(IN CONST Ring_Buffer_User_Handle_S channel, IN CONST MEDIA_FRAME_S *p_frame);
/* 工具API */
//OPERATE_RET TUYA_APP_Get_Frame(IN CONST Ring_Buffer_User_Handle_S channel, IN CONST USER_INDEX_E user_index, IN CONST BOOL_T isRetry, IN CONST BOOL_T ifBlock, INOUT MEDIA_FRAME_S *p_frame);
/* 工具API */
OPERATE_RET TUYA_APP_Enable_P2PTransfer(IN UINT_T max_users);
/* 工具API */
OPERATE_RET TUYA_APP_Enable_EchoShow_Chromecast(VOID);

/* 事件模式录像下，发生事件时，通知SDK录制 */
OPERATE_RET TUYA_APP_Trigger_Event_Storage(VOID);

/* 工具API */
OPERATE_RET TUYA_APP_Enable_CloudStorage(VOID);

#ifdef __cplusplus
}
#endif

#endif  /* _TUYA_IPC_MEDIA_HANDLER_H */
