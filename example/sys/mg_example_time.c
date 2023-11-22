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
#include "mg_time.h"
#include "mg_sim.h"

#define LOG_TAG MAKE_LOG_TAG('E', 'X', 'T', 'E')
#define DEBUG_ENABLE 1
#if DEBUG_ENABLE
#define APP_DEBUG(FORMAT, ...)  MG_TRACE_Printf(LOG_TAG, FORMAT, ##__VA_ARGS__)
#else
#define APP_DEBUG(FORMAT, ...)
#endif

static void _showDate(ST_Time *time)
{
    if (time == NULL)
        return;

    s64 nUTCSeconds = MG_TIME_Mktime(time);
    APP_DEBUG("Mktime is %d UTC", nUTCSeconds);

    s32 nTimezoneSeconds = (time->timezone) * (15 *60);
    s64 totalSeconds = nUTCSeconds + nTimezoneSeconds;

    ST_Time time1 = {0};
    MG_TIME_Gmtime(totalSeconds, &time1);
    APP_DEBUG("local date : %d.%d.%d %d:%d:%d",
            time1.year, time1.month, time1.day, time1.hour, time1.minute, time1.second);
}

static void prvThreadEntry(void *param)
{
    MG_OS_ThreadSleep(10000);
    APP_DEBUG("mgtime example entry line[%d]", __LINE__);

    s32 iResult;
    ST_Time time;
    Enum_SIM_Status state = SIM_STATUS_ABSENT;

    iResult = MG_SIM_GetState(SIM0,&state);
    if(SIM_STATUS_NORMAL == state)
    {
        s32 nTimeZoneSet = 20;
        APP_DEBUG("SIM card inserted line[%d]", __LINE__);
        MG_TIME_SetTimeZone(nTimeZoneSet);
        APP_DEBUG("set time zone %d", nTimeZoneSet);
    }
    else
    {
        APP_DEBUG("SIM card not inserted line[%d]", __LINE__);
        /*set local time 2021-12-10T12:30:18+20
        * GMT time ：2021/12/10 12:30:18
        * the Fifth  East District : 2021/12/10 17:30:18
        * the eighth East District : 2021/12/10 20:30:18
        */
        time.year = 2021;
        time.month = 12;
        time.day = 10;
        time.hour = 12;
        time.minute = 30;
        time.second = 18;
        time.timezone = 20;
        iResult = MG_TIME_SetLocalTime(&time);
        APP_DEBUG("MG_TIME_SetLocalTime(%d.%d.%d %d:%d:%d GMT timezone=%d)=%d",
                time.year, time.month, time.day, time.hour, time.minute, time.second, time.timezone, iResult);
    }

    MG_OS_ThreadSleep(1000);

    /* get local time
     * If you use the Unicom card, you may not be able to obtain the time from the base station.
     * please use NTP to synchronize the time.
    */
    if (MG_TIME_GetLocalTime(&time) == MG_RET_OK)
    {
        APP_DEBUG("get Local time successfuly :%d.%d.%d %d:%d:%d GMT timezone=%d",
                time.year, time.month, time.day, time.hour, time.minute, time.second, time.timezone);

        _showDate(&time);
    }
    else
    {
        APP_DEBUG("get Local time failed line[%d]", __LINE__);
    }

    APP_DEBUG("mg example exit line[%d]", __LINE__);
    MG_OS_ThreadSleep(1000);
    MG_OS_ThreadExit();
}

void proc_main_task(void *param)
{
    APP_DEBUG("mg example enter line[%d]", __LINE__);
    MG_OS_ThreadCreate("mgtime", prvThreadEntry, NULL, Thread_PRIORITY_NORMAL, 1024*2, 4);
}
