#include "GBClientInfoManager.h"
#include <stdlib.h>

CGBClientInfoManager::CGBClientInfoManager()
{

}

void CGBClientInfoManager::Insert(SessionHandle handle, ConnectParam* param )
{
    ConnectParam* connect = new ConnectParam;
    memcpy(connect, param, sizeof(*connect));
    m_lock.Lock();
    m_client_map.insert(std::make_pair(handle,connect ));
    m_lock.Unlock();
}

void CGBClientInfoManager::DeleteClient(SessionHandle handle)
{
    ConnectParam* connect = NULL;
    m_lock.Lock();
   std::map<SessionHandle, ConnectParam*>::iterator iter = m_client_map.find(handle);
   if(iter!=m_client_map.end()) {
       connect = iter->second;
       m_client_map.erase(iter);
   }
    m_lock.Unlock();
    if(connect){
        delete connect;
    }
}

bool CGBClientInfoManager::GenerateSipAddress(SessionHandle handle,
                                                                      const std::string& device_id,
                                                                      std::string & from,
                                                                      std::string& to,
                                                                      std::string& route )
{
    bool result = false;
    std::string ip;
    uint16_t     port;

   m_lock.Lock();
   std::map<SessionHandle, ConnectParam*>::iterator iter = m_client_map.find(handle);
   if(iter!=m_client_map.end()) {
       ConnectParam* connect = iter->second;
       ip = connect->ip;
       port = connect->port;
       result = true;
   }
   m_lock.Unlock();

   if(!result) {
       return false;
   }
   char buffer[256] = {0};
   snprintf(   buffer, 255, "sip:%s@%s:%d",
                                  m_local_code.c_str()  ,
                                  m_local_ip.c_str(),
                                  m_local_port

             );
   from = buffer;

   snprintf(   buffer, 255, "sip:%s@%s:%d",
                                    device_id.c_str(),
                                    ip.c_str(),
                                    port
              );
   to = buffer;


   snprintf(   buffer, 255, "sip:%s@%s:%d",
                                      device_id.c_str(),
                                      ip.c_str(),
                                      port
              );
   route = buffer;
       return true;
}
