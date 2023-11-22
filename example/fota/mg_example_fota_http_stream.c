#include "mg_common.h"
#include "mg_type.h"
#include "mg_error.h"
#include "mg_os.h"
#include "mg_trace.h"
#include "mg_utils.h"
#include "mg_sys.h"
#include "mg_mem.h"
#include "mg_network.h"
#include "mg_http.h"
#include "mg_fota.h"

#define LOG_TAG MAKE_LOG_TAG('E', 'X', 'F', 'O')
#define DEBUG_ENABLE 1
#if DEBUG_ENABLE
#define APP_DEBUG(FORMAT, ...)  MG_TRACE_Printf(LOG_TAG, FORMAT, ##__VA_ARGS__)
#else
#define APP_DEBUG(FORMAT, ...)
#endif

#define FOTA_HTTP_URL "http://116.247.69.94:18080/meig_test_root/fota/up.pack"
#define HTTP_REQUESR_TIMOUT_MS 30*1000
#define APP_VERSION1 "V01"
#define APP_VERSION2 "V02"
/*Build version V01*/
#define APP_VERSION  APP_VERSION1
/*Build version V02*/
//#define APP_VERSION  APP_VERSION2


#define APP_FOTA_STREAM_HTTP_STATE_IDLE   0
#define APP_FOTA_STREAM_HTTP_STATE_START  1
#define APP_FOTA_STREAM_HTTP_STATE_WRITE  2
#define APP_FOTA_STREAM_HTTP_STATE_END    3

static s32 g_fotaStreamHttpState = APP_FOTA_STREAM_HTTP_STATE_IDLE;

// auto activate (cid = 1)
static BOOL _doAct(void)
{
    s32 iResult = 0;

    /*get csq*/
    u8 nSignalLevel = 99;
    iResult = MG_NW_GetSignalQuality(SIM0, &nSignalLevel);
    APP_DEBUG("get csq[%d] iResult:%d, nSignalLevel:%d", __LINE__, iResult, nSignalLevel);
    if (iResult != MG_RET_OK)
    {
        return FALSE;
    }
    MG_OS_ThreadSleep(100);

    /*get network state*/
    Enum_NwState nNwState = NW_STATE_NO_REGISTERED;
    iResult = MG_NW_GetNetWorkState(SIM0, PDP_CID_1, &nNwState);
    APP_DEBUG("get network state[%d] iResult:%d, nNwState:%d", __LINE__, iResult, nNwState);
    if (nNwState == NW_STATE_NO_REGISTERED)
    {
        return FALSE;
    }
    else if (nNwState == NW_STATE_LTE_ACTIVED)
    {
        return TRUE;
    }

    return FALSE;
}

// Activate manually
static BOOL _doActEx(u8 cid)
{
    s32 iResult = 0;

    if ( cid == 1 )
    {
        APP_DEBUG("cid 1 is automatically activated[%d]", __LINE__);
        return FALSE;
    }

    /*get csq*/
    u8 nSignalLevel = 99;
    iResult = MG_NW_GetSignalQuality(SIM0, &nSignalLevel);
    APP_DEBUG("get csq[%d] iResult:%d, nSignalLevel:%d", __LINE__, iResult, nSignalLevel);
    if (iResult != MG_RET_OK)
    {
        return FALSE;
    }
    MG_OS_ThreadSleep(100);

    /*get network state*/
    Enum_NwState nNwState = NW_STATE_NO_REGISTERED;
    iResult = MG_NW_GetNetWorkState(SIM0, cid, &nNwState);
    APP_DEBUG("get network state[%d] iResult:%d, nNwState:%d", __LINE__, iResult, nNwState);
    if (nNwState == NW_STATE_NO_REGISTERED)
    {
        return FALSE;
    }
    else if (nNwState == NW_STATE_LTE_ACTIVED)
    {
        return TRUE;
    }

    /*set default network cofig*/
    iResult = MG_NW_Config(SIM0, cid, NULL);
    APP_DEBUG("cid %d set default network cofig[%d] iResult:%d", cid, __LINE__, iResult);
    if (iResult != MG_RET_OK)
    {
        return FALSE;
    }
    MG_OS_ThreadSleep(100);

    /*do act, timeout 10s*/
    iResult = MG_NW_Activate(SIM0, cid, 10*1000);
    APP_DEBUG("cid %d do act[%d] iResult:%d", cid, __LINE__, iResult);
    if (iResult != MG_RET_OK)
    {
        return FALSE;
    }
    MG_OS_ThreadSleep(100);

    return TRUE;
}

