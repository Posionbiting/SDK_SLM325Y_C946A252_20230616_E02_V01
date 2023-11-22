/**
  ******************************************************************************
  * @file    mg_usrl.h
  * @author  MEIG Development Team
  * @version V1.0.1
  * @date    08-Mar-2023
  * @brief   This file contains all the functions prototypes for the USB serial.
  ******************************************************************************
  * @attention
  *
  * THE SOFTWARE IS PROTECTED BY COPYRIGHT AND THE INFORMATION CONTAINED HEREIN
  * IS CONFIDENTIAL. THE SOFTWARE MAY NOT BE COPIED AND THE INFORMATION CONTAINED
  * HEREIN MAY NOT BE USED OR DISCLOSED EXCEPT WITH THE WRITTEN PERMISSION OF
  * MEIG SMART TECHNOLOGY CO., LTD.
  *
  * <h2><center>&copy; COPYRIGHT 2023 MEIG SMART</center></h2>
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MG_USRL_H
#define __MG_USRL_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "mg_type.h"

/** @addtogroup SLM3xx_OpenCPU_Driver
  * @{
  */

/** @addtogroup USRL
  * @brief USRL driver modules
  * @{
  */

/** @defgroup USRL_Exported_Types
  * @{
  */

typedef void (*usrlCallback_t)(u8 *data, u32 size);

/**
  * @}
  */


/** @defgroup USRL_Exported_Constants
  * @{
  */

/**
  * @}
  */

/** @defgroup USRL_Exported_Functions
  * @{
  */

/**
  * @brief  Open the USB serial port.
  *
  * @param  cb              Callback function for usb serial port.
  *
  * @retval FALSE           Execution failed.
  * @retval TRUE            Execution succeeded.
  */
BOOL MG_USRL_Open(usrlCallback_t cb);

/**
  * @brief  Close the USB serial port.
  *
  * @param  None.
  *
  * @retval FALSE           Execution failed.
  * @retval TRUE            Execution succeeded.
  */
BOOL MG_USRL_Close(void);

/**
  * @brief  Send data to USB serial port.
  *
  * @param  data            Pointer to data buf to transmit.
  * @param  size            The length of the data to be send
  *
  * @retval -1              Port is closed, Open it first.
  * @retval -2              Parameters error.
  * @retval -3              Execution failed.
  * @retval  0              Send data success.
  */
s32 MG_USRL_Write(const u8 *data, u64 size);

/**
  * @brief  Receive data from USB serial port.
  *
  * @param  data            Pointer to data buf to transmit.
  * @param  size            The length of the data to be send
  *
  * @retval -1              Port is closed, Open it first.
  * @retval -2              Parameters error.
  * @retval  0              Receive data success.
  */
s32 MG_USRL_Read(u8 *buffer, u64 size);

#ifdef __cplusplus
}
#endif

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

#endif /* __MG_USRL_H */
/******************* (C) COPYRIGHT 2023 MEIG SMART*****END OF FILE*************/