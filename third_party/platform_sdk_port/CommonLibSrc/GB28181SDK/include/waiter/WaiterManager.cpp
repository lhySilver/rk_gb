#include "WaiterManager.h"
#include "ResponseWaiter.h"


CGbWaiterManager::CGbWaiterManager()
{

}

bool CGbWaiterManager::WakeUpWaitResponse( int sn, const std::string& cmd, const std::string& device_id, int  listNum,  void* msg )
{

    std::string identifier = MakeIdentifier( sn, cmd, device_id);
    if(identifier.empty()) {
        return NULL;
    }
    bool flag = false;
    m_lock.Lock();
    std::map<std::string, CGbResponseWaiter*>::iterator iter  = m_waiter_map.find(identifier);
    if(iter != m_waiter_map.end()) {
         flag = true;
         iter->second->Wakeup(msg,listNum);
    }
    m_lock.Unlock();
         return flag;
}

void CGbWaiterManager::CancleWaitResponse(CGbResponseWaiter* waiter)
{
    m_lock.Lock();
    m_waiter_map.erase( waiter->m_identifiler );
    m_lock.Unlock();
    if( waiter ){
        delete waiter;
    }
}

bool CGbWaiterManager::WaitResponse(CGbResponseWaiter* waiter, int timeout, void* result)
{
    bool res = waiter->WaitTimeout(result,timeout);
     CancleWaitResponse(waiter);
    if( res == false) {
        return  false;
    }
        return  true;
}

std::string CGbWaiterManager::MakeIdentifier(int sn, const  std::string& cmd,  const std::string& device_id)
{
    char buffer[64] ={0};
    sprintf(buffer,"%d", sn);
    std::string id;
    id.append(   buffer ).append("-");
    id.append(   cmd    ).append("-");
    id.append(    device_id   );
    return id;
}

CGbResponseWaiter* CGbWaiterManager::CreateResponseWaiter(int sn, const std::string& cmd, const std::string& device_id)
{
    std::string identifier = MakeIdentifier(sn,   cmd,  device_id);
   if(identifier.empty()){
       return NULL;
   }
   CGbResponseWaiter *p = new CGbResponseWaiter(cmd,identifier);
    if( p ){
       m_lock.Lock();
       m_waiter_map.insert(std::pair<std::string, CGbResponseWaiter*>(identifier,p));
       m_lock.Unlock();
    }
    return p;
}





