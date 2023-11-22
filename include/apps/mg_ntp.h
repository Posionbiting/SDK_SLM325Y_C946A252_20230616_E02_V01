/**
  ******************************************************************************
  * @file    mg_ntp.h
  * @author  MEIG Development Team
  * @version V1.0.0
  * @date    16-Sep-2022
  * @brief   This file provides all the NTP function APIs.
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
#ifndef __MG_NTP_H__
#define __MG_NTP_H__
#include "mg_type.h"


#ifdef __cplusplus
extern "C" {
#endif
/****************************************************************************
 * Def config
 ***************************************************************************/


/* ntp service host name length maximum*/
#define SNTP_HOST_NAME_LENGTH_MAX 128

/******************************************************************************
* Function:     MG_SNTP_SyncTime
*
* Description:
*               Synchronize system time by NTP.
*
* Parameters:
*               host:
*                       [in] NTP server address
*               timeout:
*                       [in] timeout in milliseconds
* Return:
*               MG_RET_OK, this function succeeds.
*               Other values, error code.
******************************************************************************/
s32 MG_SNTP_SyncTime(char *host, u32 timeout);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __MG_NTP_H__ */
