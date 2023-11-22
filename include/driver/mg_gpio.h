/**
  ******************************************************************************
  * @file    mg_gpio.h
  * @author  MEIG Development Team
  * @version V1.0.3
  * @date    29-Dec-2022
  * @brief   This file contains all the functions prototypes for the GPIO.
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
#ifndef __MG_GPIO_H
#define __MG_GPIO_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "mg_type.h"

/** @addtogroup SLM3xx_OpenCPU_Driver
  * @{
  */

/** @addtogroup GPIO
  * @brief GPIO driver modules
  * @{
  */

/** @defgroup GPIO_Exported_Types
  * @{
  */

/**
  * @brief  Module PIN number enumeration
  */
typedef enum
{
#include "mg_module_pin.h"
    MG_PIN_INDEX_MAX = 0xFF
} Enum_PinNum;

/**
  * @brief  GPIO direction enumeration
  */
typedef enum
{
    MG_GPIO_DIR_IN = 0,
    MG_GPIO_DIR_OUT,
} Enum_GpioDir;

/**
  * @brief  GPIO level enumeration
  */
typedef enum
{
    MG_GPIO_LEVEL_LOW = 0,
    MG_GPIO_LEVEL_HIGH,
} Enum_GpioLevel;

/**
  * @brief  GPIO pull type enumeration
  */
typedef enum
{
    MG_GPIO_PULL_UP = 0,        /**< Select internal pull up */
    MG_GPIO_PULL_DOWN,          /**< Select internal pull down  */
    MG_GPIO_PULL_AUTO,          /**< Pull up/down is controlled by muxed alt function */
} Enum_GpioPullSel;

/**
  * @brief  GPIO interrupt configuration
  */
typedef enum
{
    MG_GPIO_INTERRUPT_DISABLED     = 0,    /**< Disable interrupt */
    MG_GPIO_INTERRUPT_LOW_LEVEL    = 1,    /**< Low-level interrupt */
    MG_GPIO_INTERRUPT_HIGH_LEVEL   = 2,    /**< High-level interrupt */
    MG_GPIO_INTERRUPT_FALLING_EDGE = 3,    /**< Falling edge interrupt */
    MG_GPIO_INTERRUPT_RISING_EDGE  = 4,    /**< Rising edge interrupt */
} Enum_GpioInterruptConfig;

typedef void (*mg_gpioCallback)(void *param, BOOL status);
/**
  * @}
  */

/** @defgroup GPIO_Exported_Constants
  * @{
  */

/**
  * @}
  */

/** @defgroup GPIO_Exported_Functions
  * @{
  */

/**
  * @brief  Initializes the PINx as GPIO function.
  *
  * @param  pinNum          The number of specified module pin in @Enum_PinNum.
  * @param  dir             The direction of GPIO to configure.
  * @param  level           The initial level of GPIO, only be used in output mode.
  * @param  pullSel         Pull mode select for GPIO.
  *
  * @retval FALSE           Initializes the PINx as GPIO fail.
  * @retval TRUE            Initializes the PINx as GPIO success.
  */
BOOL MG_GPIO_Init(Enum_PinNum pinNum, Enum_GpioDir dir, Enum_GpioLevel level, Enum_GpioPullSel pullSel);

/**
  * @brief  Deinitializes GPIO driver, disable GPIO clock and perform some clearups.
  *
  * @param  None.
  *
  * @retval FALSE           Deinitializes GPIO function fail.
  * @retval TRUE            Deinitializes GPIO function success.
  */
BOOL MG_GPIO_Deinit(void);

/**
  * @brief  Set the level of the specified PINx.
  *
  * @param  pinNum          The number of specified module PIN, one value of @Enum_PinNum.
  * @param  level           The level of GPIO, one value of @Enum_GpioLevel.
  *
  * @retval -1              The input PIN is invalid.
  * @retval 0               Set the level to the specified PINx fail.
  * @retval 1               Set the level to the specified PINx success.
  */
s32 MG_GPIO_SetLevel(Enum_PinNum pinNum, Enum_GpioLevel level);

/**
  * @brief  Get the level of the specified PINx.
  *
  * @param  pinNum          The number of specified module PIN, one value of @Enum_PinNum.
  *
  * @retval -1              The input PIN is invalid.
  * @retval others          The level value of the specified PINx, which is nonnegative integer.
  */
s32 MG_GPIO_GetLevel(Enum_PinNum pinNum);

/**
  * @brief  Configure the specified PINx interrupt type.
  *
  * @param  pinNum          The number of specified module PIN, one value of @Enum_PinNum.
  * @param  cfg             GPIO interrupt type, one value of @Enum_GpioInterruptConfig.
  *
  * @retval -1              The input PIN is invalid.
  * @retval 0               Configure the interrupt function fail.
  * @retval 1               Configure the interrupt function success.
  */
s32 MG_GPIO_SetInterrupt(Enum_PinNum pinNum, Enum_GpioInterruptConfig cfg);

/**
  * @brief  Configure the specified PINx interrupt type.
  *
  * @param  pinNum          The number of specified module PIN, one value of @Enum_PinNum.
  * @param  cb              Callback function for the GPIO interrupt controller.
  * @param  ctx             Parameter point to callback function.
  *
  * @retval -1              The input PIN is invalid.
  * @retval 1               Set the callback function success.
  */
s32 MG_GPIO_SetCallback(Enum_PinNum pinNum, mg_gpioCallback cb, void *ctx);

/**
  * @brief  Enable GPIO interrupt in the XIC interrupt controller.
  *
  * @param  None.
  *
  * @retval None.
  */
void MG_GPIO_EnableInterrupt(void);

/**
  * @brief  Disable GPIO interrupt in the XIC interrupt controller.
  *
  * @param  None.
  *
  * @retval None.
  */
void MG_GPIO_DisableInterrupt(void);

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

#endif /* __MG_GPIO_H */
/******************* (C) COPYRIGHT 2022 MEIG SMART*****END OF FILE*************/