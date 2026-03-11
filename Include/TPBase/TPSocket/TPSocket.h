#ifndef	__TP_SOCKET_H
#define	__TP_SOCKET_H

#ifdef  __cplusplus
extern "C" {
#endif

#include	"TPDef.h"

#define	SA	struct sockaddr

#ifdef WIN32
static signed char __initialized = 0;
#endif

#ifdef WIN32
#define	bzero(ptr,n)		memset(ptr, 0, n)
#endif

socket_t	tp_tcp_server(const port_t, const int);
socket_t	tp_tcp_connect(const char *, const port_t);
socket_t	tp_tcp_client(char *, char *);

socket_t	tp_udp_server(const port_t);
socket_t	tp_udp_connect(const char *, const port_t);
socket_t	tp_udp_client(const char *, const port_t, SA *, socklen_t *);

int			mcast_set_loop(socket_t, unsigned char);

socket_t	tp_socket(int, int, int);
int			tp_bind(socket_t, const SA *, socklen_t);
int			tp_listen(socket_t, int);
socket_t	tp_accept(socket_t, SA *, socklen_t *);
int			tp_connect(socket_t, const SA *, socklen_t);

int			tp_socket_init(void);
int			tp_socket_cleanup(void);
int			tp_close(socket_t);
int			tp_close_socket(socket_t *);

int			tp_gethostbyname(const char *, struct in_addr *);
int			tp_gethostbyname2(const char *, struct in_addr *);
int			tp_getpeername(socket_t, SA *, socklen_t *);
int			tp_getsockname(socket_t, SA *, socklen_t *);
int			tp_gethostname(char *, size_t);
int			tp_sethostname(char *, size_t);

int			tp_getsockopt(socket_t, int, int, void *, socklen_t *);
int			tp_setsockopt(socket_t, int, int, const void *, socklen_t);
int			tp_shutdown(socket_t, int);
int			tp_select(int, fd_set *, fd_set *, fd_set *, struct timeval *);

int			tp_set_nodelay(socket_t);
int			tp_set_nonblock(int, socket_t);

int			tp_writen(socket_t, const void *, size_t);
int			tp_writeton( socket_t, const void *, size_t, const SA *, int);

int			tp_socketpair(int family, int type, int protocol, int fd[2]);

#ifdef WIN32
int			inet_aton(const char *, struct in_addr *);
int			inet_pton(int, const char *, void *);
#endif
int			set_address(char *, char *, struct sockaddr_in *, char *);

#define ez_err_ret		printf
#define ez_err_sys		printf
#define	ez_err_dump		printf
#define	ez_err_msg		printf
#define	ez_err_quit		printf

#ifdef  __cplusplus
}
#endif

#endif	/* __TP_SOCKET_H */


