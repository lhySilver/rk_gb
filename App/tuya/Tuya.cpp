#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "Tuya.h"

extern void tuya_ipc_motion_detect(int status,int type);

static Ring_Buffer_User_Handle_S s_mainVideoHandle = NULL;
static Ring_Buffer_User_Handle_S s_subVideoHandle = NULL;
static Ring_Buffer_User_Handle_S s_audioHandle = NULL;

bool bIsSyncUTCTime = false; 					//时间同步标识
extern unsigned char bStartPrivateMode; 		//是否开启隐私模式
extern int DeviceMode_g;
extern INT_T g_time_zone;
extern INT_T g_time_dst;
extern INT_T s_mqtt_status;
extern INT_T tuay_main();
extern VOID IPC_APP_report_anbao_light_status(BOOL_T status);
extern VOID IPC_APP_report_siren_status(BOOL_T status);

static int onCapture_fn(int media_chn,
        int media_type,
        int media_subtype,
        unsigned long long frame_pts,
        unsigned char *frame_data,
        int frame_len,
        int frame_end_flag)
{
	int ret = 0;
	unsigned char *pFrmBuff = frame_data;
	unsigned int uiFrmSize = (unsigned int)frame_len;
	unsigned long long ullTimeStamp = frame_pts;

	if (DMC_MEDIA_VIDEO_MAIN_STREAM == media_chn)
	{
		if( !s_mainVideoHandle )
			return -1;
		
		{
			MEDIA_FRAME_S h264_frame;
			memset( &h264_frame, 0, sizeof(h264_frame) );
			
			//注意: 部分编码器出I帧时SPS/PPS/SEI/IDR分开上传，需要合并为一个连续帧后传入，且不能删除NALU的分割符
			h264_frame.type = (DMC_MEDIA_SUBTYPE_IFRAME == media_subtype) ? E_VIDEO_I_FRAME : E_VIDEO_PB_FRAME;
			h264_frame.p_buf = pFrmBuff;
			h264_frame.size = uiFrmSize;
			h264_frame.pts = ullTimeStamp*1000 - (g_time_zone*1000*1000) - (g_time_dst*1000*1000);
			h264_frame.timestamp = h264_frame.pts;

			/* 将高清视频数据送入SDK */
			ret = TUYA_APP_Put_Frame(s_mainVideoHandle, &h264_frame);
			RtspPutFrame_Main(media_subtype, &ullTimeStamp, (char *)pFrmBuff, uiFrmSize);
			// printf("----------------------------->onCapture, stream %d, size = %d, ret = %d\n", media_chn, frame_len, ret);	
		}
	}
	else if  (DMC_MEDIA_VIDEO_SUB_STREAM == media_chn)
	{
		if( !s_subVideoHandle )
			return -1;

		{
			MEDIA_FRAME_S h264_frame;
			memset( &h264_frame, 0, sizeof(h264_frame) );
			
			//注意: 部分编码器出I帧时SPS/PPS/SEI/IDR分开上传，需要合并为一个连续帧后传入，且不能删除NALU的分割符
			h264_frame.type = (DMC_MEDIA_SUBTYPE_IFRAME == media_subtype) ? E_VIDEO_I_FRAME : E_VIDEO_PB_FRAME;
			h264_frame.p_buf = pFrmBuff;
			h264_frame.size = uiFrmSize;
			h264_frame.pts = ullTimeStamp*1000 - (g_time_zone*1000*1000) - (g_time_dst*1000*1000);
			h264_frame.timestamp = h264_frame.pts;

			/* 将高清视频数据送入SDK */
			ret = TUYA_APP_Put_Frame(s_subVideoHandle, &h264_frame);
			RtspPutFrame_Sub(media_subtype, &ullTimeStamp, (char *)pFrmBuff, uiFrmSize);
			// printf("----------------------------->onCapture, stream %d, size = %d, ret = %d\n", media_chn, frame_len, ret);	
		}
	}
	else if (DMC_MEDIA_AUDIO_FRIST_STREAM == media_chn)
	{
		if( !s_audioHandle )
			return -1;

		{
			MEDIA_FRAME_S pcm_frame;
			memset( &pcm_frame, 0, sizeof(pcm_frame) );
			pcm_frame.type = E_AUDIO_FRAME;	
			pcm_frame.size = uiFrmSize;
			pcm_frame.p_buf = (BYTE_T *)pFrmBuff;
			pcm_frame.pts = ullTimeStamp*1000 - (g_time_zone*1000*1000) - (g_time_dst*1000*1000);
			pcm_frame.timestamp = pcm_frame.pts; 

			ret = TUYA_APP_Put_Frame(s_audioHandle, &pcm_frame);

			RtspPutFrame_Audio(media_subtype, &ullTimeStamp, (char *)pFrmBuff, uiFrmSize);
			RtspPutFrame_Audio1(media_subtype, &ullTimeStamp, (char *)pFrmBuff, uiFrmSize);
			// printf("----------------------------->onCapture, stream %d, size = %d, ret = %d\n", media_chn, frame_len, ret);	
		}
	}

	return 0;
}

