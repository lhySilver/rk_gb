/*
 *SD update support
 */

#include <malloc.h>
#include <asm/io.h>
#include <spi_flash.h>
#include <fat.h>

#include <common.h>
#include <boot_rkimg.h>
#include <dm.h>
#include <part.h>
#include <spl.h>
#include <spl_rkfw.h>
#include <linux/compiler.h>
#include <errno.h>
#include <asm/u-boot.h>
#include <errno.h>
#include <console.h>
#include <image.h>

#include <linux/mtd/mtd.h>
#include <spi.h>
#include <dm/device-internal.h>

#include <stdlib.h>
#include <stdio.h>


#define DG_SPI_NOR_SF
//#define DG_SPI_NOR_BLK

#define xmin(a, b) (((a) < (b)) ? (a) : (b))

/* where to load files into memory */
#define LOAD_ADDR ((unsigned char *)0x000e00000)

#define DG_UPDATE_PACKAGE_NAME 				"linux.bin"
#define DG_UPDATE_PACKAGE_HW_PLATFORM 		"rv1106"
#define DG_UPDATE_PACKAGE_MAGIC				(0xABCD1234)

typedef enum{
	DG_UPGRADE_IMAGE_TYPE_ENV           = 1,
	DG_UPGRADE_IMAGE_TYPE_IDBLOCK       = 2,
	DG_UPGRADE_IMAGE_TYPE_UBOOT 		= 3,
	DG_UPGRADE_IMAGE_TYPE_KERNEL 		= 4,
	DG_UPGRADE_IMAGE_TYPE_ROOTFS 		= 5,
	DG_UPGRADE_IMAGE_TYPE_OEM           = 6,
	DG_UPGRADE_IMAGE_TYPE_CONF          = 7,
	DG_UPGRADE_IMAGE_TYPE_MAX,
}DG_UPGRADE_IMAGE_TYPE_E;

typedef struct{
	char hw_platform[20]; 		//DG_UPDATE_PACKAGE_HW_PLATFORM
	unsigned int magic;
	unsigned int package_crc;
	unsigned int package_size;
}DG_UPDATE_PACKAGE_HEAD_S;


typedef struct{
	unsigned int image_type;
	unsigned int image_size;
	unsigned int start_addr;
}DG_UPDATE_IMAGE_HEAD_S;

#define DG_UPDATE_PACKAGE_MAX_SIZE			(0x1000000+sizeof(DG_UPDATE_PACKAGE_HEAD_S)+(sizeof(DG_UPDATE_IMAGE_HEAD_S)*DG_UPGRADE_IMAGE_TYPE_MAX))


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

static int DG_check_header_valid(long nbytes)
{
	DG_UPDATE_PACKAGE_HEAD_S *hdr;

	hdr = (DG_UPDATE_PACKAGE_HEAD_S *)LOAD_ADDR;

	if (nbytes < sizeof(*hdr)) {
		printf("Image %s bad header SIZE\n", DG_UPDATE_PACKAGE_NAME);
		return -1;
	}

	if (strcmp(hdr->hw_platform, DG_UPDATE_PACKAGE_HW_PLATFORM) != 0) {
		printf("Image %s bad hw platform\n", DG_UPDATE_PACKAGE_NAME);
		return -1;
	}
	
	if (ntohl(hdr->magic) != DG_UPDATE_PACKAGE_MAGIC) {
		printf("Image %s bad MAGIC\n", DG_UPDATE_PACKAGE_NAME);
		return -1;
	}

	return 0;
}

