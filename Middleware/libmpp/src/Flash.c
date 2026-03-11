#include <linux/types.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/mount.h>
#include <mtd/mtd-user.h>
#include <string.h>

#include <stdio.h>
#include "PAL/Flash.h"
#include <unistd.h>

#include "PAL/MW_Common.h"

//#define MNT_DETACH	0x00000002	/* Just detach from the tree */
//#define MNT_EXPIRE	0x00000004	/* Mark for expiry */
#define ERR_PRINT EMSG
#define MTD_SECT_COUNT 5  //flash分区个数

#define ACCESS_FLASH_DISABLE		0x1604
#define ACCESS_FLASH_ENABLE		0xE1604

#define MTD_START		0x00000000
#define MTD_SIZE		0x1000000    //16M FLASH
#define MTD_SMALL_SECT_SIZE 0x10000 // 一个扇区大小

#define MTD_PROC	"/proc/mtd"

typedef struct tagFLASH_SECT
{
	int fd;
	int index;
	unsigned int startaddr;
	unsigned int size;
	unsigned int erasesize;
	char name[32];
	char mtdname[32];
}FLASH_SECT;


FLASH_SECT g_sect[MTD_SECT_COUNT];
static int g_flash_fd = -1;
static int g_sect_num = 0;
static unsigned long g_mtd_saddr = 0;
static unsigned long g_mtd_esize = 0;

int FlashInit(void)
{
	FLASH_SECT *psect;
	unsigned int startaddr;
	FILE *fp;
	char line[128];	
	/* umount mtd devices */
	//if (umount2("/mnt/custom", MNT_DETACH))
	//{
	//	ERR_PRINT("umount Custom Failed\n");
	//}
	//if (umount2("/mnt/web", MNT_DETACH))
	//{
	//	ERR_PRINT("umount Web Failed\n");
	//}
	/*
	if (umount2("/mnt/mtd", MNT_DETACH))
	{
		perror("mtd");
	}
	*/
	g_flash_fd = -1;

	if ((fp = fopen(MTD_PROC, "r")) != NULL)
	{
		fgets(line, sizeof(line), fp);
		if(strstr(line, "dev:") == NULL)
		{
			return -1;
		}
		g_sect_num = 0;
		startaddr = MTD_START;
		while(fgets(line, sizeof(line), fp))
		{
			if(strstr(line, "mtd") != NULL)
			{
				psect = &(g_sect[g_sect_num]);
				memset(psect->name,0,32);
				sscanf(line,"mtd%d: %x %x \"%s\"",&(psect->index), &(psect->size), &(psect->erasesize),(psect->name));

				if(psect->size < MTD_SMALL_SECT_SIZE || psect->size > MTD_SIZE)
				{
					break;
				}
				psect->startaddr = startaddr;
				sprintf(psect->mtdname,"/dev/mtd%d",psect->index);
				psect->fd = open(psect->mtdname, O_RDWR);
				if (psect->fd < 0)
				{
					ERR_PRINT("Open %s error",psect->mtdname);
					break;
				}
				
				EMSG("FlashInit: mtd%d,start:%x,size:%x,esize:%x name:%s\n",psect->index,psect->startaddr,psect->size,psect->erasesize,psect->name);
				if(psect->erasesize > g_mtd_esize)
				{
					g_mtd_esize = psect->erasesize;
				}
				g_sect_num++;
				startaddr = startaddr + psect->size;
			}
			else
			{
				break;
			}
		}
		fclose(fp);	
	}

	return 0;
}

static int FlashMap(unsigned long addr)
{
	int ret = -1;
	int i;
	FLASH_SECT *psect;
	if ((addr < MTD_START) || (addr > (MTD_START + MTD_SIZE)))
	{
		return -1;
	}
	
	for(i = 0; i < g_sect_num; i++)
	{
		psect = &(g_sect[i]);
		if(addr >= psect->startaddr && addr < (psect->startaddr + psect->size))
		{
			g_flash_fd = psect->fd;
			g_mtd_saddr = psect->startaddr;
			g_mtd_esize = psect->erasesize;
			//EMSG("flash:g_mtd_saddr:%x\n",g_mtd_saddr);
			ret = 0;
			break;
		}
	}
	return ret;
}

int FlashDestroy(void)
{
	int i;
	FLASH_SECT *psect;
	for(i = 0; i < g_sect_num; i++)
	{
		psect = &(g_sect[i]);
		if(psect->fd > 0)
		{
			close(psect->fd);
		}
		psect->fd = -1;
	}
	g_flash_fd = -1;
	g_mtd_saddr = 0;
	g_mtd_esize = 0;

	return 0;
}

/************************************************************************
 * Copy memory to flash.
 * Note!! only write one sector 
 ************************************************************************/
int FlashWrite(unsigned char *buf, unsigned long addr, unsigned long cnt)
{
	int ret;
	unsigned int i = 0;
	loff_t offset;
	struct erase_info_user erase_info;
	if(FlashMap(addr))
	{
		ERR_PRINT("Invalid Addr: %lx\n", addr);
		return -1;
	}
	offset = addr - g_mtd_saddr;
	//EMSG("addr=0x%x,offset=%d\n",addr,offset);
	if (offset % g_mtd_esize != 0)
	{
		ERR_PRINT("offset 0x%llx  Doesn't Alias 0x%lx %s\n",offset, g_mtd_esize, __FUNCTION__);
		return -1;
	}

	for(i = 0; i < ((cnt + (g_mtd_esize - 1)) / g_mtd_esize); i++)
	{
		erase_info.start = offset + (i * g_mtd_esize);
		erase_info.length = g_mtd_esize;
		//EMSG("start:%x,length:%x\n",erase_info.start,erase_info.length);
		if (ioctl(g_flash_fd, MEMERASE, &erase_info) == -1)
		{
	        ERR_PRINT("Mem Erase Failed\n");
	        return -1;
		}
	}

	lseek(g_flash_fd, offset, SEEK_SET);
	ret = write(g_flash_fd, buf, cnt);
	EMSG("wrtie [0x%x] addr=0x%x,offset=%d\n",cnt,addr,offset);
	if (ret != cnt)
	{
		ERR_PRINT("FlashWrite Error\n");
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

int FlashRead(unsigned char *buf, unsigned long addr, unsigned long cnt)
{
	int ret = -1;
	loff_t offset;
	
	if(FlashMap(addr))
	{
		ERR_PRINT("Invalid Addr: %lx\n", addr);
		return -1;
	}
	offset = addr - g_mtd_saddr;
	
	lseek(g_flash_fd, offset, SEEK_SET);
	ret = read(g_flash_fd, buf, cnt);
	if (ret != cnt)
	{
		ERR_PRINT("FlashRead Error\n");
		return -1;
	}
	return ret;
}

int FlashGetSectorSize(unsigned long *size)
{
	*size = g_mtd_esize;

	return 0;
}

int FlashGetClientArea(unsigned long *from, unsigned long *to)
{
	return 0;
}
