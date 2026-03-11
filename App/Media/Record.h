
#ifndef __IMPLEMENT_MEDIA_RECORD_H__
#define __IMPLEMENT_MEDIA_RECORD_H__

#include "Common.h"

//定义录像模式
#define REC_SYS					0x0001		/// 系统预录事件
#define REC_TIM					0x0002		/// 定时录像事件
#define REC_MTD					0x0004		/// 动检录像事件
#define REC_ALM					0x0008		/// 报警录像事件
#define REC_CRD					0x0010		/// 卡号录像事件
#define REC_MAN					0x0020		/// 手动录像事件
#define REC_CLS					0x0040		/// 停止所有录像，并不能录像 
#define REC_ALL					0x007e		/// 所有录像事件(不包括预录)

class CRecordManager : public CThread
{
	PATTERN_SINGLETON_DECLARE(CRecordManager);
public:

	/// 开启录像管理对象
	bool Start();
	/// 得到录像支持的通道数
	int getChannels();	
	/// 关闭录像管理对象,停止一切录像（不改变预录状态）
	bool Stop();
	/// 开始录像，供外部接口调用
	uint startRecord(Param arg, int channel = -1);  // -1代表所有通道
	/// 停止录像，供外部接口调用
	uint stopRecord(Param arg, int channel = -1, std::string strCode = "", bool bNotify = false);
	/// 获得指定录像模式的录像状态
	/// \param [in] arg 要获取指定录像模式的录像状态
	uint getState(ulong mode = 0);
	//手工开始录象,跟配置无关,配置改动后,效果消失
	void StartRecordManual(int iChannel);
	//手工停止录象,跟配置无关,配置改动后,效果消失
	void StopRecordManual(int iChannel);
	//触发报警录像
	void DoRecord(int iChannel);
	//消除报警录像
	void ClearRecord(int iChannel);

private:
	/// 线程的执行体, 主要代替定时器的任务
	void ThreadProc();

	/// 录像配置变化时
	void onConfigRecord(const CConfigTable& cCfgRecord, int& iRet);

	uint saveRecordConfig();

private:
	
	RecordConfigAll	m_cCfgRecord;			/// 录像配置类

	// CRecord*		m_pRecord[N_SYS_CH+N_DECORDR_CH];		/// 通道对象
	// bool			m_bAllowPack[N_SYS_CH+N_DECORDR_CH];
	// VideoChannel 	m_cfgVideoChannel[N_SYS_CH + N_DECORDR_CH];	// 视频通道配置

	CMutex			m_RecordCtrlMutex;				/// 录像开关锁
	// SystemTime		m_SystemTime;					/// 系统时间
	// CTimer			m_DetectTimer;					/// 用来检测时间是否跳变的定时器
	// DateTime		m_dateTime;						/// 用来检测系统时间
	int				m_nChannels;
	bool			m_started;
	uint			m_FtpNotify[N_SYS_CH+N_DECORDR_CH];
	uint			m_recMode;				// 录像模式 
};

#define g_RecordManager (*CRecordManager::instance())

#endif
