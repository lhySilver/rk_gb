#include "HttpServer.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <signal.h>

#ifdef EVENT__HAVE_SYS_UN_H
#include <sys/un.h>
#endif
#ifdef EVENT__HAVE_AFUNIX_H
#include <afunix.h>
#endif
char uri_root[512];

static const struct table_entry {
	const char *extension;
	const char *content_type;
} content_type_table[] = {
	{ "txt", "text/plain" },
	{ "c", "text/plain" },
	{ "h", "text/plain" },
	{ "html", "text/html" },
	{ "htm", "text/htm" },
	{ "css", "text/css" },
	{ "gif", "image/gif" },
	{ "jpg", "image/jpeg" },
	{ "jpeg", "image/jpeg" },
	{ "png", "image/png" },
	{ "pdf", "application/pdf" },
	{ "ps", "application/postscript" },
	{ NULL, NULL },
};
/* Try to guess a good content-type for 'path' */
static const char *
guess_content_type(const char *path)
{
	const char *last_period, *extension;
	const struct table_entry *ent;
	last_period = strrchr(path, '.');
	if (!last_period || strchr(last_period, '/'))
		goto not_found; /* no exension */
	extension = last_period + 1;
	for (ent = &content_type_table[0]; ent->extension; ++ent) {
		if (!evutil_ascii_strcasecmp(ent->extension, extension))
			return ent->content_type;
	}

not_found:
	return "application/misc";
}

/* Callback used for the /dump URI, and for every non-GET request:
 * dumps all information to stdout and gives back a trivial 200 ok */
static void
dump_request_cb(struct evhttp_request *req, void *arg)
{
	const char *cmdtype;
	struct evkeyvalq *headers;
	struct evkeyval *header;
	struct evbuffer *buf;

	switch (evhttp_request_get_command(req)) {
	case EVHTTP_REQ_GET: cmdtype = "GET"; break;
	case EVHTTP_REQ_POST: cmdtype = "POST"; break;
	case EVHTTP_REQ_HEAD: cmdtype = "HEAD"; break;
	case EVHTTP_REQ_PUT: cmdtype = "PUT"; break;
	case EVHTTP_REQ_DELETE: cmdtype = "DELETE"; break;
	case EVHTTP_REQ_OPTIONS: cmdtype = "OPTIONS"; break;
	case EVHTTP_REQ_TRACE: cmdtype = "TRACE"; break;
	case EVHTTP_REQ_CONNECT: cmdtype = "CONNECT"; break;
	case EVHTTP_REQ_PATCH: cmdtype = "PATCH"; break;
	default: cmdtype = "unknown"; break;
	}

	printf("Received a %s request for %s\nHeaders:\n",
		cmdtype, evhttp_request_get_uri(req));

	headers = evhttp_request_get_input_headers(req);
	for (header = headers->tqh_first; header;
		header = header->next.tqe_next) {
		printf("  %s: %s\n", header->key, header->value);
	}

	buf = evhttp_request_get_input_buffer(req);
	puts("Input data: <<<");
	while (evbuffer_get_length(buf)) {
		int n;
		char cbuf[128];
		n = evbuffer_remove(buf, cbuf, sizeof(cbuf));
		if (n > 0)
			(void) fwrite(cbuf, 1, n, stdout);
	}
	puts(">>>");

	evhttp_send_reply(req, 200, "OK", NULL);
}

