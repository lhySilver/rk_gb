
///////////////////////////ShareSDK.h

#ifndef _SAHRE_SDK_H_
#define _SAHRE_SDK_H_

#include "base_type_define.h"

#ifdef WIN32

#include <typeinfo.h>

#ifdef SHARE_SDK_EXPORT_API//动态库编译
#define _SHARE_SDK  __declspec(dllexport)
#else
#ifdef SHARE_SDK_EXPORT_STATIC//静态库编译和使用
#define _SHARE_SDK 
#else//使用者编译
#define _SHARE_SDK  __declspec(dllimport)
#endif
#endif

#else
#define _SHARE_SDK __attribute__ ((visibility("default")))
#endif

#ifdef __ENVIRONMENT_LINUX_ANDROID__
#else
#include <strstream>
#endif
class CLogWrapperWriterProxy;

typedef enum _printf_level_
{
	PRINTF_LEVEL_DEBUG = 1,
	PRINTF_LEVEL_RUN,
	PRINTF_LEVEL_WARN,
	PRINTF_LEVEL_ERROR,
}PRINTF_LEVEL;

typedef enum _printf_model_
{
	PRINTF_MODEL_DR = 0x01,
	PRINTF_MODEL_PUB = 0x02,
	PRINTF_MODEL_AP = 0x04,
	PRINTF_MODEL_ALL = PRINTF_MODEL_DR | PRINTF_MODEL_PUB | PRINTF_MODEL_AP,
}PRINTF_MODEL;

/**
 * @class CLogWrapper 
 *
 * @brief 写日志类
 * 
 * CLogWrapper提供了写日志接口和对日志的控制功能：
 * @n 1、提供更多的日志级别
 * @n 从低到高分别是LEVEL_DEBUG(调试)，LEVEL_INFO(信息)，LEVEL_WARN(警告)，
 *  LEVEL_ERROR(错误)，LEVEL_FATAL(致命错误)。日志级别可通过配置文件(参考
 *  log4cxx的配置)或者调用接口SetLevel()指定。高于配置级别的日志信息都会被
 *  记录到日志中
 *  @n ------------------------------ 
 *  @n CLogWrapper级别   对应的log4cxx级别
 *  @n LEVEL_DEBUG  DEBUG 
 *  @n LEVEL_INFO   INFO 
 *  @n LEVEL_WARN   WARN 
 *  @n LEVEL_ERROR  ERROR
 *  @n LEVEL_FATAL  FATAL
 *  @n ------------------------------
 * @n 2、提供了多种写日志接口
 * @li使用宏：TVT_LOG_DEBUG()、TVT_LOG_INFO()、TVT_LOG_WARN()、TVT_LOG_ERROR()、TVT_LOG_FATAL()。
 *     这些宏自动关联了级别，可使用c++流方式直接写入多种数据类型，并且同时可
 *     记录当前的文件名和行号,例如：
 * @n   LOG_INFO("Test Info Message" << 5 << 1.1);
 * @n   LOG_ERROR("Test ERROR Message" << 5 << 0x10);
 * @li使用接口方法：Debug()、Info()、Warn()、Error()、Fatal()、Log()、LogTo()
 * @n   CLogWrapper::Debug("Debug message");
 *
 *  3、与原有的CMyDebug兼容问题
 * @n SHARESDK_AbnormalOutput	对应 TVT_LOG_ERROR
 * @n SHARESDK_NormalOutput		对应 TVT_LOG_INFO
 * @n SHARESDK_DebugOutput		对应 TVT_LOG_DEBUG
 */

class _SHARE_SDK CLogWrapper
{
public:
	/// 日志级别
	enum LOG_LEVEL
	{
		LEVEL_DEBUG = 10000, /**< Debug级别，最低*/
		LEVEL_INFO  = 20000, /**< Info级别*/
		LEVEL_WARN  = 30000, /**< Warn级别*/
		LEVEL_ERROR = 40000, /**< Error级别*/
		LEVEL_FATAL = 50000
	};
public:
	/**
	 * @brief 构造函数
	 */
	CLogWrapper();
	/**
	 * @brief 析构函数
	 */
	virtual ~CLogWrapper();
	
