/*********************************************************************************
  *Copyright(C),2015-2020, 涂鸦科技 www.tuya.comm
  *FileName:    tuya_ipc_p2p_demo
**********************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/statfs.h>
#include <sys/time.h>
#include <sys/prctl.h>

#include "include/tuya_ipc_common.h"
#include "include/tuya_ipc_system_control.h"
#include "include/tuya_ipc_media_handler.h"
#include "include/sdk_inc/tuya_ipc_media.h"
#include "include/sdk_inc/tuya_ipc_stream_storage.h"
#include "include/sdk_inc/tuya_ipc_p2p.h"
#include "include/tuya_ipc_dp_handler.h"
#include "include/sdk_inc/uni_log.h"

#include "Tuya_common.h"

static int pb_status = 0;
static int pb_audio_muted = 0;
static int pb_client_index_g = -1;
int pb_handler_g = -1;

static int dl_client_index_g = -1;
int dl_handler_g = -1;


extern IPC_MEDIA_INFO_S s_media_info;
extern INT_T g_time_zone;
extern INT_T g_time_dst;


/*typedef enum{
	TY_SPEED_05TIMES = 0,			   // 0.5倍速
	TY_SPEED_10TIMES = 1 ,			   // 正常速度
	TY_SPEED_20TIMES = 3,			   // 2.0倍
		TY_SPEED_MAX,
}TY_PLAYBACK_SPEED;*/

typedef struct
{
    BOOL_T enabled;
    TRANSFER_VIDEO_CLARITY_TYPE_E live_clarity;
    UINT_T max_users;
    TUYA_CODEC_ID p2p_audio_codec;
}TUYA_APP_P2P_MGR;

STATIC TUYA_APP_P2P_MGR s_p2p_mgr = {0};

STATIC VOID __TUYA_APP_media_frame_TO_trans_video(IN CONST MEDIA_FRAME_S *p_in, INOUT TRANSFER_VIDEO_FRAME_S *p_out)
{
    UINT_T codec_type = 0;
    codec_type = (p_in->type & 0xff00) >> 8;
    p_out->video_codec = (codec_type == 0 ? TUYA_CODEC_VIDEO_H264 : TUYA_CODEC_VIDEO_H265);
    p_out->video_frame_type = (p_in->type && 0xff) == E_VIDEO_PB_FRAME ? TY_VIDEO_FRAME_PBFRAME:TY_VIDEO_FRAME_IFRAME;
    p_out->p_video_buf = p_in->p_buf;
    p_out->buf_len = p_in->size;
    p_out->pts = p_in->pts;
    p_out->timestamp = p_in->timestamp;
}

STATIC VOID __TUYA_APP_media_frame_TO_trans_audio(IN CONST MEDIA_FRAME_S *p_in, INOUT TRANSFER_AUDIO_FRAME_S *p_out)
{  
    p_out->audio_codec = s_media_info.audio_codec[E_IPC_STREAM_AUDIO_MAIN];
    p_out->audio_sample = s_media_info.audio_sample[E_IPC_STREAM_AUDIO_MAIN];
    p_out->audio_databits = s_media_info.audio_databits[E_IPC_STREAM_AUDIO_MAIN];
    p_out->audio_channel = s_media_info.audio_channel[E_IPC_STREAM_AUDIO_MAIN];
    p_out->p_audio_buf = p_in->p_buf;
    p_out->buf_len = p_in->size;
    p_out->pts = p_in->pts;
    p_out->timestamp = p_in->timestamp;
}

STATIC VOID __TUYA_APP_ss_pb_event_cb(IN UINT_T pb_idx, IN SS_PB_EVENT_E pb_event, IN PVOID_T args)
{
    PR_DEBUG("ss pb rev event: %u %d", pb_idx, pb_event);
    if(pb_event == SS_PB_FINISH)
    {
        tuya_ipc_playback_send_finish(pb_idx);
    }
}

STATIC VOID __TUYA_APP_ss_pb_get_video_cb(IN UINT_T pb_idx, IN CONST MEDIA_FRAME_S *p_frame)
{
    TRANSFER_VIDEO_FRAME_S video_frame;
	memset(&video_frame, 0, sizeof(video_frame));
    __TUYA_APP_media_frame_TO_trans_video(p_frame, &video_frame);
    tuya_ipc_playback_send_video_frame(pb_idx, &video_frame);
}

STATIC VOID __TUYA_APP_ss_pb_get_audio_cb(IN UINT_T pb_idx, IN CONST MEDIA_FRAME_S *p_frame)
{
    TRANSFER_AUDIO_FRAME_S audio_frame;
	memset(&audio_frame, 0, sizeof(audio_frame));
    __TUYA_APP_media_frame_TO_trans_audio(p_frame, &audio_frame);
    tuya_ipc_playback_send_audio_frame(pb_idx, &audio_frame);
}

STATIC VOID __depereated_online_cb(IN TRANSFER_ONLINE_E status)
{

}

