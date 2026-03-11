/*********************************************************************************
  *Copyright(C),2015-2020, 涂鸦科技 www.tuya.comm
  *FileName: tuya_ipc_media_demo.c
**********************************************************************************/
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/statfs.h>  
#include "include/tuya_ipc_common.h"
#include "include/sdk_inc/tuya_ipc_api.h"
#include "include/sdk_inc/tuya_ipc_media.h"
#include "include/tuya_ipc_media_handler.h"
#include "include/sdk_inc/tuya_ipc_stream_storage.h"
#include "include/sdk_inc/tuya_ipc_cloud_storage.h"
#include "include/sdk_inc/tuya_ring_buffer.h"
#include "include/sdk_inc/tuya_ipc_doorbell.h"
#include "include/sdk_inc/uni_log.h"

#include "Tuya_common.h"

IPC_MEDIA_INFO_S s_media_info = {0};



extern CHAR_T s_raw_path[128];

/* 设置音视频属性 */
// VOID IPC_APP_Set_Media_Info(VOID)
// {
//     memset(&s_media_info, 0 , sizeof(IPC_MEDIA_INFO_S));
	
// 	//获取sensor 确定主码流参数
// 	int main_videow = 3840;//3200;//1920;
// 	int main_videoh = 1080;//544;//1080;
// 	int sub_videow = 1920;//1066;
// 	int sub_videoh = 544;//180;
// 	int denominator = 15;
// 	int gop = denominator * 2;
	
	
//     /* 高清主码流，视频配置 */
//     /* 注意1： 如果设备主码流支持多种视频流配置，这里请把各项设置为允许配置的上限值 */
//     /* 注意2： E_CHANNEL_VIDEO_MAIN必须存在，是SDK内部存储业务的数据源。对于只有编码器只出一路流的情况，请关闭E_CHANNEL_VIDEO_SUB */
//     s_media_info.channel_enable[E_IPC_STREAM_VIDEO_MAIN] 	= TRUE;    /* 是否开启本地高清视频流 */
//     s_media_info.video_fps[E_IPC_STREAM_VIDEO_MAIN] 		= denominator;  /* FPS */
//     s_media_info.video_gop[E_IPC_STREAM_VIDEO_MAIN] 		= gop;  /* GOP */
//     s_media_info.video_bitrate[E_IPC_STREAM_VIDEO_MAIN] 	= TUYA_VIDEO_BITRATE_2M; /* 码率上限 */
//     s_media_info.video_width[E_IPC_STREAM_VIDEO_MAIN] 	    = main_videow; /* 单帧分辨率 宽 */
//     s_media_info.video_height[E_IPC_STREAM_VIDEO_MAIN] 	= main_videoh;/* 单帧分辨率 高 */
//     s_media_info.video_freq[E_IPC_STREAM_VIDEO_MAIN] 		= 90000; /* 时钟频率 */
//     s_media_info.video_codec[E_IPC_STREAM_VIDEO_MAIN] 	    = TUYA_CODEC_VIDEO_H265; /* 编码格式 */

//     /* 标清子码流，视频配置 */
//     /* 请注意如果设备子码流支持多种视频流配置，这里请把各项设置为允许配置的上限值 */
//     s_media_info.channel_enable[E_IPC_STREAM_VIDEO_SUB] 	= TRUE;     /* 是否开启本地标清视频流 */
//     s_media_info.video_fps[E_IPC_STREAM_VIDEO_SUB] 		= denominator;  /* FPS */
//     s_media_info.video_gop[E_IPC_STREAM_VIDEO_SUB] 		= gop;  /* GOP */
//     s_media_info.video_bitrate[E_IPC_STREAM_VIDEO_SUB] 	= TUYA_VIDEO_BITRATE_2M; /* 码率上限 */
//     s_media_info.video_width[E_IPC_STREAM_VIDEO_SUB] 		= sub_videow; /* 单帧分辨率 宽 */
//     s_media_info.video_height[E_IPC_STREAM_VIDEO_SUB] 	    = sub_videoh;/* 单帧分辨率 高 */
//     s_media_info.video_freq[E_IPC_STREAM_VIDEO_SUB] 		= 90000; /* 时钟频率 */
//     s_media_info.video_codec[E_IPC_STREAM_VIDEO_SUB] 		= TUYA_CODEC_VIDEO_H265; /* 编码格式 */

//     /* 音频码流配置. 注意：SDK内部P2P预览、云存储、本地存储使用的都是E_CHANNEL_AUDIO的数据 */
// 	s_media_info.channel_enable[E_IPC_STREAM_AUDIO_MAIN] 		= TRUE;         /* 是否开启本地声音采集 */
// 	s_media_info.audio_codec[E_IPC_STREAM_AUDIO_MAIN] 			= TUYA_CODEC_AUDIO_PCM;//TUYA_CODEC_AUDIO_G711A;/* 编码格式 */
// 	s_media_info.audio_sample[E_IPC_STREAM_AUDIO_MAIN] 			= TUYA_AUDIO_SAMPLE_8K;/* 采样率 */
// 	s_media_info.audio_databits[E_IPC_STREAM_AUDIO_MAIN] 		= TUYA_AUDIO_DATABITS_16;/* 位宽 */
// 	s_media_info.audio_channel[E_IPC_STREAM_AUDIO_MAIN] 		= TUYA_AUDIO_CHANNEL_MONO;/* 信道 */
// 	s_media_info.audio_fps[E_IPC_STREAM_AUDIO_MAIN] 			= 25;/* 每秒分片数 */

