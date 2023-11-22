/**
  ******************************************************************************
  * @file    mg_unity_net.h
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

#ifndef _MG_UNITY_NET_H_
#define _MG_UNITY_NET_H_
#include "mg_type.h"

#ifdef __cplusplus
extern "C" {
#endif

#define IPC_SIM_GET_ICCID_REQ_ID                       0x0001
#define IPC_SIM_GET_IMSI_REQ_ID                        0x0002

#define IPC_NET_PS_SET_CGDCONT_REQ_ID                  0x0101
#define IPC_NET_PS_GET_CGDCONT_REQ_ID                  0x0102
#define IPC_NET_PS_GET_CEREG_REQ_ID                    0x0103
#define IPC_NET_PS_SET_CFUN_REQ_ID                     0x0104
#define IPC_NET_PS_GET_CFUN_REQ_ID                     0x0105
#define IPC_NET_PS_GET_ECSTATUS_REQ_ID                 0x0106
#define IPC_NET_PS_GET_ECBCINFOREQ_REQ_ID              0x0107
#define IPC_NET_PS_SET_CGACT_REQ_ID                    0x0108

#define IPC_MM_SET_CPSMS_REQ_ID                        0x0201
#define IPC_MM_GET_CPSMS_REQ_ID                        0x0202

u32 MG_UNITYNET_GetIpcSyncReqMinTimeout(u32 id);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif // End-of _MG_UNITY_NET_H_