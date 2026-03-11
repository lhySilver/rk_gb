#ifndef __QRCODE_H__
#define __QRCODE_H__

#include "Manager/User.h"
#include "Manager/Console.h"
#include "Infra/Thread.h"
#include "Infra/Function.h"
#include "Infra/Singleton.h"
#include "Infra/Wrapper.h"
#include <map>
#include <string>
#include <algorithm>

#include <time.h>
#include <getopt.h>
#include <signal.h>
#include <unistd.h>
#include <sys/time.h>
#include <string.h>
#include <sys/stat.h>
#include <errno.h>
#include <stdint.h>

#include "Infra/Packet.h"

typedef void *(*funcThreadRoute)(void*);
typedef bool (*QrCodeExtractedCallback)(const std::string &strResult);


class CQrCode : public CThread
{
public:
	
	PATTERN_SINGLETON_DECLARE(CQrCode);

	/// 线程执行体
	static int OnCapture(char *pYUVBuffer, int bufferLen,int w,int h);
	///创建
	bool Create(QrCodeExtractedCallback cb);
	///销毁
	bool Destory();
	/// 开启
	bool Start();
	/// 停止
	bool Stop();
	/// 获取运行状态
	bool GetCreatedStatus();
	/// 获取是否正在识别二维码
	bool GetWorkStatus();

	/// 获取二维码中的token信息
	void GetToken(std::string &token);
private:
		/// 线程执行体
	void ThreadProc();
	bool m_bGetCode; 		//是否识别到二维码标志
	bool m_bCerated; 		//模块创建标志
	bool m_bExtracting; 	//正在识别二维码标志
	bool m_bIsExit; 		//
	std::string m_token; 	//二维码信息中的token
	QrCodeExtractedCallback m_cb; 	//提取到二维码时的回调
};

#define g_QrCodeConHandle (*CQrCode::instance())



#endif// __QRCODE_H__

