/**
  ******************************************************************************
  * @file    drv_odm_gpio.h
  * @author  MEIG Development Team
  * @version V1.0.3
  * @date    11-Jan-2023
  * @brief   This file contains all the functions prototypes for the GPIO.
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
#ifndef __DRV_ODM_GPIO_H
#define __DRV_ODM_GPIO_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stdlib.h"
#include "stdio.h"
#include "stdbool.h"
#include "gpio.h"

/** @addtogroup SLM3xx_Driver
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
#include "drv_module_pin.h"
    DRV_PIN_INDEX_MAX = 0xFF
} pinNum_e;

/**
  * @brief  GPIO direction enumeration
  */
typedef enum
{
    DRV_GPIO_DIR_IN = 0,
    DRV_GPIO_DIR_OUT,
} gpioDir_e;

/**
  * @brief  GPIO level enumeration
  */
typedef enum
{
    DRV_GPIO_LEVEL_LOW = 0,
    DRV_GPIO_LEVEL_HIGH,
} gpioLevel_e;

/**
  * @brief  GPIO pull type enumeration
  */
typedef enum
{
    DRV_GPIO_PULL_UP = 0,        /**< Select internal pull up */
    DRV_GPIO_PULL_DOWN,          /**< Select internal pull down  */
    DRV_GPIO_PULL_AUTO,          /**< Pull up/down is controlled by muxed alt function */
} gpioPullSel_e;

/**
  * @brief  GPIO interrupt configuration
  */
typedef enum
{
    DRV_GPIO_INTERRUPT_DISABLED     = 0,    /**< Disable interrupt */
    DRV_GPIO_INTERRUPT_LOW_LEVEL    = 1,    /**< Low-level interrupt */
    DRV_GPIO_INTERRUPT_HIGH_LEVEL   = 2,    /**< High-level interrupt */
    DRV_GPIO_INTERRUPT_FALLING_EDGE = 3,    /**< Falling edge interrupt */
    DRV_GPIO_INTERRUPT_RISING_EDGE  = 4,    /**< Rising edge interrupt */
} gpioInterruptConfig_e;

typedef void (*drvGpioCallback)(void *param, bool status);
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
  * @retval false           Initializes the PINx as GPIO fail.
  * @retval true            Initializes the PINx as GPIO success.
  */
bool drvGpioInit(pinNum_e pinNum, gpioDir_e dir, gpioLevel_e level, gpioPullSel_e pullSel);

/**
  * @brief  Deinitializes GPIO driver, disable GPIO clock and perform some clearups.
  *
  * @param  None.
  *
  * @retval false           Deinitializes GPIO function fail.
  * @retval true            Deinitializes GPIO function success.
  */
bool drvGpioDeinit(void);

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
int32_t drvGpioSetLevel(pinNum_e pinNum, gpioLevel_e level);

/**
  * @brief  Get the level of the specified PINx.
  *
  * @param  pinNum          The number of specified module PIN, one value of @Enum_PinNum.
  *
  * @retval -1              The input PIN is invalid.
  * @retval others          The level value of the specified PINx, which is nonnegative integer.
  */
int32_t drvGpioGetLevel(pinNum_e pinNum);

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
int32_t drvGpioSetInterrupt(pinNum_e pinNum, gpioInterruptConfig_e cfg);

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
int32_t drvGpioSetCallback(pinNum_e pinNum, drvGpioCallback cb, void *ctx);

/**
  * @brief  Enable GPIO interrupt in the XIC interrupt controller.
  *
  * @param  None.
  *
  * @retval None.
  */
void drvGpioEnableInterrupt(void);

/**
  * @brief  Disable GPIO interrupt in the XIC interrupt controller.
  *
  * @param  None.
  *
  * @retval None.
  */
void drvGpioDisableInterrupt(void);

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

#endif /* __DRV_ODM_GPIO_H */
/******************* (C) COPYRIGHT 2022 MEIG SMART*****END OF FILE*************/