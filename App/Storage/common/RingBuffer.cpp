#include <errno.h>
#include "RingBuffer.h"
#include "Log/DebugDef.h"


CRingBuffer::CRingBuffer()
{
	m_bInit = false;
	m_iBitrate_kbps = 0;
	m_iFps = 0;
	m_iMaxBufferSeconds = 0;	
	m_iReadPos = 0;
	m_iWritePos = 0;
	m_iBufferLen = 0;
	m_pBuffer = NULL;
	m_bBufferFull = false;
	int ret = pthread_mutex_init(&m_mutex, NULL);
	if (0 != ret)
		printf("%s %d pthread_mutex_init: %d, errno: %d\n", __FILE__, __LINE__, ret, errno);
}

CRingBuffer::~CRingBuffer()
{
	int ret = pthread_mutex_destroy(&m_mutex);
	if (0 != ret)
		printf("%s %d pthread_mutex_destroy: %d, errno: %d\n", __FILE__, __LINE__, ret, errno);
}

int CRingBuffer::Init(int bitrate_kbps, int fps, int max_buffer_seconds)
{
	int ret;
	ret = pthread_mutex_lock(&m_mutex);
	if (0 != ret)
		printf("%s %d pthread_mutex_lock: %d, errno: %d\n", __FILE__, __LINE__, ret, errno);

	if( m_bInit )
	{
		ret = pthread_mutex_unlock(&m_mutex);
		if (0 != ret)
			printf("%s %d pthread_mutex_unlock: %d, errno: %d\n", __FILE__, __LINE__, ret, errno);
		return 0;
	}

	m_iBufferLen = ( (bitrate_kbps*1024/8) + (sizeof(ring_buffer_data_header_S)*fps) ) * max_buffer_seconds;
	AppInfo("m_iBufferLen : %d\n", m_iBufferLen);

	m_pBuffer = new char [m_iBufferLen];
	if( NULL == m_pBuffer )
	{
		m_iBufferLen = 0;
		ret = pthread_mutex_unlock(&m_mutex);
		if (0 != ret)
			printf("%s %d pthread_mutex_unlock: %d, errno: %d\n", __FILE__, __LINE__, ret, errno);
		return -1;
	}

	m_iBitrate_kbps = bitrate_kbps;
	m_iFps = fps;
	m_iMaxBufferSeconds = max_buffer_seconds;

	m_bInit = true;
	
	ret = pthread_mutex_unlock(&m_mutex);
	if (0 != ret)
		printf("%s %d pthread_mutex_unlock: %d, errno: %d\n", __FILE__, __LINE__, ret, errno);

	return 0;
}

int CRingBuffer::UnInit()
{
	int ret;
	ret = pthread_mutex_lock(&m_mutex);
	if (0 != ret)
		printf("%s %d pthread_mutex_lock: %d, errno: %d\n", __FILE__, __LINE__, ret, errno);
	
	if( false == m_bInit )
	{
		ret = pthread_mutex_unlock(&m_mutex);
		if (0 != ret)
			printf("%s %d pthread_mutex_unlock: %d, errno: %d\n", __FILE__, __LINE__, ret, errno);
		return 0;
	}

	if( NULL != m_pBuffer )
	{
		delete [] m_pBuffer;
		m_pBuffer = NULL;
	}
	
	m_iBufferLen = 0;
	
	m_iFps = 0;
	m_iMaxBufferSeconds = 0;	
	m_iReadPos = 0;
	m_iWritePos = 0;
	m_bBufferFull = false;
	
	m_bInit = false;
	
	ret = pthread_mutex_unlock(&m_mutex);
	if (0 != ret)
		printf("%s %d pthread_mutex_unlock: %d, errno: %d\n", __FILE__, __LINE__, ret, errno);

	return 0;
}

