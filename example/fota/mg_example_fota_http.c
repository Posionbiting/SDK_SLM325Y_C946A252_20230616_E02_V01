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
#define HTTP_REQUESR_TIMOUT_MS 10*1000
#define APP_VERSION1 "V01"
#define APP_VERSION2 "V02"
/*Build version V01*/
#define APP_VERSION  APP_VERSION1
/*Build version V02*/
//#define APP_VERSION  APP_VERSION2

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

/*
 * 1. When the pack is relatively small, it can be read at one time through HTTP;
 * 2. When the pack is relatively large and the server supports segmented download,
 *    you can read it in segments and write the pack data in segments through the
 *    function MG_FOTA_WriteData.
 *
 * The following example is a full package download
*/
static s32 _getFotaPackDatabyHttp(ST_HttpRequestCtx *request, ST_HttpResponseInfo *response, const char *url)
{
    if (request == NULL || response == NULL || url == NULL)
        return -1;
    /* use default header
        * you may set option HTTP_OPT_USR_REQUEST_HEADER to config usr's header.
    */
    // request get
    s32 iret = MG_HTTP_Get(request, url, response, HTTP_REQUESR_TIMOUT_MS);
    if (iret != MG_RET_OK)
    {
        APP_DEBUG("http get request fail %d", iret);
        return -1;
    }

    s32 nStatusCode = MG_HTTP_GetResponseStatusCode(response);
    APP_DEBUG("http get request code %d", nStatusCode);
    if (nStatusCode == 200)
        return 0;
    else
        return -1;
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

    /*download pack data*/
    //create http context
    s32 iResult = 0;
    ST_HttpInfo httpinfo = {0};
    httpinfo.cid = nCurcid;
    ST_HttpRequestCtx *request = MG_HTTP_Create(&httpinfo);

    // new http response context
    ST_HttpResponseInfo *response = MG_HTTP_ResponseNew();
    char *pBuffer = NULL;
    s32 len = 0;
    s32 nContentLen = 0;
    BOOL bIsOk = FALSE;
    iResult = _getFotaPackDatabyHttp(request, response, FOTA_HTTP_URL);
    if (iResult < 0)
    {
        APP_DEBUG(" http get fota pack fail line[%d]", __LINE__);
    }
    else
    {
        nContentLen = MG_HTTP_GetResponseInfoSize(response, HTTP_RESPONSE_CONTENT);
        APP_DEBUG(" http get content length line[%d] %d", __LINE__, nContentLen);
        if (nContentLen > 0)
        {
            pBuffer = MG_MEM_Calloc(1, nContentLen+1);
            len = MG_HTTP_GetResponseInfoValue(response, HTTP_RESPONSE_CONTENT, (u8 *)pBuffer, nContentLen+1);
            if (nContentLen == len)
            {
                bIsOk = TRUE;
                APP_DEBUG(" http get fota pack success line[%d]", __LINE__);
            }
            else
            {
                APP_DEBUG(" http get fota pack fail line[%d] len %d", __LINE__, len);
            }
        }
    }

    /*flush pack data*/
    char *pPack = pBuffer;
    s32 nPackLen = nContentLen;
    s32 nWrSize = 0;
    s32 nPos = 0;
    BOOL bFlushPackIsOk = TRUE;
    if (bIsOk)
    {
        /*fota init*/
        iResult = MG_FOTA_Init();
        if (iResult == MG_RET_OK)
        {
            /*flush pack start*/
            iResult = MG_FOTA_Start();
            APP_DEBUG(" fota start fail line[%d] iResult : %d", __LINE__, iResult);
            /*flush pack data*/
            while (nPackLen > 0)
            {
                nWrSize = MG_FOTA_WriteData((u8 *)pPack+nPos, nPackLen);
                APP_DEBUG(" fota write line[%d] len %d", __LINE__, nWrSize);
                if (nWrSize < 0)
                {
                    bFlushPackIsOk = FALSE;
                    break;
                }
                nPackLen = nPackLen - nWrSize;
                nPos = nPos + nWrSize;
            }
            /*flush pack finish*/
            iResult = MG_FOTA_Finish();
            APP_DEBUG(" fota finish line[%d] iResult : %d", __LINE__, iResult);

            /*updata*/
            if (bFlushPackIsOk)
            {
                iResult = MG_FOTA_Update();
                if (iResult != MG_RET_OK)
                {
                    bFlushPackIsOk = false;
                    APP_DEBUG(" fota update fail line[%d] iret : %d", __LINE__, iResult);
                }
            }

            if (!bFlushPackIsOk)
            {
                /** clear the previously refreshed pack data*/
                MG_FOTA_Init();
                APP_DEBUG(" fota update fail line[%d]", __LINE__);
            }
            else
            {
                APP_DEBUG(" fota update success line[%d]", __LINE__);
                /** reboot*/
                MG_SYSTEM_Reboot();
            }
        }
        else
        {
            APP_DEBUG(" fota init fail line[%d]", __LINE__);
        }
    }


    if (pPack)
        MG_MEM_Free(pPack);

    // close request
    MG_HTTP_Close(request);
    // delete response context
    MG_HTTP_ResponseDelete(response);
    // destoty request context
    MG_HTTP_Destroy(request);

    APP_DEBUG("mg example exit line[%d]", __LINE__);
    MG_OS_ThreadSleep(1000);
    MG_OS_ThreadExit();
}

void proc_main_task(void)
{
    APP_DEBUG("mg example enter line[%d]", __LINE__);
    MG_OS_ThreadCreate("mgfota", prvThreadEntry, NULL, Thread_PRIORITY_NORMAL, 1024 * 2, 4);
}
