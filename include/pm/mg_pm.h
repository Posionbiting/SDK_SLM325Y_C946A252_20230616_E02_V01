/**
  ******************************************************************************
  * @file    mg_pm.h
  * @author  MEIG Development Team
  * @version V1.0.0
  * @date    28-Mar-2022
  * @brief   This file provides all the pm function APIs.
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

#ifndef _MG_PM_H_
#define _MG_PM_H_

#include "mg_type.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define PM_ENTER_SLEEP_DELAY_MS      (500)

#define PM_SLEEP_MODE_0               0   ///< low clock,   enable DRX,        disable eDRX, PSM
#define PM_SLEEP_MODE_1               1   ///< low clock,   enable DRX, PSM,   disable eDRX

#define PM_SLEEP_LOG_OUTPUT_NO        0  ///< no log output
#define PM_SLEEP_LOG_OUTPUT_UART      1  ///< log is output to UART
#define PM_SLEEP_LOG_OUTPUT_USB       2  ///< log is output to USB

#define MCI_POWER_KEY_RELEASE         0
#define MCI_POWER_KEY_PRESS           1
typedef void (*powerKeyCallback)(u8 pwrKey_status, void *arg);

#define MCI_WAKEUP_IN_PIN_LEVEL_LOW   0
#define MCI_WAKEUP_IN_PIN_LEVEL_HIGH  1
typedef void (*WakeupInCallback)(u8 index, u8 pin_level, void *arg);

typedef struct
{
    /** the sleep mode
      * default : PM_SLEEP_MODE_0
      */
    u8 sleep_mode;

    /** the delay is required before going to sleep
      * because the Serial may be in the process.
      * unit : millisecond
      * default : PM_ENTER_SLEEP_DELAY_MS
      */
    u32 Serial_delay;

    /** select log output
      * default : PM_SLEEP_LOG_OUTPUT_NO
      */
    u8 log_output;

    /** power key callback */
    powerKeyCallback pwrKeyCb;
    void *pwrKeyCb_arg;

    /** wakeup in callback */
    WakeupInCallback WakeupInCb;
    void *WakeupInCb_arg;
} ST_PmSleepOpt;

/******************************************************************************
* Function:     MG_PM_SetT3412
*
* Description:
*               Set Requested_Periodic-TAU.
*
* Parameters:
*               simId:
*                       [IN] the sim card ID
*               value:
*                       [IN] Requested_Periodic-TAU, 8bit String of Byte.
*                            e.g. "01000111"
*               length:
*                       [IN] Size of data in value. Must be 8.
*
* Return:
*               MG_RET_OK, this function succeeds.
*               Other values, error code.
******************************************************************************/
s32 MG_PM_SetT3412(u8 simId, u8 *value, u8 length);

/******************************************************************************
* Function:     MG_PM_SetT3324
*
* Description:
*               Set Requested_Active-Time.
*
* Parameters:
*               simId:
*                       [IN] the sim card ID
*               value:
*                       [IN] Requested_Active-Time, 8bit String of Byte.
*                            e.g. "01100101"
*               length:
*                       [IN] Size of data in value. Must be 8.
*
* Return:
*               MG_RET_OK, this function succeeds.
*               Other values, error code.
******************************************************************************/
s32 MG_PM_SetT3324(u8 simId, u8 *value, u8 length);

/******************************************************************************
* Function:     MG_PM_EnterSleep
*
* Description:
*               enter sleep mode.
*
*               if the parameter opt is set to NULL, the sleep option
*               will be set to the default value
*
* Parameters:
*               simId:
*                       [IN] the sim card ID
*               opt:
*                       [IN] the point to the sleep option
*
* Return:
*               MG_RET_OK, this function succeeds.
*               Other values, error code.
******************************************************************************/
s32 MG_PM_EnterSleep(u8 simId, ST_PmSleepOpt *opt);

/******************************************************************************
* Function:     MG_PM_ExitSleep
*
* Description:
*               exit sleep mode.
*
* Parameters:
*               simId:
*                       [IN] the sim card ID
*
* Return:
*               MG_RET_OK, this function succeeds.
*               Other values, error code.
******************************************************************************/
s32 MG_PM_ExitSleep(u8 simId);






#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif // End-of _MG_PM_H_
