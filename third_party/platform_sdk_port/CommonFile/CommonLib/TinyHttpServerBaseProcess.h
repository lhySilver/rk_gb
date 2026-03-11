/**
* @brief http 请求处理的基类， 也是导出类
* 二次开发时，将命令处理类注册到动态库中的map中，有该命令的请求时，底层调用过程如下:
* 1)、框架通过内置的一个小型的反射机制构造TinyHttpServerBaseProcess
* 2)、将processor加入线程池，等待调用
* 3)、在线程i中调用processRequest
* 4)、 processRequest返回之后，在线程i中调用
*  4.1) getRespStatus
*  4.2) getRetHeader
*  4.3) getRetQueries
*  4.4) getRetContent
* 5)、以上4执行完毕之后，向客户端返回http请求处理结果
*
*
* 注意事项：
* 1、导出类中使用到了STL、string， 而STL是源码方式提供，不同版本的stl实现上存在一些差异
* 2、在使用该动态库时，需要保证动态库的libc与可执行程序的libc保持一致（使用同一个版本的编译环境即可）
* 3、在动态库中，为了便于业务的处理，实现了一个小型的反射机制， 反射机制命令处理类注册方法如下：
*  3.1) 写一个命令处理类，继承TinyHttpServerBaseProcess
*  3.2) 在命令处理类中，通过 BIND_URL_TO_PROCESS 宏，注册， BIND_URL_TO_PROCESS的语法规则为：BIND_URL_TO_PROCESS(http请求的uri, 类名称)
*  @example
*   class PingRequest : public TinyHttpServerBaseProcess{
    protected:
        virtual funs();
    };
    BIND_URL_TO_PROCESS("ping", PingRequest);

    则 http 服务器能够处理如下格式的请求
    http://srvIP:port/ping?var1=arg1&var2=arg2
    http://srvIP:port/ping
*/

#pragma once

#include <map>
#include <string>
#include "WarningDisable.h"

#ifdef __WINDOWS__
#define TINY_HTTP_SERVER_API __declspec(dllexport)
#else
#define TINY_HTTP_SERVER_API __attribute__((visibility("default")))
#endif

namespace tiny {
    // http 请求需要处理的参数
    class TINY_HTTP_SERVER_API HttpRequestParam
    {
    public:
        std::string m_method;   // GET、 PUT、DELETE etc
        std::string m_uri;      // uri
        std::string m_content;  // content, 支持chuncked， 目前没有封装压缩、序列化相关，需要应用层处理
        std::map<std::string, std::string> m_queries;  // 示例: http://127.0.0.1:8080/ping?key1=val&key=val, m_queries中保存key*, val*
        std::map<std::string, std::string> m_headers;  // http请求中的头部信息
    };

    // 处理接口的基类
    class TINY_HTTP_SERVER_API TinyHttpServerBaseProcess
    {
    public:
        explicit TinyHttpServerBaseProcess();
        virtual ~TinyHttpServerBaseProcess();

    public:
        /**
         * @brief http请求处理，底层在线程池中处理
         * @param requestParam, @ref HttpRequestParam
         * @warning: 由于底层框架是在线程池中调用该函数，故处理过程是阻塞的。不要试图将requestParam缓存下来，并且另外开启线程处理
         */
        virtual void processRequest(const HttpRequestParam* requestParam) = 0;

        // 执行完之后, 获取返回信息，待发送给客户端
        /**
         * @brief 获取http请求处理结果代码， 200 404 etc
         */
        virtual int  getRespStatus() {
            return m_respCode;
        }

        /**
        * @brief 获取http请求处理结果需要返回的http头
        */
        virtual std::map<std::string, std::string> getRetHeader() {
            // 跨域资源共享，主要用于浏览器支持
            m_respHeaders["Access-Control-Allow-Origin"] = "*";
            return m_respHeaders;
        }

        /**
        * @brief 无效函数, 可不用实现
        */
        virtual std::map<std::string, std::string> getRetQueries() {
            return m_respQueries;
        }

        /**
        * @brief 获取http请求处理结果需要返回的http消息体(是否序列化有应用层决定)
        */
        virtual std::string getRetContent() {
            return m_respContent;
        }

    protected:
        int         m_respCode;
        std::string m_respContent;
        std::map<std::string, std::string>  m_respHeaders;
        std::map<std::string, std::string>  m_respQueries;
    };

    //////////////////////////////////////////////////////////////////////////
    //  -------------------------   反射接口 注册
    // 以下接口不需要特别的使用，只是在定义命令处理类时，调用 BIND_URL_TO_PROCESS 即可
    //////////////////////////////////////////////////////////////////////////
    class TINY_HTTP_SERVER_API user_never_call_ProcessorRegister
    {
    public:
        user_never_call_ProcessorRegister(std::string type, class TinyHttpServerBaseProcess * (*_cb)());
    };

#define BIND_URL_TO_PROCESS(http_url, class_name)                     \
    static class tiny::TinyHttpServerBaseProcess* Creator_##class_name(){       \
        return new class_name();                                    \
    }                                                               \
    static tiny::user_never_call_ProcessorRegister g_creator_##class_name(http_url, Creator_##class_name);
}