int open_tuya_ring_buffer_handle()
{
	s_mainVideoHandle = tuya_ipc_ring_buffer_open(0, 0, E_IPC_STREAM_VIDEO_MAIN, E_RBUF_WRITE);
	if(s_mainVideoHandle == NULL)
	{
		printf("tuya open E_IPC_STREAM_VIDEO_MAIN ring buffer failed.\n");
		goto err_exit;
	}
	s_subVideoHandle = tuya_ipc_ring_buffer_open(0, 0, E_IPC_STREAM_VIDEO_SUB, E_RBUF_WRITE);
	if(s_subVideoHandle == NULL)
	{
		printf("tuya open E_IPC_STREAM_VIDEO_SUB ring buffer failed.\n");
		goto err_exit;
	}
	s_audioHandle = tuya_ipc_ring_buffer_open(0, 0, E_IPC_STREAM_AUDIO_MAIN, E_RBUF_WRITE);
	if(s_audioHandle == NULL)
	{
		printf("tuya open E_IPC_STREAM_AUDIO_MAIN ring buffer failed.\n");
		goto err_exit;
	}

	return 0;
err_exit:
	if( s_mainVideoHandle )
		tuya_ipc_ring_buffer_close(s_mainVideoHandle);
	if( s_subVideoHandle )
		tuya_ipc_ring_buffer_close(s_subVideoHandle);
	if( s_audioHandle )
		tuya_ipc_ring_buffer_close(s_audioHandle);
	return -1;
}

//音视频初始化
// int create_streamout_thread()
// {
// 	g_AVManager.VideoInit();
// 	g_AVManager.RealTimeStreamStart(DMC_MEDIA_TYPE_H264 | DMC_MEDIA_TYPE_H265 | DMC_MEDIA_TYPE_AUDIO, onCapture_fn);

// 	enable_sub_video = 1;
// 	enable_main_video = 1;

// 	return 0;
// }
// int destroy_streamout_thread()
// {
// 	enable_sub_video = 0;
// 	enable_main_video = 0;
// 	g_AVManager.RealTimeStreamStop(DMC_MEDIA_TYPE_H264 | DMC_MEDIA_TYPE_H265 | DMC_MEDIA_TYPE_AUDIO);
// 	g_AVManager.VideoDeInit();

// 	return 0;
// }

PATTERN_SINGLETON_IMPLEMENT(CTuya);

CTuya::CTuya() : CThread("Tuya", 50)
{
	m_bTuyaSdkStarted = false;
	m_bIsUpdate = false;
	IEventManager::instance()->attach(IEventManager::Proc(&CTuya::onAppEvent, this));
}

CTuya::~CTuya()
{
}