int CRingBuffer::PutData(stream_data_header_S *pStreamDataHeader, const char *pData, int iDataLen)
{
	int ret;
	ret = pthread_mutex_lock(&m_mutex);
	if (0 != ret)
		printf("%s %d pthread_mutex_lock: %d, errno: %d\n", __FILE__, __LINE__, ret, errno);
	
	if( false == m_bInit )
	{
		ret = pthread_mutex_unlock(&m_mutex);
		if (0 != ret)
			printf("%s %d pthread_mutex_unlock: %d, errno: %d\n", __FILE__, __LINE__, ret, errno);
		return -1;
	}

	//参数异常或帧数据太大, 无法存储
	if ( (NULL == pData) || ((iDataLen + sizeof(ring_buffer_data_header_S)) > m_iBufferLen) )
	{
		ret = pthread_mutex_unlock(&m_mutex);
		if (0 != ret)
			printf("%s %d pthread_mutex_unlock: %d, errno: %d\n", __FILE__, __LINE__, ret, errno);
		return -1;
	}

	int iFreeBufferLen = 0; 		//buffer中剩余空间大小

	//计算buffer中剩余空间大小
	if ( m_iWritePos > m_iReadPos )
	{
		iFreeBufferLen = ( m_iBufferLen - m_iWritePos ) + m_iReadPos;
	}
	else if ( m_iWritePos < m_iReadPos ) 
	{
		iFreeBufferLen = m_iReadPos - m_iWritePos;
	}
	else 	//m_iWritePos == m_iReadPos
	{
		if( false == m_bBufferFull )
			iFreeBufferLen = m_iBufferLen;
		else
			iFreeBufferLen = 0;
	}

	//剩余空间不足, 则移除旧数据, 腾出足够的空间
	if ( (iDataLen + sizeof(ring_buffer_data_header_S)) > iFreeBufferLen )
	{
		int iTempRreadPos = m_iReadPos;
		int iTempFreeBufferLen = iFreeBufferLen;
		int iWholeFrameLen = 0;
		int iFrameDataLen = 0;
		int iCount = 0;
		
		while(1)
		{
			int length = m_iBufferLen - iTempRreadPos;	//不循环前可读的空间大小
			if ( length < sizeof(ring_buffer_data_header_S) )
			{
				ring_buffer_data_header_S stRingBufferDataHeader = {0};
				char *pTemp = (char *)&stRingBufferDataHeader;
				memcpy( pTemp, m_pBuffer + iTempRreadPos, length );
				memcpy( pTemp + length, m_pBuffer, sizeof(ring_buffer_data_header_S) - length );

				iFrameDataLen = stRingBufferDataHeader.iDataLen;
				iWholeFrameLen = stRingBufferDataHeader.iDataLen + sizeof(ring_buffer_data_header_S);
			}
			else
			{
				ring_buffer_data_header_S *pRingBufferDataHeader = NULL;
				pRingBufferDataHeader = (ring_buffer_data_header_S *)(m_pBuffer + iTempRreadPos);

				iFrameDataLen = pRingBufferDataHeader->iDataLen;
				iWholeFrameLen = pRingBufferDataHeader->iDataLen + sizeof(ring_buffer_data_header_S);
			}
			
			//数据异常
			if ( (iFrameDataLen < 0) || (iFrameDataLen > (m_iBufferLen - sizeof(ring_buffer_data_header_S))) )
			{
				m_iReadPos = 0; 		//清空数据
				m_iWritePos = 0;
				m_bBufferFull = false;
				break;
			}
			
			iTempFreeBufferLen += iWholeFrameLen;

			//临时读指针移到一下帧位置
			iTempRreadPos += iWholeFrameLen;
			iTempRreadPos %= m_iBufferLen;
			
			iCount++;
			
			if ( iTempFreeBufferLen >= (iDataLen + sizeof(ring_buffer_data_header_S)) ) 		//有足够的buffer空间了
			{
				m_iReadPos = iTempRreadPos; 	//实际读指针移动, 移除旧数据
				break;
			}

#if 0 		//<shang> 如果存了很多小数据帧, 突然要存一个大数据的帧, 这种情况下此判断有误
			if( iCount > (m_iFps * m_iMaxBufferSeconds) ) 		//防止出现异常的时候死循环, 出现异常后清空所有数据
			{
				m_iReadPos = 0;
				m_iWritePos = 0;
				break;
			}
#endif
		}
		//printf("ringbuffer cover %d frames.\n", iCount);
	}


	//构建数据帧头
	ring_buffer_data_header_S stRingBufferDataHeader = {0};
	memcpy( &stRingBufferDataHeader, pStreamDataHeader, sizeof(stream_data_header_S) );
	stRingBufferDataHeader.iDataLen = iDataLen;

	//写入数据
	int length = m_iBufferLen - m_iWritePos; 	//不循环前可写的空间大小
	if ( length < sizeof(ring_buffer_data_header_S) ) 		//不循环前buffer不足以写完帧头
	{
		//写入帧头
		memcpy( m_pBuffer + m_iWritePos, &stRingBufferDataHeader, length );
		m_iWritePos += length;
		m_iWritePos %= m_iBufferLen;

		char *pTemp = (char *)&stRingBufferDataHeader;
		memcpy( m_pBuffer + m_iWritePos, pTemp + length, sizeof(ring_buffer_data_header_S) - length );
		m_iWritePos += sizeof(ring_buffer_data_header_S) - length;
		m_iWritePos %= m_iBufferLen;

		//写入帧数据
		memcpy( m_pBuffer + m_iWritePos, pData, iDataLen );
		m_iWritePos += iDataLen;
		m_iWritePos %= m_iBufferLen;
	}
	else
	{
		//写入帧头
		memcpy( m_pBuffer + m_iWritePos, &stRingBufferDataHeader, sizeof(ring_buffer_data_header_S) );
		m_iWritePos += sizeof(ring_buffer_data_header_S);
		m_iWritePos %= m_iBufferLen;

		//写入帧数据
		length = m_iBufferLen - m_iWritePos;	//不循环前可写的空间大小
		if ( length < iDataLen ) 			//不循环前buffer不足以写完帧数据
		{
			memcpy( m_pBuffer + m_iWritePos, pData, length );
			m_iWritePos += length;
			m_iWritePos %= m_iBufferLen;

			memcpy( m_pBuffer + m_iWritePos, pData+length, iDataLen-length );
			m_iWritePos += iDataLen-length;
			m_iWritePos %= m_iBufferLen;
		}
		else
		{
			memcpy( m_pBuffer + m_iWritePos, pData, iDataLen );
			m_iWritePos += iDataLen;
			m_iWritePos %= m_iBufferLen;
		}
	}

	if( m_iReadPos == m_iWritePos ) 	//如果写完数据之后, 读跟写的位置一样, 就表示 buffer 别写满了
		m_bBufferFull = true;

	ret = pthread_mutex_unlock(&m_mutex);
	if (0 != ret)
		printf("%s %d pthread_mutex_unlock: %d, errno: %d\n", __FILE__, __LINE__, ret, errno);
	
	return 0;
}

