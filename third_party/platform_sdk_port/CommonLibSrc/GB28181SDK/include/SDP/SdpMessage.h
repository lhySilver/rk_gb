#ifndef SDPUTIL_H
#define SDPUTIL_H


#include  "osipparser2/sdp_message.h"
#include  "osipparser2/osip_uri.h"
#include  <string>

    enum SRFlag 
    {
        eUnknown  = 0x0000, 
        eRecvOnly = 0x0001, 
        eSendOnly = 0x0010, 
        eSendRecv = 0x0011, 
    };

    class  CSipUri
	{
	public:
		CSipUri();
		CSipUri(const char *uri);
		CSipUri(osip_uri *uri);
		CSipUri(const CSipUri &uri);
		CSipUri& operator = (const CSipUri &uri);
		CSipUri& operator = (const osip_uri *uri);
		CSipUri& operator = (const char *uri);
		~CSipUri();

		char* getUsername();
		char* getHost();
		char* getPort();
		char* getString();

	private:
		osip_uri *m_uri;
	};

    class  CSdpBandwidth
    {
    public:
        CSdpBandwidth();
        CSdpBandwidth(const char *type, const char *width);
        CSdpBandwidth(const CSdpBandwidth &bandwidth);
        ~CSdpBandwidth();
        CSdpBandwidth& operator = (const CSdpBandwidth &bandwidth);
        int Attach(sdp_bandwidth_t *bandwidth);
        sdp_bandwidth_t* Detach();
        int SetBandwidth(sdp_bandwidth_t *bandwidth);
        int SetType(const char *type);
        const char* GetType();
        int SetWidth(const char *width);
        const char* GetWidth();
        bool Invalid();

    private:
        sdp_bandwidth_t *m_pBandwidth;
    };

    class  CSdpTimeDescr
    {
    public:
        CSdpTimeDescr();
        CSdpTimeDescr(const char *start, const char *stop);
        CSdpTimeDescr(const CSdpTimeDescr &timeDescr);
        ~CSdpTimeDescr();
        CSdpTimeDescr& operator = (const CSdpTimeDescr &timeDescr);
        int Attach(sdp_time_descr_t *timeDescr);
        sdp_time_descr_t* Detach();
        int SetTimeDescr(sdp_time_descr_t *timeDescr);
        int SetTime(const char *start, const char *stop);
        int SetStartTime(const char *startTime);
        const char* GetStartTime();
        int SetStopTime(const char *stopTime);
        const char* GetStopTime();
        int AddRepeat(const char *repeat);
        const char* GetRepeat(int pos);
        bool Invalid();

    private:
        sdp_time_descr_t *m_pTimeDescr;
    };

    class  CSdpKey
    {
    public:
        CSdpKey();
        CSdpKey(const CSdpKey &key);
        ~CSdpKey();
        int Attach(sdp_key_t *key);
        sdp_key_t* Detach();
        int SetKey(sdp_key_t *key);
        int SetType(const char *type);
        const char* GetType();
        int SetData(const char *data);
        const char* GetData();
        bool Invalid();

    private:
        sdp_key_t *m_pKey;
    };

    class  CSdpAttribute
    {
    public:
        CSdpAttribute();
        CSdpAttribute(const char *field);
        CSdpAttribute(const char *field, const char *value);
        CSdpAttribute(const CSdpAttribute &attr);
        ~CSdpAttribute();
        int Attach(sdp_attribute_t *attr);
        sdp_attribute_t* Detach();
        CSdpAttribute& operator = (const CSdpAttribute &attr);
        int SetAttribute(sdp_attribute_t *attr);
        int SetAttribute(const char *field);
        int SetAttribute(const char *field, const char *value);
        std::string GetField();
        std::string GetValue();
        bool Invalid();

    private:
        sdp_attribute_t *m_pAttribute;
    };

    class  CSdpConnection
    {
    public:
        CSdpConnection();
        CSdpConnection(const char *address);
        CSdpConnection(const CSdpConnection &conn);
        ~CSdpConnection();
	    CSdpConnection& operator = (const CSdpConnection &conn);
        int Attach(sdp_connection_t *conn);
        sdp_connection_t* Detach();
        int SetConnection(sdp_connection_t *conn);
        int SetNetType(const char *netType);
        const char* GetNetType();
        int SetAddrType(const char *addrType);
        const char* GetAddrType();
        int SetAddress(const char *address);
        const char* GetAddress();
        int SetAddrMulticastTTL(const char *ttl);
        const char* GetAddrMulticastTTL();
        int SetAddrMulticastNumber(const char *number);
        const char* GetAddrMulticastNumber();
        bool Invalid();

    private:
        sdp_connection_t *m_pConn;
    };

    class  CSdpMedia
    {
    public:
        CSdpMedia();
        CSdpMedia(const CSdpMedia &media);
        ~CSdpMedia();
	    CSdpMedia& operator = (const CSdpMedia &media);
        int Attach(sdp_media_t *media);
        sdp_media_t* Detach();
        int SetMedia(sdp_media_t *media);
        int SetType(const char *type);
        std::string GetType();
        int SetPort11(int port);
        int GetPort();
        int GetPortCount();
        int SetProtocol(const char *protocol);
        std::string GetProtocol();

        // payload
        int SetPayload(const char *payload, int pos = -1);
        int AddPayload(const char *payload);
        std::string GetPayload(int pos);
        int GetPayloadCount();
        int RemovePayload(int pos);
        int ClearPayload();

        std::string GetFmt();
        bool Invalid();

        // i
        int SetInfo(const char *info);
        std::string GetInfo();

        // c
        int AddConnection(CSdpConnection conn);
        CSdpConnection& GetConnection(int pos);

        // b
        int AddBandwidth(CSdpBandwidth bandwidth);
        CSdpBandwidth& GetBandwidth(int pos);

        // k
        int SetKey(CSdpKey key);
        CSdpKey& GetKey();

        // a
        int AddAttribute(const char *field);
        int AddAttribute(const char *field, const char *value);
        int AddAttribute(CSdpAttribute attribute);
        CSdpAttribute& GetAttribute(int pos);
        CSdpAttribute& GetAttribute(const char *field);
        std::string GetAttrValue(const char *field);
        int RemoveAttribute(int pos);
        int RemoveAttribute(const char *field);
        int GetAttrCount();
        int ClearAttribute();

        int GetRecvSendFlag();

    private:
        // methods for sdp message copy
        int ClonePayloadList(osip_list_t *li);
        int CloneConnectionList(osip_list_t *li);
        int CloneBandwidthList(osip_list_t *li);
        int CloneAttributeList(osip_list_t *li);
        int CloneKey(sdp_key_t *key);

        void __InitVisitor();
        void __FreeVisitor();

    private:
        sdp_media_t *m_pMedia;

        // 成员引用对象：为方便访问SDP属性而设置
        CSdpKey m_key;
        CSdpBandwidth m_bandwidth;
        CSdpConnection m_connection;
        CSdpAttribute m_attribute;
    };

    class  CSdpOrigin
    {
    public:
        CSdpOrigin();
        CSdpOrigin(const CSdpOrigin &origin);
        ~CSdpOrigin();
        CSdpOrigin& operator = (const CSdpOrigin &origin);

        void SetUsername(const char *username);
        void SetSessionID(const char *sessionId);
        void SetSessionVersion(const char *sessionVersion);
        void SetNetType(const char *netType);
        void SetAddrType(const char *addrType);
        void SetAddress(const char *address);

        std::string& GetUsername();
        std::string& GetSessionID();
        std::string& GetSessionVersion();
        std::string& GetNetType();
        std::string& GetAddrType();
        std::string& GetAddress();

    private:
        std::string m_Username;
        std::string m_SessionID;
        std::string m_SessionVersion;
        std::string m_NetType;
        std::string m_AddrType;
        std::string m_Address;
    };

    /**
     * @class CSdpMessage
     * @brief SDP消息对象
     * 使用SDP消息时需要注意，其中的一些字段必须要设置或者存在，
     * 否则将会导致消息的解析或者Clone等方法的失败。具体哪些字段
     * SDP所必须的可以参考RFC文档。
     * 示例：
        CSdpMessage sdp;

        // v= 字段
        sdp.SetVersion("0");

        // o= 字段
        sdp.SetOrigin("123", "123456789", "2.0", "IN", "IP4", "127.0.0.1");

        // t= 字段必须存在
        sdp.AddTimeDescr(CSdpTimeDescr("0", "0"));

        // c= 字段
        CSdpConnection conn;
        conn.SetAddress("127.0.0.1");
        conn.SetAddrType("IP4");
        conn.SetNetType("IN");
        sdp.SetConnection(conn);

        // m= 字段必须存在
        CSdpMedia media;
        media.SetPort(10000);
        media.SetType("video");
        media.SetProtocol("RTP/AVP");
        sdp.AddMedia(media);
     **/
    class  CSdpMessage
    {
    public:
        CSdpMessage();
        CSdpMessage(const char *buf);
        CSdpMessage(sdp_message_t *sdp);
        CSdpMessage(const CSdpMessage &sdp);
        ~CSdpMessage();
        CSdpMessage& operator = (const CSdpMessage &sdp);

        int Parse(const char *buf);
        void Release();
        const char* ToString();
        int GetLength();
        CSdpMessage* Clone();

        // v
        int SetVersion(const char *version);
        const char* GetVersion();

        // o
        int SetOrigin(CSdpOrigin &origin);
        int SetOrigin(const char *username, 
                      const char *sessionId, 
                      const char *sessionVersion, 
                      const char *netType, 
                      const char *addrType, 
                      const char *addr);
        int SetUsername(const char *username);
        int SetSessionID(const char *sessId);
        int SetSessionVersion(const char *sessVersion = NULL);
        int SetNetType(const char *netType);
        int SetAddrType(const char *addrType);
        int SetAddress(const char *addr);
        CSdpOrigin& GetOrigin();
        const char* GetUsername();
        const char* GetSessionID();
        const char* GetSessionVersion();
        const char* GetNetType();
        const char* GetAddrType();
        const char* GetAddress();

        // s
        int SetSessionName(const char *sessionName);
        const char* GetSessionName();

        // i
        int SetSessionInfo(const char *sessionInfo);
        const char* GetSessionInfo();

        // u
        int SetUri(const char *uri);
        const char* GetUri();

        // e
        int AddEmail(const char *email);
        const char* GetEmail(int pos);

        // p
        int AddPhone(const char *phone);
        const char* GetPhone(int pos);

        // c
        int SetConnection(CSdpConnection connection);
        CSdpConnection& GetConnection();

        // b
        int AddBandwidth(const char *type, const char *bandwidth);
        CSdpBandwidth& GetBandwidth(int pos);

        // t
        int SetTimeDescr(CSdpTimeDescr timeDescr, int pos = -1);
        CSdpTimeDescr& GetTimeDescr(int pos);
        int AddTimeDescr(CSdpTimeDescr timeDescr);
        int RemoveTimeDescr(int pos);
        int ClearTimeDescr();

        // z
        int SetZAdjustment(const char *zAdjustment);
        const char* GetZAdjustment();

        // k
        int SetKey(CSdpKey key);
        CSdpKey& GetKey();

        // a
        int AddAttribute(const char *field);
        int AddAttribute(const char *field, const char *value);
        int AddAttribute(CSdpAttribute attribute);
        CSdpAttribute& GetAttribute(int pos);
        int RemoveAttribute(int pos);
        int RemoveAttribute(const char *field);
        int ClearAttribute();

        // m
        int SetMedia(CSdpMedia media, int pos = 0);
        int AddMedia(CSdpMedia media);
        CSdpMedia& GetMedia(int pos);
        CSdpMedia& GetMedia(const char *type);
        int RemoveMedia(int pos);
        int RemoveMedia(const char *type);
        int GetMediaCount();
        int ClearMedia();

        int GetRecvSendFlag();

    private:
        void __InitVisitor();
        void __FreeVisitor();

    private:
        sdp_message_t *m_pSdp;
        char *m_pBuf;
        int m_nLength;

        // 成员引用对象：为方便访问SDP属性而设置
        CSdpKey m_key;
        CSdpOrigin m_origin;
        CSdpBandwidth m_bandwidth;
        CSdpTimeDescr m_timeDescr;
        CSdpConnection m_connection;
        CSdpAttribute m_attribute;
        CSdpMedia m_media;
    };

    /**
     * @class CSdpUtility
     * @brief SDP消息辅助工具类
     * 
     **/
    class  CSdpUtility
    {
    public:
        CSdpUtility();
        ~CSdpUtility();
        static std::string GenerateSessionID();
    };

#endif // CSDPUTIL_H
