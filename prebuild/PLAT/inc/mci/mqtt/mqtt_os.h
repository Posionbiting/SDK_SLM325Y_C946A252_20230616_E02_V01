#ifndef __MQTT_OS_H_
#define __MQTT_OS_H_

#include "mqtt_common.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ERRORNO */
#define MCI_RET_OK                             MG_RET_OK
#define MCI_RET_ERROR                          MG_RET_ERR

#define MCI_RET_ERROR_SOCK_EINPROGRESS         MG_RET_ERR_SOCK_EINPROGRESS
#define MCI_RET_ERROR_SOCK_EWOULDBLOCK         MG_RET_ERR_SOCK_EWOULDBLOCK

#define MCI_RET_ERROR_SSL_WANTREAD             MG_RET_ERR_SSL_WANTREAD
#define MCI_RET_ERROR_SSL_WANTWRITE            MG_RET_ERR_SSL_WANTWRITE
#define MCI_RET_ERROR_SSL_TIMEOUT              MG_RET_ERR_SSL_TIMEOUT
#define MCI_RET_ERROR_SSL_CRYPTOINPROGRESS     MG_RET_ERR_SSL_CRYPTOINPROGRESS
#define MCI_RET_ERROR_SSL_ASYNCINPROGRESS      MG_RET_ERR_SSL_ASYNCINPROGRESS

#define MCI_RET_ERR_MQTT_ERROR                 MG_RET_ERR_MQTT_ERROR
#define MCI_RET_ERR_MQTT_EINVAL                MG_RET_ERR_MQTT_EINVAL
#define MCI_RET_ERR_MQTT_ENOMEM                MG_RET_ERR_MQTT_ENOMEM
#define MCI_RET_ERR_MQTT_ENOCREATE             MG_RET_ERR_MQTT_ENOCREATE

#define MCI_RET_ERR_MQTT_EBUF_TOO_SHORT        MG_RET_ERR_MQTT_EBUFSHORT
#define MCI_RET_ERR_MQTT_EPACKET_READ          MG_RET_ERR_MQTT_EPACKETREAD
#define MCI_RET_ERR_MQTT_EPACKET_SEND          MG_RET_ERR_MQTT_EPACKETSEND
#define MCI_RET_ERR_MQTT_EPACKET_SERIALIZE     MG_RET_ERR_MQTT_EPACKETSERIAL
#define MCI_RET_ERR_MQTT_EPACKET_DESERIALIZE   MG_RET_ERR_MQTT_EPACKETDESERIAL
#define MCI_RET_ERR_MQTT_EACK_TYPE             MG_RET_ERR_MQTT_EACKTYPE
#define MCI_RET_ERR_MQTT_EACK_TOO_MUCH         MG_RET_ERR_MQTT_EACKTOOMUCH
#define MCI_RET_ERR_MQTT_EREQ_NODE_EXIST       MG_RET_ERR_MQTT_EREQNODEEXIST
#define MCI_RET_ERR_MQTT_EREQ_RECORED          MG_RET_ERR_MQTT_EREQRECORED
#define MCI_RET_ERR_MQTT_ECREQ_SUBACK          MG_RET_ERR_MQTT_ECREQSUBACK
#define MCI_RET_ERR_MQTT_EPUBLISH_DUP          MG_RET_ERR_MQTT_ERROR

#define MCI_RET_ERR_MQTT_ESOCK_GETSOCKETID     MG_RET_ERR_MQTT_ESOCKID
#define MCI_RET_ERR_MQTT_ESOCK_CONFAIL         MG_RET_ERR_MQTT_ESOCKCONFAIL
#define MCI_RET_ERR_MQTT_ESOCK_READ            MG_RET_ERR_MQTT_ESOCKREAD
#define MCI_RET_ERR_MQTT_ESOCK_SEND            MG_RET_ERR_MQTT_ESOCKSEND

#define MCI_RET_ERR_MQTT_ECONTIMEOUT           MG_RET_ERR_MQTT_ECONTIMEOUT
#define MCI_RET_ERR_MQTT_EALREADYCONN          MG_RET_ERR_MQTT_EALREADYCONN
#define MCI_RET_ERR_MQTT_ECONNECTED            MG_RET_ERR_MQTT_ECONNECTED
#define MCI_RET_ERR_MQTT_ECONFAIL              MG_RET_ERR_MQTT_ENOCREATECONN
#define MCI_RET_ERR_MQTT_EDISCONNECTED         MG_RET_ERR_MQTT_EDISCONNECTED
/** Refused protocol version */
#define MCI_RET_ERR_MQTT_EPROTOVERSION         MG_RET_ERR_MQTT_EPROTOVERSION
/** Refused identifier */
#define MCI_RET_ERR_MQTT_EIDENTIFIER           MG_RET_ERR_MQTT_EIDENTIFIER
/** Refused server */
#define MCI_RET_ERR_MQTT_ESERVER               MG_RET_ERR_MQTT_ESERVER
/** Refused user credentials */
#define MCI_RET_ERR_MQTT_EUSERNAME_PASS        MG_RET_ERR_MQTT_EUSERNAME_PASS
/** Refused not authorized */
#define MCI_RET_ERR_MQTT_ENOTAUTHORIZED        MG_RET_ERR_MQTT_ENOTAUTHORIZED


