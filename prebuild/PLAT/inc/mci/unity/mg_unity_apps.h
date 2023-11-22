/**
  ******************************************************************************
  * @file    mg_unity_apps.h
  * @author  MEIG Development Team
  * @version V1.0.0
  * @date    28-Mar-2022
  * @brief   This file provides APIs for internal use.
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

/* Define to prevent recursive inclusion -------------------------------------*/

#ifndef _MG_UNITY_APPS_H_
#define _MG_UNITY_APPS_H_
#include "mg_type.h"

#ifdef __cplusplus
extern "C" {
#endif

s32 MG_UNITY_APPS_IPAddrPortToSockAddr(void *ipAddr, u16 nPort, void *sockAddr, s32 *len);
s32 MG_UNITY_APPS_SockAddrToIPAddrPort(void *sockAddr, void *ipAddr, u16 *nPort);
s32 MG_UNITY_APPS_GetLastError(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif // End-of _MG_UNITY_NET_H_