
#include "MySingleLock.h"
#include "FrameData.h"
#include "ShareSDK.h"

#define new DEBUG_NEW

// CFrameData::CFrameData(const void *pFrameData, int iLen, bool bIsAutoDestroy)
// {
// 	ASSERT(NULL != pFrameData);
// 	ASSERT(iLen > 0);
// 	/////////是否自动销毁
// 	m_bAutoDestroy = bIsAutoDestroy;
// 	m_iLen = iLen;
// 
// 	if(bIsAutoDestroy)
// 	{
// 		m_pFrameData = new BYTE[iLen];
// 		memcpy(m_pFrameData, pFrameData, m_iLen);		
// 	}
// 	else
// 	{
// 		m_pFrameData = static_cast<BYTE *>(const_cast<void *>(pFrameData));
// 	}
// 
// 	m_Count = 0;
// }
// 
// CFrameData::~CFrameData(void)
// {
// 	if(m_bAutoDestroy)
// 	{	
// 		ASSERT(!m_Count);
// 		if(NULL != m_pFrameData)
// 		{
// 			delete [] m_pFrameData;
// 			m_pFrameData = NULL;
// 
// 			m_iLen = 0;
// 		}
// 	}
// }
// 
// int CFrameData::Add()
// {
// 	ASSERT(m_bAutoDestroy);
// 	CMySingleLock  lockAction(m_Lock);
// 	++m_Count;
// 	return m_Count;
// }
// 
// int CFrameData::Dec()
// {
// 	int iCount = 0;
// 
// 	{
// 		CMySingleLock  lockAction(m_Lock);
// 		--m_Count;
// 		iCount = m_Count;
// 
// 		if (!m_bAutoDestroy)		//不是自动销毁则什么都不用做,直接返回
// 		{
// 			return iCount;
// 		}
// 	}
// 
// 	//如果计数为0并且自动销毁,销毁自己
// 	if (!iCount)
// 	{
// 		delete this;
// 	}
// 	
// 	return iCount;	
// }
// 
// int CFrameData::GetFrameDataLen() const
// {
// 	return m_iLen;
// }
// 
// ////////////////获取帧数据
// const BYTE * CFrameData::GetFrameData() const
// {
// 	return m_pFrameData;
// }
// 
// ////////////////是否关键帧
// bool CFrameData::IsKeyframe()
// {
// 	NVMS_DEFINE::STREAM_HEADER_INFO  FrameStreamInfo;
// 	memset(&FrameStreamInfo,0,sizeof(FrameStreamInfo));
// 	if(GetStreamInfoOfFrame(FrameStreamInfo))
// 	{
// 		if(FrameStreamInfo.swFlag == streamHEADERFLAG)
// 		{
// 			return !!FrameStreamInfo.byIsKeyFrame;
// 		}
// 	}
// 
// 	ASSERT(false);
// 	return false;
// }
// 
// ///获取缓冲区的去掉头之后的帧长度
// int CFrameData::GetDataLen() const
// {
// 	if((GetStreamType() == NVMS_DEFINE::LIVE_STREAM_GPS) || (GetStreamType() == NVMS_DEFINE::PLAYBACK_STREAM_GPS))
// 	{
// 		return m_iLen-NVMS_DEFINE::NET_STREAM_HEADER_LENGTH;
// 	}
// 	else
// 	{
// 		NVMS_DEFINE::FRAME_HEADER_INFO frameInfo;
// 		memset(&frameInfo,0,sizeof(frameInfo));
// 		GetHeaderInfoOfFrame(frameInfo);
// 		return frameInfo.dwRealFrameLen;
// 	}
// }
// 
// ///获取缓冲区的去掉头之后的帧数据
// const BYTE * CFrameData::GetData() const
// {
// 	if((GetStreamType() == NVMS_DEFINE::LIVE_STREAM_GPS) || (GetStreamType() == NVMS_DEFINE::PLAYBACK_STREAM_GPS))
// 	{
// 		return m_pFrameData+NVMS_DEFINE::NET_STREAM_HEADER_LENGTH;
// 	}
// 	else
// 	{
// 		NVMS_DEFINE::FRAME_HEADER_INFO &frameInfo=GetHeaderInfoOfFrame();
// 		return m_pFrameData+NVMS_DEFINE::NET_STREAM_HEADER_LENGTH+NVMS_DEFINE::FRAME_HEADER_INFO_LENGTH+frameInfo.byExtInfoLen;
// 	}
// 
// }
// 
// 
// ///获取缓冲区的扩展数据长度
// int CFrameData::GetExDataLen() const
// {
// 	if((GetStreamType() == NVMS_DEFINE::LIVE_STREAM_GPS) || (GetStreamType() == NVMS_DEFINE::PLAYBACK_STREAM_GPS))
// 	{
// 		return 0;
// 	}
// 	else
// 	{
// 		NVMS_DEFINE::FRAME_HEADER_INFO frameInfo;
// 		memset(&frameInfo,0,sizeof(frameInfo));
// 		GetHeaderInfoOfFrame(frameInfo);
// 		return (int)frameInfo.byExtInfoLen;
// 	}
// }
// ///获取缓冲区的扩展数据
// const BYTE * CFrameData::GetExData() const
// {
// 	if((GetStreamType() == NVMS_DEFINE::LIVE_STREAM_GPS) || (GetStreamType() == NVMS_DEFINE::PLAYBACK_STREAM_GPS))
// 	{
// 		return NULL;
// 	}
// 	else
// 	{
// 		return m_pFrameData+NVMS_DEFINE::NET_STREAM_HEADER_LENGTH+NVMS_DEFINE::FRAME_HEADER_INFO_LENGTH;
// 	}
// }
// 
// NVMS_DEFINE::FRAME_HEADER_INFO & CFrameData::GetHeaderInfoOfFrame() const
// {
// 	CMySingleLock lockAction(m_Lock);
// 
// 	if((GetStreamType() == NVMS_DEFINE::LIVE_STREAM_GPS) || (GetStreamType() == NVMS_DEFINE::PLAYBACK_STREAM_GPS)) ASSERT(false);
// 	ASSERT(m_iLen >= static_cast<int>( NVMS_DEFINE::NET_STREAM_HEADER_LENGTH + NVMS_DEFINE::FRAME_HEADER_INFO_LENGTH));
// 
// 	return *reinterpret_cast<NVMS_DEFINE::FRAME_HEADER_INFO *>(m_pFrameData +  NVMS_DEFINE::NET_STREAM_HEADER_LENGTH);
// }
// 
// ////////////////获取帧头信息
// bool CFrameData::GetHeaderInfoOfFrame(NVMS_DEFINE::FRAME_HEADER_INFO &FrameHeaderInfo) const
// {
// 	CMySingleLock lockAction(m_Lock);
// 	if((GetStreamType() == NVMS_DEFINE::LIVE_STREAM_GPS) || (GetStreamType() == NVMS_DEFINE::PLAYBACK_STREAM_GPS)) ASSERT(false);
// 
// 	if(m_iLen >= static_cast<int>(NVMS_DEFINE::NET_STREAM_HEADER_LENGTH + NVMS_DEFINE::FRAME_HEADER_INFO_LENGTH))
// 	{
// 		const NVMS_DEFINE::FRAME_HEADER_INFO *pHeaderInfo = reinterpret_cast<const NVMS_DEFINE::FRAME_HEADER_INFO *>(m_pFrameData + NVMS_DEFINE::NET_STREAM_HEADER_LENGTH);
// 
// 		memcpy(&FrameHeaderInfo, pHeaderInfo, NVMS_DEFINE::FRAME_HEADER_INFO_LENGTH);
// 		return true;
// 	}
// 	else
// 	{
// 		ASSERT(false);
// 		//////////////帧数据错误
// 		return false;
// 	}
// }
// 
// ////////////////获取流类型
// int CFrameData::GetStreamType() const
// {
// 	CMySingleLock lockAction(m_Lock);
// 
// 	if(static_cast<size_t>(m_iLen) >= (sizeof(NVMS_DEFINE::STREAM_HEADER_INFO)))
// 	{
// 		NVMS_DEFINE::STREAM_HEADER_INFO   *pStreamHeaderInfo = (NVMS_DEFINE::STREAM_HEADER_INFO *)(m_pFrameData);
// 
// 		if(pStreamHeaderInfo->swFlag == streamHEADERFLAG)
// 		{
// 			return pStreamHeaderInfo->byStreamType;
// 		}
// 	}
// 
// 	/*ASSERT(FALSE);*/
// 	return NVMS_DEFINE::STREAM_NULL;
// }
// 
// /////////////////////////////////////////////
// void CFrameData::SetStreamType(BYTE streamType)
// {
// 	CMySingleLock lockAction(m_Lock);
// 
// 	if(static_cast<size_t>(m_iLen) >= (sizeof(NVMS_DEFINE::STREAM_HEADER_INFO)))
// 	{
// 		NVMS_DEFINE::STREAM_HEADER_INFO   *pStreamHeaderInfo = (NVMS_DEFINE::STREAM_HEADER_INFO *)(m_pFrameData);
// 		
// 		ASSERT(pStreamHeaderInfo->swFlag == streamHEADERFLAG);
// 		pStreamHeaderInfo->byStreamType=streamType;
// 	}
// #if _DEBUG
// 	else
// 	{
// 		ASSERT(false);
// 	}
// #endif
// }
// 
// ////////////////获取帧的流信息
// NVMS_DEFINE::STREAM_HEADER_INFO & CFrameData::GetStreamInfoOfFrame() const
// {
// 	CMySingleLock lockAction(m_Lock);
// 	
// 	ASSERT(static_cast<size_t>(m_iLen) >= (sizeof(NVMS_DEFINE::STREAM_HEADER_INFO)));
// 	ASSERT(reinterpret_cast<NVMS_DEFINE::STREAM_HEADER_INFO *>(m_pFrameData)->swFlag == streamHEADERFLAG);
// 	return *reinterpret_cast<NVMS_DEFINE::STREAM_HEADER_INFO *>(m_pFrameData);
// }
// 
// ////////////////////////////////////////////////////////////
// bool CFrameData::GetStreamInfoOfFrame(NVMS_DEFINE::STREAM_HEADER_INFO & StreamHeaderInfo) const
// {
// 	CMySingleLock lockAction(m_Lock);
// 	if(static_cast<size_t>(m_iLen) >= (sizeof(NVMS_DEFINE::STREAM_HEADER_INFO)))
// 	{
// 		StreamHeaderInfo = *reinterpret_cast<NVMS_DEFINE::STREAM_HEADER_INFO *>(m_pFrameData);
// 		ASSERT(StreamHeaderInfo.swFlag == streamHEADERFLAG);
// 		return true;
// 	}
// 
// 	return false;
// }
// 
// ///////////////获取帧的通道ID
// GUID CFrameData::GetChannelID()
// {
// 	NVMS_DEFINE::STREAM_HEADER_INFO  FrameStreamInfo;
// 	memset(&FrameStreamInfo,0,sizeof(FrameStreamInfo));
// 	if(GetStreamInfoOfFrame(FrameStreamInfo))
// 	{
// 		if(FrameStreamInfo.swFlag == streamHEADERFLAG)
// 		{
// 			return FrameStreamInfo.nodeID;
// 		}
// 	}
// 
// 	/*ASSERT(FALSE);*/
// 	return GUID_NULL;
// }
// 
// //////////////设置发送索引的值
// bool CFrameData::SetSendIndex(tuint32 dwSendIndex)
// {
// 	CMySingleLock lockAction(m_Lock);
// 	if(static_cast<size_t>(m_iLen) >= (sizeof(NVMS_DEFINE::STREAM_HEADER_INFO)))
// 	{
// 		NVMS_DEFINE::STREAM_HEADER_INFO   *pStreamHeaderInfo = (NVMS_DEFINE::STREAM_HEADER_INFO *)(m_pFrameData);
// 	
// 		ASSERT(pStreamHeaderInfo->swFlag == streamHEADERFLAG);
// 		pStreamHeaderInfo->dwSendFrameIndex = dwSendIndex;
// 		return true;
// 	}
// 
// 	ASSERT(false);
// 	return false;
// }


