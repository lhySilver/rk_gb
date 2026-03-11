#ifndef __TP_LINUX_DEF_H
#define __TP_LINUX_DEF_H

#include	<sys/types.h>
#include	<sys/socket.h>
#include	<sys/time.h>
#include	<time.h>
#include	<netinet/in.h>
#include	<arpa/inet.h>
#include	<errno.h>
#include	<fcntl.h>
#include	<netdb.h>
#include	<signal.h>
#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include	<sys/stat.h>
#include	<sys/uio.h>
#include	<unistd.h>
#include	<sys/wait.h>
#include	<sys/un.h>

#include	<stdio.h>
#include	<string.h>
#include	<errno.h>
#include	<unistd.h>
#include	<sys/socket.h>
#include	<sys/types.h>
#include	<linux/if_ether.h>
#include	<net/if.h>
#include	<sys/ioctl.h>
#include	<unistd.h>
#include	<netinet/tcp.h>
#include	<arpa/nameser.h>
#include	<resolv.h>

#define MAXLINE 1024
#define BUFFSIZE 1024

typedef int socket_t;
typedef ushort port_t;

#define INVALID_SOCKET -1
#define SOCKET_ERROR -1

#define tp_set_errno(x) errno = (x)


#endif /* __TP_LINUX_DEF_H */
