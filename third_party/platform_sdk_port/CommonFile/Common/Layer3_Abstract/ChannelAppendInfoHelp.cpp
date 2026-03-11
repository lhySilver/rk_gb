#include "base_type_define.h"
#include "ChannelAppendInfoHelp.h"
#include "ShareSDK.h"
#include "Iterator.h"
#include <map>
#include <algorithm>
#include "NVMS_Define.h"

CChannelAppendInfoHelp::CChannelAppendInfoHelp(void)
{
	m_ptrRoot = m_AppendXML.AddValue("Root");
}

CChannelAppendInfoHelp::CChannelAppendInfoHelp(const char *szAppendInfo)
{
	m_ptrRoot = m_AppendXML.AddValue("Root");
	m_AppendXML.CloneValue(m_ptrRoot, szAppendInfo);
}

CChannelAppendInfoHelp::~CChannelAppendInfoHelp(void)
{
}

bool CChannelAppendInfoHelp::SetProductModel(const char *szProductModelInfo)
{
	tuint_ptr ptrPos = m_AppendXML.GetFirstItem(m_ptrRoot, "model");
	if (ptrPos == NULL)
	{
		ptrPos = m_AppendXML.AddValue(m_ptrRoot, "model");
	}
	else
	{
		m_AppendXML.DeleteChildrenValue(ptrPos);
	}

	m_AppendXML.SetValue(ptrPos, szProductModelInfo); 
	return true;
}

bool CChannelAppendInfoHelp::GetProductModel(char *szProductModelInfo, tuint32 nBufLen)
{
	return GetChildInfo("model", szProductModelInfo, nBufLen);
}

bool CChannelAppendInfoHelp::SetBindWindowIndex(tint32 nIndex)
{
    tuint_ptr ptrPos = m_AppendXML.GetFirstItem(m_ptrRoot, "win");
    if (ptrPos == NULL)
    {
        ptrPos = m_AppendXML.AddValue(m_ptrRoot, "win");
    }
    m_AppendXML.SetAttribute(ptrPos, "idx", nIndex);

	return true;
}

tint32 CChannelAppendInfoHelp::GetBindWindowIndex(void)
{
    tuint_ptr ptrPos = m_AppendXML.GetFirstItem(m_ptrRoot, "win");
	if (ptrPos != NULL)
	{
        tint32 nIndex = 0;
        m_AppendXML.GetAttribute_i32(ptrPos, "idx", nIndex);
		return nIndex;
	}

	return -1;
}

bool CChannelAppendInfoHelp::SetChlAppendInfo(const char *szInfo)
{
	tuint_ptr ptrPos = m_AppendXML.GetFirstItem(m_ptrRoot, "chlNoFunc");
	if (ptrPos == NULL)
	{
		ptrPos = m_AppendXML.AddValue(m_ptrRoot, "chlNoFunc");
	}
	else
	{
		m_AppendXML.DeleteChildrenValue(ptrPos);
	}

	m_AppendXML.SetValue(ptrPos, szInfo); 
	return true;
}

bool CChannelAppendInfoHelp::GetChlAppendInfo(char *szInfo, tuint32 nBufLen)
{
	return GetChildInfo("chlNoFunc", szInfo, nBufLen);
}

bool CChannelAppendInfoHelp::SetDeviceAppendInfo(const char *szDeviceInfo)
{
	tuint_ptr ptrPos = m_AppendXML.GetFirstItem(m_ptrRoot, "devNoFunc");
	if (ptrPos == NULL)
	{
		ptrPos = m_AppendXML.AddValue(m_ptrRoot, "devNoFunc");
	}
	else
	{
		m_AppendXML.DeleteChildrenValue(ptrPos);
	}

	m_AppendXML.SetValue(ptrPos, szDeviceInfo);

    return true;
	//return SetChildInfo("devInfo", szDeviceInfo);
}

bool CChannelAppendInfoHelp::GetDeviceAppendInfo(char *szDeviceInfo, tuint32 nBufLen)
{
	return GetChildInfo("devNoFunc", szDeviceInfo, nBufLen);
}

bool CChannelAppendInfoHelp::SetAlarmOutTimes(const char *AlarmOutTimes)
{
	tuint_ptr ptrPos = m_AppendXML.GetFirstItem(m_ptrRoot, "alarmOut");
	if (ptrPos == NULL)
	{
		//ptrPos = m_AppendXML.AddValue(m_ptrRoot, "alarmOut");
	}
	else
	{
		m_AppendXML.DeleteValue(ptrPos);
	}

	m_AppendXML.CloneValue(m_ptrRoot, AlarmOutTimes);

	return true;
}

bool CChannelAppendInfoHelp::GetAlarmOutTimes(char *AlarmOutTimes, int nBufLen)
{
	tuint_ptr ptrPos = m_AppendXML.GetFirstItem(m_ptrRoot, "alarmOut");
	if (ptrPos == NULL)
	{
		return false;
	}
	else
	{
		std::string strInfo;
		m_AppendXML.GetXMLData(ptrPos, strInfo);
		strncpy(AlarmOutTimes, strInfo.c_str(), nBufLen-1);
	}
	return true;
	
}
bool CChannelAppendInfoHelp::GetAlarmOutTimes(int nIndex, int &AlarmOutTime)
{
	tuint_ptr ptrPos = m_AppendXML.GetFirstItem(m_ptrRoot, "alarmOut");
	if (ptrPos == NULL)
	{
		return false;
	}
	else
	{
		tuint_ptr ptritem = m_AppendXML.GetFirstItem(ptrPos, "item");
		while(ptritem)
		{
			int id = 0;
			m_AppendXML.GetValue_i32(ptritem, "id", id);
			if (id == nIndex)
			{
				m_AppendXML.GetValue_i32(ptritem, "alarmHoldTime", AlarmOutTime);
				break;
			}
			ptritem = m_AppendXML.GetNextSiblingItem(ptritem);
		}
		return true;
	}	
}

bool CChannelAppendInfoHelp::SetMainStreamAbility(const char *szAbility)
{
    return SetChildInfo("mainAbility", szAbility);
}

bool CChannelAppendInfoHelp::GetMainStreamAbility(char *szAbility, tuint32 nBufLen)
{
    return GetChildInfo("mainAbility", szAbility, nBufLen);
}

