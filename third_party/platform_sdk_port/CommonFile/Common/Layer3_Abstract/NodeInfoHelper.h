
#ifndef __NODE_INFO_HELPER__
#define __NODE_INFO_HELPER__

#include "NVMS_Define.h"

class CNodeInfoHelper
{
public:
    explicit CNodeInfoHelper(tuint32 dwNodeID);///节点ID 通道或设备节点的ID
	~CNodeInfoHelper(void);

    bool SupportAudioCtrl();
    bool SupportPTZCtrl();	/////////支持
	/////////////////////////////////////////////////////////////  


	//通道ID////////////////////////////////////
    bool SupportMultiChPlay();//是否支持多通道同时打开回放
    bool SupportFunVideoLoss();
    bool SupportFunMotion();
    bool SupportFunPTZ();//是否支持云台控制（现场预览用的这个）
    bool SupportFunPTZProtocol();//是否支持云台配置协议（监控点配置云台协议）
    bool SupportFunRecord();
    bool SupportFunEncodeInfoCfg(); //编码信息配置，即图像质量界面
    bool SupportFunCruiseLine(); //巡航线
    bool SupportFunPresetPoint(); //预置点
    bool SupportFunPTZTrajectory(); //轨迹
    bool SuppotrFunColorRegulate(); //色彩调节
    bool SupportFunPTZOther(); //灯光、雨刷、自动扫描
    bool SupportStreamToFile();//是否支持流保存成文件比如远程备份、AVI录像等
	bool SupportFunImageDisplay(); //是否支持图像显示配置
	bool SupportFunMotionConfig();  //是否支持移动侦测配置(除移动区域外的)

	bool SupportFunOSC();//物品看护
	bool SupportFunAVD();//视频异常侦测
	bool SupportFunPEA();//越界或者入侵
    bool SupportFunVehicle();//车牌
	//智能IPC新增
	bool SupportFunCDD();//人群密度检测
	bool SupportFunIPD();//人员入侵侦测
	bool SupportFunCPC();//人数统计
	bool SupportFunVFD();//人脸检测
    bool SupportFunVFDMatch();//人脸比对（智能服务）
	bool SupportFunFisheye();//鱼眼

	bool SupportFunROI();//ROI
	bool SupportFunAZ();//对焦
	bool SupportFunOsdMask();//视频遮挡
	bool SupportSDCard();

	bool SupportFunChlTalkback();//是否支持ip通道对讲
	bool SupportFunAOIEntry();//进入区域
	bool SupportFunAOILeave();//离开区域
	bool SupportFunHeatMap();//热地图
	bool SupportFunRegionStatistic();//区域统计

	bool SupportHumanTemperatureDetect();//是否支持人体温检测， TVT的热成像ipc属于这
	bool SupportTemperaturePad();//针对TVT的温控平板
	bool SupportAccessControl();//支持门禁控制
	bool SupportTemperatureEx();//工业热成像
	//////////////////////////////////////////////////////////


	//设备ID//////////////////////////////////////
    bool SupportFunSensorAlarmIn();
    bool SupportFunDisk();
    bool SupportFunAlarmOut();
    bool SupportFunLogQuery();
    bool SupportFunAbnormalAlarm();
    bool SupportFunTalkback();
    bool SupportFunUserManager();
    bool SupportFunEMail();
    bool SupportFunDDNS();
    bool SupportFuncWIFI();
    bool SupportFunc3G();
    bool SupportFuncAutoRestart();
    bool SupportFuncAutoReport();
    bool SupportFuncMobileInfo();
    bool SupportFuncMobileSensor();
    bool SupportFuncMobileTemp();
    bool SupportFuncMobileSMS();
    bool SupportFuncMobileSpeed();
    bool SupportFuncMobileAcce();
    bool SupportFuncOnlineUsers();
	bool SupportFuncRecRewind();//设备是否支持倒放
    bool SupportFuncFaceMatch();
	bool SupportFunLocalFaceMatch();
	bool SupportFunPOS();
	/////////////////////////////////////////////////
	bool SupportFuncAudioAlarmOut();
	bool SupportFuncWhiteLightAlarmOut();

	bool IsSmartSupport();//是否是智能IPC
    //是否支持算法设置
    bool SupportAlgorithmSetting();
	bool SupportFuncHumanTemperature();//设备是否支持人体温检测
	bool SupportTransparentCom();//设备是否支持透明串口设置
	bool SupportSchedule();//是否支持排程设置
	bool SupportFtp();//是否支持FTP设置
	bool IsNvr(); //区分nvr or dvr
private:
    tuint32   m_dwNodeID;

    bool m_bFuncSupportAudio;
    bool m_bFuncEnablePTZ; ////云台使能是否可配的标识
    bool m_bSFunVideoLoss;
    bool m_bSFunMotion;
    bool m_bSFunPTZ;
    bool m_bSFunPTZProtocol;
    bool m_bSFunRecord;
    bool m_bSFunEncodeInfoCfg;
    bool m_bSFunCuriseLine;
    bool m_bSFunPresetPoint;
    bool m_bSFunPTZTrajectory;
    bool m_bSFunPTZOther;
    bool m_bSFunColorRegulate;
    bool m_bSFunStreamToFile;
	bool m_bSFunImageDisplay;
	bool m_bSFunMotionConfig;

