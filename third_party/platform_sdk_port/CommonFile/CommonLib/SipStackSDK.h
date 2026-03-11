#ifndef SIPSTACK_H
#define SIPSTACK_H
#include "SipDefs.h"


class CSipClientImpl;
class CSipServerImpl;

class EXPORT_SIP_SDK  SipUserAgentClient
{
public:
    SipUserAgentClient();
    ~SipUserAgentClient();
    int   Start(SipTransportType type , const NetAddress* local,const char* local_name);
    void Stop();
    void SetHandler(const SipClientHandler* handler);

    // Sip register
    int   Register(const SipRegistParam* message, const SipConnectParam* info, int timeout , SipData** result );
    int   UnRegister();

    //  sip call
    int   Invite(const SipMessage* message, int timeout,  SipData** result );
    int   Info(const  SipDialogKey* key ,const SipMessage* message,  int timeout,   SipData** result);
    int   Ack( const  SipDialogKey* key ,const SipMessage* message, int timeout,   SipData** result );
    int   Bye( const  SipDialogKey* key ,const SipMessage* message, int timeout,   SipData** result );
	int   RequestWithCall(const  SipDialogKey* key, const SipMessage* message, int timeout, SipData** result);

    // sip subcribe
    int   Subcribe(const SipMessage* message, int timeout,  SipData** result );
    int   Notify(const  SipDialogKey* key ,const SipMessage* message, int timeout,  SipData** result );

    // sip message
    int   Message(const SipMessage* message, int timeout,  SipData** result);
	int   MessageToStr(const SipMessage* message, char** buf, size_t* buf_len);
    int   Response( const  SipDialogKey* key, const SipMessage* message);
    void FreeSipResult(SipData* data);
	void FreeSipBuffer(char** buf);
private:
    CSipClientImpl* m_client_impl;
};


class EXPORT_SIP_SDK  SipUserAgentServer
{
public:
    SipUserAgentServer();
    ~SipUserAgentServer();
    int   Start(SipTransportType type , const NetAddress* local, const char* local_name);
    void Stop();
    void SetHandler(const SipServerHandler* handler);

    //  sip call
    int   Invite(SipSessionHandle handle, const SipMessage* message, int timeout,  SipData** result );
    int   Info(SipSessionHandle handle, const  SipDialogKey* key ,const SipMessage* message,  int timeout,   SipData** result);
    int   Ack( SipSessionHandle handle, const  SipDialogKey* key ,const SipMessage* message, int timeout,   SipData** result );
    int   Bye( SipSessionHandle handle, const  SipDialogKey* key ,const SipMessage* message, int timeout,   SipData** result );

    // sip subcribe
    int   Subcribe(SipSessionHandle handle, const SipMessage* message, int timeout,  SipData** result );
    int   RefrsehSubcribe(SipSessionHandle handle, const SipMessage* message,const  SipDialogKey* key , int timeout,  SipData** result);
    int   TermainateSubcribe(SipSessionHandle handle, const SipMessage* message,const  SipDialogKey* key , int timeout,  SipData** result);
    int   Notify(SipSessionHandle handle, const  SipDialogKey* key ,const SipMessage* message, int timeout,  SipData** result );

    // sip message
    int   Message(SipSessionHandle handle, const SipMessage* message, int timeout,  SipData** result);

    int   Response( SipSessionHandle handle, const  SipDialogKey* key, const SipMessage* message);
    void FreeSipResult(SipData* data);
private:
    CSipServerImpl* m_server_impl;
};


#endif // SIPSTACK_H