static void
info_request_cb(struct evhttp_request *req, void *arg)
{
	const char *cmdtype;
	struct evkeyvalq *headers;
	struct evkeyval *header;
	struct evbuffer *buf;
	size_t buf_size;

	switch (evhttp_request_get_command(req)) {
	case EVHTTP_REQ_GET: cmdtype = "GET"; break;
	case EVHTTP_REQ_POST: cmdtype = "POST"; break;
	case EVHTTP_REQ_HEAD: cmdtype = "HEAD"; break;
	case EVHTTP_REQ_PUT: cmdtype = "PUT"; break;
	case EVHTTP_REQ_DELETE: cmdtype = "DELETE"; break;
	case EVHTTP_REQ_OPTIONS: cmdtype = "OPTIONS"; break;
	case EVHTTP_REQ_TRACE: cmdtype = "TRACE"; break;
	case EVHTTP_REQ_CONNECT: cmdtype = "CONNECT"; break;
	case EVHTTP_REQ_PATCH: cmdtype = "PATCH"; break;
	default: cmdtype = "unknown"; break;
	}

	printf("Received a %s request for %s\nHeaders:\n",
		cmdtype, evhttp_request_get_uri(req));

	headers = evhttp_request_get_input_headers(req);
	for (header = headers->tqh_first; header;
		header = header->next.tqe_next) {
		printf("  %s: %s\n", header->key, header->value);
	}

	char cbuf[CBUF_MAX+1];

	buf = evhttp_request_get_input_buffer(req);
	buf_size = evbuffer_get_length(buf);
	#if 1
	if (buf_size <= 0)
	{
		printf("buf_size is empty!\n");
		return;
	}
	else
	{
		size_t copy_size = buf_size > CBUF_MAX ? CBUF_MAX : buf_size;
		printf("line:%d,len:%d, copy_len:%d\n",__LINE__,buf_size,copy_size);
		memcpy(cbuf, (char *)EVBUFFER_DATA(buf), copy_size);
		cbuf[buf_size] = '\0';
		printf("line:%d,msg:%s\n",__LINE__,cbuf);
	}
	#else
	puts("\nInput data: <<<");
	int i = 0;
	int n;
	while (evbuffer_get_length(buf)) {
		if (i >= sizeof(cbuf)-1)
			break;
		n = evbuffer_remove(buf, cbuf+i, sizeof(cbuf)-1);
		i+=n;
		if (n > 0)
			(void) fwrite(cbuf, 1, n, stdout);
	}
	puts(">>>\n");
	#endif

	CHttpServer::OPTIONS *po = (CHttpServer::OPTIONS *) arg;
	
	if (po && po->info)
	{
		std::string resq = po->info(cbuf);
		printf("resp=%s\n",resq.c_str());
		struct evbuffer* evbuf = evbuffer_new();
		evbuffer_add_printf(evbuf, "%s\n",resq.c_str());
		evhttp_send_reply(req, HTTP_OK, "OK", evbuf);
		evbuffer_free(evbuf);
	}
	else
	{
		evhttp_send_reply(req, HTTP_OK, "OK", NULL);
	}
}
static void
get_req_cb(struct evhttp_request *req, void *arg)
{
	const char *cmdtype;
	struct evkeyvalq *headers;
	struct evkeyval *header;
	struct evbuffer *buf;
	size_t buf_size;

	switch (evhttp_request_get_command(req)) {
	case EVHTTP_REQ_GET: cmdtype = "GET"; break;
	case EVHTTP_REQ_POST: cmdtype = "POST"; break;
	case EVHTTP_REQ_HEAD: cmdtype = "HEAD"; break;
	case EVHTTP_REQ_PUT: cmdtype = "PUT"; break;
	case EVHTTP_REQ_DELETE: cmdtype = "DELETE"; break;
	case EVHTTP_REQ_OPTIONS: cmdtype = "OPTIONS"; break;
	case EVHTTP_REQ_TRACE: cmdtype = "TRACE"; break;
	case EVHTTP_REQ_CONNECT: cmdtype = "CONNECT"; break;
	case EVHTTP_REQ_PATCH: cmdtype = "PATCH"; break;
	default: cmdtype = "unknown"; break;
	}

	printf("Received a %s request for %s\nHeaders:\n",
		cmdtype, evhttp_request_get_uri(req));

	headers = evhttp_request_get_input_headers(req);

	for (header = headers->tqh_first; header;
		header = header->next.tqe_next) {
		printf("  %s: %s\n", header->key, header->value);
	}


	// char cbuf[CBUF_MAX+1];

	// buf = evhttp_request_get_input_buffer(req);
	// buf_size = evbuffer_get_length(buf);
	// #if 1
	// if (buf_size <= 0)
	// {
	// 	printf("buf_size is empty!\n");
	// 	return;
	// }
	// else
	// {
	// 	size_t copy_size = buf_size > CBUF_MAX ? CBUF_MAX : buf_size;
	// 	printf("line:%d,len:%d, copy_len:%d\n",__LINE__,buf_size,copy_size);
	// 	memcpy(cbuf, (char *)EVBUFFER_DATA(buf), copy_size);
	// 	cbuf[buf_size] = '\0';
	// 	printf("line:%d,msg:%s\n",__LINE__,cbuf);
	// }
	// #else
	// puts("\nInput data: <<<");
	// int i = 0;
	// int n;
	// while (evbuffer_get_length(buf)) {
	// 	if (i >= sizeof(cbuf)-1)
	// 		break;
	// 	n = evbuffer_remove(buf, cbuf+i, sizeof(cbuf)-1);
	// 	i+=n;
	// 	if (n > 0)
	// 		(void) fwrite(cbuf, 1, n, stdout);
	// }
	// puts(">>>\n");
	
	// #endif

	CHttpServer::OPTIONS *po = (CHttpServer::OPTIONS *) arg;
	
	if (po && po->get_req)
	{
		std::string resq = po->get_req(evhttp_request_get_uri(req));
		printf("resp=%s\n",resq.c_str());
		struct evbuffer* evbuf = evbuffer_new();
		evbuffer_add_printf(evbuf, "%s\n",resq.c_str());
		evhttp_send_reply(req, HTTP_OK, "OK", evbuf);
		evbuffer_free(evbuf);
	}
	else
	{
		evhttp_send_reply(req, HTTP_OK, "OK", NULL);
	}
}
static void
cmd_request_cb(struct evhttp_request *req, void *arg)
{
	const char *cmdtype;
	struct evkeyvalq *headers;
	struct evkeyval *header;
	struct evbuffer *buf;
	size_t buf_size;

	switch (evhttp_request_get_command(req)) {
	case EVHTTP_REQ_GET: cmdtype = "GET"; break;
	case EVHTTP_REQ_POST: cmdtype = "POST"; break;
	case EVHTTP_REQ_HEAD: cmdtype = "HEAD"; break;
	case EVHTTP_REQ_PUT: cmdtype = "PUT"; break;
	case EVHTTP_REQ_DELETE: cmdtype = "DELETE"; break;
	case EVHTTP_REQ_OPTIONS: cmdtype = "OPTIONS"; break;
	case EVHTTP_REQ_TRACE: cmdtype = "TRACE"; break;
	case EVHTTP_REQ_CONNECT: cmdtype = "CONNECT"; break;
	case EVHTTP_REQ_PATCH: cmdtype = "PATCH"; break;
	default: cmdtype = "unknown"; break;
	}

	printf("Received a %s request for %s\nHeaders:\n",
		cmdtype, evhttp_request_get_uri(req));

	headers = evhttp_request_get_input_headers(req);
	for (header = headers->tqh_first; header;
		header = header->next.tqe_next) {
		printf("  %s: %s\n", header->key, header->value);
	}

	char cbuf[CBUF_MAX+1];

	buf = evhttp_request_get_input_buffer(req);
	buf_size = evbuffer_get_length(buf);
	#if 1
	if (buf_size <= 0)
	{
		printf("buf_size is empty!\n");
		return;
	}
	else
	{
		size_t copy_size = buf_size > CBUF_MAX ? CBUF_MAX : buf_size;
		printf("line:%d,len:%d, copy_len:%d\n",__LINE__,buf_size,copy_size);
		memcpy(cbuf, (char *)EVBUFFER_DATA(buf), copy_size);
		cbuf[buf_size] = '\0';
		printf("line:%d,msg:%s\n",__LINE__,cbuf);
	}
	#else
	puts("\nInput data: <<<");
	int i = 0;
	int n;
	while (evbuffer_get_length(buf)) {
		if (i >= sizeof(cbuf)-1)
			break;
		n = evbuffer_remove(buf, cbuf+i, sizeof(cbuf)-1);
		i+=n;
		if (n > 0)
			(void) fwrite(cbuf, 1, n, stdout);
	}
	puts(">>>\n");
	
	#endif

	CHttpServer::OPTIONS *po = (CHttpServer::OPTIONS *) arg;
	
	if (po && po->cmd)
	{
		std::string resq = po->cmd(cbuf);
		printf("resp=%s\n",resq.c_str());
		struct evbuffer* evbuf = evbuffer_new();
		evbuffer_add_printf(evbuf, "%s\n",resq.c_str());
		evhttp_send_reply(req, HTTP_OK, "OK", evbuf);
		evbuffer_free(evbuf);
	}
	else
	{
		evhttp_send_reply(req, HTTP_OK, "OK", NULL);
	}
}
static void
check_request_cb(struct evhttp_request *req, void *arg)
{
	const char *cmdtype;
	struct evkeyvalq *headers;
	struct evkeyval *header;
	struct evbuffer *buf;
	size_t buf_size;

	switch (evhttp_request_get_command(req)) {
	case EVHTTP_REQ_GET: cmdtype = "GET"; break;
	case EVHTTP_REQ_POST: cmdtype = "POST"; break;
	case EVHTTP_REQ_HEAD: cmdtype = "HEAD"; break;
	case EVHTTP_REQ_PUT: cmdtype = "PUT"; break;
	case EVHTTP_REQ_DELETE: cmdtype = "DELETE"; break;
	case EVHTTP_REQ_OPTIONS: cmdtype = "OPTIONS"; break;
	case EVHTTP_REQ_TRACE: cmdtype = "TRACE"; break;
	case EVHTTP_REQ_CONNECT: cmdtype = "CONNECT"; break;
	case EVHTTP_REQ_PATCH: cmdtype = "PATCH"; break;
	default: cmdtype = "unknown"; break;
	}

	printf("Received a %s request for %s\nHeaders:\n",
		cmdtype, evhttp_request_get_uri(req));

	headers = evhttp_request_get_input_headers(req);
	for (header = headers->tqh_first; header;
		header = header->next.tqe_next) {
		printf("  %s: %s\n", header->key, header->value);
	}

	// char cbuf[CBUF_MAX+1];

	// buf = evhttp_request_get_input_buffer(req);
	// buf_size = evbuffer_get_length(buf);
	// #if 1
	// if (buf_size <= 0)
	// {
	// 	printf("buf_size is empty!\n");
	// 	return;
	// }
	// else
	// {
	// 	size_t copy_size = buf_size > CBUF_MAX ? CBUF_MAX : buf_size;
	// 	printf("line:%d,len:%d, copy_len:%d\n",__LINE__,buf_size,copy_size);
	// 	memcpy(cbuf, (char *)EVBUFFER_DATA(buf), copy_size);
	// 	cbuf[buf_size] = '\0';
	// 	printf("line:%d,msg:%s\n",__LINE__,cbuf);
	// }
	// #else
	// puts("\nInput data: <<<");
	// int i = 0;
	// int n;
	// while (evbuffer_get_length(buf)) {
	// 	if (i >= sizeof(cbuf)-1)
	// 		break;
	// 	n = evbuffer_remove(buf, cbuf+i, sizeof(cbuf)-1);
	// 	i+=n;
	// 	if (n > 0)
	// 		(void) fwrite(cbuf, 1, n, stdout);
	// }
	// puts(">>>\n");
	
	// #endif

	CHttpServer::OPTIONS *po = (CHttpServer::OPTIONS *) arg;
	
	if (po && po->check)
	{
		std::string resq = po->check("");
		printf("resp=%s\n",resq.c_str());
		struct evbuffer* evbuf = evbuffer_new();
		evbuffer_add_printf(evbuf, "%s\n",resq.c_str());
		evhttp_send_reply(req, HTTP_OK, "OK", evbuf);
		evbuffer_free(evbuf);
	}
	else
	{
		evhttp_send_reply(req, HTTP_OK, "OK", NULL);
	}
}
static void
getlicense_request_cb(struct evhttp_request *req, void *arg)
{
	const char *cmdtype;
	struct evkeyvalq *headers;
	struct evkeyval *header;
	struct evbuffer *buf;
	size_t buf_size;

	switch (evhttp_request_get_command(req)) {
	case EVHTTP_REQ_GET: cmdtype = "GET"; break;
	case EVHTTP_REQ_POST: cmdtype = "POST"; break;
	case EVHTTP_REQ_HEAD: cmdtype = "HEAD"; break;
	case EVHTTP_REQ_PUT: cmdtype = "PUT"; break;
	case EVHTTP_REQ_DELETE: cmdtype = "DELETE"; break;
	case EVHTTP_REQ_OPTIONS: cmdtype = "OPTIONS"; break;
	case EVHTTP_REQ_TRACE: cmdtype = "TRACE"; break;
	case EVHTTP_REQ_CONNECT: cmdtype = "CONNECT"; break;
	case EVHTTP_REQ_PATCH: cmdtype = "PATCH"; break;
	default: cmdtype = "unknown"; break;
	}

	printf("Received a %s request for %s\nHeaders:\n",
		cmdtype, evhttp_request_get_uri(req));

	headers = evhttp_request_get_input_headers(req);
	for (header = headers->tqh_first; header;
		header = header->next.tqe_next) {
		printf("  %s: %s\n", header->key, header->value);
	}

	// char cbuf[CBUF_MAX+1];

	// buf = evhttp_request_get_input_buffer(req);
	// buf_size = evbuffer_get_length(buf);
	// #if 1
	// if (buf_size <= 0)
	// {
	// 	printf("buf_size is empty!\n");
	// 	return;
	// }
	// else
	// {
	// 	size_t copy_size = buf_size > CBUF_MAX ? CBUF_MAX : buf_size;
	// 	printf("line:%d,len:%d, copy_len:%d\n",__LINE__,buf_size,copy_size);
	// 	memcpy(cbuf, (char *)EVBUFFER_DATA(buf), copy_size);
	// 	cbuf[buf_size] = '\0';
	// 	printf("line:%d,msg:%s\n",__LINE__,cbuf);
	// }
	// #else
	// puts("\nInput data: <<<");
	// int i = 0;
	// int n;
	// while (evbuffer_get_length(buf)) {
	// 	if (i >= sizeof(cbuf)-1)
	// 		break;
	// 	n = evbuffer_remove(buf, cbuf+i, sizeof(cbuf)-1);
	// 	i+=n;
	// 	if (n > 0)
	// 		(void) fwrite(cbuf, 1, n, stdout);
	// }
	// puts(">>>\n");
	
	// #endif

	CHttpServer::OPTIONS *po = (CHttpServer::OPTIONS *) arg;
	
	if (po && po->getlicense)
	{
		std::string resq = po->getlicense("");
		printf("resp=%s\n",resq.c_str());
		struct evbuffer* evbuf = evbuffer_new();
		evbuffer_add_printf(evbuf, "%s\n",resq.c_str());
		evhttp_send_reply(req, HTTP_OK, "OK", evbuf);
		evbuffer_free(evbuf);
	}
	else
	{
		evhttp_send_reply(req, HTTP_OK, "OK", NULL);
	}
}
static void
wifi_info_request_cb(struct evhttp_request *req, void *arg)
{
	const char *cmdtype;
	struct evkeyvalq *headers;
	struct evkeyval *header;
	struct evbuffer *buf;
	size_t buf_size;

	switch (evhttp_request_get_command(req)) {
	case EVHTTP_REQ_GET: cmdtype = "GET"; break;
	case EVHTTP_REQ_POST: cmdtype = "POST"; break;
	case EVHTTP_REQ_HEAD: cmdtype = "HEAD"; break;
	case EVHTTP_REQ_PUT: cmdtype = "PUT"; break;
	case EVHTTP_REQ_DELETE: cmdtype = "DELETE"; break;
	case EVHTTP_REQ_OPTIONS: cmdtype = "OPTIONS"; break;
	case EVHTTP_REQ_TRACE: cmdtype = "TRACE"; break;
	case EVHTTP_REQ_CONNECT: cmdtype = "CONNECT"; break;
	case EVHTTP_REQ_PATCH: cmdtype = "PATCH"; break;
	default: cmdtype = "unknown"; break;
	}

	printf("Received a %s request for %s\nHeaders:\n",
		cmdtype, evhttp_request_get_uri(req));

	headers = evhttp_request_get_input_headers(req);
	for (header = headers->tqh_first; header;
		header = header->next.tqe_next) {
		printf("  %s: %s\n", header->key, header->value);
	}

	// char cbuf[CBUF_MAX+1];

	// buf = evhttp_request_get_input_buffer(req);
	// buf_size = evbuffer_get_length(buf);
	// #if 1
	// if (buf_size <= 0)
	// {
	// 	printf("buf_size is empty!\n");
	// 	return;
	// }
	// else
	// {
	// 	size_t copy_size = buf_size > CBUF_MAX ? CBUF_MAX : buf_size;
	// 	printf("line:%d,len:%d, copy_len:%d\n",__LINE__,buf_size,copy_size);
	// 	memcpy(cbuf, (char *)EVBUFFER_DATA(buf), copy_size);
	// 	cbuf[buf_size] = '\0';
	// 	printf("line:%d,msg:%s\n",__LINE__,cbuf);
	// }
	// #else
	// puts("\nInput data: <<<");
	// int i = 0;
	// int n;
	// while (evbuffer_get_length(buf)) {
	// 	if (i >= sizeof(cbuf)-1)
	// 		break;
	// 	n = evbuffer_remove(buf, cbuf+i, sizeof(cbuf)-1);
	// 	i+=n;
	// 	if (n > 0)
	// 		(void) fwrite(cbuf, 1, n, stdout);
	// }
	// puts(">>>\n");
	
	// #endif

	CHttpServer::OPTIONS *po = (CHttpServer::OPTIONS *) arg;
	
	if (po && po->wifi_info)
	{
		std::string resq = po->wifi_info("wifi info");
		printf("resp=%s\n",resq.c_str());
		struct evbuffer* evbuf = evbuffer_new();
		evbuffer_add_printf(evbuf, "%s\n",resq.c_str());
		evhttp_send_reply(req, HTTP_OK, "OK", evbuf);
		evbuffer_free(evbuf);
	}
	else
	{
		evhttp_send_reply(req, HTTP_OK, "OK", NULL);
	}
}

