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


#define START_PROCESS( file, cmd, ... ) \
{\
	int status;\
	pid_t pid = vfork( );\
	printf("DAEMON_START_PROCESS------------------->>>>>pid = %d, errno = %d\n", pid, errno);\
	if ( pid  == 0 )\
	{\
		execlp ( file, cmd, ##__VA_ARGS__ );\
		exit ( -1 );\
	}\
	waitpid ( pid, &status, 0 );\
}


#define min(x, y)		(x <= y) ? x : y


#define MTD_START					0x00000000
#define MTD_SIZE					0x2000000    //32M FLASH
#define MTD_SMALL_SECT_SIZE 		0x10000 // 一个扇区大小

#define MTD_PROC	"/proc/mtd"

#define MTD_NAME_MAX_SIZE 	(32)
#define MTD_DEV_MAX_SIZE 	(10)

typedef struct{
	int mtd_index;
	int mtd_size;
	int mtd_erasesize;
	char mtd_name[MTD_NAME_MAX_SIZE];
}mtd_dev_s;

int mtd_dev_count_g = 0;
mtd_dev_s mtd_dev_g[MTD_DEV_MAX_SIZE];


#define DG_UPDATE_PACKAGE_PATH 				"/tmp/upgrade.bin" 
#define DG_UPDATE_PACKAGE_HW_PLATFORM 		"rv1106"
#define DG_UPDATE_PACKAGE_MAGIC				(0xABCD1234)

typedef enum{
	DG_UPGRADE_IMAGE_TYPE_ENV 		    = 1,
	DG_UPGRADE_IMAGE_TYPE_IDBLOCK 		= 2,
	DG_UPGRADE_IMAGE_TYPE_UBOOT 		= 3,
	DG_UPGRADE_IMAGE_TYPE_KERNEL 		= 4,
	DG_UPGRADE_IMAGE_TYPE_ROOTFS 		= 5,
	DG_UPGRADE_IMAGE_TYPE_OEM		    = 6,
	DG_UPGRADE_IMAGE_TYPE_MAX,
}DG_UPGRADE_IMAGE_TYPE_E;


typedef struct{
	char hw_platform[20]; 		//Ingenic
	unsigned int magic;
	unsigned int package_crc;
	unsigned int package_size;
}DG_UPDATE_PACKAGE_HEAD_S;


typedef struct{
	unsigned int image_type;
	unsigned int image_size;
	unsigned int start_addr;
}DG_UPDATE_IMAGE_HEAD_S;



static int my_read(int fd, char *buffer, int size)
{
	int ret;
	int read_count = 0;
	
	while(read_count < size)
	{
		ret = read(fd, buffer+read_count, size-read_count);
		if( ret <= 0 )
			break;
		read_count += ret;
	}

	return read_count;
}

static int my_write(int fd, char *buffer, int size)
{
	int ret;
	int write_count = 0;
	
	while(write_count < size)
	{
		ret = write(fd, buffer+write_count, size-write_count);
		if( ret <= 0 )
			break;
		write_count += ret;
	}

	return write_count;
}


static int ImageTypeConvertToMtdIndex(unsigned int image_type)
{
	/*
	dev:    size   erasesize  name
	mtd0: 00050000 00010000 "uboot"
	mtd1: 00400000 00010000 "boot"
	mtd2: 00af0000 00010000 "rootfs"
	mtd3: 01000000 00010000 "oem"
	mtd4: 00070000 00010000 "userdata"
	*/
	switch(image_type)
	{
		case DG_UPGRADE_IMAGE_TYPE_ENV:
			return 0;
		case DG_UPGRADE_IMAGE_TYPE_IDBLOCK:
			return 1;
		case DG_UPGRADE_IMAGE_TYPE_UBOOT:
			return 2;
		case DG_UPGRADE_IMAGE_TYPE_KERNEL:
			return 3;
		case DG_UPGRADE_IMAGE_TYPE_ROOTFS:
			return 4;
		case DG_UPGRADE_IMAGE_TYPE_OEM:
			return 5;
		default:
			return -1;
	}
	return -1;
}

static int InitMtdDevInfo()
{
	int i;
	FILE *fp = NULL;
	int ret;
	int result = -1;
	int mtd_index, mtd_size, mtd_erasesize;
	char mtd_name[128];
	char buffer[128];
	
	//获取mtd信息
	fp = fopen(MTD_PROC, "r");
	if( NULL == fp )
	{
		printf("[Error] open /proc/mtd faied.\n");
		goto exit;
	}

	if( ! fgets(buffer, sizeof(buffer), fp) )
	{
		printf("[Error] get /proc/mtd error.\n");
		goto exit;
	}
	if(strstr(buffer, "dev:") == NULL)
	{
		printf("[Error] get /proc/mtd error.\n");
		goto exit;
	}

	while(fgets(buffer, sizeof(buffer), fp))
	{
		printf("buffer: %s\n", buffer);
		memset(mtd_name, 0, sizeof(mtd_name));
		ret = sscanf(buffer, "mtd%d: %x %x \"%s\"", &mtd_index, &mtd_size, &mtd_erasesize, mtd_name);
		if( 4 != ret )
			continue;
		mtd_dev_g[mtd_dev_count_g].mtd_index = mtd_index;
		mtd_dev_g[mtd_dev_count_g].mtd_size = mtd_size;
		mtd_dev_g[mtd_dev_count_g].mtd_erasesize = mtd_erasesize;
		snprintf(mtd_dev_g[mtd_dev_count_g].mtd_name, MTD_NAME_MAX_SIZE, "%s", mtd_name);
		mtd_dev_count_g++;
	}

	if( 0 == mtd_dev_count_g )
	{
		printf("[Error] get mtd dev info failed.\n");
		goto exit;
	}

	for( i = 0; i < mtd_dev_count_g; i++ )
		printf("mtd%d -->\n\tsize: %d\n\terase size: %d\n\tname: %s\n", mtd_dev_g[i].mtd_index, mtd_dev_g[i].mtd_size, mtd_dev_g[i].mtd_erasesize, mtd_dev_g[i].mtd_name);

	result = 0;
	
exit:
	if( fp )
		fclose(fp);
	return result;

}

static const mtd_dev_s *GetMtdDevInfo(int mtd_index)
{
	int i;
	for(i = 0; i < MTD_DEV_MAX_SIZE; i++)
	{
		if( mtd_index == mtd_dev_g[i].mtd_index )
			return &mtd_dev_g[i];
	}
	return NULL;
}

static int FlashOpen(int mtd_index)
{
	int fd;
	char path[128];
	
	snprintf(path, sizeof(path), "/dev/mtd%d", mtd_index);
	printf("path:%s\n", path);
	fd = open(path, O_RDWR);
	if (fd  < 0)
	{
		printf("Open %s error\n", path);
		return -1;
	}

	return fd;
}

static void FlashClose(int fd)
{
	close(fd);
}


/************************************************************************
 * Copy memory to flash.
 * Note!! only write one sector 
 ************************************************************************/
int FlashWrite(int fd, const mtd_dev_s *mtd_dev, char *buf, int offset, int cnt)
{
	int ret;
	int sec_count = 0;
	int i = 0;
	struct erase_info_user erase_info;

	//printf("addr=0x%x,offset=%d\n",addr,offset);
	if( (offset % mtd_dev->mtd_erasesize) != 0)
	{
		printf("offset 0x%llx  Doesn't Alias 0x%lx\n",offset, mtd_dev->mtd_erasesize);
		return -1;
	}

	sec_count = (cnt + (mtd_dev->mtd_erasesize - 1)) / mtd_dev->mtd_erasesize;
	for(i = 0; i < sec_count; i++)
	{
		erase_info.start = offset + (i * mtd_dev->mtd_erasesize);
		erase_info.length = mtd_dev->mtd_erasesize;
		if (ioctl(fd, MEMERASE, &erase_info) == -1)
		{
			printf("Mem Erase Failed\n");
			return -1;
		}
	}

	lseek(fd, offset, SEEK_SET);
	ret = write(fd, buf, cnt);
	printf("wrtie [0x%x] addr=0x%x,offset=%d\n",cnt, offset);
	if (ret != cnt)
	{
		printf("FlashWrite Error\n");
		return -1;
	}
	return 0;
} /* end FlashWrite */

/************************************************************************
 * Erase Flash
 ************************************************************************/
int FlashErase(unsigned long SectorAddr)
{
	return 0;
} /* end FlashErase */

int FlashRead(int fd, char *buf, int offset, int cnt)
{
	int ret = -1;
		
	lseek(fd, offset, SEEK_SET);
	ret = read(fd, buf, cnt);
	if (ret != cnt)
	{
		printf("FlashRead Error\n");
		return -1;
	}
	return 0;
}


#if 0
static int DG_check_cksum_valid(long nbytes)
{
	DG_UPDATE_PACKAGE_HEAD_S *hdr;
	unsigned long checksum;

	hdr = (DG_UPDATE_PACKAGE_HEAD_S *)LOAD_ADDR;

	if (nbytes != (sizeof(*hdr) + ntohl(hdr->package_size))) {
		printf("Image %s bad total SIZE\n", DG_UPDATE_PACKAGE_NAME);
		return -1;
	}
	/* check the data CRC */
	checksum = ntohl(hdr->package_crc);

	if (crc32(0, (unsigned char const *)(LOAD_ADDR + sizeof(*hdr)),
			ntohl(hdr->package_size)) != checksum) {
		printf("Image %s bad data checksum\n", DG_UPDATE_PACKAGE_NAME);
		return -1;
	}

	return 0;
}
#endif

static int DG_check_header_valid(DG_UPDATE_PACKAGE_HEAD_S *hdr)
{
	if (strcmp(hdr->hw_platform, DG_UPDATE_PACKAGE_HW_PLATFORM) != 0) {
		printf("Image %s bad hw platform\n", DG_UPDATE_PACKAGE_PATH);
		return -1;
	}
	
	if (ntohl(hdr->magic) != DG_UPDATE_PACKAGE_MAGIC) {
		printf("Image %s bad MAGIC\n", DG_UPDATE_PACKAGE_PATH);
		return -1;
	}

	return 0;
}

static int DG_check_update_pkg(int pkg_fd)
{
	int ret;
	int offset;
	unsigned int image_size;
	DG_UPDATE_PACKAGE_HEAD_S pkg_hdr;
	DG_UPDATE_IMAGE_HEAD_S image_hdr;

	lseek(pkg_fd, 0, SEEK_SET);
	
	ret = my_read(pkg_fd, (char *)&pkg_hdr, sizeof(pkg_hdr));
	if( ret != sizeof(pkg_hdr) )
	{
		printf("[Error] read package header failed\n");
		goto ERR_exit;
	}

	//校验包头
	ret = DG_check_header_valid(&pkg_hdr);
	if( ret != 0 )
	{
		printf("[Error] package header error\n");
		goto ERR_exit;
	}

	offset = sizeof(DG_UPDATE_PACKAGE_HEAD_S);
	while(1)
	{
		ret = lseek(pkg_fd, offset, SEEK_SET);
		if( ret != offset )
		{
			printf("[Error] lseek failed. ret : %d\n", ret);
			goto ERR_exit;
		}

		ret = my_read(pkg_fd, (char *)&image_hdr, sizeof(image_hdr));
		if( ret != sizeof(image_hdr) )
		{
			if( 0 == ret )
				break;
			printf("[Error] read image header failed\n");
			goto ERR_exit;
		}
		
		image_size = ntohl(image_hdr.image_size);

		offset += sizeof(DG_UPDATE_IMAGE_HEAD_S) + image_size;
	}

	return 0;

ERR_exit:
	return -1;
}

static int DG_do_update(int pkg_fd)
{
	int result = -1;
	DG_UPDATE_IMAGE_HEAD_S image_hdr;
	unsigned int image_type, start_addr, image_size;
	unsigned int write_len;
	unsigned int offset;
	unsigned int todo;
	int rplen, rflen;
	int ret;
	int mtd_fd = -1;
	int write_sum = 0;
	int mtd_index;
	const mtd_dev_s *pmtd_dev_info = NULL;
	char *pkg_buf = NULL;
	char *mtd_buf = NULL;

	pkg_buf = (char *)malloc(MTD_SMALL_SECT_SIZE);
	mtd_buf = (char *)malloc(MTD_SMALL_SECT_SIZE);
	if( NULL == pkg_buf || NULL == mtd_buf )
	{
		printf("malloc failed\n");
		goto err_ext;
	}
	
	lseek(pkg_fd, sizeof(DG_UPDATE_PACKAGE_HEAD_S), SEEK_SET);
	while( 1 )
	{
		//读镜像头
		ret = my_read(pkg_fd, (char *)&image_hdr, sizeof(image_hdr));
		if( ret != sizeof(image_hdr) )
		{
			if( ret == 0 )
				break; 		//没有镜像文件了
			printf("[Error] read image header failed\n");
			goto err_ext;
		}

		image_type = ntohl(image_hdr.image_type);
		start_addr = ntohl(image_hdr.start_addr);
		image_size = ntohl(image_hdr.image_size);

		printf("\n===========================\n");

		if (DG_UPGRADE_IMAGE_TYPE_ENV == image_type)
		{
			printf("image_type : env\n");
		}
		else if (DG_UPGRADE_IMAGE_TYPE_IDBLOCK == image_type)
		{
			printf("image_type : idblock\n");
		}
		else if (DG_UPGRADE_IMAGE_TYPE_UBOOT == image_type)
		{
			printf("image_type : uboot\n");
		}
		else if (DG_UPGRADE_IMAGE_TYPE_KERNEL == image_type)
		{
			printf("image_type : kernel\n");
		}
		else if (DG_UPGRADE_IMAGE_TYPE_ROOTFS == image_type)
		{
			printf("image_type : rootfs\n");
		}
		else if (DG_UPGRADE_IMAGE_TYPE_OEM == image_type)
		{
			printf("image_type : oem\n");
		}
		else
		{
			printf("image_type : unknown\n");
		}

		printf("image_type : %d\n", image_type);
		printf("image_size : %d\n", image_size);
		printf("start_addr : %x\n", start_addr);


		mtd_index = ImageTypeConvertToMtdIndex(image_type);
		if( mtd_index < 0 )
		{
			printf("[Error] ImageTypeConvertToMtdIndex failed\n");
			goto err_ext;
		}
		printf("mtd_index : %x\n", mtd_index);

		{
			pmtd_dev_info = GetMtdDevInfo(mtd_index);
			if( !pmtd_dev_info )
			{
				printf("[Error] GetMtdDevInfo failed\n");
				goto err_ext;
			}
			if( image_size > pmtd_dev_info->mtd_size )
			{
				printf("[Error] image_size error. image size : %x, mtd[%d] size : %x\n", image_size, pmtd_dev_info->mtd_index, pmtd_dev_info->mtd_size);
				goto err_ext;
			}
		
		
			mtd_fd = FlashOpen(mtd_index);
			if(mtd_fd < 0)
			{
				printf("[Error] FlashOpen failed\n");
				goto err_ext;
			}
					
			write_len = image_size;
			
			printf("write_len : %lu\n", write_len);
			for( offset = 0; offset < write_len; offset += todo )
			{
	//			printf("offset : %lu\n", offset);
				todo = min(write_len-offset, MTD_SMALL_SECT_SIZE);
	//			printf("todo : %lu\n", todo);

				rplen = my_read(pkg_fd, pkg_buf, todo);
				if( rplen != todo )
				{
					printf("[Error] read image header failed\n");
					break;
				}
			
				rflen = FlashRead(mtd_fd, mtd_buf, offset, todo);
				if( rflen )
				{
					printf("SPI flash read failed, return %d\n", rflen);
					break;
				}
				
				if( memcmp(pkg_buf, mtd_buf, todo) != 0 )
				{				
					/* copy the data from RAM to FLASH */
	//				printf("flash write %lu...\n", todo);
					ret = FlashWrite(mtd_fd, pmtd_dev_info, pkg_buf, offset, todo);
					if (ret)
					{
						printf("FlashWrite failed, return %d\n", ret);
						break;
					}		
				}
				else
				{
//					printf("same.\n");
				}
			}
			FlashClose(mtd_fd);
		}

		
		if( offset < write_len )
		{
			printf("image type %d update failed.\n", image_type);
			goto err_ext;
		}
		
		printf("image type %d update finish.\n", image_type);
		
	}
	
	result = 0;

err_ext:
	if( mtd_buf )
		free(mtd_buf);
	if( pkg_buf )
		free(pkg_buf);

	return result;
}




int DG_update()
{
	int ret;
	int pkg_fd = -1;
	
	
	ret = InitMtdDevInfo();
	if( ret != 0 )
	{
		printf("[Error] InitMtdDevInfo failed\n");
		goto exit;
	}

	pkg_fd = open(DG_UPDATE_PACKAGE_PATH, O_RDONLY);
	if( pkg_fd < 0 )
	{
		printf("[Error] open %s failed\n", DG_UPDATE_PACKAGE_PATH);
		goto exit;
	}

	//校验升级包
	ret = DG_check_update_pkg(pkg_fd);
	if( ret != 0 )
	{
		printf("[Error] package error\n");
		goto exit;
	}

	ret = DG_do_update(pkg_fd);
	if( ret != 0 )
	{
		printf("[Error] DG_do_update failed\n");
		goto exit;
	}

	printf("\nupdate finish.\n");

exit:
	if( pkg_fd != -1 )
		close(pkg_fd);
	return 0;
}




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
	DG_update();
	return 0;
}