static s32 _fota_http_stream_state_get()
{
    return g_fotaStreamHttpState;
}

static void _fota_http_stream_state_set(s32 state)
{
    u32 nCritical = MG_OS_EnterCritical();
    g_fotaStreamHttpState = state;
    MG_OS_ExitCritical(nCritical);
}

s32 fota_http_stream_input_data(s32 err, s32 statusCode, u8 *data, u32 data_len, void *param, s32 isEnd)
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
        APP_DEBUG("fota input data fail err=%d line[%d]", err, __LINE__);
        if (state == APP_FOTA_STREAM_HTTP_STATE_WRITE)
        {
            MG_FOTA_Finish();
        }
        _fota_http_stream_state_set(APP_FOTA_STREAM_HTTP_STATE_END);
        return err;
    }

    if (statusCode != 200)
    {
        APP_DEBUG("fota input data fail statusCode=%d line[%d]", statusCode, __LINE__);
        return MG_RET_ERR;
    }

    switch (state)
    {
        case APP_FOTA_STREAM_HTTP_STATE_START:
        {
            iret = MG_FOTA_Init();
            if (iret != MG_RET_OK)
            {
                APP_DEBUG(" fota init fail line[%d]", __LINE__);
                break;
            }

            iret = MG_FOTA_Start();
            if (iret != MG_RET_OK)
            {
                APP_DEBUG(" fota start fail line[%d]", __LINE__);
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
                APP_DEBUG(" fota write line[%d] nWrSize %d totalLen %d", __LINE__, nWrSize, totalLen);
                if (nWrSize < 0)
                {
                    iret = nWrSize;
                    break;
                }
                remainLen = remainLen - nWrSize;
                nPos = nPos + nWrSize;
            }
        }
        break;

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
        APP_DEBUG(" fota update iret %d line[%d]", iret, __LINE__);
        _fota_http_stream_state_set(APP_FOTA_STREAM_HTTP_STATE_END);
    }

    return iret;
}

static void _fota_http_stream(u8 cid, const char *url)
{
    s32 iResult = 0;
    /* create http request context */
    ST_HttpInfo httpinfo = {0};
    httpinfo.cid = cid;
    ST_HttpRequestCtx *request = MG_HTTP_Create(&httpinfo);

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
        APP_DEBUG(" fota update fail line[%d]", __LINE__);
    }
    else
    {
        APP_DEBUG(" fota update success line[%d]", __LINE__);
        MG_OS_ThreadSleep(5000);
        /** reboot*/
        MG_SYSTEM_Reboot();
    }

    // close request
    MG_HTTP_Close(request);
    // destoty request context
    MG_HTTP_Destroy(request);
}


static void prvThreadEntry(void *param)
{
    BOOL isAct = FALSE;
    u8 nCurcid = 0;
    APP_DEBUG("mgfota example entry line[%d]", __LINE__);

    //MG_TRACE_SetLogLevel(MG_TRACE_LOG_LEVEL_VERBOSE);
    MG_OS_ThreadSleep(10000);

    APP_DEBUG("APP_VERSION line[%d] %s", __LINE__, APP_VERSION);
    MG_OS_ThreadSleep(1000);
    if (MG_UTILS_Strcmp(APP_VERSION, APP_VERSION2) == 0)
    {
        while (1)
        {
            APP_DEBUG("fota updata success line[%d]", __LINE__);
            MG_OS_ThreadSleep(1000);
        }
    }

    /*do act 1 */
    isAct = FALSE;
    while (!isAct)
    {
        isAct = _doAct();
        APP_DEBUG("_doAct[%d] cid:1, isAct:%d", __LINE__, isAct);
        MG_OS_ThreadSleep(1000);
    }
    /*do act 2 */
    isAct = FALSE;
    while (!isAct)
    {
        isAct = _doActEx(PDP_CID_2);
        APP_DEBUG("_doAct[%d] cid:2, isAct:%d", __LINE__, isAct);
        MG_OS_ThreadSleep(1000);
    }

    /* select current cid */
    nCurcid = PDP_CID_1; //PDP_CID_2

    _fota_http_stream(nCurcid, FOTA_HTTP_URL);

    APP_DEBUG("mg example exit line[%d]", __LINE__);
    MG_OS_ThreadSleep(1000);
    MG_OS_ThreadExit();
}

void proc_main_task(void)
{
    APP_DEBUG("mg example enter line[%d]", __LINE__);
    MG_OS_ThreadCreate("mgfota", prvThreadEntry, NULL, Thread_PRIORITY_NORMAL, 1024 * 2, 4);
}
