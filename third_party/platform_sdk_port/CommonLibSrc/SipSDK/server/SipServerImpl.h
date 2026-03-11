#ifndef CSIPSERVERIMPL_H
#define CSIPSERVERIMPL_H
#include "SipDefs.h"
class CSipEventManager;

class CSipServerImpl
{
public:
    CSipServerImpl();
    ~CSipServerImpl();
    int   Start(SipTransportType type , const NetAddress* local, const char* local_name);
    void Stop();
    void SetHandler(const SipServerHandler* handler);
    int   Connect(const NetAddress* remote , const char* remote_server_name );

    //  sip call
    int   Invite(SipSessionHandle handle, const SipMessage* message, int timeout,  SipData** result );
    int   Info(SipSessionHandle handle, const  SipDialogKey* key ,const SipMessage* message,  int timeout,   SipData** result);
    int   Ack( SipSessionHandle handle, const  SipDialogKey* key ,const SipMessage* message, int timeout,   SipData** result );
    int   Bye( SipSessionHandle handle, const  SipDialogKey* key ,const SipMessage* message, int timeout,   SipData** result );

    // sip subcribe
    int   Subcribe(SipSessionHandle handle, const SipMessage* message, int timeout,  SipData** result );
    int   Notify(SipSessionHandle handle, const  SipDialogKey* key ,const SipMessage* message, int timeout,  SipData** result );
	int   TermainateSubcribe(SipSessionHandle handle, const SipMessage* message,const  SipDialogKey* key, int timeout,  SipData** result );
	int   RefreshSubcribe(SipSessionHandle handle, const SipMessage* message,const  SipDialogKey* key , int timeout,  SipData** result );

    // sip message
    int   Message(SipSessionHandle handle, const SipMessage* message, int timeout,  SipData** result);

    int   Response( SipSessionHandle handle, const  SipDialogKey* key, const SipMessage* message);
    void FreeSipResult(SipData* data);
private:
    CSipEventManager*           m_event_manager;
};

#endif // CSIPSERVERIMPL_H
