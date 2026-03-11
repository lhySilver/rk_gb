#include "NetPacket.h"
#include "PFHelper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

tuint32 CNetPacket::m_dwPageSize=0;

CNetPacket::CNetPacket(void) : m_pTempBuffer(NULL)
, m_tempBufferLen(0)
{
	if (0 == m_dwPageSize)
	{
		m_dwPageSize=CPFHelper::GetPageSize();
	}

	memset(&m_NetPacketBuffer, 0, sizeof(m_NetPacketBuffer));
}

CNetPacket::~CNetPacket(void)
{
	FreeBuffer(m_NetPacketBuffer);
}

bool CNetPacket::AllocateBuffer(NET_PACKET_BUFFER& packetBuffer , size_t size)
{
	if (size <= 0)
	{
		TASSERT(false);
		return false;
	}

	FreeBuffer(packetBuffer);

	size = ((size + m_dwPageSize - 1) / m_dwPageSize) * m_dwPageSize;
	try
	{
		packetBuffer.pBuffer = new char[size];
	}
	catch (...)
	{
		ABNORMAL_OUTPUT("bool CNetPacket::AllocateBuffer packetBuffer.pBuffer = new char[size=%d]",size);
		return false;
	}
	

	if (NULL == packetBuffer.pBuffer)
	{
		TASSERT(false);
		return false;
	}
	else
	{
		packetBuffer.size = size;
		return true;
	}	
}

bool CNetPacket::FreeBuffer(NET_PACKET_BUFFER& packetBuffer)
{
	if (NULL != packetBuffer.pBuffer)
	{
		TASSERT(packetBuffer.size > 0);
		delete [] packetBuffer.pBuffer;

	}
	memset(&packetBuffer, 0, sizeof(packetBuffer));
	return true;
}
