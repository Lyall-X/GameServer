/*
Special config for MSVC build of hiredis

Does absolutely nothing for Unix!
*/
#ifndef __CONFIG_H
#define __CONFIG_H

#include "hiredis.h"
#ifdef HIREDIS_WIN

#ifndef va_copy
/* WARNING - DANGER - ASSUMES TYPICAL STACK MACHINE */
#define va_copy(dst, src) ((void)((dst) = (src)))
#endif

#if defined( _MSC_VER ) && !defined( __cplusplus )
#define inline __inline
#endif

#define EINPROGRESS WSAEINPROGRESS

#define snprintf sprintf_s
#define strcasecmp strcmp
#define strncasecmp _strnicmp
#define strerror_r(errorno, buf, len) strerror_s(buf, len, errorno)

#pragma warning(disable : 4996)
#pragma warning(disable : 4091)
#define _CRT_SECURE_NO_DEPRECATE 1
#define _CRT_SECURE_COPP_OVERLOAD_STANDARD_NAMES 1
#pragma warning(disable:4251)		// dll-interface bullshit


#endif

#endif