/************************************************************************/
/*                                                                      */
/************************************************************************/
CFrameDataEx::CFrameDataEx(const void *pFrameData, tuint32 iLen, bool bIsAutoDestroy):m_InterLock(0)
{
	ASSERT(NULL != pFrameData);

	/////////是否自动销毁
	m_bAutoDestroy = bIsAutoDestroy;
	if(iLen == 0xFFFFFFFF)
	{
        m_hMem = (tuint_ptr)pFrameData;
		m_pFrameData = NULL;
		m_iLen = MEM_POOL_GetLength(m_hMem);
		MEM_POOL_GetContent(m_hMem, m_FrameContent);

        CChildPairContainer<BYTE *, tint32>::CMyIterator  myIter = m_FrameContent.begin();
        BYTE *pHeadBuf=myIter->first;
        m_pStreamHead=(NVMS_DEFINE::STREAM_HEADER_INFO*)pHeadBuf;
        m_pFrameInfo=(NVMS_DEFINE::FRAME_HEADER_INFO*)(pHeadBuf+sizeof(NVMS_DEFINE::STREAM_HEADER_INFO));
        if(m_pFrameInfo->byExtInfoLen > 0)
            m_pExInfo=(NVMS_DEFINE::FRAME_EXTENSION_INFO*)(pHeadBuf+sizeof(NVMS_DEFINE::STREAM_HEADER_INFO)+sizeof(NVMS_DEFINE::FRAME_HEADER_INFO));
        else
            m_pExInfo=NULL;
        if(bIsAutoDestroy)
		{
			MEM_POOL_AddReference(m_hMem);
		}
	}
	else
	{
		m_hMem = 0;
		m_iLen = iLen; 
		if(bIsAutoDestroy)
		{
			m_pFrameData = NULL;
			try
			{
				m_pFrameData = new BYTE[iLen];
				memcpy(m_pFrameData, pFrameData, m_iLen);
			}
			catch (...)
			{
				if (m_pFrameData)
				{
					delete[] m_pFrameData;
					m_pFrameData = NULL;
				}
			}
		}
		else
		{
			m_pFrameData = static_cast<BYTE *>(const_cast<void *>(pFrameData));
		}
		if (m_pFrameData)
		{
			m_pStreamHead=(NVMS_DEFINE::STREAM_HEADER_INFO*)m_pFrameData;
			m_pFrameInfo=(NVMS_DEFINE::FRAME_HEADER_INFO*)(m_pFrameData+sizeof(NVMS_DEFINE::STREAM_HEADER_INFO));
			if(m_pFrameInfo->byExtInfoLen > 0)
				m_pExInfo=(NVMS_DEFINE::FRAME_EXTENSION_INFO*)(m_pFrameData+sizeof(NVMS_DEFINE::STREAM_HEADER_INFO)+sizeof(NVMS_DEFINE::FRAME_HEADER_INFO));
			else
				m_pExInfo=NULL;
		}
    }
}

