/*********************************************************************************
  *Copyright(C),2015-2020, 
  *TUYA 
  *www.tuya.comm
  *FileName:    tuya_ipc_motion_detect_demo
**********************************************************************************/

#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/statfs.h>
#include <sys/time.h>
#include <sys/prctl.h>

#include "include/sdk_inc/tuya_ipc_cloud_storage.h"
#include "include/sdk_inc/tuya_ipc_stream_storage.h"
#include "include/sdk_inc/tuya_ipc_api.h"
#include "include/tuya_ipc_dp_handler.h"
#include "include/sdk_inc/tuya_ipc_event.h"

#include "Tuya_common.h"

//AI detect should enable SUPPORT_AI_DETECT
#define SUPPORT_AI_DETECT 1
#if SUPPORT_AI_DETECT
#include "include/sdk_inc/tuya_ipc_ai_detect_storage.h"
#endif

//According to different chip platforms, users need to implement the interface of capture.
void get_motion_snapshot(char *snap_addr, int *snap_size)
{
    printf("get_motion_snapshot\n");
	int ret = CaptureSnapshotGetBuffer(snap_addr, MAX_SNAPSHOT_BUFFER_SIZE, 5);
	if( ret <= 0 )
	{
		AppErr("snapshot err!!!\n");
        *snap_size = 0;
		return ;
	}
	
	printf("get_motion_snapshot ok\n");
	*snap_size = ret;
}

#if SUPPORT_AI_DETECT
//According to different chip platforms, users need to implement the interface of capture.
VOID tuya_ipc_get_snapshot_cb(char* pjbuf, int* size)
{
	printf("tuya_ipc_get_snapshot_cb\n");
	get_motion_snapshot(pjbuf, size);
}
#endif


#if 0
void send_voice_alarm()
{
	bool report_event = false;
	int report_sum = 0;	
	CHAR_T sMode[2];
	char snap_addr[MAX_SNAPSHOT_BUFFER_SIZE] = {0};
    int snap_size = 0;
	snprintf(sMode,2,"%d",2);
	tuya_ipc_dp_report(NULL,245,PROP_ENUM,sMode,1);
	report_event = true;
	report_sum = 0;
	if(g_PtzHandle.GetPtzstatus() == false)
	{
		if(IPC_APP_get_motion_track_onoff())
		{
			sleep(3);
			if(g_PtzHandle.GetPtzstatus() == true)
				return;
		}
	get_motion_snapshot(snap_addr,&snap_size);
	tuya_ipc_notify_alarm(snap_addr, snap_size, NOTIFICATION_NAME_ABNORMAL_SOUND, TRUE);
	}
}
#endif

