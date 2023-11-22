/**
  ******************************************************************************
  * @file    mg_example_adc.c
  * @author  MEIG Development Team
  * @version V1.0.2
  * @date    01-Feb-2023
  * @brief   This file provides a simple ADC application example.
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

#include "mg_adc.h"
#include "mg_common.h"
#include "mg_trace.h"
#include "mg_os.h"

#define LOG_TAG MAKE_LOG_TAG('E', 'A', 'D', 'C')

#define DEBUG_ENABLE 1
#if DEBUG_ENABLE
#define APP_DEBUG(FORMAT, ...)  MG_TRACE_Printf(LOG_TAG, FORMAT, ##__VA_ARGS__)
#else
#define APP_DEBUG(FORMAT, ...)
#endif

/*
   This project demotrates the usage of ADC, in this example, we keep sampling THERMAL, VBAT, AIO channels.
 */
static void prvThreadEntry(void *param)
{
    MG_OS_ThreadSleep(1000 * 5);

    MG_ADC_channelInit(MG_ADC_CHANNEL_AIO1);
    MG_ADC_channelInit(MG_ADC_CHANNEL_AIO2);
    MG_ADC_channelInit(MG_ADC_CHANNEL_AIO3);
    MG_ADC_channelInit(MG_ADC_CHANNEL_AIO4);
    MG_ADC_channelInit(MG_ADC_CHANNEL_VBAT);
    MG_ADC_channelInit(MG_ADC_CHANNEL_THERMAL);

    while (1)
    {

        APP_DEBUG("Aio1 = %d uV, Aio2 = %d uV, Aio3 = %d uV, Aio4 = %d, uVVbat = %d uV, Thermal = %d",
                    MG_ADC_getChannelValue(MG_ADC_CHANNEL_AIO1),
                    MG_ADC_getChannelValue(MG_ADC_CHANNEL_AIO2),
                    MG_ADC_getChannelValue(MG_ADC_CHANNEL_AIO3),
                    MG_ADC_getChannelValue(MG_ADC_CHANNEL_AIO4),
                    MG_ADC_getChannelValue(MG_ADC_CHANNEL_VBAT),
                    MG_ADC_getChannelValue(MG_ADC_CHANNEL_THERMAL));

        MG_OS_ThreadSleep(1000);
    }
}

void proc_main_task(void)
{
    MG_OS_ThreadCreate("mg_adc", prvThreadEntry, NULL, Thread_PRIORITY_NORMAL, 1024 * 2, 4);
}