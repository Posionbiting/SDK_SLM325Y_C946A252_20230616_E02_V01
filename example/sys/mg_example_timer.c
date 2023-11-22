#include "mg_common.h"
#include "mg_type.h"
#include "mg_error.h"
#include "mg_os.h"
#include "mg_queue.h"
#include "mg_trace.h"
#include "mg_utils.h"
#include "mg_sys.h"
#include "mg_timer.h"

#define LOG_TAG MAKE_LOG_TAG('E', 'X', 'T', 'M')
#define DEBUG_ENABLE 1
#if DEBUG_ENABLE
#define APP_DEBUG(FORMAT, ...)  MG_TRACE_Printf(LOG_TAG, FORMAT, ##__VA_ARGS__)
#else
#define APP_DEBUG(FORMAT, ...)
#endif

/** low power mode */
bool bLowPowerModeEn = false;

/**  Periodic timer count*/
s32 nPeriodicTimerCnt = 0;
/**  Irq periodic timer count */
s32 nIrqOneshotTimerCnt = 0;
/**  Irq periodic timer count */
s32 nIrqPeriodicTimerCnt = 0;

static void _threadsleep(s32 ms, bool bLowPowerEn)
{
    if (bLowPowerEn)
        MG_OS_ThreadSleepLowPower(ms);
    else
        MG_OS_ThreadSleep(ms);
}

static s32 _timerStart(ST_Timer *timer, u32 ms, bool autoRepeat, bool bLowPowerEn)
{
    if (bLowPowerEn)
        return MG_TIMER_StartLowPower(timer, ms, autoRepeat);
    else
        return MG_TIMER_Start(timer, ms, autoRepeat);
}

void prvTimerCb(void *param)
{
    s32 nTimerId = *(s32 *)param;
    if (nTimerId == 0)
    {
        APP_DEBUG("one shot timer entry callback line[%d]", __LINE__);
    }
    else if (nTimerId == 1)
    {
        nPeriodicTimerCnt++;
        APP_DEBUG("Periodic timer entry callback line[%d] cnt %d", __LINE__, nPeriodicTimerCnt);
    }
}

void prvIrqTimerCb(void *param)
{
    /** Use MG_TIMER_IrqCreate to establish timer. Blocking function
      * and log printing cannot be used in callback function */
    s32 nTimerId = *(s32 *)param;
    if (nTimerId == 2)
    {
        nIrqOneshotTimerCnt++;
    }
    else if (nTimerId == 3)
    {
        nIrqPeriodicTimerCnt++;
    }
}

