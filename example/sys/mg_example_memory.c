#include "mg_common.h"
#include "mg_trace.h"
#include "mg_os.h"
#include "mg_mem.h"

#define LOG_TAG MAKE_LOG_TAG('E', 'X', 'M', 'E')
#define DEBUG_ENABLE 1
#if DEBUG_ENABLE
#define APP_DEBUG(FORMAT, ...)  MG_TRACE_Printf(LOG_TAG, FORMAT, ##__VA_ARGS__)
#else
#define APP_DEBUG(FORMAT, ...)
#endif

static void prvThreadEntry(void *param)
{
    MG_OS_ThreadSleep(10000);
    APP_DEBUG("mg_memory example entry line[%d]", __LINE__);
    MG_OS_ThreadSleep(1000);

    u32 *test_malloc = MG_MEM_Malloc(1000);
    APP_DEBUG("malloc 1000 bytes %x line[%d]", test_malloc, __LINE__);
    MG_OS_ThreadSleep(1000);

    if (test_malloc)
        MG_MEM_Free(test_malloc);
    MG_OS_ThreadSleep(1000);

    u32 *test_calloc = MG_MEM_Calloc(2, 1000);
    APP_DEBUG("calloc 1000 bytes  %x line[%d]", test_calloc, __LINE__);
    MG_OS_ThreadSleep(1000);

    if (test_calloc)
        MG_MEM_Free(test_calloc);

	APP_DEBUG("mgmemory example exit line[%d]", __LINE__);
    MG_OS_ThreadSleep(1000);
    MG_OS_ThreadExit();
}

void proc_main_task(void)
{
    APP_DEBUG("mg example enter line[%d]", __LINE__);
    MG_OS_ThreadCreate("mgmemory", prvThreadEntry, NULL, Thread_PRIORITY_NORMAL, 1024 * 2, 4);
}