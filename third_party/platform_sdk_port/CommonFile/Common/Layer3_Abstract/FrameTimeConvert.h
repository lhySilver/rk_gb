
#ifndef __FRAME_TIME_CONVERT__
#define __FRAME_TIME_CONVERT__
#include "ShareSDK.h"
#include "PFTime.h"
/************************************************************************

************************************************************************/
//此模块时间全部是以100纳秒为单位的FILETIME时间定义方式
class CFrameTimeConvert
{
public:
	CFrameTimeConvert();
	void SetFirstTime(LONGLONG localTime,LONGLONG remoteTime);//在回放时使用，指定第一帧绝对时间和相对时间
	virtual ~CFrameTimeConvert();
	inline const FILETIME &GetFrameLocalTime(const LONGLONG &remoteTime);
	inline const FILETIME &GetFrameLocalTime(const FILETIME &remoteTime);
	inline LONGLONG GetFrameLocalTimeLL(const LONGLONG &remoteTime);
	inline LONGLONG GetFrameLocalTimeLL(const FILETIME &remoteTime);

protected:
	LONGLONG m_LocalTime;//和SYSTEMTIME一样
	LONGLONG m_RemoteTime;//和SystemTime一样
};


const FILETIME &CFrameTimeConvert::GetFrameLocalTime(const LONGLONG &remoteTime)
{
	if(m_LocalTime == 0)//第一次接收到帧
	{
		CPFTime pfTime;
		FILETIME tmpLocalTime;
		pfTime.GetFileTime(tmpLocalTime);
		memcpy(&m_LocalTime,&tmpLocalTime,sizeof(ULONGLONG));
		m_RemoteTime=remoteTime;

		FILETIME* pft = (FILETIME*)&m_LocalTime;
		return pft[0];
	}
	else
	{
		LONGLONG tmp1=10000000/*116444736000000000*/, tmp2=137903904000000000; //1601/*1970*/-2038
		if (remoteTime<tmp1 || remoteTime>tmp2)
		{
			NORMAL_OUTPUT("设备发送过来的帧时间异常 remoteTime=%lld", remoteTime);
		}
		else if (remoteTime < m_RemoteTime)
		{
			/////////////写日志
			NORMAL_OUTPUT("设备发送过来的帧时间出现了后一帧小于前一帧的时间 remoteTime=%lld, m_RemoteTime=%lld", remoteTime,m_RemoteTime);
			if (((tuint32)((m_RemoteTime - remoteTime)/10)) > (1000*1000*10))//前面有除10，后值代表10秒
			{
				m_RemoteTime=remoteTime;
			}
		}
		else
		{
			if( ((tuint32)((remoteTime - m_RemoteTime)/10)) > (1000*1000*10) )//前面有除10，后值代表10秒
			{	
				/////////////写日志
				NORMAL_OUTPUT("设备发送过来的帧时间错误  时间间隔   %d  ", ((tuint32)((remoteTime - m_RemoteTime)/10)));
				m_LocalTime += 10*1000*1000;//1秒
			}
			else
			{
				m_LocalTime += (remoteTime-m_RemoteTime);
			}

			m_RemoteTime=remoteTime;
		}

		return ((FILETIME*)&m_LocalTime)[0];
	}
}

const FILETIME &CFrameTimeConvert::GetFrameLocalTime(const FILETIME &remoteTime)
{
	LONGLONG time=((LONGLONG*)&remoteTime)[0];
	return GetFrameLocalTime(time);
}

LONGLONG CFrameTimeConvert::GetFrameLocalTimeLL(const LONGLONG &remoteTime)
{
	FILETIME ft=GetFrameLocalTime(remoteTime);
	return ((LONGLONG*)&ft)[0];
}

LONGLONG CFrameTimeConvert::GetFrameLocalTimeLL(const FILETIME &remoteTime)
{
	FILETIME ft=GetFrameLocalTime(remoteTime);
	return ((LONGLONG*)&ft)[0];
}

#endif //__FRAME_TIME_CONVERT__