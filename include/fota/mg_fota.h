/**
  ******************************************************************************
  * @file    mg_fota.h
  * @author  MEIG Development Team
  * @version V1.0.0
  * @date    26-July-2022
  * @brief   This file contains all the functions prototypes for the FOTA.
  ******************************************************************************
  * @attention
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
#ifndef _MG_FOTA_H_
#define _MG_FOTA_H_
#include "mg_type.h"

/******************************************************************************
* Function:     MG_FOTA_Init
*
* Description:
*               Initialise fota related functions , and
*               remove previous update package data.
*
* Parameters:
*               void
* Return:
*               MG_RET_OK, this function succeeds.
*               Other values, error code.
******************************************************************************/
s32 MG_FOTA_Init(void);

/******************************************************************************
* Function:     MG_FOTA_Start
*
* Description:
*               Establish a channel for refreshing fota package to the special
*               space in the module.
*               After established the channel and refreshed the packet, please 
*               use the function MG_FOTA_Finish to close the channel
* Parameters:
*               void
* Return:
*               MG_RET_OK, this function succeeds.
*               Other values, error code.
******************************************************************************/
s32 MG_FOTA_Start(void);

/******************************************************************************
* Function:     MG_FOTA_WriteData
*
* Description:
*               refresh fota package to the special space in the module.
*
* Parameters:
*               void
* Return:
*               MG_RET_OK, this function succeeds.
*               Other values, error code.
******************************************************************************/
s32 MG_FOTA_WriteData(u8 *buffer, s32 len);

/******************************************************************************
* Function:     MG_FOTA_Finish
*
* Description:
*               Close the transferring channel.
*
* Parameters:
*               void
* Return:
*               MG_RET_OK, this function succeeds.
*               Other values, error code.
******************************************************************************/
s32 MG_FOTA_Finish(void);

/******************************************************************************
* Function:     MG_FOTA_Update
*
* Description:
*               Check the remaining space to ensure successful upgrade. and check
*               whether update fota data is valid. Finally, set update to ready status. 
*               When the module is restarted, it will be updated according to the
*               ready status.
*
*               After the function fails to execute, you can use the function MG_FOTA_Init
*               to remove fota package.
*
* Parameters:
*               void
* Return:
*               MG_RET_OK, this function succeeds.
*               Other values, error code.
******************************************************************************/
s32 MG_FOTA_Update(void);

#endif  // End-of _MG_FOTA_H_
