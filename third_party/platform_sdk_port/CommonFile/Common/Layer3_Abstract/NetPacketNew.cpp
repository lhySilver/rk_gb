#include "NetPacketNew.h"
#include "PFHelper.h"
#include "MemPoolSDK.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

tuint32 CNetPacketNew::m_dwPageSize=0;

CNetPacketNew::CNetPacketNew(void)
{
	if (0 == m_dwPageSize)
	{
		m_dwPageSize=CPFHelper::GetPageSize();
	}

    m_NetPacketBufferSize=0;
}

CNetPacketNew::~CNetPacketNew(void)
{
    std::list<tuint_ptr>::iterator iter=m_MemList.begin();
    while((iter != m_MemList.end()))
    {
       MEM_POOL_Delete(*iter);
       iter++;
    }
}


bool CNetPacketNew::SaveBuffer(const char* pBuffer, size_t len)
{
    ASSERT(NULL != pBuffer);
    if(len == 0xffffffff)
    {
        tuint_ptr MemBuf=(tuint_ptr)pBuffer;
        MEM_POOL_AddReference(MemBuf);
        m_MemList.push_back(MemBuf);
        m_NetPacketBufferSize += MEM_POOL_GetLength(MemBuf);
    }
    else
    {
        tuint_ptr newBuf=MEM_POOL_New(pBuffer,(tint32)len);
        m_MemList.push_back(newBuf);
        m_NetPacketBufferSize += len;
    }
    return true;
}

bool CNetPacketNew::SaveBufferFront(const char* pBuffer, size_t len)
{
    ASSERT(NULL != pBuffer);
    ASSERT(len > 0);
    tuint_ptr newBuf=MEM_POOL_New(pBuffer,(tint32)len);
    m_MemList.push_front(newBuf);
    m_NetPacketBufferSize += len;

    return false;
}

bool  CNetPacketNew::HaveData(size_t packetLen)
{
    return (m_NetPacketBufferSize >= packetLen);
}

const char* CNetPacketNew::GetPacket(size_t packetLen)
{
    size_t buffer_size = m_NetPacketBufferSize;//未读取的残余数据长度
    if (buffer_size < packetLen)
    {
        return NULL;
    }
    else   //有残留数据
    {
        tuint_ptr memBuf=0;
        size_t offset=0;
        std::list<tuint_ptr>::iterator iter=m_MemList.begin();
        while(iter != m_MemList.end())
        {
            tuint32 tmpBuf=*iter;
            tint32 len=MEM_POOL_GetLength(tmpBuf);
            if(len > (tint32)(packetLen-offset))
            {
                if(memBuf == 0)
                {
                    memBuf=MEM_POOL_NewCopyFrom(tmpBuf);
                }
                else
                {
                    MEM_POOL_Appending(memBuf,tmpBuf);
                }
                MEM_POOL_TruncateBack(memBuf,len-(packetLen-offset));
                break;
            }
            else
            {
                if(memBuf == 0)
                {
                    memBuf=MEM_POOL_NewCopyFrom(tmpBuf);
                }
                else
                {
                    MEM_POOL_Appending(memBuf,tmpBuf);
                }
                iter++;
                offset+=len;
            }
        }
        return (char*)memBuf;
    }
}

void CNetPacketNew::ReleasePack(const char *pBuf)
{
    MEM_POOL_Delete((tuint_ptr)pBuf);
}

bool CNetPacketNew::GetPacketHead(const void *pBuf,size_t packetLen)
{
    size_t buffer_size = m_NetPacketBufferSize;//未读取的残余数据长度
    if (packetLen > buffer_size)
    {
        return false;
    }
    else//有残留数据
    {
        size_t offset=0;
        std::list<tuint_ptr>::iterator iter=m_MemList.begin();
        while((iter != m_MemList.end()) && (offset < packetLen))
        {
            tint32 len=MEM_POOL_GetLength(*iter);
            if(len >= (tint32)(packetLen-offset))
            {
                MEM_POOL_Memcpy(*iter,(BYTE*)pBuf+offset,packetLen-offset);
                break;
            }
            else
            {
                MEM_POOL_Memcpy(*iter,(BYTE*)pBuf+offset,len);
                iter++;
                offset+=len;
            }
        }
        return true;
    }
}

bool CNetPacketNew::TruncateFront(size_t resetsize)
{
    size_t buffer_size = m_NetPacketBufferSize;//未读取的残余数据长度
    if (resetsize > buffer_size)
    {
        TASSERT(false);
        return false;
    }
    else
    {
        size_t total=resetsize;
        std::list<tuint_ptr>::iterator iter=m_MemList.begin();
        while((iter != m_MemList.end()) && (total > 0))
        {
            tuint_ptr memBuf=*iter;
            tint32 len=MEM_POOL_GetLength(memBuf);
            if((tint32)total >= len)
            {
                MEM_POOL_Delete(memBuf);
                m_MemList.erase(iter++);
                total-=len;
            }
            else
            {
                MEM_POOL_TruncateFront(memBuf,total);
                break;
            }
        }
        m_NetPacketBufferSize-=resetsize;
        return true;
    }
}
