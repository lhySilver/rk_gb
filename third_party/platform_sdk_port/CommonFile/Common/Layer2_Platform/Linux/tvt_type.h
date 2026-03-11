#ifndef _TVT_TYPE_DEFINE_HH_
#define _TVT_TYPE_DEFINE_HH_


//
// 基本类型, 不能修改       
//
typedef signed char  	tchar, tint8;			// 8  bit signed
typedef unsigned char  	tbyte, tuchar, tuint8;  // 8  bit unsigned
typedef short          	tint16; 				// 16 bit signed
typedef unsigned short  tuint16; 				// 16 bit unsigned
typedef int    			tint32; 				// 32 bit signed
typedef unsigned int  	tuint32; 				// 32 bit unsigned
typedef long long    	tint64; 				// 64 bit signed
typedef unsigned long long   tuint64; 			// 64 bit unsigned



#if defined(OS_IS_64BIT)
    typedef tuint64     tuint_ptr;
    typedef tint64      tint_ptr;
    
    #define WINAPI
#else
    typedef tuint32     tuint_ptr;
    typedef tint32      tint_ptr;
    
    #define WINAPI      __attribute__((stdcall))
#endif //


//****
//扩展类型,  不能随便添加
//****
typedef int 				 BOOL;
typedef tuint_ptr            HANDLE;
typedef void *               HWND;
typedef void               * LPVOID, *HDC, VOID;
typedef tuint64              ULONGLONG;
typedef tint64               LONGLONG;
typedef tuint32              COLORREF;
typedef tuint8               BYTE;

#endif