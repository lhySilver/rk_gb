#ifndef _MEDIA_PHONE_MSG_H_
#define _MEDIA_PHONE_MSG_H_

#include "Infra/Thread.h"
#include "Types/Defs.h"
#include "Manager/EventManager.h"
#include "ExchangeAL/Exchange.h"
#include "ExchangeAL/NetWorkExchange.h"
#include <queue>
#include "Infra/Packet.h"
#include "Infra/Mutex.h"


//短消息
struct ShortMsg
{
	char pText[256];        //消息正文
};


//彩信
struct MultimediaMsg
{
	char pMMSName[64];//打包好的要发送的mms文件名称
};


class CPhoneMsg : CThread
{
public:
	static CPhoneMsg* instance();
	CPhoneMsg();
	~CPhoneMsg();

	bool start();
	void onAppEvent(std::string code, int index, appEventAction action, const EventHandler* pEventHandle, CConfigTable *pTable);

private:
	void createShortMsg(int index, int alarmType, appEventAction action);
	void createMultimediaMsg(int alarmChn, int snapChn, int alarmType, appEventAction action);
	bool packMultimediaMsg(const char* pDesPhoneNum, const char* pTitle, const unsigned char* pBuf, int len, MultimediaMsg* pMM);
	void ThreadProc(void);
	void onConfigShortMsg(const CConfigTable& config, int& ret);
	void onConfigMultimediaMsg(const CConfigTable& config, int& ret);
	int  onConsole(int argc, char **argv);

private:
	std::queue<ShortMsg> m_SMQueue;          //要发送的短信队列
	std::queue<MultimediaMsg> m_MMQueue;     //要发送的彩信队列
	NetShortMsgCfg       m_shortMsgCfg;      //短信发送配置
	NetMultimediaMsgCfg  m_multimediaMsgCfg; //彩信发送配置
	char*                m_pShortMsgRecever[32];  //指向存储接收短信的手机号的缓冲
	CMutex               m_MMDeleteMutex;    //彩信队列及文件的删除互斥锁，保证一个文件在使用时不被其它地方删除

};

#endif //_MEDIA_PHONE_MSG_H_
