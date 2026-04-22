#include "SipStackSDK.h"
#include "client/SipClientImpl.h"
#include "server/SipServerImpl.h"

SipUserAgentClient::SipUserAgentClient()
{
   m_client_impl = new CSipClientImpl;
}

SipUserAgentClient::~SipUserAgentClient()
{
    if(m_client_impl){
        delete m_client_impl;
    }
}

void SipUserAgentClient::FreeSipResult(SipData* data)
{
    m_client_impl->FreeSipResult(data);
}

void SipUserAgentClient::FreeSipBuffer(char** buf)
{
	m_client_impl->FreeSipBuffer(buf);
}

int SipUserAgentClient::Start(SipTransportType type , const NetAddress* addr, const char* local_name)
{
    return m_client_impl->Start(type , addr, local_name);
}

void SipUserAgentClient::Stop()
{
    return m_client_impl->Stop();
}

const char* SipUserAgentClient::GetLocalIp() const
{
    return m_client_impl->GetLocalIp();
}

uint16_t SipUserAgentClient::GetLocalPort() const
{
    return m_client_impl->GetLocalPort();
}

void SipUserAgentClient::SetHandler(const SipClientHandler* handler)
{
    return m_client_impl->SetHandler(handler);
}

// Sip register
int SipUserAgentClient::Register(const SipRegistParam* message, const SipConnectParam* info, int timeout , SipData** result )
{
    return m_client_impl->Register(message,info,timeout,result);
}

int SipUserAgentClient::UnRegister()
{
   return m_client_impl->UnRegister();
}

//  sip call
int SipUserAgentClient::Invite(const SipMessage* message, int timeout,  SipData** result )
{
    return m_client_impl->Invite(message, timeout, result);
}

int SipUserAgentClient::Info(const  SipDialogKey* key ,const SipMessage* message,  int timeout,   SipData** result)
{
     return m_client_impl->Info(key,message, timeout, result);
}

int SipUserAgentClient::Ack( const  SipDialogKey* key ,const SipMessage* message, int timeout,   SipData** result )
{
    return m_client_impl->Ack(key,message, timeout, result);
}

int SipUserAgentClient::Bye( const  SipDialogKey* key ,const SipMessage* message, int timeout,   SipData** result )
{
    return m_client_impl->Bye(key,message, timeout, result);
}


// sip subcribe
int SipUserAgentClient::Subcribe(const SipMessage* message, int timeout,  SipData** result )
{
    return m_client_impl->Subcribe(message, timeout, result);
}
int SipUserAgentClient::Notify(const  SipDialogKey* key ,const SipMessage* message, int timeout,  SipData** result )
{
    return m_client_impl->Notify(key,message, timeout, result);
}


// sip message
int SipUserAgentClient::Message(const SipMessage* message, int timeout,  SipData** result)
{
     return m_client_impl->Message(message, timeout, result);
}

int SipUserAgentClient::MessageToStr(const SipMessage* message, char** buf, size_t* buf_len)
{
	return m_client_impl->MessageToStr(message,buf,buf_len);
}

int SipUserAgentClient::Response( const  SipDialogKey* key,  const SipMessage* message)
{
    return m_client_impl->Response( key, message);
}

int  SipUserAgentClient::RequestWithCall(const  SipDialogKey* key, const SipMessage* message, int timeout, SipData** result)
{
	return m_client_impl->RequestWithCall(key, message,  timeout, result);
}


/* server impl*/

SipUserAgentServer::SipUserAgentServer()
{
    m_server_impl = new CSipServerImpl;
}

SipUserAgentServer::~SipUserAgentServer()
{
    if(m_server_impl){
        delete m_server_impl;
    }
}



int SipUserAgentServer::Start(SipTransportType type , const NetAddress* local,const char* local_name)
{
    return m_server_impl->Start( type , local, local_name);
}

void SipUserAgentServer::Stop()
{
    m_server_impl->Stop();
}

void SipUserAgentServer::SetHandler(const SipServerHandler* handler)
{
     return m_server_impl->SetHandler(handler);
}

//  sip call
int SipUserAgentServer::Invite(SipSessionHandle handle, const SipMessage* message, int timeout,  SipData** result )
{
     return m_server_impl->Invite(handle,  message, timeout,  result);
}
int SipUserAgentServer::Info(SipSessionHandle handle, const  SipDialogKey* key ,const SipMessage* message,  int timeout,   SipData** result)
{
     return m_server_impl->Info(handle,key, message, timeout,  result);
}
int SipUserAgentServer::Ack( SipSessionHandle handle, const  SipDialogKey* key ,const SipMessage* message, int timeout,   SipData** result )
{
      return m_server_impl->Ack(handle,key, message, timeout,  result);
}
int SipUserAgentServer::Bye( SipSessionHandle handle, const  SipDialogKey* key ,const SipMessage* message, int timeout,   SipData** result )
{
      return m_server_impl->Bye(handle,key, message, timeout,  result);
}

// sip subcribe
int SipUserAgentServer::Subcribe(SipSessionHandle handle, const SipMessage* message, int timeout,  SipData** result )
{
      return m_server_impl->Subcribe(handle, message, timeout,  result);
}

int SipUserAgentServer::RefrsehSubcribe(SipSessionHandle handle, const SipMessage* message,const  SipDialogKey* key , int timeout,  SipData** result )
{
	return m_server_impl->RefreshSubcribe(handle, message, key, timeout, result);
}

int SipUserAgentServer::TermainateSubcribe(SipSessionHandle handle, const SipMessage* message,const  SipDialogKey* key , int timeout,  SipData** result)
{
	return m_server_impl->TermainateSubcribe(handle, message, key, timeout, result);
}

int SipUserAgentServer::Notify(SipSessionHandle handle, const  SipDialogKey* key ,const SipMessage* message, int timeout,  SipData** result )
{
     return m_server_impl->Notify(handle, key,message, timeout,  result);
}

// sip message
int SipUserAgentServer::Message(SipSessionHandle handle, const SipMessage* message, int timeout,  SipData** result)
{
    return m_server_impl->Message(handle, message,timeout,  result);
}

int SipUserAgentServer::Response( SipSessionHandle handle, const  SipDialogKey* key, const SipMessage* message)
{
     return m_server_impl->Response(handle, key, message);
}


void SipUserAgentServer::FreeSipResult(SipData* data)
{
    m_server_impl->FreeSipResult(data);
}


