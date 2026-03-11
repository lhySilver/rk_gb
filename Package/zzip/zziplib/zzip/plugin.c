/*
 * Author: 
 *	Guido Draheim <guidod@gmx.de>
 *      Mike Nordell <tamlin@algonet.se>
 *
 *	Copyright (c) 2002 Guido Draheim
 * 	    All rights reserved,
 *	    use under the restrictions of the
 *	    Lesser GNU General Public License
 *          note the additional license information 
 *          that can be found in COPYING.ZZIP
 */

#include <zzip/lib.h>
#include <zzip/plugin.h>

#include <string.h>
#include <sys/stat.h>
#include <errno.h>
#include <stdlib.h>
#ifdef DEBUG
#include <stdio.h>
#endif
#include <assert.h>

#include <zzip/file.h>
#include <zzip/format.h>

#if 0
zzip_off_t
zzip_filesize(int fd)
{
  struct stat st;

  if (fstat(fd, &st) < 0)
    return -1;

# ifdef DEBUG 
  if (! st.st_size && st.st_blocks > 1) /* seen on some darwin 10.1 machines */
      fprintf(stderr, "broken fstat(2) ?? st_size=%ld st_blocks=%ld\n", 
	      (long) st.st_size, (long) st.st_blocks);
# endif

  return st.st_size;
}

#if defined ZZIP_WRAPWRAP
int		zzip_wrap_read(int fd, void* p, unsigned int len)
				{ return _zzip_read (fd, p, len); }
zzip_off_t	zzip_wrap_lseek(int fd, zzip_off_t offset, int whence)
				{ return _zzip_lseek (fd, offset, whence); }
#else
#define zzip_wrap_read  _zzip_read
#define zzip_wrap_lseek _zzip_lseek
#endif

static const struct zzip_plugin_io default_io =
{
    &open,
    &close,
    /* (int (*)(int, void*, unsigned)) */
    &zzip_wrap_read,
    /* (zzip_off_t (*)(int, zzip_off_t, int)) */
    &zzip_wrap_lseek,
    &zzip_filesize,
    1
};

/** => zzip_init_io
 * This function returns a zzip_plugin_io_t handle to static defaults
 * wrapping the posix io file functions for actual file access.
 */
zzip_plugin_io_t
zzip_get_default_io()
{
    return &default_io;
}

/**
 * This function initializes the users handler struct to default values 
 * being the posix io functions in default configured environments.
 */
int zzip_init_io(struct zzip_plugin_io* io, int flags)
{
    if (!io) {
        return ZZIP_ERROR;
    }
    memcpy(io, &default_io, sizeof(default_io));
    io->use_mmap = flags;
    return 0;
}
#endif

typedef struct {
	unsigned char * baseAddr;
	unsigned long zzip_memory_size;
	unsigned long zzip_memory_pos;
} luazip_t;

#define ZIP_FD_NUMS 100

static luazip_t *luazip_fds[ZIP_FD_NUMS];
unsigned long global_zzip_memory_size;

int luazip_open(zzip_char_t* name, unsigned long memory_size)
{
	luazip_t *myzip = NULL;
	
	int i;
	int fd = -1;
	
	/*
	 *  这里的索引值不能从0开始,不知道为什么,可能程序中其他地方存在Bug
	 */
	for( i = 1; i < ZIP_FD_NUMS; i++) {
		if(luazip_fds[i] == NULL) {
			myzip = (luazip_t *)malloc(sizeof(luazip_t));
			if(myzip) {
				luazip_fds[i] = myzip;
				fd = i;
				break;
			}
		}
	}
	
	if(myzip) {
		memset(myzip, 0, sizeof(luazip_t));
		myzip->baseAddr = (unsigned char *) name;
		myzip->zzip_memory_size = memory_size;
	}
	return fd;
}

static int luazip_close(int fd)
{
	assert((fd < ZIP_FD_NUMS) && (fd >= 0));
	
	free(luazip_fds[fd]);
	
	luazip_fds[fd] = NULL;

	return 0;
}

static zzip_ssize_t luazip_read(int fd, void* buf, zzip_size_t len)
{
	int nLen = len;
	luazip_t *myzip;
	
	assert((fd < ZIP_FD_NUMS) && (fd >= 0));
	myzip = luazip_fds[fd];

	if(myzip->zzip_memory_pos + len > myzip->zzip_memory_size) {
		nLen = myzip->zzip_memory_size - myzip->zzip_memory_pos;
	}
		
	memcpy(buf, myzip->baseAddr + myzip->zzip_memory_pos, nLen);
	myzip->zzip_memory_pos += nLen;
	return nLen;
}
static zzip_off_t   luazip_seeks(int fd, zzip_off_t offset, int whence)
{
	luazip_t *myzip;
	
	assert((fd < ZIP_FD_NUMS) && (fd >= 0));
	myzip = luazip_fds[fd];
	
	/* Change position pointer */
 	switch (whence) {
  	case SEEK_SET:
   		myzip->zzip_memory_pos = offset;
   	break;
  	case SEEK_CUR:
   		myzip->zzip_memory_pos += offset;
   		break;
  	case SEEK_END:
   		myzip->zzip_memory_pos = myzip->zzip_memory_size-1;
   		break;
 	}
 	/* Limit position to the last byte of the file */
 	if (myzip->zzip_memory_pos>(myzip->zzip_memory_size-1)) {
  		myzip->zzip_memory_pos=myzip->zzip_memory_size-1;
 	}
 	/* Return current position */ 
 	return(myzip->zzip_memory_pos);	
}

static zzip_off_t   luazip_filesize(int fd)
{
	luazip_t *myzip;
	
	assert((fd < ZIP_FD_NUMS) && (fd >= 0));
	myzip = luazip_fds[fd];

	return myzip->zzip_memory_size;
}


const struct zzip_plugin_io mem_io =
{
    NULL,
    &luazip_close,
    /* (int (*)(int, void*, unsigned)) */
    &luazip_read,
    /* (zzip_off_t (*)(int, zzip_off_t, int)) */
    &luazip_seeks,
    &luazip_filesize,
    0
};

/* 
 * Local variables:
 * c-file-style: "stroustrup"
 * End:
 */
