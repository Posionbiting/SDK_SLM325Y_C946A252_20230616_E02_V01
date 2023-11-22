#include "mg_common.h"
#include "mg_type.h"
#include "mg_error.h"
#include "mg_os.h"
#include "mg_trace.h"
#include "mg_sim.h"
#include "mg_sys.h"
#include "mg_network.h"

#define LOG_TAG MAKE_LOG_TAG('E', 'S', 'I', 'M')
#define DEBUG_ENABLE 1
#if DEBUG_ENABLE
#define APP_DEBUG(FORMAT, ...)  MG_TRACE_Printf(LOG_TAG, FORMAT, ##__VA_ARGS__)
#else
#define APP_DEBUG(FORMAT, ...)
#endif

u32 nSimSwitchCount = 0;

void _netStatus(s32 eventId, ST_NwRegInfo *info, void *param)
{
    ST_NwRegInfo *pInfo = info;
    switch(eventId)
    {
        case EV_OPEN_SIM_CARD_INSERT:
            APP_DEBUG("EV_OPEN_SIM_CARD_INSERT line[%d]", __LINE__);
            nSimSwitchCount++;
            break;
        case EV_OPEN_SIM_CARD_PULL_OUT:
            APP_DEBUG("EV_OPEN_SIM_CARD_PULL_OUT line[%d]", __LINE__);
            break;
        case EV_OPEN_PS_DOMAIN_NO_REGISTER:
            APP_DEBUG("EV_OPEN_PS_DOMAIN_NO_REGISTER line[%d]", __LINE__);
            break;
        case EV_OPEN_PS_DOMAIN_REGISTER:
            APP_DEBUG("EV_OPEN_PS_DOMAIN_REGISTER line[%d]", __LINE__);
            break;
        case EV_OPEN_PDP_MANUAL_DEACTIVATION:
            APP_DEBUG("EV_OPEN_PDP_MANUAL_DEACTIVATION sim:%d cid:%d line[%d]", pInfo->nSim, pInfo->nCid, __LINE__);
            break;
        case EV_OPEN_PDP_MANUAL_ACTIVATION:
            APP_DEBUG("EV_OPEN_PDP_MANUAL_ACTIVATION sim:%d cid:%d line[%d]", pInfo->nSim, pInfo->nCid, __LINE__);
            break;
        case EV_OPEN_PDP_AUTO_DEACTIVATION:
            APP_DEBUG("EV_OPEN_PDP_AUTO_DEACTIVATION sim:%d cid:%d line[%d]", pInfo->nSim, pInfo->nCid, __LINE__);
            break;
        case EV_OPEN_PDP_AUTO_ACTIVATION:
            APP_DEBUG("EV_OPEN_PDP_AUTO_ACTIVATION sim:%d cid:%d line[%d]", pInfo->nSim, pInfo->nCid, __LINE__);
            break;

        default:
            break;
    }
    MG_OS_ThreadSleep(100);
}

static void prvThreadEntry(void *param)
{
    /* Register Network Callback */
    MG_NW_Register(_netStatus, NULL);

    APP_DEBUG("mg_sim example entry line[%d]", __LINE__);
    MG_OS_ThreadSleep(10000);

    u32 ret = 0;
    u8 hotPlugSwitch = SIM_HOTPLUG_DISABLE;
    u8 hotPlugLevel = SIM_HOTPLUG_HIGH;
    //MG_TRACE_SetLogLevel(MG_TRACE_LOG_LEVEL_VERBOSE);

    ret = MG_SIM_GetOpt(SIM_OPT_HOT_PLUG_SWITCH, &hotPlugSwitch, sizeof(hotPlugSwitch));
    APP_DEBUG("MG_SIM_GetOpt ret:%d, hotPlugSwitch: %d", ret, hotPlugSwitch);
    ret = MG_SIM_GetOpt(SIM_OPT_HOT_PLUG_VOLT_TRIG_LEVEL, &hotPlugLevel, sizeof(hotPlugLevel));
    APP_DEBUG("MG_SIM_GetOpt ret:%d, hotPlugLevel: %d", ret, hotPlugLevel);
    MG_OS_ThreadSleep(1000);

    if (hotPlugSwitch == SIM_HOTPLUG_DISABLE)
    {
        /* Enable SIM card hot plug function */
        hotPlugSwitch = SIM_HOTPLUG_ENABLE;
        hotPlugLevel = SIM_HOTPLUG_LOW;
        ret = MG_SIM_SetOpt(SIM_OPT_HOT_PLUG_SWITCH, &hotPlugSwitch, sizeof(hotPlugSwitch));
        APP_DEBUG("MG_SIM_SetOpt ret:%d, hotPlugSwitch: %d", ret, hotPlugSwitch);
        ret = MG_SIM_SetOpt(SIM_OPT_HOT_PLUG_VOLT_TRIG_LEVEL, &hotPlugLevel, sizeof(hotPlugLevel));
        APP_DEBUG("MG_SIM_SetOpt ret:%d, hotPlugLevel: %d", ret, hotPlugLevel);

        MG_OS_ThreadSleep(5000);
        /** reboot*/
        MG_SYSTEM_Reboot();
    }
    else
    {

        while (nSimSwitchCount < 3)
        {
            APP_DEBUG("nSimSwitchCount %d", nSimSwitchCount);
            MG_OS_ThreadSleep(1000);
        }

        hotPlugSwitch = SIM_HOTPLUG_DISABLE;
        hotPlugLevel = SIM_HOTPLUG_LOW;
        ret = MG_SIM_SetOpt(SIM_OPT_HOT_PLUG_SWITCH, &hotPlugSwitch, sizeof(hotPlugSwitch));
        APP_DEBUG("MG_SIM_SetOpt ret:%d, hotPlugSwitch: %d", ret, hotPlugSwitch);
        ret = MG_SIM_SetOpt(SIM_OPT_HOT_PLUG_VOLT_TRIG_LEVEL, &hotPlugLevel, sizeof(hotPlugLevel));
        APP_DEBUG("MG_SIM_SetOpt ret:%d, hotPlugLevel: %d", ret, hotPlugLevel);

        MG_OS_ThreadSleep(1000);

        ret = MG_SIM_GetOpt(SIM_OPT_HOT_PLUG_SWITCH, &hotPlugSwitch, sizeof(hotPlugSwitch));
        APP_DEBUG("MG_SIM_GetOpt ret:%d, hotPlugSwitch: %d", ret, hotPlugSwitch);
        ret = MG_SIM_SetOpt(SIM_OPT_HOT_PLUG_VOLT_TRIG_LEVEL, &hotPlugLevel, sizeof(hotPlugLevel));
        APP_DEBUG("MG_SIM_SetOpt ret:%d, hotPlugLevel: %d", ret, hotPlugLevel);
        MG_OS_ThreadSleep(1000);
    }

    APP_DEBUG("mgsim example exit line[%d]", __LINE__);
    MG_OS_ThreadSleep(1000);
    MG_OS_ThreadExit();
}

void proc_main_task(void *param)
{
    APP_DEBUG("mg example enter line[%d]", __LINE__);
    MG_OS_ThreadCreate("mgsim", prvThreadEntry, NULL, Thread_PRIORITY_NORMAL, 1024 * 4, 4);
}
