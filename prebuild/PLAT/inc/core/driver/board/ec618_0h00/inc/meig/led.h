/**
  ******************************************************************************
  * @file    led.h
  * @author  MEIG Development Team
  * @version V1.0.4
  * @date    21-Feb-2023
  * @brief   This file contains all the functions prototypes for the LED.
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
#ifndef __LED_H
#define __LED_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
/* Must include "freeRTOS.h" together with "timers.h" */
#include "FreeRTOS.h"
#include "timers.h"

/** @addtogroup SLM3xx_Driver
  * @{
  */

/** @addtogroup LED
  * @brief LED driver modules
  * @{
  */

/** @defgroup LED_Exported_Types
  * @{
  */

/** \brief duration of LED on or off. Unit: ms */
#define NET_STATUS_SEARCHING_LED_ON_DURATION        (200)
#define NET_STATUS_SEARCHING_LED_OFF_DURATION       (1800)
#define NET_STATUS_STANDBY_LED_ON_DURATION          (1800)
#define NET_STATUS_STANDBY_LED_OFF_DURATION         (200)
#define NET_STATUS_TRANS_LED_ON_DURATION            (125)
#define NET_STATUS_TRANS_LED_OFF_DURATION           (125)

#define LED_ON                                      (1)
#define LED_OFF                                     (0)

#define NET_MODE_LED_ON                             (1)
#define NET_MODE_LED_OFF                            (0)

/** \brief NET STATUS LED location */
#if defined(CONFIG_MG_LED_NET_STATUS_PAD_INDEX)
#define LED_NET_STATUS_PAD_INDEX                    (CONFIG_MG_LED_NET_STATUS_PAD_INDEX)
#else
#define LED_NET_STATUS_PAD_INDEX                    (0xFF)
#endif
#if defined(CONFIG_MG_LED_NET_STATUS_GPIO)
#define LED_NET_STATUS_GPIO                         (CONFIG_MG_LED_NET_STATUS_GPIO)
#else
#define LED_NET_STATUS_GPIO                         (0xFF)
#endif
#define LED_NET_STATUS_PAD_ALT_FUNC                 (PAD_MUX_ALT0)

/** \brief SYS STATUS LED location  */
#if defined(CONFIG_MG_LED_STATUS_PAD_INDEX)
#define LED_SYS_STATUS_PAD_INDEX                    (CONFIG_MG_LED_STATUS_PAD_INDEX)
#else
#define LED_SYS_STATUS_PAD_INDEX                    (0xFF)
#endif
#if defined(CONFIG_MG_LED_STATUS_GPIO)
#define LED_SYS_STATUS_GPIO                         (CONFIG_MG_LED_STATUS_GPIO)
#else
#define LED_SYS_STATUS_GPIO                         (0xFF)
#endif
#define LED_SYS_STATUS_PAD_ALT_FUNC                 (PAD_MUX_ALT0)

/** \brief NET MODE LED location  */
#if defined(CONFIG_MG_LED_NET_MODE_PAD_INDEX)
#define LED_NET_MODE_PAD_INDEX                      (CONFIG_MG_LED_NET_MODE_PAD_INDEX)
#else
#define LED_NET_MODE_PAD_INDEX                      (0xFF)
#endif
#if defined(CONFIG_MG_LED_NET_MODE_GPIO)
#define LED_NET_MODE_GPIO                           (CONFIG_MG_LED_NET_MODE_GPIO)
#else
#define LED_NET_MODE_GPIO                           (0xFF)
#endif
#define LED_NET_MODE_PAD_ALT_FUNC                   (PAD_MUX_ALT0)

/** \brief SYS SLEEP IND LED location  */
#if defined(CONFIG_MG_LED_SLEEP_IND_PAD_INDEX)

#define LED_SLEEP_IND_PAD_INDEX                     (CONFIG_MG_LED_SLEEP_IND_PAD_INDEX)
#else
#define LED_SLEEP_IND_PAD_INDEX                     (0xFF)
#endif
#if defined(CONFIG_MG_LED_SLEEP_IND_GPIO)
#define LED_SLEEP_IND_GPIO                          (CONFIG_MG_LED_SLEEP_IND_GPIO)
#else
#define LED_SLEEP_IND_GPIO                          (0xFF)
#endif
#define LED_SLEEP_IND_PAD_ALT_FUNC                  (PAD_MUX_ALT0)

typedef struct
{
    BOOL                init;           ///< Mark the initialization status of the GPIO instance
    UINT32              port;
    UINT32              pin;
    UINT32              index;
    UINT32              func;
} drvLed_t;

typedef enum
{
    LED_NET_STATUS = 0,                 ///< LED for net status
    LED_SYS_STATUS,                     ///< LED for module status
    LED_NET_MODE,                       ///< LED for net mode
    LED_SYS_SLEEP_IND,                  ///< LED for system sleep status
    LED_NUM_MAX,
} ledId_e;

typedef enum
{
    NET_STATUS_NULL     = -1,
    NET_STATUS_SEARCH,
    NET_STATUS_STANDBY,
    NET_STATUS_TRANS,
    NET_STATUS_CALLING,
} netStatus_e;

typedef enum
{
    NET_MODE_REGISTER_LTE = 0,
    NET_MODE_UNKNOWN,
} netMode_e;

typedef struct
{
    struct
    {
        netStatus_e         status;
        TimerHandle_t       led_on_timer;
        TimerHandle_t       led_off_timer;
        UINT32              led_on_duration;
        UINT32              led_off_duration;
    } sim[2];
} drvNetStatusCtx_t;

/**
  * @}
  */

/** @defgroup GPS_Exported_Functions
  * @{
  */

/**
  * @brief  Write level to the GPIO of LED.
  *
  * @param  id              The id of LED, one value in @enum ledId_e.
  *
  * @retval false           Write fail.
  * @retval true            Write success.
  */
BOOL drvLedLevelWrite(ledId_e id, bool level);

/**
  * @brief  Turn on the NET MODE LED.
  *
  * @param  None.
  *
  * @retval None.
  */
void drvNetModeLedOn(void);

/**
  * @brief  Turn off the NET MODE LED.
  *
  * @param  None.
  *
  * @retval None.
  */
void drvNetModeLedOff(void);

/**
  * @brief  Turn on the NET STATUS LED.
  *
  * @param  nSim            Sim which is using.
  * @param  status          Current status of NET STATUS LED, one value in @enum netStatus_e.
  *
  * @retval None.
  */
void drvNetStatusLedOn(uint8_t nSim, netStatus_e status);

/**
  * @brief  Turn off the NET STATUS LED.
  *
  * @param  nSim            Sim which is using.
  *
  * @retval None.
  */
void drvNetStatusLedOff(uint8_t nSim);

/**
  * @brief  Turn on the SYSTEM STATUS LED.
  *
  * @param  None.
  *
  * @retval None.
  */
void drvSysStatusLedOn(void);

/**
  * @brief  Turn off the SYSTEM STATUS LED.
  *
  * @param  None.
  *
  * @retval None.
  */
void drvSysStatusLedOff(void);

/**
  * @brief  Turn on the SYSTEM SLEEP LED.
  *
  * @param  None.
  *
  * @retval None.
  */
void drvSleepIndLedOn(void);

/**
  * @brief  Turn off the SYSTEM SLEEP LED.
  *
  * @param  None.
  *
  * @retval None.
  */
void drvSleepIndLedOff(void);

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

#endif /* __LED_H */
/******************* (C) COPYRIGHT 2023 MEIG SMART*****END OF FILE*************/