int CRingBuffer::GetData(stream_data_header_S *pStreamDataHeader, char *pData, int max_buff_size)
{
	int ret;
	ret = pthread_mutex_lock(&m_mutex);
	if (0 != ret)
		printf("%s %d pthread_mutex_lock: %d, errno: %d\n", __FILE__, __LINE__, ret, errno);
	
	if( false == m_bInit )
	{
		ret = pthread_mutex_unlock(&m_mutex);
		if (0 != ret)
			printf("%s %d pthread_mutex_unlock: %d, errno: %d\n", __FILE__, __LINE__, ret, errno);
		return -1;
	}
	
	//参数异常
	if ( (NULL == pStreamDataHeader) || (NULL == pData) )
	{
		ret = pthread_mutex_unlock(&m_mutex);
		if (0 != ret)
			printf("%s %d pthread_mutex_unlock: %d, errno: %d\n", __FILE__, __LINE__, ret, errno);
		return -1;
	}

	//没有可读取的数据
	if ( (false == m_bBufferFull) && (m_iWritePos == m_iReadPos) )
	{
		ret = pthread_mutex_unlock(&m_mutex);
		if (0 != ret)
			printf("%s %d pthread_mutex_unlock: %d, errno: %d\n", __FILE__, __LINE__, ret, errno);
		return 0;
	}

	int iFrameDataLen = 0; 		//帧数据长度

	//读取帧头
	int length = m_iBufferLen - m_iReadPos;	//不循环前可读的空间大小
	if ( length < sizeof(ring_buffer_data_header_S) )
	{
		ring_buffer_data_header_S stRingBufferDataHeader = {0};
		char *pTemp = (char *)&stRingBufferDataHeader;
		memcpy( pTemp, m_pBuffer + m_iReadPos, length );
		memcpy( pTemp + length, m_pBuffer, sizeof(ring_buffer_data_header_S) - length );

		memcpy(pStreamDataHeader, &stRingBufferDataHeader, sizeof(stream_data_header_S));

		iFrameDataLen = stRingBufferDataHeader.iDataLen;
	}
	else
	{
		ring_buffer_data_header_S *pRingBufferDataHeader = NULL;
		pRingBufferDataHeader = (ring_buffer_data_header_S *)(m_pBuffer + m_iReadPos);
		
		memcpy(pStreamDataHeader, pRingBufferDataHeader, sizeof(stream_data_header_S));

		iFrameDataLen = pRingBufferDataHeader->iDataLen;
	}

	//传入的buffer大小不足
	if ( iFrameDataLen > max_buff_size )
	{
		printf("<shang>------CRingBuffer::GetData buff size is not enough. iFrameDataLen : %d\n", iFrameDataLen);
		ret = pthread_mutex_unlock(&m_mutex);
		if (0 != ret)
			printf("%s %d pthread_mutex_unlock: %d, errno: %d\n", __FILE__, __LINE__, ret, errno);
		return -2;
	}

	//数据异常
	if ( (iFrameDataLen < 0) || (iFrameDataLen > (m_iBufferLen - sizeof(ring_buffer_data_header_S))) )
	{
		m_iReadPos = 0; 		//清空数据
		m_iWritePos = 0;
		m_bBufferFull = false;
		
		ret = pthread_mutex_unlock(&m_mutex);
		if (0 != ret)
			printf("%s %d pthread_mutex_unlock: %d, errno: %d\n", __FILE__, __LINE__, ret, errno);
		return -3;
	}
	
	//临时读指针, 跳过帧头定位到帧数据位置
	int iTempRreadPos = m_iReadPos;
	iTempRreadPos += sizeof(ring_buffer_data_header_S);
	iTempRreadPos %= m_iBufferLen;
	
	length = m_iBufferLen - iTempRreadPos;	//不循环前可读的空间大小
	if ( length < iFrameDataLen )
	{
		memcpy(pData, m_pBuffer + iTempRreadPos, length);
		memcpy(pData + length, m_pBuffer, iFrameDataLen - length);
	}
	else
	{
		memcpy(pData, m_pBuffer + iTempRreadPos, iFrameDataLen);
	}

	//偏移读指针
	m_iReadPos = iTempRreadPos + iFrameDataLen;
	m_iReadPos %= m_iBufferLen;
	
	if( m_iReadPos == m_iWritePos ) 	//如果读取数据之后, 读跟写的位置一样, 就表示 buffer 别读空了
		m_bBufferFull = false;
	
	ret = pthread_mutex_unlock(&m_mutex);
	if (0 != ret)
		printf("%s %d pthread_mutex_unlock: %d, errno: %d\n", __FILE__, __LINE__, ret, errno);
	
	return iFrameDataLen;
}

