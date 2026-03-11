/*********************************************************************
* 版权所有 (C)2017, 深圳市同为视频技术有限公司。
* 
* 文件名称： // TVTIAInterface.h
* 内容摘要： // Defines the exported functions for the DLL application.
* 当前版本： // 1.0.0.1
* 完成日期： // 2017年9月15日
* 与对应lib使用
* 使用顺序： 人脸检测-->人脸特征检测-->人脸识别
* OMRON算法, deprecated
**********************************************************************/
#if 0

#ifndef TVT_IA_INTERFACE__H
#define TVT_IA_INTERFACE__H

#include "TVTIAHeader.h"

//////////////////////////////////////////////////////////////////////////
//                            tic   toc
//////////////////////////////////////////////////////////////////////////
TVT_IA_LIB void __stdcall TVT_IA_Tic();
TVT_IA_LIB void __stdcall TVT_IA_Toc();


//////////////////////////////////////////////////////////////////////////
//                          图片转为灰度图
//////////////////////////////////////////////////////////////////////////
TVT_IA_LIB bool __stdcall TVT_IA_ConvertImageToGray(const char* path,       /* input image path */
                                                    GrayFormat& grayImage); /* output */

TVT_IA_LIB bool __stdcall TVT_IA_ConvertYUV422ToGray(tuint8*  yuvData,    /* input */
                                           tint32   nWidth,     /* input */
                                           tint32   nHeight,    /* input */    
                                           GrayFormat&  pGray); /* output */

TVT_IA_LIB bool __stdcall TVT_IA_ReadJpgFromMemory(const tint32& width, const tint32& height, 
                                                   const tuint8* data,
                                                   GrayFormat& pGray, bool show = false);

//////////////////////////////////////////////////////////////////////////
//                         人脸检测导出函数， 需按顺序调用
//////////////////////////////////////////////////////////////////////////
// 初始化
TVT_IA_LIB TVT_FaceDetectHandle __stdcall TVT_IA_FaceDetect_CreateHandle();

/** 设置一次可检测的最大人脸数量
*   nMaxDetectNum: 0--1023
**/
TVT_IA_LIB bool __stdcall TVT_IA_FaceDetect_MaxNum(TVT_FaceDetectHandle pHandle, 
                                                   int nMaxDetectNum = 1000);

// 设置检测参数/默认参数 2选1, 参数定义： TVT_FaceDetect_Param
TVT_IA_LIB bool __stdcall TVT_IA_FaceDetect_Set_Parameter(TVT_FaceDetectHandle pHandle, FaceDetectParam sParam);
TVT_IA_LIB bool __stdcall TVT_IA_FaceDetect_Set_Default_Parametr(TVT_FaceDetectHandle pHandle, TVT_FaceDetectMode mode); 

// 获取人脸识别参数

TVT_IA_LIB bool __stdcall TVT_IA_FaceDetect_Get_Parameter(TVT_FaceDetectHandle pHandle,
                                                          FaceDetectParam& sResult);

/** 传入待检测图片
*   sGrayImage: 传入的灰度图
*/
TVT_IA_LIB bool __stdcall TVT_IA_FaceDetect(TVT_FaceDetectHandle pHandle,
                                            const GrayFormat& sGrayImage);

/** 获取检测结果
*   nFaceCountResult: 检测到的人脸数量
*   返回值： 指向人脸数据的数组
*/
TVT_IA_LIB bool __stdcall TVT_IA_FaceDetect_Result(TVT_FaceDetectHandle pHandle,
                                                   tint32& nFaceCountResult,         /* output */
                                                   FaceDetectResult* & pFaceResult); /* output */
// 清理
TVT_IA_LIB bool __stdcall TVT_IA_FaceDetect_Cleanup(TVT_FaceDetectHandle pHandle);


//////////////////////////////////////////////////////////////////////////
//                      面部特征检测
//////////////////////////////////////////////////////////////////////////
// 创建面部特征检测句柄
TVT_IA_LIB TVT_FacialDetectHandle __stdcall TVT_IA_FacialDetect_CreateHandle();

/** 检测
*   GrayFormat: 待检测图片的灰度图
*   FaceDetectResult： 人脸检测检测出的人脸的位置
**/
TVT_IA_LIB bool __stdcall TVT_IA_FacialDetect(TVT_FacialDetectHandle pHandle,
                                              const GrayFormat& sGrayImage, 
                                              const FaceDetectResult& sFaceInfo);

/** 获取各面部特征的位置 数组
    PT_POINT_LEFT_EYE = 0,   Center of left eye        
    PT_POINT_RIGHT_EYE,      Center of right eye       
    PT_POINT_MOUTH,          Mouth Center              
    PT_POINT_LEFT_EYE_IN,    Inner corner of left eye  
    PT_POINT_LEFT_EYE_OUT,   Outer corner of left eye  
    PT_POINT_RIGHT_EYE_IN,   Inner corner of right eye 
    PT_POINT_RIGHT_EYE_OUT,  Outer corner of right eye 
    PT_POINT_MOUTH_LEFT,     Left corner of mouth      
    PT_POINT_MOUTH_RIGHT,    Right corner of mouth     
    PT_POINT_NOSE_LEFT,      Left Nostril              
    PT_POINT_NOSE_RIGHT,     Right Nostril             
    PT_POINT_MOUTH_UP,       Mouth top                   
**/
TVT_IA_LIB bool __stdcall TVT_IA_Get_FacialParts(TVT_FacialDetectHandle pHandle, 
                                                   tint32&  nPartsCountResult,  // output
                                                   POINT* & pPartsResult,       // output
                                                   tint32*& pPartsConfidence);  // output

