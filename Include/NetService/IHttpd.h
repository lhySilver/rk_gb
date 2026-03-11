#ifndef _IHTTPD_H
#define _IHTTPD_H

#include "Infra/Types.h"
#include "Infra/Function.h"
#include "PAL/Net.h"


class IHttpd 
{
public:
	static IHttpd *instance();
	virtual ~IHttpd();

	enum ConfigKind
	{
		HTTPD_CFG = 0,
		HTTPD_SYSINFO,
	};

	typedef TFunction3<int, int, void *, int> Proc;

	virtual int HttpdStart(int iWebPort, int iAppPort, int iRtspPort, int iLanguage, const char* strWebRoot, const char* webFile) = 0;
	virtual int HttpdSetActiveXUrl(const char* pURL) = 0;
	virtual int HttpdStop(void) = 0;
	virtual int HttpdRestart(void) = 0;
	virtual int HttpdSetMapPort(int iWebPort, int iAppPort) = 0;
	virtual int HttpdCallBack(Proc proc) = 0;
	virtual bool HttpdConfig(ConfigKind cfgKind, const char *pConfig, int nSize) = 0;
	virtual int HttpdStart(WEB_PARAM *pWebParam) = 0;

private:
};

#endif //_IHTTPD_H