#ifdef DG_SPI_NOR_BLK
static int DG_do_update_blk(long sz)
{
	int result = 0;
	DG_UPDATE_IMAGE_HEAD_S image_hdr;
	unsigned long pkg_offset;
	unsigned long start, len;
	unsigned long write_len;
	unsigned long offset;
	unsigned long todo;
	unsigned char bChange;
	int rc;
	void *buf = NULL;
	char *pbuf = NULL;
	char *cmp_buf = NULL;
	unsigned int erase_size;

	int byte_per_block = 0x200;// 512 Byte
	int cur_devnump = 2;
	int dev = (int)simple_strtoul("2", NULL, 10);
	if (!blk_show_device(IF_TYPE_MTD, dev)) 
	{
		cur_devnump = dev;
		printf("... is now current device\n");
	} 
	else 
	{
		return -1;
	}
	erase_size = 0xA0000;//64 KB

	cmp_buf = (char *)malloc(erase_size);
	if( NULL == cmp_buf ){
		printf("cmp_buf failed\n");
		goto err_ext;
	}

	buf = map_physmem((unsigned long)LOAD_ADDR, sz, MAP_WRBACK);
	if (!buf) {
		puts("Failed to map physical memory\n");
		goto err_ext;
	}

	pkg_offset = sizeof(DG_UPDATE_PACKAGE_HEAD_S);
	while( pkg_offset < sz ) {
		bChange = 0;
		memcpy(&image_hdr, buf + pkg_offset, sizeof(DG_UPDATE_IMAGE_HEAD_S));

		start = ntohl(image_hdr.start_addr);
		len = ntohl(image_hdr.image_size);

		unsigned int image_type = ntohl(image_hdr.image_type);
		unsigned int image_size = ntohl(image_hdr.image_size);
		unsigned int start_addr = ntohl(image_hdr.start_addr);

		printf("\n");

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
		
		printf("image_size : %d\n", image_size);
		printf("start_addr : 0x%x\n", start_addr);
		printf("pkg_offset : %lu\n", pkg_offset);
		pbuf = (buf + pkg_offset + sizeof(image_hdr));
		write_len = len;
		printf("write_len : %lu\n", write_len);

		for( offset = 0; offset < write_len; offset += todo )
		{
			todo = xmin(write_len-offset, erase_size);
			lbaint_t blk = (start+offset)/byte_per_block;
			ulong cnt = erase_size/byte_per_block;
			ulong n;
			// printf("\n%s read: device %d block # "LBAFU", count %lu ... ",if_name, cur_devnump, blk, cnt);
			n = blk_read_devnum(IF_TYPE_MTD, cur_devnump, blk, cnt,cmp_buf);
			// printf("%ld blocks read: %s\n", n, n == cnt ? "OK" : "ERROR");
			if (n == cnt ) {rc = 0;}else{rc = 1;}
			if( rc ) {
				printf("SPI flash read failed, return %d\n", rc);
				goto err_ext;
			}

			if( memcmp(pbuf+offset, cmp_buf, todo) != 0 )
			{
				bChange = 1;
			}
		}

		if (1 == bChange)
		{
			// {
			// 	lbaint_t blk = (start)/byte_per_block;
			// 	ulong cnt = write_len/byte_per_block;
			// 	ulong n;
			// 	printf("erase: device:%d block:%lu  cnt: %lu ... \n", cur_devnump, blk, cnt);
			// 	n = blk_erase_devnum(IF_TYPE_MTD, cur_devnump, blk, cnt);
			// 	// printf("%ld blocks erased: %s\n", n, n == cnt ? "OK" : "ERROR");
			// 	if (n == cnt ) {rc = 0;}else{rc = 1;}
			// 	if (rc) {
			// 		printf("SPI flash sector erase failed\n");
			// 		goto err_ext;
			// 	}
			// }

			{
				lbaint_t blk = start/byte_per_block;
				ulong cnt = write_len/byte_per_block;
				ulong n;
				printf("write: device:%d block:%lu  cnt: %lu ... \n", cur_devnump, blk, cnt);
				n = blk_write_devnum(IF_TYPE_MTD, cur_devnump, blk, cnt, (ulong *)pbuf);
				printf("%ld blocks written: %s\n", n, n == cnt ? "OK" : "ERROR");
				if (n == cnt ) {rc = 0;}else{rc = 1;}
				if (rc) {
					printf("SPI flash write failed, return %d\n", rc);
					goto err_ext;
				}
			}
		}
		else
		{
			printf("the same.\n");
		}

		printf("\nimage type %d update finish.\n", ntohl(image_hdr.image_type));
		pkg_offset += sizeof(image_hdr) + len;
		printf("new pkg_offset : %lu\n", pkg_offset);
		if( (DG_UPGRADE_IMAGE_TYPE_UBOOT == ntohl(image_hdr.image_type)) && (1 == bChange) )
		{
			result = 1;
			printf("\n\nIf u-boot had been updated, we need to reset.\n\n");
			break;
		}
		else if( (DG_UPGRADE_IMAGE_TYPE_ENV == ntohl(image_hdr.image_type)) && (1 == bChange) )
		{
			result = 1;
			printf("If env had been updated, we need to reset.\n");
			break;
		}
		else if( (DG_UPGRADE_IMAGE_TYPE_IDBLOCK == ntohl(image_hdr.image_type)) && (1 == bChange) )
		{
			result = 1;
			printf("If idblock had been updated, we need to reset.\n");
			break;
		}
	}
	
err_ext:
	if( cmp_buf )
		free(cmp_buf);

	if( buf )
		unmap_physmem(buf, sz);

	return result;
}
#endif

