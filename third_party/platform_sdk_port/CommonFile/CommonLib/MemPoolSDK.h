
///////////////////////////MemPoolSDK.h

#ifndef _MEM_POOL_SDK_H_
#define _MEM_POOL_SDK_H_

#include "base_type_define.h"
#include "CustomContainer.h"

#ifdef WIN32

#include <typeinfo.h>

#ifdef MEM_POOL_SDK_EXPORT_API//动态库编译
#define _MEM_POOL_SDK  __declspec(dllexport)
#else
#ifdef MEM_POOL_SDK_EXPORT_STATIC//静态库编译和使用
#define _MEM_POOL_SDK 
#else//使用者编译
#define _MEM_POOL_SDK  __declspec(dllimport)
#endif
#endif

#else
#define _MEM_POOL_SDK __attribute__ ((visibility("default")))
#endif


//
_MEM_POOL_SDK bool MEM_POOL_Initial(tuint32 memM);

//
_MEM_POOL_SDK void MEM_POOL_Quit();

_MEM_POOL_SDK tuint_ptr MEM_POOL_New(const void *pDataBuf, tint32 nDataLen, tint32 nMemLen = 0);  ////分配,nMemLen为0时按实际大小占用内存,否则按nMemLen对齐占用内存
_MEM_POOL_SDK tuint_ptr MEM_POOL_NewCopyFrom(tuint_ptr hMem);  ////分配
_MEM_POOL_SDK void MEM_POOL_Delete(tuint_ptr hMem);  ////释放

_MEM_POOL_SDK bool MEM_POOL_Prepend(tuint_ptr hMem, const void *pDataBuf, tint32 nDataLen);	 ////前追加
_MEM_POOL_SDK bool MEM_POOL_Appending(tuint_ptr hMem, const void *pDataBuf, tint32 nDataLen, tint32 nMemLen = 0);	 ////后追加,nMemLen为0时按实际大小占用内存,否则按nMemLen对齐占用内存
_MEM_POOL_SDK bool MEM_POOL_Prepend(tuint_ptr hMem, const tuint32 hMemAppended);	 ////前追加
_MEM_POOL_SDK bool MEM_POOL_Appending(tuint_ptr hMem, const tuint32 hMemAppended);	 ////后追加

_MEM_POOL_SDK tint32 MEM_POOL_AddReference(tuint_ptr hMem, tint32 nValue = 1);
_MEM_POOL_SDK tint32 MEM_POOL_DecReference(tuint_ptr hMem, tint32 nValue = 1);	

_MEM_POOL_SDK tint32 MEM_POOL_GetLength(tuint_ptr hMem);
_MEM_POOL_SDK tint32 MEM_POOL_Memcpy(tuint_ptr hMem, BYTE *pDstBuf, tint32 DstBufLen);	
_MEM_POOL_SDK tint32 MEM_POOL_Memcpy(tuint_ptr hMem, tint32 offset,BYTE *pDstBuf, tint32 DstBufLen);


_MEM_POOL_SDK bool MEM_POOL_GetContent(tuint_ptr hMem, CChildPairContainer<BYTE *, tint32> &Content); 

////////truncate
_MEM_POOL_SDK bool MEM_POOL_TruncateFront(tuint_ptr hMem, tuint32 Length);	////从0位置开始截取
_MEM_POOL_SDK bool MEM_POOL_TruncateBack(tuint_ptr hMem, tuint32 Length);///从末尾位置开始截取

//NetSocket使用
_MEM_POOL_SDK tuint_ptr MEM_POOL_NewNoData(LPVOID &pDataBuf, tint32 &nBufLen);  ////分配一个内部块大小的空内容BUF
_MEM_POOL_SDK bool MEM_POOL_SetDataLen(tuint_ptr hMem, tuint32 Length);  ////分配一个内部块大小的空内容BUF

#endif    /////_MEM_POOL_SDK_H_



