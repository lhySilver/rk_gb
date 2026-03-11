/*********************************************************************************
  *Copyright(C),2015-2020, 涂鸦科技 www.tuya.comm
  *FileName: tuya_ipc_system_control_demo.c
  *
  * 文件描述：
  * 该demo显示了涂鸦SDK如何使用callback的方式实现系统控制如：
  * 1. 设置本地ID。
  * 2. 重启System和重启进程。
  * 3. OTA升级。
  * 4. 声音和LED提示。
  *
**********************************************************************************/

#include <string.h>
#include <stdio.h>
#include <sys/sysinfo.h>
#include "include/sdk_inc/tuya_ipc_media.h"
#include "include/sdk_inc/tuya_cloud_com_defs.h"
#include "include/sdk_inc/tuya_cloud_types.h"
#include "include/tuya_ipc_common.h"
#include "include/tuya_ipc_system_control.h"
#include "include/sdk_inc/uni_log.h"

#include <iostream>
#include <fstream>
#include <algorithm>

#include "Tuya.h"


static FW_UG_S FW_UG_g = {0};

extern IPC_MGR_INFO_S s_mgr_info;
extern INT_T g_time_zone; 	//时区, 以秒为单位
extern INT_T g_time_dst;

// extern void delete_file(const char *path);
// extern void onCapture(int iChannel, uint dwStreamType, CPacket *packet);
// extern void onCapture_2(int iChannel, uint dwStreamType, CPacket *packet);
// extern void onAudioCapture(int iChannel, uint dwStreamType, CPacket *packet);

/* 
当用户在APP上点击移除设备时的回调
*/
VOID IPC_APP_Reset_System_CB(GW_RESET_TYPE_E type)
{
    AppErr("reset ipc success. please restart the ipc %d\n", type);
    SystemReset();
}

VOID IPC_APP_Restart_Process_CB(VOID)
{
    AppErr("sdk internal restart request. please restart the ipc\n");
	AbnormalRestart();
}

/* Developers need to implement the corresponding prompt sound playback and LED prompts,
   you can refer to the SDK attached files, using TUYA audio files. */
VOID IPC_APP_Notify_LED_Sound_Status_CB(IPC_APP_NOTIFY_EVENT_E notify_event)
{
    AppInfo("curr event:%d \r\n", notify_event);
    switch (notify_event)
    {
        case IPC_BOOTUP_FINISH: /* 启动成功 */
        {
            break;
        }
        case IPC_START_WIFI_CFG: /* 开始配置网络 */
        {
			//语音提示
			CAudioPrompt::AudioFileParm audioFile;
			audioFile.strFileName = AUDIO_FILE_PLEASE_SET_WIFI;
            audioFile.type = 0;
			g_AudioPrompt.aoPlay(audioFile);
            break;
        }
        case IPC_REV_WIFI_CFG: /* 收到网络配置信息 */
        {
            break;
        }
        case IPC_CONNECTING_WIFI: /* 开始连接WIFI */
        {
            break;
        }
        case IPC_MQTT_ONLINE: /* MQTT上线 */
        {
            break;
        }
        case IPC_RESET_SUCCESS: /* 重置完成 */
        {
			CAudioPrompt::AudioFileParm audioFile;
			audioFile.strFileName = AUDIO_FILE_DEV_RESET;
            audioFile.type = 0;
			g_AudioPrompt.aoPlay(audioFile);
            break;
        }
        default:
        {
            break;
        }
    }
}

/* 对讲模式声音回调，开启关闭扬声器硬件 */
VOID TUYA_APP_Enable_Speaker_CB(BOOL_T enabled)
{
    AppDebug("enable speaker %d \r\n", enabled);
    //TODO
    /* Developers need to turn on or off speaker hardware operations. 
    If IPC hardware features do not need to be explicitly turned on, the function can be left blank. */

	if( TRUE == enabled )
	{
		IAudioManager::instance()->StartAudioOut(IAudioManager::AUDIO_TALK_TYPE);
	}
	else
	{
		IAudioManager::instance()->StopAudioOut(IAudioManager::AUDIO_TALK_TYPE);
	}
}

/* 对讲模式声音回调，开启关闭扬声器硬件 */
VOID TUYA_APP_Rev_Audio_CB(IN CONST MEDIA_FRAME_S *p_audio_frame,
                            TUYA_AUDIO_SAMPLE_E audio_sample,
                            TUYA_AUDIO_DATABITS_E audio_databits,
                            TUYA_AUDIO_CHANNEL_E audio_channel)
{
   // printf("rev audio cb len:%u sample:%d db:%d channel:%d\r\n", p_audio_frame->size, audio_sample, audio_databits, audio_channel);
    //PCM-Format 8K 16Bit MONO
    //TODO
    /* 开发者需要实现扬声器播放声音操作 */
	IAudioManager::instance()->PlayVoice( p_audio_frame->p_buf, p_audio_frame->size, IAudioManager::AUDIO_TALK_TYPE);
}

OPERATE_RET IPC_APP_Sync_Utc_Time(VOID)
{
    TIME_T time_utc;
    INT_T time_zone;
    OPERATE_RET ret = tuya_ipc_get_service_time_force(&time_utc, &time_zone);

    if(ret != OPRT_OK)
    {
        return ret;
    }
    //API返回OK，说明成功获取到了UTC时间，如果返回不是OK，说明还没有取到时间。

    PR_DEBUG("Get Server Time Success: %lu %d", time_utc, time_zone);
//  TODO
//  使用对应的系统接口设置时间如 settimeofday;

	g_time_zone = time_zone; 		//记录时区, 搜索录像、回放等会用到

	BOOL_T isDls = FALSE;
	tuya_ipc_check_in_dls(time_utc, &isDls);
	if( TRUE == isDls )
	{
	
		g_time_dst = 3600;
		time_utc += 3600;   //夏令时调整一小时
	}
	else
	{
		g_time_dst = 0;
	}

	struct timeval tv;
	gettimeofday(&tv, NULL);
	int value = (int)(tv.tv_sec) - (time_utc + time_zone);
	if( (abs(value) >= 5) )
	{
		tv.tv_sec = time_utc + time_zone;
		tv.tv_usec = 0;
		settimeofday(&tv, NULL);
		tuya_ipc_set_service_time(time_utc);
	}
    return OPRT_OK;
}