bool CChannelAppendInfoHelp::SetSubStreamAbility(const char *szAbility)
{
    return SetChildInfo("subAbility", szAbility);
}

bool CChannelAppendInfoHelp::GetSubStreamAbility(char *szAbility, tuint32 nBufLen)
{
    return GetChildInfo("subAbility", szAbility, nBufLen);
}

bool CChannelAppendInfoHelp::SetAUX1StreamAbility(const char *szAbility)
{
    return SetChildInfo("aux1Ability", szAbility);
}

bool CChannelAppendInfoHelp::GetAUX1StreamAbility(char *szAbility, tuint32 nBufLen)
{
    return GetChildInfo("aux1Ability", szAbility, nBufLen);
}

bool CChannelAppendInfoHelp::SetStreamInfo(const char *szStream)
{
	return SetChildInfo("stream", szStream);
}

bool CChannelAppendInfoHelp::ClearStreamInfo()
{
	tuint_ptr itemPosPTR_First = m_AppendXML.GetFirstItem(m_ptrRoot, "stream");
	if(itemPosPTR_First)
		m_AppendXML.DeleteValue(itemPosPTR_First);
	return true;
}

bool CChannelAppendInfoHelp::SetStreamInfo(int nIndex, const char *szStream)
{
	tuint_ptr itemPosPTR_First = m_AppendXML.GetFirstItem(m_ptrRoot, "stream");
	if(!itemPosPTR_First)
		itemPosPTR_First = m_AppendXML.AddValue(m_ptrRoot, "stream");

	tuint_ptr itemPosPTR_Second = m_AppendXML.GetFirstItem(itemPosPTR_First, "s");
	while (itemPosPTR_Second)
	{
		int nTmpIndex = -1;
        m_AppendXML.GetAttribute_i32(itemPosPTR_Second, "idx", nTmpIndex);
		if ((nTmpIndex - 1) == nIndex)
		{
  			m_AppendXML.DeleteValue(itemPosPTR_Second);
			m_AppendXML.CloneValue(itemPosPTR_First, szStream);
			return true;
		}
		itemPosPTR_Second = m_AppendXML.GetNextSiblingItem(itemPosPTR_Second);
	} 

	m_AppendXML.CloneValue(itemPosPTR_First, szStream); 
	return true;
}

bool CChannelAppendInfoHelp::GetStreamInfo(int nIndex, tuint32 &videoRes, tuint32 &dwRate, tuint32 &dwQuality)
{
	tuint_ptr ptrPos = m_AppendXML.GetFirstItem(m_ptrRoot, "stream/s");
    //std::string    strDebug = m_AppendXML.GetDoc();
	while (ptrPos)
	{
		int nTmpIndex = -1;
        m_AppendXML.GetAttribute_i32(ptrPos, "idx", nTmpIndex);
		if ((nTmpIndex - 1) == nIndex)
		{
			std::string strAttrib;
			m_AppendXML.GetAttributeStr(ptrPos, "res", strAttrib); 
			videoRes = GetVideoResolution(strAttrib.c_str());  

            m_AppendXML.GetAttributeDWORD(ptrPos, "fps", dwRate);
            m_AppendXML.GetAttributeDWORD(ptrPos, "QoI", dwQuality);
			return true;
		}
		ptrPos = m_AppendXML.GetNextSiblingItem(ptrPos);
	}
	return false;
}  

bool CChannelAppendInfoHelp::GetStreamInfo(int nIndex, tuint32 &videoRes, tuint32 &dwRate, tuint32 &dwQuality, char *szEncType)
{
    tuint_ptr ptrPos = m_AppendXML.GetFirstItem(m_ptrRoot, "stream/s");
    //std::string    strDebug = m_AppendXML.GetDoc();
    while (ptrPos)
    {
        int nTmpIndex = -1;
        m_AppendXML.GetAttribute_i32(ptrPos, "idx", nTmpIndex);
        if ((nTmpIndex - 1) == nIndex)
        {
            std::string strAttrib;
            m_AppendXML.GetAttributeStr(ptrPos, "res", strAttrib);
            videoRes = GetVideoResolution(strAttrib.c_str());

            m_AppendXML.GetAttributeDWORD(ptrPos, "fps", dwRate);
            m_AppendXML.GetAttributeDWORD(ptrPos, "QoI", dwQuality);

            if(szEncType != NULL)
            {
                std::string   strTmpValue;
                m_AppendXML.GetAttributeStr(ptrPos, "enct", strTmpValue);
                strcpy(szEncType, strTmpValue.c_str());
            }
            return true;
        }
        ptrPos = m_AppendXML.GetNextSiblingItem(ptrPos);
    }
    return false;
}

bool CChannelAppendInfoHelp::GetStreamInfo(int nIndex, char *szStream, tuint32 nBufLen)
{
	tuint_ptr ptrPos = m_AppendXML.GetFirstItem(m_ptrRoot, "stream/s");
	while (ptrPos)
	{
		int nTmpIndex = -1;
        m_AppendXML.GetAttribute_i32(ptrPos, "idx", nTmpIndex);
		if ((nTmpIndex - 1) == nIndex)
		{
			std::string strXMLValue;
			m_AppendXML.GetXMLData(ptrPos, strXMLValue);
			memcpy(szStream, strXMLValue.c_str(), std::min( (nBufLen - 1), (tuint32)strXMLValue.length()));
			return true;
		}
		ptrPos = m_AppendXML.GetNextSiblingItem(ptrPos);
	}
	return false;  
}

bool CChannelAppendInfoHelp::SetChildInfo(const char *szName, const char *szBuf)
{
	tuint_ptr ptrPos = m_AppendXML.GetFirstItem(m_ptrRoot, szName);
	if (ptrPos == NULL)
	{
		ptrPos = m_AppendXML.AddValue(m_ptrRoot, szName);
	}
	else
	{
		m_AppendXML.DeleteChildrenValue(ptrPos);
	} 
	m_AppendXML.CloneValue(ptrPos, szBuf); 
	return true;
}

