/**
  ******************************************************************************
  * @file    mg_example_i2c.c
  * @author  MEIG Development Team
  * @version V1.0.0
  * @date    18-Oct-2022
  * @brief   This file provides a simple I2C application example.
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

#include "mg_trace.h"
#include <stdio.h>
#include "mg_i2c.h"
#include "mg_common.h"
#include "mg_os.h"
#include "string.h"

#define LOG_TAG MAKE_LOG_TAG('E', 'I', '2', 'C')
#define DEBUG_ENABLE 1
#if DEBUG_ENABLE
#define APP_DEBUG(FORMAT, ...)  MG_TRACE_Printf(LOG_TAG, FORMAT, ##__VA_ARGS__)
#else
#define APP_DEBUG(FORMAT, ...)
#endif

// EEPROM device addr
#define EEPROM_DEVICE_ADDR           0x50
#define EEPROM_DEVICE_PAGESIZE       32

u8 writeBuffer[EEPROM_DEVICE_PAGESIZE];
u8 readBuffer[EEPROM_DEVICE_PAGESIZE];

static BOOL EEPROM_byteWrite(u16 addr, u8 value)
{
    return (MG_I2C_MasterSend(I2C_CH1, EEPROM_DEVICE_ADDR, addr, &value, 1));
}

static BOOL EEPROM_pageWrite(u16 addr, u8 *data, u8 length)
{
    return (MG_I2C_MasterSend(I2C_CH1, EEPROM_DEVICE_ADDR, addr, data, length));
}

static BOOL EEPROM_read(uint16_t addr, uint8_t *dataBuf, uint16_t length)
{
    return (MG_I2C_MasterRecv(I2C_CH1, EEPROM_DEVICE_ADDR, addr, dataBuf, length));
}

static void prvThreadEntry(void *param)
{
    u32 i = 0;

    MG_OS_ThreadSleep(1000 * 5);

    MG_I2C_Init(I2C_CH1, I2C_BPS_100K);

    for( i = 0; i < EEPROM_DEVICE_PAGESIZE; i++)
    {
        writeBuffer[i] = i;
    }

    memset(readBuffer, 0x00, EEPROM_DEVICE_PAGESIZE);

    while (1)
    {
        EEPROM_pageWrite(0x00, writeBuffer, EEPROM_DEVICE_PAGESIZE);

        // minimal 5ms interval required between write and read opration
        MG_OS_ThreadSleep(5);

        EEPROM_read(0, readBuffer, EEPROM_DEVICE_PAGESIZE);

        for (i = 0; i < EEPROM_DEVICE_PAGESIZE; i++)
        {
            if (writeBuffer[i] != readBuffer[i])
                break;
        }
        if (!memcmp(readBuffer, writeBuffer, EEPROM_DEVICE_PAGESIZE))
            APP_DEBUG("data write/read success");
        else
            APP_DEBUG("data write/read fail");

        MG_OS_ThreadSleep(100);
    }
}

void proc_main_task(void)
{
    MG_OS_ThreadCreate("mg_i2c", prvThreadEntry, NULL, Thread_PRIORITY_NORMAL, 1024 * 2, 4);
}
