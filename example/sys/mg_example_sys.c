#include "mg_common.h"
#include "mg_type.h"
#include "mg_error.h"
#include "mg_os.h"
#include "mg_trace.h"
#include "mg_utils.h"
#include "mg_sys.h"

#define LOG_TAG MAKE_LOG_TAG('E', 'X', 'S', 'Y')
#define DEBUG_ENABLE 1
#if DEBUG_ENABLE
#define APP_DEBUG(FORMAT, ...)  MG_TRACE_Printf(LOG_TAG, FORMAT, ##__VA_ARGS__)
#else
#define APP_DEBUG(FORMAT, ...)
#endif

static void prvThreadEntry(void *param)
{
    MG_OS_ThreadSleep(10000);
    APP_DEBUG("mgsys example entry line[%d]", __LINE__);

    s32 nRet = 0;
    /** get reboot cause */
    s32 nBootCause = MG_SYSTEM_GetBootMode();
    APP_DEBUG("module reboot cause line[%d] %x", __LINE__, nBootCause);
    MG_OS_ThreadSleep(1000);

    /** get module version */
    u8 nModuleVersion[MODULE_VERSION_MIN_LENGTH] = {0};
    nRet = MG_SYSTEM_GetModuleVer(nModuleVersion, MODULE_VERSION_MIN_LENGTH);
    if (nRet < 0)
    {
        APP_DEBUG("get module version fail line[%d]", __LINE__);
    }
    else
    {
        APP_DEBUG("module version line[%d] %s", __LINE__, nModuleVersion);
    }
    MG_OS_ThreadSleep(1000);

    /** get module info */
    u8 nModuleInfo[MODULE_INFO_MIN_LENGTH] = {0};
    nRet = MG_SYSTEM_GetModuleInfo(nModuleInfo, MODULE_INFO_MIN_LENGTH);
    if (nRet < 0)
    {
        APP_DEBUG("get module info fail line[%d]", __LINE__);
    }
    else
    {
        APP_DEBUG("module info line[%d] %s", __LINE__, nModuleInfo);
    }
    MG_OS_ThreadSleep(1000);

    /** get SN*/
    u8 sn[MODULE_SN_MIN_LENGTH]={0};
    nRet = MG_SYSTEM_GetSN(MODULE_SN1,sn,MODULE_SN_MIN_LENGTH);
    if(nRet > 0)
    {
        APP_DEBUG("module SN line[%d],SN = %s,SN_len = %d", __LINE__, sn, nRet);
    }
    else
    {
        APP_DEBUG("get SN err line[%d]",__LINE__);
    }
    MG_OS_ThreadSleep(1000);

    /** get chip tick */
    u64 tick = MG_SYSTEM_GetChipTickTime();
    APP_DEBUG("get chip tick line[%d] %d", __LINE__, tick);
    MG_OS_ThreadSleep(1000);

    /** get chip tick millisecond*/
    u64 tick_ms = MG_SYSTEM_GetChipTickTimeToMs();
    APP_DEBUG("get chip tick millisecond line[%d] %d", __LINE__, tick_ms);
    MG_OS_ThreadSleep(1000);

    /** get vbat voltage*/
    u32 nVal = MG_SYSTEM_GetVbatVol();
    APP_DEBUG("get vbat voltage line[%d] %ld", __LINE__, nVal);
    MG_OS_ThreadSleep(1000);

#if 0
    /** reboot*/
    MG_SYSTEM_Reboot();
#endif

#if 0
    /** power off*/
    MG_SYSTEM_PowerOff();
#endif

    APP_DEBUG("mgsys example exit line[%d]", __LINE__);
    MG_OS_ThreadSleep(1000);
    MG_OS_ThreadExit();
}

void proc_main_task(void *param)
{
    APP_DEBUG("mg example enter line[%d]", __LINE__);
    MG_OS_ThreadCreate("mgtime", prvThreadEntry, NULL, Thread_PRIORITY_NORMAL, 1024*2, 4);
}


