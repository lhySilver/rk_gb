#ifndef __RINGBUFFER_H__
#define __RINGBUFFER_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>


#pragma pack(1)
typedef struct
{
	int iStreamType; 					//流类型, 1-视频, 2-音频
	int iFrameType; 					//帧类型, 1-I帧, 2-P/B帧
	unsigned long long ullTimestamp; 	//时间戳
	int iCodeType; 						//编码类型
}stream_data_header_S;
#pragma pack()

#pragma pack(1)
typedef struct
{
	int iStreamType; 					//流类型, 1-视频, 2-音频
	int iFrameType; 					//帧类型, 1-I帧, 2-P帧
	unsigned long long ullTimestamp; 	//时间戳
	int iCodeType; 						//编码类型
	int iDataLen; 						//帧数据长度
//	char *pData; 						//帧数据指针
}ring_buffer_data_header_S;
#pragma pack()



class CRingBuffer
{
private:
	bool m_bInit;
	int m_iBitrate_kbps;
	int m_iFps;
	int m_iMaxBufferSeconds;
	int m_iReadPos;
	int m_iWritePos;
	int m_iBufferLen;
	char *m_pBuffer;
	bool m_bBufferFull; 		//标识 buffer 是否是满的, 在 m_iReadPos = m_iWritePos 的时候, buffer可能是满的, 也可能是空的, 所以用 m_bBufferFull 来标识
	pthread_mutex_t m_mutex;;

public:
	CRingBuffer();
	~CRingBuffer();

	int Init(int bitrate_kbps, int fps, int max_buffer_seconds);
	int UnInit();

	int PutData(stream_data_header_S *pStreamDataHeader, const char *pData, int iDataLen);
	int GetData(stream_data_header_S *pStreamDataHeader, char *pData, int max_buff_size, bool bThrow = false);

	int ClearData();

	int GetBufferLen();
	int GetFreeBufferLen();
	
	int GetStreamDuration();
	/*
	 *@ brief 丢弃 ullTimestamp_ms 之前最近的一个 I 帧之前所有的数据
	 */
	int DiscardDataOnTime(unsigned long long ullTimestamp_ms);
	
};



#endif 	//__RINGBUFFER_H__