int CRingBuffer::ClearData()
{
	int ret;
	ret = pthread_mutex_lock(&m_mutex);
	if (0 != ret)
		printf("%s %d pthread_mutex_lock: %d, errno: %d\n", __FILE__, __LINE__, ret, errno);
	
	m_iReadPos = 0; 		//清空数据
	m_iWritePos = 0;
	m_bBufferFull = false;
	
	ret = pthread_mutex_unlock(&m_mutex);
	if (0 != ret)
		printf("%s %d pthread_mutex_unlock: %d, errno: %d\n", __FILE__, __LINE__, ret, errno);

	return 0;
}

int CRingBuffer::GetBufferLen()
{
	return m_iBufferLen;
}

int CRingBuffer::GetFreeBufferLen()
{
	int iFreeBufferLen = 0;

	int ret;
	ret = pthread_mutex_lock(&m_mutex);
	if (0 != ret)
		printf("%s %d pthread_mutex_lock: %d, errno: %d\n", __FILE__, __LINE__, ret, errno);

	//计算buffer中剩余空间大小
	if ( m_iWritePos > m_iReadPos )
	{
		iFreeBufferLen = ( m_iBufferLen - m_iWritePos ) + m_iReadPos;
	}
	else if ( m_iWritePos < m_iReadPos ) 
	{
		iFreeBufferLen = m_iReadPos - m_iWritePos;
	}
	else 	//m_iWritePos == m_iReadPos
	{
		if( false == m_bBufferFull )
			iFreeBufferLen = m_iBufferLen;
		else
			iFreeBufferLen = 0;
	}
	
	ret = pthread_mutex_unlock(&m_mutex);
	if (0 != ret)
		printf("%s %d pthread_mutex_unlock: %d, errno: %d\n", __FILE__, __LINE__, ret, errno);

	return iFreeBufferLen;
}

