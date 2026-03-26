#ifndef CClientInfoManager_H
#define CClientInfoManager_H
#include <string>
#include <set>
#include "Interlocked.h"
#include "SipDefs.h"
#include "Lock.h"
#include "SipUtil.h"

static Interlocked g_client_id;

class ClientInfo
{
public:
    std::string RemoteSipSrvName;
    std::string Username;
    std::string Password;

    int clientId;
    int RegId;
    int Expire;
    bool Reg_flag;
    bool new_reg;
    bool auth_flag;

    std::string RemoteIp;
    int            RemotePort;
    std::string  local_name;
    std::string LocalIp;
    int            LocalPort;

    std::string  from;
    std::string  to;
    std::string  route;
	std::string  contact;
    bool UseZeroConfigHeaders;
    std::string DisplayName;
    std::string StringCode;
    std::string MacAddress;
    std::string LineId;
    std::string Manufacturer;
    std::string Model;
    std::string CustomProtocolVersion;
    ClientInfo(){
        clientId = g_client_id.Increment();
        RegId = -1;
        Reg_flag =  false;
        auth_flag = false;
        UseZeroConfigHeaders = false;
    }
    void SetFromeAndTo( )  {
        char temp[256] = {0};
        snprintf(   temp, 255, "sip:%s@%s:%d",
                                       local_name.c_str()  ,
                                       LocalIp.c_str(),
									   LocalPort

                  );
        from = temp;

        snprintf(   temp, 255, "sip:%s@%s:%d",
                                      RemoteSipSrvName.c_str()  ,
                                      RemoteIp.c_str(),
									  RemotePort

                  );
        to = temp;


        snprintf(   temp, 255, "sip:%s@%s:%d",
                                      RemoteSipSrvName.c_str()  ,
                                      RemoteIp.c_str(),
                                      RemotePort
                );
        route = temp;

		snprintf(   temp, 255, "<sip:%s@%s:%d>",
			                    local_name.c_str(),
			                    LocalIp.c_str(),
			                    LocalPort
			);
		contact = temp;
    }
};

struct PeerInfo
{

    std::string name;
    std::string ip;
    int port;

};

class CClientInfoManager
{
public:
    CClientInfoManager();
    ~CClientInfoManager();
    void InsertClientSession(ClientInfo* handle);
    void DeletClientSession(ClientInfo*  handle);
    bool FindClientSession(ClientInfo*   handle);
    ClientInfo* FindClient(PeerInfo*);
    PeerInfo* GetPeerInfo(osip_message_t* message,bool is_request);
private:
    CLock   m_lock;
    std::set<ClientInfo*>  m_client_set;
};

#endif // CClientInfoManager_H
