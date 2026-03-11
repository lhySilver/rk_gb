#ifndef __TP_WIN_DEF_H
#define __TP_WIN_DEF_H

#define AS_HAS_WINSOCK2 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <WinError.h>

#include "Types/Defs.h"

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "mswsock.lib")
#pragma comment(lib, "wsock32.lib")

typedef int socket_t;
typedef int socklen_t;
typedef unsigned short port_t;
typedef void *	LPVOID;
typedef char* caddr_t;

/// Protocols
#define IPPROTO_IP              0               /// dummy for IP
#define IPPROTO_ICMP            1               /// control message protocol
#define IPPROTO_TCP             6               /// tcp
#define IPPROTO_UDP             17              /// user datagram protocol

#define INADDR_ANY              (unsigned long)0x00000000
#define INADDR_LOOPBACK         0x7f000001
#define INADDR_BROADCAST        (unsigned long)0xffffffff
#define INADDR_NONE             0xffffffff

#define WSADESCRIPTION_LEN      256
#define WSASYS_STATUS_LEN       128

#define INVALID_SOCKET  (socket_t)(~0)
#define SOCKET_ERROR            (-1)

/// Types
#define SOCK_STREAM     1               /// stream socket
#define SOCK_DGRAM      2               /// datagram socket
#define SOCK_RAW        3               /// raw-protocol interface
#define SOCK_RDM        4               /// reliably-delivered message
#define SOCK_SEQPACKET  5               /// sequenced packet stream

/// Option Flags
#define SO_DEBUG        0x0001          /// turn on debugging info recording
#define SO_ACCEPTCONN   0x0002          /// socket has had listen()
#define SO_REUSEADDR    0x0004          /// allow local address reuse
#define SO_KEEPALIVE    0x0008          /// keep connections alive
#define SO_DONTROUTE    0x0010          /// just use interface addresses
#define SO_BROADCAST    0x0020          /// permit sending of broadcast msgs
#define SO_USELOOPBACK  0x0040          /// bypass hardware when possible
#define SO_LINGER       0x0080          /// linger on close if data present
#define SO_OOBINLINE    0x0100          /// leave received OOB data in line

#define SO_SNDBUF       0x1001          /// send buffer size
#define SO_RCVBUF       0x1002          /// receive buffer size
#define SO_SNDLOWAT     0x1003          /// send low-water mark
#define SO_RCVLOWAT     0x1004          /// receive low-water mark
#define SO_SNDTIMEO     0x1005          /// send timeout
#define SO_RCVTIMEO     0x1006          /// receive timeout
#define SO_ERROR        0x1007          /// get error status and clear
#define SO_TYPE         0x1008          /// get socket type

#define TCP_NODELAY     0x0001

/// EtherNet families
#define AF_UNSPEC       0               /// unspecified
#define AF_UNIX         1               /// local to host (pipes, portals)
#define AF_INET         2               /// internetwork: UDP, TCP, etc.
#define AF_IMPLINK      3               /// arpanet imp addresses
#define AF_PUP          4               /// pup protocols: e.g. BSP
#define AF_CHAOS        5               /// mit CHAOS protocols
#define AF_NS           6               /// XEROX NS protocols
#define AF_IPX          AF_NS           /// IPX protocols: IPX, SPX, etc.
#define AF_ISO          7               /// ISO protocols
#define AF_OSI          AF_ISO          /// OSI is ISO
#define AF_ECMA         8               /// european computer manufacturers
#define AF_DATAKIT      9               /// datakit protocols
#define AF_CCITT        10              /// CCITT protocols, X.25 etc
#define AF_SNA          11              /// IBM SNA
#define AF_DECnet       12              /// DECnet
#define AF_DLI          13              /// Direct data link interface
#define AF_LAT          14              /// LAT
#define AF_HYLINK       15              /// NSC Hyperchannel
#define AF_APPLETALK    16              /// AppleTalk
#define AF_NETBIOS      17              /// NetBios-style addresses
#define AF_VOICEVIEW    18              /// VoiceView
#define AF_FIREFOX      19              /// Protocols from Firefox
#define AF_UNKNOWN1     20              /// Somebody is using this!
#define AF_BAN          21              /// Banyan
#define AF_ATM          22              /// Native ATM Services
#define AF_INET6        23              /// Internetwork Version 6
#define AF_CLUSTER      24              /// Microsoft Wolfpack
#define AF_12844        25              /// IEEE 1284.4 WG AF
#define AF_IRDA         26              /// IrDA
#define AF_NETDES       28              /// Network Designers OSI & gateway enabled protocols
#define AF_TCNPROCESS   29
#define AF_TCNMESSAGE   30
#define AF_ICLFXBM      31
#define AF_MAX          32

/// Level number for (get/set)sockopt() to apply to socket itself.
#define SOL_SOCKET      0xffff          /// options for socket level

/// Maximum queue length specifiable by listen.
#define SOMAXCONN       0x7fffffff

