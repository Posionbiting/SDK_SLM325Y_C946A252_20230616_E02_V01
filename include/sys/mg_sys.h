/**
  ******************************************************************************
  * @file    mg_sys.h
  * @author  MEIG Development Team
  * @version V1.0.0
  * @date    28-Mar-2022
  * @brief   This file provides all the SYSTEM function APIs.
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
#ifndef __MG_SYS_H__
#define __MG_SYS_H__
#include "mg_type.h"
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
/****************************************************************************
 * Def config
 ***************************************************************************/
/** module info min length*/
#define MODULE_INFO_MIN_LENGTH  65

/** module version min length*/
#define MODULE_VERSION_MIN_LENGTH  33

/** module SN LENGTH */
#define MODULE_SN_MIN_LENGTH  24
#define MODULE_SN1            0
#define MODULE_SN2            1

/** module reboot case*/
#define SYS_BOOTCAUSE_BIT0_UNKNOWN      (1 << 0)         // unknown reason
#define SYS_BOOTCAUSE_BIT1_PWRKEY       (1 << 1)         // boot by power key
#define SYS_BOOTCAUSE_BIT2_PIN_RESET    (1 << 2)         // boot by pin reset
#define SYS_BOOTCAUSE_BIT3_ALARM        (1 << 3)         // boot by alarm
#define SYS_BOOTCAUSE_BIT4_CHARGE       (1 << 4)         // boot by charge in
#define SYS_BOOTCAUSE_BIT5_WDG          (1 << 5)         // boot by watchdog
#define SYS_BOOTCAUSE_BIT6_PIN_WAKEUP   (1 << 6)         // boot by wakeup
#define SYS_BOOTCAUSE_BIT7_PSM_WAKEUP   (1 << 7)         // boot from PSM wakeup
#define SYS_BOOTCAUSE_BIT8_PANIC        (1 << 8)         // boot by panic reset
#define SYS_BOOTCAUSE_BIT9_FORCE        (1 << 9)         // boot by calling MG_SYSTEM_Reboot

/*****************************************************************
* Function:     MG_SYSTEM_Reboot
*
* Description:
*               This function resets the system.
*
* Return:
*               MG_RET_OK, success.
*               MG_RET_ERR_SYS_FAILED , failed.
*****************************************************************/
s32 MG_SYSTEM_Reboot(void);
/*****************************************************************
* Function:     MG_SYSTEM_PowerOff
*
* Description:
*               Power Off.
*
* Return:
*               MG_RET_OK, success.
*               MG_RET_ERR_SYS_FAILED , failed.
*****************************************************************/
s32 MG_SYSTEM_PowerOff(void);
/*****************************************************************
* Function:     MG_SYSTEM_GetChipTickTime
*
* Description:
*               Get the tick time of the chip.
*
* Return:
*               The tick time of the chip.
*****************************************************************/
u64 MG_SYSTEM_GetChipTickTime(void);
/*****************************************************************
* Function:     MG_SYSTEM_GetChipTickTimeToMs
*
* Description:
*               Convert the tick time of the chip to millisecond.
*
* Return:
*               millisecond.
*****************************************************************/
u64 MG_SYSTEM_GetChipTickTimeToMs(void);
/*****************************************************************
* Function:     MG_SYSTEM_GetModuleInfo
*
* Description:
*               Get the info of the module.
*
* Parameters:
*               info:
*                   [out] Point to a unsigned char buffer.
*               len:
*                   [in] It must be equal or greater than the length of version ID.
*                         Otherwise error code will be returned.
* Return:
*               The length of mode info success.
*               MG_RET_ERR_SYS_FAILED , failed.
*****************************************************************/
s32 MG_SYSTEM_GetModuleInfo(u8 *info, u32 len);
/*****************************************************************
* Function:     MG_SYSTEM_GetModuleVer
*
* Description:
*               Get the version of the module.
*
* Parameters:
*               ver:
*                   [out] Point to a unsigned char buffer.
*               len:
*                   [in] It must be equal or greater than the length of version ID.
*                         Otherwise error code will be returned.
* Return:
*               The length of mode info success.
*               MG_RET_ERR_SYS_FAILED , failed.
*****************************************************************/
s32 MG_SYSTEM_GetModuleVer(u8 *ver, u32 len);
/*****************************************************************************
* Function:     MG_SYSTEM_GetSN
*
* Description:
*               get the SN of SIM.
*
* Parameters:
*               index:
*                       [IN] sn index.
*               sn:
*                       [OUT] the pointer to SN buffer .
*               sn_size:
*                       [IN] the SN buffer size.
* Return:
*               >0, the SN number length.
*               Other values, error code.
*****************************************************************************/
s32 MG_SYSTEM_GetSN(u8 index, u8 *sn, u8 sn_size);
/*****************************************************************
* Function:     MG_SYSTEM_GetBootMode
*
* Description:
*               Get the reboot cause of the module.
*
* Parameters:
*               void
* Return:
*               the reboot cause of the module.
*****************************************************************/
s32 MG_SYSTEM_GetBootMode(void);

/*****************************************************************
* Function:     MG_SYSTEM_GetVbatVol
*
* Description:
*               Get Vbat voltage.
*
* Return:
*               the volt vlaue.
*****************************************************************/
u32 MG_SYSTEM_GetVbatVol(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* __MG_SYS_H__ */
