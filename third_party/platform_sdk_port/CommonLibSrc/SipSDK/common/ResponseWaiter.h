#ifndef CResponseWaiter_H
#define CResponseWaiter_H
#include "Lock.h"
#include "Interlocked.h"
#include "SipDefs.h"


class CResponseWaiter
{
public:
    CResponseWaiter(std::string identifiler);
    ~CResponseWaiter();
    void AddRef();
    void UnRef();
    void Wakeup(const SipData *Data );
    bool WaitTimeout(SipData *Result, int timeout );
public:
     std::string     m_identifiler;
private:
   SipData        m_sip_data;
   CMyEvent      m_wait_event;
    Interlocked    m_recv_flag;
    Interlocked    m_reference_count;
 /*   SipMethod     m_method;
    std::string     m_call_id;
    std::string     m_tag;
    uint32_t        m_cseq; */
};

#endif // CResponseWaiter_H
