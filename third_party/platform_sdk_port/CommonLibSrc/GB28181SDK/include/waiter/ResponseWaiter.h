#ifndef CGbResponseWaiter_H
#define CGbResponseWaiter_H
#include "Lock.h"
#include "Interlocked.h"
#include "SipDefs.h"


class CGbResponseWaiter
{
public:
    CGbResponseWaiter(std::string cmd, std::string identifiler);
    ~CGbResponseWaiter();
    void AddRef();
    void UnRef();
    void Wakeup(void* msg , uint32_t sum );
    bool WaitTimeout(void *result, int timeout );
public:
    std::string     m_identifiler;
private:
   void OnDeviceStatus(void* msg ,  uint32_t sum);
   bool OnDeviceCatalog(void* msg ,  uint32_t sum);
   bool OnDeviceRecord(void* msg ,  uint32_t sum);
   void OnDeviceInfo(void* msg ,  uint32_t sum);
   void OnDevicePreset(void* msg ,  uint32_t sum);

private:
     std::string     m_cmd;
     uint32_t        m_sum;
     uint32_t        m_currentNum;
     void*            m_buffer;
    CMyEvent       m_wait_event;
    Interlocked     m_recv_flag;
    Interlocked     m_reference_count;
 /*   SipMethod     m_method;
    std::string     m_call_id;
    std::string     m_tag;
    uint32_t        m_cseq; */
};

#endif // CGbResponseWaiter_H