//#define TUYA_PB_DBG_PRNT_TIME
//回放录像回调
static void onPlayback(unsigned char *data, int size, Mp4DemuxerFrameInfo_s *pStMp4DemuxerFrameInfo, void *parm)
{
	if( (NULL == data) && (0 == size) && (NULL == pStMp4DemuxerFrameInfo) )
	{
		//文件回放结束
//		tuya_ipc_playback_send_finish(pb_client_index_g);
		return ;
	}
	
	if( (pb_status != 1) || (pb_client_index_g < 0) )
	{
		return ;
	}
	int ret = 0;
#ifdef TUYA_PB_DBG_PRNT_TIME
	struct timeval tv_before;
	struct timeval tv_after;
#endif

	if( 1==pStMp4DemuxerFrameInfo->iStreamType )
	{
		TRANSFER_VIDEO_FRAME_S video_frame;
		memset(&video_frame, 0, sizeof(TRANSFER_VIDEO_FRAME_S));
		
		video_frame.video_codec = TUYA_CODEC_VIDEO_H265;//s_media_info.video_codec[E_IPC_STREAM_VIDEO_MAIN];
		video_frame.video_frame_type = (1==pStMp4DemuxerFrameInfo->iFrameType) ? TY_VIDEO_FRAME_IFRAME : TY_VIDEO_FRAME_PBFRAME;
		video_frame.p_video_buf = (BYTE_T *)data;
		video_frame.buf_len = size;
		video_frame.pts = pStMp4DemuxerFrameInfo->ullTimestamp - (g_time_zone * 1000) - (g_time_dst * 1000); 		//utc microsecond
		video_frame.timestamp = video_frame.pts;

#ifdef TUYA_PB_DBG_PRNT_TIME
		gettimeofday(&tv_before, NULL);
#endif

        ret = tuya_ipc_playback_send_video_frame(pb_client_index_g, &video_frame);
		if( 0 != ret )
			AppErr("----------------------->play back send vedio error!!! ret: %d\n", ret);

#ifdef TUYA_PB_DBG_PRNT_TIME
		gettimeofday(&tv_after, NULL);
		unsigned long long s_ms = tv_before.tv_sec * 1000 + tv_before.tv_usec / 1000;
		unsigned long long e_ms = tv_after.tv_sec * 1000 + tv_after.tv_usec / 1000;
		printf("send pb video takes %lld ms\n", (e_ms - s_ms));
#endif
	}
	if( 2==pStMp4DemuxerFrameInfo->iStreamType )
	{
		//<shang>目前回放时一帧pcm是2048字节, 太大了, 所以要分两次投到涂鸦 SDK 的buffer
//		printf("pb audio frame len : %d\n", size);
		int remain_len = size;
		int send_len = 0;
		int frm_len;
		while(remain_len > 0)
		{
			frm_len = remain_len > 1024 ? 1024 : remain_len;
			TRANSFER_AUDIO_FRAME_S audio_frame;
			memset(&audio_frame, 0, sizeof(TRANSFER_AUDIO_FRAME_S));
			
			audio_frame.audio_codec = TUYA_CODEC_AUDIO_PCM;//s_media_info.audio_codec[E_IPC_STREAM_AUDIO_MAIN];
			audio_frame.audio_sample = TUYA_AUDIO_SAMPLE_8K;//s_media_info.audio_sample[E_IPC_STREAM_AUDIO_MAIN];
			audio_frame.audio_databits = TUYA_AUDIO_DATABITS_16;//s_media_info.audio_databits[E_IPC_STREAM_AUDIO_MAIN];
			audio_frame.audio_channel = TUYA_AUDIO_CHANNEL_MONO;//s_media_info.audio_channel[E_IPC_STREAM_AUDIO_MAIN];
			audio_frame.p_audio_buf = (BYTE_T *)data+send_len;
			audio_frame.buf_len = frm_len;
			audio_frame.pts = pStMp4DemuxerFrameInfo->ullTimestamp - (g_time_zone * 1000) - (g_time_dst * 1000); 		//utc microsecond
			audio_frame.timestamp = audio_frame.pts;

#ifdef TUYA_PB_DBG_PRNT_TIME
			gettimeofday(&tv_before, NULL);
#endif

			ret = tuya_ipc_playback_send_audio_frame(pb_client_index_g, &audio_frame);
			if( 0 != ret )
				AppErr("----------------------->play back send audio error!!!\n");

#ifdef TUYA_PB_DBG_PRNT_TIME
			gettimeofday(&tv_after, NULL);
			unsigned long long s_ms = tv_before.tv_sec * 1000 + tv_before.tv_usec / 1000;
			unsigned long long e_ms = tv_after.tv_sec * 1000 + tv_after.tv_usec / 1000;
			printf("send pb audio takes %lld ms\n", (e_ms - s_ms));
#endif

			remain_len -= frm_len;
			send_len += frm_len;
		}
	}
}

