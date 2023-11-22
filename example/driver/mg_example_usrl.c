/**
  ******************************************************************************
  * @file    mg_example_usrl.c
  * @author  MEIG Development Team
  * @version V1.0.0
  * @date    07-Mar-2023
  * @brief   This file provides a simple USB serial port application example.
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
#include "mg_usrl.h"
#include "mg_common.h"
#include "mg_os.h"
#include "string.h"

#define LOG_TAG MAKE_LOG_TAG('U', 'S', 'R', 'L')
#define DEBUG_ENABLE 1
#if DEBUG_ENABLE
#define APP_DEBUG(FORMAT, ...)  MG_TRACE_Printf(LOG_TAG, FORMAT, ##__VA_ARGS__)
#else
#define APP_DEBUG(FORMAT, ...)
#endif

#define THREAD_PRIO         (Thread_PRIORITY_NORMAL)
#define THREAD_STK          (1024 * 16)
#define THREAD_EVENT_CNT    (0)

#define RECV_BUFFER_LEN     (1024)

u8 recv_data[RECV_BUFFER_LEN] = {0x00};

static void prvUsrlCallback(u8 *data, u32 size)
{
    memset(recv_data, 0x00, sizeof(recv_data));
    MG_USRL_Read(recv_data, sizeof(recv_data));
    MG_USRL_Write(recv_data, strlen((char *)recv_data));
}

static void prvThreadEntry(void *param)
{
    MG_OS_ThreadSleep(1000 * 5);

    MG_USRL_Open(prvUsrlCallback);

    while (1)
    {
        MG_OS_ThreadSleep(1000);
    }
}

void proc_main_task(void)
{
    MG_OS_ThreadCreate("mg_usrl", prvThreadEntry, NULL, THREAD_PRIO, THREAD_STK, THREAD_EVENT_CNT);
}
/******************* (C) COPYRIGHT 2023 MEIG SMART*****END OF FILE*************/