#include <linux/types.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/mount.h>
#include <mtd/mtd-user.h>
#include <arpa/inet.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "stdarg.h"
#include "limits.h"



typedef enum
{
    false = 0,
    true  = 1,
} bool;


#define MAX_FILE_PATH_LEN 	(256)
#define MAX_PARTITION_NUM 	(16)


#define DG_UPDATE_PACKAGE_NAME 				"linux.bin"
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


typedef struct
{
	long start_addr;
	long partition_size;
	long type;
	char file[MAX_FILE_PATH_LEN];
} PartitionInfo_s;

typedef struct
{
	long flash_size;
	long flash_section_size;
	int partition_num;
	PartitionInfo_s partition_info[MAX_PARTITION_NUM];
} BurnInfo_s;

BurnInfo_s stBurnInfo_g = {0};


extern uint crc32(uint crc, char *buff, int len);

void debug_printf(const char *func, int line, const char *fmt, ...)
{
	va_list args;
	
	printf("[%s]:[%d]: ", func, line);
			
	va_start (args, fmt);
	vprintf(fmt, args);
	va_end (args);
	printf("\n");
}


#define my_printf(fmt...) 	debug_printf(__FUNCTION__, __LINE__, fmt)



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

static int my_fread(FILE *fp, char *buffer, int size)
{
	int ret;
	int read_count = 0;
	
	while(read_count < size)
	{
		ret = fread(buffer+read_count, 1, size-read_count, fp);
		if( ret <= 0 )
			break;
		read_count += ret;
	}

	return read_count;
}

static int my_fwrite(FILE *fp, char *buffer, int size)
{
	int ret;
	int write_count = 0;
	
	while(write_count < size)
	{
		ret = fwrite(buffer+write_count, 1, size-write_count, fp);
		if( ret <= 0 )
			break;
		write_count += ret;
	}

	return write_count;
}




// ini read
static bool bInited_g = false;
static FILE *IniFp_g = NULL;
static char *pIniBuffer_g = NULL;
int Ini_open(const char *path)
{
	if( bInited_g )
	{
		my_printf("[Error] ini file is already open.\n");
		return -1;
	}

	if(!path)
	{
		my_printf("[Error] Ini_open path error.\n");
		return -1;
	}
	
	int ret;
    struct stat st = {0};

	IniFp_g = fopen(path, "r");
	if( !IniFp_g )
	{
		my_printf("[Error] open %s failed\n", path);
		goto err_exit;
	}

    if (stat(path, &st) == -1)
    {
		my_printf("[Error] stat %s failed\n", path);
		goto err_exit;
	}

	if( 0 == st.st_size )
	{
		my_printf("[Error] %s is empty.\n", path);
		goto err_exit;
	}

	pIniBuffer_g = (char *)malloc(st.st_size + 1);
	if( !pIniBuffer_g )
	{
		my_printf("[Error] malloc failed.\n");
		goto err_exit;
	}
	
	memset(pIniBuffer_g, 0, st.st_size + 1);

	ret = my_fread(IniFp_g, pIniBuffer_g, st.st_size);
	if( ret != st.st_size )
	{
		my_printf("[Error] read %s failed.\n", path);
		goto err_exit;
	}

	bInited_g = true;

	return 0;

err_exit:

	if( pIniBuffer_g )
		free(pIniBuffer_g);
	pIniBuffer_g = NULL;
	
	if( IniFp_g )
		fclose(IniFp_g);
	IniFp_g = NULL;
	
	return -1;
}

int Ini_close()
{
	if( !bInited_g )
		return 0;

	if( pIniBuffer_g )
		free(pIniBuffer_g);
	pIniBuffer_g = NULL;
	
	if( IniFp_g )
		fclose(IniFp_g);
	IniFp_g = NULL;
	
	bInited_g = false;

	return 0;
}

