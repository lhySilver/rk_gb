/*
 * Copyright (c) 2023 Fancy Code.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef ILOG_H
#define ILOG_H
#ifdef __cplusplus
extern "C" {
#endif
enum LogType
{
    LOG_TYPE_VERBOSE = 0,
    LOG_TYPE_DEBUG,
    LOG_TYPE_INFORMATION,
    LOG_TYPE_WARNING,
    LOG_TYPE_ERROR,
    LOG_TYPE_TRACE,
    LOG_TYPE_TEST_TIPS,
    LOG_TYPE_END
};
typedef struct LogSetting
{
    const char *fileName; // File name of saving log.
    const char *maxSize;  // Max size of saving log.
} LogSetting;
enum LogInstance
{
    LOG_SERIAL_PRINT = 0, // for serial print.
    LOG_EASYLOGGING,      // for easylogging++.
    LOG_INSTANCE_TYPE_END
};
#ifndef __F_FILE__
    #define __F_FILE__ "fancy"
#endif
#ifndef LOG_DISABLE
    #define LogVerbose(...) ILogPrintf(__F_FILE__, __LINE__, LOG_TYPE_VERBOSE, __VA_ARGS__)
    #define LogDebug(...) ILogPrintf(__F_FILE__, __LINE__, LOG_TYPE_DEBUG, __VA_ARGS__)
    #define LogInfo(...) ILogPrintf(__F_FILE__, __LINE__, LOG_TYPE_INFORMATION, __VA_ARGS__)
    #define LogWarning(...) ILogPrintf(__F_FILE__, __LINE__, LOG_TYPE_WARNING, __VA_ARGS__)
    #define LogError(...) ILogPrintf(__F_FILE__, __LINE__, LOG_TYPE_ERROR, __VA_ARGS__)
    #define LogTrace(...) ILogPrintf(__F_FILE__, __LINE__, LOG_TYPE_TRACE, __VA_ARGS__)
#else
    #define LogVerbose(...)
    #define LogDebug(...)
    #define LogInfo(...)
    #define LogWarning(...)
    #define LogError(...)
    #define LogTrace(...)
#endif
#if 1 // For OpenHarmony log, should delete finally.// TODO:
    #define LOGD(...)
    #define LOGI(...)
    #define LOGW(...)
    #define LOGE(...)
    #define LOGF(...)
#endif
void CreateLogModule(void);
void DestroyLogModule(void);
void ILogInit(const enum LogInstance log);
void ILogUnInit(void);
int ILogPrintf(const char *file, const int line, const enum LogType type, const char *format, ...);
#ifdef __cplusplus
}
#endif
#endif