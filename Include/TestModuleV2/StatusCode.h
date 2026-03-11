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
#ifndef STATUS_CODE_H
#define STATUS_CODE_H
#include <stdbool.h>
#ifdef __cplusplus
extern "C" {
#endif
enum STATUS_CODE
{
    STATUS_CODE_OK = 0,
    STATUS_CODE_NOT_OK,
    STATUS_CODE_VIRTUAL_FUNCTION,
    STATUS_CODE_INVALID_PARAMENTER,
    STATUS_CODE_MAKE_SHARED_PTR_FAILED,
    STATUS_CODE_END
};
typedef struct status_code StatusCode;
typedef struct status_code
{
    /**
     * @brief Prints the error code string.
     */
    const char *(*mPrintStringCode)(const StatusCode);
    /**
     * @brief Determines whether the error code is equal to an enumeration value.
     */
    const bool (*mCodeEqual)(const StatusCode, const char *);
    /**
     * @brief Error code enumeration value.
     */
    const long int mStatusCode;
} StatusCode;
/**
 * @brief Create a status code structure data.
 * @param code Status code enumeration, see "STATUS_CODE" for details; the enumeration value can be accumulated by
 * different function modules, and the value may be repeated. If there is any objection, use the "PrintStringCode"
 * function to view the string analysis.
 * @return const StatusCode Returns the status code data structure.
 */
const StatusCode CreateStatusCode(const long int code);
/**
 * @brief Prints a string interpretation of the status code.
 * @param code Status code structure "StatusCode".
 * @return const char* Returns a string parsed of the printed status code.
 */
static inline const char *PrintStringCode(const StatusCode code)
{
    return code.mPrintStringCode(code);
}
/**
 * @brief Whether the status code represents a normal/successful business status. The enumeration value is
 * "STATUS_CODE_OK".
 * @param code Status code structure "StatusCode".
 * @return true
 * @return false
 */
static inline bool IsCodeOK(const StatusCode code)
{
    return STATUS_CODE_OK == code.mStatusCode ? true : false;
}
/**
 * @brief Status code string equality judgment. Status code enumeration values ​​can be accumulated independently by
 * different modules, and there is a possibility of duplication. To judge whether they are equal, string definitions are
 * required.
 * @param code Status code structure "StatusCode".
 * @param value String definition of the status code enumeration value.
 * @return true Status code and string matching.
 * @return false The status code and string do not match.
 */
static inline bool StatusCodeEqual(const StatusCode code, const char *value)
{
    return code.mCodeEqual(code, value);
}
#ifdef __cplusplus
}
#endif
#endif