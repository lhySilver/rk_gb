
/************************************************************************/
/* 带流媒体和存储功能的SDK                                                                     */
/************************************************************************/
///////////////////////////NVRServerSDK

#ifndef _N_SDK_COMMON_OBSERVER_H
#define _N_SDK_COMMON_OBSERVER_H

#include "base_type_define.h"
#include "Interlocked.h"
#include "NetProtocolDefine_ForNVMS.h"

//很多功能函数都使用了以下这个通知模式，在初始化时将自己加入观察者队列，退出时移除
//执行操作时返回一个唯一的ID，这个ID在观察者回调函数里面用来判断是不是本次操作
//没有使用在调用函数时传入观察者的原因是因为可能存在操作返回时对象已经被销毁的情况

//全部阻塞函数返回CBufferData*指针的，都需要调用者自己Dec一次，失败时会返回NULL,错误原因通过NVRServer_SDK_GetLastError获得，_ErrorDefine.h里面定义
class CWaitObserver
{
public:
	CWaitObserver(void) : m_vlCount(1) {};		//对象创建时引用为1

	//几乎所有的函数都具有操作对象，destID就是操作对象的ID
	virtual void OnWaitForResult(tuint32 optID,tuint32 destID,bool isOK,const void *pFrameData, tuint32 frameDataLen,LPVOID lParam)=0;

	void Add(void)
	{
		m_vlCount.Increment();
	}

	void Release(void)
	{
		if (m_vlCount.Decrement() == 0)
		{
			delete this;
		}
	}

protected:
	virtual ~CWaitObserver(void) {}

private:
	Interlocked m_vlCount;
};

class CStreamCaptureObserver
{
public:
	CStreamCaptureObserver(void){};
public:
	virtual ~CStreamCaptureObserver(void){};

	/////////////////////////////////////////
	virtual void StreamCapture(const tuint32 streamID, const char *pBuf,tuint32 iLen,tuint32 errorCode)=0;
};


class CCommonInfoObserver
{
public:
	CCommonInfoObserver(void){};
public:
	virtual ~CCommonInfoObserver(void){};

	/////////////////////////////////////////
	virtual void SetDogLicenseInfo(NVMS_NET_PROTOCOL::ECMS_REPORT_DOG_TIME st){return;};
	virtual void SetVisitorTimeoutInfo(NVMS_NET_PROTOCOL::ECMS_REPORT_VISITOR_TIMEOUT st){return;};
	virtual void SetOutLicenseForAddChannel(NVMS_NET_PROTOCOL::NET_OUT_LICENSE_FOR_ADDCHANNEL st){return;};
	virtual void SetServerStateWithRSU(NVMS_NET_PROTOCOL::ECMS_REPORT_SERVER_STATE st){return;};
    virtual void UpdateDevOffLineReasion(void* pInBuffer, int len){return;};
};



typedef enum _read_record_position_{READ_RECORD_POSITON_RANGE_BEGIN, READ_RECORD_DEVICE, READ_RECORD_STORAGE, READ_RECORD_LOCAL, READ_RECORD_POSITON_RANGE_END}READ_RECORD_POSITION;


#endif  //////_N_SDK_COMMON_OBSERVER_H











