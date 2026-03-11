
/* MD5.H - header file for MD5C.C */


/*  Copyright (C) 1991-2, RSA Data Security, Inc. Created 1991. All
    rights reserved.

    License to copy and use this software is granted provided that it
    is identified as the &quot;RSA Data Security, Inc. MD5 Message-Digest
    Algorithm&quot; in all material mentioning or referencing this software
    or this function.

    License is also granted to make and use derivative works provided
    that such works are identified as &quot;derived from the RSA Data
    Security, Inc. MD5 Message-Digest Algorithm&quot; in all material
    mentioning or referencing the derived work.

    RSA Data Security, Inc. makes no representations concerning either
    the merchantability of this software or the suitability of this
    software for any particular purpose. It is provided &quot;as is&quot;
    without express or implied warranty of any kind.

    These notices must be retained in any copies of any part of this
    documentation and/or software.
*/

//bUpper true	szOut里用大写字母表示十六进制数
//       false  szOut里用小写字母表示十六进制数

#ifndef __PUB_MD5_H__
#define __PUB_MD5_H__

#include "base_type_define.h"

void PUB_MD5Encrypt(const void *pIn, tint32 iLenIn, char szOut[32], bool bUpper = true);

#endif