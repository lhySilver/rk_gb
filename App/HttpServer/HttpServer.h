#ifndef _HTTP_SERVER_H
#define _HTTP_SERVER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <sys/ioctl.h>
#include <net/if.h>

#include <sys/prctl.h>
#include <fcntl.h>
#include <Types/Defs.h>
#include <errno.h>
#include <time.h>
#include <sys/time.h>
#include <netdb.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include <thread>
#include <memory>
#include <iostream>
#include <string>
#include <sys/prctl.h>

#include "Infra/Singleton.h"
#include "Infra/Thread.h"

#include "event2/http.h"
#include "event2/http_struct.h"
#include "event2/event.h"
#include "event2/buffer.h"
#include "event2/buffer_compat.h"
#include "event2/keyvalq_struct.h"
#include "event2/listener.h"
#define CBUF_MAX (1024)
#define FILE_CBUF_MAX (10*1024)
class CHttpServer : public CThread
{
	PATTERN_SINGLETON_DECLARE(CHttpServer);

public:
	typedef std::string (*HandleCb)(std::string sreq);
	typedef struct options {
		int port;
		int iocp;
		int verbose;
		int unlink;
		char cmd_req_path[64];
		char info_req_path[64];
		char check_req_path[64];
		char getlicense_req_path[64];
		char push_req_path[64];
		char pull_req_path[64];
		char burn_license_req_path[64];
		char push_file_name[64];
		char wifi_info_req_path[64];

		char get_req_path[64];
		HandleCb get_req;

		HandleCb cmd;
		HandleCb info;
		HandleCb check;
		HandleCb getlicense;
		HandleCb push_file;
		HandleCb pull_file;
		HandleCb burn_license;
		HandleCb wifi_info;
		const char *unixsock;
		const char *docroot;
	}OPTIONS;

	int Init();
	int UnInit();
	int Start(OPTIONS *opt);
	int Stop();

private:
	void ThreadProc(void);

	struct event_config *cfg;
	struct event_base *base;
	struct evhttp *http;
	struct evhttp_bound_socket *handle;
	struct evconnlistener *lev;
	struct event *term;
	OPTIONS o;
};

#define g_HttpServer (*CHttpServer::instance())

#endif

