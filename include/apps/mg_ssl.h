/**
  ******************************************************************************
  * @file    mg_ssl.h
  * @author  MEIG Development Team
  * @version V1.0.0
  * @date    28-Mar-2022
  * @brief   This file provides all the ssl function APIs.
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

#ifndef _MG_SSL_H_
#define _MG_SSL_H_
#include "mg_type.h"

#ifdef __cplusplus
extern "C" {
#endif

/****************************************************************************
 *Definitons and Declarations
 ***************************************************************************/
typedef enum _SslVerify{
    VERIFY_NONE = 0,
    VERIFY_OPTIONAL = 1,
    VERIFY_REQUIRED = 2,
    VERIFY_MAX = 3,
}Enum_SslVerify;

typedef enum _SslVsn{
    VERSION_SSL3_0 = 0,
    VERSION_TLS1_0 = 1,
    VERSION_TLS1_1 = 2,
    VERSION_TLS1_2 = 3,
    VERSION_MAX = 4,
}Enum_SslVersion;

typedef struct
{
    Enum_SslVersion vsn;
    Enum_SslVerify verify;
    char *cacert;
    char *clicert;
    char *pkey;
}ST_SslInfo;

typedef enum {
    /**  bind cid  (u8) */
    SOCK_SSL_CID,
    /** set verify type  (u8) */
    SOCK_SSL_VERIFY,

    SOCK_SSL_MAX,
}Enum_SslOpt;


typedef struct _SslSock ST_SslSock;

/******************************************************************************
* Function:     MG_SOCK_SSL_Create
*
* Description:
*               Create ssl context.
*
* Parameters:
*               info:
*                       [IN] ssl initial parameter configuration.
* Return:
*               If no error occurs, MG_SOCK_SSL_Create returns the ssl
*               context. Otherwise, returns NULL.
******************************************************************************/
ST_SslSock* MG_SOCK_SSL_Create(ST_SslInfo* info);

/******************************************************************************
* Function:     MG_SOCK_SSL_Destroy
*
* Description:
*               Destroy ssl context.
*
* Parameters:
*               sock:
*                       [IN] ssl context.
* Return:
*               MG_RET_OK, this function succeeds.
*               Other values, error code.
******************************************************************************/
s32 MG_SOCK_SSL_Destroy(ST_SslSock *sock);

/******************************************************************************
* Function:     MG_SOCK_SSL_Connect
*
* Description:
*               ssl connect.
*
* Parameters:
*               sock:
*                       [IN] ssl context.
*               hostName:
*                       [IN] server address.
*               port:
*                       [IN] server port.
*               timeout:
*                       [IN] timeout in milliseconds.
* Return:
*               MG_RET_OK, this function succeeds.
*               Other values, error code.
******************************************************************************/
s32 MG_SOCK_SSL_Connect(ST_SslSock* sock, const char *hostName, u16 port, s32 timeout);

/******************************************************************************
* Function:     MG_SOCK_SSL_Disconnect
*
* Description:
*               ssl disconnect.
*
* Parameters:
*               sock:
*                       [IN] ssl context.
* Return:
*               MG_RET_OK, this function succeeds.
*               Other values, error code.
******************************************************************************/
s32 MG_SOCK_SSL_Disconnect(ST_SslSock *sock);

/******************************************************************************
* Function:     MG_SOCK_SSL_Write
*
* Description:
*               ssl write.
*
* Parameters:
*               sock:
*                       [IN] ssl context.
*               pData:
*                       [IN] the pointer to data .
*               dataLen:
*                       [IN] data length.
* Return:
*               >=0, written data length.
*               Other values, error code.
******************************************************************************/
s32 MG_SOCK_SSL_Write(ST_SslSock* sock, const u8 *pData, u32 dataLen);

/******************************************************************************
* Function:     MG_SOCK_SSL_Read
*
* Description:
*               ssl read.
*
* Parameters:
*               sock:
*                       [IN] ssl context.
*               pData:
*                       [OUT] the pointer to output data buffer .
*               size:
*                       [IN] the output data buffer size.
* Return:
*               >=0, read data length.
*               Other values, error code.
******************************************************************************/
s32 MG_SOCK_SSL_Read(ST_SslSock* sock, u8 *pData, u32 size);

/******************************************************************************
* Function:     MG_SOCK_SSL_GetSocketId
*
* Description:
*               Get socket id.
*
* Parameters:
*               sock:
*                       [IN] ssl context.
* Return:
*               >0, socket id.
*               Other values, error code.
******************************************************************************/
s32 MG_SOCK_SSL_GetSocketId(ST_SslSock *sock);
/******************************************************************************
* Function:     MG_SOCK_SSL_SetOpt
*
* Description:
*               Set the SSL options.
*
* Parameters:
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

s32 MG_SOCK_SSL_SetOpt(ST_SslSock *sock, Enum_SslOpt optName, void *optVal, u32 optLen);

/******************************************************************************
* Function:     MG_SOCK_SSL_GetOpt
*
* Description:
*               Get the SSL options.
*
* Parameters:
*               optName:
*                       [IN] the request option name.
*               optVal:
*                       [OUT] the pointer to the request option value.
*               optLen:
*                       [IN] the request option value length.
* Return:
*               MG_RET_OK, this function succeeds.
*               Other values, error code.
******************************************************************************/

s32 MG_SOCK_SSL_GetOpt(ST_SslSock *sock, Enum_SslOpt optName, void *optVal, u32 optLen);



#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif // End-of _MG_SSL_H_