static void
burn_license_request_cb(struct evhttp_request *req, void *arg)
{
	const char *cmdtype;
	struct evkeyvalq *headers;
	struct evkeyval *header;
	struct evbuffer *buf;
	size_t buf_size;

	switch (evhttp_request_get_command(req)) {
	case EVHTTP_REQ_GET: cmdtype = "GET"; break;
	case EVHTTP_REQ_POST: cmdtype = "POST"; break;
	case EVHTTP_REQ_HEAD: cmdtype = "HEAD"; break;
	case EVHTTP_REQ_PUT: cmdtype = "PUT"; break;
	case EVHTTP_REQ_DELETE: cmdtype = "DELETE"; break;
	case EVHTTP_REQ_OPTIONS: cmdtype = "OPTIONS"; break;
	case EVHTTP_REQ_TRACE: cmdtype = "TRACE"; break;
	case EVHTTP_REQ_CONNECT: cmdtype = "CONNECT"; break;
	case EVHTTP_REQ_PATCH: cmdtype = "PATCH"; break;
	default: cmdtype = "unknown"; break;
	}

	printf("Received a %s request for %s\nHeaders:\n",
		cmdtype, evhttp_request_get_uri(req));

	headers = evhttp_request_get_input_headers(req);
	for (header = headers->tqh_first; header;
		header = header->next.tqe_next) {
		printf("  %s: %s\n", header->key, header->value);
	}

	char cbuf[CBUF_MAX+1];

	buf = evhttp_request_get_input_buffer(req);
	buf_size = evbuffer_get_length(buf);
	#if 1
	if (buf_size <= 0)
	{
		printf("buf_size is empty!\n");
		return;
	}
	else
	{
		size_t copy_size = buf_size > CBUF_MAX ? CBUF_MAX : buf_size;
		printf("line:%d,len:%d, copy_len:%d\n",__LINE__,buf_size,copy_size);
		memcpy(cbuf, (char *)EVBUFFER_DATA(buf), copy_size);
		cbuf[buf_size] = '\0';
		printf("line:%d,msg:%s\n",__LINE__,cbuf);
	}
	#else
	puts("\nInput data: <<<");
	int i = 0;
	int n;
	while (evbuffer_get_length(buf)) {
		if (i >= sizeof(cbuf)-1)
			break;
		n = evbuffer_remove(buf, cbuf+i, sizeof(cbuf)-1);
		i+=n;
		if (n > 0)
			(void) fwrite(cbuf, 1, n, stdout);
	}
	puts(">>>\n");

	#endif

	CHttpServer::OPTIONS *po = (CHttpServer::OPTIONS *) arg;
	
	if (po && po->burn_license)
	{
		std::string resq = po->burn_license(cbuf);
		printf("resp=%s\n",resq.c_str());
		struct evbuffer* evbuf = evbuffer_new();
		evbuffer_add_printf(evbuf, "%s\n",resq.c_str());
		evhttp_send_reply(req, HTTP_OK, "OK", evbuf);
		evbuffer_free(evbuf);
	}
	else
	{
		evhttp_send_reply(req, HTTP_OK, "OK", NULL);
	}
}
static void
pull_file_request_cb(struct evhttp_request *req, void *arg)
{
	const char *cmdtype;
	struct evkeyvalq *headers;
	struct evkeyval *header;
	struct evbuffer *buf;
	size_t buf_size;

	switch (evhttp_request_get_command(req)) {
	case EVHTTP_REQ_GET: cmdtype = "GET"; break;
	case EVHTTP_REQ_POST: cmdtype = "POST"; break;
	case EVHTTP_REQ_HEAD: cmdtype = "HEAD"; break;
	case EVHTTP_REQ_PUT: cmdtype = "PUT"; break;
	case EVHTTP_REQ_DELETE: cmdtype = "DELETE"; break;
	case EVHTTP_REQ_OPTIONS: cmdtype = "OPTIONS"; break;
	case EVHTTP_REQ_TRACE: cmdtype = "TRACE"; break;
	case EVHTTP_REQ_CONNECT: cmdtype = "CONNECT"; break;
	case EVHTTP_REQ_PATCH: cmdtype = "PATCH"; break;
	default: cmdtype = "unknown"; break;
	}

	printf("Received a %s request for %s\nHeaders:\n",
		cmdtype, evhttp_request_get_uri(req));

	headers = evhttp_request_get_input_headers(req);
	for (header = headers->tqh_first; header;
		header = header->next.tqe_next) {
		printf("  %s: %s\n", header->key, header->value);
	}

	char cbuf[FILE_CBUF_MAX+1];

	buf = evhttp_request_get_input_buffer(req);
	buf_size = evbuffer_get_length(buf);
	#if 1
	if (buf_size <= 0)
	{
		printf("buf_size is empty!\n");
		return;
	}
	else
	{
		size_t copy_size = buf_size > FILE_CBUF_MAX ? FILE_CBUF_MAX : buf_size;
		printf("line:%d,len:%d, copy_len:%d\n",__LINE__,buf_size,copy_size);
		memcpy(cbuf, (char *)EVBUFFER_DATA(buf), copy_size);
		cbuf[buf_size] = '\0';
		printf("line:%d,msg:%s\n",__LINE__,cbuf);
	}
	#else
	puts("\nInput data: <<<");
	int i = 0;
	int n;
	while (evbuffer_get_length(buf)) {
		if (i >= sizeof(cbuf)-1)
			break;
		n = evbuffer_remove(buf, cbuf+i, sizeof(cbuf)-1);
		i+=n;
		if (n > 0)
			(void) fwrite(cbuf, 1, n, stdout);
	}
	puts(">>>\n");

	#endif

	CHttpServer::OPTIONS *po = (CHttpServer::OPTIONS *) arg;
	
	if (po && po->pull_file)
	{
		std::string resq = po->pull_file(cbuf);
		printf("resp=%s\n",resq.c_str());
		struct evbuffer* evbuf = evbuffer_new();
		evbuffer_add_printf(evbuf, "%s\n",resq.c_str());
		evhttp_send_reply(req, HTTP_OK, "OK", evbuf);
		evbuffer_free(evbuf);
	}
	else
	{
		evhttp_send_reply(req, HTTP_OK, "OK", NULL);
	}
}
/* This callback gets invoked when we get any http request that doesn't match
 * any other callback.  Like any evhttp server callback, it has a simple job:
 * it must eventually call evhttp_send_error() or evhttp_send_reply().
 */

