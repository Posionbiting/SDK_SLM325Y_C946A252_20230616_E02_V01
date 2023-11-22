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
#include "mg_time.h"

#define LOG_TAG MAKE_LOG_TAG('E', 'X', 'U', 'T')
#define DEBUG_ENABLE 1
#if DEBUG_ENABLE
#define APP_DEBUG(FORMAT, ...)  MG_TRACE_Printf(LOG_TAG, FORMAT, ##__VA_ARGS__)
#else
#define APP_DEBUG(FORMAT, ...)
#endif

static void prvThreadEntry(void *param)
{
    MG_OS_ThreadSleep(10000);
    APP_DEBUG("mgutils example entry line[%d]", __LINE__);

    /** Random */
    ST_Time time = {0};
    MG_TIME_GetLocalTime(&time);
    u32 seed = (u32)MG_TIME_Mktime(&time);
    u32 randomNumber = MG_UTILS_GetRandom(seed);
    APP_DEBUG("random number: %u", randomNumber);
    MG_OS_ThreadSleep(1000);

    u8 l[3] = {0x31, 0x32, 0x34};
    s8 o[7] = {0};
    if (MG_UTILS_Hex2Str(l, 3, o, 7) > 0)
        APP_DEBUG("Hex 0x%x,0x%x,0x%x to sring %s", l[0], l[1], l[2], o);

    u8 oo[3] = {0};
    if (MG_UTILS_Str2Hex(o, 6, oo, 3) > 0)
        APP_DEBUG("sring %s to Hex 0x%x,0x%x,0x%x", o, oo[0], oo[1], oo[2]);

    u8 buf[20] = {0};
    s32 len = MG_UTILS_Snprintf(buf, 20, "number:%u", randomNumber);
    APP_DEBUG("buf is %s, len = %d", buf, len);

    APP_DEBUG("mgutils example exit line[%d]", __LINE__);
    MG_OS_ThreadSleep(1000);
    MG_OS_ThreadExit();
}

void proc_main_task(void *param)
{
    APP_DEBUG("mg example enter line[%d]", __LINE__);
    MG_OS_ThreadCreate("mgutils", prvThreadEntry, NULL, Thread_PRIORITY_NORMAL, 1024 * 2, 4);
}