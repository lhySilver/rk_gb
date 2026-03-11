#ifndef HTTP_DATA_PACKET_HEAD
#define HTTP_DATA_PACKET_HEAD
#include "HttpDefine.h"
#include <deque>
#include <string>
#include "ShareSDK.h"
using namespace std;

typedef struct _DATA_PACK_
{
	_DATA_PACK_():m_pBuff(0), m_nLen(0){}
	~_DATA_PACK_()
	{
		if (m_pBuff != 0)
		{
			delete []m_pBuff;
			m_pBuff = 0;
		}
	}
	_DATA_PACK_ &operator=(const _DATA_PACK_ &other)
	{
		if (this != &other)
		{
			this->m_nLen = other.m_nLen;

			if (this->m_pBuff != 0)
			{
				delete []this->m_pBuff;
				this->m_pBuff = 0;
			}

			if (other.m_pBuff != 0 && other.m_nLen > 0)
			{
				this->m_pBuff = new char[this->m_nLen+1];
				memset(this->m_pBuff, 0, this->m_nLen+1);
				memcpy(this->m_pBuff, other.m_pBuff, this->m_nLen);
			}
		}

		return *this;
	}

	void Append(const char *pBuff, const int nLen)
	{
		ASSERT(pBuff != 0 && nLen > 0);
		if (m_pBuff == 0)
		{
			m_nLen = nLen;
			m_pBuff = new char[m_nLen+1];
			m_pBuff[m_nLen] = 0;
			memcpy(m_pBuff, pBuff, m_nLen);
		}
		else
		{
			char *pTemp = new char[m_nLen+nLen+1];
			pTemp[m_nLen+nLen] = 0;
			memcpy(pTemp, m_pBuff, m_nLen);
			memcpy(pTemp+m_nLen, pBuff, nLen);
			m_nLen += nLen;

			delete []m_pBuff;
			m_pBuff = pTemp;
		}
	}

	void StrCpy(const char *pSrc, const int nLen)
	{
		ASSERT(pSrc != 0 && nLen > 0);

		m_nLen = nLen;
		if (m_pBuff != 0)
		{
			delete []m_pBuff;
			m_pBuff = 0;
		}

		m_pBuff = new char[m_nLen+1];
		m_pBuff[m_nLen] = 0;
		memcpy(m_pBuff, pSrc, nLen);
	}

	void DelFromHeader(const int nLen)
	{
		ASSERT(nLen <= (int)m_nLen && m_pBuff != 0);
		if (nLen <= (int)m_nLen && m_pBuff != 0)
		{
			int nTempLen = m_nLen-nLen;
			char *pTemp = 0;
			if (nTempLen > 0)
			{
				pTemp = new char[nTempLen+1];
				pTemp[nTempLen] = 0;
				memcpy(pTemp, m_pBuff+nLen, nTempLen);
			}

			m_nLen = nTempLen;
			delete []m_pBuff;
			m_pBuff = pTemp;
		}
	}

	int GetHeader(std::string &strHeader)
	{
		int nHeaderLen = 0;
		if (m_pBuff == 0)
		{
			return 0;
		}

		// 查找HTTP头结束标志
		char *ptmp = strstr(m_pBuff, HTTP_DEFINE::DOUBLE_ENTER_LINE);
		if (ptmp)
		{
			nHeaderLen = (ptmp - m_pBuff) + strlen(HTTP_DEFINE::DOUBLE_ENTER_LINE);
			strHeader.assign(m_pBuff, nHeaderLen);
		}

		return nHeaderLen;
	}

	int Length(void)const{return m_nLen;}

	char *m_pBuff;									// 数据包
	tuint32 m_nLen;									// 数据包长度
}*P_DATA_PACK, DATA_PACK;

class CHttpDataPacket
{
public:
	CHttpDataPacket(void);
	~CHttpDataPacket(void);

	void ReleasePack(P_DATA_PACK &pDataPack);
	void PushBack(const char *szHttp, tuint32 len);					// 把接收到的未解析的数据插入缓冲区
	bool IsComplete(void);								// 判断是否已经成功接收一个HTTP请求包
	bool HasPendingPacket(void);						// 是否还有未处理的包
	P_DATA_PACK GetNextPendingPacket(void);				// 获取下一个未处理的包
	tuint32 getPayLoadLen();
private:
	CHttpDataPacket &operator=(const CHttpDataPacket &other);
	bool RecombinePacket(void);							// 从接收缓冲区中重组一个完整的数据包

private:
	deque<P_DATA_PACK> m_RevPendingPackMap;	// 已接收到的但是未处理的包

	DATA_PACK m_RevBuff;				// 接收到的数据包
	tuint32 m_nPayLoadLen;				// 首个数据包负载长度
};

#endif //HTTP_DATA_PACKET_HEAD
