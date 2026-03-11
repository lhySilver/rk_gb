#ifndef __AUDIO_PROMPT_H__
#define __AUDIO_PROMPT_H__

#include "Common.h"

#include <deque>


//系统提示音
#define AUDIO_FILE_IAT 					"/oem/usr/audio/iat.g711u" 				//启动生产测试模式
#define AUDIO_FILE_NOTHING				"/oem/usr/audio/nothing.g711u"
#define AUDIO_FILE_DINGDINGDING 		"/oem/usr/audio/DingDingDing.g711u" 	//叮叮叮
#define AUDIO_FILE_DIDI 				"/oem/usr/audio/a_o.g711u" 				//啊哦
#define AUDIO_FILE_SIREN1               "/oem/usr/audio/siren1.g711u"
#define AUDIO_FILE_SIREN2               "/oem/usr/audio/siren2.g711u"
#define AUDIO_FILE_SIREN3               "/oem/usr/audio/siren3.g711u"
#define AUDIO_FILE_SIREN4               "/oem/usr/audio/siren4.g711u"
#define AUDIO_FILE_SIREN5               "/oem/usr/audio/siren5.g711u"
#define AUDIO_FILE_SIREN6               "/oem/usr/audio/siren6.g711u"

//自定义提示音
#define AUDIO_FILE_AP_ESTABLISHED 		"/userdata/audio/ap-jianliwancheng.g711u" 					//AP建立完成
#define AUDIO_FILE_AP_ESTABLISHING 		"/userdata/audio/ap-jianlizhong.g711u" 						//AP建立中
#define AUDIO_FILE_DEV_RESET 			"/userdata/audio/chongzhishexiangjichenggong.g711u" 		//重置摄像机成功
#define AUDIO_FILE_QRCODE_GET_COMPLETE 	"/userdata/audio/erweimasaomiaochenggong.g711u" 			//二维码扫描成功
#define AUDIO_FILE_QRCODE_INVALID 		"/userdata/audio/erweimwuxiao-qingchongzhi.g711u" 			//二维码无效，请重试
#define AUDIO_FILE_SD_FORMAT_FAILED 	"/userdata/audio/geshihuashibai.g711u" 						//格式化失败
#define AUDIO_FILE_SD_FORMAT_COMPLETE 	"/userdata/audio/geshihuawancheng.g711u" 					//格式化完成
#define AUDIO_FILE_DEV_UPDATING 		"/userdata/audio/userdata/audio/gujianshengjizhong-qingshaodeng.g711u" 				//固件升级中，请稍等
#define AUDIO_FILE_WIFI_CONNECTING 		"/userdata/audio/lianjiezhong.g711u" 						//连接中，请稍等
#define AUDIO_FILE_PLEASE_SET_WIFI 		"/userdata/audio/qingpeizhiwifi.g711u" 						//请配置WIFI
#define AUDIO_FILE_SD_ERROR 			"/userdata/audio/sd-kayichang.g711u" 						//SD卡异常
#define AUDIO_FILE_DEV_UPDATE_COMPLETE 	"/userdata/audio/shengjiwancheng.g711u" 					//升级完成
#define AUDIO_FILE_WIFI_CONNECTED 		"/userdata/audio/wifi-lianjiechenggong.g711u" 				//WIFI连接成功
#define AUDIO_FILE_WIFI_INCORRECT 		"/userdata/audio/wifi-mimacuowu.g711u" 						//WIFI密码错误
#define AUDIO_FILE_WIFI_CONNECT_ERROR 	"/userdata/audio/wifi-connecterror.g711u" 					//WIFI连接失败
#define AUDIO_FILE_BIND_SUCCESS         "/etc/conf/audio/bindsuccess.g711u" 						//绑定平台成功
#define AUDIO_FILE_WIFI_MODULE_ERROR 	"/userdata/audio/WiFi-mokuaiyichang.g711u" 					//WIFI模块异常
#define AUDIO_FILE_SYSTEM_STARTING 		"/userdata/audio/xitongqidongzhong.g711u" 					//系统启动中
#define AUDIO_FILE_SD_FORMATING 		"/userdata/audio/zhengzaigeshihua-qingshaodeng.g711u" 		//正在格式化，请稍等
#define AUDIO_FILE_SIREN 				"/userdata/audio/jingdi.g711u" 								//警笛声
#define AUDIO_FILE_DINGDONG 			"/userdata/audio/dingdong.g711u"

class CAudioPrompt : public CThread
{
public:
	typedef struct _AudioFileParm
	{
//		FILE * pFile;
		std::string strFileName;
		int type;//0,响一次 1，常响 2，间隔响
		bool clean;//清除循环报警
	}AudioFileParm;


public:
	CAudioPrompt();
	~CAudioPrompt();

	static CAudioPrompt* instance();

	/// 开始语音对讲
	bool start();
	
	/// 停止语音对讲
	bool stop();
	

	void ThreadProc();

	int aoPlay(AudioFileParm file);
	int aoPlayForReboot(AudioFileParm file);
	int playSiren();	
	void stopPlay();
	int getPlayStatus();
	std::string get_play_siren_list(int list);
private:
	int getAudioFile(AudioFileParm &file);
	int m_play;
	bool m_bRebootStart;
	bool m_bStopPlay;
	std::deque<AudioFileParm>	m_audioFileDeque;
	CMutex m_Mutex;
	static CAudioPrompt *m_instance;
};

#define g_AudioPrompt (*CAudioPrompt::instance())

#endif 		//__AUDIO_PROMPT_H__