bool CChannelAppendInfoHelp::GetChildInfo(const char *szName, char *szBuf, tuint32 nBufLen)
{
	tuint_ptr ptrPos = m_AppendXML.GetFirstItem(m_ptrRoot, szName);
	if (ptrPos != NULL)
	{
		std::string strInfo;
		m_AppendXML.GetChildrenXMLData(ptrPos, strInfo);
		strncpy(szBuf, strInfo.c_str(), nBufLen-1);
		return true;
	}

	return false;
}

bool CChannelAppendInfoHelp::SetSubStream(tuint32 VideoRes, tuint32 dwRate, tuint32 dwQuality)
{
	SetStreamConfig("sub", VideoRes, dwRate, dwQuality);
	return true;
}

bool CChannelAppendInfoHelp::GetSubStream(tuint32 &VideoRes, tuint32 &dwRate, tuint32 &dwQuality)
{
    tuint_ptr ptrPos = GetStreamConfig("sub", VideoRes, dwRate, dwQuality, NULL);
	return ptrPos != NULL;
}

bool CChannelAppendInfoHelp::SetAutoNormalRecStream(tuint32 VideoRes, tuint32 dwRate, tuint32 dwQuality, bool isAudio, tuint32 nStreamType/*0为主码流 1是子码流 2是第三码流*/)
{
	return SetRecordStream("an", VideoRes, dwRate, dwQuality, isAudio, nStreamType);
}

bool CChannelAppendInfoHelp::GetAutoNormalRecStream(tuint32 &VideoRes, tuint32 &dwRate, tuint32 &dwQuality, bool &isAudio, tuint32 &nStreamType/*0为主码流 1是子码流 2是第三码流*/)
{
	return GetRecordStream("an", VideoRes, dwRate, dwQuality, isAudio, nStreamType);
}

bool CChannelAppendInfoHelp::SetAutoEventRecStream(tuint32 VideoRes, tuint32 dwRate, tuint32 dwQuality, bool isAudio, tuint32 nStreamType/*0为主码流 1是子码流 2是第三码流*/)
{
	return SetRecordStream("ae", VideoRes, dwRate, dwQuality, isAudio, nStreamType);
}

bool CChannelAppendInfoHelp::GetAutoEventRecStream(tuint32 &VideoRes, tuint32 &dwRate, tuint32 &dwQuality, bool &isAudio, tuint32 &nStreamType/*0为主码流 1是子码流 2是第三码流*/)
{
	return GetRecordStream("ae", VideoRes, dwRate, dwQuality, isAudio, nStreamType);
}

bool CChannelAppendInfoHelp::SetManualNormalRecStream(tuint32 VideoRes, tuint32 dwRate, tuint32 dwQuality, bool isAudio, tuint32 nStreamType/*0为主码流 1是子码流 2是第三码流*/)
{
	return SetRecordStream("mn", VideoRes, dwRate, dwQuality, isAudio, nStreamType);
}

bool CChannelAppendInfoHelp::GetManualNormalRecStream(tuint32 &VideoRes, tuint32 &dwRate, tuint32 &dwQuality, bool &isAudio, tuint32 &nStreamType/*0为主码流 1是子码流 2是第三码流*/)
{
	return GetRecordStream("mn", VideoRes, dwRate, dwQuality, isAudio, nStreamType);
}

bool CChannelAppendInfoHelp::SetManualNormalRecStream(tuint32 VideoRes, tuint32 dwRate, tuint32 dwQuality, bool isAudio, tuint32 nStreamType/*0为主码流 1是子码流 2是第三码流*/, GUID timefpsGuid)
{
	return SetRecordStream("mn", VideoRes, dwRate, dwQuality, isAudio, nStreamType, timefpsGuid);
}

bool CChannelAppendInfoHelp::GetManualNormalRecStream(tuint32 &VideoRes, tuint32 &dwRate, tuint32 &dwQuality, bool &isAudio, tuint32 &nStreamType/*0为主码流 1是子码流 2是第三码流*/, GUID &timefpsGuid)
{
	return GetRecordStream("mn", VideoRes, dwRate, dwQuality, isAudio, nStreamType, timefpsGuid);
}

bool CChannelAppendInfoHelp::SetManualEventRecStream(tuint32 VideoRes, tuint32 dwRate, tuint32 dwQuality, bool isAudio, tuint32 nStreamType/*0为主码流 1是子码流 2是第三码流*/)
{
	return SetRecordStream("me", VideoRes, dwRate, dwQuality, isAudio, nStreamType);
}

bool CChannelAppendInfoHelp::GetManualEventRecStream(tuint32 &VideoRes, tuint32 &dwRate, tuint32 &dwQuality, bool &isAudio, tuint32 &nStreamType/*0为主码流 1是子码流 2是第三码流*/)
{
	return GetRecordStream("me", VideoRes, dwRate, dwQuality, isAudio, nStreamType);
}

bool CChannelAppendInfoHelp::SetRecordAppendTime(tuint32 dwPerrecordTime, tuint32 dwPostRecordTime)
{
	tuint_ptr ptrPos = m_AppendXML.GetFirstItem(m_ptrRoot, "rec");
	if (ptrPos == NULL)
	{
		ptrPos = m_AppendXML.AddValue(m_ptrRoot, "rec");
	}

	m_AppendXML.SetAttribute(ptrPos, "per", dwPerrecordTime);
	m_AppendXML.SetAttribute(ptrPos, "post", dwPostRecordTime);

	return true;
}

bool CChannelAppendInfoHelp::GetRecordAppendTime(tuint32 &dwPerrecordTime, tuint32 &dwPostRecordTime)
{
	tuint_ptr ptrPos = m_AppendXML.GetFirstItem(m_ptrRoot, "rec");
	if (ptrPos != NULL)
	{
		m_AppendXML.GetAttributeDWORD(ptrPos, "per", dwPerrecordTime);
		m_AppendXML.GetAttributeDWORD(ptrPos, "post", dwPostRecordTime);

		return true;
	}

	return false;
}

bool CChannelAppendInfoHelp::SetFTPRecordStream(tuint32 nStreamType/*0为主码流 1是子码流 2是第三码流*/, bool isAudio)
{
    tuint_ptr ptrPos = m_AppendXML.GetFirstItem(m_ptrRoot, "ftpRec");
	if (ptrPos == NULL)
	{
        ptrPos = m_AppendXML.AddValue(m_ptrRoot, "ftpRec");
	}

	if (nStreamType == 0)
	{
		m_AppendXML.SetAttribute(ptrPos, "type", "main");
	}
	else if (nStreamType == 2)
	{
		m_AppendXML.SetAttribute(ptrPos, "type", "third");
	}
	else
	{
		m_AppendXML.SetAttribute(ptrPos, "type", "sub");
	}
    
	m_AppendXML.SetAttribute(ptrPos, "audio", isAudio ? "ON":"OFF");

	return true;
}

