/**
  ******************************************************************************
  * @file    mg_uart.h
  * @author  MEIG Development Team
  * @version V1.0.4
  * @date    21-Apr-2023
  * @brief   This file contains all the functions prototypes for the UART.
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
#ifndef __MG_UART_H
#define __MG_UART_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "mg_type.h"

/** @addtogroup SLM3xx_OpenCPU_Driver
  * @{
  */

/** @addtogroup UART
  * @brief UART driver modules
  * @{
  */

/** @defgroup UART_Exported_Types
  * @{
  */

/**
  * @brief  UART port number enumeration
  */
typedef enum
{
    UART_PORT0 = 0,     /**< phy uart0 */
    UART_PORT1,         /**< phy uart1 */
    UART_PORT2,         /**< phy uart2 */
} Enum_UartPort;

/**
  * @brief  Serial device subtype definition
  */
typedef enum
{
    UART_FUN_AT = 0,
    UART_FUN_DATA,      /**< only support for UART_PORT1 */
    UART_FUN_LOG,       /**< only support for UART_PORT0 */
} Enum_UartFunSel;

/**
  * @brief  UART port data bit enumeration
  */
typedef enum
{
    DB_BIT5 = 0,        /**< 5 data bit */
    DB_BIT6,            /**< 6 data bit */
    DB_BIT7,            /**< 7 data bit */
    DB_BIT8             /**< 8 data bit */
} Enum_DataBits;

/**
  * @brief  UART port stop bit enumeration
  */
typedef enum
{
    SB_ONE = 1,         /**< 1 stop bit */
    SB_TWO,             /**< 2 stop bit */
    SB_ONE_DOT_FIVE     /**< 1.5 stop bit */
} Enum_StopBits;

/**
  * @brief  UART port parity bit enumeration
  */
typedef enum
{
    PB_NONE = 0,        /**< none parity bit */
    PB_ODD,             /**< odd parity bit */
    PB_EVEN,            /**< even parity bit */
} Enum_ParityBits;

/**
  * @brief  UART port flow ctrl enumeration
  */
typedef enum
{
    FC_NONE = 1,        /**< None Flow Control */
    FC_HW,              /**< Hardware Flow Control */
    FC_SW               /**< Software Flow Control */
} Enum_FlowCtrl;

typedef struct {
    u32             baudRate;
    Enum_DataBits   dataBits;
    Enum_StopBits   stopBits;
    Enum_ParityBits parity;
    Enum_FlowCtrl   flowCtrl;
    u32             buffSize;
} ST_UartCfg;

/**
  * @}
  */

/** @defgroup UART_Exported_Constants
  * @{
  */

typedef void (*mgUartCallback_t)(Enum_UartPort port, u32 data_len);
/**
  * @}
  */

/** @defgroup UART_Exported_Constants
  * @{
  */

/**
  * @}
  */

/** @defgroup UART_Exported_Functions
  * @{
  */

/**
  * @brief  Initializes the specified UART port.
  *
  * @param  port            The port name.
  *                         This parameter can be a value of @enum Enum_UartPort.
  * @param  cfg             Point to config parameter, refer to @enum Enum_UartCfg.
  * @param  cb              Callback function for uart, event refer to @enum Enum_UartEvent.
  *                         Only for UART_FUN_DATA mode.
  *
  * @retval -1              The port is invalid.
  * @retval 0               Initializes the UARTx success.
  */
s32 MG_UART_Init(Enum_UartPort port, Enum_UartFunSel fun, ST_UartCfg *cfg, mgUartCallback_t cb);

/**
  * @brief  Deinitializes the specified UART port.
  *
  * @param  port            The port name.
  *                         This parameter can be a value of @enum Enum_UartPort.
  *
  * @retval -1              The port is invalid.
  * @retval -2              Parameters error.
  * @retval 0               Deinitializes the UARTx success.
  */
s32 MG_UART_Deinit(Enum_UartPort port);

/**
  * @brief  send data to the specified UART port.
  *
  * @param  port            The port name.
  *                         This parameter can be a value of @enum Enum_UartPort.
  * @param  buf             Pointer to data buf to transmit.
  * @param  size            The length of the data to be send.
  *
  * @retval -1              The port is invalid.
  * @retval -2              Parameters error.
  * @retval 0               Send data success.
  */
s32 MG_UART_Send(Enum_UartPort port, const u8 *buf, u32 size);

/**
  * @brief  Receive data from the specified UART port.
  *
  * @param  port            The port name.
  *                         This parameter can be a value of @enum Enum_UartPort.
  * @param  buf             Pointer to the received data buf.
  * @param  size            The length of the data to receive.
  *
  * @retval -1              The port is invalid.
  * @retval -2              Parameters error.
  * @retval 0               Receive data success.
  */
s32 MG_UART_Receive(Enum_UartPort port, u8 *buf, u32 size);

/**
  * @brief  Receive the length of data in the RX FIFO of the specified UART port.
  *
  * @param  port            The port name.
  *                         This parameter can be a value of @enum Enum_UartPort.
  *
  * @retval -1              The port is invalid.
  * @retval if >= 0         Receive the length success, and the return value
  *                         is the length of actual received data length.
  */
s32 MG_UART_ReadSendAvail(Enum_UartPort port);

/**
  * @brief  Receive the length of data in the TX FIFO of the specified UART port.
  *
  * @param  port            The port name.
  *                         This parameter can be a value of @enum Enum_UartPort.
  *
  * @retval -1              The port is invalid.
  * @retval if >= 0         Receive the length success, and the return value
  *                         is the available space byte count of the TX FIFO.
  */
s32 MG_UART_ReadRecvAvail(Enum_UartPort port);

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

#endif /* __MG_UART_H */
/******************* (C) COPYRIGHT 2023 MEIG SMART*****END OF FILE*************/