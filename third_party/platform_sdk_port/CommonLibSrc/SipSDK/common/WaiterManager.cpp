#include "WaiterManager.h"
#include "ResponseWaiter.h"
#include "eXosip2/eXosip.h"
#include "SipDefs.h"
#include "SipUtil.h"

CWaiterManager::CWaiterManager()
{

}

bool CWaiterManager::WakeUpWaitResponse( const osip_message* message,SipData& data )
{

    std::string identifier = MakeIdentifier(message);
    if(identifier.empty()) {
        return NULL;
    }
    bool flag = false;
    m_lock.Lock();
    std::map<std::string, CResponseWaiter*>::iterator iter  = m_waiter_map.find(identifier);
    if(iter != m_waiter_map.end()) {
         flag = true;
         iter->second->Wakeup(&data );
    }
    m_lock.Unlock();
         return flag;
}

bool CWaiterManager::WakeUpWaitResponse( std::string& identifier,SipData& data )
{
	if(identifier.empty()) {
		return NULL;
	}
	bool flag = false;
	m_lock.Lock();
	std::map<std::string, CResponseWaiter*>::iterator iter  = m_waiter_map.find(identifier);
	if(iter != m_waiter_map.end()) {
		flag = true;
		iter->second->Wakeup(&data );
	}
	m_lock.Unlock();
	return flag;
}



void CWaiterManager::CancleWaitResponse(CResponseWaiter* waiter)
{
    m_lock.Lock();
    m_waiter_map.erase( waiter->m_identifiler );
    m_lock.Unlock();
    if( waiter ){
        delete waiter;
    }
}

int CWaiterManager::WaitResponse(CResponseWaiter* waiter, int timeout, SipData* result)
{
    bool res = waiter->WaitTimeout(result,timeout);
     CancleWaitResponse(waiter);
    if( res == false) {
        return kSipWaitTimeout;
    }
        return  kSipSuccess;
}

std::string CWaiterManager::MakeIdentifier(const osip_message* message)
{
    char *buffer = NULL;
    std::string call_id;
    std::string tag;
    std::string method;
    //int cseq;
    if( osip_call_id_to_str(message->call_id, &buffer ) ) {
        return "";
    }
    call_id = buffer;
    osip_free(buffer);

    if( !message->cseq->method ) {
        return "";
    }
    method = message->cseq->method;

    osip_uri_param_t *pTagParam = NULL;
    if( !osip_from_get_tag(message->from, &pTagParam) ) {
        tag = pTagParam->gvalue;
    }else{
        return "";
    }

    std::string id;
    id.append(   call_id  ).append("-");
    id.append(   tag    ).append("-");
    id.append(  method   );
    return id;
}

CResponseWaiter* CWaiterManager::CreateResponseWaiter(const osip_message* message, SipMethod method)
{

   std::string identifier = MakeIdentifier(message);
   if(identifier.empty()){
       return NULL;
   }
   CResponseWaiter *p = new CResponseWaiter(identifier);
    if( p ){
       m_lock.Lock();
       m_waiter_map.insert(std::pair<std::string, CResponseWaiter*>(identifier,p));
       m_lock.Unlock();
    }
    return p;
}




