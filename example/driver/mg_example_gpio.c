/**
  ******************************************************************************
  * @file    mg_example_gpio.c
  * @author  MEIG Development Team
  * @version V1.0.0
  * @date    29-Dec-2022
  * @brief   This file provides a simple GPIO application example for SLM325Y.
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
#include "mg_gpio.h"
#include "mg_common.h"
#include "mg_os.h"
#include DEBUG_LOG_HEADER_FILE

#define LOG_TAG MAKE_LOG_TAG('G', 'P', 'I', 'O')

#define DEBUG_ENABLE 1
#if DEBUG_ENABLE
#define APP_DEBUG(FORMAT, ...)  MG_TRACE_Printf(LOG_TAG, FORMAT, ##__VA_ARGS__)
#else
#define APP_DEBUG(FORMAT, ...)
#endif

static void prvGpioTestOutput(void)
{
    MG_GPIO_Init(PIN3, MG_GPIO_DIR_OUT, MG_GPIO_LEVEL_HIGH, MG_GPIO_PULL_UP);
    MG_GPIO_Init(PIN4, MG_GPIO_DIR_OUT, MG_GPIO_LEVEL_HIGH, MG_GPIO_PULL_UP);
    MG_GPIO_Init(PIN10, MG_GPIO_DIR_OUT, MG_GPIO_LEVEL_HIGH, MG_GPIO_PULL_UP);
    MG_GPIO_Init(PIN13, MG_GPIO_DIR_OUT, MG_GPIO_LEVEL_HIGH, MG_GPIO_PULL_UP);
    MG_GPIO_Init(PIN14, MG_GPIO_DIR_OUT, MG_GPIO_LEVEL_HIGH, MG_GPIO_PULL_UP);
    MG_GPIO_Init(PIN15, MG_GPIO_DIR_OUT, MG_GPIO_LEVEL_HIGH, MG_GPIO_PULL_UP);
    MG_GPIO_Init(PIN16, MG_GPIO_DIR_OUT, MG_GPIO_LEVEL_HIGH, MG_GPIO_PULL_UP);
    MG_GPIO_Init(PIN33, MG_GPIO_DIR_OUT, MG_GPIO_LEVEL_HIGH, MG_GPIO_PULL_UP);
    MG_GPIO_Init(PIN34, MG_GPIO_DIR_OUT, MG_GPIO_LEVEL_HIGH, MG_GPIO_PULL_UP);
    MG_GPIO_Init(PIN39, MG_GPIO_DIR_OUT, MG_GPIO_LEVEL_HIGH, MG_GPIO_PULL_UP);
    MG_GPIO_Init(PIN40, MG_GPIO_DIR_OUT, MG_GPIO_LEVEL_HIGH, MG_GPIO_PULL_UP);
    MG_GPIO_Init(PIN48, MG_GPIO_DIR_OUT, MG_GPIO_LEVEL_HIGH, MG_GPIO_PULL_UP);
    MG_GPIO_Init(PIN51, MG_GPIO_DIR_OUT, MG_GPIO_LEVEL_HIGH, MG_GPIO_PULL_UP);
    MG_GPIO_Init(PIN52, MG_GPIO_DIR_OUT, MG_GPIO_LEVEL_HIGH, MG_GPIO_PULL_UP);
    MG_GPIO_Init(PIN54, MG_GPIO_DIR_OUT, MG_GPIO_LEVEL_HIGH, MG_GPIO_PULL_UP);
    MG_GPIO_Init(PIN55, MG_GPIO_DIR_OUT, MG_GPIO_LEVEL_HIGH, MG_GPIO_PULL_UP);
    MG_GPIO_Init(PIN62, MG_GPIO_DIR_OUT, MG_GPIO_LEVEL_HIGH, MG_GPIO_PULL_UP);
    MG_GPIO_Init(PIN63, MG_GPIO_DIR_OUT, MG_GPIO_LEVEL_HIGH, MG_GPIO_PULL_UP);
    MG_GPIO_Init(PIN64, MG_GPIO_DIR_OUT, MG_GPIO_LEVEL_HIGH, MG_GPIO_PULL_UP);
    MG_GPIO_Init(PIN65, MG_GPIO_DIR_OUT, MG_GPIO_LEVEL_HIGH, MG_GPIO_PULL_UP);
    MG_GPIO_Init(PIN66, MG_GPIO_DIR_OUT, MG_GPIO_LEVEL_HIGH, MG_GPIO_PULL_UP);
    MG_GPIO_Init(PIN67, MG_GPIO_DIR_OUT, MG_GPIO_LEVEL_HIGH, MG_GPIO_PULL_UP);

    for (int i = 0; i < 2; i++)
    {
        MG_GPIO_SetLevel(PIN3, MG_GPIO_LEVEL_HIGH);
        MG_GPIO_SetLevel(PIN4, MG_GPIO_LEVEL_HIGH);
        MG_GPIO_SetLevel(PIN10, MG_GPIO_LEVEL_HIGH);
        MG_GPIO_SetLevel(PIN13, MG_GPIO_LEVEL_HIGH);
        MG_GPIO_SetLevel(PIN14, MG_GPIO_LEVEL_HIGH);
        MG_GPIO_SetLevel(PIN15, MG_GPIO_LEVEL_HIGH);
        MG_GPIO_SetLevel(PIN16, MG_GPIO_LEVEL_HIGH);
        MG_GPIO_SetLevel(PIN33, MG_GPIO_LEVEL_HIGH);
        MG_GPIO_SetLevel(PIN34, MG_GPIO_LEVEL_HIGH);
        MG_GPIO_SetLevel(PIN39, MG_GPIO_LEVEL_HIGH);
        MG_GPIO_SetLevel(PIN40, MG_GPIO_LEVEL_HIGH);
        MG_GPIO_SetLevel(PIN48, MG_GPIO_LEVEL_HIGH);
        MG_GPIO_SetLevel(PIN51, MG_GPIO_LEVEL_HIGH);
        MG_GPIO_SetLevel(PIN52, MG_GPIO_LEVEL_HIGH);
        MG_GPIO_SetLevel(PIN54, MG_GPIO_LEVEL_HIGH);
        MG_GPIO_SetLevel(PIN55, MG_GPIO_LEVEL_HIGH);
        MG_GPIO_SetLevel(PIN62, MG_GPIO_LEVEL_HIGH);
        MG_GPIO_SetLevel(PIN63, MG_GPIO_LEVEL_HIGH);
        MG_GPIO_SetLevel(PIN64, MG_GPIO_LEVEL_HIGH);
        MG_GPIO_SetLevel(PIN65, MG_GPIO_LEVEL_HIGH);
        MG_GPIO_SetLevel(PIN66, MG_GPIO_LEVEL_HIGH);
        MG_GPIO_SetLevel(PIN67, MG_GPIO_LEVEL_HIGH);
        MG_OS_ThreadSleep(1000);
        MG_GPIO_SetLevel(PIN3, MG_GPIO_LEVEL_LOW);
        MG_GPIO_SetLevel(PIN4, MG_GPIO_LEVEL_LOW);
        MG_GPIO_SetLevel(PIN10, MG_GPIO_LEVEL_LOW);
        MG_GPIO_SetLevel(PIN13, MG_GPIO_LEVEL_LOW);
        MG_GPIO_SetLevel(PIN14, MG_GPIO_LEVEL_LOW);
        MG_GPIO_SetLevel(PIN15, MG_GPIO_LEVEL_LOW);
        MG_GPIO_SetLevel(PIN16, MG_GPIO_LEVEL_LOW);
        MG_GPIO_SetLevel(PIN33, MG_GPIO_LEVEL_LOW);
        MG_GPIO_SetLevel(PIN34, MG_GPIO_LEVEL_LOW);
        MG_GPIO_SetLevel(PIN39, MG_GPIO_LEVEL_LOW);
        MG_GPIO_SetLevel(PIN40, MG_GPIO_LEVEL_LOW);
        MG_GPIO_SetLevel(PIN48, MG_GPIO_LEVEL_LOW);
        MG_GPIO_SetLevel(PIN51, MG_GPIO_LEVEL_LOW);
        MG_GPIO_SetLevel(PIN52, MG_GPIO_LEVEL_LOW);
        MG_GPIO_SetLevel(PIN54, MG_GPIO_LEVEL_LOW);
        MG_GPIO_SetLevel(PIN55, MG_GPIO_LEVEL_LOW);
        MG_GPIO_SetLevel(PIN62, MG_GPIO_LEVEL_LOW);
        MG_GPIO_SetLevel(PIN63, MG_GPIO_LEVEL_LOW);
        MG_GPIO_SetLevel(PIN64, MG_GPIO_LEVEL_LOW);
        MG_GPIO_SetLevel(PIN65, MG_GPIO_LEVEL_LOW);
        MG_GPIO_SetLevel(PIN66, MG_GPIO_LEVEL_LOW);
        MG_GPIO_SetLevel(PIN67, MG_GPIO_LEVEL_LOW);
        MG_OS_ThreadSleep(1000);
    }
    MG_GPIO_Deinit();
}
static void prvGpioCallback(void *param, BOOL status)
{
    APP_DEBUG("PIN3 interrupt, level = %d", status);
    if (status)
        MG_GPIO_SetInterrupt(PIN3, MG_GPIO_INTERRUPT_FALLING_EDGE);
    else
        MG_GPIO_SetInterrupt(PIN3, MG_GPIO_INTERRUPT_RISING_EDGE);
}

void prvThreadEntry(void *p)
{
    MG_OS_ThreadSleep(1000);

    // Test output mode of GPIO.
    prvGpioTestOutput();

    // Test interrupt mode of GPIO, you can connnect PIN3 and PIN4.
    MG_GPIO_Init(PIN3, MG_GPIO_DIR_IN, MG_GPIO_LEVEL_LOW, MG_GPIO_PULL_UP);
    MG_GPIO_Init(PIN4, MG_GPIO_DIR_OUT, MG_GPIO_LEVEL_LOW, MG_GPIO_PULL_UP);

    MG_GPIO_SetInterrupt(PIN3, MG_GPIO_INTERRUPT_RISING_EDGE);
    MG_GPIO_SetCallback(PIN3, prvGpioCallback, NULL);
    MG_GPIO_EnableInterrupt();

    while(1)
    {
        MG_GPIO_SetLevel(PIN4, MG_GPIO_LEVEL_LOW);
        MG_OS_ThreadSleep(1000 * 1);
        MG_GPIO_SetLevel(PIN4, MG_GPIO_LEVEL_HIGH);
        MG_OS_ThreadSleep(1000 * 1);
    }
}

void proc_main_task(void)
{
    MG_OS_ThreadCreate("mg_gpio", prvThreadEntry, NULL, Thread_PRIORITY_NORMAL, 1024 * 2, 4);
}
/******************* (C) COPYRIGHT 2022 MEIG SMART*****END OF FILE*************/