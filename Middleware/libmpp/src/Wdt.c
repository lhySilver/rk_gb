
#include "PAL/MW_Common.h"
#include "PAL/WatchDog.h"

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <pthread.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/time.h>
#include <unistd.h>
#include <time.h>
#include <getopt.h>
#include <sys/signal.h>

//watchdog 
#define WATCHDOG_IOCTL_BASE     'W'

struct watchdog_info 
{
    unsigned int options;          /* Options the card/driver supports */
    unsigned int firmware_version; /* Firmware version of the card */
    char identity[32];     /* Identity of the board */
};

#define WDIOC_GETSUPPORT        _IOR(WATCHDOG_IOCTL_BASE, 0, struct watchdog_info)
#define WDIOC_GETSTATUS         _IOR(WATCHDOG_IOCTL_BASE, 1, int)
#define WDIOC_GETBOOTSTATUS     _IOR(WATCHDOG_IOCTL_BASE, 2, int)
#define WDIOC_GETTEMP           _IOR(WATCHDOG_IOCTL_BASE, 3, int)
#define WDIOC_SETOPTIONS        _IOR(WATCHDOG_IOCTL_BASE, 4, int)
#define WDIOC_KEEPALIVE         _IOR(WATCHDOG_IOCTL_BASE, 5, int)
#define WDIOC_SETTIMEOUT        _IOWR(WATCHDOG_IOCTL_BASE, 6, int)
#define WDIOC_GETTIMEOUT        _IOR(WATCHDOG_IOCTL_BASE, 7, int)
#define WDIOC_SETPRETIMEOUT     _IOWR(WATCHDOG_IOCTL_BASE, 8, int)
#define WDIOC_GETPRETIMEOUT     _IOR(WATCHDOG_IOCTL_BASE, 9, int)
#define WDIOC_GETTIMELEFT       _IOR(WATCHDOG_IOCTL_BASE, 10, int)

#define WDIOF_OVERHEAT          0x0001  /* Reset due to CPU overheat */
#define WDIOF_FANFAULT          0x0002  /* Fan failed */
#define WDIOF_EXTERN1           0x0004  /* External relay 1 */
#define WDIOF_EXTERN2           0x0008  /* External relay 2 */
#define WDIOF_POWERUNDER        0x0010  /* Power bad/power fault */
#define WDIOF_CARDRESET         0x0020  /* Card previously reset the CPU */
#define WDIOF_POWEROVER         0x0040  /* Power over voltage */
#define WDIOF_SETTIMEOUT        0x0080  /* Set timeout (in seconds) */
#define WDIOF_MAGICCLOSE        0x0100  /* Supports magic close char */
#define WDIOF_PRETIMEOUT        0x0200  /* Pretimeout (in seconds), get/set */
#define WDIOF_KEEPALIVEPING     0x8000  /* Keep alive ping reply */

#define WDIOS_DISABLECARD       0x0001  /* Turn off the watchdog timer */
#define WDIOS_ENABLECARD        0x0002  /* Turn on the watchdog timer */
#define WDIOS_TEMPPANIC         0x0004  /* Kernel panic on temperature trip */

int wdt_fd;
int time_out = 10;
//#define DEFAULT_PING_RATE    1


////////////////////////////////////////////////////////////////////////////
////////////ø¥√≈π∑///////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////


int WatchdogCreate(int timeout)
{
	int ret;
    //static int count = 0;
    struct watchdog_info wdt_info;
    //unsigned int ping_rate = DEFAULT_PING_RATE;

    wdt_fd = open("/dev/watchdog", O_RDWR);
    if(wdt_fd < 0)
    {
        EMSG("open /dev/watchdog failed.\n");
		return -1;
    }

    /* get watchdog infomation struct */
    ret = ioctl(wdt_fd, WDIOC_GETSUPPORT, &wdt_info);
    if (ret < 0)
	{
        EMSG("ioctl WDIOC_GETSUPPORT failed.\n");
	}
    else
    {
        MSG("ioctl WDIOC_GETSUPPORT options = 0x%x,id = %s\n", wdt_info.options, wdt_info.identity);
    }

    ret = ioctl(wdt_fd, WDIOC_SETTIMEOUT, &time_out);
    if (ret < 0)
	{
        EMSG("ioctl WDIOC_SETTIMEOUT failed.\n");
	}

	return 0;
}

int WatchdogDestory(void)
{	
	int val = 0 , ret = 0;

    val = WDIOS_DISABLECARD;
    ret = ioctl(wdt_fd, WDIOC_SETOPTIONS, &val);
    if (ret < 0)
	{
        EMSG("ioctl WDIOC_GETSUPPORT failed with %d.\n", ret);
	}
	
    MSG("watchdog will be closed\n");
    close(wdt_fd);
    MSG("watchdog closed\n");

	return 0;
}

int WatchdogFeed(void)
{
	int ret = ioctl(wdt_fd,WDIOC_KEEPALIVE,NULL);
	if (ret < 0)
	{
		EMSG("ioctl WDIOC_KEEPALIVE failed.\n");
	}

	return 0;
}