bool CChannelAppendInfoHelp::GetFTPRecordStream(tuint32 &nStreamType/*0为主码流 1是子码流 2是第三码流*/, bool &isAudio)
{
    tuint_ptr ptrPos = m_AppendXML.GetFirstItem(m_ptrRoot, "ftpRec");
	if (ptrPos != NULL)
	{
		std::string strAttrib;
		m_AppendXML.GetAttributeStr(ptrPos, "type", strAttrib);

        if (strAttrib.compare("main") == 0)
		{
			nStreamType = 0;
		}
		else if (strAttrib.compare("third") == 0)
		{
			nStreamType = 2;
		}
		else
		{
			nStreamType = 1;
		}

		m_AppendXML.GetAttributeStr(ptrPos, "audio", strAttrib);

		if (strAttrib.compare("OFF") == 0)
		{
			isAudio = false;
		}
		else
		{
			TASSERT(strAttrib.compare("ON") == 0);
			isAudio = true;
		}

		return true;
	}

	return false;
}

std::string CChannelAppendInfoHelp::GetAppendString(void)
{
	std::string strAppendXML;
	m_AppendXML.GetChildrenXMLData("Root", strAppendXML);
	return strAppendXML;
}

tuint_ptr CChannelAppendInfoHelp::SetStreamConfig(const char *szStreamName, tuint32 VideoRes, tuint32 dwRate, tuint32 dwQuality)
{
	tuint_ptr ptrPos = m_AppendXML.GetFirstItem(m_ptrRoot, szStreamName);
	if (ptrPos == NULL)
	{
		ptrPos = m_AppendXML.AddValue(m_ptrRoot, szStreamName);
	}

	m_AppendXML.SetAttribute(ptrPos, "res", GetVideoResolutionName(VideoRes));
    m_AppendXML.SetAttribute(ptrPos, "fps", dwRate);
    m_AppendXML.SetAttribute(ptrPos, "QoI", dwQuality);

	return ptrPos;
}

tuint_ptr CChannelAppendInfoHelp::GetStreamConfig(const char *szStreamName, tuint32 &VideoRes, tuint32 &dwRate, tuint32 &dwQuality, char *szEncType)
{
	tuint_ptr ptrPos = m_AppendXML.GetFirstItem(m_ptrRoot, szStreamName);
	if (ptrPos != NULL)
	{
		std::string strAttrib;
		m_AppendXML.GetAttributeStr(ptrPos, "res", strAttrib);

		VideoRes = GetVideoResolution(strAttrib.c_str());

        m_AppendXML.GetAttributeDWORD(ptrPos, "fps", dwRate);
        m_AppendXML.GetAttributeDWORD(ptrPos, "QoI", dwQuality);

        if(szEncType != NULL)
        {
            std::string   strTmpValue;
            m_AppendXML.GetAttributeStr(ptrPos, "enct", strTmpValue);
            strcpy(szEncType, strTmpValue.c_str());
            //memcpy(szEncType, strTmpValue.c_str(), std::min(strTmpValue.size(), sizeof(szEncType)-1));
        }
	}

	return ptrPos;
}

bool CChannelAppendInfoHelp::SetRecordStream(const char *szRecordType, tuint32 VideoRes, tuint32 dwRate, tuint32 dwQuality, bool isAudio, tuint32 nStreamType/*0为主码流 1是子码流 2是第三码流*/)
{
	tuint_ptr ptrPos = SetStreamConfig(szRecordType, VideoRes, dwRate, dwQuality);
	m_AppendXML.SetAttribute(ptrPos, "audio", isAudio?"ON":"OFF");
	if (nStreamType == 0)
	{
		m_AppendXML.SetAttribute(ptrPos, "type", "main");
	}
	else if (nStreamType == 2)
	{
		m_AppendXML.SetAttribute(ptrPos, "type", "third");
	}
	else
	{
		m_AppendXML.SetAttribute(ptrPos, "type", "sub");
	}

	return true;
}

bool CChannelAppendInfoHelp::GetRecordStream(const char *szRecordType, tuint32 &VideoRes, tuint32 &dwRate, tuint32 &dwQuality, bool &isAudio, tuint32 &nStreamType/*0为主码流 1是子码流 2是第三码流*/)
{
    tuint_ptr ptrPos = GetStreamConfig(szRecordType, VideoRes, dwRate, dwQuality, NULL);
	if (ptrPos != NULL)
	{
		std::string strAttrib;
		m_AppendXML.GetAttributeStr(ptrPos, "audio", strAttrib);
		if (strAttrib.compare("ON") == 0)
		{
			isAudio = true;
		}
		else
		{
			TASSERT(strAttrib.compare("OFF") == 0);
			isAudio = false;
		}

		m_AppendXML.GetAttributeStr(ptrPos, "type", strAttrib);
		if (strAttrib.compare("main") == 0)
		{
			nStreamType = 0;
		}
		else if (strAttrib.compare("third") == 0)
		{
			nStreamType = 2;
		}
		else
		{
			nStreamType = 1;
		}

		return true;
	}

	return false;
}

bool CChannelAppendInfoHelp::SetRecordStream(const char *szRecordType, tuint32 VideoRes, tuint32 dwRate, tuint32 dwQuality, bool isAudio, tuint32 nStreamType/*0为主码流 1是子码流 2是第三码流*/, GUID timefpsGuid)
{
	tuint_ptr ptrPos = SetStreamConfig(szRecordType, VideoRes, dwRate, dwQuality);
	m_AppendXML.SetAttribute(ptrPos, "audio", isAudio?"ON":"OFF");
	if (nStreamType == 0)
	{
		m_AppendXML.SetAttribute(ptrPos, "type", "main");
	}
	else if (nStreamType == 2)
	{
		m_AppendXML.SetAttribute(ptrPos, "type", "third");
	}
	else
	{
		m_AppendXML.SetAttribute(ptrPos, "type", "sub");
	}
	m_AppendXML.SetAttribute(ptrPos, "timeFpsGuid", timefpsGuid);

	return true;
}

