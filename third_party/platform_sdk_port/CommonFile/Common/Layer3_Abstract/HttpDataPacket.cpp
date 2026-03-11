// HttpSocket.cpp: implementation of the CHttpSocket class.
//
//////////////////////////////////////////////////////////////////////
#include "base_type_define.h"
#include "HttpDataPacket.h"
#include "HttpDefine.h"
#include <sstream>
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//const int MAX_PAYLOAD_LEN = 80*1024*1024;
CHttpDataPacket::CHttpDataPacket(void)
{
	m_nPayLoadLen = 0;
}


CHttpDataPacket::~CHttpDataPacket(void)
{
	while (!m_RevPendingPackMap.empty())
	{
		deque<P_DATA_PACK>::iterator PackIter = m_RevPendingPackMap.begin();
		delete *PackIter;
		m_RevPendingPackMap.pop_front();
	}

	ASSERT(m_RevPendingPackMap.empty());
}

void CHttpDataPacket::PushBack(const char *szHttp, tuint32 len)
{
	m_RevBuff.Append(szHttp, len);

	// 重组数据包
	while(RecombinePacket() && m_RevBuff.Length() > 0);
}

void CHttpDataPacket::ReleasePack(P_DATA_PACK &pDataPack)
{
	if (pDataPack == 0)
	{
		ASSERT(false);
		return;
	}

	delete pDataPack;
	pDataPack = 0;
}

bool CHttpDataPacket::IsComplete(void)
{
	// 如果接收的负载数据大于10MB，则标志为接收完成(具体的应用进行处理)
// 	if (m_nPayLoadLen > MAX_PAYLOAD_LEN)
// 	{
// 		return true;
// 	}

	if (m_RevPendingPackMap.empty() || m_RevBuff.Length() > 0 )
	{
		return false;
	}

	return true;
}

bool CHttpDataPacket::RecombinePacket(void)
{
	std::string strHeader;
	int nHeaderLen = 0;
	nHeaderLen = m_RevBuff.GetHeader(strHeader);
	if(nHeaderLen <= 0)
	{
		return false;
	}

	// 查找"Content-Length"是否存在，如不存在则表示该请求没有数据部分
	int nLenPos = strHeader.find(HTTP_DEFINE::CONTENT_LENGTH);
	int nLenPos2 = -1;
	nLenPos2 = strHeader.find(HTTP_DEFINE::CONTENT_LENGTH_EX);//兼容谷歌插件Postman
	if (nLenPos2>=0)
	{
		nLenPos = nLenPos2;
	}
	if (nLenPos < 0)
	{
		P_DATA_PACK pPack = new DATA_PACK;
		pPack->StrCpy(m_RevBuff.m_pBuff, nHeaderLen);
		m_RevPendingPackMap.push_back(pPack);
		m_RevBuff.DelFromHeader(nHeaderLen);

		if (m_RevBuff.Length() > 0)
		{
			return true;
		}

		return false;
	}

	// 查找"Content-Length"字段所在行的结束位置
	int nLenEndPos = strHeader.find("\r\n", nLenPos);
	int nLenBeginPos = nLenPos + string(HTTP_DEFINE::CONTENT_LENGTH).length();

	// 获取数据部分长度
	string strPayLoadLen = strHeader.substr(nLenBeginPos, nLenEndPos-nLenBeginPos); 
	m_nPayLoadLen = atoi(strPayLoadLen.c_str());


	// 获取已经收到的数据长度
	//int nFist = strHeader.find(HTTP_DEFINE::DOUBLE_ENTER_LINE) + strlen(HTTP_DEFINE::DOUBLE_ENTER_LINE);
	int nRevLen = m_RevBuff.m_nLen - nHeaderLen;//strHeader.length() - nFist;

	//NORMAL_OUTPUT("请求文本描述是%d,实际是%d",m_nPayLoadLen,nRevLen);
// 	if (m_nPayLoadLen > MAX_PAYLOAD_LEN)
// 	{
// 		P_DATA_PACK pPack = new DATA_PACK;
// 		pPack->StrCpy(m_RevBuff.m_pBuff, nHeaderLen+nRevLen);
// 		m_RevPendingPackMap.push_back(pPack);
// 		m_RevBuff.DelFromHeader(nHeaderLen+nRevLen);
// 		return true;
// 	}

	if ((int)m_nPayLoadLen <= nRevLen)
	{
		P_DATA_PACK pPack = new DATA_PACK;
		pPack->StrCpy(m_RevBuff.m_pBuff, nHeaderLen+m_nPayLoadLen);
		m_RevPendingPackMap.push_back(pPack);
		m_RevBuff.DelFromHeader(nHeaderLen+m_nPayLoadLen);
		return true;
	}
	else if ((int)m_nPayLoadLen > nRevLen)		// 如果数据还没有接收完成
	{
		return false;
	}

	ASSERT(false);
	return false;
}

bool CHttpDataPacket::HasPendingPacket(void)
{
	return (!m_RevPendingPackMap.empty());
}

P_DATA_PACK CHttpDataPacket::GetNextPendingPacket(void)
{
	P_DATA_PACK pPacket = (m_RevPendingPackMap.front());
	m_RevPendingPackMap.pop_front();
	return pPacket;
}

tuint32 CHttpDataPacket::getPayLoadLen()
{
	return m_nPayLoadLen;
}