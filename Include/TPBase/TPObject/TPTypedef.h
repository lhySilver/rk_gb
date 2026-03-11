#ifndef _TPETYPEDEF_H_
#define _TPETYPEDEF_H_

#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>

#ifdef WIN32
//#include <WINSOCK2.H>
#ifdef __cplusplus
extern "C" {
#endif
#include "../TPSocket/TPDef.h"
#ifdef __cplusplus
}
#endif
#else
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netinet/tcp.h>		/* TCP_NODELAY */
#include <pthread.h>
#include	<arpa/inet.h>	/* inet(3) functions */
#include <unistd.h>
#define closesocket(S) close(S)
#endif

#include	<errno.h>
#include	<fcntl.h>		/* for nonblocking */


#define REVBUF_LEN  1024
#define KEEP_ALIVE_TIME     180  //3 Min

#ifndef WIN32
#define INVALID_SOCKET -1
#endif

#define MAX_ACCEPT_DLGS  20

#define TP_ERROR_BASE -1
#define TP_NET_ERROR -2
#define TP_NORMAL_RET (TP_ERROR_BASE + 1)
#define TP_ERROR_UNSUPPORT  (TP_ERROR_BASE - 100)


#define TP_SELF	-11;

typedef struct _client_list
{
    unsigned int ip;
    unsigned short port;
    int socket;
    unsigned int id;
    int online;//0:using 1:unusing
    unsigned int timemark;
}CLIENT_LIST;

/*
 * 缓冲区行数据.send实际把数据保存在一个DataRow中然后放在队列中
 * data保存的是发送方的指针,数据在onSendDataAck中删除.
 */
class DataRow
{
public:
	const char* data;
	int len;

	int id;
	int socket;
	int sequence;
};

typedef enum
{
	TP_SEND = 1,
	TP_RECEIVE
} TPType;

#endif