static void
push_file_request_cb(struct evhttp_request *req, void *arg)
{
	struct evbuffer *evb = NULL;
	CHttpServer::OPTIONS *o = (CHttpServer::OPTIONS *) arg;
	const char *uri = evhttp_request_get_uri(req);
	//struct evhttp_uri *decoded = NULL;
	//const char *path;
	//char *decoded_path;
	char *whole_path = NULL;
	//size_t len;
	int fd = -1;
	struct stat st;

	if (evhttp_request_get_command(req) != EVHTTP_REQ_GET) {
		dump_request_cb(req, arg);
		return;
	}

	if (o) {
		whole_path = o->push_file_name;
	}
	else {
		dump_request_cb(req, arg);
		return;
	}

	printf("Got a GET request for <%s>\n",  uri);
	#if 0
	/* Decode the URI */
	decoded = evhttp_uri_parse(uri);
	if (!decoded) {
		printf("It's not a good URI. Sending BADREQUEST\n");
		evhttp_send_error(req, HTTP_BADREQUEST, 0);
		return;
	}

	/* Let's see what path the user asked for. */
	path = evhttp_uri_get_path(decoded);
	if (!path) path = "/";

	/* We need to decode it, to see what path the user really wanted. */
	decoded_path = evhttp_uridecode(path, 0, NULL);
	if (decoded_path == NULL)
		goto err;
	/* Don't allow any ".."s in the path, to avoid exposing stuff outside
	 * of the docroot.  This test is both overzealous and underzealous:
	 * it forbids aceptable paths like "/this/one..here", but it doesn't
	 * do anything to prevent symlink following." */
	if (strstr(decoded_path, ".."))
		goto err;

	len = strlen(decoded_path)+strlen(o->docroot)+2;
	if (!(whole_path = (char*)malloc(len))) {
		perror("malloc");
		goto err;
	}
	evutil_snprintf(whole_path, len, "%s/%s", o->docroot, decoded_path);

	if (stat(whole_path, &st)<0) {
		goto err;
	}
	#endif
	
	/* This holds the content we're sending. */
	evb = evbuffer_new();

	{
		/* Otherwise it's a file; add it to the buffer to get
		 * sent via sendfile */
		const char *type = content_type_table[0].content_type;//guess_content_type(decoded_path);
		if ((fd = open(whole_path, O_RDONLY)) < 0) {
			perror("open");
			goto err;
		}

		if (fstat(fd, &st)<0) {
			/* Make sure the length still matches, now that we
			 * opened the file :/ */
			perror("fstat");
			goto err;
		}
		evhttp_add_header(evhttp_request_get_output_headers(req),
			"Content-Type", type);
		evbuffer_add_file(evb, fd, 0, st.st_size);
	}

	evhttp_send_reply(req, 200, "OK", evb);
	goto done;
err:
	evhttp_send_error(req, 404, "Document was not found");
	if (fd>=0)
		close(fd);
done:
	// if (decoded)
	// 	evhttp_uri_free(decoded);
	// if (decoded_path)
	// 	free(decoded_path);
	// if (whole_path)
	// 	free(whole_path);
	if (evb)
		evbuffer_free(evb);
}
#if 0
static void
send_document_cb(struct evhttp_request *req, void *arg)
{
	struct evbuffer *evb = NULL;
	CHttpServer::OPTIONS *o = (CHttpServer::OPTIONS *) arg;
	const char *uri = evhttp_request_get_uri(req);
	struct evhttp_uri *decoded = NULL;
	const char *path;
	char *decoded_path;
	char *whole_path = NULL;
	size_t len;
	int fd = -1;
	struct stat st;

	if (evhttp_request_get_command(req) != EVHTTP_REQ_GET) {
		dump_request_cb(req, arg);
		return;
	}

	printf("Got a GET request for <%s>\n",  uri);

	/* Decode the URI */
	decoded = evhttp_uri_parse(uri);
	if (!decoded) {
		printf("It's not a good URI. Sending BADREQUEST\n");
		evhttp_send_error(req, HTTP_BADREQUEST, 0);
		return;
	}

	/* Let's see what path the user asked for. */
	path = evhttp_uri_get_path(decoded);
	if (!path) path = "/";

	/* We need to decode it, to see what path the user really wanted. */
	decoded_path = evhttp_uridecode(path, 0, NULL);
	if (decoded_path == NULL)
		goto err;
	/* Don't allow any ".."s in the path, to avoid exposing stuff outside
	 * of the docroot.  This test is both overzealous and underzealous:
	 * it forbids aceptable paths like "/this/one..here", but it doesn't
	 * do anything to prevent symlink following." */
	if (strstr(decoded_path, ".."))
		goto err;

	len = strlen(decoded_path)+strlen(o->docroot)+2;
	if (!(whole_path = (char*)malloc(len))) {
		perror("malloc");
		goto err;
	}
	evutil_snprintf(whole_path, len, "%s/%s", o->docroot, decoded_path);

	if (stat(whole_path, &st)<0) {
		goto err;
	}

	/* This holds the content we're sending. */
	evb = evbuffer_new();

	if (S_ISDIR(st.st_mode)) {
		/* If it's a directory, read the comments and make a little
		 * index page */
#ifdef _WIN32
		HANDLE d;
		WIN32_FIND_DATAA ent;
		char *pattern;
		size_t dirlen;
#else
		DIR *d;
		struct dirent *ent;
#endif
		const char *trailing_slash = "";

		if (!strlen(path) || path[strlen(path)-1] != '/')
			trailing_slash = "/";

#ifdef _WIN32
		dirlen = strlen(whole_path);
		pattern = malloc(dirlen+3);
		memcpy(pattern, whole_path, dirlen);
		pattern[dirlen] = '\\';
		pattern[dirlen+1] = '*';
		pattern[dirlen+2] = '\0';
		d = FindFirstFileA(pattern, &ent);
		free(pattern);
		if (d == INVALID_HANDLE_VALUE)
			goto err;
#else
		if (!(d = opendir(whole_path)))
			goto err;
#endif

		evbuffer_add_printf(evb,
                    "<!DOCTYPE html>\n"
                    "<html>\n <head>\n"
                    "  <meta charset='utf-8'>\n"
		    "  <title>%s</title>\n"
		    "  <base href='%s%s'>\n"
		    " </head>\n"
		    " <body>\n"
		    "  <h1>%s</h1>\n"
		    "  <ul>\n",
		    decoded_path, /* XXX html-escape this. */
		    path, /* XXX html-escape this? */
		    trailing_slash,
		    decoded_path /* XXX html-escape this */);
#ifdef _WIN32
		do {
			const char *name = ent.cFileName;
#else
		while ((ent = readdir(d))) {
			const char *name = ent->d_name;
#endif
			evbuffer_add_printf(evb,
			    "    <li><a href=\"%s\">%s</a>\n",
			    name, name);/* XXX escape this */
#ifdef _WIN32
		} while (FindNextFileA(d, &ent));
#else
		}
#endif
		evbuffer_add_printf(evb, "</ul></body></html>\n");
#ifdef _WIN32
		FindClose(d);
#else
		closedir(d);
#endif
		evhttp_add_header(evhttp_request_get_output_headers(req),
			"Content-Type", "text/html");
	} else {
		/* Otherwise it's a file; add it to the buffer to get
		 * sent via sendfile */
		const char *type = guess_content_type(decoded_path);
		if ((fd = open(whole_path, O_RDONLY)) < 0) {
			perror("open");
			goto err;
		}

		if (fstat(fd, &st)<0) {
			/* Make sure the length still matches, now that we
			 * opened the file :/ */
			perror("fstat");
			goto err;
		}
		evhttp_add_header(evhttp_request_get_output_headers(req),
			"Content-Type", type);
		evbuffer_add_file(evb, fd, 0, st.st_size);
	}

	evhttp_send_reply(req, 200, "OK", evb);
	goto done;
err:
	evhttp_send_error(req, 404, "Document was not found");
	if (fd>=0)
		close(fd);
done:
	if (decoded)
		evhttp_uri_free(decoded);
	if (decoded_path)
		free(decoded_path);
	if (whole_path)
		free(whole_path);
	if (evb)
		evbuffer_free(evb);
}
#endif
static void
do_term(int sig, short events, void *arg)
{
	struct event_base *base = (struct event_base *)arg;
	event_base_loopbreak(base);
	fprintf(stderr, "Got %i, Terminating\n", sig);
}

