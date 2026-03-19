#include "ClientInfoManager.h"
#include "eXosip2/eXosip.h"
#include "ShareSDK.h"

CClientInfoManager::CClientInfoManager()
{

}

CClientInfoManager::~CClientInfoManager()
{

}

void CClientInfoManager::InsertClientSession(ClientInfo* handle)
{
    m_lock.Lock();
    m_client_set.insert(handle);
    m_lock.Unlock();
}

void CClientInfoManager::DeletClientSession(ClientInfo* handle)
{
    m_lock.Lock();
    m_client_set.erase(handle);
    m_lock.Unlock();
}

bool CClientInfoManager::FindClientSession(ClientInfo* handle)
{
    m_lock.Lock();
    std::set<ClientInfo*>::iterator iter = m_client_set.find(handle);
    if( iter != m_client_set.end() ){
         m_lock.Unlock();
        return true;
    }
    m_lock.Unlock();
        return false;
}

ClientInfo* CClientInfoManager::FindClient(PeerInfo* info)
{
    ClientInfo* client = NULL;
    ClientInfo* temp = NULL;
    m_lock.Lock();
    std::set<ClientInfo*>::iterator iter = m_client_set.begin();
    while( iter != m_client_set.end() ) {

        temp = *iter;

        if(  info->name == temp->RemoteSipSrvName ) {
           client = temp;
            break;
        }

		if (info->ip == temp->RemoteIp && info->port == temp->RemotePort ) {
			client = temp;
			break;
		}
            iter++;
    }
    if (client) {
        TVT_LOG_INFO("sip peer matched client"
                     << " peer_name=" << info->name
                     << " peer=" << info->ip << ":" << info->port
                     << " remote_name=" << client->RemoteSipSrvName
                     << " remote=" << client->RemoteIp << ":" << client->RemotePort);
    } else {
        TVT_LOG_ERROR("sip peer match failed"
                      << " peer_name=" << info->name
                      << " peer=" << info->ip << ":" << info->port
                      << " client_count=" << m_client_set.size());
    }
    m_lock.Unlock();
    return client;
}

PeerInfo* CClientInfoManager::GetPeerInfo(osip_message_t *pMsg, bool is_request)
{
	if (!pMsg) {
		return NULL;
	}

	osip_header_t * dest = NULL;
	osip_message_header_get_byname(pMsg, "Host", 0, &dest);
	if (dest == NULL || dest->hvalue == NULL ) {
		return NULL;
	}

	PeerInfo* peer = new PeerInfo;
	char host_ip[64] = { 0 };
	sscanf(dest->hvalue, "%[^:]:%d", host_ip,&(peer->port));
	peer->ip = host_ip;
    std::string name_source = "none";
	osip_contact_t * pContact = NULL;
	if (OSIP_SUCCESS == osip_message_get_contact(pMsg, 0, &pContact)
		&& pContact && pContact->url && pContact->url->host && pContact->url->port)
	{
//		osip_uri_t * pUri = pContact->url;

		if (pContact->url->username) {
			peer->name = pContact->url->username;
            name_source = "contact.username";
		}
		else {

			if (pMsg->from) {

				osip_uri_t * pUri = osip_to_get_url(pMsg->from);

				if (NULL != pUri && pUri->username) {
                    peer->name = pUri->username;
                    name_source = "from.username_fallback";
				}

			}

		}
	}
	else
	{

		if (!is_request) {

			if (pMsg->to)
			{
				osip_uri_t * pUri = osip_to_get_url(pMsg->to);
				if (NULL != pUri)
				{
					char * pUserName = osip_uri_get_username(pUri);
						if (NULL != pUserName)
						{
							peer->name = pUserName;
                            name_source = "to.username";
					}
				}
			}

		}
		else {

			if (pMsg->from)
			{
				osip_uri_t * pUri = osip_to_get_url(pMsg->from);
				if (NULL != pUri)
				{
					char * pUserName = osip_uri_get_username(pUri);
						if (NULL != pUserName)
						{
							peer->name = pUserName;
                            name_source = "from.username";
					}
				}
			}

		}
	}
    TVT_LOG_INFO("sip peer parsed"
                 << " is_request=" << (is_request ? 1 : 0)
                 << " host=" << dest->hvalue
                 << " peer_name=" << peer->name
                 << " peer=" << peer->ip << ":" << peer->port
                 << " name_source=" << name_source);
	return peer;
}
