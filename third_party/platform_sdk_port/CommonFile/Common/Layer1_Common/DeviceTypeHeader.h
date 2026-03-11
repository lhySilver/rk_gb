

/////////////////////////DeviceTypeHeader.h
/*	
	统一公司产品类型定义，每个产品内部的具体型号由开发此产品的组内部自己自己维护

*/


#ifndef	_DEVICE_TYPE_H_
#define _DEVICE_TYPE_H_

namespace DEVICE_TYPE_DEF
{
  	/************************************************************************/                                                                  
	/* 	设备类型信息的数据头	*/
	/************************************************************************/
	const tuint32 DEVICE_TYPE_HEADFLAG = ( (tuint32)(unsigned char)('h') | ( (tuint32)(unsigned char)('e') << 8 ) |	\
		( (tuint32)(unsigned char)('a') << 16 ) | ( (tuint32)(unsigned char)('d') << 24 ) );		
		
	//设备类型信息的数据头长度
	const size_t DEV_HEAD_INFO_LEN = 64;

  	/************************************************************************/                                                                  
	/* 
		此设备类型定义的标准为:  设备类型_系列_通道数量
		设备类型
			DVS/IPCAMERA/DVR/DVRCARD/CMS/DECODER(解码器或解码卡)
		系列:
			DVS: 11系列
			IPCAMERA: 11、12系列
			DVR:20、23、24系列
			DVRCARD：30、31、33、41系列	 */
	/************************************************************************/
	typedef enum _tvt_product_series_
	{
		TVT_PRODUCT_DVR,			//嵌入式硬盘录像机
		TVT_PRODUCT_DVS,			//嵌入式视频服务器
		TVT_PRODUCT_IPC,			//嵌入式网络摄像机
		TVT_PRODUCT_NVR,			//嵌入式网络硬盘录像机
		TVT_PRODUCT_MDVR,			//嵌入式车载硬盘录像机
		TVT_PRODUCT_DECODER,		//嵌入式解码器
		TVT_PRODUCT_STORAGE,		//存储器
		TVT_PRODUCT_INTELLIGENCE,    //智能分析模块
		TVT_PRODUCT_NETKEYBOARD,    //网络键盘
		TVT_PRODUCT_CMS,			//单机版控制中心
		TVT_PRODUCT_ECMS,			//专业版控制平台
		TVT_PRODUCT_CLIENT,			//网络客户端
		TVT_PRODUCT_MOBILE_APP,		//移动客户端
	}TVT_PRODUCT_SERIES;

	/************************************************************************/
	/*   控制中心连接上设备后会先收到下面的结构体信息进行解析，通过设备类型创建相应的  
	服务对象，同时会把这64个字节的信息直接通过参数传递给服务对象*/                                                                     
	/************************************************************************/	
	typedef struct _net_device_type_info_n9k
	{
		tuint32 headFlag; ////取值为HEADFLAG
		tuint32 productSeries; ///产品系列  devicetype; /////////设备类型 DVS  IPCAMERA  DVR  SUPERDVR
		tuint32 producttype;  /////产品型号
		tuint32 netprotrocolver; ///////////设备对应的网络协议版本
		tuint32 configver; ///////////设备对应的配置版本
		tuint32 devID; //////
		tuint32 encryptType;	//数据加密方式，0表示没有加密
		tuint32 encryptParam;	//数据加密方式，0表示没有加密

		BYTE MAC[8];			//MAC地址      ---wangyu 20180913 接入NVR1.3.5  
		tuint32 dwSoftwareVer;  //
		BYTE loginEncrypt;      //登录加密类型,0 不加密,1 md5加密,2 用户名加密
		BYTE loginNonce[3];     //md5登录随机数
		tuint32 bSuportSoftEncrypt;
		BYTE transportEncryptType; //传输加密类型,0 不加密,1 AES加密,2 url加密
		BYTE byReserve[3];
		tuint32 ulReserve2[2];/////保留信息，以便以后扩充
		//tuint32 ulReserve[8];/////保留信息，以便以后扩充
	}NETDEVICE_TYPE_INFO_N9K, *LPNETDEVICE_TYPE_INFO_N9K;


	/************************************************************************/
	/*   控制中心连接上设备后会先收到下面的结构体信息进行解析，通过设备类型创建相应的  
	服务对象，同时会把这64个字节的信息直接通过参数传递给服务对象*/                                                                     
	/************************************************************************/	
	typedef struct _net_device_type_info
	{
		tuint32 headFlag; ////取值为HEADFLAG
		tuint32 productSeries; ///产品系列  devicetype; /////////设备类型 DVS  IPCAMERA  DVR  SUPERDVR
		tuint32 producttype;  /////产品型号
		tuint32 netprotrocolver; ///////////设备对应的网络协议版本
		tuint32 configver; ///////////设备对应的配置版本
		tuint32 devID; //////
		tuint32 encryptType;	//数据加密方式，0表示没有加密
		tuint32 encryptParam;	//数据加密方式，0表示没有加密
		tuint32 msgType;
		tuint32 ptzType;
		BYTE customMsgExt;//非0表示普天国密对接（仅ipc）
		BYTE byReserve[2];
		BYTE transportEncryptType; //传输加密类型,0 不加密,1 AES加密
		BYTE loginEncrypt;      //登录加密类型,0 不加密,1 md5加密,2 用户名加密
		BYTE loginNonce[3];     //md5登录随机数
		tuint32 ulReserve[4];/////保留信息，以便以后扩充
	}NETDEVICE_TYPE_INFO, *LPNETDEVICE_TYPE_INFO;

	//上报设备id和token
	enum e_EncryptType{
		T_ENCRYTYPE_NULL = 0, //不加密
		T_PUTIAN,//普天方式
	};

	//上报设备id和token
	typedef struct _encrypt_report
	{
		BYTE		clientID[52];		//设备唯一ID
		BYTE		token[36];			//设备token
		BYTE		reserve[16];	//保留字节
	}EncryptReport;
}  ////////////namespace DEVICE_TYPE_DEF end

#endif /////#ifndef _DEVICE_TYPE_H_








