/******************************************************************************

  Copyright (C), 2008-2018, Hisilicon Tech. Co., Ltd.
  ******************************************************************************
  File Name     : hi_mjpeg_api.h
  Version       : Initial Draft
  Author        : Hisilicon multimedia
  Created       : 2008/02/21
  Description   : for system  compatibility
  History       :
  1.Date        : 2008/02/21
  Author        : f65132
  Modification  : Created file
******************************************************************************/


#ifndef __HI_MJPEG_API__
#define __HI_MJPEG_API__


#define  HI_MJPEG_DEC_OK          0
#define  HI_MJPEG_NO_PICTURE      -1
#define  HI_MJPEG_ERR_HANDLE      -2

#ifdef __cplusplus
extern "C" {
#endif

typedef void* HI_HANDLE;

typedef struct hiMJPEG_USERDATA_S
{
    HI_U32             uUserDataType;    //Type of userdata
    HI_U32             uUserDataSize;    //Length of userdata in byte
    HI_UCHAR*          pData;            //Buffer contains userdata stuff
    struct hiMJPEG_USERDATA_S* pNext;    //Pointer to next userdata
} MJPEG_USERDATA_S;

typedef struct hiMJPEG_DEC_FRAME_S
{
    HI_U8 *pY;
    HI_U8 *pU;
    HI_U8 *pV;
    HI_U32 uWidth;
    HI_U32 uHeight;    
    HI_U32 uYStride;
    HI_U32 uCStride;

    //HI_S32 cWidth;
    //HI_S32 cHeight;
    HI_S32 uPictureFormat;       // 0: YUV420;  
                                 // 1: YUV422; 
                                 // 2: YUV444; 
                                 // 3: YUV422V; 
                                 // 4: YUV400; 
                                 // >=5: reserved
    HI_S32 bError;               // 0: No error
                                 // 1: MCU error
    HI_U64 ullPTS;               // time stamp
    HI_U32 reserved;             // reserved for future
    HI_S32 ErrorCode;            // reserved
    MJPEG_USERDATA_S *pUserData; //Pointer to the first userdata    
} MJPEG_DEC_FRAME_S;

/**********************************************************************
* This data structure describes the build information
* and frame markers set of decoder library.
***********************************************************************/
typedef struct hiMJPEG_LIBINFO_S
{
    HI_U32  uMajor;              // Major Version
    HI_U32  uMinor;              // Minor Version
    HI_U32  uRelease;            // Release Version 
    HI_U32  uBuild;              // Build Version
    const HI_CHAR*   sVersion;   // Version information
    const HI_CHAR*   sCopyRight; // CopyRight information

    HI_U32  uPictureFormat;      // Picture format support 
                                 // bit0: YUV420
                                 // bit1: YUV422
                                 // bit2: YUV444
                                 // bit3: YUV422V
                                 // bit4: YUV400
                                 // bit5~bit31: reserved
    HI_U32  uFrameMarkersSet;    // Frame markers set of the decode library
                                 // bit0:  SOF0   baseline DCT, huffman
                                 // bit1:  SOF1   extended sequential DCT, huffman
                                 // bit2:  SOF2   progressive DCT, huffman
                                 // bit3:  SOF3   lossless (sequential), huffman
                                 // bit4:  reserved 
                                 // bit5:  SOF5   differential sequential DCT, huffman
                                 // bit6:  SOF6   differential progressive DCT, huffman
                                 // bit7:  SOF7   differential lossless (sequential), huffman
                                 // bit8:  JPG    JPEG extensions
                                 // bit9:  SOF9   extended sequential DCT, arithmetic
                                 // bit10: SOF10  progressive DCT, arithmetic
                                 // bit11: SOF11  lossless (sequential), arithmetic
                                 // bit12: reserved
                                 // bit13: SOF13  differential sequential DCT, arithmetic
                                 // bit14: SOF14  differential progressive DCT, arithmetic
                                 // bit15: SOF15  differential lossless (sequential), arithmetic
                                 // bit16 ~ bit31 reserved

    HI_U32  uStreamInType;       // Input stream type(reserved)
    HI_U32  uPicWidth;           // The max Width of picture in pixel
    HI_U32  uPicHeight;          // The max Height of picture in pixel  
    HI_U32  uReserved;           // Reserved
} MJPEG_LIBINFO_S;

/***************************************************************************
 * This data structure describes the attributes during the decoding process.
****************************************************************************/
typedef struct hiMJPEG_DEC_ATTR_S
{
    HI_U32  uPictureFormat;       // Decoded output picture format (reserved)
    HI_U32  uStreamInType;        // Input stream type (reserved)
    HI_U32  uPicWidth;            // The width of picture
    HI_U32  uPicHeight;           // The height of picture
    HI_U32  uWorkMode;            // reserved
    MJPEG_USERDATA_S  *pUserData; //Buffer contains userdata stuff
    HI_U32  uReserved;
} MJPEG_DEC_ATTR_S;


/*********************************************************************
* Function Name  : HiMJPEGDecCreate
* Description    : Create and initialize MJPEG decoder handle
* Parameters     : pDecAttr:   a pointer referring to MJPEG_DEC_ATTR_S 
*                              which contain the needed  attributes to 
*                              initialize the decoder
* Return Type    : if success, return a decoder handle; 
*                  otherwise,  return NULL. 
* Last Modified  : 2008-2-21
*********************************************************************/
HI_HANDLE HI_DLLEXPORT HiMJPEGDecCreate( MJPEG_DEC_ATTR_S *pDecAttr );


/***************************************************************** 
* Function Name  : HiMJPEGDecDestroy
* Description    : destroy MJPEG decoder handle
* Parameters     : hDec:  decoder handle created by HiMJPEGDecCreat
* Return Type    : none
* Last Modified  : 2008-2-21
*****************************************************************/
void HI_DLLEXPORT HiMJPEGDecDestroy( HI_HANDLE hDec );


/**************************************************************************** 
* Function Name : HiMJPEGDecGetInfo
* Description   : get the build and owner infomation of this function library
* Parameters    : pLibInfo
* Return Type   : if success, return 0; 
*                 otherwise,  return -1.
* Last Modified : 2008-2-21
****************************************************************************/
HI_S32 HI_DLLEXPORT HiMJPEGDecGetInfo( MJPEG_LIBINFO_S *pLibInfo );

/********************************************************************************* 
* Function Name  : HiMJPEGDecFrame
* Description    : input stream and decode (single-thread version)
* Parameters     : hDec       : decoder handle created by HiMJPEGDecCreat
*                : pStream    : stream buffer 
*                : iStreamLen : stream length in byte
*                : ullPTS     : time stamp
*                : pDecFrame  : denoting whether there is a picture frame to display, 
*                               and the decoded Parameters of the picture frame 
*                               (referring to MJPEG_DEC_FRAME_S ).
*                : uFlags     : working mode (reserved) 
* Return Type    0 : HI_JPEG_DEC_OK:     success, output a decoded picture
                 -1: HI_JPEG_NO_PICTURE: error in decoding, no output picture
             -2: HI_JPEG_ERR_HANDLE: error in input parameters
* Last Modified  : 2008-2-21
**********************************************************************************/
HI_S32 HI_DLLEXPORT HiMJPEGDecFrame( 
    HI_HANDLE          hDec, 
    HI_U8             *pStream, 
    HI_U32             iStreamLen, 
    HI_U64             ullPTS,
    MJPEG_DEC_FRAME_S *pDecFrame,
    HI_U32             uFlags );

#ifdef __cplusplus
}
#endif

#endif __HI_MJPEG_API__