static int
display_listen_sock(struct evhttp_bound_socket *handle)
{
	struct sockaddr_storage ss;
	evutil_socket_t fd;
	ev_socklen_t socklen = sizeof(ss);
	char addrbuf[128];
	void *inaddr;
	const char *addr;
	int got_port = -1;

	fd = evhttp_bound_socket_get_fd(handle);
	memset(&ss, 0, sizeof(ss));
	if (getsockname(fd, (struct sockaddr *)&ss, &socklen)) {
		perror("getsockname() failed");
		return 1;
	}

	if (ss.ss_family == AF_INET) {
		got_port = ntohs(((struct sockaddr_in*)&ss)->sin_port);
		inaddr = &((struct sockaddr_in*)&ss)->sin_addr;
	} else if (ss.ss_family == AF_INET6) {
		got_port = ntohs(((struct sockaddr_in6*)&ss)->sin6_port);
		inaddr = &((struct sockaddr_in6*)&ss)->sin6_addr;
	}
#ifdef EVENT__HAVE_STRUCT_SOCKADDR_UN
	else if (ss.ss_family == AF_UNIX) {
		printf("Listening on <%s>\n", ((struct sockaddr_un*)&ss)->sun_path);
		return 0;
	}
#endif
	else {
		fprintf(stderr, "Weird address family %d\n",
			ss.ss_family);
		return 1;
	}

	addr = evutil_inet_ntop(ss.ss_family, inaddr, addrbuf,
		sizeof(addrbuf));
	if (addr) {
		printf("Listening on %s:%d\n", addr, got_port);
		evutil_snprintf(uri_root, sizeof(uri_root),
			"http://%s:%d",addr,got_port);
	} else {
		fprintf(stderr, "evutil_inet_ntop failed\n");
		return 1;
	}

	return 0;
}

