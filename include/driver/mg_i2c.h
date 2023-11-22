/**
  ******************************************************************************
  * @file    mg_i2c.h
  * @author  MEIG Development Team
  * @version V1.0.1
  * @date    08-May-2023
  * @brief   This file contains all the functions prototypes for the I2C.
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
#ifndef __MG_I2C_H
#define __MG_I2C_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "mg_type.h"

/** @addtogroup SLM3xx_OpenCPU_Driver
  * @{
  */

/** @addtogroup I2C
  * @brief I2C driver modules
  * @{
  */

/** @defgroup I2C_Exported_Types
  * @{
  */

/**
  * @brief  I2C channel enumeration
  */
typedef enum
{
    I2C_CH1 = 0,    /**< I2C0 */
    I2C_CH2,        /**< I2C1 */
} Enum_I2cChannel;

/**
  * @brief  I2C bps enumeration
  */
typedef enum
{
    I2C_BPS_100K = 1,   /**< normal 100Kbps */
    I2C_BPS_400K,       /**< fast 400Kbps */
    I2C_BPS_1M,         /**< fast+ 1Mbps */
    I2C_BPS_3P4M,       /**< high speed 3.4Mbps */
} Enum_I2cBps;

/**
  * @}
  */

/** @defgroup I2C_Exported_Constants
  * @{
  */

/**
  * @}
  */

/** @defgroup I2C_Exported_Functions
  * @{
  */

/**
  * @brief  Initialize the I2C master function.
  *
  * @param  ch              I2C channel.
  * @param  bps             The speed of I2C.
  *
  * @retval FALSE           Configure the I2C fail.
  * @retval TRUE            Configure the I2C success.
  */
BOOL MG_I2C_Init(Enum_I2cChannel ch, Enum_I2cBps bps);

/**
  * @brief  Deinitialize the I2C master function.
  *
  * @param  ch              I2C channel.
  *
  * @retval FALSE           Deinitialize the I2C master fail.
  * @retval TRUE            Deinitialize the I2C master success.
  */
BOOL MG_I2C_Deinit(Enum_I2cChannel ch);

/**
  * @brief  Write data to the specified 8bit register(or address) of slave.
  *
  * @param  ch              I2C channel.
  * @param  slave_addr      Slave address.
  * @param  reg_addr        Pointer to a 8bit register address where to write.
  * @param  buf             Pointer to the data to write.
  * @param  size            Size of data to write.
  *
  * @retval FALSE           Write data to slave fail.
  * @retval TRUE            Write data to slave success.
  */
BOOL MG_I2C_MasterSend(Enum_I2cChannel ch, u8 slave_addr, u16 reg_addr, u8 *data, u32 size);

/**
  * @brief  Read data form the specified 8bit register(or address) of slave.
  *
  * @param  ch              I2C channel.
  * @param  slave_addr      Slave address.
  * @param  reg_addr        Pointer to a 8bit register address where to read start.
  * @param  buf             Pointer to the buffer of reading the specified register from slave.
  * @param  size            Size of data to read.
  *
  * @retval FALSE           Read data from slave fail.
  * @retval TRUE            Read data from slave success.
  */
BOOL MG_I2C_MasterRecv(Enum_I2cChannel ch, u8 slave_addr, u16 reg_addr, u8 *buf, u32 size);

BOOL MG_I2C_SlaveSend(Enum_I2cChannel ch, u8 *data, u32 size);

BOOL MG_I2C_SlaveRecv(Enum_I2cChannel ch, u8 *buf, u32 size);

s32 MG_I2C_GetDataCount(Enum_I2cChannel ch);

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

#endif /* __MG_I2C_H */
/******************* (C) COPYRIGHT 2023 MEIG SMART*****END OF FILE*************/