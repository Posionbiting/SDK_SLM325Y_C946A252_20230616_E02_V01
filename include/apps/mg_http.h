/**
  ******************************************************************************
  * @file    mg_http.h
  * @author  MEIG Development Team
  * @version V1.0.0
  * @date    28-Mar-2022
  * @brief   This file provides all the http function APIs.
  ******************************************************************************
  *  @attention
  *
  * THE SOFTWARE IS PROTECTED BY COPYRIGHT AND THE INFORMATION CONTAINED HEREIN
  * IS CONFIDENTIAL. THE SOFTWARE MAY NOT BE COPIED AND THE INFORMATION CONTAINED
  * HEREIN MAY NOT BE USED OR DISCLOSED EXCEPT WITH THE WRITTEN PERMISSION OF
  * MEIG SMART TECHNOLOGY CO., LTD.
  *
  * <h2><center>&copy; COPYRIGHT 2022 MEIG SMART</center></h2>
  ******************************************************************************
  */

#ifndef _MG_HTTP_H_
#define _MG_HTTP_H_
#include "mg_type.h"

#ifdef __cplusplus
extern "C" {
#endif

/****************************************************************************
 *Definitons and Declarations
 ***************************************************************************/

#define HTTP_OPT_DISENABLE 0
#define HTTP_OPT_ENABLE 1

#define HTTP_OPT_RECV_BUFFER_MIN_SIZE 255
#define HTTP_OPT_RECV_BUFFER_MAX_SIZE 4096

/**
 * mbedtls tls verify
 */
#define HTTP_TLS_VERIFY_NONE 0
#define HTTP_TLS_VERIFY_OPTIONAL 1
#define HTTP_TLS_VERIFY_REQUIRED 2

/**
 * @brief mbedtls version config
 */
typedef enum _HttpTlsVersion {
    HTTP_TLS_VERSION_SSL3_0 = 0,
    HTTP_TLS_VERSION_TLS1_0,
    HTTP_TLS_VERSION_TLS1_1,
    HTTP_TLS_VERSION_TLS1_2,
    HTTP_TLS_VERSION_MAX,
}Enum_HttpTlsVersion;

/**
 * @brief The configuration information used to establish the HTTP request context
 */
typedef struct _HttpInfo {
    Enum_HttpTlsVersion vsn;
    s32 verify;
    char *cacert;
    char *clicert;
    char *pkey;
    u8 cid;
}ST_HttpInfo;

/**
 * @brief The HTTP option type
 */
typedef enum {
    /** use usr added request header
     *  default:HTTP_OPT_DISENABLE
    */
    HTTP_OPT_USR_REQUEST_HEADER,

    /** long connect and not to close socket
     *  default:HTTP_OPT_DISENABLE
    */
    HTTP_OPT_LONG_CONNECT,

    /** set recv buffer size
     *  default:HTTP_OPT_RECV_BUFFER_MIN_SIZE
    */
    HTTP_OPT_RECV_BUF_SIZE,

    HTTP_OPT_MAX,
} Enum_HttpOpt;

/**
 * @brief The HTTP register callback type
 */
typedef enum {
    /* register callback to accept content date */
    HTTP_REG_TYPE_CONTENT_INPUT_DATA,

    HTTP_REG_TYPE_MAX,
} Enum_HttpRegCallbackType;

/**
 * @brief The HTTP response info type
 */
typedef enum {
    /** get response first line   (char *) */
    HTTP_RESPONSE_FIRST_LINE,

    /** get response header (char *) */
    HTTP_RESPONSE_HEADER,

    /** get response content (char *) */
    HTTP_RESPONSE_CONTENT,

    HTTP_RESPONSE_MAX,
} Enum_HttpResponseInfoType;

/**
 * @brief The HTTP request info
 */
typedef struct _HttpResponseInfo ST_HttpResponseInfo;

/**
 * @brief The HTTP request context
 */
typedef struct _HttpRequestCtx ST_HttpRequestCtx;

/**
 * @brief register callback to accept content data
 */
typedef s32(*callback_http_content_input_data)(s32 err, s32 statusCode, u8 *data, u32 data_len, void *param, s32 isEnd);


/******************************************************************************
* Function:     MG_HTTP_Create
*
* Description:
*               Create http request context.
*
* Parameters:
*               info:
*                       [IN] http initial parameter configuration.
*                            If the parameter is null, configured by default.
* Return:
*               If no error occurs, MG_HTTP_Create returns the http request
*               context Otherwise, returns NULL.
******************************************************************************/
ST_HttpRequestCtx *MG_HTTP_Create(ST_HttpInfo *info);

/******************************************************************************
* Function:     MG_HTTP_Destroy
*
* Description:
*               Destroy http request context.
*
* Parameters:
*               request:
*                       [IN] the http request context.
* Return:
*               void
******************************************************************************/
void MG_HTTP_Destroy(ST_HttpRequestCtx *request);

/******************************************************************************
* Function:     MG_HTTP_SetOpt
*
* Description:
*               Set the http request options.
*
* Parameters:
*               request:
*                       [IN] the http request context.
*               optName:
*                       [IN] the request option name.
*               optVal:
*                       [IN] the pointer to the request option value.
*               optLen:
*                       [IN] the request option value length.
* Return:
*               MG_RET_OK, this function succeeds.
*               Other values, error code.
******************************************************************************/
s32 MG_HTTP_SetOpt(ST_HttpRequestCtx *request, Enum_HttpOpt optName, void *optVal, u32 optLen);


/******************************************************************************
* Function:     MG_HTTP_Register
*
* Description:
*               Register http callback.
*
* Parameters:
*               request:
*                       [IN] the http request context.
*               type:
*                       [IN] the Register callback type.
*               cb:
*                       [IN] the callback function.
*               param:
*                       [IN] the the callback function paramter.
* Return:
*               MG_RET_OK, this function succeeds.
*               Other values, error code.
******************************************************************************/
s32 MG_HTTP_Register(ST_HttpRequestCtx *request, Enum_HttpRegCallbackType type, void *cb, void *param);

/******************************************************************************
* Function:     MG_HTTP_RequestHeaderAdd
*
* Description:
*               The function may add user's own http request header when the
*               option HTTP_OPT_USR_REQUEST_HEADER is configured to enable.
*               Otherwise, use default request header
*
* Parameters:
*               request:
*                       [IN] the http request context.
*               name:
*                       [IN] the request hedaer name string.
*               value:
*                       [IN] the request hedaer value string.
* Return:
*               MG_RET_OK, this function succeeds.
*               Other values, error code.
******************************************************************************/
s32 MG_HTTP_RequestHeaderAdd(ST_HttpRequestCtx *request, char *name, char *value);

/******************************************************************************
* Function:     MG_HTTP_RequestHeaderClear
*
* Description:
*               Clear the added request hedaer
*
* Parameters:
*               request:
*                       [IN] the http request context.
* Return:
*               void
******************************************************************************/
void MG_HTTP_RequestHeaderClear(ST_HttpRequestCtx *request);

/******************************************************************************
* Function:     MG_HTTP_ResponseNew
*
* Description:
*               New http response context.
*
* Parameters:
*               void:
* Return:
*               If no error occurs, MG_HTTP_ResponseNew returns the
*               http response context. Otherwise, returns NULL.
******************************************************************************/
ST_HttpResponseInfo *MG_HTTP_ResponseNew(void);

/******************************************************************************
* Function:     MG_HTTP_ResponseClear
*
* Description:
*               Clear the http response context
*
* Parameters:
*               response:
*                       [IN] the http response context.
* Return:
*               void
******************************************************************************/
void MG_HTTP_ResponseClear(ST_HttpResponseInfo *response);

/******************************************************************************
* Function:     MG_HTTP_ResponseDelete
*
* Description:
*               Delete the http response context
*
* Parameters:
*               response:
*                       [IN] the http response context.
* Return:
*               void
******************************************************************************/
void MG_HTTP_ResponseDelete(ST_HttpResponseInfo *response);

/******************************************************************************
* Function:     MG_HTTP_GetResponseStatusCode
*
* Description:
*               Get the http response code
*
* Parameters:
*               response:
*                       [IN] the http response context.
* Return:
*               >=0, the http response code
*               <0, error code.
******************************************************************************/
s32 MG_HTTP_GetResponseStatusCode(ST_HttpResponseInfo *response);

/******************************************************************************
* Function:     MG_HTTP_GetResponseInfoSize
*
* Description:
*               Get the http response info size by Enum_HttpResponseInfoType
*
* Parameters:
*               response:
*                       [IN] the http response context.
*               type:
*                       [IN] the response info type.
* Return:
*               >=0, the http response info size
*               <0, error code.
******************************************************************************/
s32 MG_HTTP_GetResponseInfoSize(ST_HttpResponseInfo *response, Enum_HttpResponseInfoType type);

/******************************************************************************
* Function:     MG_HTTP_GetResponseInfoValue
*
* Description:
*               Get the http response info
*
* Parameters:
*               response:
*                       [IN] the http response context.
*               type:
*                       [IN] the response info type.
*               value:
*                       [OUT] Output buffer.
*               size:
*                       [IN] Output buffer szie.
* Return:
*               >=0, the info length
*               <0, error code.
******************************************************************************/
s32 MG_HTTP_GetResponseInfoValue(ST_HttpResponseInfo *response, Enum_HttpResponseInfoType type, u8 *value, s32 size);

/******************************************************************************
* Function:     MG_HTTP_Get
*
* Description:
*               Http get request
*
* Parameters:
*               request:
*                       [IN] the http request context.
*               uri:
*                       [IN] the URL string.
*               response:
*                       [IN] the http response context.
*               timeout:
*                       [IN] timeout in milliseconds.
* Return:
*               MG_RET_OK, this function succeeds.
*               Other values, error code.
******************************************************************************/
s32 MG_HTTP_Get(ST_HttpRequestCtx *request, const char *uri, ST_HttpResponseInfo *response, s32 timeout);

/******************************************************************************
* Function:     MG_HTTP_Post
*
* Description:
*               Http post request
*
* Parameters:
*               request:
*                       [IN] the http request context.
*               uri:
*                       [IN] the URL string.
*               data:
*                       [IN] the post data.
*               dataLen:
*                       [IN] the post data length.
*               response:
*                       [IN] the http response context.
*               timeout:
*                       [IN] timeout in milliseconds.
* Return:
*               MG_RET_OK, this function succeeds.
*               Other values, error code.
******************************************************************************/
s32 MG_HTTP_Post(ST_HttpRequestCtx *request, const char *uri, const u8 *data, u32 dataLen, ST_HttpResponseInfo *response, s32 timeout);

/******************************************************************************
* Function:     MG_HTTP_Head
*
* Description:
*               Http head request
*
* Parameters:
*               request:
*                       [IN] the http request context.
*               uri:
*                       [IN] the URL string.
*               response:
*                       [IN] the http response context.
*               timeout:
*                       [IN] timeout in milliseconds.
* Return:
*               MG_RET_OK, this function succeeds.
*               Other values, error code.
******************************************************************************/
s32 MG_HTTP_Head(ST_HttpRequestCtx *request, const char *uri, ST_HttpResponseInfo *response, s32 timeout);

/******************************************************************************
* Function:     MG_HTTP_Put
*
* Description:
*               Http put request
*
* Parameters:
*               request:
*                       [IN] the http request context.
*               uri:
*                       [IN] the URL string.
*               data:
*                       [IN] the put data.
*               dataLen:
*                       [IN] the put data length.
*               response:
*                       [IN] the http response context.
*               timeout:
*                       [IN] timeout in milliseconds.
* Return:
*               MG_RET_OK, this function succeeds.
*               Other values, error code.
******************************************************************************/
s32 MG_HTTP_Put(ST_HttpRequestCtx *request, const char *uri, const u8 *data, u32 dataLen, ST_HttpResponseInfo *response, s32 timeout);

/******************************************************************************
* Function:     MG_HTTP_Close
*
* Description:
*               This function clear the usr's added request hedaer,
*               close request connect when the option HTTP_OPT_LONG_CONNECT
*               is configured to enable.
*
* Parameters:
*               request:
*                       [IN] the http request context.
* Return:
*               void
******************************************************************************/
void MG_HTTP_Close(ST_HttpRequestCtx *request);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif // End-of _MG_HTTP_H_