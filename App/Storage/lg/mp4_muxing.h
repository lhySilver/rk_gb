#ifndef __MP4_MUXING_H__
#define __MP4_MUXING_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>


#ifdef  __cplusplus
extern "C" {
#endif
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libavutil/opt.h"
#ifdef  __cplusplus
}
#endif



typedef struct
{
	AVFormatContext *ofmt_ctx;
	AVBSFContext *a_bsf_ctx;
	pthread_mutex_t mutex;
	int64_t v_start_time;
	int64_t a_start_time;
	int wait_IDR;
} mp4_ctx_s;



class CMp4Muxer
{		
public:
	
	CMp4Muxer();
	~CMp4Muxer();

	int mp4_mux_init(AVCodecContext *v_codec, AVCodecContext *a_codec, int v_frame_rate, const char *outMP4FileName);
	int mp4_mux_uninit(void);
	int mp4_mux_write(AVPacket *packet, enum AVMediaType codec_type);

	bool IsOpenFile();

private:
	
	int copy_codec_params_4_video(AVCodecContext *i_codec, AVCodecContext *o_codec);
	
	int copy_codec_params_4_audio(AVCodecContext *i_codec, AVCodecContext *o_codec);
	
	AVStream *get_video_stream(AVFormatContext *ctx);
	
	AVStream *get_audio_stream(AVFormatContext *ctx);

public:
	static void InitLib();

private:
	static bool m_bIsInitedLib;

	mp4_ctx_s m_stMp4Ctx;
	
};


#endif //__MP4_MUXING_H__

