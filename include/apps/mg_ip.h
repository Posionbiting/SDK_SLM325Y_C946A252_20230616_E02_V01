/**
  ******************************************************************************
  * @file    mg_ip.h
  * @author  MEIG Development Team
  * @version V1.0.0
  * @date    28-Mar-2022
  * @brief   This file provides all the ip function APIs.
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

#ifndef _MG_IP_H_
#define _MG_IP_H_
#include "mg_type.h"

#ifdef __cplusplus
extern "C" {
#endif


/** 255.255.255.255 */
#define INADDR_NONE_V4         "255.255.255.255"
/** 127.0.0.1 */
#define INADDR_LOOPBACK_V4     "127.0.0.1"
/** 0.0.0.0 */
#define INADDR_ANY_V4          "0.0.0.0"
/** 255.255.255.255 */
#define INADDR_BROADCAST_V4    "255.255.255.255"


typedef struct _ip4_addr
{
  u32 addr;
} ST_ip4Addr;

typedef struct _ip6_addr {
  u32 addr[4];
  u8 zone;
}ST_ip6Addr;

typedef enum _ip_addr_type {
  /** IPv4 */
  IP_ADDR_TYPE_V4 =   0U,
  /** IPv6 */
  IP_ADDR_TYPE_V6 =   6U,
}Enum_ipType;

typedef struct _ip_addr {
  union {
    ST_ip4Addr ip4;
    ST_ip6Addr ip6;
  } u_addr;

/* Enum_ipType */
  u8 type;
} ST_ipAddr;

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif // End-of _MG_IP_H_