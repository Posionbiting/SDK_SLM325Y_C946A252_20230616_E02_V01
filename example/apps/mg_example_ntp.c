#include "mg_common.h"
#include "mg_trace.h"
#include "mg_os.h"
#include "mg_ntp.h"
#include "mg_time.h"
#include "mg_network.h"

#define LOG_TAG MAKE_LOG_TAG('E', 'X', 'N', 'T')

#define DEBUG_ENABLE 1
#if DEBUG_ENABLE
#define APP_DEBUG(FORMAT, ...)  MG_TRACE_Printf(LOG_TAG, FORMAT, ##__VA_ARGS__)
#else
#define APP_DEBUG(FORMAT, ...)
#endif

#define NTP_SERVICE            "cn.pool.ntp.org"
//#define NTP_SERVICE           "ntp.aliyun.com"
#define NTP_REQ_TIMEOUT_MS     30000

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

static BOOL _doAct(void)
{
    s32 iResult = 0;
    /*get network state*/
    Enum_NwState nNwState = NW_STATE_NO_REGISTERED;
    iResult = MG_NW_GetNetWorkState(SIM0, PDP_CID_1, &nNwState);
    APP_DEBUG("get network state[%d] iResult:%d, nNwState:%d", __LINE__, iResult, nNwState);
    if (nNwState == NW_STATE_NO_REGISTERED)
    {
        return FALSE;
    }
    else if (nNwState == NW_STATE_LTE_ACTIVED)
    {
        return TRUE;
    }

    return FALSE;
}

static void prvThreadEntry(void *param)
{
    s32 iResult = 0;
    BOOL isAct = FALSE;
    MG_OS_ThreadSleep(10000);
    APP_DEBUG("MG_NTP MainProcTask (%s , %s)", __DATE__, __TIME__);

    /*do act*/
    while (!isAct)
    {
        isAct = _doAct();
        APP_DEBUG("_doAct[%d] isAct:%d", __LINE__, isAct);
        MG_OS_ThreadSleep(1000);
    }

    /*get network state*/
    Enum_NwState nNwState = NW_STATE_NO_REGISTERED;
    iResult = MG_NW_GetNetWorkState(SIM0, PDP_CID_1, &nNwState);
    if(nNwState == NW_STATE_LTE_ACTIVED)
    {
        APP_DEBUG("LTE ACT[%d]", __LINE__);
    }
    else
    {
        APP_DEBUG("get network state[%d] iResult:%d, nNwState:%d", __LINE__, iResult, nNwState);
    }
    MG_OS_ThreadSleep(1000);

    /** set system timezone */
    s32 nTimeZoneSet = 32;
    MG_TIME_SetTimeZone(nTimeZoneSet);
    APP_DEBUG("set time zone %d line[%d]", nTimeZoneSet, __LINE__);

    ST_Time time;
    /** ntp sync time */
    while (1)
    {
        iResult = MG_SNTP_SyncTime(NTP_SERVICE, NTP_REQ_TIMEOUT_MS);
        if (iResult != MG_RET_OK)
        {
            APP_DEBUG("ntp sync time fail line[%d] iResult : %d", __LINE__, iResult);
        }
        else
        {
            /* get local time */
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
        }
        MG_OS_ThreadSleep(1000);
    }

    APP_DEBUG("mgntp example exit line[%d]", __LINE__);
    MG_OS_ThreadSleep(1000);
    MG_OS_ThreadExit();
}

void proc_main_task(void)
{
    APP_DEBUG("mg example enter line[%d]", __LINE__);
    MG_OS_ThreadCreate("mgntp", prvThreadEntry, NULL, Thread_PRIORITY_NORMAL, 1024 * 2, 4);
}
