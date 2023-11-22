#include "app_include.h"
#include "mg_http.h"
#include "mg_fota.h"

#define LOG_TAG MAKE_LOG_TAG('U', 'S', 'E', 'R')

#define HTTP_REQUESR_TIMOUT_MS      30*1000

enum
{
    APP_FOTA_STREAM_HTTP_STATE_IDLE     = 0,
    APP_FOTA_STREAM_HTTP_STATE_START    = 1,
    APP_FOTA_STREAM_HTTP_STATE_WRITE    = 2,
    APP_FOTA_STREAM_HTTP_STATE_END      = 3
};

static s32 g_fotaStreamHttpState = APP_FOTA_STREAM_HTTP_STATE_IDLE;
static ST_Semaphore *g_ota_semaphore = NULL;

static s32 _fota_http_stream_state_get(void)
{
    return g_fotaStreamHttpState;
}

static void _fota_http_stream_state_set(s32 state)
{
    u32 nCritical = MG_OS_EnterCritical();
    g_fotaStreamHttpState = state;
    MG_OS_ExitCritical(nCritical);
}

static s32 fota_http_stream_input_data(s32 err, s32 statusCode, u8 *data, u32 data_len, void *param, s32 isEnd)
{
    s32 iret = MG_RET_OK;
    s32 state = _fota_http_stream_state_get();
    u32 remainLen = 0;
    u8 *pDate = NULL;
    s32 nWrSize = 0;
    s32 nPos = 0;
    static u32 totalLen = 0;

    if (err != MG_RET_OK)
    {
        APP_DEBUG("[%d] fota input data fail err=%d line[%d]", __LINE__, err);
        if (state == APP_FOTA_STREAM_HTTP_STATE_WRITE)
        {
            MG_FOTA_Finish();
        }
        _fota_http_stream_state_set(APP_FOTA_STREAM_HTTP_STATE_END);
        return err;
    }

    if (statusCode != 200)
    {
        APP_DEBUG("[%d] fota input data fail statusCode=%d", __LINE__, statusCode);
        return MG_RET_ERR;
    }

    switch (state)
    {
        case APP_FOTA_STREAM_HTTP_STATE_START:
        {
            iret = MG_FOTA_Init();
            if (iret != MG_RET_OK)
            {
                APP_DEBUG("[%d] fota init fail", __LINE__);
                break;
            }

            iret = MG_FOTA_Start();
            if (iret != MG_RET_OK)
            {
                APP_DEBUG("[%d] fota start fail", __LINE__);
                break;
            }

            totalLen = 0;
            _fota_http_stream_state_set(APP_FOTA_STREAM_HTTP_STATE_WRITE);
        }

        case APP_FOTA_STREAM_HTTP_STATE_WRITE:
        {
            pDate = data;
            remainLen = data_len;
            while (remainLen > 0)
            {
                nWrSize = MG_FOTA_WriteData(pDate+nPos, remainLen);
                totalLen += nWrSize;
                APP_DEBUG("[%d] fota write, nWrSize %d totalLen %d", __LINE__, nWrSize, totalLen);
                if (nWrSize < 0)
                {
                    iret = nWrSize;
                    break;
                }
                remainLen = remainLen - nWrSize;
                nPos = nPos + nWrSize;
            }

            break;
        }

        default:
            iret = MG_RET_ERR;
            break;
    }

    if (isEnd)
    {
        if (state == APP_FOTA_STREAM_HTTP_STATE_WRITE)
        {
            MG_FOTA_Finish();
        }
        iret = MG_FOTA_Update();
        APP_DEBUG("[%d] fota update iret %d", __LINE__, iret);
        _fota_http_stream_state_set(APP_FOTA_STREAM_HTTP_STATE_END);
    }

    return iret;
}

static void _fota_http_stream(u8 cid, const char *url)
{
    s32 iResult = 0;
    ST_HttpInfo httpinfo = {0};
    ST_HttpRequestCtx *request = NULL;

    httpinfo.cid = cid;
    if (strstr((char*)url, "https://"))
    {
        httpinfo.vsn = HTTP_TLS_VERSION_TLS1_2;
        httpinfo.verify = HTTP_TLS_VERIFY_NONE;
        httpinfo.cacert = NULL;
    }

    request = MG_HTTP_Create(&httpinfo);
    if (NULL == request)
        return;

    /* register http callback */
    MG_HTTP_Register(request, HTTP_REG_TYPE_CONTENT_INPUT_DATA, fota_http_stream_input_data, (void *)NULL);

    /* use default header
        * you may set option HTTP_OPT_USR_REQUEST_HEADER to config usr's header.
    */

    /* set fota state */
    _fota_http_stream_state_set(APP_FOTA_STREAM_HTTP_STATE_START);

    iResult = MG_HTTP_Get(request, url, NULL, HTTP_REQUESR_TIMOUT_MS);
    if (iResult < 0)
    {
        /** clear the previously refreshed pack data*/
        MG_FOTA_Init();
        APP_DEBUG("[%d] fota update fail", __LINE__);
    }
    else
    {
        APP_DEBUG("[%d] fota update success", __LINE__);
        app_util_threadSleep(5*1000, g_app_cfg.bLowPowerModeEn);
        /** reboot*/
        MG_SYSTEM_Reboot();
    }

    // close request
    MG_HTTP_Close(request);
    // destoty request context
    MG_HTTP_Destroy(request);
}

static void app_fota_thread(void *param)
{
    g_ota_semaphore = MG_OS_SemaphoreCreate(1, 0);

    while(1)
    {
        if (g_ota_semaphore)
        {
            MG_OS_SemaphoreAcquireLowPower(g_ota_semaphore);

        }
        else
        {
            app_util_threadSleep(1000, g_app_cfg.bLowPowerModeEn);

        }

        if (g_app_cfg.netState == NET_ACTED && g_app_cfg.nvConfig.isSysUpdate)
        {
            APP_DEBUG("[%d] fota ing...", __LINE__);
            _fota_http_stream(g_app_cfg.netInfo.cid, (const char *)g_app_cfg.fotaFileDownloadUrl);
            g_app_cfg.isSysUpdate = false;
        }
    }

    APP_DEBUG("[%d] fota exit thread", __LINE__);
    app_util_threadSleep(1000, g_app_cfg.bLowPowerModeEn);
    MG_OS_ThreadExit();
}

void app_fota_task(void)
{
    MG_OS_ThreadCreate("appfota", app_fota_thread, NULL, Thread_PRIORITY_NORMAL, 1024 * 10, 4);
}

void app_fota_start(void)
{
    if (g_ota_semaphore)
    {
        MG_OS_SemaphoreRelease(g_ota_semaphore);
    }
}

