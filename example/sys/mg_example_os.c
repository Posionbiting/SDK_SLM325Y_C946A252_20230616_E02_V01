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
#include "mg_opt_event.h"
#include "mg_os.h"
#include "mg_trace.h"

#define LOG_TAG MAKE_LOG_TAG('E', 'X', 'O', 'S')
#define DEBUG_ENABLE 1
#if DEBUG_ENABLE
#define APP_DEBUG(FORMAT, ...)  MG_TRACE_Printf(LOG_TAG, FORMAT, ##__VA_ARGS__)
#else
#define APP_DEBUG(FORMAT, ...)
#endif

#define EV_SEND_ID (EV_OPEN_EV_BASE_ + 1)
#define EV_END_ID (EV_OPEN_EV_BASE_ + 2)

#define EX_TIMEOUT_MS (1000)

/** low power mode */
bool bLowPowerModeEn = false;
/** test buffer*/
char testbuf[3];
ST_Thread *thread1 = NULL;
ST_Thread *thread2 = NULL;

static void _threadsleep(s32 ms, bool bLowPowerEn)
{
    if (bLowPowerEn)
        MG_OS_ThreadSleepLowPower(ms);
    else
        MG_OS_ThreadSleep(ms);
}

static bool _mutexLock(ST_Mutex *mutex, s32 ms, bool bLowPowerEn)
{
    if (bLowPowerEn)
        return MG_OS_MutexLockLowPower(mutex);
    else
        return MG_OS_MutexLock(mutex, ms);
}

static bool _semaphoreAcquire(ST_Semaphore *sem, s32 ms, bool bLowPowerEn)
{
    if (bLowPowerEn)
        return MG_OS_SemaphoreAcquireLowPower(sem);
    else
        return MG_OS_SemaphoreAcquire(sem, ms);
}

static bool _eventWait(ST_Thread *thread, ST_Event *event, s32 ms, bool bLowPowerEn)
{
    if (bLowPowerEn)
        return MG_OS_EventWaitLowPower(thread, event);
    else
        return MG_OS_EventWait(thread, event, ms);
}

static bool _eventSend(ST_Thread *thread, ST_Event *event, s32 ms, bool bLowPowerEn)
{
    if (bLowPowerEn)
        return MG_OS_EventSendLowPower(thread, event);
    else
        return MG_OS_EventSend(thread, event, ms);
}

static void prvThreadEntry1(void *param)
{
    _threadsleep(10000, bLowPowerModeEn);
    APP_DEBUG("mgOS1 example entry line[%d]", __LINE__);

    ST_Thread *curThread = MG_OS_ThreadCurrent();
    APP_DEBUG("mgOS1 curThread:%x, thread1:%x line[%d]", curThread, thread1, __LINE__);
    _threadsleep(1000, bLowPowerModeEn);

    ST_Event event = {0};
    ST_Mutex *mutex = NULL;
    ST_Semaphore *semaphore = NULL;
    char *pbuffer = NULL;
    s32 cnt = 0;
    while (1)
    {
        if(_eventWait(thread1, &event, EX_TIMEOUT_MS, bLowPowerModeEn))
        {
            if(event.id == EV_SEND_ID)
            {
                mutex = (ST_Mutex *)event.param1;
                semaphore = (ST_Semaphore *)event.param2;
                pbuffer = (char *)event.param3;
                _mutexLock(mutex, EX_TIMEOUT_MS, bLowPowerModeEn);
                pbuffer[0]++;
                cnt = pbuffer[0];
                APP_DEBUG("mgOS1 EV_SEND_ID count %d line[%d]", cnt, __LINE__);
                MG_OS_MutexUnlock(mutex);
                MG_OS_SemaphoreRelease(semaphore);
            }
            else if(event.id == EV_END_ID)
            {
                mutex = (ST_Mutex *)event.param1;
                semaphore = (ST_Semaphore *)event.param2;
                MG_OS_MutexDelete(mutex);
                MG_OS_SemaphoreDelete(semaphore);
                APP_DEBUG("mgOS1 EV_END_ID line[%d]", __LINE__);
                break;
            }
        }
    }

    /** Critical*/
    u32 nCritical = MG_OS_EnterCritical();
    testbuf[0] = 0;
    testbuf[1] = 0;
    MG_OS_ExitCritical(nCritical);

    APP_DEBUG("mgOS1 example exit line[%d]", __LINE__);
    _threadsleep(1000, bLowPowerModeEn);
    MG_OS_ThreadExit();
}

static void prvThreadEntry2(void *param)
{
    _threadsleep(10000, bLowPowerModeEn);
    APP_DEBUG("mgOS2 example entry line[%d]", __LINE__);

    ST_Thread *curThread = MG_OS_ThreadCurrent();
    APP_DEBUG("mgOS2 curThread:%x, thread2:%x line[%d]", curThread, thread2, __LINE__);
    _threadsleep(1000, bLowPowerModeEn);

    /** create mutex */
    ST_Mutex *mutex = MG_OS_MutexCreate();
    if (mutex == NULL)
        APP_DEBUG("mgOS2 create mutex fail line[%d]", __LINE__);
    /** create semaphore */
    ST_Semaphore *semaphore = MG_OS_SemaphoreCreate(1, 0);
    if (semaphore == NULL)
        APP_DEBUG("mgOS2 create semaphore fail line[%d]", __LINE__);
    /** event */
    ST_Event event = {0};
    event.id = EV_SEND_ID;
    event.param1 = (u32)mutex;
    event.param2 = (u32)semaphore;
    event.param3 = (u32)testbuf;

    /** send count*/
    s32 cnt = 0;

    while (1)
    {
        if(!_eventSend(thread1, &event, EX_TIMEOUT_MS, bLowPowerModeEn))
        {
            APP_DEBUG("mgOS2 send event to thread1 fail line[%d]", __LINE__);
            continue;
        }

        if(_semaphoreAcquire(semaphore, EX_TIMEOUT_MS, bLowPowerModeEn))
        {
            _mutexLock(mutex, EX_TIMEOUT_MS, bLowPowerModeEn);
            testbuf[1]++;
            cnt = testbuf[1];
            APP_DEBUG("mgOS2 Acquire Semaphore count %d line[%d]", cnt, __LINE__);
            MG_OS_MutexUnlock(mutex);
        }

        if(cnt == 10)
            break;
    }

    /** end*/
    event.id = EV_END_ID;
    event.param1 = (u32)mutex;
    event.param2 = (u32)semaphore;
    event.param3 = (u32)testbuf;
    _eventSend(thread1, &event, OS_DELAY_MAX, bLowPowerModeEn);

    APP_DEBUG("mgOS2 example exit line[%d]", __LINE__);
    _threadsleep(1000, bLowPowerModeEn);
    MG_OS_ThreadExit();
}

void proc_main_task(void)
{
    APP_DEBUG("mg example enter line[%d]", __LINE__);
    thread1 = MG_OS_ThreadCreate("mgOS1", prvThreadEntry1, NULL, Thread_PRIORITY_NORMAL, 1024 * 2, 4);
    thread2 = MG_OS_ThreadCreate("mgOS2", prvThreadEntry2, NULL, Thread_PRIORITY_NORMAL, 1024 * 2, 4);
}