static void prvThreadEntry(void *param)
{
    _threadsleep(10000, bLowPowerModeEn);
    APP_DEBUG("mgtimer example entry line[%d]", __LINE__);

    s32 iret;
    s32 cnt;
    s32 nTimerId;
    ST_Timer *timer;

    /** one shot timer*/
    nTimerId = 0;
    APP_DEBUG("one shot timer line[%d] timer Id : %d", __LINE__, nTimerId);
    timer = MG_TIMER_Create(prvTimerCb, &nTimerId);
    if (timer == NULL)
    {
        APP_DEBUG("one shot timer create fail line[%d]", __LINE__);
    }
    else
    {
        iret = _timerStart(timer, 1000, false, bLowPowerModeEn);
        if (iret != MG_RET_OK)
        {
            APP_DEBUG("one shot timer start fail line[%d], ret = %d", __LINE__,iret);
        }
    }
    _threadsleep(5000, bLowPowerModeEn);
    iret = MG_TIMER_Stop(timer);
    APP_DEBUG("one shot timer stop line[%d] iret : %d", __LINE__, iret);
    iret = MG_TIMER_Delete(timer);
    APP_DEBUG("one shot timer delete line[%d] iret : %d", __LINE__, iret);
    _threadsleep(1000, bLowPowerModeEn);

    /** Periodic timer*/
    nTimerId = 1;
    nPeriodicTimerCnt = 0;
    APP_DEBUG("Periodic timer line[%d] timer Id : %d", __LINE__, nTimerId);
    timer = MG_TIMER_Create(prvTimerCb, &nTimerId);
    if (timer == NULL)
    {
        APP_DEBUG("Periodic timer create fail line[%d]", __LINE__);
    }
    else
    {
        iret = _timerStart(timer, 1000, true, bLowPowerModeEn);
        if (iret != MG_RET_OK)
        {
            APP_DEBUG("Periodic timer start fail line[%d], ret = %d", __LINE__,iret);
        }
    }
    _threadsleep(5000, bLowPowerModeEn);
    iret = MG_TIMER_Stop(timer);
    APP_DEBUG("Periodic timer stop line[%d] iret : %d", __LINE__, iret);
    iret = MG_TIMER_Delete(timer);
    APP_DEBUG("Periodic timer delete line[%d] iret : %d", __LINE__, iret);
    _threadsleep(1000, bLowPowerModeEn);

    /** one shot irq timer*/
    cnt = 0;
    nTimerId = 2;
    nIrqOneshotTimerCnt = 0;
    APP_DEBUG("one shot irq timer line[%d] timer Id : %d", __LINE__, nTimerId);
    timer = MG_TIMER_IrqCreate(prvIrqTimerCb, &nTimerId);
    if (timer == NULL)
    {
        APP_DEBUG("one shot irq timer create fail line[%d]", __LINE__);
    }
    else
    {
        iret = _timerStart(timer, 1000, false, bLowPowerModeEn);
        if (iret != MG_RET_OK)
        {
            APP_DEBUG("one shot irq timer start fail line[%d], ret = %d", __LINE__,iret);
        }
    }
    while (cnt++ < 10)
    {
        _threadsleep(500, bLowPowerModeEn);
        APP_DEBUG("one shot irq timer line[%d] nIrqOneshotTimerCnt %d", __LINE__, nIrqOneshotTimerCnt);
    }

    iret = MG_TIMER_Stop(timer);
    APP_DEBUG("one shot irq timer stop line[%d] iret : %d", __LINE__, iret);
    iret = MG_TIMER_Delete(timer);
    APP_DEBUG("one shot irq timer delete line[%d] iret : %d", __LINE__, iret);
    _threadsleep(1000, bLowPowerModeEn);

    /** Periodic timer*/
    cnt = 0;
    nTimerId = 3;
    nIrqPeriodicTimerCnt = 0;
    APP_DEBUG("Periodic irq timer line[%d] timer Id : %d", __LINE__, nTimerId);
    timer = MG_TIMER_IrqCreate(prvIrqTimerCb, &nTimerId);
    if (timer == NULL)
    {
        APP_DEBUG("Periodic irq timer create fail line[%d]", __LINE__);
    }
    else
    {
        iret = _timerStart(timer, 1000, true, bLowPowerModeEn);
        if (iret != MG_RET_OK)
        {
            APP_DEBUG("Periodic irq timer start fail line[%d], ret = %d", __LINE__,iret);
        }
    }
    while (cnt++ < 10)
    {
        _threadsleep(500, bLowPowerModeEn);
        APP_DEBUG("Periodic irq timer line[%d] nIrqPeriodicTimerCnt %d", __LINE__, nIrqPeriodicTimerCnt);
    }
    iret = MG_TIMER_Stop(timer);
    APP_DEBUG("Periodic irq timer stop line[%d] iret : %d", __LINE__, iret);
    iret = MG_TIMER_Delete(timer);
    APP_DEBUG("Periodic irq timer delete line[%d] iret : %d", __LINE__, iret);
    _threadsleep(1000, bLowPowerModeEn);

    APP_DEBUG("mgtimer example exit line[%d]", __LINE__);
    _threadsleep(1000, bLowPowerModeEn);
    MG_OS_ThreadExit();
}

void proc_main_task(void *param)
{
    APP_DEBUG("mg example enter line[%d]", __LINE__);
    MG_OS_ThreadCreate("mgtime", prvThreadEntry, NULL, Thread_PRIORITY_NORMAL, 1024*2, 4);
}

