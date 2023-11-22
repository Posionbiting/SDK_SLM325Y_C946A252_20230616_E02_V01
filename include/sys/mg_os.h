/**
  ******************************************************************************
  * @file    mg_os.h
  * @author  MEIG Development Team
  * @version V1.0.0
  * @date    28-Mar-2022
  * @brief   This file provides all the os function APIs.
  ******************************************************************************
  *  @attention
  *
  * THE SOFTWARE IS PROTECTED BY COPYRIGHT AND THE INFORMATION CONTAINED HEREIN
  * IS CONFIDENTIAL. THE SOFTWARE MAY NOT BE COPIED AND THE INFORMATION CONTAINED
  * HEREIN MAY NOT BE USED OR DISCLOSED EXCEPT WITH THE WRITTEN PERMISSION OF
  * MEIG SMART TECHNOLOGY CO., LTD.
  *
  * <h2><center>&copy; COPYRIGHT 2022 MEIG SMART</center></h2>
  ******************************************************************************
  */

#ifndef _MG_OS_H_
#define _MG_OS_H_
#include "mg_type.h"

#ifdef __cplusplus
extern "C" {
#endif

/****************************************************************************
 * opaque data structure for os
 ***************************************************************************/
typedef osiMutex_t ST_Mutex;

typedef osiSemaphore_t ST_Semaphore;

typedef osiThread_t ST_Thread;

typedef osiThreadEntry_t FU_osThreadEntry;

/**
 * event, with ID and 3 parameters
 */
typedef struct _osiEvent
{
    u32 id;     ///< event identifier
    u32 param1; ///< 1st parameter
    u32 param2; ///< 2nd parameter
    u32 param3; ///< 3rd parameter
} ST_Event;

typedef enum _osiThreadPriority
{
    Thread_PRIORITY_IDLE = 1, // reserved
    Thread_PRIORITY_LOW = 8,
    Thread_PRIORITY_BELOW_NORMAL = 16,
    Thread_PRIORITY_NORMAL = 24,
    Thread_PRIORITY_ABOVE_NORMAL = 32,
    Thread_PRIORITY_HIGH = 40,
    Thread_PRIORITY_REALTIME = 48,
    Thread_PRIORITY_HISR = 56, // reserved
}Enum_ThreadPriority;

#define OS_DELAY_MAX (0xFFFFFFFFU)
#define OS_THREAD_STACK_SIZE_DEFAIT (1024)

/******************************************************************************
* Function:     MG_OS_MutexCreate
*
* Description:
*               create a mutex.
*
* Parameters:
*               void
* Return:
*               If no error occurs, MG_OS_MutexCreate returns the mutex
*               pointer. Otherwise, returns NULL.
******************************************************************************/
ST_Mutex *MG_OS_MutexCreate(void);

/******************************************************************************
* Function:     MG_OS_MutexUnlock
*
* Description:
*               unlock the mutex.
*
* Parameters:
*               mutex:
*                       [IN] the mutex pointer.
* Return:
*               void
******************************************************************************/
void MG_OS_MutexUnlock(ST_Mutex *mutex);

/******************************************************************************
* Function:     MG_OS_MutexLock
*
* Description:
*               lock the mutex.
*               When the whole system is in sleep state, the timeout will cause
*               the system to wake up. you can use OS_DELAY_MAX to avoid timeout
* Parameters:
*               mutex:
*                       [IN] the mutex pointer.
*               timeout:
*                       [IN] timeout in milliseconds.
* Return:
*               true on success
*               false on timeout
******************************************************************************/
bool MG_OS_MutexLock(ST_Mutex *mutex, s32 timeout);

/******************************************************************************
* Function:     MG_OS_MutexLockLowPower
*
* Description:
*               the function is a power optimization version of MG_OS_MutexLock,
*               it is blocking.
*
* Parameters:
*               mutex:
*                       [IN] the mutex pointer.
* Return:
*               true on success
*               false on timeout
******************************************************************************/
bool MG_OS_MutexLockLowPower(ST_Mutex *mutex);

/******************************************************************************
* Function:     MG_OS_MutexDelete
*
* Description:
*               delete the mutex.
*
* Parameters:
*               mutex:
*                       [IN] the mutex pointer.
* Return:
*               void
******************************************************************************/
void MG_OS_MutexDelete(ST_Mutex *mutex);

/******************************************************************************
* Function:     MG_OS_SemaphoreCreate
*
* Description:
*               create a semaphore.
*               When maxCount is 1, it is a binary semaphore. Otherwise, it is
*               counting semaphore.
*               the count of the semaphore reduces 1, when MG_OS_SemaphoreAcquire
*               acquires success. if the count of the semaphore is 0,
*               MG_OS_SemaphoreAcquire will acquire fail.
*               the count of the semaphore plus 1, when MG_OS_SemaphoreRelease
*               release success.
* Parameters:
*               maxCount:
*                       [IN] maximum count of the semaphore.
*               initValue:
*                       [IN] initial count of the semaphore.
* Return:
*               If no error occurs, MG_OS_SemaphoreCreate returns the semaphore
*               pointer. Otherwise, returns NULL.
******************************************************************************/
ST_Semaphore *MG_OS_SemaphoreCreate(u32 maxCount, u32 initValue);

/******************************************************************************
* Function:     MG_OS_SemaphoreRelease
*
* Description:
*               release a semaphore.
*
* Parameters:
*               sem:
*                       [IN] the semaphore pointer.
* Return:
*               void
******************************************************************************/
void MG_OS_SemaphoreRelease(ST_Semaphore *sem);

/******************************************************************************
* Function:     MG_OS_SemaphoreAcquire
*
* Description:
*               acquire a semaphore.
*               When the whole system is in sleep state, the timeout will cause
*               the system to wake up. you can use OS_DELAY_MAX to avoid timeout
* Parameters:
*               sem:
*                       [IN] the semaphore pointer.
*               timeout:
*                       [IN] timeout in milliseconds.
* Return:
*               true on success
*               false on timeout
******************************************************************************/
bool MG_OS_SemaphoreAcquire(ST_Semaphore *sem, s32 timeout);

/******************************************************************************
* Function:     MG_OS_SemaphoreAcquireLowPower
*
* Description:
*               the function is a power optimization version of MG_OS_SemaphoreAcquire,
*               it is blocking.
*
* Parameters:
*               sem:
*                       [IN] the semaphore pointer.
* Return:
*               true on success
*               false on timeout
******************************************************************************/
bool MG_OS_SemaphoreAcquireLowPower(ST_Semaphore *sem);

/******************************************************************************
* Function:     MG_OS_SemaphoreDelete
*
* Description:
*               delete a semaphore.
*
* Parameters:
*               sem:
*                       [IN] the semaphore pointer.
* Return:
*               void
******************************************************************************/
void MG_OS_SemaphoreDelete(ST_Semaphore *sem);

/******************************************************************************
* Function:     MG_OS_EventWait
*
* Description:
*               wait an event with timeout.
*               When the whole system is in sleep state, the timeout will cause
*               the system to wake up. you can use OS_DELAY_MAX to avoid timeout
* Parameters:
*               thread:
*                       [IN] the thread Handle.
*               event:
*                       [IN] the event struct.
*               timeout:
*                       [IN] timeout in milliseconds.
* Return:
*               true on success
*               false on timeout
******************************************************************************/
bool MG_OS_EventWait(ST_Thread *thread, ST_Event *event, s32 timeout);

/******************************************************************************
* Function:     MG_OS_EventWaitLowPower
*
* Description:
*               the function is a power optimization version of MG_OS_EventWait,
*               it is blocking.
*
* Parameters:
*               thread:
*                       [IN] the thread Handle.
*               event:
*                       [IN] the event struct.
* Return:
*               true on success
*               false on timeout
******************************************************************************/
bool MG_OS_EventWaitLowPower(ST_Thread *thread, ST_Event *event);

/******************************************************************************
* Function:     MG_OS_EventSend
*
* Description:
*               send an event with timeout.
*               When the whole system is in sleep state, the timeout will cause
*               the system to wake up. you can use OS_DELAY_MAX to avoid timeout
* Parameters:
*               thread:
*                       [IN] the thread Handle.
*               event:
*                       [IN] the event struct.
*               timeout:
*                       [IN] timeout in milliseconds.
* Return:
*               true on success
*               false on timeout
******************************************************************************/
bool MG_OS_EventSend(ST_Thread *thread, ST_Event *event, s32 timeout);

/******************************************************************************
* Function:     MG_OS_EventSendLowPower
*
* Description:
*               the function is a power optimization version of MG_OS_EventSend,
*               it is blocking.
* Parameters:
*               thread:
*                       [IN] the thread Handle.
*               event:
*                       [IN] the event struct.
* Return:
*               true on success
*               false on timeout
******************************************************************************/
bool MG_OS_EventSendLowPower(ST_Thread *thread, ST_Event *event);

/******************************************************************************
* Function:     MG_OS_ThreadCreate
*
* Description:
*               create a thread.
*               if thread stack size less than OS_THREAD_STACK_SIZE_DEFAIT,
*               it Will default to OS_THREAD_STACK_SIZE_DEFAIT.
* Parameters:
*               name:
*                       [IN] the thread name.
*               entry:
*                       [IN] thread entry function.
*               argument:
*                       [IN] thread entry function argument.
*               priority:
*                       [IN] thread priority.
*               stack_size:
*                       [IN] thread stack size in byte.
*               eventCount:
*                       [IN] thread event queue depth (count of events can be hold).
* Return:
*               If no error occurs, MG_OS_ThreadCreate returns the thread
*               pointer. Otherwise, returns NULL.
******************************************************************************/
ST_Thread *MG_OS_ThreadCreate(const char *name, FU_osThreadEntry entry, void *argument,u32 priority, u32 stack_size,u32 eventCount);

/******************************************************************************
* Function:     MG_OS_ThreadCreate
*
* Description:
*               get current thread handle.
*
* Parameters:
*               void
* Return:
*               the current thread handle
******************************************************************************/
ST_Thread *MG_OS_ThreadCurrent(void);

/******************************************************************************
* Function:     MG_OS_ThreadExit
*
* Description:
*               exit thread.
*
* Parameters:
*               void
* Return:
*               void
******************************************************************************/
void MG_OS_ThreadExit(void);

/******************************************************************************
* Function:     MG_OS_ThreadSleep
*
* Description:
*               current thread sleep.
*               if the whole system is in sleep state, it will wake up when
*               the thread sleep time is over.
* Parameters:
*               ms:
*                       [IN] the thread sleep time (milliseconds).
* Return:
*               void
******************************************************************************/
void MG_OS_ThreadSleep(u32 ms);

/******************************************************************************
* Function:     MG_OS_ThreadSleepLowPower
*
* Description:
*               the function is a power optimization version of MG_OS_ThreadSleep,
*               if the whole system is in sleep state, it will not wake up when
*               the thread sleep time is over.
* Parameters:
*               ms:
*                       [IN] the thread sleep time (milliseconds).
* Return:
*               void
******************************************************************************/
void MG_OS_ThreadSleepLowPower(u32 ms);

/******************************************************************************
* Function:     MG_OS_EnterCritical
*
* Description:
*               enter critical section.
*
* Parameters:
*               void
* Return:
*               the critical section flags
******************************************************************************/
u32 MG_OS_EnterCritical(void);

/******************************************************************************
* Function:     MG_OS_ExitCritical
*
* Description:
*               exit critical section.
*
* Parameters:
*               critical:
*                       [IN] the critical section flags.
* Return:
*               void
******************************************************************************/
void MG_OS_ExitCritical(u32 critical);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif // End-of _MG_OS_H_
