#ifndef CHANNEL_APPEND_INFO_HELP_HEAD
#define CHANNEL_APPEND_INFO_HELP_HEAD

#include <string>
#include "TVTXMLParse.h"

/****************************************************************
<chlNoFunc>
--------------------
</chlNoFunc>
<devNoFunc>
--------------------
</devNoFunc>
<mainCaps>
    <res fps="25">1920x1080</res>
    <res fps="25">1280x720</res>
</mainCaps>
<subCaps>
    <res fps="25">1280x720</res>
    <res fps="25">640x480</res>
    <res fps="4">320x240</res>
</subCaps>
<stream>
    <s res="1920x1080" fps="8" />
    <s res="640x480" fps="18" />
</stream>
<sub res="640x480" fps="18" QoI ="4" />
<mn res="1920x1080" fps="20" QoI ="2" audio="ON" type ="sub" />
<me res="1280x720" fps="25" QoI ="5" audio="OFF" type ="main" />
<an res="1920x1080" fps="15" QoI ="1" audio="ON" type ="sub" />
<ae res="1280x720" fps="25" QoI ="5" audio="OFF" type ="main" />
<win idx="8" />
<ftpRec type="main" audio="OFF" />
<record per="10" post="60" />
*****************************************************************/

#define MAKETUINT32(usWidth, usHeight) ((tuint32)((tuint16)(usWidth)|(((tuint32)(tuint16)(usHeight))<<16)))
#define LOTUINT16(l)           ((tuint16)(((tuint32)(l)) & 0xffff))
#define HITUINT16(l)           ((tuint16)((((tuint32)(l)) >> 16) & 0xffff))


class CChannelAppendInfoHelp
{
public:
    static GUID GetNodeGUIDByWinIndex(int nLogicWinIndex);
    static tuint32 GetNodeIDByWinIndex(int nLogicWinIndex);

	static std::string GetVideoResolutionName(tuint32 VideoRes);
	static tuint32 GetVideoResolution(const char *szVideoResName);/////从字符串 转换为 低16位为宽  高16位为高的32位数
	static tuint32 GetResolutionMultiplication(const char *szVideoResName);/////获取分辨率的乘积
	static bool GetVideoResolutionWH(tuint32 VideoRes, tuint32 &dwWidth, tuint32 &dwHeight);


	CChannelAppendInfoHelp(void);							//设置时用此构造函数
	CChannelAppendInfoHelp(const char *szAppendInfo);		//解释时用此构造函数

	~CChannelAppendInfoHelp(void);

	bool SetProductModel(const char *szProductModelInfo);					//
	bool GetProductModel(char *szProductModelInfo, tuint32 nBufLen);						//

	bool SetBindWindowIndex(tint32 nIndex);					//设置绑定窗口索引
	tint32 GetBindWindowIndex(void);						//获取绑定窗口索引,失败返回-1

	bool SetDeviceAppendInfo(const char *szDeviceInfo);				//设置设备附加信息
	bool GetDeviceAppendInfo(char *szDeviceInfo, tuint32 nBufLen);	//获取设备附加信息

	bool SetAlarmOutTimes(const char *AlarmOutTimes);//设备报警输出联动时长
	bool GetAlarmOutTimes(char *AlarmOutTimes, int nBufLen);
	bool GetAlarmOutTimes(int nIndex, int &AlarmOutTime);

	bool SetChlAppendInfo(const char *szInfo);				//设置
	bool GetChlAppendInfo(char *szInfo, tuint32 nBufLen);	//获取

	bool SetMainStreamAbility(const char *szAbility);			//设置主码流能力信息,用于接收到码流信息后更新
    bool GetMainStreamAbility(char *szAbility, tuint32 nBufLen);//获取主码流能力信息

	bool SetSubStreamAbility(const char *szAbility);			//设置子码流能力信息,用于接收到码流信息后更新
	bool GetSubStreamAbility(char *szAbility, tuint32 nBufLen);	//获取子码流能力信息

    bool SetAUX1StreamAbility(const char *szAbility);
    bool GetAUX1StreamAbility(char *szAbility, tuint32 nBufLen);

	bool SetStreamInfo(int nIndex, const char *szStream);				//设置码流信息,用于接收到码流信息后更新    index下标从0开始   0表示主码流
	bool SetStreamInfo(const char *szStream);
	bool ClearStreamInfo();
	bool GetStreamInfo(int nIndex, tuint32 &videoRes, tuint32 &dwRate, tuint32 &dwQuality);////    index下标从0开始  0表示主码流
    bool GetStreamInfo(int nIndex, tuint32 &videoRes, tuint32 &dwRate, tuint32 &dwQuality, char *szEncType);////    index下标从0开始  0表示主码流
	bool GetStreamInfo(int nIndex, char *szStream, tuint32 nBufLen);	//获取码流信息      index下标从0开始  0表示主码流