#define MSG_OOB         0x1             /// process out-of-band data
#define MSG_PEEK        0x2             /// peek at incoming message
#define MSG_DONTROUTE   0x4             /// send without using routing tables
#define MSG_PARTIAL     0x8000          /// partial send or recv for message xport

/// options for fcntl
#define O_NDELAY 				0x04 
#define FNDELAY					O_NDELAY
#define F_GETFL					3
#define F_SETFL					4

#define IOCPARM_MASK    0x7f           
#define IOC_VOID        0x20000000			/// no parameters
#define IOC_OUT         0x40000000			/// copy out parameters
#define IOC_IN          0x80000000			/// copy in parameters 
#define IOC_INOUT       (IOC_IN|IOC_OUT)	/// 0x20000000 distinguishes new & old ioctl's

#define _IO(x,y)        (IOC_VOID|((x)<<8)|(y))
#define _IOR(x,y,t)     (IOC_OUT|(((long)sizeof(t)&IOCPARM_MASK)<<16)|((x)<<8)|(y))
#define _IOW(x,y,t)     (IOC_IN|(((long)sizeof(t)&IOCPARM_MASK)<<16)|((x)<<8)|(y))

/// options for ioctl
#define SIOCATMARK				7           /// check for out of bound data
#define FIONREAD    _IOR('f', 127, ulong)	/// get # bytes to read
#define FIONBIO     _IOW('f', 126, ulong)	/// set/clear non-blocking i/o


#define EAFNOSUPPORT            WSAEAFNOSUPPORT
#define ECONNRESET				WSAECONNRESET

#ifndef	EINTR
#define	EINTR 4
#endif

#ifndef ENOTCONN
#define ENOTCONN	WSAENOTCONN
#endif

#ifndef ETIME
#define ETIME	62
#endif

#define snprintf _snprintf
#define tp_set_errno(x)		WSASetLastError((x))

static int	__socket_initialized	= 0;

/// generic socket address
struct sockaddr
{
    ushort	sa_family;		/// address family
    char	sa_data[14];	/// up to 14 bytes of direct address
};

/// Internet address
struct in_addr
{
	ulong	s_addr;
};

/// Internet socket address
struct sockaddr_in
{
    short			sin_family;
    ushort			sin_port;
    struct in_addr	sin_addr;
    char			sin_zero[8];
};

/// address and length
struct iovec 
{
    char	*iov_base;   /// address
    int		iov_len;    /// len
};

/// Message header for recvmsg and sendmsg.
struct msghdr
{
    char			*msg_name;		/// optional address
    int				msg_namelen;	/// size of address
    struct iovec	*msg_iov;		/// scatter/gather array
    int				msg_iovlen;		/// # elements in msg_iov
    char			*msg_accrights;	/// access rights sent/received
    int				msg_accrightslen;
};

struct  hostent
{
	char	*h_name;
	char	**h_aliases;
	short	h_addrtype;
	short	h_length;
	char	**h_addr_list;
#define h_addr  h_addr_list[0]
};

struct  netent 
{
	char	*n_name;
	char	**n_aliases;
	short	n_addrtype;
	ulong	n_net;
};

struct  servent
{
	char	*s_name; 
	char	**s_aliases;
#ifdef _WIN64
	char	*s_proto;
	short	s_port;
#else
	short	s_port;
	char	*s_proto;
#endif
};

struct  protoent {
	char	*p_name;
	char	**p_aliases;
	short	p_proto;
};

#ifndef FD_SETSIZE
#define FD_SETSIZE	64
#endif

typedef struct fd_set
{
	uint		fd_count;
	socket_t	fd_array[FD_SETSIZE];
} fd_set;

#undef far
#undef near
#undef pascal

#define far
#define near
#define PASCAL      __stdcall

#ifndef FAR
#define FAR                 far
#endif

extern int PASCAL FAR __WSAFDIsSet(socket_t, fd_set FAR *);

#define FD_CLR(fd, set) do { \
	uint __i; \
	for (__i = 0; __i < ((fd_set FAR *)(set))->fd_count ; __i++) { \
	if (((fd_set FAR *)(set))->fd_array[__i] == fd) { \
	while (__i < ((fd_set FAR *)(set))->fd_count-1) { \
	((fd_set FAR *)(set))->fd_array[__i] = \
	((fd_set FAR *)(set))->fd_array[__i+1]; \
	__i++; \
	} \
	((fd_set FAR *)(set))->fd_count--; \
	break; \
	} \
	} \
} while(0)

#define FD_SET(fd, set) do { \
	uint __i; \
	for (__i = 0; __i < ((fd_set FAR *)(set))->fd_count; __i++) { \
	if (((fd_set FAR *)(set))->fd_array[__i] == (fd)) { \
	break; \
	} \
	} \
	if (__i == ((fd_set FAR *)(set))->fd_count) { \
	if (((fd_set FAR *)(set))->fd_count < FD_SETSIZE) { \
	((fd_set FAR *)(set))->fd_array[__i] = (fd); \
	((fd_set FAR *)(set))->fd_count++; \
	} \
	} \
} while(0)

