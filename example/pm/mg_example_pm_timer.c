/* Copyright (C) 2019 RDA Technologies Limited and/or its affiliates("RDA").
 * All rights reserved.
 *
 * This software is supplied "AS IS" without any warranties.
 * RDA assumes no responsibility or liability for the use of the software,
 * conveys no license or title under any patent, copyright, or mask work
 * right to the product. RDA reserves the right to make changes in the
 * software without notification.  RDA also make no representation or
 * warranty that such application will be suitable for the specified use
 * without further testing or modification.
 */

#include "mg_common.h"
#include "mg_type.h"
#include "mg_error.h"
#include "mg_os.h"
#include "mg_trace.h"
#include "mg_utils.h"
#include "mg_sim.h"
#include "mg_timer.h"
#include "mg_pm.h"

#define LOG_TAG MAKE_LOG_TAG('E', 'X', 'P', 'M')
#define DEBUG_ENABLE 1
#if DEBUG_ENABLE
#define APP_DEBUG(FORMAT, ...)  MG_TRACE_Printf(LOG_TAG, FORMAT, ##__VA_ARGS__)
#else
#define APP_DEBUG(FORMAT, ...)
#endif

#define PM_MODE_NORMAL           0
#define PM_MODE_DEBUG            1

#define PM_MODE                  PM_MODE_NORMAL

void prvIrqTimerCb(void *param)
{
    /** Use MG_TIMER_IrqCreate to establish timer. Blocking function
      * and log printing cannot be used in callback function
     */
    ST_Semaphore *semaphore = (ST_Semaphore *)param;
    MG_OS_SemaphoreRelease(semaphore);
}

static void prvThreadEntry(void *param)
{
    MG_OS_ThreadSleep(10000);
    APP_DEBUG("mgpm example entry line[%d]", __LINE__);

    ST_Semaphore *semaphore = MG_OS_SemaphoreCreate(1, 0);
    /* timer */
    ST_Timer *stTimer = MG_TIMER_IrqCreate(prvIrqTimerCb, (void *)semaphore);
    MG_TIMER_Start(stTimer, 20000, false);

    APP_DEBUG("pm enter sleep line[%d]", __LINE__);
    MG_OS_ThreadSleep(1000);

    /* enter sleep mode */
#if PM_MODE == PM_MODE_NORMAL
    /* close log output */
    MG_PM_EnterSleep(SIM0, NULL);
#else   //PM_MODE == PM_MODE_DEBUG
    /* This is the debugging mode, you may select usb/uart as the log output */
    ST_PmSleepOpt opt = {
        .sleep_mode = PM_SLEEP_MODE_0,
        .Serial_delay = PM_ENTER_SLEEP_DELAY_MS,
        .log_output = PM_SLEEP_LOG_OUTPUT_USB,
    };
    MG_PM_EnterSleep(SIM0, &opt);
#endif

    MG_OS_SemaphoreAcquireLowPower(semaphore);
    MG_PM_ExitSleep(SIM0); // exit sleep mode

    while (1)
    {
        APP_DEBUG("pm timer exit sleep line[%d]", __LINE__);
        MG_OS_ThreadSleep(1000);
    }

    MG_OS_SemaphoreDelete(semaphore);
    APP_DEBUG("mgpm example exit line[%d]", __LINE__);
    MG_OS_ThreadSleep(1000);
    MG_OS_ThreadExit();
}

void proc_main_task(void *param)
{
    APP_DEBUG("mg example enter line[%d]", __LINE__);
    MG_OS_ThreadCreate("mgpm", prvThreadEntry, NULL, Thread_PRIORITY_NORMAL, 1024 * 2, 4);
}