#ifdef DG_SPI_NOR_SF
static int DG_do_update_sf(long sz)
{
	int result = 0;
	int rc;

	unsigned long pkg_offset;
	unsigned long offset;
	unsigned long todo;
	unsigned char bChange;
	
	void *buf = NULL;
	char *pbuf = NULL;
	char *cmp_buf = NULL;
	unsigned int erase_size;
	DG_UPDATE_IMAGE_HEAD_S image_hdr;


	static struct spi_flash *flash = NULL;
	unsigned int bus = 2;//CONFIG_SF_DEFAULT_BUS;
	unsigned int cs = CONFIG_SF_DEFAULT_CS;
	unsigned int speed = CONFIG_SF_DEFAULT_SPEED;
	unsigned int mode = CONFIG_SF_DEFAULT_MODE;

	#if 0 //CONFIG_DM_SPI_FLASH
	struct udevice *new, *bus_dev;
	int ret = spi_find_bus_and_cs(bus, cs, &bus_dev, &new);
	if (!ret) {
		device_remove(new, DM_REMOVE_NORMAL);
	}
	flash = NULL;
	ret = spi_flash_probe_bus_cs(bus, cs, speed, mode, &new);
	if (ret) {
		printf("Failed to initialize SPI flash at %u:%u (error %d)\n",bus, cs, ret);
		return -1;
	}

	flash = dev_get_uclass_priv(new);
	#else
	flash = spi_flash_probe(bus, cs, speed, mode);
	if (!flash) {
		printf("Failed to initialize SPI flash at %u:%u\n", bus, cs);
		return -1;
	}
	#endif

	erase_size = flash->sector_size*16;
	printf("flash name         : %s\n", flash->name);
	printf("flash size         : %u\n", flash->size);
	printf("flash page size    : %u\n", flash->page_size);
	printf("flash sector size  : %u\n", flash->sector_size);
	printf("flash erase size   : %u\n", erase_size);

	cmp_buf = (char *)malloc(erase_size);
	if( NULL == cmp_buf ) {
		printf("cmp_buf failed\n");
		goto err_ext;
	}

	buf = map_physmem((unsigned long)LOAD_ADDR, sz, MAP_WRBACK);
	if (!buf) {
		puts("Failed to map physical memory\n");
		goto err_ext;
	}

	pkg_offset = sizeof(DG_UPDATE_PACKAGE_HEAD_S);

	while( pkg_offset < sz ) {

		bChange = 0;
		memcpy(&image_hdr, buf + pkg_offset, sizeof(DG_UPDATE_IMAGE_HEAD_S));

		unsigned int image_type = ntohl(image_hdr.image_type);
		unsigned int image_size = ntohl(image_hdr.image_size);
		unsigned int start_addr = ntohl(image_hdr.start_addr);

		if (DG_UPGRADE_IMAGE_TYPE_ENV == image_type) {
			printf("image type : env\n");
		}
		else if (DG_UPGRADE_IMAGE_TYPE_IDBLOCK == image_type) {
			printf("image type : idblock\n");
		}
		else if (DG_UPGRADE_IMAGE_TYPE_UBOOT == image_type) {
			printf("image type : uboot\n");
		}
		else if (DG_UPGRADE_IMAGE_TYPE_KERNEL == image_type) {
			printf("image type : kernel\n");
		}
		else if (DG_UPGRADE_IMAGE_TYPE_ROOTFS == image_type) {
			printf("image type : rootfs\n");
		}
		else if (DG_UPGRADE_IMAGE_TYPE_OEM == image_type) {
			printf("image type : oem\n");
		}
		else {
			printf("image type : unknown\n");
		}

		printf("start addr : 0x%x\n", start_addr);
		printf("image size : 0x%x\n", image_size);
		printf("pkg offset : 0x%lx\n", pkg_offset);

		pbuf = (buf + pkg_offset + sizeof(image_hdr));

		for( offset = 0; offset < image_size; offset += todo ) {

			todo = xmin(image_size-offset, erase_size);
			rc = spi_flash_read(flash, start_addr+offset, erase_size, cmp_buf);
			if( rc ) {
				printf("SPI flash read failed, return %d\n", rc);
				goto err_ext;
			}

			if( memcmp(pbuf+offset, cmp_buf, todo) != 0 ) {

				bChange = 1;
				/* erase the address range. */
				rc = spi_flash_erase(flash, start_addr+offset, erase_size);
				if (rc) {
					printf("SPI flash sector erase failed\n");
					goto err_ext;
				}
				printf(".");
				/* copy the data from RAM to FLASH */
				if( todo < erase_size ) {
					memcpy(cmp_buf, pbuf+offset, todo);
					rc = spi_flash_write(flash, start_addr+offset, erase_size, cmp_buf);
					if (rc) {
						printf("SPI flash write failed, return %d\n", rc);
						goto err_ext;
					}
				}
				else {
					rc = spi_flash_write(flash, start_addr+offset, todo, pbuf+offset);
					if (rc) {
						printf("SPI flash write failed, return %d\n", rc);
						goto err_ext;
					}
				}
			}
		}
		if (1 == bChange) {
			printf("\nimage update finish\n");
		}
		else {
			printf("image data is same\n");
		}
		pkg_offset += sizeof(image_hdr) + image_size;
		printf("\n");

		if( (DG_UPGRADE_IMAGE_TYPE_UBOOT == ntohl(image_hdr.image_type)) && (1 == bChange) ) {
			result = 1;
			printf("\n\nIf u-boot had been updated, we need to reset\n\n");
			break;
		}
		else if( (DG_UPGRADE_IMAGE_TYPE_ENV == ntohl(image_hdr.image_type)) && (1 == bChange) ) {
			result = 1;
			printf("\n\nIf env had been updated, we need to reset\n\n");
			break;
		}
		else if( (DG_UPGRADE_IMAGE_TYPE_IDBLOCK == ntohl(image_hdr.image_type)) && (1 == bChange) ) {
			result = 1;
			printf("\n\nIf idblock had been updated, we need to reset\n\n");
			break;
		}
	}
	
err_ext:
	if( cmp_buf )
		free(cmp_buf);

	if( buf )
		unmap_physmem(buf, sz);

	return result;
}
#endif
/*
 * If none of the update file(u-boot, kernel or rootfs) was found
 * in the medium, return -1;
 * If u-boot has been updated, return 1;
 * Others, return 0;
 */