//等待SDK内部缓存是否发送完成
static void* thread_wait_dl_send_over(void* args)
{
	int isSendOver;
	AppInfo("thread_wait_dl_send_over.\n");
	
	while( dl_client_index_g >= 0 )
	{
		//查询SDK内部缓存是否发送完成
		isSendOver = tuya_ipc_4_app_download_is_send_over(dl_client_index_g);
		printf("tuya_ipc_4_app_download_is_send_over ---> %d\n"), isSendOver;
		if( isSendOver )
		{
			tuya_ipc_4_app_download_status(dl_client_index_g, 100);
			break;
		}
		sleep(1);
	}
	return NULL;
}

//#define TUYA_DL_DBG_PRNT_TIME
//下载录像回调
static void onDownload(unsigned char *data, int size, Mp4DemuxerFrameInfo_s *pStMp4DemuxerFrameInfo, void *parm)
{
	if( (NULL == data) && (0 == size) && (NULL == pStMp4DemuxerFrameInfo) )
	{
		//文件回放结束
		//查询SDK内部缓存是否发送完成
		int isSendOver = tuya_ipc_4_app_download_is_send_over(dl_client_index_g);
		printf("tuya_ipc_4_app_download_is_send_over ---> %d\n"), isSendOver;

		if( isSendOver )
		{
			tuya_ipc_4_app_download_status(dl_client_index_g, 100);
		}
		else
		{
			CreateDetachedThread((char*)"thread_wait_dl_send_over",thread_wait_dl_send_over, (void *)NULL, true);
		}
		
		return ;
	}

#ifdef TUYA_DL_DBG_PRNT_TIME
	struct timeval tv_before;
	struct timeval tv_after;
#endif

	int ret = 0;
	STORAGE_FRAME_HEAD_S stHead;
	if(2 == pStMp4DemuxerFrameInfo->iStreamType)
		stHead.type = E_AUDIO_FRAME;
	else if(1 == pStMp4DemuxerFrameInfo->iFrameType)
		stHead.type = E_VIDEO_I_FRAME;
	else
		stHead.type = E_VIDEO_PB_FRAME;
	stHead.size = size;
	stHead.timestamp = pStMp4DemuxerFrameInfo->ullTimestamp - (g_time_zone * 1000) - (g_time_dst * 1000); 		//utc microsecond
	stHead.pts = stHead.timestamp;
	//printf("stHead.type : %d, size : %u, pts : %llu\n", stHead.type, stHead.size, stHead.pts);
	
	if( dl_client_index_g >= 0 )
	{
#ifdef TUYA_DL_DBG_PRNT_TIME
		gettimeofday(&tv_before, NULL);
#endif

		ret = tuya_ipc_4_app_download_data(dl_client_index_g, &stHead, (CONST CHAR_T *)data);
		if( 0 != ret )
			AppErr("----------------------->tuya_ipc_4_app_download_data error!!!\n");
		
#ifdef TUYA_DL_DBG_PRNT_TIME
		gettimeofday(&tv_after, NULL);
		unsigned long long s_ms = tv_before.tv_sec * 1000 + tv_before.tv_usec / 1000;
		unsigned long long e_ms = tv_after.tv_sec * 1000 + tv_after.tv_usec / 1000;
		printf("send dl data takes %lld ms\n", (e_ms - s_ms));
#endif
	}
}


int speakcount = 0;

