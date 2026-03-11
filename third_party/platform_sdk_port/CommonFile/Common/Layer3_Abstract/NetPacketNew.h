
#ifndef __NET_PACKET__
#define __NET_PACKET__

#include <list>
#include "base_type_define.h"
#include "ShareSDK.h"


class CNetPacketNew
{
public:
    CNetPacketNew(void);
    virtual ~CNetPacketNew(void);

    bool  HaveData(size_t packetLen);
    const char * GetPacket(size_t packetLen);
    void ReleasePack(const char *pBuf);
    bool GetPacketHead(const void *pBuf,size_t packetLen);
    bool SaveBuffer(const char* pBuffer, size_t len);
    bool SaveBufferFront(const char* pBuffer, size_t len);
    bool TruncateFront(size_t resetsize);
private:
	static tuint32 m_dwPageSize;
    size_t m_NetPacketBufferSize;

    std::list<tuint_ptr> m_MemList;
};


#endif //__NET_PACKET__
