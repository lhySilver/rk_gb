#include "Infra/IPAddress.h"
#include <stdlib.h>
#include <stdio.h>

CIPAddress::CIPAddress(ulong addr)
{
	SetAddress(addr);
}

CIPAddress::CIPAddress(const std::string &addr)
{
	SetAddress(addr);
}

std::string CIPAddress::Format() const
{
	uchar cIp[4];
	char buf[32];

	for (int i=0; i<4; i++)
	{
		cIp[i] = (uchar)((m_addr>>(i*8)) & (0xff));
	}
	sprintf(buf, "%d.%d.%d.%d", cIp[0], cIp[1], cIp[2], cIp[3]);
	return buf;
}

uint CIPAddress::GetAddress() const
{
	return m_addr;
}

void CIPAddress::SetAddress(ulong addr)
{
	m_addr = addr;
}

/// 设置IP地址
void CIPAddress::SetAddress(const std::string &addr)
{
	signed char j = 0;
	uchar cIp;
	uint uiRet = 0;
	const char *p = addr.c_str();

	cIp = (unsigned char)atoi(p);
	uiRet |= ( ((unsigned int)(cIp)) << (j*8) );
	j++;

	while ( (*p != 0)
		&& j >= 0)
	{
		if (*p != '.')
		{
			p++;
			continue;
		}

		cIp = (unsigned char)atoi(++p);
		uiRet |= ( ((unsigned int)(cIp)) << (j*8) );
		j++;
	}
	m_addr = uiRet;
}