//     PR_DEBUG("channel_enable:%d %d %d", s_media_info.channel_enable[0], s_media_info.channel_enable[1], s_media_info.channel_enable[2]);

//     PR_DEBUG("fps:%u", s_media_info.video_fps[E_IPC_STREAM_VIDEO_MAIN]);
//     PR_DEBUG("gop:%u", s_media_info.video_gop[E_IPC_STREAM_VIDEO_MAIN]);
//     PR_DEBUG("bitrate:%u kbps", s_media_info.video_bitrate[E_IPC_STREAM_VIDEO_MAIN]);
//     PR_DEBUG("video_main_width:%u", s_media_info.video_width[E_IPC_STREAM_VIDEO_MAIN]);
//     PR_DEBUG("video_main_height:%u", s_media_info.video_height[E_IPC_STREAM_VIDEO_MAIN]);
//     PR_DEBUG("video_freq:%u", s_media_info.video_freq[E_IPC_STREAM_VIDEO_MAIN]);
//     PR_DEBUG("video_codec:%d", s_media_info.video_codec[E_IPC_STREAM_VIDEO_MAIN]);

//     PR_DEBUG("audio_codec:%d", s_media_info.audio_codec[E_IPC_STREAM_AUDIO_MAIN]);
//     PR_DEBUG("audio_sample:%d", s_media_info.audio_sample[E_IPC_STREAM_AUDIO_MAIN]);
//     PR_DEBUG("audio_databits:%d", s_media_info.audio_databits[E_IPC_STREAM_AUDIO_MAIN]);
//     PR_DEBUG("audio_channel:%d", s_media_info.audio_channel[E_IPC_STREAM_AUDIO_MAIN]);


// }


/*
---------------------------------------------------------------------------------
RingBuffer相关代码起始位置
---------------------------------------------------------------------------------
*/
// OPERATE_RET TUYA_APP_Init_Ring_Buffer(VOID)
// 	{
// 		STATIC BOOL_T s_ring_buffer_inited = FALSE;
// 		if(s_ring_buffer_inited == TRUE)
// 		{
// 			PR_DEBUG("The Ring Buffer Is Already Inited");
// 			return OPRT_OK;
// 		}
	
// 		INT_T channel;
// 		OPERATE_RET ret;
// 		Ring_Buffer_Init_Param_S param;
// 		for( channel = E_IPC_STREAM_VIDEO_MAIN; channel < E_IPC_STREAM_MAX; channel++ )
// 		{
// 			PR_DEBUG("init ring buffer Channel:%d Enable:%d", channel, s_media_info.channel_enable[channel]);
// 			if(s_media_info.channel_enable[channel] == TRUE)
// 			{
// 				if(channel == E_IPC_STREAM_AUDIO_MAIN)
// 				{
// 					param.bitrate = s_media_info.audio_sample[E_IPC_STREAM_AUDIO_MAIN]*s_media_info.audio_databits[E_IPC_STREAM_AUDIO_MAIN]/1024;
// 					param.fps = s_media_info.audio_fps[E_IPC_STREAM_AUDIO_MAIN];
// 					param.max_buffer_seconds = 0;
// 					param.requestKeyFrameCB = NULL;
// 					PR_DEBUG("audio_sample %d, audio_databits %d, audio_fps %d",s_media_info.audio_sample[E_IPC_STREAM_AUDIO_MAIN],s_media_info.audio_databits[E_IPC_STREAM_AUDIO_MAIN],s_media_info.audio_fps[E_IPC_STREAM_AUDIO_MAIN]);
// 					ret = tuya_ipc_ring_buffer_init(0,0,(IPC_STREAM_E)channel,&param);
// 				}
// 				else
// 				{
// 					param.bitrate = s_media_info.video_bitrate[channel];
// 					param.fps = s_media_info.video_fps[channel];
// 					param.max_buffer_seconds = 0;
// 					param.requestKeyFrameCB = NULL;
// 					PR_DEBUG("video_bitrate %d, video_fps %d",s_media_info.video_bitrate[channel],s_media_info.video_fps[channel]);
// 					ret = tuya_ipc_ring_buffer_init(0,0,(IPC_STREAM_E)channel,&param);
// 				}
// 				if(ret != 0)
// 				{
// 					PR_ERR("init ring buffer fails. %d %d", channel, ret);
// 					return OPRT_MALLOC_FAILED;
// 				}
// 				PR_DEBUG("init ring buffer success. channel:%d", channel);
// 			}
// 		}
	
// 		s_ring_buffer_inited = TRUE;
	
// 		return OPRT_OK;
// 	}







OPERATE_RET TUYA_APP_Put_Frame(IN CONST Ring_Buffer_User_Handle_S handle, IN CONST MEDIA_FRAME_S *p_frame)
{
	AppDebug("Put Frame. type:%d size:%u pts:%llu ts:%llu", p_frame->type, p_frame->size, p_frame->pts, p_frame->timestamp);

    //OPERATE_RET ret = tuya_ipc_ring_buffer_append_data(channel,p_frame->p_buf, p_frame->size,p_frame->type,p_frame->pts);
	OPERATE_RET ret=tuya_ipc_ring_buffer_append_data(handle, p_frame->p_buf,p_frame->size,p_frame->type, p_frame->pts);

    if(ret != OPRT_OK)
    {
        PR_ERR("Put Frame Fail.%d  type:%d size:%u pts:%llu ts:%llu",ret,
			p_frame->type, p_frame->size, p_frame->pts, p_frame->timestamp);
    }
    return ret;
}



