/**
  ******************************************************************************
  * @file    mg_spi.h
  * @author  MEIG Development Team
  * @version V1.0.2
  * @date    08-May-2023
  * @brief   This file contains all the functions prototypes for the SPI.
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
#ifndef __MG_SPI_H
#define __MG_SPI_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "mg_type.h"

/** @addtogroup SLM3xx_OpenCPU_Driver
  * @{
  */

/** @addtogroup SPI
  * @brief SPI driver modules
  * @{
  */

/** @defgroup SPI_Exported_Types
  * @{
  */

/**
  * @brief  SPI channel enumeration
  */
typedef enum
{
    SPI_CH1     = 0,
} Enum_SpiChannel;

/**
  * @brief  SPI config structure
  */

typedef struct
{
    BOOL                cpol;       /**< spi idle level: FALSE to low, TRUE to high */
    BOOL                cpha;       /**< edge number to start transfer: FALSE to 1 edge, TRUE to 2 edge */
    u8                  frsize;     /**< frame size, 8 or 16 */
    u8                  order;      /**< Bit order, 0:MSB_LSB, 1:LSB_MSB */
    u8                  master;     /**< master or slave, 0:master, 1:slave, default:0 */
    u32                 freq;       /**< SPI freq */
} ST_SpiConfig;

/**
  * @}
  */

/** @defgroup SPI_Exported_Constants
  * @{
  */

/**
  * @}
  */

/** @defgroup SPI_Exported_Functions
  * @{
  */

/**
  * @brief  Initialize the SPI master function.
  *
  * @param  ch          SPI channel.
  * @param  cfg         Config parameter for SPI.
  *
  * @retval FALSE       Initialize the SPI master fail.
  * @retval TRUE        Initialize the SPI master success.
  */
BOOL MG_SPI_Init(Enum_SpiChannel ch, ST_SpiConfig *cfg);

/**
  * @brief  Deinitialize the SPI master function.
  *
  * @param  ch          SPI channel.
  *
  * @retval FALSE       Deinitialize the SPI master fail.
  * @retval TRUE        Deinitialize the SPI master success.
  */
BOOL MG_SPI_Deinit(Enum_SpiChannel ch);

/**
  * @brief  Start receiving data from SPI interface.
  *
  * @param  ch          SPI channel.
  * @param  data        Pointer to the data reading from salve.
  * @param  size        Length of data to read.
  *
  * @retval FALSE       Read data fail.
  * @retval TRUE        Read data success.
  */
BOOL MG_SPI_Recv(Enum_SpiChannel ch, u8 *data, u32 size);

/**
  * @brief  Start sending data to SPI interface.
  *
  * @param  ch          SPI channel.
  * @param  data        Pointer to the data to send.
  * @param  size        Length of data to send.
  *
  * @retval FALSE       Send data fail.
  * @retval TRUE        Send data success.
  */
BOOL MG_SPI_Send(Enum_SpiChannel ch, u8 *data, u32 size);

/**
  * @brief  Start sending/receiving data to/from SPI.
  *
  * @param  ch          SPI channel.
  * @param  send        Pointer to the data to send.
  * @param  recv        Pointer to the date reading from the slave.
  * @param  size        Length of data to send.
  *
  * @retval FALSE       Send data fail.
  * @retval TRUE        Send data success.
  */
BOOL MG_SPI_Transfer(Enum_SpiChannel ch, u8 *send, u8 *recv, u32 size);

/**
  * @brief  Control the CS pin status of SPI.
  *
  * @param  ch          SPI channel.
  * @param  status      Status of CS pin, FALSE for low level and TRUE for high level.
  *
  * @retval None.
  */
void MG_SPI_PinControl(Enum_SpiChannel ch, BOOL status);

/**
  * @brief  Get transferred data count.
  *
  * @param  ch          SPI channel..
  *
  * @retval Return the data count.
  */
u32 MG_SPI_GetDataCount(Enum_SpiChannel ch);

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
#endif /* __MG_SPI_H */
/******************* (C) COPYRIGHT 2023 MEIG SMART*****END OF FILE*************/