CFrameDataEx::~CFrameDataEx(void)
{
	if(m_bAutoDestroy)
	{	
		if(NULL != m_pFrameData)
		{
			delete [] m_pFrameData;
			m_pFrameData = NULL; 
		}

		if(m_hMem)
		{
			MEM_POOL_DecReference(m_hMem);
			m_hMem = 0;
		}
	}	
	m_iLen = 0;
}

int CFrameDataEx::Add()
{
	ASSERT(m_bAutoDestroy);
    return m_InterLock.Increment();
}

int CFrameDataEx::Dec()
{
    int iCount = m_InterLock.Decrement();

    if (!m_bAutoDestroy)		//不是自动销毁则什么都不用做,直接返回
    {
        return iCount;
	}
    else
    {
        //如果计数为0并且自动销毁,销毁自己
        if (!iCount)
        {
            delete this;
        }
        return iCount;
    }
}

tuint32 CFrameDataEx::GetFrameDataLen()
{
    if(m_hMem)
        return 0xFFFFFFFF;

	return m_iLen;
}

tuint32 CFrameDataEx::GetRealFrameDataLen()
{
	return m_iLen;
}

////////////////获取帧数据
const BYTE * CFrameDataEx::GetFrameData() 
{
	if(m_hMem)
    {
        return (BYTE *)m_hMem;
    }
	else
	{
		TASSERT(m_pFrameData != NULL);
		return m_pFrameData;
	}
}