	bool SetSubStream(tuint32 VideoRes, tuint32 dwRate, tuint32 dwQuality);		//设置子码流信息
	bool GetSubStream(tuint32 &VideoRes, tuint32 &dwRate, tuint32 &dwQuality);		//获取子码流信息

	bool SetAutoNormalRecStream(tuint32 VideoRes, tuint32 dwRate, tuint32 dwQuality, bool isAudio, tuint32 nStreamType/*0为主码流 1是子码流 2是第三码流*/);	//设置自动模式时平时录像码流
	bool GetAutoNormalRecStream(tuint32 &VideoRes, tuint32 &dwRate, tuint32 &dwQuality, bool &isAudio, tuint32 &nStreamType/*0为主码流 1是子码流 2是第三码流*/);//获取自动模式时平时录像码流

	bool SetAutoEventRecStream(tuint32 VideoRes, tuint32 dwRate, tuint32 dwQuality, bool isAudio, tuint32 nStreamType/*0为主码流 1是子码流 2是第三码流*/);			//设置自动模式时事件录像码流
	bool GetAutoEventRecStream(tuint32 &VideoRes, tuint32 &dwRate, tuint32 &dwQuality, bool &isAudio, tuint32 &nStreamType/*0为主码流 1是子码流 2是第三码流*/);	//获取自动模式时事件录像码流

	bool SetManualNormalRecStream(tuint32 VideoRes, tuint32 dwRate, tuint32 dwQuality, bool isAudio, tuint32 nStreamType/*0为主码流 1是子码流 2是第三码流*/);		//设置手动模式时平时录像码流
	bool GetManualNormalRecStream(tuint32 &VideoRes, tuint32 &dwRate, tuint32 &dwQuality, bool &isAudio, tuint32 &nStreamType/*0为主码流 1是子码流 2是第三码流*/);	//获取手动模式时平时录像码流
	bool SetManualNormalRecStream(tuint32 VideoRes, tuint32 dwRate, tuint32 dwQuality, bool isAudio, tuint32 nStreamType/*0为主码流 1是子码流 2是第三码流*/, GUID timefpsGuid);	//设置手动模式时平时录像码流
	bool GetManualNormalRecStream(tuint32 &VideoRes, tuint32 &dwRate, tuint32 &dwQuality, bool &isAudio, tuint32 &nStreamType/*0为主码流 1是子码流 2是第三码流*/, GUID &timefpsGuid);	//获取手动模式时平时录像码流

	bool SetManualEventRecStream(tuint32 VideoRes, tuint32 dwRate, tuint32 dwQuality, bool isAudio, tuint32 nStreamType/*0为主码流 1是子码流 2是第三码流*/);		//设置手动模式时事件录像码流
	bool GetManualEventRecStream(tuint32 &VideoRes, tuint32 &dwRate, tuint32 &dwQuality, bool &isAudio, tuint32 &nStreamType/*0为主码流 1是子码流 2是第三码流*/);	//获取手动模式时事件录像码流

	bool SetRecordAppendTime(tuint32 dwPerrecordTime, tuint32 dwPostRecordTime);					//设置预录像和延迟录像时长
	bool GetRecordAppendTime(tuint32 &dwPerrecordTime, tuint32 &dwPostRecordTime);					//获取预录像和延迟录像时长

	bool SetFTPRecordStream( tuint32 nStreamType/*0为主码流 1是子码流 2是第三码流*/, bool isAudio);
	bool GetFTPRecordStream( tuint32 &nStreamType/*0为主码流 1是子码流 2是第三码流*/, bool &isAudio);

	bool SetChlNum(tuint32 num);//设置通道数
	bool GetChlNum(tuint32 &num);//获取通道数

	bool SetSensorNum(tuint32 num);//设置传感器数
	bool GetSensorNum(tuint32 &num);//

	bool SetAlarmOutNum(tuint32 num);//设置报警输出
	bool GetAlarmOutNum(tuint32 &num);//

	bool SetDevType(const char * pType);//设置类型
	std::string GetDevType();//
	
	bool SetDevModel(const char *szDevModel);				//设置
	bool GetDevModel(char *szDevModel, tuint32 nBufLen);	//获取

	bool SetProductType(const char *szProductType);				//设置设备类型，
	bool GetProductType(char *szProductType, tuint32 nBufLen);	//获取设备类型，设备类型的值大于等于0x2000的时候为DVR，小于0x2000的时候为NVR,此类型只针对N9K设备使用，

