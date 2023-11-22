#include "mg_common.h"
#include "mg_trace.h"
#include "mg_os.h"

#define LOG_TAG MAKE_LOG_TAG('E', 'X', 'C', 'T')

#define DEBUG_ENABLE 1
#if DEBUG_ENABLE
#define APP_DEBUG(FORMAT, ...)  MG_TRACE_Printf(LOG_TAG, FORMAT, ##__VA_ARGS__)
#else
#define APP_DEBUG(FORMAT, ...)
#endif

#if 0
static void prvThreadEntry(void *param)
{
    u32 loopCnt = 0;

    while(1)
    {
        APP_DEBUG("hello world %d line[%d]", loopCnt++, __LINE__);
        MG_OS_ThreadSleep(1000);
    }
}
#endif

void app_main(void);

void proc_main_task(void)
{
    //APP_DEBUG("mg example enter line[%d]", __LINE__);
    //MG_OS_ThreadCreate("CUSTOM", prvThreadEntry, NULL, Thread_PRIORITY_NORMAL, 1024 * 2, 4);
	
	app_main();
}