////////////////是否关键帧
bool CFrameDataEx::IsKeyframe()
{
    if(m_pStreamHead->swFlag == streamHEADERFLAG)
    {
        return !!m_pStreamHead->byIsKeyFrame;
	}

	ASSERT(false);
	return false;
}

///获取缓冲区的去掉头之后的帧长度
int CFrameDataEx::GetDataLen() 
{
    return m_pFrameInfo->dwRealFrameLen;
}

///获取缓冲区去掉头之后的帧数据
const BYTE * CFrameDataEx::GetData() 
{
    if(m_hMem)
    {
        CChildPairContainer<BYTE *, tint32>::CMyIterator  myIter = m_FrameContent.begin();
        if(m_FrameContent.size()>1)
        {
            ++myIter;
            return myIter->first;
        }
        else
        {
            return myIter->first+NVMS_DEFINE::NET_STREAM_HEADER_LENGTH+NVMS_DEFINE::FRAME_HEADER_INFO_LENGTH+m_pFrameInfo->byExtInfoLen;
        }
    }
    else
    {
        return m_pFrameData+NVMS_DEFINE::NET_STREAM_HEADER_LENGTH+NVMS_DEFINE::FRAME_HEADER_INFO_LENGTH+m_pFrameInfo->byExtInfoLen;
    }
}