//获取设备注册激活状态
bool CTuya::GetRegisterStatus()
{
	return (E_IPC_ACTIVEATED == tuya_ipc_get_register_status());
}

void CTuya::ThreadProc()
{
	int ret = -1;
	//配置文件读取掉电前隐私模式状态
	{
		bool re;
		Json::Value table;
		int status = 0;
		re = g_configManager.getConfig(getConfigName(CFG_PRIVATE), table);
		AppWarning("get CFG_PRIVATE config %s\n", re ? "succ" : "failed");
		AppWarning("CFG_PRIVATE status %d\n",table["Status"].asInt());
		status = table["Status"].asInt();
	
		if(status)
			bStartPrivateMode=1;
		else
			bStartPrivateMode=0;
	}
	//启动音视频模块
	g_AVManager.RealTimeStreamStart(DMC_MEDIA_TYPE_H264 | DMC_MEDIA_TYPE_H265 | DMC_MEDIA_TYPE_AUDIO, onCapture_fn);

	//启动涂鸦模块
	tuay_main();
	//tuya ring buffer初始化
	ret = open_tuya_ring_buffer_handle();
	AppInfo("open_tuya_ring_buffer_handle. ret : %d\n", ret);
	
	
	m_bTuyaSdkStarted = true;

	/* 判断SDK是否连接到MQTT */
	int mqtt_count = 0;
    while(s_mqtt_status != 1)
    {
        sleep(1);
		mqtt_count ++;
        AppErr("wait for mqtt online count=%d\n",mqtt_count);
        if (mqtt_count >= 2*60)
        {
            break;
        }
    }

	if (1 == s_mqtt_status)
	{
		//连上涂鸦上报一下灯状态
		ReportAnbaoLightSwich(g_Camera.GetWhiteLedStatus());

		g_IndicatorLight.setLightStatus(CIndicatorLight::ENUM_POWER_INDICATOR_LIGHT_ALWAYS_OFF);
		g_IndicatorLight.setLightStatus(CIndicatorLight::ENUM_LINK_INDICATOR_LIGHT_ALWAYS_ON);
		AppErr("mqtt ok\n");
	}
	else
	{
		AppErr("mqtt offline\n");
	}

	/* 判断SDK是否校时成功*/
	int sync_time_count = 0;
    while(bIsSyncUTCTime != true)
    {
        sleep(1);
		sync_time_count ++;
        AppErr("wait for sync time count=%d\n",sync_time_count);
        if (sync_time_count >= 1*60)
        {
            break;
        }
    }

	if (bIsSyncUTCTime)
	{
		AppErr("sync time ok\n");
	}
	else
	{
		AppErr("sync time fail\n");
	}

	//摄像头参数控制模块
	g_Camera.start();
	//启动报警模块
	g_Alarm.SetAllowMotionDetTime(time(NULL)+10);
	g_Alarm.Start();
	//启动安保灯控制模块
//	g_AnBaoLightManager.Start();
	//启动警笛控制模块
	g_Siren.Start();
	//启动录像模块
	g_RecordManager.Start();

	// bool bhumenDet = IPC_APP_get_human_filter();
	while(m_bLoop)
	{
		// bool bHD;
		// if (access("/tmp/HD", F_OK) == 0)
		// 	bHD = true;
		// else
		// 	bHD = false;
		// if (bhumenDet != bHD)
		// {
		// 	IPC_APP_set_human_filter(bHD);
		// 	bhumenDet = bHD;
		// }
		sleep(1);

		if( true == m_bIsUpdate )
			continue;
		
		//监测SD卡状态, 如果卡异常一分钟, 则重启设备
		{
			static int sdcard_error_count = 0;
			//获取SD卡状态
			static int iLastDiskStatus = DISK_STATUS_UNKNOWN;
			int iDiskStatus = DISK_STATUS_UNKNOWN;
			iDiskStatus = g_StorageManager->GetDiskState();

			if( DISK_STATUS_UNKNOWN == iLastDiskStatus )
			{
				iLastDiskStatus = iDiskStatus;
			}
			if( iLastDiskStatus != iDiskStatus )
			{
				iLastDiskStatus = iDiskStatus;
				IPC_APP_report_sd_status_changed();
			}
			
			if( DISK_STATUS_ERROR == iDiskStatus )
			{
				sdcard_error_count++;
			}
			else
			{
				sdcard_error_count = 0;
			}

			if( sdcard_error_count >= 60 )
			{
				sdcard_error_count = 0;
				AppErr("sd card error more than 1 min. reboot\n");
				AbnormalRestart();
			}
		}

		//监测主码流编码
		if (1)
		{
			static unsigned long long LastFrameCount = 0;
			static int SameCount = 0;

			if( 1 )
			{
				unsigned int FrameCount = CaptureGetEncodeFrameCount(0);
//				AppInfo("[main encode] ----------frame count : %u\n", FrameCount);
				if (LastFrameCount == FrameCount) 
				{
					SameCount ++;
				}
				else
				{
					SameCount = 0;
				}
					
				if (SameCount > 60) 
				{
					AppErr("CheckMainStreamEncodeFrameCount count=[%d] system reboot\n",SameCount);
					AbnormalRestart();
				}
				LastFrameCount = FrameCount;
			}
			else
			{
				SameCount = 0;
			}
		}
		
		//监测子码流编码
		if(1)
		{
			static unsigned long long LastFrameCount = 0;
			static int SameCount = 0;

			if( 1 )
			{
				unsigned int FrameCount = CaptureGetEncodeFrameCount(1);
//				AppInfo("[sub encode] ----------frame count : %u\n", FrameCount);
				if (LastFrameCount == FrameCount) 
				{
					SameCount ++;
				}
				else
				{
					SameCount = 0;
				}
					
				if (SameCount > 60) 
				{
					AppErr("CheckSubStreamEncodeFrameCount count=[%d] system reboot\n",SameCount);
					AbnormalRestart();
				}
				LastFrameCount = FrameCount;
			}
			else
			{
				SameCount = 0;
			}
		}
	}
}

