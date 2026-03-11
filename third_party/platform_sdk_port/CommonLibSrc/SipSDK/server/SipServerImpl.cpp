#include "SipServerImpl.h"
#include "common/SipEventManager.h"

CSipServerImpl::CSipServerImpl()
{
    m_event_manager = new CSipEventManager;
}

CSipServerImpl::~CSipServerImpl()
{
    if(m_event_manager) {
        delete m_event_manager;
    }
}

void CSipServerImpl::FreeSipResult(SipData* data)
{
    FreeSipData(data);
}

int CSipServerImpl::Start(SipTransportType type , const NetAddress* local, const char* local_name)
{
      return  m_event_manager->StartSipListen(type,local,local_name);
}

void CSipServerImpl::Stop()
{
     m_event_manager->Stop();
}

void CSipServerImpl::SetHandler(const SipServerHandler* handler)
{
     m_event_manager->SetSipHandler(handler);
}

    //  sip call
int   CSipServerImpl::Invite(SipSessionHandle handle, const SipMessage* message, int timeout,  SipData** result )
{
    ClientInfo* client_info = (ClientInfo*)handle;
    return  m_event_manager->SendSipResquest(
                                                                      kSipInviteMethod,
                                                                      timeout,
                                                                      client_info,
                                                                      NULL,
                                                                      message,
                                                                      result
                                                                    );
}

int   CSipServerImpl::Info(SipSessionHandle handle, const  SipDialogKey* key ,const SipMessage* message,  int timeout,   SipData** result)
{
    ClientInfo* client_info = (ClientInfo*)handle;
    return  m_event_manager->SendSipResquest(
                                                                      kSipInfoMethod,
                                                                      timeout,
                                                                      client_info,
                                                                      key,
                                                                      message,
                                                                      result
                                                                    );
}


int   CSipServerImpl::Ack( SipSessionHandle handle, const  SipDialogKey* key ,const SipMessage* message, int timeout,   SipData** result )
{
    ClientInfo* client_info = (ClientInfo*)handle;
    return  m_event_manager->SendSipResquest(
                                                                      kSipAckMethod,
                                                                      timeout,
                                                                      client_info,
                                                                      key,
                                                                      message,
                                                                      result
                                                                    );
}

int   CSipServerImpl::Bye( SipSessionHandle handle, const  SipDialogKey* key ,const SipMessage* message, int timeout,   SipData** result )
{
    ClientInfo* client_info = (ClientInfo*)handle;
    return  m_event_manager->SendSipResquest(
                                                                      kSipByeMethod,
                                                                      timeout,
                                                                      client_info,
                                                                      key,
                                                                      message,
                                                                      result
                                                                    );


}



    // sip subcribe

int   CSipServerImpl::Subcribe(SipSessionHandle handle, const SipMessage* message, int timeout,  SipData** result )
{
    ClientInfo* client_info = (ClientInfo*)handle;
    return  m_event_manager->SendSipResquest(
                                                                      kSipSubscribeMethod,
                                                                      timeout,
                                                                      client_info,
                                                                      NULL,
                                                                      message,
                                                                      result
                                                                    );
}

int   CSipServerImpl::RefreshSubcribe(SipSessionHandle handle, const SipMessage* message, const  SipDialogKey* key ,int timeout,  SipData** result )
{
	ClientInfo* client_info = (ClientInfo*)handle;
	return  m_event_manager->SendSipResquest(
																	kSipRefreshSubscribeMethod,
																	timeout,
																	client_info,
																	key,
																	message,
																	result
																	);
}

int CSipServerImpl::TermainateSubcribe(SipSessionHandle handle, const SipMessage* message,const  SipDialogKey* key , int timeout,  SipData** result )
{
	ClientInfo* client_info = (ClientInfo*)handle;
	return  m_event_manager->SendSipResquest(
																	kSipCancelSubscribeMethod,
																	timeout,
																	client_info,
																	key,
																	message,
																	result
																	);
}
int   CSipServerImpl::Notify(SipSessionHandle handle, const  SipDialogKey* key ,const SipMessage* message, int timeout,  SipData** result )
{

    ClientInfo* client_info = (ClientInfo*)handle;
    return  m_event_manager->SendSipResquest(
                                                                      kSipNotifyMethod,
                                                                      timeout,
                                                                      client_info,
                                                                      key,
                                                                      message,
                                                                      result
                                                                    );
}


    // sip message
int   CSipServerImpl::Message(SipSessionHandle handle, const SipMessage* message, int timeout,  SipData** result)
{

    ClientInfo* client_info = (ClientInfo*)handle;
    return  m_event_manager->SendSipResquest(
                                                                      kSipMessageMethod,
                                                                      timeout,
                                                                      client_info,
                                                                      NULL,
                                                                      message,
                                                                      result
                                                                     );
}

int   CSipServerImpl::Response( SipSessionHandle handle, const  SipDialogKey* key, const SipMessage* message)
{
    ClientInfo* client_info = (ClientInfo*)handle;
     return  m_event_manager->SendSipResponse(client_info, key, message);
}