	/**
	 * @brief 将logInfo写入到当前日志
	 *
	 * 写日志时不要直接使用这个方法，可以使用更简单的Debug()、Error()
	 * 等方法，或者更为方便的宏：LOG_DEBUG()等。
	 * 
	 * @param[in] level 该条日治信息级别
	 * @param[in] logInfo 需要写入到日志的信息
	 * @param[in] file 当前文件名，默认为0，可使用宏__FILE__
	 * @param[in] line 当前行号，默认为-1，可使用宏__LINE__
	 * @return void
	 */
	static void Log(const LOG_LEVEL level, 
		const char *logInfo,
		const char* file = 0,
		const char* fun = 0,
		int line = -1);
	/**
	 * @brief 写入一条LEVEL_DEBUG级别的日志信息
	 * @param[in] logInfo 需要写入日志的信息
	 * @return void
	 * @remarks 如果当前日志级别高于LEVEL_DEBUG，该条信息不会被写入
	 */
	static void Debug(const char * logInfo);
	/**
	 * @brief 写入一条LEVEL_INFO级别的日志信息
	 * @param[in] logInfo 需要写入日志的信息
	 * @return void
	 * @remarks 如果当前日志级别高于LEVEL_INFO，该条信息不会被写入
	 */
	static void Info(const char* logInfo);
	/**
	 * @brief 写入一条LEVEL_WARN级别的日志信息
	 * @param[in] logInfo 需要写入日志的信息
	 * @return void
	 * @remarks 如果当前日志级别高于LEVEL_WARN，该条信息不会被写入
	 */
	static void Warn(const char* logInfo);
	/**
	 * @brief 写入一条LEVEL_ERROR级别的日志信息
	 * @param[in] logInfo 需要写入日志的信息
	 * @return void
	 * @remarks 如果当前日志级别高于LEVEL_ERROR，该条信息不会被写入
	 */
	static void Error(const char* logInfo);
	/**
	 * @brief 写入一条LEVEL_FATAL级别的日志信息
	 * @param[in] logInfo 需要写入日志的信息
	 * @return void
	 * @remarks 如果当前日志级别高于LEVEL_FATAL，该条信息不会被写入
	 */
	static void Fatal(const char* logInfo);
	/**
	 * @brief 设置日志级别
	 * @param[in] level 日志级别
	 * @param[in] logName 日志名称，默认值为0，为0时设置当前正在使用日
	 *                    志的级别
	 * @return 0  for success
	 * @return -1 for error，使用CZxSystem::GetLastError()检查错误
	 */
	static int SetLevel(LOG_LEVEL level);
	/**
	 * @brief
	 *
	 * 
	 * @param
	 * @return 0  for success
	 * @return -1 for error,
	 */
	static const LOG_LEVEL GetLevel();

	static void Init(const char* exeName = NULL,const char* dirPath = NULL);
	static void DeInit();
};

typedef void (*LPTHREAD_ROUTINE )(LPVOID lpThreadParameter);
class COutputBase
{
public:
	COutputBase(void){};
public:
	virtual ~COutputBase(void){};

	//出现了理论上不可能发生的事情时调用此函数，有点类似断言
	virtual void AbnormalOutput(const char* logInfo, const char *pTime, const char* fileName, tuint32 line)=0;
	//软件运行日志，关键步骤的关键节点输出的信息
	virtual void NormalOutput(const char* logInfo, const char *pTime, const char* fileName, tuint32 line)=0;
	//调试输出，可能会出现刷屏情况，信息本身有助于分析模块的问题
	virtual void DebugOutput(const char* logInfo,const char *pTime,  const char* fileName, tuint32 line)=0;

	//调试输出，可能会出现刷屏情况，信息本身有助于分析模块的问题
	virtual void DebugPrintf(const char* logInfo,const char *pTime,  const char* fileName, tuint32 line)=0;
	//软件运行日志，关键步骤的关键节点输出的信息
	virtual void RunPrintf(const char* logInfo, const char *pTime, const char* fileName, tuint32 line)=0;
	//出现了理论上不可能发生的事情时调用此函数，有点类似断言
	virtual void WarnPrintf(const char* logInfo, const char *pTime, const char* fileName, tuint32 line)=0;
	//出现了不允许的错误，也未做容错处理，一般会导致系统宕机
	virtual void ErrorPrintf(const char* logInfo, const char *pTime, const char* fileName, tuint32 line)=0;
};