bool CChannelAppendInfoHelp::GetRecordStream(const char *szRecordType, tuint32 &VideoRes, tuint32 &dwRate, tuint32 &dwQuality, bool &isAudio, tuint32 &nStreamType/*0为主码流 1是子码流 2是第三码流*/, GUID &timefpsGuid)
{
	tuint_ptr ptrPos = GetStreamConfig(szRecordType, VideoRes, dwRate, dwQuality, NULL);
	if (ptrPos != NULL)
	{
		std::string strAttrib;
		m_AppendXML.GetAttributeStr(ptrPos, "audio", strAttrib);
		if (strAttrib.compare("ON") == 0)
		{
			isAudio = true;
		}
		else
		{
			TASSERT(strAttrib.compare("OFF") == 0);
			isAudio = false;
		}

		m_AppendXML.GetAttributeStr(ptrPos, "type", strAttrib);
		if (strAttrib.compare("main") == 0)
		{
			nStreamType = 0;
		}
		else if (strAttrib.compare("third") == 0)
		{
			nStreamType = 2;
		}
		else
		{
			nStreamType = 1;
		}

		m_AppendXML.GetAttributeGUID(ptrPos, "timeFpsGuid", timefpsGuid);

		return true;
	}

	return false;
}

std::string CChannelAppendInfoHelp::GetVideoResolutionName(tuint32 VideoRes)
{
	char szVideoRes[16];
	memset(szVideoRes, 0, sizeof(szVideoRes));
	sprintf(szVideoRes, "%dx%d", LOTUINT16(VideoRes), HITUINT16(VideoRes) );

	return szVideoRes;
}

tuint32 CChannelAppendInfoHelp::GetVideoResolution(const char *szVideoResName)
{
	tuint32 dwWidth = 0;
	tuint32 dwHeight = 0;
	sscanf(szVideoResName, "%dx%d", &dwWidth, &dwHeight);

	return MAKETUINT32(dwWidth, dwHeight);//dwHeight放在高位
}

tuint32 CChannelAppendInfoHelp::GetResolutionMultiplication(const char *szVideoResName)
{
	tuint32 dwWidth = 0;
	tuint32 dwHeight = 0;
	sscanf(szVideoResName, "%dx%d", &dwWidth, &dwHeight);

	return (dwWidth * dwHeight);//
}

bool CChannelAppendInfoHelp::GetVideoResolutionWH(tuint32 VideoRes, tuint32 &dwWidth, tuint32 &dwHeight)
{
	dwWidth = LOTUINT16(VideoRes);
	dwHeight = HITUINT16(VideoRes);
	return true;
}

GUID CChannelAppendInfoHelp::GetNodeGUIDByWinIndex(int nLogicWinIndex)
{
// 	CIPChannelIterator  IPChannelIter;
// 	while(IPChannelIter.HasNext())
// 	{
// 		CLocalNode *pLocalNode = IPChannelIter.GetNext();
//         char  szNodeAppendInfo[CFGPACK_DEFINE::APPEND_INFO_BUF_LENGTH] = {0};
// 		pLocalNode->GetNodeAppendInfo(szNodeAppendInfo, sizeof(szNodeAppendInfo));
// 		CChannelAppendInfoHelp    nodeAppendInfo(szNodeAppendInfo);
// 		if( nodeAppendInfo.GetBindWindowIndex() == nLogicWinIndex )
// 		{
// 			return pLocalNode->GetNodeGUID();
// 		}
// 	}
	return GUID_NULL;
}


tuint32 CChannelAppendInfoHelp::GetNodeIDByWinIndex(int nLogicWinIndex)
{
// 	CIPChannelIterator  IPChannelIter;
// 	while(IPChannelIter.HasNext())
// 	{
// 		CLocalNode *pLocalNode = IPChannelIter.GetNext();
//         char  szNodeAppendInfo[CFGPACK_DEFINE::APPEND_INFO_BUF_LENGTH] = {0};
// 		pLocalNode->GetNodeAppendInfo(szNodeAppendInfo, sizeof(szNodeAppendInfo));
// 		CChannelAppendInfoHelp    nodeAppendInfo(szNodeAppendInfo);
// 		if( nodeAppendInfo.GetBindWindowIndex() == nLogicWinIndex )
// 		{
// 			return pLocalNode->GetNodeID();
// 		}
// 	}
	return NVMS_DEFINE::INVALID_NODE_ID;
}


bool CChannelAppendInfoHelp::SetChlNum(tuint32 num)//设置通道数
{
	tuint_ptr ptrPos = m_AppendXML.GetFirstItem(m_ptrRoot, "chls");
	if (ptrPos == NULL)
	{
		ptrPos = m_AppendXML.AddValue_ui64(m_ptrRoot, "chls",num);
		return true;
	}
	else
	{
		m_AppendXML.SetValue_ui64(ptrPos,num);
		return true;
	}
	return false;
}

bool CChannelAppendInfoHelp::GetChlNum(tuint32 &num)//获取通道数
{
	tuint_ptr ptrPos = m_AppendXML.GetFirstItem(m_ptrRoot, "chls");
	if (ptrPos != NULL)
	{
		std::string strAttrib;
		m_AppendXML.GetValueDWORD(ptrPos, num);
		return true;
	}
	return false;
}

bool CChannelAppendInfoHelp::SetSensorNum(tuint32 num)//设置传感器数
{
	tuint_ptr ptrPos = m_AppendXML.GetFirstItem(m_ptrRoot, "sensors");
	if (ptrPos == NULL)
	{
		ptrPos = m_AppendXML.AddValue_ui64(m_ptrRoot, "sensors",num);
		return true;
	}
	else
	{
		m_AppendXML.SetValue_ui64(ptrPos,num);
		return true;
	}
	return false;
}

bool CChannelAppendInfoHelp::GetSensorNum(tuint32 &num)//
{
	tuint_ptr ptrPos = m_AppendXML.GetFirstItem(m_ptrRoot, "sensors");
	if (ptrPos != NULL)
	{
		std::string strAttrib;
		m_AppendXML.GetValueDWORD(ptrPos, num);
		return true;
	}
	return false;
}