	bool m_bSFunOSC; //物品看护侦测报警
	bool m_bSFunAVD; //视频异常诊断功能检测
	bool m_bSFunPEA; //区域入侵及越界
	bool m_bSFunCDD;//人群密度检测
	bool m_bSFunIPD;//人员入侵识别（人型识别）
	bool m_bSFunCPC;//人数统计
	bool m_bSFunVFD;//人脸检测
    bool m_bSFunVFDMatch;//人脸比对（智能服务）
	bool m_bSFunFisheye;//鱼眼
	bool m_bSFunROI;//ROI
	bool m_bSFunAZ;//对焦
	bool m_bSFunOsdMask;//视频遮挡
	bool m_bSDCard;
    bool m_bSFunPassLine;//过线统计
	bool m_bSFunChlTalkback;
	bool m_bSFunAoiEntry;//进入区域
	bool m_bSFunAoiLeave;//离开区域
	bool m_bSFunHeatMap;//热地图
	bool m_bSFunRegionStatistic;//区域统计

    bool m_bSFunSensorAlarmIn;
    bool m_bSFunDisk;
    bool m_bSFunAlarmOut;
    bool m_bSFunLogQuery;
    bool m_bSFunAbnormalAlarm;
    bool m_bSFunTalkback;
    bool m_bSFunUserManager;
    bool m_bSFunDDNS;
    bool m_bSFunEmail;
    bool m_bSWIFI;//支持WIFI
    bool m_bS3G;//支持3G
    bool m_bSAutoRestart;//支持自动重启
    bool m_bSAutoReport;//支持主动上报
    bool m_bSMobileInfo;//支持车载基本信息
    bool m_bSMobileSensor;//支持车载传感器
    bool m_bSMobileTemp;//支持车载温度报警
    bool m_bSMobileSMS;//支持车载短信
    bool m_bSMobileSpeed;//支持车辆速度报警
    bool m_bSMobileAcce;//支持车载加速度报警
    bool m_bSFunOnlineUsers; //在线用户查询
	bool m_bSFunRecRewind; //是否支持倒放
    bool m_bSFunFaceMatch; //是否支持人脸比对
	bool m_bSFunLocalFaceMatch; //是否支持后端本地人脸---NVR
	bool m_bSFunPOS; //是否支持POS机---NVR
    bool m_bIntiSupportFun; //是否初始化过
	bool m_bSFunAudioAlarmOut;	// 是否支持声音报警输出
	bool m_bSFunWhiteLightAlarmOut; // 是否支持闪光灯报警输出
	bool m_bSFunVehicle;	// 是否支持车牌识别
	bool m_bSFunTripwire;	// 是否支持绊线检测
	bool m_bSFunPerimeter;	// 是否支持越界侦测
	bool m_bSFunHumanTemperatureDetect; //通道是否支持人体温检测
	bool m_bSDevTemperatureDect;//设备是否支持人体温检测
	bool m_bSFunTemperaturePad; //是否是温控平板
	bool m_bSFunAccessControl; //是否支持门禁控制
	bool m_bTransparentCom;     //是否支持透明串口设置
	bool m_bSchedule;           //是否支持排程设置
	bool m_bFtp;                //是否支持FTP设置
	bool m_isNvr;                //用于区分nvr/dvr设备默认nvr
	bool m_bSFunTemperatureEx; //通道是否支持工业热成像

	//初始化Supprot
    bool InitChannelSupprotFun();
    bool InitDeviceSupprotFun();

    void SetAllChannelFlagState(bool bState);
    void SetAllDeviceFlagState(bool bState);
};


//通道功能 
//如c:e:l:即表示通道不支持巡航线、编码信息配置及视频丢失功能
// 简写代码		          功能说明
// 	c			巡航线 
//	d			图像显示配置
// 	e			编码信息配置
// 	l			视频丢失
// 	m			移动侦测
//	mc			移动侦测配置（除灵敏度外）
// 	r			录像
// 	s			色彩调节
// 	p			PTZ
// 	pp			PTZ协议
// 	t			PTZ轨迹
// 	w			PTZ其他
// 	z			PTZ预置点
// 	v			流到文件
// 	a                      不支持声音
// 	ep                     不支持关闭和启用云台的配置功能
// 	av			视频异常侦测
// 	os			物品看护
// 	pa			越界或者入侵
//  perimeter	区域入侵
//  tripwire	绊线检测
// 	cpc			人数统计
//  passlinecount 过线统计
// 	ipd			人员入侵侦测
// 	cdd			人群密度检测
//  vfd         人脸检测
//  vfdm          人脸比对（智能服务face match）
//  fisheye     鱼眼
//  roi         ROI
//  az          对焦
//  osdmask     视频遮挡
//  vehicle		车牌识别
//  报警功能, 如aao:awlo: 即通道不支持声音输出告警、不支持闪光灯输出告警
//  aao			通道是否支持告警声音输出(alarm_audio_out)
//  awlo			通道是否支持闪灯输出（alarm_white_light_out）
//  aoie			进入区域( aoiEntry )
//  aoil           离开区域( aoiLeave )
//  humanTemperature  人体温检测
//  temperaturePad  温控平板与热成像区分
//  ac           门禁控制，比如同为的平板支持开门
//  hm           热地图heat map
//  rs           区域统计 region statistic

// 如d:e:l:即表示设备不支持硬盘、异常报警及日志查询功能
// 
// 	简写代码		           功能说明
// 	d			硬盘
// 	e			异常报警
// 	l			日志查询
// 	m			Email
// 	n			DDNS
// 	o			报警输出
// 	oc			在线用户
// 	r			自动重启
// 	t			对讲
// 	u			用户管理
// 	w			Wifi
// 	s			传感器报警输入
// 	3			3G
// 	a			自动报告
// 	ma			车载传感器
// 	mg			车载加速度报警
// 	mi			车载基本信息
// 	mm			车载短信
// 	ms			车载加速度报警
// 	mt			车载温度报警
//  rewind      倒放
//  f           人脸比对
//	pos			pos机


#endif //__NODE_INFO_HELPER__