//
static int getLine(const char *pIn, char *line, int *line_size, int line_max_size)
{
	if( !pIn || !line || !line_size )
		return -1;

	int next_offset = 0;
	int lineLen = 0;
	char *pTmp = strstr(pIn, "\r\n");
	if( !pTmp )
	{
		pTmp = strchr(pIn, '\n');
		if( !pTmp )
		{
			lineLen = strlen(pIn);
			next_offset = lineLen;
		}
		else
		{
			lineLen = pTmp - pIn;
			next_offset = lineLen + 1;
		}
	}
	else
	{
		lineLen = pTmp - pIn;
		next_offset = lineLen + 2;
	}

	if( lineLen > line_max_size )
		return -1;

	memcpy(line, pIn, lineLen);
	line[lineLen] = '\0';

	*line_size = lineLen;

	return next_offset;
}

int Ini_readStringValue(const char* section, const char* key, char *value, int value_max_size)
{
	if( !bInited_g )
	{
		my_printf("[Error] ini file have no opened.\n");
		return -1;
	}

	int offset = 0;
	char lineBuffer[1024];
	char tmpBuffer[1024];
	int lineLen;
	char *pTmp = NULL;
	char *pPos = pIniBuffer_g;

	bool bGotValue = false;
	bool bFoundSection = false;
	
	while(1)
	{
		pPos += offset;
		offset = getLine(pPos, lineBuffer, &lineLen, sizeof(lineBuffer) - 1);
		if( offset <= 0 )
			break;
		if( 0 == lineLen )
			continue;
		if( '#' == lineBuffer[0] || ';' == lineBuffer[0] )
			continue;

		if( !bFoundSection )
		{
			snprintf(tmpBuffer, sizeof(tmpBuffer), "[%s]", section);
			pTmp = strstr(lineBuffer, tmpBuffer);
			if( !pTmp )
				continue;

			bFoundSection = true;
		}
		else
		{
			if( strchr(lineBuffer, '[') && strchr(lineBuffer, ']') )
				break;

			snprintf(tmpBuffer, sizeof(tmpBuffer), "%s=", key);
			pTmp = strstr(lineBuffer, tmpBuffer);
			if( pTmp )
			{
				snprintf(value, value_max_size, "%s", pTmp+strlen(tmpBuffer));
				bGotValue = true;
				break;
			}
		}
	}
	
	return (bGotValue?0:-1);
}


