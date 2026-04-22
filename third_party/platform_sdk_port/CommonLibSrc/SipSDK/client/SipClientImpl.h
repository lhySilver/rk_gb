#ifndef CSIPCLIENTIMPL_H
#define CSIPCLIENTIMPL_H
#include "SipDefs.h"

class SipServerHandlerImpl : public SipServerHandler
{

     void OnRequest(SipSessionHandle handle,SipMethod  method,const SipData* data)
    {
        client_handler->OnRequest(method,data);
    }

     void OnResponse(SipSessionHandle handle,SipMethod  method, const SipData* data)
     {
         client_handler->OnResponse(method,data);
     }

     int   OnClientRegist(SipSessionHandle handle, const SipRegistParam* regist, const SipConnectParam* param)
     {
           return 1;      
     }

   public:
     SipClientHandler* client_handler;
};

class CSipEventManager;
struct ClientInfo;

class CSipClientImpl
{
public:
    CSipClientImpl();
    ~CSipClientImpl();
    int   Start(SipTransportType type , const NetAddress* local,const char*local_name);
    void Stop();
    const char* GetLocalIp() const;
    uint16_t GetLocalPort() const;
    void SetHandler(const SipClientHandler* handler);
    // Sip register
    int   Register(const SipRegistParam* mesage ,const SipConnectParam* info,int timeout ,SipData** result );
    int   UnRegister();

    //  sip call
    int   Invite(const SipMessage* message, int timeout,  SipData** result );
    int   Info(const  SipDialogKey* key ,const SipMessage* message,  int timeout,   SipData** result);
    int   Ack( const  SipDialogKey* key ,const SipMessage* message, int timeout,   SipData** result );
    int   Bye( const  SipDialogKey* key ,const SipMessage* message, int timeout,   SipData** result );


    // sip subcribe
    int   Subcribe(const SipMessage* message, int timeout,  SipData** result );
    int   Notify(const  SipDialogKey* key ,const SipMessage* message, int timeout,  SipData** result );


    // sip message
    int   Message(const SipMessage* message, int timeout,  SipData** result);

	int MessageToStr(const SipMessage* message, char** buf, size_t* buf_len);

    int   Response( const  SipDialogKey* key, const SipMessage* message);

	int   RequestWithCall(const  SipDialogKey* key, const SipMessage* message, int timeout, SipData** result);

    void FreeSipResult(SipData* data);
	void FreeSipBuffer(char** buf);
private:
    SipServerHandlerImpl*       m_sip_server_handler;
    CSipEventManager*           m_event_manager;
    ClientInfo*                          m_client_info;
};

#endif // CSIPCLIENTIMPL_H