///获取缓冲区扩展数据长度
int CFrameDataEx::GetExDataLen() 
{
    return (int)m_pFrameInfo->byExtInfoLen;
}

///获取缓冲区扩展数据
const BYTE * CFrameDataEx::GetExData() 
{
    return (BYTE*)m_pExInfo;
}

NVMS_DEFINE::FRAME_HEADER_INFO & CFrameDataEx::GetHeaderInfoOfFrame() 
{
    return *m_pFrameInfo;

}

////////////////获取流类型
int CFrameDataEx::GetStreamType() 
{
    if(m_pStreamHead->swFlag == streamHEADERFLAG)
    {
       return m_pStreamHead->byStreamType;
    }

	return NVMS_DEFINE::STREAM_NULL; 
}

// /////////////////////////////////////////////
void CFrameDataEx::SetStreamType(BYTE streamType)
{
    ASSERT(m_pStreamHead->swFlag == streamHEADERFLAG);
    m_pStreamHead->byStreamType=streamType;
}

////////////////获取帧流信息
NVMS_DEFINE::STREAM_HEADER_INFO & CFrameDataEx::GetStreamInfoOfFrame() 
{
    return *m_pStreamHead;
}

///////////////获取帧的通道ID
const GUID &CFrameDataEx::GetChannelID()
{
    if(m_pStreamHead->swFlag == streamHEADERFLAG)
    {
        return m_pStreamHead->nodeID;
	}

	/*ASSERT(FALSE);*/
	return GUID_NULL;
}

//////////////设置发送索引的值
bool CFrameDataEx::SetSendIndex(tuint32 dwSendIndex)
{
    ASSERT(m_pStreamHead->swFlag == streamHEADERFLAG);
    m_pStreamHead->dwSendFrameIndex = dwSendIndex;
    return true;
}

tuint32 CFrameDataEx::GetSendIndex()
{
    return   m_pStreamHead->dwSendFrameIndex;
}
