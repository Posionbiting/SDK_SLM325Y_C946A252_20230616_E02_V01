#ifndef __HTTP_URI_H_
#define __HTTP_URI_H_

#include "http_common.h"

#ifdef __cplusplus
extern "C" {
#endif

#define HTTP_CR "\r"
#define HTTP_LF "\n"
#define HTTP_CRLF "\r\n"
#define HTTP_SP " "
#define HTTP_COLON ":"
#define HTTP_VER10 "HTTP/1.0"
#define HTTP_VER11 "HTTP/1.1"

#define HTTP_URI_KNKOWN_PORT (-1)
#define HTTP_NET_URI_DEFAULT_PATH "/"
#define HTTP_URI_DEFAULT_HTTP_PORT 80
#define HTTP_URI_DEFAULT_HTTPS_PORT 443

#define HTTP_NET_IPV4_ADDRSTRING_MAXSIZE ((3 * 4) + (1 * 3) + 1)
#define HTTP_NET_IPV6_ADDRSTRING_MAXSIZE (1 + (8 * 4) + (1 * 7) + 1 + 1)

#define http_uri_isabsolute(urip) ((0 < http_string_length(urip->protocol)) ? true : false)

typedef struct _mUpnpDictionary {
    UNITY_LIST_STRUCT_MEMBERS

    ST_httpString *key;
    ST_httpString *value;
} ST_HttpDictionaryElement, ST_HttpDictionary;

typedef struct _HttpUri {
    ST_httpString *uri;
    ST_httpString *protocol;
    ST_httpString *user;
    ST_httpString *password;
    ST_httpString *host;
    u16 port;
    ST_httpString *path;
    ST_httpString *query;
    ST_httpString *fragment;
    ST_httpString *request;
    ST_HttpDictionary *queryDictionary;
} ST_HttpUri;

/**
 * \brief  This function new uri context.
 */
ST_HttpUri *http_uri_new();

/**
 * \brief  This function delete uri context.
 */
void http_uri_delete(ST_HttpUri *uri);

/**
 * \brief  This function parse uri string.
 */
void http_uri_setvalue(ST_HttpUri *uri, const char *value);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif // End-of __HTTP_URI_H_