/* 传输事件回调 */
INT_T __TUYA_APP_p2p_event_cb(IN CONST TRANSFER_EVENT_E event, IN CONST PVOID_T args)
{
    PR_DEBUG("p2p rev event cb=[%d] ", event);
	AppInfo("--------------------------------p2p event : %d--------------------------------\n\n", event);
	if(args == NULL)
    {
        PR_DEBUG("p2p rev event args null ");
        return -1;
    }
    switch (event)
    {
        case TRANS_LIVE_VIDEO_START:
        {
            C2C_TRANS_CTRL_VIDEO_START * parm = (C2C_TRANS_CTRL_VIDEO_START *)args;
            PR_DEBUG("chn[%u] type[%d]video start",parm->channel,parm->type);
			
			CaptureForceIFrame(0,0);
			CaptureForceIFrame(1,0);
            break;
        }
        case TRANS_LIVE_VIDEO_STOP:
        {
            C2C_TRANS_CTRL_VIDEO_STOP * parm = (C2C_TRANS_CTRL_VIDEO_STOP *)args;
            PR_DEBUG("chn[%u] type[%d] video stop",parm->channel,parm->type);
            break;
        }
        case TRANS_LIVE_AUDIO_START:
        {
            C2C_TRANS_CTRL_AUDIO_START * parm = (C2C_TRANS_CTRL_AUDIO_START *)args;
            PR_DEBUG("chn[%u] audio start",parm->channel);
            TRANSFER_EVENT_RETURN_E ret = TRANS_EVENT_SUCCESS;
			return ret;
        }
        case TRANS_LIVE_AUDIO_STOP:
        {
            C2C_TRANS_CTRL_AUDIO_STOP * parm = (C2C_TRANS_CTRL_AUDIO_STOP *)args;
            PR_DEBUG("chn[%u] audio stop",parm->channel);
            TRANSFER_EVENT_RETURN_E ret = TRANS_EVENT_SUCCESS;
			return ret;
        }
        case TRANS_SPEAKER_START:
        {
            TRANSFER_SOURCE_TYPE_E type = *(TRANSFER_SOURCE_TYPE_E *)args;
            PR_DEBUG("enbale audio speaker by %d",type);
            TUYA_APP_Enable_Speaker_CB(TRUE);
            TRANSFER_EVENT_RETURN_E ret = TRANS_EVENT_SUCCESS;
            //if start failed, return TRANS_EVENT_SPEAKER_ISUSED/TRANS_EVENT_SPEAKER_REPSTART
            return ret;
        }
        case TRANS_SPEAKER_STOP:
        {
            TRANSFER_SOURCE_TYPE_E type = *(TRANSFER_SOURCE_TYPE_E *)args;
            PR_DEBUG("disable audio speaker by %d",type);
            TUYA_APP_Enable_Speaker_CB(FALSE);
            TRANSFER_EVENT_RETURN_E ret = TRANS_EVENT_SUCCESS;
            //if start failed, return TRANS_EVENT_SPEAKER_STOPFAILED
            return ret;
        }
        case TRANS_LIVE_LOAD_ADJUST:
        {
            C2C_TRANS_LIVE_LOAD_PARAM_S *quality = (C2C_TRANS_LIVE_LOAD_PARAM_S *)args;
            PR_DEBUG("live quality %d -> %d", quality->curr_load_level, quality->new_load_level);
            break;
        }
        case TRANS_PLAYBACK_LOAD_ADJUST:
        {
            C2C_TRANS_PB_LOAD_PARAM_S *quality= (C2C_TRANS_PB_LOAD_PARAM_S *)args;
            PR_DEBUG("pb idx:%d quality %d -> %d", quality->client_index, quality->curr_load_level, quality->new_load_level);
            break;
        }
        case TRANS_ABILITY_QUERY:
        {
            C2C_TRANS_QUERY_FIXED_ABI_REQ * pAbiReq;
            pAbiReq = (C2C_TRANS_QUERY_FIXED_ABI_REQ *)args;
            pAbiReq->ability_mask = TY_CMD_QUERY_IPC_FIXED_ABILITY_TYPE_VIDEO |
                                    TY_CMD_QUERY_IPC_FIXED_ABILITY_TYPE_SPEAKER |
                                    TY_CMD_QUERY_IPC_FIXED_ABILITY_TYPE_MIC;          
            break;
        }
        case TRANS_PLAYBACK_QUERY_MONTH_SIMPLIFY:
        {
			AppInfo("=====================TRANS_PLAYBACK_QUERY_MONTH_SIMPLIFY====================\n");

            C2C_TRANS_QUERY_PB_MONTH_REQ *p = (C2C_TRANS_QUERY_PB_MONTH_REQ *)args;
            PR_DEBUG("pb query by month: %d-%d", p->year, p->month);

			p->day = g_StorageManager->CheckRecordOnMonth(p->year, p->month);

			AppInfo("------------------------------------->>>p->day = %08x\n", p->day);

            break;
        }
        case TRANS_PLAYBACK_QUERY_DAY_TS:
        {
#ifdef RC0240_LGV10
			AppInfo("=====================TRANS_PLAYBACK_QUERY_DAY_TS====================\n");
			
            C2C_TRANS_QUERY_PB_DAY_RESP *pquery = (C2C_TRANS_QUERY_PB_DAY_RESP *)args;
            PR_DEBUG("pb_ts query by day: idx[%d]%d-%d-%d", pquery->channel,pquery->year, pquery->month, pquery->day);
			
			int j = 0;
			TRecordFileTimeList ListSearchResult;
			TRecordFileTimeList ListAlarmRecTime;
			TRecordFileTimeList::iterator	it;
			TRecordFileTimeList::iterator	alarm_it;
			ListSearchResult.clear();
			ListAlarmRecTime.clear();
			g_StorageManager->SearchRecordByDay(pquery->year, pquery->month, pquery->day, ListSearchResult, ListAlarmRecTime);
			AppInfo("------------------------------------->>>record [%d]\n", ListSearchResult.size());

//			PLAY_BACK_ALARM_INFO_ARR * pResult = (PLAY_BACK_ALARM_INFO_ARR *)malloc(sizeof(PLAY_BACK_ALARM_INFO_ARR) + (ListSearchResult.size() + ListAlarmRecTime.size()) * sizeof(PLAY_BACK_ALARM_FRAGMENT));
			int file_count = ListSearchResult.size() + ListAlarmRecTime.size() * 2;
			PLAY_BACK_ALARM_INFO_ARR * pResult = (PLAY_BACK_ALARM_INFO_ARR *)malloc(sizeof(PLAY_BACK_ALARM_INFO_ARR) + file_count * sizeof(PLAY_BACK_ALARM_FRAGMENT));
			if (NULL == pResult)
			{
				AppErr("malloc failed \n");
				pquery->alarm_arr = NULL;
				break;
			}

//			pResult->file_count = ListSearchResult.size() + ListAlarmRecTime.size();
//			pResult->file_count = file_count;

			alarm_it = ListAlarmRecTime.begin();
			for(it = ListSearchResult.begin(); it != ListSearchResult.end(); it++)
			{
				#if 0
				unsigned int uiStartTime = ( it->iStartTime > st ) ? it->iStartTime : st; 	//过滤
				unsigned int uiEndTime = ( it->iEndTime < et ) ? it->iEndTime : et;
				#else 	//<shang>底层接口已过滤
				#endif
				
				pResult->file_arr[j].type = it->iRecType;
				pResult->file_arr[j].time_sect.start_timestamp	= it->iStartTime - g_time_zone - g_time_dst;	//本地时间转成UTC时间

				for (; alarm_it != ListAlarmRecTime.end() && 0 == it->iRecType && 
					alarm_it->iStartTime >= it->iStartTime && alarm_it->iEndTime <= it->iEndTime; alarm_it++)
				{
					pResult->file_arr[j].time_sect.end_timestamp	= alarm_it->iStartTime - g_time_zone - g_time_dst;	//本地时间转成UTC时间
					j++;
					pResult->file_arr[j].type = alarm_it->iRecType;
					pResult->file_arr[j].time_sect.start_timestamp	= alarm_it->iStartTime - g_time_zone - g_time_dst;	//本地时间转成UTC时间
					pResult->file_arr[j].time_sect.end_timestamp	= alarm_it->iEndTime - g_time_zone - g_time_dst;	//本地时间转成UTC时间
					j++;
					pResult->file_arr[j].type = it->iRecType;
					pResult->file_arr[j].time_sect.start_timestamp	= alarm_it->iEndTime - g_time_zone - g_time_dst;	//本地时间转成UTC时间
				}

				pResult->file_arr[j].time_sect.end_timestamp 	= it->iEndTime - g_time_zone - g_time_dst; 	//本地时间转成UTC时间
				
//				AppInfo("<record>---------start_timestamp[%d] = %d\n", j, pResult->file_arr[j].time_sect.start_timestamp);
//				AppInfo("<record>-----------end_timestamp[%d] = %d\n", j, pResult->file_arr[j].time_sect.end_timestamp);

				j++;
			}
			
//			for(it = ListAlarmRecTime.begin(); it != ListAlarmRecTime.end(); it++)
//			{
//				pResult->file_arr[j].type = it->iRecType;
//				pResult->file_arr[j].time_sect.start_timestamp 	= it->iStartTime - g_time_zone - g_time_dst; 	//本地时间转成UTC时间
//				pResult->file_arr[j].time_sect.end_timestamp 	= it->iEndTime - g_time_zone - g_time_dst; 	//本地时间转成UTC时间
//				j++;
//			}
			
			pResult->file_count = j;
			pquery->alarm_arr = pResult;
#else
			AppInfo("=====================TRANS_PLAYBACK_QUERY_DAY_TS====================\n");
			
            C2C_TRANS_QUERY_PB_DAY_RESP *pquery = (C2C_TRANS_QUERY_PB_DAY_RESP *)args;
            PR_DEBUG("pb_ts query by day: idx[%d]%d-%d-%d", pquery->channel,pquery->year, pquery->month, pquery->day);
			
			int j = 0;
			TRecordFileTimeList ListSearchResult;
			TRecordFileTimeList::iterator	it;
			ListSearchResult.clear();
			g_StorageManager->SearchRecordByDay(pquery->year, pquery->month, pquery->day, ListSearchResult);
			AppInfo("------------------------------------->>>record [%d]\n", ListSearchResult.size());

			PLAY_BACK_ALARM_INFO_ARR * pResult = (PLAY_BACK_ALARM_INFO_ARR *)malloc(sizeof(PLAY_BACK_ALARM_INFO_ARR) + ListSearchResult.size() * sizeof(PLAY_BACK_ALARM_FRAGMENT));
			if (NULL == pResult)
			{
				 AppErr("malloc failed \n");
				 pquery->alarm_arr = NULL;
				 break;
			}

			pResult->file_count = ListSearchResult.size();

			for(it = ListSearchResult.begin(); it != ListSearchResult.end(); it++)
			{
				#if 0
				unsigned int uiStartTime = ( it->iStartTime > st ) ? it->iStartTime : st; 	//过滤
				unsigned int uiEndTime = ( it->iEndTime < et ) ? it->iEndTime : et;
				#else 	//<shang>底层接口已过滤
				unsigned int uiStartTime = it->iStartTime; 	//过滤
				unsigned int uiEndTime = it->iEndTime;
				#endif
				pResult->file_arr[j].time_sect.start_timestamp 	= uiStartTime - g_time_zone - g_time_dst; 	//本地时间转成UTC时间
				pResult->file_arr[j].time_sect.end_timestamp 	= uiEndTime - g_time_zone - g_time_dst; 	//本地时间转成UTC时间
				
//				AppInfo("<record>---------start_timestamp[%d] = %d\n", j, pResult->file_arr[j].time_sect.start_timestamp);
//				AppInfo("<record>-----------end_timestamp[%d] = %d\n", j, pResult->file_arr[j].time_sect.end_timestamp);

				j++;
			}
			
			pquery->alarm_arr = pResult;
#endif
            break;
        }
        case TRANS_PLAYBACK_START_TS:
        {
			static struct tm s_lastTm = {0};

			AppInfo("=====================TRANS_PLAYBACK_START_TS====================\n");

            /* 开始回放时client会带上开始时间点，这里简单起见，只进行了日志打印 */
            C2C_TRANS_CTRL_PB_START *pParam = (C2C_TRANS_CTRL_PB_START *)args;
            PR_DEBUG("PB StartTS idx:%d %u [%u %u]", pParam->channel, pParam->playTime, pParam->time_sect.start_timestamp, pParam->time_sect.end_timestamp);

			//发现现在涂鸦是按时间段下发

			pb_status = 1;
			pb_client_index_g = pParam->channel;

			time_t utcStartTime =  pParam->playTime + g_time_zone + g_time_dst;		//	UTC时间换算成本地时间
			time_t utcEndTime/* =  pParam->time_sect.end_timestamp + g_time_zone + g_time_dst*/;

			struct tm curTm = {0};
			localtime_r((time_t*)&utcStartTime, &curTm);
			AppInfo("utcStartTime : %d-%d-%d %d:%d:%d\n", curTm.tm_year+1900, curTm.tm_mon+1, curTm.tm_mday, 
															curTm.tm_hour, curTm.tm_min, curTm.tm_sec);
			{
				SystemTime stEndTime;
				
				stEndTime.year		= curTm.tm_year+1900;
				stEndTime.month 	= curTm.tm_mon+1;
				stEndTime.day		= curTm.tm_mday;
				stEndTime.hour		= 23;
				stEndTime.minute	= 59;
				stEndTime.second	= 59;
				
				utcEndTime = SystemTimeToSecond(&stEndTime);
				utcEndTime += 1;	//再加 1 s
			}

			AppInfo("utcStartTime =%u\n",utcStartTime);
			AppInfo("utcEndTime   =%u\n",utcEndTime);

			AppInfo("pb_handler_g  =%d\n", pb_handler_g);
			AppInfo("pb_status  =%d\n", pb_status);

			//第一次start的时候收到的开始时间一定是当天录像的最早时间,现在的策略是start的时候直接指定当天最早时间到当天结束

			if( pb_handler_g >= 0 )
			{
				if( (curTm.tm_year != s_lastTm.tm_year) || 
					(curTm.tm_mon != s_lastTm.tm_mon) || 
					(curTm.tm_mday != s_lastTm.tm_mday) )
				{
					g_StorageManager->StopPlaybackOnTime(pb_handler_g);
					pb_handler_g = g_StorageManager->StartPlaybackOnTime(utcStartTime, utcEndTime, onPlayback, NULL);
					s_lastTm = curTm;
				}
				else
				{
					g_StorageManager->SeekTimeOnTime(pb_handler_g, utcStartTime);
				}
			}
			else
			{
				pb_handler_g = g_StorageManager->StartPlaybackOnTime(utcStartTime, utcEndTime, onPlayback, NULL);
				s_lastTm = curTm;
			}
			
            break;
        }
        case TRANS_PLAYBACK_PAUSE:
        {
			AppInfo("=====================TRANS_PLAYBACK_PAUSE====================\n");
			
            C2C_TRANS_CTRL_PB_PAUSE *pParam = (C2C_TRANS_CTRL_PB_PAUSE *)args;
            PR_DEBUG("PB Pause idx:%d", pParam->channel);

			pb_status = 2;
			g_StorageManager->PausePlay(pb_handler_g, true);

            break;
        }
        case TRANS_PLAYBACK_RESUME:
        {
			AppInfo("=====================TRANS_PLAYBACK_RESUME====================\n");

            C2C_TRANS_CTRL_PB_RESUME *pParam = (C2C_TRANS_CTRL_PB_RESUME *)args;
            PR_DEBUG("PB Resume idx:%d", pParam->channel);

			pb_status = 1;
			g_StorageManager->PausePlay(pb_handler_g, false);

            break;
        }
        case TRANS_PLAYBACK_MUTE:
        {
			AppInfo("=====================TRANS_PLAYBACK_MUTE====================\n");
			
            C2C_TRANS_CTRL_PB_MUTE *pParam = (C2C_TRANS_CTRL_PB_MUTE *)args;
            PR_DEBUG("PB idx:%d mute", pParam->channel);

			pb_audio_muted = SS_PB_MUTE;

            break;
        }
        case TRANS_PLAYBACK_UNMUTE:
        {
			AppInfo("=====================TRANS_PLAYBACK_UNMUTE====================\n");
			
            C2C_TRANS_CTRL_PB_UNMUTE *pParam = (C2C_TRANS_CTRL_PB_UNMUTE *)args;
            PR_DEBUG("PB idx:%d unmute", pParam->channel);

			pb_audio_muted = SS_PB_UN_MUTE;

            break;
        }
        case TRANS_PLAYBACK_STOP:
        {
			AppInfo("=====================TRANS_PLAYBACK_STOP====================\n");
			
            C2C_TRANS_CTRL_PB_STOP *pParam = (C2C_TRANS_CTRL_PB_STOP *)args;
            PR_DEBUG("PB Stop idx:%d", pParam->channel);

			pb_status = 0;
			g_StorageManager->StopPlaybackOnTime(pb_handler_g);
			pb_handler_g = -1;
			pb_client_index_g = -1;

            break;
        }
		case TRANS_PLAYBACK_SET_SPEED:
		{
			AppInfo("=====================TRANS_PLAYBACK_SET_SPEED====================\n");
			C2C_TRANS_CTRL_PB_SET_SPEED *pParam = (C2C_TRANS_CTRL_PB_SET_SPEED *)args;
			PR_DEBUG("chn[%u] video set speed[%d]\n", pParam->channel, pParam->speed);
			/* typedef enum{
				TY_SPEED_05TIMES = 0,			   // 0.5倍速
				TY_SPEED_10TIMES = 1 ,			   // 正常速度
				TY_SPEED_20TIMES = 3,			   // 2.0倍
					TY_SPEED_MAX,
			}TY_PLAYBACK_SPEED; */

			float fSpeed;
			if(TY_SPEED_05TIMES == pParam->speed)
				fSpeed = 0.5;
			else if(TY_SPEED_10TIMES == pParam->speed)
				fSpeed = 1.0;
			else if(TY_SPEED_20TIMES == pParam->speed)
				fSpeed = 2.0;
			else
				break;
			
			g_StorageManager->SetPlaybackSpeed(pb_handler_g, fSpeed);
			
			break;
		}
        case TRANS_LIVE_VIDEO_CLARITY_SET: 			///< 切换清晰度时并未收到此消息
        {
            C2C_TRANS_LIVE_CLARITY_PARAM_S *pParam = (C2C_TRANS_LIVE_CLARITY_PARAM_S *)args;
            PR_DEBUG("set clarity:%d", pParam->clarity);
            if((pParam->clarity == TY_VIDEO_CLARITY_STANDARD)||(pParam->clarity == TY_VIDEO_CLARITY_HIGH))
            {
                PR_DEBUG("set clarity:%d OK", pParam->clarity);
                s_p2p_mgr.live_clarity = pParam->clarity;
				
				if( pParam->clarity == TY_VIDEO_CLARITY_STANDARD )
				{
					CaptureForceIFrame(1,0);
				}
				else
				{
					CaptureForceIFrame(0,0);
				}
            }
            break;
        }
        case TRANS_LIVE_VIDEO_CLARITY_QUERY:
        {
            C2C_TRANS_LIVE_CLARITY_PARAM_S *pParam = (C2C_TRANS_LIVE_CLARITY_PARAM_S *)args;
            pParam->clarity = s_p2p_mgr.live_clarity;
            PR_DEBUG("query larity:%d", pParam->clarity);
            break;
        }
        case TRANS_DOWNLOAD_START:
        {
			printf("=====================TRANS_DOWNLOAD_START====================\n");
            C2C_TRANS_CTRL_DL_START *pParam = (C2C_TRANS_CTRL_DL_START *)args;
			
			AppInfo("channel           : %u\n", pParam->channel);//0
			AppInfo("fileNum           : %u\n", pParam->fileNum);//1
			AppInfo("downloadStartTime : %u\n", pParam->downloadStartTime);//1637666391
			AppInfo("downloadEndTime   : %u\n", pParam->downloadEndTime);//1637666691
			for( int i = 0; i < pParam->fileNum; i++ )
			{
				AppInfo("pFileInfo[%d]:\n", i);
				AppInfo("\tstart_timestamp : %u\n", pParam->pFileInfo->start_timestamp);//1637666169
				AppInfo("\tend_timestamp : %u\n", pParam->pFileInfo->end_timestamp);//1637666781
			}
			
            AppInfo("PB Download StartTS idx:%d [%u %u]", pParam->channel, pParam->downloadStartTime, pParam->downloadEndTime);

			time_t utcStartTime =  pParam->downloadStartTime + g_time_zone + g_time_dst; 	//	UTC时间换算成本地时间
			time_t utcEndTime =  pParam->downloadEndTime + g_time_zone + g_time_dst;

			AppInfo("utcStartTime = %u\n", utcStartTime);
			AppInfo("utcEndTime   = %u\n", utcEndTime);

			AppInfo("dl_handler_g  =%d\n", dl_handler_g);

			dl_client_index_g = pParam->channel;

			if( dl_handler_g >= 0 )
			{
				g_StorageManager->StopDownload(dl_handler_g);
			}
			dl_handler_g = g_StorageManager->StartDownload(utcStartTime, utcEndTime, onDownload, NULL);
			
            break;
        }
        case TRANS_DOWNLOAD_STOP:
        {
			printf("=====================TRANS_DOWNLOAD_STOP====================\n");
            C2C_TRANS_CTRL_DL_STOP *pParam = (C2C_TRANS_CTRL_DL_STOP *)args;
			g_StorageManager->StopDownload(dl_handler_g);
            break;
        }
        case TRANS_DOWNLOAD_PAUSE:
        {
			printf("=====================TRANS_DOWNLOAD_PAUSE====================\n");
            C2C_TRANS_CTRL_DL_PAUSE *pParam = (C2C_TRANS_CTRL_DL_PAUSE *)args;
			g_StorageManager->PauseDownload(dl_handler_g, true);
            break;
        }
        case TRANS_DOWNLOAD_RESUME:
        {
			printf("=====================TRANS_DOWNLOAD_RESUME====================\n");
            C2C_TRANS_CTRL_DL_RESUME *pParam = (C2C_TRANS_CTRL_DL_RESUME *)args;
			g_StorageManager->PauseDownload(dl_handler_g, false);
            break;
        }
        case TRANS_DOWNLOAD_CANCLE:
        {
			printf("=====================TRANS_DOWNLOAD_CANCLE====================\n");
            C2C_TRANS_CTRL_DL_CANCLE *pParam = (C2C_TRANS_CTRL_DL_CANCLE *)args;
			g_StorageManager->StopDownload(dl_handler_g);
            break;
        }
        case TRANS_PLAYBACK_DELETE:
        {
			AppInfo("=====================TRANS_PLAYBACK_DELETE====================\n");
            C2C_TRANS_CTRL_PB_DELDATA_BYDAY_REQ *pParam = (C2C_TRANS_CTRL_PB_DELDATA_BYDAY_REQ *)args;
			printf("delete ---> [%u] %u-%u-%u\n", pParam->channel, pParam->year, pParam->month, pParam->day);

			//先停止回放
			pb_status = 0;
			g_StorageManager->StopPlaybackOnTime(pb_handler_g);
			pb_handler_g = -1;
			pb_client_index_g = -1;

			//先停止下载
			g_StorageManager->StopDownload(dl_handler_g);
			dl_handler_g = -1;
			dl_client_index_g = -1;
			
			g_StorageManager->DeleteRecordByDay(pParam->year, pParam->month, pParam->day);
			tuya_ipc_delete_video_finish(pParam->channel);

            break;
        }
        default:
            break;
    }
    return 0;
}

