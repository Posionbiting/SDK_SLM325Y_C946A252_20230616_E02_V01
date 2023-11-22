/**
  ******************************************************************************
  * @file    mg_crypto.h
  * @author  MEIG Development Team
  * @version V1.0.0
  * @date    28-Mar-2022
  * @brief   This file provides all the crypto function APIs.
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

#ifndef _MG_CRYPTO_H_
#define _MG_CRYPTO_H_
#include "mg_type.h"

#ifdef __cplusplus
extern "C" {
#endif

/****************************************************************************
 *Definitons and Declarations
 ***************************************************************************/
#define  HMAC_KEY_MAX_LEN (64)
#define  HMAC_STRING_OFFSET (1)
#define  HMAC_MD5_DIGEST_STRING_LEN ((32)+HMAC_STRING_OFFSET)
#define  HMAC_SHA1_DIGEST_STRING_LEN ((40)+HMAC_STRING_OFFSET)
#define  HMAC_SHA256_DIGEST_STRING_LEN ((64)+HMAC_STRING_OFFSET)
 
/******************************************************************************
* Function:     MG_CRYPTO_HmacMD5
*
* Description:
*               hmacMD5 algorithms.
*
* Parameters:
*               msg:
*                       [IN] Messages that need to be encrypted
*               msgLen:
*                       [IN] Messages length
*               digest:
*                       [OUT] Output buffer
*               digestSize:
*                       [OUT] Output buffer size
*               key:
*                       [IN] secret key
*               keyLen:
*                       [IN] secret key length
* Return:
*               MG_RET_OK, this function succeeds.
*               Other values, error code.
******************************************************************************/
s32 MG_CRYPTO_HmacMD5(const char *msg, u32 msgLen, char *digest, u32 digestSize, const char *key, u32 keyLen);

/******************************************************************************
* Function:     MG_CRYPTO_HmacSHA1
*
* Description:
*               hmacSHA1 algorithms.
*
* Parameters:
*               msg:
*                       [IN] Messages that need to be encrypted
*               msgLen:
*                       [IN] Messages length
*               digest:
*                       [OUT] Output buffer
*               digestSize:
*                       [IN] Output buffer size
*               key:
*                       [IN] secret key
*               keyLen:
*                       [IN] secret key length
* Return:
*               MG_RET_OK, this function succeeds.
*               Other values, error code.
******************************************************************************/
s32 MG_CRYPTO_HmacSHA1(const char *msg, u32 msgLen, char *digest, u32 digestSize, const char *key, u32 keyLen);
 
/******************************************************************************
* Function:     MG_CRYPTO_HmacSHA256
*
* Description:
*               hmacSHA256 algorithms.
*
* Parameters:
*               msg:
*                       [IN] Messages that need to be encrypted
*               msgLen:
*                       [IN] Messages length
*               digest:
*                       [OUT] Output buffer
*               digestSize:
*                       [IN] Output buffer size
*               key:
*                       [IN] secret key
*               keyLen:
*                       [IN] secret key length
* Return:
*               MG_RET_OK, this function succeeds.
*               Other values, error code.
******************************************************************************/
s32 MG_CRYPTO_HmacSHA256(const char *msg, u32 msgLen, char *digest, u32 digestSize, const char *key, u32 keyLen);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif // End-of _MG_CRYPTO_H_


