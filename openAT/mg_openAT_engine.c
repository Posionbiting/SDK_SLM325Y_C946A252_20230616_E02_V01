#include "at_util.h"

#ifdef FEATURE_OPENCPU_AT_ENABLE

#include "mg_openAT_engine.h"

#include "mg_common.h"
#include "mg_trace.h"
#include "mg_os.h"
#include "mg_utils.h"

typedef struct _DataEngine_t
{
    u8 isUse;

    openAtCmdBypassCB_t cb;
    void *cb_ctx;

} ST_DataEngine;

ST_Thread *g_openAtTaskHandle = NULL;

ST_DataEngine g_openAtDataEngine =
{
    .isUse = 0,
    .cb = NULL,
    .cb_ctx = NULL,
};

CmsRetId mg_openAT_InDataProcess(AtChanEntityP pAtChanEty, UINT8 *pInput, UINT16 length)
{
    CmsRetId rc = CMS_FAIL;
    s32 iRet = -1;

    if ( g_openAtDataEngine.isUse )
    {
        if (g_openAtDataEngine.cb)
        {
            iRet = g_openAtDataEngine.cb(g_openAtDataEngine.cb_ctx, pAtChanEty->chanId, (void *)pInput, (u32)length);
            rc = ( iRet < 0 ) ? (CMS_FAIL) : (CMS_RET_SUCC);
        }
    }

    if (rc != CMS_RET_SUCC)
    {
        g_openAtDataEngine.isUse = 0;
        g_openAtDataEngine.cb = NULL;
        g_openAtDataEngine.cb_ctx = NULL;
    }

    return rc;
}

CmsRetId mg_openAT_SetLineMode(UINT8 chanId)
{
    CmsRetId rc = atcChangeChannelState(chanId, ATC_COMMAND_STATE);

    if (rc == CMS_RET_SUCC)
    {
        g_openAtDataEngine.isUse = 0;
        g_openAtDataEngine.cb = NULL;
        g_openAtDataEngine.cb_ctx = NULL;
    }

    return rc;
}

CmsRetId mg_openAT_SetBypassMode(UINT8 chanId, openAtCmdBypassCB_t cb, void *cb_ctx)
{
    CmsRetId rc = atcChangeChannelState(chanId, ATC_OPEN_INPUT_DATA_STATE);

    if (rc == CMS_RET_SUCC)
    {
        g_openAtDataEngine.isUse = 1;
        g_openAtDataEngine.cb = cb;
        g_openAtDataEngine.cb_ctx = cb_ctx;
    }

    return rc;
}

static void prvThreadOpenAtEntry(void *param)
{
    ST_Event event = {0};
    ST_Thread *pThread = MG_OS_ThreadCurrent();

    for (;;)
    {
        MG_UTILS_Memset(&event, 0, sizeof(ST_Event));
        MG_OS_EventWait(pThread, &event, OS_DELAY_MAX);

        switch (event.id)
        {
            case EV_OPEN_AT_EV_CALLBACK:
            {
                openAtCallback_t cb = (openAtCallback_t)event.param1;
                if (cb != NULL)
                    cb((void *)event.param2);

                break;
            }

            default:
                break;
        }
    }
}

BOOL mg_openAT_EngineCallback(openAtCallback_t cb, void *cb_ctx)
{
    bool bRet = false;
    ST_Event event = {0};
    ST_Thread *pThread = mg_openAT_getOpenAtEngine();
    event.id = EV_OPEN_AT_EV_CALLBACK;
    event.param1 = (u32)cb;
    event.param2 = (u32)cb_ctx;

    bRet = MG_OS_EventSend(pThread, &event, OS_DELAY_MAX);

    return (bRet) ? (TRUE) : (FALSE);
}

ST_Thread *mg_openAT_getOpenAtEngine(void)
{
    return g_openAtTaskHandle;
}

void mg_openAt_EngineCreate(void)
{
    g_openAtTaskHandle = MG_OS_ThreadCreate("openAt", (FU_osThreadEntry)prvThreadOpenAtEntry, NULL, Thread_PRIORITY_NORMAL, 1024 * 4, 16);
}

#else

CmsRetId mg_openAT_InDataProcess(AtChanEntityP pAtChanEty, UINT8 *pInput, UINT16 length)
{
    return CMS_FAIL;
}

#endif