int CRingBuffer::GetStreamDuration()
{
	unsigned long long ullStartTimestamp_ms, ullEndTimestamp_ms, ullDuration_ms;
	
	int ret;
	ret = pthread_mutex_lock(&m_mutex);
	if (0 != ret)
		printf("%s %d pthread_mutex_lock: %d, errno: %d\n", __FILE__, __LINE__, ret, errno);
	
	if( false == m_bInit )
	{
		ret = pthread_mutex_unlock(&m_mutex);
		if (0 != ret)
			printf("%s %d pthread_mutex_unlock: %d, errno: %d\n", __FILE__, __LINE__, ret, errno);
		return -1;
	}

	//没有可读取的数据
	if ( (false == m_bBufferFull) && (m_iWritePos == m_iReadPos) )
	{
		ret = pthread_mutex_unlock(&m_mutex);
		if (0 != ret)
			printf("%s %d pthread_mutex_unlock: %d, errno: %d\n", __FILE__, __LINE__, ret, errno);
		return 0;
	}
	
	int length;
	int iVideoFrameCount = 0;
	int iAudioFrameCount = 0;
	int iFrameDataLen;
	unsigned long long ullTmpTimestamp_s;
	ring_buffer_data_header_S stRingBufferDataHeader = {0};

	int iTempRreadPos = m_iReadPos;

	while( iTempRreadPos != m_iWritePos )
	{
		//读取帧头
		length = m_iBufferLen - iTempRreadPos; //不循环前可读的空间大小
		if ( length < sizeof(ring_buffer_data_header_S) )
		{
			char *pTemp = (char *)&stRingBufferDataHeader;
			memcpy( pTemp, m_pBuffer + iTempRreadPos, length );
			memcpy( pTemp + length, m_pBuffer, sizeof(ring_buffer_data_header_S) - length );
				
			iFrameDataLen = stRingBufferDataHeader.iDataLen;
			ullTmpTimestamp_s = stRingBufferDataHeader.ullTimestamp;
		}
		else
		{
			ring_buffer_data_header_S *pRingBufferDataHeader = NULL;
			pRingBufferDataHeader = (ring_buffer_data_header_S *)(m_pBuffer + iTempRreadPos);

			memcpy( &stRingBufferDataHeader, pRingBufferDataHeader, sizeof(ring_buffer_data_header_S) );
					
			iFrameDataLen = pRingBufferDataHeader->iDataLen;
			ullTmpTimestamp_s = pRingBufferDataHeader->ullTimestamp;
		}

		if( 1 == stRingBufferDataHeader.iStreamType ) 	//video
		{
			if( 0 == iVideoFrameCount )
				ullStartTimestamp_ms = ullTmpTimestamp_s;
			ullEndTimestamp_ms = ullTmpTimestamp_s;

			iVideoFrameCount++;
		}
		else if( 2 == stRingBufferDataHeader.iStreamType ) 	//audio
		{
			iAudioFrameCount++;
		}

		iTempRreadPos += sizeof(ring_buffer_data_header_S) + iFrameDataLen;
		iTempRreadPos %= m_iBufferLen;

	}

	AppInfo("cur video frame count : %d\n", iVideoFrameCount);
	AppInfo("cur audio frame count : %d\n", iAudioFrameCount);
	AppInfo("first frame timestamp : %llu\n", ullStartTimestamp_ms);
	AppInfo("last frame timestamp  : %llu\n", ullEndTimestamp_ms);
	
	ret = pthread_mutex_unlock(&m_mutex);
	if (0 != ret)
		printf("%s %d pthread_mutex_unlock: %d, errno: %d\n", __FILE__, __LINE__, ret, errno);

	ullDuration_ms = ullEndTimestamp_ms - ullStartTimestamp_ms;
	return (int)ullDuration_ms;
}