//出现了理论上不可能发生的事情时调用此函数，有点类似断言
_SHARE_SDK void SHARESDK_AbnormalOutput(const char* logInfo, const char* fileName, tuint32 line);

//软件运行日志，关键步骤的关键节点输出的信息
_SHARE_SDK void SHARESDK_NormalOutput(const char* logInfo, const char* fileName, tuint32 line);

//调试输出，可能会出现刷屏情况，信息本身有助于分析模块的问题
_SHARE_SDK void SHARESDK_DebugOutput(const char* logInfo, const char* fileName, tuint32 line);

//软件运行日志，关键步骤的关键节点输出的信息
_SHARE_SDK void SHARESDK_RunPrintf(const char ch,const char* logInfo, const char* fileName, tuint32 line);

//调试输出，可能会出现刷屏情况，信息本身有助于分析模块的问题
_SHARE_SDK void SHARESDK_DebugPrintf(const char ch,const char* logInfo, const char* fileName, tuint32 line);

//出现了理论上不可能发生的事情时调用此函数，有点类似断言
_SHARE_SDK void SHARESDK_WarnPrintf(const char ch,const char* logInfo, const char* fileName, tuint32 line);

//出现了不允许的错误，也未做容错处理，一般会导致系统宕机
_SHARE_SDK void SHARESDK_ErrorPrintf(const char ch,const char* logInfo, const char* fileName, tuint32 line);

//增加输出的观察者，外部用来处理输出
_SHARE_SDK void SHARESDK_AddOutputObserver(COutputBase *pOutObject);
//删除输出的观察者，外部用来处理输出
_SHARE_SDK void SHARESDK_DelOutputObserver(COutputBase *pOutObject);

//#define ABNORMAL_OUTPUT(x) SHARESDK_AbnormalOutput(x,__FILE__,__LINE__)
//#define NORMAL_OUTPUT(x) SHARESDK_NormalOutput(x,__FILE__,__LINE__)
//#define DEBUG_OUTPUT(x) SHARESDK_DebugOutput(x,__FILE__,__LINE__)

