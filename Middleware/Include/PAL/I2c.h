#ifndef __I2C_H__
#define __I2C_H__

typedef unsigned long long Uint64;
typedef unsigned int Uint32;
typedef unsigned short Uint16;
typedef unsigned char Uint8;

#define I2C_DEFAULT_INST_ID  (0)

#define I2C_TRANSFER_SIZE_MAX   (254)

typedef struct 
{
  int fd;
}I2cHndl;

int I2cOpen(I2cHndl *hndl, Uint8 instId);
int I2cRead8(I2cHndl *hndl, Uint16 devAddr, Uint8 *reg, Uint8 *value, Uint32 count);
int I2cWrite8(I2cHndl *hndl, Uint16 devAddr, Uint8 *reg, Uint8 *value, Uint32 count);
//int I2cRawWrite8(I2cHndl *hndl, Uint16 devAddr, Uint8 *value, Uint32 count);
//int I2cRawRead8(I2cHndl *hndl, Uint16 devAddr, Uint8 *value, Uint32 count);
int I2cRawWrite8(I2cHndl *hndl, Uint16 devAddr, Uint8 reg,Uint8 *value, Uint32 count);
int I2cRawRead8(I2cHndl *hndl, Uint16 devAddr, Uint8 reg,Uint8 *value, Uint32 count);

int I2cClose(I2cHndl *hndl);

#endif

