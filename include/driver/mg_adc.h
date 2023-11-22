/**
  ******************************************************************************
  * @file    mg_adc.h
  * @author  MEIG Development Team
  * @version V1.0.2
  * @date    01-Feb-2023
  * @brief   This file contains all the functions prototypes for the ADC.
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
#ifndef __MG_ADC_H
#define __MG_ADC_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "mg_type.h"

/** @addtogroup SLM3xx_OpenCPU_Driver
  * @{
  */

/** @addtogroup ADC
  * @brief ADC driver modules
  * @{
  */

/** @defgroup ADC_Exported_Types
  * @{
  */

/**
  * @brief List of ADC channels
  */
typedef enum
{
    MG_ADC_CHANNEL_THERMAL          = 0U,      /**< ADC Thermal channel */
    MG_ADC_CHANNEL_VBAT             = 1U,      /**< ADC VBAT channel */
    MG_ADC_CHANNEL_AIO4             = 2U,      /**< ADC AIO4 channel */
    MG_ADC_CHANNEL_AIO3             = 3U,      /**< ADC AIO3 channel */
    MG_ADC_CHANNEL_AIO2             = 4U,      /**< ADC AIO2 channel */
    MG_ADC_CHANNEL_AIO1             = 5U,      /**< ADC AIO1 channel */
} Enum_AdcChannel;

/**
  * @}
  */

/** @defgroup MG_ADC_Exported_Constants
  * @{
  */

/**
  * @}
  */

/** @defgroup MG_ADC_Exported_Functions
  * @{
  */

/**
  * @brief  Initializes ADC specific channel.
  *
  * @param  channel         ADC physical channel to be configured.
  *                         This parameter can be a value of @enum Enum_AdcChannel.
  *
  * @retval -1              Parameter error.
  * @retval -2              AIO1 channel conflict(vref output has been enabled).
  * @retval 0               Initializes the ADC channel success.
  */
s32 MG_ADC_channelInit(Enum_AdcChannel channel);

/**
  * @brief  Deinitialize ADC channel.
  *
  * @param  channel         ADC physical channel to be configured.
  *                         This parameter can be a value of @enum Enum_AdcChannel.
  *
  * @retval None.
  */
void MG_ADC_channelDeInit(Enum_AdcChannel channel);

/**
  * @brief  Return the value for the MG_ADC_CHANNEL_THERMAL channel in degress celsius and other channels in uV.
  *
  * @param  channel         ADC physical channel to be configured.
  *                         This parameter can be a value of @enum Enum_AdcChannel.
  *
  * @retval -1              Failure.
  *         others          The volt or tempperature value of given channel.
  */
s32 MG_ADC_getChannelValue(Enum_AdcChannel channel);

/**
  * @brief  Return the raw adc value for the given channel.
  *
  * @param  channel         ADC physical channel to be configured.
  *                         This parameter can be a value of @enum Enum_AdcChannel.
  *
  * @retval
  *         -1              Failure.
  *         others          The raw value of given channel.
  */
u32 MG_ADC_getRawValue(Enum_AdcChannel channel);

/**
  * @brief  Enable vref output through AIO1.
  *
  * @param  None.
  *
  * @retval -2              AIO1 channel conflict(AIO1 input has been enabled)
  * @retval 0               Success.
  *
  * @note
  *         This feature and AIO1 input is mutual exclusive.
  */
s32 MG_ADC_enableVrefOutput(void);

/**
  * @brief  Disable vref output through AIO1.
  *
  * @param  None.
  *
  * @retval None.
  */
void MG_ADC_disableVrefOutput(void);

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

#endif /* __MG_ADC_H */
/******************* (C) COPYRIGHT 2023 MEIG SMART*****END OF FILE*************/