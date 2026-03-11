#ifndef CGBClientInfoManager_H
#define CGBClientInfoManager_H
#include "GB28181Defs.h"
#include "Lock.h"
#include <map>
#include <string>

class CGBClientInfoManager
{
public:
    CGBClientInfoManager();
    void Insert(SessionHandle handle, ConnectParam* param );
    void DeleteClient(SessionHandle handle);
    bool GenerateSipAddress(SessionHandle handle,
                                           const std::string& device_id,
                                           std::string & from,
                                           std::string& to,
                                           std::string& route );
public:
    std::string m_local_ip;
    uint16_t   m_local_port;
    std::string m_local_code;
private:
    CLock  m_lock;
    std::map<SessionHandle, ConnectParam*> m_client_map;
};

#endif // CGBClientInfoManager_H