/*
 *@ brief 丢弃 ullTimestamp_ms 之前最近的一个 I 帧之前所有的数据
 */
int CRingBuffer::DiscardDataOnTime(unsigned long long ullTimestamp_ms)
{
	int ret;
	ret = pthread_mutex_lock(&m_mutex);
	if (0 != ret)
		printf("%s %d pthread_mutex_lock: %d, errno: %d\n", __FILE__, __LINE__, ret, errno);
	
	if( false == m_bInit )
	{
		ret = pthread_mutex_unlock(&m_mutex);
		if (0 != ret)
			printf("%s %d pthread_mutex_unlock: %d, errno: %d\n", __FILE__, __LINE__, ret, errno);
		return -1;
	}

	//没有可读取的数据
	if ( (false == m_bBufferFull) && (m_iWritePos == m_iReadPos) )
	{
		ret = pthread_mutex_unlock(&m_mutex);
		if (0 != ret)
			printf("%s %d pthread_mutex_unlock: %d, errno: %d\n", __FILE__, __LINE__, ret, errno);
		return 0;
	}
	
	int length;
	ring_buffer_data_header_S stRingBufferDataHeader = {0};

	int iLastIFrameRreadPos = -1;
	int iTempRreadPos = m_iReadPos;

	while( iTempRreadPos != m_iWritePos )
	{
		//读取帧头
		length = m_iBufferLen - iTempRreadPos; //不循环前可读的空间大小
		if ( length < sizeof(ring_buffer_data_header_S) )
		{
			char *pTemp = (char *)&stRingBufferDataHeader;
			memcpy( pTemp, m_pBuffer + iTempRreadPos, length );
			memcpy( pTemp + length, m_pBuffer, sizeof(ring_buffer_data_header_S) - length );				
		}
		else
		{
			ring_buffer_data_header_S *pRingBufferDataHeader = NULL;
			pRingBufferDataHeader = (ring_buffer_data_header_S *)(m_pBuffer + iTempRreadPos);

			memcpy( &stRingBufferDataHeader, pRingBufferDataHeader, sizeof(ring_buffer_data_header_S) );				
		}

		if( 1 == stRingBufferDataHeader.iStreamType ) 	//video
		{
			if( stRingBufferDataHeader.ullTimestamp > ullTimestamp_ms )
			{
				break;
			}

			if( 1 == stRingBufferDataHeader.iFrameType )
			{
				iLastIFrameRreadPos = iTempRreadPos;
			}
		}

		iTempRreadPos += sizeof(ring_buffer_data_header_S) + stRingBufferDataHeader.iDataLen;
		iTempRreadPos %= m_iBufferLen;

	}

	AppInfo("DiscardDataOnTime old : %d, new :%d\n", m_iReadPos, iLastIFrameRreadPos);
	if( -1 != iLastIFrameRreadPos )
	{
		//偏移读指针
		m_iReadPos = iLastIFrameRreadPos;
		m_iReadPos %= m_iBufferLen;
	}
	
	ret = pthread_mutex_unlock(&m_mutex);
	if (0 != ret)
		printf("%s %d pthread_mutex_unlock: %d, errno: %d\n", __FILE__, __LINE__, ret, errno);

	return 0;
}