static int DG_update_to_flash(void)
{
	long sz = 0;
	int ret = 0;

	/* just loop thru all the possible files */
	/* just read the header */
	int dev, part;
	struct blk_desc *dev_desc;
	disk_partition_t info;

	part = blk_get_device_part_str("mmc", "1", &dev_desc, &info, 1);
	if (part < 0)
		return -1;

	dev = dev_desc->devnum;
	if (fat_set_blk_dev(dev_desc, &info) != 0) {
		printf("\n** Unable to use %s %d:%d for fatinfo **\n","mmc", dev, part);
		return -1;
	}
	
	if (file_fat_detectfs() != 0) {
		printf("file_fat_detectfs failed\n");
		return -1;
	}

	sz = file_fat_read(DG_UPDATE_PACKAGE_NAME, LOAD_ADDR,sizeof(DG_UPDATE_PACKAGE_HEAD_S));
	printf("read %s sz %ld hdr %d\n", DG_UPDATE_PACKAGE_NAME, sz, sizeof(DG_UPDATE_PACKAGE_HEAD_S));
	if (sz <= 0 || sz < sizeof(DG_UPDATE_PACKAGE_HEAD_S)) {
		printf("%s not found\n", DG_UPDATE_PACKAGE_NAME);
		return -1;
	}

	if (DG_check_header_valid(sz) < 0) {
		printf("%s header not valid\n", DG_UPDATE_PACKAGE_NAME);
		return -1;
	}

	sz = file_fat_read(DG_UPDATE_PACKAGE_NAME, LOAD_ADDR, DG_UPDATE_PACKAGE_MAX_SIZE);
	printf("read %s sz %ld hdr %d\n", DG_UPDATE_PACKAGE_NAME, sz, sizeof(DG_UPDATE_PACKAGE_HEAD_S));
	if (sz <= 0 || sz <= sizeof(DG_UPDATE_PACKAGE_HEAD_S)) {
		printf("%s not found\n", DG_UPDATE_PACKAGE_NAME);
		return -1;
	}

	if (DG_check_cksum_valid(sz) < 0) {
		printf("%s checksum not valid\n", DG_UPDATE_PACKAGE_NAME);
		return -1;
	}

#ifdef DG_SPI_NOR_BLK
	ret = DG_do_update_blk(sz);
#endif
#ifdef DG_SPI_NOR_SF
	ret = DG_do_update_sf(sz);
#endif
	printf("DG_do_update %d\n",ret);
	return ret;
}

int DG_do_auto_update_from_sd(void)
{
	int state = -1;
	// disable_ctrlc(1);
	// clear_ctrlc();
	state = DG_update_to_flash();
	printf("DG_update_to_flash %d\n",state);
	// disable_ctrlc(0);

	if (1 == state) {
		do_reset(NULL, 0, 0, NULL);
	}

	return 0;
}
