
#ifndef __NET_PACKET__
#define __NET_PACKET__

#include "base_type_define.h"
#include "ShareSDK.h"
class CNetPacket
{
public:
	CNetPacket(void);
	virtual ~CNetPacket(void);

	void SetBuffer(const char* pBuffer, size_t len)
	{
		m_pTempBuffer = pBuffer;
		m_tempBufferLen = len;
	}

	const char* GetBuffer() const
	{
		return m_pTempBuffer;
	}

	size_t GetBufferLen() const
	{
		return m_tempBufferLen;
	}

	size_t GetLastBufferLen() const
	{
		return m_NetPacketBuffer.write_pos - m_NetPacketBuffer.read_pos;
	}

	inline const char * GetPacket(size_t packetLen);
	inline bool SaveBuffer(const char* pBuffer, size_t len);
	inline bool SaveBufferFront(const char* pBuffer, size_t len);

	struct NET_PACKET_BUFFER 
	{
		char*	pBuffer;
		size_t	size;//分配空间长度
		size_t	write_pos;//已写位置
		size_t	read_pos;//已读位置

		NET_PACKET_BUFFER() : pBuffer(NULL), size(0), write_pos(0), read_pos(0){}
	};

	static bool AllocateBuffer(NET_PACKET_BUFFER& packetBuffer , size_t size);
	static bool FreeBuffer(NET_PACKET_BUFFER& packetBuffer);

private:
	static tuint32 m_dwPageSize;
	NET_PACKET_BUFFER m_NetPacketBuffer;

	const char* m_pTempBuffer;
	size_t m_tempBufferLen;
};

const char* CNetPacket::GetPacket(size_t packetLen)
{
	TASSERT(m_NetPacketBuffer.write_pos >= m_NetPacketBuffer.read_pos);
	size_t buffer_size = m_NetPacketBuffer.write_pos - m_NetPacketBuffer.read_pos;//未读取的残余数据长度

	if (buffer_size > 0)//有残留数据
	{
		if (buffer_size >= packetLen)//残留数据足够一个包的长度
		{
			const char* result_buffer = m_NetPacketBuffer.pBuffer + m_NetPacketBuffer.read_pos;
			m_NetPacketBuffer.read_pos += packetLen;

			if(buffer_size == packetLen)
			{
				m_NetPacketBuffer.read_pos = 0;
				m_NetPacketBuffer.write_pos = 0;
			}
			return result_buffer;
		} 
		else //残留数据不足一个包的长度
		{
			TASSERT(m_tempBufferLen > 0);
			TASSERT(NULL != m_pTempBuffer);

			size_t copy_len = packetLen - buffer_size;
			if (copy_len <= m_tempBufferLen)//足够数据
			{
				if ( (copy_len + m_NetPacketBuffer.write_pos) > m_NetPacketBuffer.size)//剩余空间不够
				{
					NET_PACKET_BUFFER tempPacketBuffer;
					AllocateBuffer(tempPacketBuffer, packetLen);
					memcpy(tempPacketBuffer.pBuffer, m_NetPacketBuffer.pBuffer + m_NetPacketBuffer.read_pos, buffer_size);
					tempPacketBuffer.write_pos = buffer_size;

					FreeBuffer(m_NetPacketBuffer);
					m_NetPacketBuffer = tempPacketBuffer;
				} 

				memcpy(m_NetPacketBuffer.pBuffer + m_NetPacketBuffer.write_pos, m_pTempBuffer, copy_len);

				const char* result_buffer = m_NetPacketBuffer.pBuffer + m_NetPacketBuffer.read_pos;
				m_NetPacketBuffer.read_pos = 0;
				m_NetPacketBuffer.write_pos = 0;

				m_tempBufferLen -= copy_len;
				m_pTempBuffer = (0 == m_tempBufferLen) ? NULL : (m_pTempBuffer + copy_len);
				
				return result_buffer;
			}
			else//数据不足
			{
				return NULL;
			}			
		}
	} 
	else//无残留数据
	{
		if (0 != m_NetPacketBuffer.write_pos)
		{
			m_NetPacketBuffer.write_pos = 0;
			m_NetPacketBuffer.read_pos = 0;
		}

		if (m_tempBufferLen > 0)//有数据
		{
			TASSERT(NULL != m_pTempBuffer);

			if (m_tempBufferLen >= packetLen)//有足够一个包的数据
			{
				const char* result_buffer = m_pTempBuffer;
				m_pTempBuffer += packetLen;
				m_tempBufferLen -= packetLen;
				return result_buffer;
			} 
			else//不足一个包的数据
			{
				//m_pTempBuffer = NULL;
				//m_tempBufferLen = 0;
				return NULL;
			}
		}
		else//无数据
		{
//			TASSERT(FALSE);
			return NULL;
		}
	}
	return NULL;
}

bool CNetPacket::SaveBuffer(const char* pBuffer, size_t len)
{
	TASSERT(NULL != pBuffer);
	TASSERT(len > 0);

	if ( (len + m_NetPacketBuffer.write_pos) > m_NetPacketBuffer.size)//剩余空间不够
	{
		char* pTempData = NULL;
		size_t tempLen = 0;

		if (m_NetPacketBuffer.write_pos > 0)
		{
			tempLen = m_NetPacketBuffer.write_pos;
			pTempData = new char[m_NetPacketBuffer.write_pos];
			TASSERT(NULL != pTempData);
			memcpy(pTempData, m_NetPacketBuffer.pBuffer, tempLen);
		}

		size_t read_pos = m_NetPacketBuffer.read_pos;
		if (!AllocateBuffer(m_NetPacketBuffer, len + m_NetPacketBuffer.write_pos))
		{
			if(pTempData)
			{
				delete [] pTempData;
				pTempData = NULL;
			}
			ABNORMAL_OUTPUT("AllocateBuffer  分配缓冲区失败！");
			return false;
		}

		if (NULL != pTempData)
		{
			memcpy(m_NetPacketBuffer.pBuffer, pTempData, tempLen);
			m_NetPacketBuffer.write_pos = tempLen;
			m_NetPacketBuffer.read_pos = read_pos;

			delete [] pTempData;
			pTempData = NULL;
		}
	}
	memcpy(m_NetPacketBuffer.pBuffer + m_NetPacketBuffer.write_pos, pBuffer, len);
	m_NetPacketBuffer.write_pos += len;	

	return true;
}

bool CNetPacket::SaveBufferFront(const char* pBuffer, size_t len)
{
	TASSERT(NULL != pBuffer);
	TASSERT(len > 0);

	if (len <= m_NetPacketBuffer.read_pos)//有足够空间
	{
		memcpy(m_NetPacketBuffer.pBuffer + m_NetPacketBuffer.read_pos - len, pBuffer, len);
		m_NetPacketBuffer.read_pos -= len;

		return true;
	}
	else
	{
		ABNORMAL_OUTPUT("SaveBufferFront缓冲溢出！");
		return false;
	}

	return false;
}

#endif //__NET_PACKET__