int Parse_ini(BurnInfo_s *pstBurnInfo, const char *partition_file)
{
	if( !pstBurnInfo || !partition_file )
		return -1;
	
	int ret;
	int result = -1;
	char *endptr = NULL;
	char value_str[1024] = {0};

	long flash_size;
	long sector_size;

	
	ret = Ini_open(partition_file);
	if( ret != 0 )
	{
		my_printf("[Error] Ini_open failed.\n");
		goto exit;
	}

	//flash_size
	ret = Ini_readStringValue("global", "flash_size", value_str, 1024);
	if( ret != 0 )
	{
		my_printf("[Error] Ini_readStringValue failed.\n");
		goto exit;
	}
	endptr = NULL;
	flash_size = strtol(value_str, &endptr, 16);	
	if( LONG_MAX == flash_size || LONG_MIN == flash_size || (NULL != endptr && '\0' != *endptr) )
	{
		my_printf("[Error] strtol flash_size failed.\n");
		goto exit;
	}

	//sector_size
	ret = Ini_readStringValue("global", "sector_size", value_str, 1024);
	if( ret != 0 )
	{
		my_printf("[Error] Ini_readStringValue failed.\n");
		goto exit;
	}
	endptr = NULL;
	sector_size = strtol(value_str, &endptr, 16);
	if( LONG_MAX == sector_size || LONG_MIN == sector_size || (NULL != endptr && '\0' != *endptr) )
	{
		my_printf("[Error] strtol sector_size failed.\n");
		goto exit;
	}
	//env
	do{
		long env_start, env_size;
		char env_file[MAX_FILE_PATH_LEN];
		//start
		ret = Ini_readStringValue("env", "start", value_str, 1024);
		if( ret != 0 )
		{
			my_printf("[Warnning] ini have no env start.\n");
			break;
		}
		endptr = NULL;
		env_start = strtol(value_str, &endptr, 16);
		if( LONG_MAX == env_start || LONG_MIN == env_start || (NULL != endptr && '\0' != *endptr) )
		{
			my_printf("[Warnning] strtol env_start failed.\n");
			break;
		}
		//size
		ret = Ini_readStringValue("env", "size", value_str, 1024);
		if( ret != 0 )
		{
			my_printf("[Warnning] ini have no env size.\n");
			break;
		}
		endptr = NULL;
		env_size = strtol(value_str, &endptr, 16);
		if( LONG_MAX == env_size || LONG_MIN == env_size || (NULL != endptr && '\0' != *endptr) )
		{
			my_printf("[Warnning] strtol env_size failed.\n");
			break;
		}
		//file
		ret = Ini_readStringValue("env", "file", value_str, 1024);
		if( ret != 0 )
		{
			my_printf("[Warnning] ini have no env file.\n");
			break;
		}
		snprintf(env_file, sizeof(env_file), "%s", value_str);

		//check param
		if( ((env_start%sector_size) != 0) || ((env_start+env_size) > flash_size) )
		{
			my_printf("[Error] env param error.\n");
			goto exit;
		}			

		pstBurnInfo->partition_info[pstBurnInfo->partition_num].start_addr = env_start;
		pstBurnInfo->partition_info[pstBurnInfo->partition_num].partition_size = env_size;
		pstBurnInfo->partition_info[pstBurnInfo->partition_num].type = DG_UPGRADE_IMAGE_TYPE_ENV;
		memcpy(pstBurnInfo->partition_info[pstBurnInfo->partition_num].file, env_file, MAX_FILE_PATH_LEN);
		pstBurnInfo->partition_num++;
		
	}while(0);
	//idblock
	do{
		long idblock_start, idblock_size;
		char idblock_file[MAX_FILE_PATH_LEN];
		//start
		ret = Ini_readStringValue("idblock", "start", value_str, 1024);
		if( ret != 0 )
		{
			my_printf("[Warnning] ini have no idblock start.\n");
			break;
		}
		endptr = NULL;
		idblock_start = strtol(value_str, &endptr, 16);
		if( LONG_MAX == idblock_start || LONG_MIN == idblock_start || (NULL != endptr && '\0' != *endptr) )
		{
			my_printf("[Warnning] strtol idblock_start failed.\n");
			break;
		}
		//size
		ret = Ini_readStringValue("idblock", "size", value_str, 1024);
		if( ret != 0 )
		{
			my_printf("[Warnning] ini have no idblock size.\n");
			break;
		}
		endptr = NULL;
		idblock_size = strtol(value_str, &endptr, 16);
		if( LONG_MAX == idblock_size || LONG_MIN == idblock_size || (NULL != endptr && '\0' != *endptr) )
		{
			my_printf("[Warnning] strtol idblock_size failed.\n");
			break;
		}
		//file
		ret = Ini_readStringValue("idblock", "file", value_str, 1024);
		if( ret != 0 )
		{
			my_printf("[Warnning] ini have no idblock file.\n");
			break;
		}
		snprintf(idblock_file, sizeof(idblock_file), "%s", value_str);

		//check param
		if( ((idblock_start%sector_size) != 0) || ((idblock_start+idblock_size) > flash_size) )
		{
			my_printf("[Error] idblock param error.\n");
			goto exit;
		}			

		pstBurnInfo->partition_info[pstBurnInfo->partition_num].start_addr = idblock_start;
		pstBurnInfo->partition_info[pstBurnInfo->partition_num].partition_size = idblock_size;
		pstBurnInfo->partition_info[pstBurnInfo->partition_num].type = DG_UPGRADE_IMAGE_TYPE_IDBLOCK;
		memcpy(pstBurnInfo->partition_info[pstBurnInfo->partition_num].file, idblock_file, MAX_FILE_PATH_LEN);
		pstBurnInfo->partition_num++;
		
	}while(0);
	//uboot
	do{
		long boot_start, boot_size;
		char boot_file[MAX_FILE_PATH_LEN];
		//start
		ret = Ini_readStringValue("uboot", "start", value_str, 1024);
		if( ret != 0 )
		{
			my_printf("[Warnning] ini have no uboot start.\n");
			break;
		}
		endptr = NULL;
		boot_start = strtol(value_str, &endptr, 16);
		if( LONG_MAX == boot_start || LONG_MIN == boot_start || (NULL != endptr && '\0' != *endptr) )
		{
			my_printf("[Warnning] strtol uboot_start failed.\n");
			break;
		}
		//size
		ret = Ini_readStringValue("uboot", "size", value_str, 1024);
		if( ret != 0 )
		{
			my_printf("[Warnning] ini have no uboot size.\n");
			break;
		}
		endptr = NULL;
		boot_size = strtol(value_str, &endptr, 16);
		if( LONG_MAX == boot_size || LONG_MIN == boot_size || (NULL != endptr && '\0' != *endptr) )
		{
			my_printf("[Warnning] strtol uboot_size failed.\n");
			break;
		}
		//file
		ret = Ini_readStringValue("uboot", "file", value_str, 1024);
		if( ret != 0 )
		{
			my_printf("[Warnning] ini have no uboot file.\n");
			break;
		}
		snprintf(boot_file, sizeof(boot_file), "%s", value_str);

		//check param
		if( ((boot_start%sector_size) != 0) || ((boot_start+boot_size) > flash_size) )
		{
			my_printf("[Error] uboot param error.\n");
			goto exit;
		}			

		pstBurnInfo->partition_info[pstBurnInfo->partition_num].start_addr = boot_start;
		pstBurnInfo->partition_info[pstBurnInfo->partition_num].partition_size = boot_size;
		pstBurnInfo->partition_info[pstBurnInfo->partition_num].type = DG_UPGRADE_IMAGE_TYPE_UBOOT;
		memcpy(pstBurnInfo->partition_info[pstBurnInfo->partition_num].file, boot_file, MAX_FILE_PATH_LEN);
		pstBurnInfo->partition_num++;
		
	}while(0);
	
	//kernel
	do{
		long kernel_start, kernel_size;
		char kernel_file[MAX_FILE_PATH_LEN];
		//start
		ret = Ini_readStringValue("kernel", "start", value_str, 1024);
		if( ret != 0 )
		{
			my_printf("[Warnning] ini have no kernel start.\n");
			break;
		}
		endptr = NULL;
		kernel_start = strtol(value_str, &endptr, 16);
		if( LONG_MAX == kernel_start || LONG_MIN == kernel_start || (NULL != endptr && '\0' != *endptr) )
		{
			my_printf("[Warnning] strtol kernel_start failed.\n");
			break;
		}
		//size
		ret = Ini_readStringValue("kernel", "size", value_str, 1024);
		if( ret != 0 )
		{
			my_printf("[Warnning] ini have no kernel size.\n");
			break;
		}
		endptr = NULL;
		kernel_size = strtol(value_str, &endptr, 16);
		if( LONG_MAX == kernel_size || LONG_MIN == kernel_size || (NULL != endptr && '\0' != *endptr) )
		{
			my_printf("[Warnning] strtol kernel_size failed.\n");
			break;
		}
		//file
		ret = Ini_readStringValue("kernel", "file", value_str, 1024);
		if( ret != 0 )
		{
			my_printf("[Warnning] ini have no kernel file.\n");
			break;
		}
		snprintf(kernel_file, sizeof(kernel_file), "%s", value_str);

		//check param
		if( ((kernel_start%sector_size) != 0) || ((kernel_start+kernel_size) > flash_size) )
		{
			my_printf("[Error] kernel param error.\n");
			goto exit;
		}			
		
		pstBurnInfo->partition_info[pstBurnInfo->partition_num].start_addr = kernel_start;
		pstBurnInfo->partition_info[pstBurnInfo->partition_num].partition_size = kernel_size;
		pstBurnInfo->partition_info[pstBurnInfo->partition_num].type = DG_UPGRADE_IMAGE_TYPE_KERNEL;
		memcpy(pstBurnInfo->partition_info[pstBurnInfo->partition_num].file, kernel_file, MAX_FILE_PATH_LEN);
		pstBurnInfo->partition_num++;

	}while(0);
		
	//rootfs
	do{
		long rootfs_start, rootfs_size;
		char rootfs_file[MAX_FILE_PATH_LEN];
		//start
		ret = Ini_readStringValue("rootfs", "start", value_str, 1024);
		if( ret != 0 )
		{
			my_printf("[Warnning] ini have no rootfs start.\n");
			break;
		}
		endptr = NULL;
		rootfs_start = strtol(value_str, &endptr, 16);
		if( LONG_MAX == rootfs_start || LONG_MIN == rootfs_start || (NULL != endptr && '\0' != *endptr) )
		{
			my_printf("[Warnning] strtol rootfs_start failed.\n");
			break;
		}
		//size
		ret = Ini_readStringValue("rootfs", "size", value_str, 1024);
		if( ret != 0 )
		{
			my_printf("[Warnning] ini have no rootfs size.\n");
			break;
		}
		endptr = NULL;
		rootfs_size = strtol(value_str, &endptr, 16);
		if( LONG_MAX == rootfs_size || LONG_MIN == rootfs_size || (NULL != endptr && '\0' != *endptr) )
		{
			my_printf("[Warnning] strtol rootfs_size failed.\n");
			break;
		}
		//file
		ret = Ini_readStringValue("rootfs", "file", value_str, 1024);
		if( ret != 0 )
		{
			my_printf("[Warnning] ini have no rootfs file.\n");
			break;
		}
		snprintf(rootfs_file, sizeof(rootfs_file), "%s", value_str);
		
		//check param
		if( ((rootfs_start%sector_size) != 0) || ((rootfs_start+rootfs_size) > flash_size) )
		{
			my_printf("[Error] rootfs param error.\n");
			goto exit;
		}			
		
		pstBurnInfo->partition_info[pstBurnInfo->partition_num].start_addr = rootfs_start;
		pstBurnInfo->partition_info[pstBurnInfo->partition_num].partition_size = rootfs_size;
		pstBurnInfo->partition_info[pstBurnInfo->partition_num].type = DG_UPGRADE_IMAGE_TYPE_ROOTFS;
		memcpy(pstBurnInfo->partition_info[pstBurnInfo->partition_num].file, rootfs_file, MAX_FILE_PATH_LEN);
		pstBurnInfo->partition_num++;

	}while(0);
	
	//oem
	do{
		long oem_start, oem_size;
		char oem_file[MAX_FILE_PATH_LEN];
		//start
		ret = Ini_readStringValue("oem", "start", value_str, 1024);
		if( ret != 0 )
		{
			my_printf("[Warnning] ini have no oem start.\n");
			break;
		}
		endptr = NULL;
		oem_start = strtol(value_str, &endptr, 16);
		if( LONG_MAX == oem_start || LONG_MIN == oem_start || (NULL != endptr && '\0' != *endptr) )
		{
			my_printf("[Warnning] strtol oem_start failed.\n");
			break;
		}
		//size
		ret = Ini_readStringValue("oem", "size", value_str, 1024);
		if( ret != 0 )
		{
			my_printf("[Warnning] ini have no oem size.\n");
			break;
		}
		endptr = NULL;
		oem_size = strtol(value_str, &endptr, 16);
		if( LONG_MAX == oem_size || LONG_MIN == oem_size || (NULL != endptr && '\0' != *endptr) )
		{
			my_printf("[Warnning] strtol oem_size failed.\n");
			break;
		}
		//file
		ret = Ini_readStringValue("oem", "file", value_str, 1024);
		if( ret != 0 )
		{
			my_printf("[Warnning] ini have no oem file.\n");
			break;
		}
		snprintf(oem_file, sizeof(oem_file), "%s", value_str);
		
		//check param
		if( ((oem_start%sector_size) != 0) || ((oem_start+oem_size) > flash_size) )
		{
			my_printf("[Error] oem param error.\n");
			goto exit;
		}			
		
		pstBurnInfo->partition_info[pstBurnInfo->partition_num].start_addr = oem_start;
		pstBurnInfo->partition_info[pstBurnInfo->partition_num].partition_size = oem_size;
		pstBurnInfo->partition_info[pstBurnInfo->partition_num].type = DG_UPGRADE_IMAGE_TYPE_OEM;
		memcpy(pstBurnInfo->partition_info[pstBurnInfo->partition_num].file, oem_file, MAX_FILE_PATH_LEN);
		pstBurnInfo->partition_num++;

	}while(0);

	pstBurnInfo->flash_size = flash_size;
	pstBurnInfo->flash_section_size = sector_size;
	
	result = 0;

exit:

	Ini_close();
	
	return result;
}


