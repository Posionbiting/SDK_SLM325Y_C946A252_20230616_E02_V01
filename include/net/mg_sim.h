/**
  ******************************************************************************
  * @file    mg_sim.h
  * @author  MEIG Development Team
  * @version V1.0.0
  * @date    28-Mar-2022
  * @brief   This file provides all the sim function APIs.
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

#ifndef _MG_SIM_H_
#define _MG_SIM_H_

/*****************************************************************************
 *Include Files
 ****************************************************************************/
#include "mg_type.h"

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************
 *Definitons and Declarations
 ****************************************************************************/
#define SIM_IMEI_SIZE    17
#define SIM_IMSI_SIZE    17
#define SIM_ICCID_SIZE   21
#define SIM_MCC_SIZE     4
#define SIM_MNC_SIZE     4

typedef enum
{
    /**No sim card inserted*/
    SIM_STATUS_ABSENT = 0,
    /**sim card normal*/
    SIM_STATUS_NORMAL = 1
} Enum_SIM_Status;

typedef enum
{
    SIM_HOTPLUG_DISABLE = 0,
    SIM_HOTPLUG_ENABLE  = 1,
} Enum_SIM_HotPlugSwitch;


typedef enum
{
    SIM_HOTPLUG_LOW  = 0,
    SIM_HOTPLUG_HIGH = 1,
} Enum_SIM_HotPlugTrigLevel;


typedef enum {
    /** SIM hot-plug enable/disable (u8)
     *  0--disable  1--enable
    */
    SIM_OPT_HOT_PLUG_SWITCH,

    /** SIM hot-plug voltage trigger level (u8)
     *  0--low level(insert card) 1--high level(insert card)
    */
    SIM_OPT_HOT_PLUG_VOLT_TRIG_LEVEL,

    SIM_OPT_MAX,
} Enum_SimOpt;

/*****************************************************************************
 *Function Declarations
 ****************************************************************************/

/*****************************************************************************
* Function:     MG_SIM_GetState
*
* Description:
*               get the status of SIM.
*
* Parameters:
*               simId:
*                       [IN] sim card index.
*               state:
*                       [IN] state of sim.
* Return:
*               MG_RET_OK, this function succeeds.
*               Other values, error code.
*****************************************************************************/
s32 MG_SIM_GetState(u8 simId, Enum_SIM_Status *state);

/*****************************************************************************
* Function:     MG_SIM_GetIMEI
*
* Description:
*               get the IMEI of SIM.
*
* Parameters:
*               simId:
*                       [IN] sim card index.
*               imei:
*                       [OUT] the pointer to IMEI buffer .
*               imei_size:
*                       [IN] the IMEI buffer size.
* Return:
*               >0, the IMEI number length.
*               Other values, error code.
*****************************************************************************/
s32 MG_SIM_GetIMEI(u8 simId,u8 *imei,u8 imei_size);

/*****************************************************************************
* Function:     MG_SIM_GetIMSI
*
* Description:
*               get the IMSI of SIM.
*
* Parameters:
*               simId:
*                       [IN] sim card index.
*               imsi:
*                       [OUT] the pointer to IMSI buffer .
*               imsi_size:
*                       [IN] the IMSI buffer size.
* Return:
*               >0, the IMSI number length.
*               Other values, error code.
*****************************************************************************/
s32 MG_SIM_GetIMSI(u8 simId,u8 *imsi,u8 imsi_size);

/*****************************************************************************
* Function:     MG_SIM_GetICCID
*
* Description:
*               get the ICCID of SIM.
*
* Parameters:
*               simId:
*                       [IN] sim card index.
*               iccid:
*                       [OUT] the pointer to ICCID buffer .
*               iccid_size:
*                       [IN] the ICCID buffer size.
* Return:
*               >0, the ICCID number length.
*               Other values, error code.
*****************************************************************************/
s32 MG_SIM_GetICCID(u8 simId,u8 *iccid,u8 iccid_size);

/******************************************************************************
* Function:     MG_SIM_SetOpt
*
* Description:
*               Set the SIM card hot plug options.
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
s32 MG_SIM_SetOpt(Enum_SimOpt optName, void *optVal, u32 optLen);

/******************************************************************************
* Function:     MG_SIM_GetOpt
*
* Description:
*               Get the SIM card hot plug options.
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
s32 MG_SIM_GetOpt(Enum_SimOpt optName, void *optVal, u32 optLen);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif // End-of _MG_SIM_H_