/**
  ******************************************************************************
  * @file    mg_example_pwm.c
  * @author  MEIG Development Team
  * @version V1.0.0
  * @date    08-Nov-2022
  * @brief   This file provides a simple PWM application example.
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
#include "mg_pwm.h"
#include "mg_common.h"
#include "mg_os.h"
#include "string.h"

#define LOG_TAG MAKE_LOG_TAG('P', 'W', 'M', ' ')
#define DEBUG_ENABLE 1
#if DEBUG_ENABLE
#define APP_DEBUG(FORMAT, ...)  MG_TRACE_Printf(LOG_TAG, FORMAT, ##__VA_ARGS__)
#else
#define APP_DEBUG(FORMAT, ...)
#endif

#define THREAD_PRIO         (Thread_PRIORITY_NORMAL)
#define THREAD_STK          (1024 * 16)
#define THREAD_EVENT_CNT    (0)

#define PWM_FREQ_HZ         (1000 * 10)

static void prvThreadEntry(void *param)
{
    static u16 dutyCycle = 10;

    MG_OS_ThreadSleep(3000);

    MG_PWM_Init(PWM_CH0);
    MG_PWM_Config(PWM_CH0, FCLK_SEL_26M, 1, PWM_FREQ_HZ, dutyCycle);
    MG_PWM_Start(PWM_CH0);

    APP_DEBUG("PWM Example Start\r\n");

    while (1)
    {
        MG_OS_ThreadSleep(1000);
        dutyCycle += 10;

        if (dutyCycle == 100)
            dutyCycle = 0;

        MG_PWM_UpdateDuty(PWM_CH0, dutyCycle);
    }
}

void proc_main_task(void)
{
    MG_OS_ThreadCreate("mg_pwm", prvThreadEntry, NULL, THREAD_PRIO, THREAD_STK, THREAD_EVENT_CNT);
}
/******************* (C) COPYRIGHT 2022 MEIG SMART*****END OF FILE*************/