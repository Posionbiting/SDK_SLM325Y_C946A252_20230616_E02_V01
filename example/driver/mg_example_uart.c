/**
  ******************************************************************************
  * @file    mg_example_uart.c
  * @author  MEIG Development Team
  * @version V1.0.3
  * @date    02-Feb-2023
  * @brief   This file provides a simple UART application example.
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

#include "mg_trace.h"
#include "mg_uart.h"
#include "mg_common.h"
#include "mg_os.h"
#include "string.h"

#define LOG_TAG MAKE_LOG_TAG('U', 'A', 'R', 'T')
#define DEBUG_ENABLE 1
#if DEBUG_ENABLE
#define APP_DEBUG(FORMAT, ...)  MG_TRACE_Printf(LOG_TAG, FORMAT, ##__VA_ARGS__)
#else
#define APP_DEBUG(FORMAT, ...)
#endif

#define THREAD_PRIO         (Thread_PRIORITY_NORMAL)
#define THREAD_STK          (1024 * 16)
#define THREAD_EVENT_CNT    (0)

#define UART1_FUN_AT        (0x01)
#define UART1_FUN_DATA      (0x02)
#define UART0_FUN_LOG       (0X03)
#define UART0_FUN_DATA      (0x04)
#define UART2_FUN_DATA      (0x05)

//#define UART_DEMO_TYPE      (UART1_FUN_AT)
//#define UART_DEMO_TYPE      (UART1_FUN_DATA)
//#define UART_DEMO_TYPE      (UART0_FUN_LOG)
//#define UART_DEMO_TYPE      (UART0_FUN_DATA)
//#define UART_DEMO_TYPE      (UART2_FUN_DATA)

#define RECV_BUFFER_LEN     (1024)

ST_Semaphore    *uart_sem = NULL;
u8 recv_data[RECV_BUFFER_LEN];

#if (UART_DEMO_TYPE == UART1_FUN_AT)
static void prvThreadEntry(void *param)
{
    Enum_UartPort port = UART_PORT1;
    u8 *greetStr = (u8 *)"Configure UART1 as AT port\n";

    ST_UartCfg cfg =
    {
        .baudRate = 115200,
        .dataBits = DB_BIT8,
        .stopBits = SB_ONE,
        .parity = PB_NONE,
        .flowCtrl = FC_NONE,
    };

    MG_OS_ThreadSleep(1000 * 5);
    MG_UART_Init(port, UART_FUN_AT, &cfg, NULL);
    MG_UART_Send(port, greetStr, strlen((const char *)greetStr));

    while (1)
    {
        MG_OS_ThreadSleep(1000);
    }
}
#elif (UART_DEMO_TYPE == UART1_FUN_DATA)
static void prvUartCallback(Enum_UartPort port, u32 data_len)
{
    memset(recv_data, 0x00, sizeof(recv_data));
    MG_UART_Receive(port, recv_data, data_len);
    MG_OS_SemaphoreRelease(uart_sem);
}

static void prvThreadEntry(void *param)
{
    Enum_UartPort port = UART_PORT1;
    u8 *greetStr = (u8 *)"UART1 Echo Demo\n";

    ST_UartCfg cfg =
    {
        .baudRate = 115200,
        .dataBits = DB_BIT8,
        .stopBits = SB_ONE,
        .parity = PB_NONE,
        .flowCtrl = FC_NONE,
        .buffSize = RECV_BUFFER_LEN,
    };

    MG_OS_ThreadSleep(1000 * 5);
    uart_sem = MG_OS_SemaphoreCreate(3, 0);
    MG_UART_Init(port, UART_FUN_DATA, &cfg, prvUartCallback);
    MG_UART_Send(port, greetStr, strlen((const char *)greetStr));

    while (1)
    {
        if (MG_OS_SemaphoreAcquire(uart_sem, 0xFFFFFFFF))
        {
            MG_UART_Send(port, recv_data, strlen((char *)recv_data));
        }
    }
}
#elif (UART_DEMO_TYPE == UART0_FUN_DATA)
static void prvUartCallback(Enum_UartPort port, u32 data_len)
{
    memset(recv_data, 0x00, sizeof(recv_data));
    MG_UART_Receive(port, recv_data, data_len);
    MG_OS_SemaphoreRelease(uart_sem);
}

static void prvThreadEntry(void *param)
{
    Enum_UartPort port = UART_PORT0;
    u8 *greetStr = (u8 *)"UART0 Echo Demo\n";

    ST_UartCfg cfg =
    {
        .baudRate = 115200,
        .dataBits = DB_BIT8,
        .stopBits = SB_ONE,
        .parity = PB_NONE,
        .flowCtrl = FC_NONE,
        .buffSize = RECV_BUFFER_LEN,
    };

    MG_OS_ThreadSleep(1000 * 5);
    uart_sem = MG_OS_SemaphoreCreate(3, 0);
    MG_UART_Init(port, UART_FUN_DATA, &cfg, prvUartCallback);
    MG_UART_Send(port, greetStr, strlen((const char *)greetStr));

    while (1)
    {
        if (MG_OS_SemaphoreAcquire(uart_sem, 0xFFFFFFFF))
        {
            MG_UART_Send(port, recv_data, strlen((char *)recv_data));
        }
    }
}

#elif (UART_DEMO_TYPE == UART0_FUN_LOG)
static void prvThreadEntry(void *param)
{
    Enum_UartPort port = UART_PORT0;

    MG_OS_ThreadSleep(1000 * 5);
    //Baudrate:3000000
    MG_UART_Init(port, UART_FUN_LOG, NULL, NULL);

    while (1)
    {
        APP_DEBUG("export log from UART0");
        MG_OS_ThreadSleep(1000);
    }
}

#elif (UART_DEMO_TYPE == UART2_FUN_DATA)

static void prvUartCallback(Enum_UartPort port, u32 data_len)
{
    memset(recv_data, 0x00, sizeof(recv_data));
    MG_UART_Receive(port, recv_data, data_len);
    MG_OS_SemaphoreRelease(uart_sem);
}

static void prvThreadEntry(void *param)
{
    Enum_UartPort port = UART_PORT2;
    u8 *greetStr = (u8 *)"UART2 Echo Demo\n";

    ST_UartCfg cfg =
    {
        .baudRate = 115200,
        .dataBits = DB_BIT8,
        .stopBits = SB_ONE,
        .parity = PB_NONE,
        .flowCtrl = FC_NONE,
        .buffSize = RECV_BUFFER_LEN,
    };

    MG_OS_ThreadSleep(1000 * 5);
    uart_sem = MG_OS_SemaphoreCreate(3, 0);
    MG_UART_Init(port, UART_FUN_DATA, &cfg, prvUartCallback);
    MG_UART_Send(port, greetStr, strlen((const char *)greetStr));

    while (1)
    {
        if (MG_OS_SemaphoreAcquire(uart_sem, 0xFFFFFFFF))
        {
            MG_UART_Send(port, recv_data, strlen((char *)recv_data));
        }
    }
}
#else
static void prvThreadEntry(void *param)
{
    while (1)
    {
        MG_OS_ThreadSleep(1000);
    }
}

#endif

void proc_main_task(void)
{
    MG_OS_ThreadCreate("mg_uart", prvThreadEntry, NULL, THREAD_PRIO, THREAD_STK, THREAD_EVENT_CNT);
}
/******************* (C) COPYRIGHT 2023 MEIG SMART*****END OF FILE*************/