bool CChannelAppendInfoHelp::SetAlarmOutNum(tuint32 num)//设置报警输出
{
	tuint_ptr ptrPos = m_AppendXML.GetFirstItem(m_ptrRoot, "alarmOuts");
	if (ptrPos == NULL)
	{
		ptrPos = m_AppendXML.AddValue_ui64(m_ptrRoot, "alarmOuts",num);
		return true;
	}
	else
	{
		m_AppendXML.SetValue_ui64(ptrPos,num);
		return true;
	}
	return false;
}

bool CChannelAppendInfoHelp::GetAlarmOutNum(tuint32 &num)
{
	tuint_ptr ptrPos = m_AppendXML.GetFirstItem(m_ptrRoot, "alarmOuts");
	if (ptrPos != NULL)
	{
		std::string strAttrib;
		m_AppendXML.GetValueDWORD(ptrPos, num);
		return true;
	}
	return false;
}

bool CChannelAppendInfoHelp::SetDevType(const char * type)//设置类型
{
	tuint_ptr ptrPos = m_AppendXML.GetFirstItem(m_ptrRoot, "type");
	if (ptrPos == NULL)
	{
		ptrPos = m_AppendXML.AddValue(m_ptrRoot,"type",type);
		return true;
	}
	else
	{
		m_AppendXML.SetValue(ptrPos,type);
		return true;
	}
	return false;
}

std::string CChannelAppendInfoHelp::GetDevType()//获取设备类型
{
	std::string strType;
	tuint_ptr ptrPos = m_AppendXML.GetFirstItem(m_ptrRoot, "type");
	if (ptrPos != NULL)
	{	
		m_AppendXML.GetValueStr(ptrPos, strType);
	}
	return strType;
}

bool CChannelAppendInfoHelp::SetDevModel(const char *szDevModel)
{
	tuint_ptr ptrPos = m_AppendXML.GetFirstItem(m_ptrRoot, "productModel");
	if (ptrPos == NULL)
	{
		ptrPos = m_AppendXML.AddValue(m_ptrRoot, "productModel", szDevModel);
		return true;
	}
	else
	{
		m_AppendXML.SetValue(ptrPos, szDevModel);
		return true;
	}
	return false;
}

bool CChannelAppendInfoHelp::GetDevModel(char *szDevModel, tuint32 nBufLen)
{
	return GetChildInfo("productModel", szDevModel, nBufLen);
}

bool CChannelAppendInfoHelp::SetDevVersion(const char *szDevVersion)
{
	tuint_ptr ptrPos = m_AppendXML.GetFirstItem(m_ptrRoot, "softwareVer");
	if (ptrPos == NULL)
	{
		ptrPos = m_AppendXML.AddValue(m_ptrRoot, "softwareVer", szDevVersion);
		return true;
	}
	else
	{
		m_AppendXML.SetValue(ptrPos, szDevVersion);
		return true;
	}
	return false;
}

bool CChannelAppendInfoHelp::GetDevVersion(char *szDevVersion, tuint32 nBufLen)
{
	return GetChildInfo("softwareVer", szDevVersion, nBufLen);
}

bool CChannelAppendInfoHelp::SetProductType(const char *szProductType)
{
	tuint_ptr ptrPos = m_AppendXML.GetFirstItem(m_ptrRoot, "producttype");
	if (ptrPos == NULL)
	{
		ptrPos = m_AppendXML.AddValue(m_ptrRoot, "producttype", szProductType);
		return true;
	}
	else
	{
		m_AppendXML.SetValue(ptrPos, szProductType);
		return true;
	}
	return false;
}

bool CChannelAppendInfoHelp::GetProductType(char *szProductType, tuint32 nBufLen)
{
	return GetChildInfo("producttype", szProductType, nBufLen);
}

//////////////////////////////////////////////////////////////////////////
// 获取协议版本号
bool CChannelAppendInfoHelp::SetDevProtocolVersion(tuint32 protocolVersion)
{
    tuint_ptr ptrPos = m_AppendXML.GetFirstItem(m_ptrRoot, "protocolVersion");
    if (ptrPos == NULL)
    {
        ptrPos = m_AppendXML.AddValue(m_ptrRoot, "protocolVersion", protocolVersion);
        return true;
    }
    else
    {
        m_AppendXML.SetValue_i32(ptrPos, protocolVersion);
        return true;
    }
    return false;
}

bool CChannelAppendInfoHelp::GetDevProtocolVersion(tuint32& protocolVersion)
{
    char buf[32] = { 0 };
    bool ret = GetChildInfo("protocolVersion", buf, sizeof(buf));
    if (!ret)
        return false;
    protocolVersion = atoi(buf);
    return ret;
}

//////////////////////////////////////////////////////////////////////////
bool CChannelAppendInfoHelp::SetDecoderOutputNum(tuint32 num)
{
	tuint_ptr ptrPos = m_AppendXML.GetFirstItem(m_ptrRoot, "outputs");
	if (ptrPos == NULL)
	{
		ptrPos = m_AppendXML.AddValue_ui64(m_ptrRoot, "outputs",num);
		return true;
	}
	else
	{
		m_AppendXML.SetValue_ui64(ptrPos,num);
		return true;
	}
	return false;
}

bool CChannelAppendInfoHelp::GetDecoderOutputNum(tuint32 &num)
{
	tuint_ptr ptrPos = m_AppendXML.GetFirstItem(m_ptrRoot, "outputs");
	if (ptrPos != NULL)
	{
		std::string strAttrib;
		m_AppendXML.GetValueDWORD(ptrPos, num);
		return true;
	}
	return false;
}