#define FD_ZERO(set) (((fd_set FAR *)(set))->fd_count=0)

#define FD_ISSET(fd, set) __WSAFDIsSet((socket_t)(fd), (fd_set FAR *)(set))

struct timeval
{
	ulong	tv_sec;
	ulong	tv_usec;
};

/// Multicast
struct ip_mreq
{
	struct in_addr imr_multiaddr;	/* IP class D multicast address */
	struct in_addr imr_interface;	/* IP address of local interface */
};

#define IP_DEFAULT_MULTICAST_TTL   1    /* normally limit m'casts to 1 hop  */
#define IP_DEFAULT_MULTICAST_LOOP  1    /* normally hear sends if a member  */
#define IP_MAX_MEMBERSHIPS         20   /* per socket; must fit in one mbuf */

#define EWOULDBLOCK WSAEWOULDBLOCK


#define WSADESCRIPTION_LEN      256
#define WSASYS_STATUS_LEN       128

typedef struct WSAData
{
	ushort		wVersion;
	ushort		wHighVersion;
#ifdef _WIN64
	ushort		iMaxSockets;
	ushort		iMaxUdpDg;
	char		*lpVendorInfo;
	char		szDescription[WSADESCRIPTION_LEN+1];
	char		szSystemStatus[WSASYS_STATUS_LEN+1];
#else
	char		szDescription[WSADESCRIPTION_LEN+1];
	char		szSystemStatus[WSASYS_STATUS_LEN+1];
	ushort		iMaxSockets;
	ushort		iMaxUdpDg;
	char		*lpVendorInfo;
#endif
} WSADATA, *LPWSADATA;

//typedef WSADATA *LPWSADATA;

int __stdcall WSAStartup(ushort wVersionRequired, LPWSADATA lpWSAData);
int __stdcall WSACleanup(void);
void __stdcall WSASetLastError(int iError);
int __stdcall WSAGetLastError(void);
Bool __stdcall WSAIsBlocking(void);
int __stdcall WSAUnhookBlockingHook(void);

typedef int (* FARPROC)();
FARPROC WSASetBlockingHook( FARPROC lpBlockFunc);

int __stdcall WSACancelBlockingCall(void);
int __stdcall WSACancelAsyncRequest(Handle hAsyncTaskHandle);
int __stdcall WSARecvEx (socket_t s, char * buf, int len, int *flags);

int __stdcall socket(int, int, int);
int __stdcall shutdown(socket_t, int);
int __stdcall closesocket(int);
int __stdcall select(int, fd_set *, fd_set *, fd_set *, struct timeval *);
int __stdcall accept(socket_t, struct sockaddr * , int *);
int __stdcall bind(socket_t, const struct sockaddr *, int);
int __stdcall connect(socket_t, const struct sockaddr *, int);
int __stdcall getsockname(socket_t, struct sockaddr *, int *);
int __stdcall getpeername(socket_t, struct sockaddr *, int *);
int	__stdcall gethostname(char *, size_t);
int __stdcall getsockopt(socket_t, int, int, char *, int *);
int __stdcall setsockopt(socket_t, int, int, const char *, int);
int __stdcall listen(socket_t, int);
int __stdcall recv(socket_t, char *, int, int);
int __stdcall recvfrom(socket_t, char *, int, int, struct sockaddr *, int *);
int __stdcall recvmsg(socket_t, struct msghdr *, int);
int __stdcall send(socket_t, const char *, int, int);
int __stdcall sendto(socket_t, char *, int, int, struct sockaddr *, int);
int __stdcall sendmsg(socket_t, struct msghdr *, int);
struct hostent * __stdcall gethostbyname_r(char *, struct hostent *, char *, int, int *);
struct hostent * __stdcall gethostbyname(const char *);
struct hostent *__stdcall gethostbyaddr_r(char *, int, int, struct hostent *, char *, int, int *);
struct hostent *__stdcall gethostbyaddr(char *, int, int);
struct servent * __stdcall getservbyport(int, const char *);
struct servent * __stdcall getservbyname(const char *, const char *);
struct protoent * __stdcall getprotobynumber(int);
struct protoent * __stdcall getprotobyname(const char *);

int __stdcall fcntlsocket(int, int, int);
int __stdcall ioctlsocket (socket_t, long, ulong *);
void __stdcall sleepsocket(int);
unsigned long  __stdcall  inet_addr(const char *);
char * __stdcall __stdcall inet_ntoa(struct in_addr);
int __stdcall closesocket(socket_t);

unsigned long __stdcall ntohl (unsigned long);
unsigned short __stdcall ntohs (unsigned short);
unsigned long __stdcall htonl (unsigned long);
unsigned short __stdcall htons (unsigned short);

#endif /* __TP_WIN_DEF_H */

