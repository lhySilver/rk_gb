#ifndef __GAT1400_CLIENT_SERVICE_H__
#define __GAT1400_CLIENT_SERVICE_H__

#include <atomic>
#include <list>
#include <map>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

#include "ProtocolExternalConfig.h"
#define ConnectParam GAT1400ConnectParam
#include "CMS1400Struct.h"
#undef ConnectParam
#include "LowerGAT1400Observer.h"

namespace protocol
{

class GAT1400ClientService
{
public:
    struct HttpResponse
    {
        int status_code;
        std::map<std::string, std::string> headers;
        std::string body;

        HttpResponse() : status_code(0) {}
    };

    GAT1400ClientService();
    ~GAT1400ClientService();

    int Start(const ProtocolExternalConfig& cfg, const GbRegisterParam& gbRegister);
    void Stop();
    int Reload(const ProtocolExternalConfig& cfg, const GbRegisterParam& gbRegister);

    bool IsStarted() const;

    void AddSubscribeObserver(CLower1400SubscribeObserver* observer);
    void RemoveSubscribeObserver(CLower1400SubscribeObserver* observer);
    void AddRegistStatusObserver(CLower1400RegistStatusObserver* observer);
    void RemoveRegistStatusObserver(CLower1400RegistStatusObserver* observer);

    int GetTime(std::string& outTime);
    int PostFaces(const std::list<GAT_1400_Face>& faceList);
    int PostPersons(const std::list<GAT_1400_Person>& personList);
    int PostMotorVehicles(const std::list<GAT_1400_Motor>& motorList);
    int PostNonMotorVehicles(const std::list<GAT_1400_NonMotor>& nonMotorList);
    int PostThings(const std::list<GAT_1400_Thing>& thingList);
    int PostScenes(const std::list<GAT_1400_Scene>& sceneList);
    int PostVideoSlices(const std::list<GAT_1400_VideoSliceSet>& videoSliceList);
    int PostImages(const std::list<GAT_1400_ImageSet>& imageList);
    int PostFiles(const std::list<GAT_1400_FileSet>& fileList);
    int PostAnalysisRules(const std::list<GAT_1400_AnalysisRule>& ruleList);
    int PostVideoLabels(const std::list<GAT_1400_VideoLabel>& labelList);
    int PostDispositions(const std::list<GAT_1400_Disposition>& dispositionList);
    int PostDispositionNotifications(const std::list<GAT_1400_Disposition_Notification>& notificationList);
    int PostSubscribes(const std::list<GAT_1400_Subscribe>& subscribeList);
    int PostSubscribeNotifications(const std::list<GAT_1400_Subscribe_Notification>& notificationList,
                                   const std::string& url);
    int PostApes(const std::list<GAT_1400_Ape>& apeList);

    std::list<GAT_1400_Subscribe> GetSubscriptions() const;
    int PostJsonWithResponseList(const char* action, const char* path, const std::string& body);

private:
    int StartServerLocked();
    void StopServerLocked();
    void ServerLoop();
    void HandleServerConnection(int clientFd);
    int HandleSubscribeHttp(const std::string& method,
                            const std::string& path,
                            const std::string& query,
                            const std::string& body,
                            int& outStatus,
                            std::string& outBody);

    int RegisterNow();
    int UnregisterNow();
    int SendKeepaliveNow();
    void HeartbeatLoop();

    int ExecuteRequest(const ProtocolExternalConfig& cfg,
                       const std::string& deviceId,
                       const std::string& method,
                       const std::string& path,
                       const std::string& body,
                       HttpResponse& response,
                       const std::string* overrideUrl = NULL) const;

    int PostJsonWithResponseStatus(const char* action,
                                   const char* path,
                                   const std::string& body,
                                   const std::string* overrideUrl = NULL);
    int PostBinaryData(const char* action, const std::string& path, const std::string& data);

    int SnapshotConfig(ProtocolExternalConfig& cfg, GbRegisterParam& gbRegister, std::string& deviceId) const;
    void UpdateRegistState(regist_state state);

    void DispatchSubscribeAdd(const GAT_1400_Subscribe& subscribe);
    void DispatchSubscribeUpdate(const GAT_1400_Subscribe& subscribe);
    void DispatchSubscribeDelete(const std::string& subscribeId);

private:
    mutable std::mutex m_state_mutex;
    ProtocolExternalConfig m_cfg;
    GbRegisterParam m_gb_register;
    std::string m_device_id;
    bool m_started;
    bool m_registered;
    regist_state m_regist_state;
    int m_listen_fd;
    std::thread m_server_thread;
    std::atomic<bool> m_server_running;
    std::thread m_heartbeat_thread;
    std::atomic<bool> m_heartbeat_running;

    mutable std::mutex m_subscribe_mutex;
    std::map<std::string, GAT_1400_Subscribe> m_subscriptions;

    mutable std::mutex m_observer_mutex;
    std::vector<CLower1400SubscribeObserver*> m_subscribe_observers;
    std::vector<CLower1400RegistStatusObserver*> m_regist_observers;
};

}

#endif