/* MEMORY */
#define MCI_CALLOC(count, size)                MG_MEM_Calloc(count, size)
#define MCI_MALLOC(size)                       MG_MEM_Malloc(size)
#define MCI_FREE(ptr)                          MG_MEM_Free(ptr)

/* TRACE */
#define MCI_TRACE(tag, fmt, ...)               MCI_TRACE_LOGD(tag, fmt, ##__VA_ARGS__)

/* UTILS */
#define MCI_MEMSET(dest, value, size)          MG_UTILS_Memset(dest, value, size)
#define MCI_MEMCPY(dest, src, size)            MG_UTILS_Memcpy(dest, src, size) 
#define MCI_STRLEN(str)                        ((str) ? (MG_UTILS_Strlen(str)) : (0))
#define MCI_STRCMP(s1, s2)                     MG_UTILS_Strcmp(s1, s2)
#define MCI_STRSTR(s1, s2)                     MG_UTILS_Strstr(s1, s2)
#define MCI_STRCHR(s1, ch)                     MG_UTILS_Strchr(s1, ch)
#define MCI_ATOI(s)                            MG_UTILS_Atoi(s)
#define MCI_SNPRINTF(buf, size, ...)           MG_UTILS_Snprintf(buf, size, ##__VA_ARGS__)

/* SYS */
#define MCI_GetChipTickTimeToMs()              MG_SYSTEM_GetChipTickTimeToMs()

/* OS */
#define MCI_DELAY_MAX                          OS_DELAY_MAX

/* THREAD */
#define MCI_Thread_PRIORITY_LOW                Thread_PRIORITY_LOW
#define MCI_Thread_PRIORITY_NORMAL             Thread_PRIORITY_NORMAL
#define MCI_Thread_PRIORITY_HIGH               Thread_PRIORITY_HIGH

#define MCI_EV_MQTT_NOTICE_ID                         0x1F000000

typedef  ST_Mutex      MCI_Mutex;
typedef  ST_Thread     MCI_Thread;
typedef  ST_Event      MCI_Event;
typedef  ST_Semaphore  MCI_Semaphore;

static inline MCI_Mutex *MCI_MutexCreate()
{
    return (MCI_Mutex *)MG_OS_MutexCreate();
}

static inline void MCI_MutexDelete(MCI_Mutex *mutex)
{
    return MG_OS_MutexDelete((ST_Mutex *)mutex);
}

static inline bool MCI_MutexLock(MCI_Mutex *mutex)
{
    return MG_OS_MutexLock((ST_Mutex *)mutex, MCI_DELAY_MAX);
}

static inline void MCI_MutexUnlock(MCI_Mutex *mutex)
{
    return MG_OS_MutexUnlock((ST_Mutex *)mutex);
}

static inline u32 MCI_EnterCritical()
{
    return MG_OS_EnterCritical();
}

static inline void MCI_ExitCritical(u32 critical)
{
    return MG_OS_ExitCritical(critical);
}

//extern ST_Thread *MG_OS_ThreadCreateEX(const char *name, FU_osThreadEntry entry, void *argument,u32 priority, u32 stack_size,u32 eventCount);
static inline MCI_Thread *MCI_ThreadCreate(const char *name, void (*entry)(void *), void *argument, u32 priority, u32 stack_size, u32 eventCount)
{
    return (MCI_Thread *)MG_OS_ThreadCreate(name, entry, argument, priority, stack_size, eventCount);
}

static inline void MCI_ThreadExit()
{
    return MG_OS_ThreadExit();
}

static inline bool MCI_ThreadEventSend(MCI_Thread *thread, MCI_Event *event, s32 timeout)
{
    return MG_OS_EventSend((ST_Thread *)thread, (ST_Event *)event, timeout);
}

static inline bool MCI_ThreadEventWait(MCI_Thread *thread, MCI_Event *event, s32 timeout)
{
    return MG_OS_EventWait((ST_Thread *)thread, (ST_Event *)event, timeout);
}

static inline MCI_Semaphore *MCI_SemaphoreCreate(u32 maxCount, u32 initValue)
{
    return (MCI_Semaphore *)MG_OS_SemaphoreCreate(maxCount, initValue);
}

static inline void MCI_SemaphoreRelease(MCI_Semaphore *sem)
{
    return MG_OS_SemaphoreRelease((ST_Semaphore *)sem);
}

static inline bool MCI_SemaphoreAcquire(MCI_Semaphore *sem, s32 timeout)
{
    return MG_OS_SemaphoreAcquire((ST_Semaphore *)sem, timeout);
}

static inline void MCI_SemaphoreDelete(MCI_Semaphore *sem)
{
    return MG_OS_SemaphoreDelete((ST_Semaphore *)sem);
}

static inline void MCI_ThreadSleep(u32 ms)
{
    return MG_OS_ThreadSleep(ms);
}

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif // End-of __MQTT_OS_H_