int CTuya::init()
{
	return 0;
}

bool CTuya::start()
{
	return CreateThread();
}

bool CTuya::stop()
{
	return DestroyThread();
}

bool CTuya::IsTuyaSdkStarted()
{
	return m_bTuyaSdkStarted;
}

int CTuya::ReportAnbaoLightSwich(int onoff)
{
	int ret = -1;
	if (1 == s_mqtt_status)
	{
		IPC_APP_report_anbao_light_status(onoff);
		ret = 0;
	}
	return ret;
}
int CTuya::ReportSirenSwitch(int onoff)
{
	int ret = -1;
	if (1 == s_mqtt_status)
	{
		IPC_APP_report_siren_status(onoff);
		ret = 0;
	}
	return ret;
}
int CTuya::ReportOnvifIp(char *pIpAddr)
{
	int ret = -1;
	if (1 == s_mqtt_status)
	{
		IPC_APP_REPORT_ONVIF_IP_ADDR(pIpAddr);
		ret = 0;
	}
	return ret;
}
int CTuya::EventNotification(int status,int type)
{
	int ret = -1;
	if ( (1 == s_mqtt_status) && (bIsSyncUTCTime))
	{
		tuya_ipc_motion_detect(status,type);
		ret = 0;
	}
	return ret;
}
bool CTuya::IsSyncUTCTime()
{
	return bIsSyncUTCTime;
}

void CTuya::onAppEvent(std::string code, int index, appEventAction action, const EventHandler *param, Json::Value* data)
{
	if( "UpgradeReleaseResource" == code )
	{
		AppErr("stop tuya encoder check\n");
		m_bIsUpdate = true; 	//正在升级，不检测视频流
	}
}

