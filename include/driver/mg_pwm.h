/**
  ******************************************************************************
  * @file    mg_pwm.h
  * @author  MEIG Development Team
  * @version V1.0.1
  * @date    08-May-2022
  * @brief   This file contains all the functions prototypes for the PWM.
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
#ifndef __MG_PWM_H
#define __MG_PWM_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "mg_type.h"

/** @addtogroup SLM3xx_OpenCPU_Driver
  * @{
  */

/** @addtogroup PWM
  * @brief PWM driver modules
  * @{
  */

/** @defgroup PWM_Exported_Types
  * @{
  */

/**
  * @brief  PWM port number enumeration
  */
typedef enum
{
    PWM_CH0 = 0,
    PWM_CH1,
} Enum_PwmChannel;

typedef enum
{
    FCLK_SEL_26M = 0,
    FCLK_SEL_32K,
} Enum_PwmClockSelet;

/**
  * @}
  */

/** @defgroup PWM_Exported_Constants
  * @{
  */

/**
  * @}
  */

/** @defgroup PWM_Exported_Functions
  * @{
  */

/**
  * @brief  Initializes the PWM channel.
  *
  * @param  ch              Pwm channel in @enum Enum_PwmChannel.
  *
  * @retval
  *         FALSE           Fail.
  *         TRUE            Success.
  */
BOOL MG_PWM_Init(Enum_PwmChannel ch);

/**
  * @brief  Deinitializes the PWM channel.
  *
  * @param  ch              Pwm channel in @enum Enum_PwmChannel.
  *
  * @retval
  *         FALSE           Fail.
  *         TRUE            Success.
  */
BOOL MG_PWM_Deinit(Enum_PwmChannel ch);

/**
  * @brief  Configure the PWM channel.
  *
  * @param  ch              Pwm channel in @enum Enum_PwmChannel.
  *         select          Clock select in @enum Enum_PwmClockSelet.
  *         prescale        Pwm clock division.
  *         freq            Pwm signal frequency in HZ.
  *         dutyCycle       PWm pulse width, the valid range is 0 to 100.
  *
  * @retval
  *         FALSE           Fail.
  *         TRUE            Success.
  */
BOOL MG_PWM_Config(Enum_PwmChannel      ch,
                    Enum_PwmClockSelet  select,
                    u32                 prescale,
                    u32                 freq,
                    u32                 dutyCycle);

/**
  * @brief  Update duty cycle of the PWM channel.
  *
  * @param  ch              Pwm channel in @enum Enum_PwmChannel.
  *         dutyCycle       pwm duty cycle.
  *
  * @retval
  *         FALSE           Fail.
  *         TRUE            Success.
  */
BOOL MG_PWM_UpdateDuty(Enum_PwmChannel ch, u16 dutyCycle);

/**
  * @brief  Start PWM output.
  *
  * @param  ch              Pwm channel in @enum Enum_PwmChannel.
  *
  * @retval
  *         FALSE           Fail.
  *         TRUE            Success.
  */
BOOL MG_PWM_Start(Enum_PwmChannel ch);

/**
  * @brief  Stop PWM output.
  *
  * @param  ch              Pwm channel in @enum Enum_PwmChannel.
  *
  * @retval
  *         FALSE           Fail.
  *         TRUE            Success.
  */
BOOL MG_PWM_Stop(Enum_PwmChannel ch);

/**
  * @}
  */

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

#endif /* __MG_PWM_H */
/******************* (C) COPYRIGHT 2023 MEIG SMART*****END OF FILE*************/