/** 获取面部角度 
*   
**/
TVT_IA_LIB bool __stdcall TVT_IA_Get_FaceAngle(TVT_FacialDetectHandle pHandle,
                                               tint32& nUpDownAngle,    // output
                                               tint32& nLeftRightAngle, // output
                                               tint32& nRollAngle);     // output


TVT_IA_LIB bool __stdcall TVT_IA_FacialDetect_Cleanup(TVT_FacialDetectHandle pHandle);

//////////////////////////////////////////////////////////////////////////
//                      人脸识别
//////////////////////////////////////////////////////////////////////////
// 创建人脸识别句柄
TVT_IA_LIB TVT_FaceRecognizeHandle __stdcall TVT_IA_FaceRecognize_CreateHandle();

// 读取album, 从文本中  或者  从二进制中,   2选1使用
TVT_IA_LIB bool __stdcall TVT_IA_FaceRecognize_LoadAlbum(TVT_FaceRecognizeHandle pHandle,
                                                         const char* pAlbumFilePath);

TVT_IA_LIB bool __stdcall TVT_IA_FaceRecognize_LoadAlbum(TVT_FaceRecognizeHandle pHandle, 
                                                         const tuint8* pBytes,
                                                         const tint32& nByteLen);

/** 在目标库中查找与输入图像相似的 最多nMaxIdentifyCount个目标
*   nMaxIdentifyCount:  在库中最多找出相似的数量
*
*   nFacialPartsCount： facial parts detect的结果
*   pFacialParts
*   pFacialPartsConfidence
*
**/
TVT_IA_LIB bool __stdcall TVT_IA_FaceRecognize_Identify(TVT_FaceRecognizeHandle pHandle,
                                                        const GrayFormat& sGrayImage,   
                                                        tint32 nMaxIdentifyCount,       
                                                        tint32 nFacialPartsCount,
                                                        POINT* pFacialParts, 
                                                        tint32* pFacialPartsConfidence);

// 获取识别出来的数量
TVT_IA_LIB tint32 __stdcall TVT_IA_FaceRecognize_GetIdentifyCount(TVT_FaceRecognizeHandle pHandle);

// 获取第nIndex个结果
TVT_IA_LIB FaceRecognizeResult TVT_IA_FaceRecognize_GetIdentifyResult(TVT_FaceRecognizeHandle pHandle, 
                                                                     tint32 nIndex);

/** 查找所给出的图像是与目标库中指定目标的相似程度
*   nMaxIdentifyCount:  在库中最多找出相似的数量
*
*   nFacialPartsCount： facial parts detect的结果
*   pFacialParts
*   pFacialPartsConfidence
**/
TVT_IA_LIB bool __stdcall TVT_IA_FaceRecognize_Verify(TVT_FaceRecognizeHandle pHandle,
                                                      const GrayFormat& sGrayImage, 
                                                      tint32 nUserID,
                                                      tint32 nFacialPartsCount,
                                                      POINT* pFacialParts, 
                                                      tint32* pFacialPartsConfidence);

// 获取相似结果
TVT_IA_LIB FaceRecognizeResult __stdcall TVT_IA_FaceRecognize_GetVerifyResult(TVT_FaceRecognizeHandle pHandle);


TVT_IA_LIB bool __stdcall TVT_IA_FaceRecognize_Cleanup(TVT_FaceRecognizeHandle pHandle);

//////////////////////////////////////////////////////////////////////////
//                          创建人脸识别样本                             //
//////////////////////////////////////////////////////////////////////////
TVT_IA_LIB TVT_AlbumHandle __stdcall TVT_IA_Album_CreateHandle();

/** 初始化 
*   albumPath: 生成的album所在的路径
*   isAppend: 是否在原album的基础上增加
**/
TVT_IA_LIB bool __stdcall TVT_IA_Album_initialize(TVT_AlbumHandle pHandle,
                                                  const char* albumPath,
                                                  bool isAppend=false);

/** 生成album
*   nUserID： albumn中目标的id
*   nDataCount： album中目标所包含的图片数量
*   pDataGrays： album中目标所包含的图片的基础信息
**/
TVT_IA_LIB bool __stdcall TVT_IA_Create_Album(TVT_AlbumHandle pHandle,
                                              tuint32 nUserID,
                                              tuint32 nDataCount,
                                              GrayFormat* pDataGrays);

TVT_IA_LIB bool __stdcall TVT_IA_Delete_Album_Target(TVT_AlbumHandle pHandle,
											  tuint32 nUserID);
/** 向指定目标库的目标中添加图片
*   nUserID： albumn中目标的id
*   nDataCount： album中目标所包含的图片数量
*   pDataGrays： album中目标所包含的图片的基础信息
**/
TVT_IA_LIB bool __stdcall TVT_IA_Add_Album_Taregt(TVT_AlbumHandle pHandle,
                                                  tuint32 nUserID,
                                                  tuint32 nDataCount,
                                                  GrayFormat* pDataGrays);


TVT_IA_LIB bool __stdcall TVT_IA_GetUserCount(TVT_AlbumHandle pHandle, tuint32* count);

TVT_IA_LIB bool __stdcall TVT_IA_GetUserDataCount(TVT_AlbumHandle pHandle, const tuint32& userId, tuint32* count);

TVT_IA_LIB bool __stdcall TVT_IA_Album_Cleanup(TVT_AlbumHandle pHandle);

#endif

#endif