//预置点数量
bool CChannelAppendInfoHelp::SetPresetNum(tuint32 num)
{
	tuint_ptr ptrPos = m_AppendXML.GetFirstItem(m_ptrRoot, "presetNum");
	if (ptrPos == NULL)
	{
		ptrPos = m_AppendXML.AddValue_ui64(m_ptrRoot, "presetNum",num);
		return true;
	}
	else
	{
		m_AppendXML.SetValue_ui64(ptrPos,num);
		return true;
	}
	return false;
}
bool CChannelAppendInfoHelp::GetPresetNum(tuint32 &num)
{
	tuint_ptr ptrPos = m_AppendXML.GetFirstItem(m_ptrRoot, "presetNum");
	if (ptrPos != NULL)
	{
		std::string strAttrib;
		m_AppendXML.GetValueDWORD(ptrPos, num);
		return true;
	}
	return false;
}
//巡航线数量
bool CChannelAppendInfoHelp::SetCrusieNum(tuint32 num)
{
	tuint_ptr ptrPos = m_AppendXML.GetFirstItem(m_ptrRoot, "cruiseNum");
	if (ptrPos == NULL)
	{
		ptrPos = m_AppendXML.AddValue_ui64(m_ptrRoot, "cruiseNum",num);
		return true;
	}
	else
	{
		m_AppendXML.SetValue_ui64(ptrPos,num);
		return true;
	}
	return false;
}
bool CChannelAppendInfoHelp::GetCrusieNum(tuint32 &num)
{
	tuint_ptr ptrPos = m_AppendXML.GetFirstItem(m_ptrRoot, "cruiseNum");
	if (ptrPos != NULL)
	{
		std::string strAttrib;
		m_AppendXML.GetValueDWORD(ptrPos, num);
		return true;
	}
	return false;
}
//轨迹数量
bool CChannelAppendInfoHelp::SetTraceNum(tuint32 num)
{
	tuint_ptr ptrPos = m_AppendXML.GetFirstItem(m_ptrRoot, "traceNum");
	if (ptrPos == NULL)
	{
		ptrPos = m_AppendXML.AddValue_ui64(m_ptrRoot, "traceNum",num);
		return true;
	}
	else
	{
		m_AppendXML.SetValue_ui64(ptrPos,num);
		return true;
	}
	return false;
}
bool CChannelAppendInfoHelp::GetTraceNum(tuint32 &num)
{
	tuint_ptr ptrPos = m_AppendXML.GetFirstItem(m_ptrRoot, "traceNum");
	if (ptrPos != NULL)
	{
		std::string strAttrib;
		m_AppendXML.GetValueDWORD(ptrPos, num);
		return true;
	}
	return false;
}

bool CChannelAppendInfoHelp::GetN9KChnGUIDs(string& strChnGUIDs)
{
	tuint_ptr ptrPos = m_AppendXML.GetFirstItem(m_ptrRoot, "n9kChlGUIDs");
	if (ptrPos != NULL)
	{
		m_AppendXML.GetChildrenXMLData(ptrPos,strChnGUIDs);
		return true;
	}
	return false;
}

bool CChannelAppendInfoHelp::SetN9KChnGUIDs(string strChnGUIDs)
{
	tuint_ptr ptrPos = m_AppendXML.GetFirstItem(m_ptrRoot, "n9kChlGUIDs");
	if (ptrPos != NULL)
	{
		m_AppendXML.DeleteValue(ptrPos);
	}

	tuint_ptr pNewN9KChlsGUIDs = m_AppendXML.AddValue(m_ptrRoot,"n9kChlGUIDs");
	m_AppendXML.CloneValue(pNewN9KChlsGUIDs,strChnGUIDs);
	return true;
}

void CChannelAppendInfoHelp::DelN9KChnGUIDs()
{
	tuint_ptr ptrPos = m_AppendXML.GetFirstItem(m_ptrRoot, "n9kChlGUIDs");
	if (ptrPos != NULL)
	{
		m_AppendXML.DeleteValue(ptrPos);
	}
	return;
}

//设置监控点的人脸闸机考勤类型 0-未知 1-进入 2-离开
bool CChannelAppendInfoHelp::SetFaceAttendType(tuint32 nType)
{
	tuint_ptr ptrPos = m_AppendXML.GetFirstItem(m_ptrRoot, "FaceAttendType");
	if (ptrPos == NULL)
	{
		ptrPos = m_AppendXML.AddValue_ui64(m_ptrRoot, "FaceAttendType",nType);
		return true;
	}
	else
	{
		m_AppendXML.SetValue_i32(ptrPos,nType);
		return true;
	}
	return false;
}

bool CChannelAppendInfoHelp::GetFaceAttendType(tuint32 &nType)//获取监控点的人脸闸机考勤类型 0-未知 1-进入 2-离开
{
	nType = 0;
	tuint_ptr ptrPos = m_AppendXML.GetFirstItem(m_ptrRoot, "FaceAttendType");
	if (ptrPos != NULL)
	{
		std::string strAttrib;
		m_AppendXML.GetValueDWORD(ptrPos, nType);
		return true;
	}
	return false;
}

bool CChannelAppendInfoHelp::SetChnIsEnableCrossLine(tuint32 nType)//设置监控点的是否启用过线统计 0-不启用 1-启用
{
	tuint_ptr ptrPos = m_AppendXML.GetFirstItem(m_ptrRoot, "ChnIsEnableCrossLine");
	if (ptrPos == NULL)
	{
		ptrPos = m_AppendXML.AddValue_ui64(m_ptrRoot, "ChnIsEnableCrossLine",nType);
		return true;
	}
	else
	{
		m_AppendXML.SetValue_i32(ptrPos,nType);
		return true;
	}
	return false;
}

bool CChannelAppendInfoHelp::GetChnIsEnableCrossLine(tuint32 &nType)//获取监控点的是否启用过线统计 0-不启用 1-启用
{
	nType = 0;
	tuint_ptr ptrPos = m_AppendXML.GetFirstItem(m_ptrRoot, "ChnIsEnableCrossLine");
	if (ptrPos != NULL)
	{
		std::string strAttrib;
		m_AppendXML.GetValueDWORD(ptrPos, nType);
		return true;
	}
	return false;
}

bool CChannelAppendInfoHelp::SetChnCrossLineType(tuint32 nType)//设置能力集不支持过线统计的监控点启用过线统计后,进出类型(0-未配置  1-进入 2-离开 3-支持过线统计的ipc不需要配置） 
{
	tuint_ptr ptrPos = m_AppendXML.GetFirstItem(m_ptrRoot, "CrossLineType");
	if (ptrPos == NULL)
	{
		ptrPos = m_AppendXML.AddValue_ui64(m_ptrRoot, "CrossLineType",nType);
		return true;
	}
	else
	{
		m_AppendXML.SetValue_i32(ptrPos,nType);
		return true;
	}
	return false;
}

