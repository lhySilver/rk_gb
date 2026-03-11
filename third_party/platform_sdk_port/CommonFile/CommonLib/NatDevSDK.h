#ifndef NAT_DEV_SDK_HEAD
#define NAT_DEV_SDK_HEAD

#ifdef WIN32

#ifdef NAT_DEV_SDK_EXPORT_STATIC//静态库编译
#define NAT_DEV_SDK extern "C"
#else
#ifdef NAT_DEV_SDK_EXPORT_API//动态库编译
#define NAT_DEV_SDK extern "C" __declspec(dllexport)
#else//使用者编译
#define NAT_DEV_SDK extern "C" __declspec(dllimport)
#endif
#endif

#define STDCALL __stdcall

#else
#define NAT_DEV_SDK __attribute__ ((visibility("default")))
#define STDCALL
#endif


typedef unsigned int TNatSocketHandle;

/****************************
*客户端连接方式定义
*****************************/
enum NatClientConnectType
{
    NatClientConnectBySN=0,  //通过SN连接设备
    NatClientConnectByToken=1 //通过访问凭证连接设备
};

//与客户端通讯回调接口
class CNatDevSDKClientNotifier
{
public:
	CNatDevSDKClientNotifier(void) {}
	virtual ~CNatDevSDKClientNotifier(void) {}
    virtual int OnRecvData(TNatSocketHandle hNatSocket, const char *pData, int nDataLen) = 0;
	virtual void OnConnect(TNatSocketHandle hNatSocket)=0;
	virtual void OnDisConnect(TNatSocketHandle hNatSocket)=0;
};

//与Nat服务器通讯回调接口
class CNatDevSDKNatServerNotifier
{
public:
	CNatDevSDKNatServerNotifier(void) {}
	virtual ~CNatDevSDKNatServerNotifier(void) {}
    virtual void OnRecvNatServerTransData(const char *pData, int nDataLen) = 0;
    virtual void OnConnectNatServer(bool bSuc)=0;
    virtual void OnDisConnectNatServer()=0;
};

//nat socket 句柄生成接口
class CNatDevNatSocketHandleGenerator
{
public:
	virtual tuint32 GenerateNatDevSocketHandle()=0;
};

enum NAT_UPNP_PORT_PROTOCOL
{
    NAT_UPNP_TCP,
    NAT_UPNP_UDP
};

class CNatDevUpnpMapper
{
public:
    virtual bool NatDevUpnpRegisterMappingPort(tuint16 localPort, tuint16 ExternalPort, NAT_UPNP_PORT_PROTOCOL upnpPortProtocol)=0;
    virtual bool NatDevUpnpUnRegisterMappingPort(tuint16 localPort, NAT_UPNP_PORT_PROTOCOL upnpPortProtocol)=0;
    virtual bool NatDevIsUpnpPortRegisted(tuint16 localPort, NAT_UPNP_PORT_PROTOCOL upnpPortProtocol)=0;
    virtual bool NatDevIsUpnpMapSuccess(tuint16 localPort,NAT_UPNP_PORT_PROTOCOL upnpPortProtocol)=0;
};

//状态通知
class CNatDevStatusNotifier
{
public:
    CNatDevStatusNotifier(void) {}
    virtual ~CNatDevStatusNotifier(void) {}

    enum NAT_DEV_STATUS
    {
        NAT_DEV_STATUS_BEGIN = 0,
        NAT_DEV_STATUS_CONN_NAT_SUCC, //连接成功
        NAT_DEV_STATUS_CONN_NAT_FAIL, //连接失败
        NAT_DEV_STATUS_CONN_NAT_DISCONNECT,//连接断开
        NAT_DEV_STATUS_END
    };

    //状态通知
    virtual void NotifyNatDevStatus(NAT_DEV_STATUS natDevStatus, const char *szMsg)=0;
};

typedef struct _tag_nat_dev_config_
{
	char natServerAddress[64];//重定向服务器地址
	unsigned short natServerPort;//重定向服务器端口
	unsigned short localUdpPort;//本地监听UDP端口
	char devSN[128];//设备序列号
	char devMAC[20];//设备mac地址
	int  devType;//设备类型
	char devModel[32];//设备型号
	char devVersion[64];//设备软件版本
	char customerId[32]; //定制客户ID, 目前都填写000
	char svnCodeId[32];  //svn代码ID
	char devInfo[512];//设备自定义信息
    char localityCode[16];//地区代码
}NatDevConfig;


NAT_DEV_SDK bool NAT_DEV_Init(CNatDevNatSocketHandleGenerator* pNatSocketHandleGenerator=NULL,CNatDevUpnpMapper* pNatDevUpnpMapper=NULL);

NAT_DEV_SDK void NAT_DEV_Quit(void);

//需要在NAT_DEV_Start前调用
NAT_DEV_SDK bool NAT_DEV_SetPrivateKey(const void* pKeyData, int dwKeyDataLen);

NAT_DEV_SDK bool NAT_DEV_Start(const NatDevConfig& natDevCfg);

NAT_DEV_SDK void NAT_DEV_Stop(void);

//设置客户端通讯回调接口
NAT_DEV_SDK void NAT_DEV_SetClientNotifier(CNatDevSDKClientNotifier* natClientNotifier, NatClientConnectType connectType);

//移除客户端通知接口
NAT_DEV_SDK void NAT_DEV_RemoveClientNotifier(CNatDevSDKClientNotifier* natClientNotifier);

//设置nat服务器通讯回调接口
NAT_DEV_SDK void NAT_DEV_SetNatServerNotifier(CNatDevSDKNatServerNotifier* natServerNotifier);

//设置状态通知接口
NAT_DEV_SDK void NAT_DEV_SetNatDevStatusNotifier(CNatDevStatusNotifier* natDevStatusNotifier);

//发送数据给客户端
NAT_DEV_SDK int  NAT_DEV_SendToClient(TNatSocketHandle hNatSocket, void* pData, int iLen, bool bBlock, int sendTimeout);

//发送数据给客户端 pData1,pData2, pContent整体发送, 适配N9000原来的发送接口
NAT_DEV_SDK int NAT_DEV_SendToClientEx(TNatSocketHandle hNatSocket, void *pData1, int iLen1,void *pData2, int iLen2, void* pContent);

//发送透传数据给服务器
NAT_DEV_SDK bool NAT_DEV_SendTransDataToNatServer(const char *pData, int iLen);

//连接Nat服务器是否成功
NAT_DEV_SDK bool NAT_DEV_IsConnectNatServerSuccess();

//销毁通讯句柄
NAT_DEV_SDK void NAT_DEV_DestroyNatSocket(TNatSocketHandle hNatSocket);

//开启、关闭信息收集
NAT_DEV_SDK void NAT_DEV_EnableInfoCollect(bool bEnable);

//解密Nat服务器的数据
NAT_DEV_SDK bool NAT_DEV_DecryptNatServerData(const char* pEncryptedData, unsigned int iEncryptedDataLen,char* pDecryptedData,unsigned int & iDecryptedDataLen);

//获取数据加密密码
NAT_DEV_SDK bool NAT_DEV_GetDataEncryptKey(const TNatSocketHandle hNatSocket, char* keyBuf, tuint32& bufLen);

//获取客户端ip端口
NAT_DEV_SDK bool NAT_DEV_GetRemoteIpPort(TNatSocketHandle hNatSocket, char* ipBuf, int bufLen, tuint16& port, bool& isIpv4);

#endif //NAT_DEV_SDK
