//
//  "$Id$"
//
//  Copyright (c)1992-2007, ZheJiang Dahua Technology Stock CO.LTD.
//  All Rights Reserved.
//
//	Description:	
//	Revisions:		Year-Month-Day  SVN-Author  Modification
//

#ifndef __MUTEX_API_H__
#define __MUTEX_API_H__

typedef void *	HANDLE;

#ifdef __cplusplus
extern "C" {
#endif


/// \defgroup MutexAPI API Mutex
/// 互斥量API，封装了不同操作系统的互斥量对象。
///	\n 调用流程图:
/// \code
///    ===========================
///                |
///           MutexCreate
///                |------------+
///      MutexEnter,MutexLeave  |
///                |------------+
///           MutexDestory
///                |
///    ===========================
/// \endcode
/// @{

/// 创建互斥量。
/// 
/// \param [out] phMutex 互斥量创建成功时，传出的互斥量句柄。
/// \param [in] nType 互斥量的类型，取mutex_type枚举类型值
/// \retval 0  创建成功
/// \retval <0  创建失败
int MutexCreate(HANDLE *phMutex, int nType);


/// 销毁互斥量。
/// 
/// \param [in] hMutex 互斥量句柄。
/// \retval 0  销毁成功
/// \retval <0 销毁失败
int MutexDestory(HANDLE hMutex);


/// 进入临界区。
/// 
/// \param [in] hMutex 互斥量句柄。
/// \retval 0  操作成功
/// \retval <0  操作失败
int MutexEnter(HANDLE hMutex);


/// 离开临界区。
/// 
/// \param [in] hMutex 互斥量句柄。
/// \retval 0  操作成功
/// \retval <0  操作失败
int MutexLeave(HANDLE hMutex);

/// @} end of group

/// \defgroup MutexAPI API Mutex
/// 互斥量API，封装了不同操作系统的互斥量对象。	-- RWMutex Operation
///	\n 调用流程图:
/// \code
///    ===========================
///                |
///           RWMutexCreate
///                |-------------------+
///      ReadMutexEnter,ReadMutexEnter  |
///                |------------+
///           RWMutexDestory
///                |
///    ===========================
/// \endcode
/// @{


/// 创建读写锁。
/// 
/// \param [out] phMutex 读写锁创建成功时，传出的读写锁句柄。
/// \retval 0  创建成功
/// \retval <0  创建失败
int RWMutexCreate(void * *phMutex);


/// 销毁读写锁。
/// 
/// \param [in] hMutex 读写锁句柄。
/// \retval 0  销毁成功
/// \retval <0 销毁失败
int RWMutexDestory(void * hMutex);


/// 进入读临界区。
/// 
/// \param [in] hMutex 读写锁句柄。
/// \retval 0  操作成功
/// \retval <0  操作失败
int ReadMutexEnter(void * hMutex);


/// 进入写临界区。
/// 
/// \param [in] hMutex 读写锁句柄。
/// \retval 0  操作成功
/// \retval <0  操作失败
int WriteMutexEnter(void * hMutex);

/// 离开读临界区。
/// 
/// \param [in] hMutex 读写锁句柄。
/// \retval 0  操作成功
/// \retval <0  操作失败
int ReadMutexLeave(void * hMutex);

/// 离开写临界区。
/// 
/// \param [in] hMutex 读写锁句柄。
/// \retval 0  操作成功
/// \retval <0  操作失败
int WriteMutexLeave(void * hMutex);

/// @} end of group

#ifdef __cplusplus
}
#endif

#endif //__MUTEX_API_H__