	bool SetDevVersion(const char *szDevVersion);				//设置
	bool GetDevVersion(char *szDevVersion, tuint32 nBufLen);	//获取

    // 协议版本
    bool SetDevProtocolVersion(tuint32 protocolVersion);				//设置
    bool GetDevProtocolVersion(tuint32& protocolVersion);	//获取

	//设置解码器输出数量 仅解码器用到
	bool SetDecoderOutputNum(tuint32 num);
	bool GetDecoderOutputNum(tuint32 &num);

	//预置点数量
	bool SetPresetNum(tuint32 num);
	bool GetPresetNum(tuint32 &num);
	//巡航线数量
	bool SetCrusieNum(tuint32 num);
	bool GetCrusieNum(tuint32 &num);
	//轨迹数量
	bool SetTraceNum(tuint32 num);
	bool GetTraceNum(tuint32 &num);

	bool GetN9KChnGUIDs(string& strChnGUIDs);
	bool SetN9KChnGUIDs(string strChnGUIDs);
	void DelN9KChnGUIDs();
	std::string GetAppendString(void);

	bool SetFaceAttendType(tuint32 nType);//设置监控点的人脸闸机考勤类型 0-未知 1-进入 2-离开
	bool GetFaceAttendType(tuint32 &nType);//获取监控点的人脸闸机考勤类型 0-未知 1-进入 2-离开

	bool SetChnIsEnableCrossLine(tuint32 nType);//设置监控点的是否启用过线统计 0-不启用 1-启用
	bool GetChnIsEnableCrossLine(tuint32 &nType);//获取监控点的是否启用过线统计 0-不启用 1-启用

	bool SetChnCrossLineType(tuint32 nType);//设置能力集不支持过线统计的监控点启用过线统计后,进出类型(0-未配置  1-进入 2-离开 3-支持过线统计的ipc不需要配置） 
	bool GetChnCrossLineType(tuint32 &nType);//获取能力集不支持过线统计的监控点启用过线统计后,进出类型(0-未配置  1-进入 2-离开 3-支持过线统计的ipc不需要配置） 

	bool SetChnIntelligentScene(string str);
	bool GetChnIntelligentScene(string& str);
    bool SetChnIASrvFaceMatch(tuint32 isIASrvFaceMatch);
    bool GetChnIASrvFaceMatch(tuint32& isIASrvFaceMatch);//判断是否启用了智能分析服务器比对
	bool GetChnEnableAccessScene();//启用了人脸门禁场景
	bool GetChnEnableAttendScene();//启用了人脸考勤场景
	bool GetChnEnableGuestScene();//启用了人脸迎宾场景

	bool SetLongitude(const std::string& str);//设置经度
	bool GetLongitude(std::string& str);//获取经度
	bool SetLatitude(const std::string& str);//设置纬度
	bool GetLatitude(std::string& str);//获取纬度
	bool SetPlaceCode(const std::string& str);//设置行政区域代码
	bool GetPlaceCode(std::string& str);//获取行政区域代码

private:
	CTVTXMLParse m_AppendXML;
	tuint_ptr m_ptrRoot;

private:
	bool SetChildInfo(const char *szName, const char *szBuf);
	bool GetChildInfo(const char *szName, char *szBuf, tuint32 nBufLen);

	tuint_ptr SetStreamConfig(const char *szStreamName, tuint32 VideoRes, tuint32 dwRate, tuint32 dwQuality);
    tuint_ptr GetStreamConfig(const char *szStreamName, tuint32 &VideoRes, tuint32 &dwRate, tuint32 &dwQuality, char *szEncType);

	bool SetRecordStream(const char *szRecordType, tuint32 VideoRes, tuint32 dwRate, tuint32 dwQuality, bool isAudio, tuint32 nStreamType/*0为主码流 1是子码流 2是第三码流*/);	
	bool GetRecordStream(const char *szRecordType, tuint32 &VideoRes, tuint32 &dwRate, tuint32 &dwQuality, bool &isAudio, tuint32 &nStreamType/*0为主码流 1是子码流 2是第三码流*/);
	bool SetRecordStream(const char *szRecordType, tuint32 VideoRes, tuint32 dwRate, tuint32 dwQuality, bool isAudio, tuint32 nStreamType/*0为主码流 1是子码流 2是第三码流*/, GUID timefpsGuid);	
	bool GetRecordStream(const char *szRecordType, tuint32 &VideoRes, tuint32 &dwRate, tuint32 &dwQuality, bool &isAudio, tuint32 &nStreamType/*0为主码流 1是子码流 2是第三码流*/, GUID &timefpsGuid);
};

#endif //CHANNEL_APPEND_INFO_HELP_HEAD