int write_partition(FILE *fp, PartitionInfo_s *pstPartitionInfo)
{
	int ret;
	int result = -1;
	int file_len;
	int write_count;
	int fill_size;
	int need_complement_count = 0; 	//需要补齐的字节数
	struct stat st = {0};
	FILE *partition_file_fp = NULL;
	unsigned char *buffer = NULL;

	DG_UPDATE_IMAGE_HEAD_S stImageHead;

	my_printf("write %s to target, start address[0x%lx].", pstPartitionInfo->file, pstPartitionInfo->start_addr);

	buffer = (unsigned char *)malloc(stBurnInfo_g.flash_section_size);
	if( ! buffer )
	{
		my_printf("[Error] malloc failed.\n");
		goto exit;
	}
	
	partition_file_fp = fopen(pstPartitionInfo->file, "r");
	if( NULL == partition_file_fp )
	{
		my_printf("[Error] open %s faied.\n", pstPartitionInfo->file);
		goto exit;
	}
	
    if (stat(pstPartitionInfo->file, &st) == -1)
    {
		my_printf("[Error] stat %s failed\n", pstPartitionInfo->file);
		goto exit;
	}

	if( st.st_size > pstPartitionInfo->partition_size )
	{
		my_printf("[Error] %s size[0x%x] is more than partition_size[0x%lx].\n", pstPartitionInfo->file, st.st_size, pstPartitionInfo->partition_size);
		goto exit;
	}
	file_len = st.st_size;

	if( DG_UPGRADE_IMAGE_TYPE_UBOOT == pstPartitionInfo->type ) 		//如果是uboot则补齐整个uboot分区的大小
	{
		need_complement_count = pstPartitionInfo->partition_size - file_len;
	}
	else
	{
		need_complement_count = 4 - (file_len % 4);
		need_complement_count %= 4;
	}

	//写入image head
	stImageHead.image_type = htonl(pstPartitionInfo->type);
	stImageHead.image_size = htonl(file_len + need_complement_count);
	stImageHead.start_addr = htonl(pstPartitionInfo->start_addr);
	ret = my_fwrite(fp, (char *)&stImageHead, sizeof(stImageHead));
	if( ret != sizeof(stImageHead) )
	{
		my_printf("[Error] write image head faied.\n");
		goto exit;
	}

	write_count = 0;
	while(write_count < file_len)
	{
		int remian_size = file_len - write_count;
		int write_len = ( remian_size > stBurnInfo_g.flash_section_size ) ? stBurnInfo_g.flash_section_size : remian_size;

		memset(buffer, 0xFF, stBurnInfo_g.flash_section_size);
		
		ret = my_fread(partition_file_fp, buffer, write_len);
		if( ret != write_len )
		{
			my_printf("[Error] read %s faied.\n", pstPartitionInfo->file);
			goto exit;
		}

		ret = my_fwrite(fp, buffer, write_len);
		if( ret != write_len )
		{
			my_printf("[Error] write %s to target file faied.\n", pstPartitionInfo->file);
			goto exit;
		}
		write_count += write_len;
	}

	//最后一包, 保证4字节对齐
	write_count = 0;
	while(write_count < need_complement_count)
	{
		int remian_size = need_complement_count - write_count;
		int write_len = ( remian_size > stBurnInfo_g.flash_section_size ) ? stBurnInfo_g.flash_section_size : remian_size;
		
		memset(buffer, 0xFF, write_len);

		ret = my_fwrite(fp, buffer, write_len);
		if( ret != write_len )
		{
			my_printf("[Error] write %s to target file faied.\n", pstPartitionInfo->file);
			goto exit;
		}
		write_count += write_len;
	}
	
	result = sizeof(stImageHead) + file_len + need_complement_count;

exit:
	if( partition_file_fp )
		fclose(partition_file_fp);

	if( buffer )
		free(buffer);
	
	return result;
}

