#ifndef __MP4_DEMUXER_H__
#define __MP4_DEMUXER_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "StorageCommon.h"


#ifdef  __cplusplus
extern "C" {
#endif
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libavutil/opt.h"
#include "libavutil/timestamp.h"
#include "libswresample/swresample.h"
#ifdef  __cplusplus
}
#endif


typedef struct  
{
	int audio_object_type;  
	int sampling_frequency_index;
	int channel_configuration;  
} AAC_ADTS_Param_s;  


typedef struct  
{
	int iStreamType; 					//流类型, 1-视频, 2-音频
	int iFrameType; 					//帧类型, 1-I帧, 2-P/B帧
	unsigned long long ullTimestamp; 	//时间戳
	int iCodeType; 						//编码类型
} Mp4DemuxerFrameInfo_s;  


class CMp4Demuxer
{
public:
	CMp4Demuxer();
	~CMp4Demuxer();

	/*
	 *@return 0 if OK, < 0 on error
	 */
	int Open(const char *pFile, STORAGE_VIDEO_ENC_TYPE_E eVideoEncType);
	
	/*
	 *@param iPercent seek文件的百分比, 范围[0, 100]
	 *@return >= 0 on success, < 0 on error
	 */
	int Seek(float fPercent);
	
	/*
	 *@return >= 0 on success, < 0 on error
	 */
	int Seek(long long llTime);
	
	/*
	 *@return 成功返回数据帧的实际长度, < 0 on error
	 */
	int Read(unsigned char *pBuffer, int iBufferSize, Mp4DemuxerFrameInfo_s *pFrameInfo);
	int Close();

private:
	int Decode_AAC_extradata(AAC_ADTS_Param_s *pstAAC_ADTS_Param, unsigned char *pExtradata, int iExtraDataSize);	
	int Create_AAC_ADTS_Context(AAC_ADTS_Param_s *pstAAC_ADTS_Param, int frame_data_len, unsigned char *pbuf, int bufsize);

private:
	AVFormatContext *m_pAVFmtCtx;
	AVPacket *m_pAVPacket;

	AVCodecContext *m_pAVCodecCtx_pcm;
	SwrContext *m_pSwrCtx_pcm;
	AVFrame *m_pAVFrame_pcm;
	int m_iPcmBufferSize;
	int m_iPcmBufferNbSamples;
	uint8_t *m_pPcmBuffer;
	int m_iPcmChannelNb;
	int m_iPcmSampleFmt;

	AVBitStreamFilterContext* m_pAVBSFC_h264;
	AVBitStreamFilterContext* m_pAVBSFC_aac;

	int m_iVideoindex;
	int m_iAudioindex;

	AAC_ADTS_Param_s m_stAAC_ADTS_Param;
};


#endif 		//__MP4_DEMUXER_H__
