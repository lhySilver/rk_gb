#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <syslog.h>
#include <stdarg.h>

#include <errno.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <ctype.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/vfs.h>
#include <sys/wait.h>
#include <sys/ioctl.h>
#include <sys/mount.h>

#include <linux/netlink.h>
#include <linux/version.h>
#include <linux/input.h>
#include <linux/types.h>

#include <mtd/mtd-user.h>

/*
* @daemon_init, make current process to be a daemon process
* @return: 0 on success, -1 on faield
*/
static int daemon_init(void)
{
	int a, max_fd, i;

	/** 1 **/
	signal(SIGHUP, SIG_IGN);
	signal(SIGCHLD, SIG_IGN);

	/*** 2 ***/
	a = fork();
	if (a > 0)
		exit(EXIT_SUCCESS);
	/*** 3 ***/
	setsid();
	/*** 4 ***/
	a = fork();
	if (a > 0)
		exit(EXIT_SUCCESS);
	/*** 5 ***/
	setpgrp();
	/*** 6 ***/
	max_fd = sysconf(_SC_OPEN_MAX);
	for (i = 3; i < max_fd; i++)
		close(i);
	/*** 7 ***/
	umask(0);
	/*** 8 ***/
	chdir("/");

	return 0;
}

#if 0
static void process_signal(unsigned int sig, siginfo_t *si, void *ptr)
{
	if(sig == SIGINT || sig == SIGSEGV) {

	}

	if(sig == SIGTERM) {
	}
	exit(1);
}

static int register_signal(void)
{
	struct sigaction s;

	s.sa_flags = SA_SIGINFO;
	s.sa_sigaction = (void *)process_signal;

	sigaction(SIGSEGV, &s, NULL);
	sigaction(SIGINT, &s, NULL);
	sigaction(SIGTERM, &s, NULL);

	signal(SIGALRM, daemon_alarm_print);	//for timer
	signal(SIGUSR1, daemon_sig_fun);		//now no use
	signal(SIGPIPE, SIG_IGN);

	return 0;
}
#endif


/* main program has became a daemon program before daemon_init operate */
int main(int argc, char **argv)
{
	daemon_init();
	//register_signal();
    
    while(1)
    {
        if(access("/tmp/ota_upgrade_flag", F_OK) == 0)
        {
            printf("daemon: get ota_upgrade_flag...\n");
            system("rm /tmp/ota_upgrade_flag -rf");

			printf("daemon: killall dgiot...\n");
			system("killall dgiot");

			printf("daemon: killall wpa_supplicant...\n");
            system("killall wpa_supplicant");

			printf("daemon: killall udhcpc...\n");
            system("killall udhcpc");

			printf("daemon:switchroot...\n");
			system("switchroot.sh /tmp/upgrade.bin");

			printf("daemon:burntool...\n");
			system("burntool");

			printf("daemon:reboot...\n");
			system("reboot -f");

            return 0;
        }
        
        sleep(2);
    }
	return 0;
}