unsigned int get_crc32(FILE *fp, int offset, int len)
{
	int ret;
	unsigned int crc = 0;
	
	if( !fp || len <= 0 )
		return 0;

	char *buff = (char *)malloc(len);
	if( NULL == buff )
	{
		my_printf("[Error] malloc failed.\n");
		return 0;
	}
	
	ret = fseek(fp, offset, SEEK_SET);
	if( ret != 0 )
	{
		my_printf("[Error] fseek failed.\n");
		goto exit;
	}

	ret = my_fread(fp, buff, len);
	if( ret != len )
	{
		my_printf("[Error] my_fread failed.\n");
		goto exit;
	}

	crc = crc32(0, buff, len);
	my_printf("crc : %x\n", crc);

exit:
	if( buff )
		free(buff);
	
	return crc;
}

int main(int argc, char **argv)
{
	if( argc != 3 )
	{
		my_printf("usage : packaging merge_ini target_file.\n");
		return -1;
	}

	int ret, bSucc;
	FILE *fp = NULL;
	int write_count = 0;

    struct stat st = {0};
	const char *partition_file_path 	= (const char *)argv[1];
	const char *target_file_path	 	= (const char *)argv[2];
	int file_size = 0;
	
	char tmp_buffer[128];
	char *buffer = NULL;

	unsigned int package_crc;
	int package_size;
	DG_UPDATE_PACKAGE_HEAD_S stPackageHead;


	//解析ini配置文件
	ret = Parse_ini(&stBurnInfo_g, partition_file_path);
	if( ret != 0 )
	{
		my_printf("[Error] Parse_ini failed.\n");
		return -1;
	}

	printf("\n=====================ini info=====================\n");
	printf("flash size          : 0x%lx\n", stBurnInfo_g.flash_size);
	printf("flash section size  : 0x%lx\n", stBurnInfo_g.flash_section_size);
	
	printf("partition num       : %d\n", stBurnInfo_g.partition_num);
	int i;
	for(i = 0; i < stBurnInfo_g.partition_num; i++)
	{
		printf("    partition[%d] start addr     : 0x%lx\n", i, stBurnInfo_g.partition_info[i].start_addr);
		printf("    partition[%d] partition size : 0x%lx\n", i, stBurnInfo_g.partition_info[i].partition_size);
		printf("    partition[%d] file           : %s\n", i, stBurnInfo_g.partition_info[i].file);
		printf("\n");
	}
	

	buffer = (char *)malloc(stBurnInfo_g.flash_section_size);
	if( ! buffer )
	{
		my_printf("[Error] malloc failed.\n");
		return -1;
	}
	
	//打开目标文件
	fp = fopen(target_file_path, "w+");
	if( NULL == fp )
	{
		my_printf("[Error] open %s faied.\n", target_file_path);
		goto exit;
	}

	//写入包头
	memset(&stPackageHead, 0, sizeof(stPackageHead));
	ret = my_fwrite(fp, (char *)&stPackageHead, sizeof(stPackageHead));
	if( ret != sizeof(stPackageHead) )
	{
		my_printf("[Error] write %s faied.\n", target_file_path);
		goto exit;
	}

	package_size = 0;
	for(i = 0; i < stBurnInfo_g.partition_num; i++)
	{
		ret = write_partition(fp, &stBurnInfo_g.partition_info[i]);
		if( -1 == ret )
		{
			my_printf("[Error] write_partition [%d] faied.\n", i);
			goto exit;
		}
		package_size += ret;
	}

	package_crc = get_crc32(fp, sizeof(DG_UPDATE_PACKAGE_HEAD_S), package_size);
	if( 0 == package_crc )
	{
		my_printf("[Error] get_crc32 faied.\n");
		goto exit;
	}
	
	//复写入包头
	ret = fseek(fp, 0, SEEK_SET);
	if( ret != 0 )
	{
		my_printf("[Error] fseek faied.\n");
		goto exit;
	}
	memset(&stPackageHead, 0, sizeof(stPackageHead));
	memcpy(stPackageHead.hw_platform, DG_UPDATE_PACKAGE_HW_PLATFORM, strlen(DG_UPDATE_PACKAGE_HW_PLATFORM));
	stPackageHead.magic = htonl(DG_UPDATE_PACKAGE_MAGIC);
	stPackageHead.package_crc = htonl(package_crc);
	stPackageHead.package_size = htonl(package_size);
	ret = my_fwrite(fp, (char *)&stPackageHead, sizeof(stPackageHead));
	if( ret != sizeof(stPackageHead) )
	{
		my_printf("[Error] write %s faied.\n", target_file_path);
		goto exit;
	}

	my_printf("finish.\n");

exit:
	if( fp )
		fclose(fp);
	if( buffer )
		free(buffer);
	return 0;
}


