#ifndef __HTTP_OS_H_
#define __HTTP_OS_H_

#include "http_common.h"

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

typedef  ST_Mutex  MCI_Mutex;

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

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif // End-of __HTTP_OS_H_


