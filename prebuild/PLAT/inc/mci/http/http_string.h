#ifndef __HTTP_STRING_H_
#define __HTTP_STRING_H_

#include "http_common.h"

#ifdef __cplusplus
extern "C" {
#endif

/* UINT_MAX : 4294967295U */
#define HTTP_STRING_INTEGER_BUFLEN 16

/* ULONG_MAX : 18446744073709551615ULL */
#define HTTP_STRING_LONG_BUFLEN 32

/*  ULLONG_MAX : 18446744073709551615ULL */
#define HTTP_STRING_LONGLONG_BUFLEN 32

#define HTTP_STRING_FLOAT_BUFLEN 64
#define HTTP_STRING_DOUBLE_BUFLEN 64

typedef ST_UnityString ST_httpString;

static inline ST_httpString *http_string_new()
{
    return (ST_httpString *)MG_UNITY_StringNew();
}

static inline void http_string_delete(ST_httpString *str)
{
    return MG_UNITY_StringDelete((ST_UnityString *)str);
}

static inline void http_string_clear(ST_httpString *str)
{
    return MG_UNITY_StringClear((ST_UnityString *)str);
}

static inline s32 http_string_length(ST_httpString *str)
{
    return MG_UNITY_StringLength((ST_UnityString *)str);
}

static inline char *http_string_getvalue(ST_httpString *str)
{
    return MG_UNITY_StringGetvalue((ST_UnityString *)str);
}

static inline s32 http_string_setvalue(ST_httpString *str, const char *value, s32 len)
{
    return MG_UNITY_StringSetvalue((ST_UnityString *)str, value, len);
}

static inline char *http_string_addvalue(ST_httpString *str, const char *value, s32 len)
{
    return MG_UNITY_StringAddvalue((ST_UnityString *)str, value, len);
}

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif // End-of __HTTP_STRING_H_