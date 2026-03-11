

#ifndef NAT_CLIENT_SDK_HEAD
#define NAT_CLIENT_SDK_HEAD

#ifdef WIN32

#ifdef NAT_CLIENT_SDK_EXPORT_STATIC//静态库编译
#define NAT_CLIENT_SDK extern "C"
#else
#ifdef NAT_CLIENT_SDK_EXPORT_API//动态库编译
#define NAT_CLIENT_SDK extern "C" __declspec(dllexport)
#else//使用者编译
#define NAT_CLIENT_SDK extern "C" __declspec(dllimport)
#endif
#endif

#define STDCALL __stdcall

#else
#define NAT_CLIENT_SDK __attribute__ ((visibility("default")))
#define STDCALL
#endif

typedef unsigned int TNatSocketHandle;

#define  INVALID_NAT_SOCKET_HANDLE 0

/****************************
*客户端连接方式定义
*****************************/
enum NatClientConnectType
{
    NatClientConnectBySN=0,  //通过SN连接设备
    NatClientConnectByToken=1 //通过访问凭证连接设备
};

enum NatClientType
{
	NatClientTypeUnknown=0,  //未知类型
	NatClientTypeWebWin=1, //windows web
	NatClientTypeWebMac=2, //mac web
	NatClientTypeAppIos=3,  // app ios
	NatClientTypeAppAnd=4, //app 安卓
	NatClientTypeCms=5 //cms 客户端
};

enum NetConnectQuality
{
	NetConnectQualityVeryGood=1, //局域网连接
	NetConnectQualityGood=2, //upnp连接
	NetConnectQualityNormal=3,  //理论上100%可以穿透
	NetConnectQualityLow=4, //可以穿透但是不保证100%
	NetConnectQualityVeryLow=5,//理论上一定穿透不了
};


typedef struct _tag_client_connect_info
{
    NatClientConnectType connectType; //连接方式，目前只支持通过sn方式连接
    char devConnectCode[4096]; //连接设备代码，通过SN连接则是SN,通过凭证连接就是凭证
}ClientConnectInfo;

typedef struct _tag_nat_client_cfg
{
    /*重定向服务器地址*/
    char natServerAddress[64];
    /*重定向服务器端口*/
    unsigned short natServerPort;
    /*客户端token*/
    char clientToken[64];
	/*运营商代码*/
	char ispCode[64];
	/*定制客户ID*/
	char customerId[32]; //目前都填写000
	/*svn代码ID*/
	char svnCodeId[32];  //svn代码ID
    /*客户端类型*/
	NatClientType cliType;
	/*客户端软件版本*/
	char szClientVer[64]; //客户端软件版本号
}NatClientConfig;

//和设备通讯回调接口
class CNatClientSDKNotifier
{
public:
    //数据接收回调接口
	virtual int OnRecvData(TNatSocketHandle hNatSocket, const unsigned char* pData, int dataLen)=0;
    //连接成功回调接口，异步连接时才会回调
	virtual void OnConnect(TNatSocketHandle hNatSocket, tuint32 dwDevType, const char* szDevVer)=0;
	//连接失败回调接口，异步连接时才会回调
	virtual void OnConnectFail(TNatSocketHandle hNatSocket, tuint32 dwDevType, const char* szDevVer)=0;
    //连接断开回调接口
	virtual void OnDisConnect(TNatSocketHandle hNatSocket)=0;
	//查询信息回调接口
	virtual void OnQueryDevInfo(TNatSocketHandle hNatSocket, const char* pDevInfo, int devInfoLen){}
	//链接切换回调接口
	virtual void OnSwitchConnect(TNatSocketHandle hNatSocket, int nConnectQuality){}
};

//和服务器通讯回调接口，目前暂时用不到该接口
class CNatClientSDKNatServerNotifier
{
public:
    //Nat数据接收回调接口
	virtual void OnRecvNatServerTransData(const unsigned char* pData, int dataLen)=0;
    //连上Nat服务器通知
    virtual void OnConnectNatServer(bool bSuc)=0;
    //断开Nat服务器通知
    virtual void OnDisConnectNatServer()=0;
};

//nat socket 句柄生成接口， 集成到n9000, netsocket模块会存在句柄冲突，可以通过实现该接口由natsocket模块统一生成通讯句柄
class CNatClientNatSocketHandleGenerator
{
public:
	virtual tuint32 GenerateNatClientSocketHandle()=0;
};

//初始化sdk,一般情况下使用默认参数初始化就可以
NAT_CLIENT_SDK bool NAT_CLIENT_Init(CNatClientNatSocketHandleGenerator* pNatSocketHandleGenerator=NULL);

//反初始化sdk
NAT_CLIENT_SDK void NAT_CLIENT_Quit(void);

//启动sdk, sdk只需要启动一次
NAT_CLIENT_SDK bool NAT_CLIENT_Start(const NatClientConfig& natClientCfg);

//停止sdk
NAT_CLIENT_SDK void NAT_CLIENT_Stop(void);

//设置和设备通讯数据回调接口
NAT_CLIENT_SDK void NAT_CLIENT_SetNotifier(CNatClientSDKNotifier* pNotifier);

//设置和Nat服务器通讯数据回调接口
NAT_CLIENT_SDK void NAT_CLIENT_SetNatServerNotifier(CNatClientSDKNatServerNotifier* pNatServerNotifier);

//连接设备
NAT_CLIENT_SDK TNatSocketHandle NAT_CLIENT_ConnectDev(const ClientConnectInfo& cliConnInfo,bool bSync,int connectTimeoutInSeconds);

//查询设备信息
NAT_CLIENT_SDK TNatSocketHandle NAT_CLIENT_QueryDevInfo(const ClientConnectInfo& cliConnInfo,bool bSync, int queryTimeoutInSeconds,char* devInfoBuf, unsigned int& bufLen);

//获取访问凭证，凭证连接模式下才会用到
NAT_CLIENT_SDK bool NAT_CLIENT_GetNatAccessToken(TNatSocketHandle hNatSocket, char* natAccessTokenBuf, unsigned int& bufLen);

//获取数据加密密码
NAT_CLIENT_SDK bool NAT_CLIENT_GetDataEncryptKey(TNatSocketHandle hNatSocket, char* keyBuf, unsigned int& bufLen);

//发送数据给设备
NAT_CLIENT_SDK int NAT_CLIENT_SendToDev(TNatSocketHandle hNatSocket, const void *pData, const int iLen, const bool bBlock, const int sendTimeout);

//发送数据给设备 pData1,pData2, pContent整体发送, 适配N9000原来的发送接口
NAT_CLIENT_SDK int NAT_CLIENT_SendToDevEx(TNatSocketHandle hNatSocket, void *pData1, int iLen1,void *pData2, int iLen2, void* pContent);

//发送数据给服务器
NAT_CLIENT_SDK bool NAT_CLIENT_SendTransDataToNatServer(const unsigned char* pData, const int iLen);

//销毁通讯句柄
NAT_CLIENT_SDK void NAT_CLIENT_DestroyNatSocket(const TNatSocketHandle hNatSocket);

//连接Nat服务器
NAT_CLIENT_SDK bool NAT_CLIENT_ConnectNatServer(const char* szClientP2PId);

//断开和Nat服务器连接
NAT_CLIENT_SDK void NAT_CLIENT_DisConnectNatServer();

#endif //NAT_CLIENT_SDK