#define ABNORMAL_OUTPUT(fmt, ...) 						\
do																\
{																\
	char buf[4096];memset(buf,0,sizeof(buf));									\
	snprintf(buf, sizeof(buf)-1, fmt, ##__VA_ARGS__);				\
	SHARESDK_AbnormalOutput(buf,__FILE__,__LINE__);				\
} while (0);

#define NORMAL_OUTPUT(fmt, ...) 						\
do																\
{																\
	char buf[4096];memset(buf,0,sizeof(buf));											\
	snprintf(buf, sizeof(buf)-1, fmt, ##__VA_ARGS__);				\
	SHARESDK_NormalOutput(buf,__FILE__,__LINE__);				\
} while (0);

#define DEBUG_OUTPUT(fmt, ...) 						\
do																\
{																\
	char buf[4096];memset(buf,0,sizeof(buf));											\
	snprintf(buf, sizeof(buf)-1, fmt, ##__VA_ARGS__);				\
	SHARESDK_DebugOutput(buf,__FILE__,__LINE__);				\
} while (0);

#define TASSERT(a)  (a?((VOID) 0):SHARESDK_AbnormalOutput("ASSERT",__FILE__,__LINE__))


#if defined(_MSC_VER)
#if _MSC_VER >= 1300
#define __LOG_FUNC__ __FUNCSIG__
#endif
#else
#if defined(__GNUC__)
#define __LOG_FUNC__ __PRETTY_FUNCTION__
#endif
#endif

/// 写LEVEL_DEBUG级别日志的宏
#define TVT_LOG_DEBUG(logInfo)\
	if (CLogWrapper::GetLevel()<=CLogWrapper::LEVEL_DEBUG)\
{\
	std::ostrstream ostr;\
	ostr << logInfo << std::ends;\
	CLogWrapper::Log(CLogWrapper::LEVEL_DEBUG,ostr.str(),__FILE__,__LOG_FUNC__,__LINE__);\
	ostr.rdbuf()->freeze(0);\
}

/// 写LEVEL_INFO级别日志的宏
#ifdef __ENVIRONMENT_LINUX_ANDROID__
#define TVT_LOG_INFO(logInfo)\
	if (CLogWrapper::GetLevel()<=CLogWrapper::LEVEL_INFO){}
	/*
{\
	std::ostrstream ostr;\
	ostr << logInfo << std::ends;\
	CLogWrapper::Log(CLogWrapper::LEVEL_INFO,ostr.str(),__FILE__,__LOG_FUNC__,__LINE__);\
	ostr.rdbuf()->freeze(0);\
}
*/
#else 
#define TVT_LOG_INFO(logInfo)\
	if (CLogWrapper::GetLevel()<=CLogWrapper::LEVEL_INFO)\
{\
	std::ostrstream ostr;\
	ostr << logInfo << std::ends;\
	CLogWrapper::Log(CLogWrapper::LEVEL_INFO,ostr.str(),__FILE__,__LOG_FUNC__,__LINE__);\
	ostr.rdbuf()->freeze(0);\
}
#endif

/// 写LEVEL_WARN级别日志的宏
#define TVT_LOG_WARN(logInfo)\
	if (CLogWrapper::GetLevel()<=CLogWrapper::LEVEL_WARN)\
{\
	std::ostrstream ostr;\
	ostr << logInfo << std::ends;\
	CLogWrapper::Log(CLogWrapper::LEVEL_WARN,ostr.str(),__FILE__,__LOG_FUNC__,__LINE__);\
	ostr.rdbuf()->freeze(0);\
}
/// 写LEVEL_ERROR级别日志的宏
#define TVT_LOG_ERROR(logInfo)\
	if (CLogWrapper::GetLevel()<=CLogWrapper::LEVEL_ERROR)\
{\
	std::ostrstream ostr;\
	ostr << logInfo << std::ends;\
	CLogWrapper::Log(CLogWrapper::LEVEL_ERROR,ostr.str(),__FILE__,__LOG_FUNC__,__LINE__);\
	ostr.rdbuf()->freeze(0);\
}\
	/// 写LEVEL_FATAL级别日志的宏

#define TVT_LOG_FATAL(logInfo)\
	if (CLogWrapper::GetLevel()<=CLogWrapper::LEVEL_FATAL)\
{\
	std::ostrstream ostr;\
	ostr << logInfo << std::ends;\
	CLogWrapper::Log(CLogWrapper::LEVEL_FATAL,ostr.str(),__FILE__,__LOG_FUNC__,__LINE__);\
	ostr.rdbuf()->freeze(0);\
}


/// 写入不同级别日志的宏
#define TVT_LOG_MESSAGE(Level,logInfo,file,line) \
{\
	CLogWrapper::Log(Level,logInfo,file,__LOG_FUNC__,line);\
}

//生成带循环的线程
_SHARE_SDK HANDLE SHARESDK_CreateThread(LPTHREAD_ROUTINE lpStartAddress,LPVOID lpParameter,tuint32 sleepTime,
										bool bCreatSuspended,const char* fileName, int lineNum, const char* description);
//生成运行一次的线程
_SHARE_SDK HANDLE SHARESDK_CreateRunOneThread(LPTHREAD_ROUTINE lpStartAddress,LPVOID lpParameter,bool bCreatSuspended,const char* fileName, int lineNum, const char* description);
//生成运行一次的线程,不用销毁，运行完自动释放资源
_SHARE_SDK bool SHARESDK_RunOneThread(LPTHREAD_ROUTINE lpStartAddress,LPVOID lpParameter,
									  const char* fileName, const char* funName, int lineNum, const char* description);
//销毁线程
_SHARE_SDK void   SHARESDK_DestroyThread(HANDLE threadHandle,const char* fileName, int lineNum);
_SHARE_SDK bool SHARESDK_AttachThreadToCPU(HANDLE threadHandle,int cpuindex);

_SHARE_SDK bool SHARESDK_SetThreadRunProperty(const char *pBufForXMLData);

//得到线程ID
_SHARE_SDK tuint_ptr  SHARESDK_GetThreadID(HANDLE threadHandle);
//让挂起的线程恢复运行
_SHARE_SDK bool  SHARESDK_ResumeThread(HANDLE threadHandle);

#define CREATE_THREAD(addr,para,sleeptime,desc) SHARESDK_CreateThread(addr,para,sleeptime,false,__FILE__,__LINE__,desc)

#define CREATE_SUSPEND_THREAD(addr,para,sleeptime,desc) SHARESDK_CreateThread(addr,para,sleeptime,true,__FILE__,__LINE__,desc)

#define CREATE_RUNONE_THREAD(addr,para,bsuspend,desc) SHARESDK_CreateRunOneThread(addr,para,bsuspend,__FILE__,__LINE__,desc)
#define RUNONE_THREAD(addr,para,desc) SHARESDK_RunOneThread(addr,para,__FILE__,__FUNCTION__,__LINE__,desc)

#define DESTROY_THREAD(handle)                           SHARESDK_DestroyThread(handle,__FILE__,__LINE__)

_SHARE_SDK void SHARESDK_CreateSingleton(LPVOID pSingleTon,const char *name);
_SHARE_SDK void SHARESDK_DestroySingleton(LPVOID pSingleTon);

#define CREATE_SINGLETON(singleton,T) SHARESDK_CreateSingleton(singleton,typeid(T).name())
#define DESTROY_SINGLETON(singleton) SHARESDK_DestroySingleton(singleton)

//时钟信号回调函数
typedef void (TimerCBFun)(tuint32 dwTimerID, tuint32 dwElapse, LPVOID lpParameter);
//设置时钟,最小精度100毫秒,即dwElapse应为100毫秒的整数倍,返回时钟句柄(成功>0)
_SHARE_SDK tuint32 SHARESDK_SetTimer(tuint32 dwElapse, TimerCBFun *pCBFun, LPVOID lpParameter);
//销毁设置的时钟,不能在时钟信号回调函数里调用
_SHARE_SDK bool SHARESDK_KillTimer(tuint32 dwTimerID);


//资源计数

//计数类型
typedef enum
{
	COUNT_TYPE_CLIENT	= 0x00000001,		//客户端数量
	COUNT_TYPE_LIVE		= 0x00000002,		//客户端现场预览
	COUNT_TYPE_FTP_REC	= 0x00000004,		//FTP录像
	COUNT_TYPE_RTSP		= 0x00000008,		//RTSP
	COUNT_TYPE_PLAYBACK	= 0x00000010,		//客户端回放
	COUNT_TYPE_BACKUP	= 0x00000020,		//客户端备份
}COUNT_TYPE;

_SHARE_SDK bool SHARESDK_SetMaxCountType(tuint32 CountType, tuint32 dwMaxNum, float fWeights = 1.0);	//CountType可以为组合类型,表示组合的几种之和不能超过此上限,fWeights是指在组合计算中的权重

_SHARE_SDK bool SHARESDK_IncrementCountType(tuint32 dwCountType);				//dwCountType为一个计数类型,超过限制返回false,否则返回true
_SHARE_SDK void SHARESDK_DecrementCountType(tuint32 dwCountType);				//dwCountType为一个计数类型

_SHARE_SDK bool SHARESDK_SetMaxDispatch(tuint32 dwMaxDispatch);					//设置每秒最大发送量,单位Mbps
_SHARE_SDK tuint32 SHARESDK_DispatchControl(tuint32 dwDataLen);					//返回当次可以发送的数据量,如果已超出,此函数内部Sleep足够的时间才返回
_SHARE_SDK void SHARESDK_UnDispatchControl(tuint32 dwDataLen);					//当发送失败时,要回溯计算

_SHARE_SDK bool SHARESDK_IsAPOpenPrint(PRINTF_LEVEL iLevel);

//看门狗相关
_SHARE_SDK bool SHARESDK_AttachThreadToDetect(HANDLE threadHandle);                  //threadHandle为线程ID,添加线程检测
_SHARE_SDK bool SHARESDK_DetachThreadToDetect(HANDLE threadHandle);                  //threadHandle为线程ID,移除线程检测
_SHARE_SDK tuint32 SHARESDK_GetDetectThreadMaxDifferenceIndex(tuint32 dwCheckIndex);   //dwCheckIndex为检测索引，返回检测线程与dwCheckIndex相差的最大差值
////////////////////////
//查询硬件序列号
_SHARE_SDK void SHARESDK_GetSerialNum(std::string &strSerialNum);
#endif








