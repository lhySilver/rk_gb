#include <linux/i2c-dev.h>
#include <linux/i2c.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "PAL/MW_Common.h"
#include "PAL/I2c.h"

int I2cOpen(I2cHndl *hndl, Uint8 instId)
{
    char deviceName[20];

    sprintf(deviceName, "/dev/i2c-%d", instId);

    hndl->fd = open(deviceName, O_RDWR);

    if(hndl->fd<0)
        return -1;

    return 0;
}

int I2cRead8(I2cHndl *hndl, Uint16 devAddr, Uint8 *reg, Uint8 *value, Uint32 count)
{
    int i, j;
    int status = 0;
    struct i2c_msg * msgs = NULL;
    struct i2c_rdwr_ioctl_data data;

    msgs = (struct i2c_msg *) malloc(sizeof(struct i2c_msg) * count * 2);

    if(msgs==NULL)
    {
        EMSG(" I2C (0x%02x): Malloc ERROR during Read !!! (reg[0x%02x], count = %d)\n", devAddr, reg[0], count);
        return -1;
    }

    for (i = 0, j = 0; i < count * 2; i+=2, j++)
    {
        msgs[i].addr  = devAddr;
        msgs[i].flags = 0;
        msgs[i].len   = 1;
        msgs[i].buf   = &reg[j];

        msgs[i+1].addr  = devAddr;
        msgs[i+1].flags = I2C_M_RD /* | I2C_M_REV_DIR_ADDR */;
        msgs[i+1].len   = 1;
        msgs[i+1].buf   = &value[j];
    }

    data.msgs = msgs;
    data.nmsgs = count * 2;

    status = ioctl(hndl->fd, I2C_RDWR, &data);
    if(status < 0)
    {
        status = -1;
    }
    else
        status = 0;

    free(msgs);

    return status;
}

int I2cWrite8(I2cHndl *hndl, Uint16 devAddr,  Uint8 *reg, Uint8 *value, Uint32 count)
{
    int i,j;
    unsigned char * bufAddr;
    int status = 0;

    struct i2c_msg * msgs = NULL;
    struct i2c_rdwr_ioctl_data data;

    msgs = (struct i2c_msg *) malloc(sizeof(struct i2c_msg) * count);

    if(msgs==NULL)
    {
        EMSG(" I2C (0x%02x): Malloc ERROR during Write !!! (reg[0x%02x], count = %d)\n", devAddr, reg[0], count);
        return -1;
    }

    bufAddr = (unsigned char *) malloc(sizeof(unsigned char) * count * 2);

    if(bufAddr == NULL)
    {
        free(msgs);

        EMSG(" I2C (0x%02x): Malloc ERROR during Write !!! (reg[0x%02x], count = %d)\n", devAddr, reg[0], count);
        return -1;

    }

    for (i = 0, j = 0; i < count; i++, j+=2)
    {
        bufAddr[j] = reg[i];
        bufAddr[j + 1] = value[i];

        msgs[i].addr  = devAddr;
        msgs[i].flags = 0;
        msgs[i].len   = 2;
        msgs[i].buf   = &bufAddr[j];
    }
    data.msgs = msgs;
    data.nmsgs = count;

    status = ioctl(hndl->fd, I2C_RDWR, &data);
    if(status < 0)
    {
        status = -1;
    }
    else
        status = 0;

    free(msgs);
    free(bufAddr);

    return status;
}

int I2cRawWrite8(I2cHndl *hndl, Uint16 devAddr, Uint8 reg,Uint8 *value, Uint32 count)
{
#if 1
    int status = 0;
	unsigned char *outbuf = (unsigned char *)malloc(sizeof(unsigned char)*(count+1));  
    if(outbuf==NULL)  
    {  
        EMSG("MALLOC\n");  
        return -1;  
    }  
    struct i2c_msg msgs[1];
    struct i2c_rdwr_ioctl_data data;

	/* The first byte indicates which register we'll write */  
    outbuf[0] = reg; 
	memcpy(outbuf+1, value, count); 

    msgs[0].addr  =  devAddr;
    msgs[0].flags = 0;
    msgs[0].len   = count+1;
    msgs[0].buf   = outbuf;

    data.msgs = msgs;
    data.nmsgs = 1;

    status = ioctl(hndl->fd, I2C_RDWR, &data);
    if(status < 0)
    {
        status = -1;
    }
    else
        status = 0;

	free(outbuf);

    return status;
#else
	
	int status = 0;
	struct i2c_rdwr_ioctl_data msg_rdwr;
	struct i2c_msg i2cmsg;

	msg_rdwr.msgs = &i2cmsg;
	msg_rdwr.nmsgs = 1;
	/* the i2c slave device address is  0x50 */
	i2cmsg.addr = 0x50;
	/* read 32 bytes */
	i2cmsg.flags = 0;
	i2cmsg.len = count;
	i2cmsg.buf = value;

	status = ioctl(hndl->fd, I2C_RDWR, &msg_rdwr);
	if(status < 0)
	{
		status = -1;
	}
	else
		status = 0;

	return status;
#endif
}

int I2cRawRead8(I2cHndl *hndl, Uint16 devAddr, Uint8 reg,Uint8 *value, Uint32 count)
{
#if 1
    int status = 0;
	unsigned char outbuf;  
    struct i2c_msg msgs[2];
    struct i2c_rdwr_ioctl_data data;
	outbuf = reg;
    msgs[0].addr  = devAddr;
    msgs[0].flags = 0;
    msgs[0].len   = sizeof(outbuf);;
    msgs[0].buf   = &outbuf;

	msgs[1].addr  = devAddr;
    msgs[1].flags = I2C_M_RD;
    msgs[1].len   = count;
    msgs[1].buf   = value;

    data.msgs = msgs;
    data.nmsgs = 2;

    status = ioctl(hndl->fd, I2C_RDWR, &data);
    if(status < 0)
    {
        status = -1;
    }
    else
        status = 0;

    return status;
#else
	int status = 0;
	struct i2c_rdwr_ioctl_data msg_rdwr;
	struct i2c_msg i2cmsg;

	msg_rdwr.msgs = &i2cmsg;
	msg_rdwr.nmsgs = 1;
	/* the i2c slave device address is  0x50 */
	i2cmsg.addr = devAddr;
	/* read 32 bytes */
	i2cmsg.flags = I2C_M_RD;
	i2cmsg.len = count;
	i2cmsg.buf = value;

	status = ioctl(hndl->fd, I2C_RDWR, &msg_rdwr);
	if(status < 0)
	{
		status = -1;
	}
	else
		status = 0;

	return status;
#endif
}

int I2cClose(I2cHndl *hndl)
{
    return close(hndl->fd);
}