VOID __TUYA_APP_rev_audio_cb(IN CONST TRANSFER_AUDIO_FRAME_S *p_audio_frame, IN CONST TRANSFER_SOURCE_TYPE_E src_type)
{
    MEDIA_FRAME_S audio_frame;
	memset(&audio_frame, 0, sizeof(audio_frame));
    audio_frame.p_buf = p_audio_frame->p_audio_buf;
    audio_frame.size = p_audio_frame->buf_len;

 //   PR_TRACE("Rev Audio. size:%u audio_codec:%d audio_sample:%d audio_databits:%d audio_channel:%d",p_audio_frame->buf_len,
  //           p_audio_frame->audio_codec, p_audio_frame->audio_sample, p_audio_frame->audio_databits, p_audio_frame->audio_channel);

    TUYA_APP_Rev_Audio_CB( &audio_frame, TUYA_AUDIO_SAMPLE_8K, TUYA_AUDIO_DATABITS_16, TUYA_AUDIO_CHANNEL_MONO);
}

OPERATE_RET TUYA_APP_Enable_P2PTransfer(IN UINT_T max_users)
{
    if(s_p2p_mgr.enabled == TRUE)
    {
        PR_DEBUG("P2P Is Already Inited");
        return OPRT_OK;
    }

    PR_DEBUG("Init P2P With Max Users:%u", max_users);

    s_p2p_mgr.enabled = TRUE;
    s_p2p_mgr.max_users = max_users;
    s_p2p_mgr.p2p_audio_codec = s_media_info.audio_codec[E_IPC_STREAM_AUDIO_MAIN];

    TUYA_IPC_TRANSFER_VAR_S p2p_var = {0};
    p2p_var.online_cb = __depereated_online_cb;
    p2p_var.on_rev_audio_cb = __TUYA_APP_rev_audio_cb;
    /*speak data format  app->ipc*/
    p2p_var.rev_audio_codec = TUYA_CODEC_AUDIO_G711U;
    p2p_var.audio_sample = TUYA_AUDIO_SAMPLE_8K;
    p2p_var.audio_databits = TUYA_AUDIO_DATABITS_16;
    p2p_var.audio_channel = TUYA_AUDIO_CHANNEL_MONO;
    /*end*/
    p2p_var.on_event_cb = __TUYA_APP_p2p_event_cb;
    p2p_var.live_quality = TRANS_LIVE_QUALITY_MAX;
    p2p_var.max_client_num = max_users;
    memcpy(&p2p_var.AVInfo,&s_media_info,sizeof(IPC_MEDIA_INFO_S));
    tuya_ipc_tranfser_init(&p2p_var);

    return OPRT_OK;
}


