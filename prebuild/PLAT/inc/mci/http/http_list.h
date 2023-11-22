#ifndef __HTTP_LIST_H_
#define __HTTP_LIST_H_

#include "http_common.h"

#ifdef __cplusplus
extern "C" {
#endif

#define HTTP_HeaderNext(pHeader)      (ST_HttpHeader *)MG_UNITY_ListNext((ST_UnityList *)pHeader)

/**
 * \brief Container for http header list.
 */
typedef struct _HttpHeader {
    UNITY_LIST_STRUCT_MEMBERS

    ST_httpString *name;
    ST_httpString *value;
} ST_HttpHeader, ST_HttpHeaderList;

/**
 * \brief  This function new http header list.
 *
 * \param  None
 *
 * \return The list pointer.
 */
ST_HttpHeaderList *http_headerlist_new();

/**
 * \brief  This function get http header by the name.
 *
 * \param  headerList      The list pointer.
 * \param  name            The name pointer.
 *
 * \return The list pointer.
 */
ST_HttpHeader *http_headerlist_get(ST_HttpHeaderList *headerList, const char *name);

/**
 * \brief  This function set the name and value.
 *
 * \param  headerList      The list pointer.
 * \param  name            The name pointer.
 * \param  name            The value pointer.
 *
 * \return None.
 */
void http_headerlist_set(ST_HttpHeaderList *headerList, const char *name, const char *value);

/**
 * \brief  This function clear http header list.
 *
 * \param  headerList      The list pointer.
 *
 * \return None.
 */
void http_headerlist_clear(ST_HttpHeaderList *headerList);

/**
 * \brief  This function delete http header list.
 *
 * \param  headerList      The list pointer.
 *
 * \return None.
 */
void http_headerlist_delete(ST_HttpHeaderList *headerList);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif // End-of __HTTP_LIST_H_