PATTERN_SINGLETON_IMPLEMENT(CHttpServer)

CHttpServer::CHttpServer():CThread("HttpServer", TP_DEFAULT)
{
	cfg = NULL;
	base = NULL;
	http = NULL;
	handle = NULL;
	lev = NULL;
	term = NULL;
}

CHttpServer::~CHttpServer()
{
}

int CHttpServer::Init()
{
	printf("======================================CHttpServer::Init()=======================================\n");
	o.port = 80;
	o.unixsock = NULL;
	o.unlink = 1;
	o.iocp = 1;
	o.verbose = 1;
	
	return 0;
}

int CHttpServer::UnInit()
{
	printf("======================================CHttpServer::UnInit()======================================\n");
	return 0;
}
int CHttpServer::Start(OPTIONS *opt)
{
	if( false == IsThreadOver() ) 	//��ֹ��ε���
	{
		return 0;
	}
	printf("======================================CHttpServer::Start()=======================================\n");
	//o = opt;
	memcpy((void*)&o,(const void*)opt,sizeof(OPTIONS));
	// printf("o.port=%d\n",o.port);
	// printf("o.unlink=%d\n",o.unlink);
	// printf("o.iocp=%d\n",o.iocp);
	// printf("o.cmd_req_path=%s\n",o.cmd_req_path);
	// printf("o.burn_license_req_path=%s\n",o.burn_license_req_path);
	// printf("o.info_req_path=%s\n",o.info_req_path);
	// printf("o.pull_req_path=%s\n",o.pull_req_path);
	// printf("o.wifi_info_req_path=%s\n",o.wifi_info_req_path);
	// printf("o.push_file_name=%s\n",o.push_file_name);
	// printf("o.push_req_path=%s\n",o.push_req_path);
	// printf("o.get_req_path=%s\n",o.get_req_path);

	int ret;

	if (signal(SIGPIPE, SIG_IGN) == SIG_ERR) {
		ret = 1;
		goto err;
	}

	setbuf(stdout, NULL);
	setbuf(stderr, NULL);

	/** Read env like in regress */
	if (o.verbose || getenv("EVENT_DEBUG_LOGGING_ALL"))
		event_enable_debug_logging(EVENT_DBG_ALL);

	cfg = event_config_new();

	base = event_base_new_with_config(cfg);
	if (!base) {
		fprintf(stderr, "Couldn't create an event_base: exiting\n");
		ret = 1;
	}
	event_config_free(cfg);
	cfg = NULL;

	/* Create a new evhttp object to handle requests. */
	http = evhttp_new(base);
	if (!http) {
		fprintf(stderr, "couldn't create evhttp. Exiting.\n");
		ret = 1;
	}

	/* The /dump URI will dump all requests to stdout and say 200 ok. */
	evhttp_set_cb(http, "/dump", dump_request_cb,  &o);
	if (o.info)
	{
		evhttp_set_cb(http, o.info_req_path, info_request_cb,  &o);
	}
	if (o.check)
	{
		evhttp_set_cb(http, o.check_req_path, check_request_cb,  &o);
	}
	if (o.getlicense)
	{
		evhttp_set_cb(http, o.getlicense_req_path, getlicense_request_cb,  &o);
	}
	if (o.cmd)
	{
		evhttp_set_cb(http, o.cmd_req_path, cmd_request_cb,  &o);
	}
	if (o.wifi_info)
	{
		evhttp_set_cb(http, o.wifi_info_req_path, wifi_info_request_cb,  &o);
	}
	if (o.push_file)
	{
		evhttp_set_cb(http, o.push_req_path, push_file_request_cb,  &o);
	}
	if (o.pull_file)
	{
		evhttp_set_cb(http, o.pull_req_path, pull_file_request_cb,  &o);
	}
	if (o.burn_license)
	{
		evhttp_set_cb(http, o.burn_license_req_path, burn_license_request_cb,  &o);
	}
	if (o.get_req)
	{
		evhttp_set_cb(http, o.get_req_path, get_req_cb,  &o);
	}
	
	
	/* We want to accept arbitrary requests, so we need to set a "generic"
	* cb.  We can also add callbacks for specific paths. */
	//evhttp_set_gencb(http, send_document_cb, &o);
	evhttp_set_gencb(http, dump_request_cb, &o);

	if (o.unixsock) {
#ifdef EVENT__HAVE_STRUCT_SOCKADDR_UN
		struct sockaddr_un addr;

		if (o.unlink && (unlink(o.unixsock) && errno != ENOENT)) {
			perror(o.unixsock);
			ret = 1;
			goto err;
		}

		addr.sun_family = AF_UNIX;
		strcpy(addr.sun_path, o.unixsock);

		lev = evconnlistener_new_bind(base, NULL, NULL,
			LEV_OPT_CLOSE_ON_FREE, -1,
			(struct sockaddr *)&addr, sizeof(addr));
		if (!lev) {
			perror("Cannot create listener");
			ret = 1;
			goto err;
		}

		handle = evhttp_bind_listener(http, lev);
		if (!handle) {
			fprintf(stderr, "couldn't bind to %s. Exiting.\n", o.unixsock);
			ret = 1;
			goto err;
		}
#else /* !EVENT__HAVE_STRUCT_SOCKADDR_UN */
		fprintf(stderr, "-U is not supported on this platform. Exiting.\n");
		ret = 1;
		goto err;
#endif /* EVENT__HAVE_STRUCT_SOCKADDR_UN */
	} else {
		handle = evhttp_bind_socket_with_handle(http, "0.0.0.0", o.port);
		if (!handle) {
			fprintf(stderr, "couldn't bind to port %d. Exiting.\n", o.port);
			ret = 1;
			goto err;
		}
	}

	if (display_listen_sock(handle)) {
		ret = 1;
		goto err;
	}

	term = evsignal_new(base, SIGINT, do_term, base);
	if (!term)
		goto err;
	if (event_add(term, NULL))
		goto err;
	
	evhttp_set_timeout(http,5);

	return CreateThread();

err:
	if (cfg)
		event_config_free(cfg);
	if (http)
		evhttp_free(http);
	if (term)
		event_free(term);
	if (base)
		event_base_free(base);

	return false;
}

int CHttpServer::Stop()
{
	printf("======================================CHttpServer::Stop()=======================================\n");
	if( true == IsThreadOver() )
	{
		return 0;
	}
	DestroyThread(true);

	if (base)
	{
		//event_base_loopexit(base,NULL);
		event_base_loopbreak(base);

		if (cfg)
			event_config_free(cfg);
		if (http)
			evhttp_free(http);
		if (term)
			event_free(term);
		if (base)
			event_base_free(base);

		cfg = NULL;
		base = NULL;
		http = NULL;
		term = NULL;
	}
	
	return 0;
}

void CHttpServer::ThreadProc(void)
{
	while (m_bLoop)
	{
		sleep(1);
		if (base)
		{
			event_base_dispatch(base);
		}
	}
}


