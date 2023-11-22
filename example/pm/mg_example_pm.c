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

static void prvThreadEntry(void *param)
{
    s32 nRet = 0;
    MG_OS_ThreadSleep(10000);
    APP_DEBUG("mgpm example entry line[%d]", __LINE__);

    nRet = MG_PM_SetT3412(SIM0, (u8 *)"01000101", 8);
    APP_DEBUG("set T3412 line[%d] %d", __LINE__, nRet);
    MG_OS_ThreadSleepLowPower(1000);

    nRet = MG_PM_SetT3324(SIM0, (u8 *)"01100101", 8);
    APP_DEBUG("set T3324 line[%d] %d", __LINE__, nRet);
    MG_OS_ThreadSleepLowPower(1000);

    APP_DEBUG("mgpm example exit line[%d]", __LINE__);
    MG_OS_ThreadSleep(1000);
    MG_OS_ThreadExit();
}

void proc_main_task(void *param)
{
    APP_DEBUG("mg example enter line[%d]", __LINE__);
    MG_OS_ThreadCreate("mgpm", prvThreadEntry, NULL, Thread_PRIORITY_NORMAL, 1024 * 2, 4);
}