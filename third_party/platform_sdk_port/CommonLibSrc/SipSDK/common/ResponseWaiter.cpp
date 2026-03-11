#include "ResponseWaiter.h"

CResponseWaiter::CResponseWaiter(std::string identifiler)
                        :m_identifiler(identifiler)
{
      m_recv_flag.SetValue(0);
      m_reference_count.SetValue(0);
}

CResponseWaiter::~CResponseWaiter()
{}
void CResponseWaiter::AddRef()
{
    m_reference_count.Increment();
}

void CResponseWaiter::UnRef()
{
     if(m_reference_count == 0 ){
         delete this;
     }
     m_reference_count .Decrement();
}

void CResponseWaiter::Wakeup(const SipData *pData )
{
        memcpy(&m_sip_data,  pData, sizeof(*pData) );
        m_recv_flag.SetValue(1);
        m_wait_event.SetEvent();
}

bool CResponseWaiter::WaitTimeout(SipData *result, int timeout )
{
        m_wait_event.WaitEvent(timeout);
        if(  m_recv_flag   ) {
            memcpy(result, &m_sip_data, sizeof(m_sip_data) );
            return true;
        }
            return false;
}
