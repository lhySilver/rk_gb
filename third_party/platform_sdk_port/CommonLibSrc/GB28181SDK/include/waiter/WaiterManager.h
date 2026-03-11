#ifndef CGBWAITERMANAGER_H
#define CGBWAITERMANAGER_H
#include <map>
#include <string>
#include "Lock.h"
#include "SipDefs.h"
class CGbResponseWaiter;
struct osip_message;

class CGbWaiterManager
{
public:
    CGbWaiterManager();
    bool WakeUpWaitResponse( int sn, const std::string& cmd, const std::string& device_id, int  listNum,  void* msg );
    bool   WaitResponse(CGbResponseWaiter* waiter, int timeout, void* msg);
    void CancleWaitResponse(CGbResponseWaiter* waiter);
    CGbResponseWaiter* CreateResponseWaiter( int sn, const std::string& cmd, const std::string& device_id);
private:
    std::string MakeIdentifier(int sn, const  std::string& cmd, const std::string& device_id );
private:
    CLock   m_lock;
    std::map<std::string, CGbResponseWaiter*> m_waiter_map;
};

#endif // CGBWAITERMANAGER_H

