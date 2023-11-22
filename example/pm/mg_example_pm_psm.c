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
#include "mg_sys.h"
#include "mg_sim.h"
#include "mg_pm.h"
#include "mg_network.h"

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

static void appInit(void)
{
	s32 nBootCause = MG_SYSTEM_GetBootMode();
    APP_DEBUG("module reboot cause line[%d] %x", __LINE__, nBootCause);

    MG_PM_ExitSleep(SIM0);
    MG_PM_SetT3324(SIM0, (u8 *)"00100100", 8);
}

static void _netStatus(s32 eventId, ST_NwRegInfo *info, void *param)
{
    ST_NwRegInfo *pInfo = info;
    switch(eventId)
    {
        case EV_OPEN_SIM_CARD_INSERT:
            APP_DEBUG("EV_OPEN_SIM_CARD_INSERT line[%d]", __LINE__);
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

static void _simStatus(void)
{
    s32 ret = 0;
    /** get sim card status*/
    Enum_SIM_Status state = SIM_STATUS_ABSENT;
    while (1)
    {
        state = SIM_STATUS_ABSENT;
        ret = MG_SIM_GetState(SIM0, &state);
        if (state == SIM_STATUS_NORMAL)
        {
            APP_DEBUG("SIM ready", __LINE__);
            return;
        }
        else if (ret == MG_RET_OK)
        {
            APP_DEBUG("No SIM card inserted line[%d]", __LINE__);
        }
        else
        {
            APP_DEBUG("get SIM status fail line[%d]", __LINE__);
        }
        MG_OS_ThreadSleep(1000);
    }
}

void _wakeupInProcess(u8 index, u8 pin_level, void *arg)
{
    if (pin_level == MCI_WAKEUP_IN_PIN_LEVEL_LOW)
    {
        APP_DEBUG("wakeup[%d] is low line[%d]", index, __LINE__);
        ST_Semaphore *semaphore = (ST_Semaphore *)arg;
        MG_OS_SemaphoreRelease(semaphore);
    }
    else
    {
        APP_DEBUG("wakeup[%d] is high line[%d]", index, __LINE__);
    }
}

void _powerKeyProcess(u8 pwrKey_status, void *arg)
{
    if (pwrKey_status == MCI_POWER_KEY_PRESS)
    {
        APP_DEBUG("power key press line[%d]", __LINE__);
        ST_Semaphore *semaphore = (ST_Semaphore *)arg;
        MG_OS_SemaphoreRelease(semaphore);
    }
    else
    {
        APP_DEBUG("power key release line[%d]", __LINE__);
    }
}

static void prvThreadEntry(void *param)
{
    s32 nRet = 0;
    ST_PmSleepOpt opt = {0};
    ST_Semaphore *semaphore = MG_OS_SemaphoreCreate(1, 0);

    MG_OS_ThreadSleep(10000);
    APP_DEBUG("mgpsm example entry line[%d]", __LINE__);

    /*Register*/
    MG_NW_Register(_netStatus, NULL);

    nRet = MG_PM_SetT3324(SIM0, (u8 *)"00100010", 8);
    APP_DEBUG("set T3324 line[%d] %d", __LINE__, nRet);
    MG_OS_ThreadSleepLowPower(1000);

    _simStatus();
    MG_OS_ThreadSleepLowPower(10000);

    /* enter sleep mode */
    opt.sleep_mode = PM_SLEEP_MODE_1;
    opt.Serial_delay = PM_ENTER_SLEEP_DELAY_MS;

#if PM_MODE == PM_MODE_NORMAL
    opt.log_output = PM_SLEEP_LOG_OUTPUT_NO;
#else   //PM_MODE == PM_MODE_DEBUG
    /* This is the debugging mode, you may select usb/uart as the log output */
    opt.log_output = PM_SLEEP_LOG_OUTPUT_USB;
#endif

    opt.pwrKeyCb = _powerKeyProcess;
    opt.pwrKeyCb_arg = (void *)semaphore;
    opt.WakeupInCb = _wakeupInProcess;
    opt.WakeupInCb_arg = (void *)semaphore;

    MG_PM_EnterSleep(SIM0, &opt);

    MG_OS_SemaphoreAcquireLowPower(semaphore);
    MG_PM_ExitSleep(SIM0); // exit sleep mode

    while (1)
    {
        APP_DEBUG("PSM exit line[%d]", __LINE__);
        MG_OS_ThreadSleepLowPower(1000);
    }

    MG_OS_SemaphoreDelete(semaphore);
    APP_DEBUG("mgpsm example exit line[%d]", __LINE__);
    MG_OS_ThreadSleepLowPower(1000);
    MG_OS_ThreadExit();
}

void proc_main_task(void *param)
{
    APP_DEBUG("mg example enter line[%d]", __LINE__);
    appInit();
    MG_OS_ThreadCreate("mgpsm", prvThreadEntry, NULL, Thread_PRIORITY_NORMAL, 1024 * 2, 4);
}