bool CChannelAppendInfoHelp::GetChnCrossLineType(tuint32 &nType)//获取能力集不支持过线统计的监控点启用过线统计后,进出类型(0-未配置  1-进入 2-离开 3-支持过线统计的ipc不需要配置） 
{
	nType = 0;
	tuint_ptr ptrPos = m_AppendXML.GetFirstItem(m_ptrRoot, "CrossLineType");
	if (ptrPos != NULL)
	{
		std::string strAttrib;
		m_AppendXML.GetValueDWORD(ptrPos, nType);
		return true;
	}
	return false;
}

bool CChannelAppendInfoHelp::SetChnIntelligentScene(string str)
{
	tuint_ptr ptrPos = m_AppendXML.GetFirstItem(m_ptrRoot, "IntelligentScene");
	if (ptrPos == NULL)
	{
		ptrPos = m_AppendXML.AddValue(m_ptrRoot, "IntelligentScene",str.c_str());
		return true;
	}
	else
	{
		m_AppendXML.SetValue(ptrPos,str.c_str());
		return true;
	}
	return false;
}

bool CChannelAppendInfoHelp::GetChnIntelligentScene(string& str)
{
	tuint_ptr ptrPos = m_AppendXML.GetFirstItem(m_ptrRoot, "IntelligentScene");
	if (ptrPos != NULL)
	{
		m_AppendXML.GetValueStr(ptrPos, str);
		return true;
	}
	return false;
}

bool CChannelAppendInfoHelp::SetChnIASrvFaceMatch(tuint32 isIASrvFaceMatch)
{
    tuint_ptr ptrPos = m_AppendXML.GetFirstItem(m_ptrRoot, "IsIASrvFaceMatch");
    if (ptrPos == NULL)
    {
        ptrPos = m_AppendXML.AddValue(m_ptrRoot, "IsIASrvFaceMatch", isIASrvFaceMatch);
        return true;
    }
    else
    {
        m_AppendXML.SetValue(ptrPos, isIASrvFaceMatch);
        return true;
    }
    return false;
}

//判断是否启用了智能分析服务器比对
bool CChannelAppendInfoHelp::GetChnIASrvFaceMatch(tuint32& isIASrvFaceMatch)
{
    tuint_ptr ptrPos = m_AppendXML.GetFirstItem(m_ptrRoot, "IsIASrvFaceMatch");
    if (ptrPos != NULL)
    {
        m_AppendXML.GetValueDWORD(ptrPos, isIASrvFaceMatch);
        return true;
    }
    return false;
}

bool CChannelAppendInfoHelp::GetChnEnableAccessScene()//启用了人脸门禁场景
{
	tuint_ptr ptrPos = m_AppendXML.GetFirstItem(m_ptrRoot, "IntelligentScene");
	if (ptrPos != NULL)
	{
		string str;
		m_AppendXML.GetValueStr(ptrPos, str);
		if (str.find("access")!=string::npos)
		{
			return true;
		}
	}
	return false;
}

bool CChannelAppendInfoHelp::GetChnEnableAttendScene()//启用了人脸考勤场景
{
	tuint_ptr ptrPos = m_AppendXML.GetFirstItem(m_ptrRoot, "IntelligentScene");
	if (ptrPos != NULL)
	{
		string str;
		m_AppendXML.GetValueStr(ptrPos, str);
		if (str.find("attend")!=string::npos)
		{
			return true;
		}
	}
	return false;
}

bool CChannelAppendInfoHelp::GetChnEnableGuestScene()//启用了人脸迎宾场景
{
	tuint_ptr ptrPos = m_AppendXML.GetFirstItem(m_ptrRoot, "IntelligentScene");
	if (ptrPos != NULL)
	{
		string str;
		m_AppendXML.GetValueStr(ptrPos, str);
		if (str.find("guest")!=string::npos)
		{
			return true;
		}
	}
	return false;
}

bool CChannelAppendInfoHelp::SetLongitude(const std::string& str)
{
	tuint_ptr ptrPos = m_AppendXML.GetFirstItem(m_ptrRoot, "longitude");
	if (ptrPos == NULL)
	{
		ptrPos = m_AppendXML.AddValue(m_ptrRoot, "longitude",str.c_str());
		return true;
	}
	else
	{
		m_AppendXML.SetValue(ptrPos,str.c_str());
		return true;
	}
	return false;
}

bool CChannelAppendInfoHelp::GetLongitude(std::string& str)
{
	tuint_ptr ptrPos = m_AppendXML.GetFirstItem(m_ptrRoot, "longitude");
	if (ptrPos != NULL)
	{
		m_AppendXML.GetValueStr(ptrPos, str);
		return true;
	}
	return false;
}

bool CChannelAppendInfoHelp::SetLatitude(const std::string& str)
{
	tuint_ptr ptrPos = m_AppendXML.GetFirstItem(m_ptrRoot, "latitude");
	if (ptrPos == NULL)
	{
		ptrPos = m_AppendXML.AddValue(m_ptrRoot, "latitude",str.c_str());
		return true;
	}
	else
	{
		m_AppendXML.SetValue(ptrPos,str.c_str());
		return true;
	}
	return false;
}

bool CChannelAppendInfoHelp::GetLatitude(std::string& str)
{
	tuint_ptr ptrPos = m_AppendXML.GetFirstItem(m_ptrRoot, "latitude");
	if (ptrPos != NULL)
	{
		m_AppendXML.GetValueStr(ptrPos, str);
		return true;
	}
	return false;
}

bool CChannelAppendInfoHelp::SetPlaceCode(const std::string& str)
{
	tuint_ptr ptrPos = m_AppendXML.GetFirstItem(m_ptrRoot, "placeCode");
	if (ptrPos == NULL)
	{
		ptrPos = m_AppendXML.AddValue(m_ptrRoot, "placeCode",str.c_str());
		return true;
	}
	else
	{
		m_AppendXML.SetValue(ptrPos,str.c_str());
		return true;
	}
	return false;
}

bool CChannelAppendInfoHelp::GetPlaceCode(std::string& str)
{
	tuint_ptr ptrPos = m_AppendXML.GetFirstItem(m_ptrRoot, "placeCode");
	if (ptrPos != NULL)
	{
		m_AppendXML.GetValueStr(ptrPos, str);
		return true;
	}
	return false;
}