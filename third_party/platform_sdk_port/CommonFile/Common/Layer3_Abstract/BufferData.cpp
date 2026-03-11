#include "MySingleLock.h"
#include "BufferData.h"

#define new DEBUG_NEW

CBufferData::CBufferData(const void *pFrameData, int iLen, bool bIsAutoDestroy)
{
	ASSERT(NULL != pFrameData);
	ASSERT(iLen > 0);
	/////////是否自动销毁
	m_bAutoDestroy = bIsAutoDestroy;
	m_iLen = iLen;

	if(bIsAutoDestroy)
	{
		m_pFrameData = new BYTE[iLen];
		memcpy(m_pFrameData, pFrameData, m_iLen);		
	}
	else
	{
		m_pFrameData = static_cast<BYTE *>(const_cast<void *>(pFrameData));
	}

	m_Count = 0;
}

CBufferData::~CBufferData(void)
{
	if(m_bAutoDestroy)
	{	
		ASSERT(!m_Count);
		if(NULL != m_pFrameData)
		{
			delete [] m_pFrameData;
			m_pFrameData = NULL;

			m_iLen = 0;
		}
	}
}

int CBufferData::Add()
{
	CMySingleLock  lockAction(m_Lock);
	++m_Count;
	return m_Count;
}

int CBufferData::Dec()
{
	int iCount = 0;

	{
		CMySingleLock  lockAction(m_Lock);
		--m_Count;
		iCount = m_Count;
		if (!m_bAutoDestroy)		//不是自动销毁则什么都不用做,直接返回
		{
			return iCount;
		}
	}

	if(!iCount)
	{
		delete this;
		return iCount;
	}

	return iCount;	
}

int CBufferData::GetBufferDataLen() const
{
	return m_iLen;
}

////////////////获取帧数据
const BYTE * CBufferData::GetBufferData() const
{
	return m_pFrameData;
}
