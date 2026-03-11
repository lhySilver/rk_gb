#pragma once

#include <stdint.h>

#ifdef __WINDOWS__
#define TINY_HTTP_SERVER_API __declspec(dllexport)
#else
#define TINY_HTTP_SERVER_API __attribute__((visibility("default")))
#endif
#ifdef __cplusplus
extern "C" {
#endif
    // c 接口
    struct HttpServerParam
    {
        // 只能是 http, https, http2, https2，其中之一
        char*    m_type; // 服务器类型
        uint16_t m_port;
        char*    m_certPath;    // 如果使用openssl, 则需要提供ssl证书路径
        char*    m_privKeyPath; // 如果使用openssl, 则需要提供ssl证书的私钥
    };

    /**
    * @brief 初始化webroot
    * @param webRoot, 指定web服务器读取html文件的基路径
    * @tips 如果webRoot为空，则默认不支持web服务器
    */
    TINY_HTTP_SERVER_API bool initHttpServerEnv(const char* webRoot = "");
    
    /**
     * @brief 反初始化，程序退出的时候使用
     *
     */
    TINY_HTTP_SERVER_API void deinitHttpServerEnv();

    /**
     * @brief 创建并开启 http 服务器
     * 暂时支持 http1.1 & http1.1 + openssl
     */
    TINY_HTTP_SERVER_API int createHttpServer(const HttpServerParam& cfg);

    /**
     * @brief 停止并删除 http 服务器
     */
    TINY_HTTP_SERVER_API void  destroyHttpServer(int srvId);
#ifdef __cplusplus
}
#endif