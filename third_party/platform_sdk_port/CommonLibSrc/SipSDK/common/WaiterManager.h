#ifndef CWAITERMANAGER_H
#define CWAITERMANAGER_H
#include <map>
#include <string>
#include "Lock.h"
#include "SipDefs.h"
class CResponseWaiter;
struct osip_message;

class CWaiterManager
{
public:
    CWaiterManager();
    bool WakeUpWaitResponse( const osip_message* message, SipData& data );
	bool WakeUpWaitResponse( std::string& identifier,SipData& data );
    int WaitResponse(CResponseWaiter* waiter, int timeout, SipData* result);
    void CancleWaitResponse(CResponseWaiter* waiter);
    CResponseWaiter* CreateResponseWaiter(const osip_message* message, SipMethod method);
    std::string MakeIdentifier(const osip_message* message);
private:
    CLock   m_lock;
    std::map<std::string, CResponseWaiter*> m_waiter_map;
};

#endif // CWAITERMANAGER_H