void send_notify_alarm(int type)
{
	bool report_event = false;
	int report_sum = 0;
	int ret = -1;
	CHAR_T sMode[2];
	char snap_addr[MAX_SNAPSHOT_BUFFER_SIZE] = {0};
	int snap_size = 0;
	static int s_iLastMdNotifyTime[3] = {0};

	if(type & ALARM_TYPE_MOTION)//移动侦测
	{
		if( time(0) - s_iLastMdNotifyTime[0] > g_Alarm.GetAlarmInterval() )//当前时间间隔大于报警间隔
		{
			// s_iLastMdNotifyTime[0] = time(0);//放在此处如果出现抓图失败的情况，报警时间间隔就相当于2倍GetAlarmInterval
			get_motion_snapshot(snap_addr,&snap_size);

			if(snap_size > 0)
			{
				s_iLastMdNotifyTime[0] = time(0);//时间刷新放在此处才是正确的
				report_event = true;
				report_sum = 0;
				snprintf(sMode,2,"%d",0);
				ret = tuya_ipc_notify_alarm(snap_addr, snap_size, NOTIFICATION_NAME_MOTION, TRUE);
				printf("\033[1;36m  tuya_ipc_notify_alarm   \033[0m\n");//add on 2025.01.08 printf
				if (ret)
					printf("notify alarm motion error\n");
				else
					printf("notify alarm motion\n");

				tuya_ipc_dp_report(NULL,245,PROP_ENUM,sMode,1);
				printf("dp report alarm\n");
			}
		}
	}
	if (type & ALARM_TYPE_HUMAN)//人形检测
	{
		
		if( time(0) - s_iLastMdNotifyTime[1] > g_Alarm.GetAlarmInterval() )
		{
			// s_iLastMdNotifyTime[1] = time(0);//放在此处如果出现抓图失败的情况，报警时间间隔就相当于2倍GetAlarmInterval

			get_motion_snapshot(snap_addr,&snap_size);

			if(snap_size > 0)
			{
				s_iLastMdNotifyTime[1] = time(0);//时间刷新放在此处才是正确的
				report_event = true;
				report_sum = 0;
				snprintf(sMode,2,"%d",1);
				ret = tuya_ipc_notify_alarm(snap_addr, snap_size, NOTIFICATION_NAME_HUMAN, TRUE);
				printf("\033[1;35m  tuya_ipc_notify_alarm   \033[0m\n");//add on 2025.01.08 printf
				if (ret)
					printf("notify alarm human error\n");
				else
					printf("notify alarm human\n");
				tuya_ipc_dp_report(NULL,245,PROP_ENUM,sMode,1);
				printf("dp report alarm\n");
			}
		}
	}
	if (type & ALARM_TYPE_CAR)//车辆检测
	{
		if( time(0) - s_iLastMdNotifyTime[2] > g_Alarm.GetAlarmInterval() )
		{
			

			get_motion_snapshot(snap_addr,&snap_size);

			if(snap_size > 0)
			{
				s_iLastMdNotifyTime[2] = time(0);//时间刷新放在此处才是正确的
				report_event = true;
				report_sum = 0;
				snprintf(sMode,2,"%d",2);
				ret = tuya_ipc_notify_alarm(snap_addr, snap_size, NOTIFICATION_NAME_CAR, TRUE);
				if (ret)
					printf("notify alarm car error\n");
				else
					printf("notify alarm car\n");
				tuya_ipc_dp_report(NULL,245,PROP_ENUM,sMode,1);
				printf("dp report car\n");
			}
		}
	}
}

void tuya_ipc_motion_detect(int status,int type)
{
	TIME_T current_time;
    int motion_flag = status;
	
    static int motion_alarm_is_triggerd = FALSE;
	static TIME_T last_md_time;

	tuya_ipc_get_utc_time(&current_time);
	if(motion_flag)
	{
		last_md_time = current_time;
		if(!motion_alarm_is_triggerd)
		{
			motion_alarm_is_triggerd = TRUE;

			printf("first start storage\n");
			tuya_ipc_start_storage(E_ALARM_CLOUD_STORAGE);
			printf("first start notify alarm\n");
			send_notify_alarm(type);
		}
		else
		{
			if(ClOUD_STORAGE_TYPE_EVENT == tuya_ipc_cloud_storage_get_store_mode()
				&& FALSE == tuya_ipc_cloud_storage_get_status())
			{
				tuya_ipc_start_storage(E_ALARM_CLOUD_STORAGE);
				printf("once more start storage\n");
			}
			send_notify_alarm(type);//移动侦测持续触发时间超报警间隔，在此处进行上报
		}
	}
	else
	{
		//No motion detect for more than 10 seconds, stop the event
		if(current_time - last_md_time > 10 && motion_alarm_is_triggerd)
		{
			#if SUPPORT_AI_DETECT
			tuya_ipc_ai_detect_storage_stop();
			#endif		
			tuya_ipc_stop_storage(E_ALARM_CLOUD_STORAGE);
			printf("stop storage\n");
			motion_alarm_is_triggerd = FALSE;
		}
	}

    return;
}

#if SUPPORT_AI_DETECT
extern IPC_MEDIA_INFO_S s_media_info;
OPERATE_RET TUYA_APP_Enable_AI_Detect()
{
    AppWarning("tuya_ipc_ai_detect_storage_initn");
    int ret = tuya_ipc_ai_detect_storage_init(&s_media_info);
    AppWarning("tuya_ipc_ai_detect_storage_init %d\n",ret);

    return OPRT_OK;
}
#endif

