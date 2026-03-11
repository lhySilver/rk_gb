#ifndef __WARNING_DISABLE_
#define __WARNING_DISABLE_
#ifdef _WIN32
#pragma warning( disable:4100 ) //未引用的形参
#pragma warning( disable:4995 ) //名称被标记为 #pragma deprecated
#pragma warning( disable:4996 ) //函数更新
#pragma warning( disable:4800 ) //BOOL与bool转换
#pragma warning( disable:4005 ) //“_WIN32_WINNT”: 宏重定义
#pragma warning( disable:4819 ) //该文件包含不能在当前代码页(936)中表示的字符。请将该文件保存为 Unicode 格式以防止数据丢失
#pragma warning( disable:4251 )//需要有 dll 接口由 class“CLocalNode”的客户端使用
#pragma warning( disable:4275 ) // 非 dll 接口 class“CTypeCheck”用作 dll 接口 class“CLocalNode”的基
#pragma warning( disable:4482 )//使用了非标准扩展: 限定名中使用了枚举
#endif
#endif