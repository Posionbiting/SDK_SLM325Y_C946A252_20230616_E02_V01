#include "mg_common.h"
#include "mg_type.h"
#include "mg_error.h"
#include "mg_os.h"
#include "mg_trace.h"
#include "mg_utils.h"
#include "mg_sys.h"
#include "mg_network.h"
#include "mg_http.h"

#define LOG_TAG MAKE_LOG_TAG('E', 'X', 'H', 'P')

#define DEBUG_ENABLE 1
#if DEBUG_ENABLE
#define APP_DEBUG(FORMAT, ...)  MG_TRACE_Printf(LOG_TAG, FORMAT, ##__VA_ARGS__)
#else
#define APP_DEBUG(FORMAT, ...)
#endif

#define HTTP_REQUESR_TIMOUT_MS 30*1000
#define HTTP_REQUESR_BUFFER_SIZE 1024
char gBuffer[HTTP_REQUESR_BUFFER_SIZE] = {0};

#define HTTP_SELECT_CID_EN          0

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

static void ex_http_head_test(u8 cid)
{
    s32 iret = MG_RET_OK;
    char *url = "http://httpbin.org/get";
    (void)iret;
    (void)url;
    // create request context
#if (HTTP_SELECT_CID_EN == 0)
    //use current cid
    ST_HttpRequestCtx *request = MG_HTTP_Create(NULL);
#else
    //select cid
    ST_HttpInfo httpinfo = {0};
    httpinfo.cid = cid;
    ST_HttpRequestCtx *request = MG_HTTP_Create(&httpinfo);
#endif

    if (request == NULL)
    {
        APP_DEBUG("http head request create fail [%d]", __LINE__);
        return;
    }
    // set optVal
    s32 usr_header_en = HTTP_OPT_ENABLE;
    MG_HTTP_SetOpt(request, HTTP_OPT_USR_REQUEST_HEADER, &usr_header_en, sizeof(s32));
    // add header

    MG_HTTP_RequestHeaderClear(request);
    MG_HTTP_RequestHeaderAdd(request, "Accept", "*/*");
    MG_HTTP_RequestHeaderAdd(request, "Host", "httpbin.org");
    // new response context
    ST_HttpResponseInfo *response = MG_HTTP_ResponseNew();
    // request head
    iret = MG_HTTP_Head(request, url, response, HTTP_REQUESR_TIMOUT_MS);

    if (iret != MG_RET_OK)
    {
        APP_DEBUG("http head request fail %d", iret);
    }
    else
    {
        // get response status code
        APP_DEBUG("http head request success, status code: %d", MG_HTTP_GetResponseStatusCode(response));
        MG_OS_ThreadSleep(1000);
        // get response header size
        s32 size = MG_HTTP_GetResponseInfoSize(response, HTTP_RESPONSE_HEADER);
        APP_DEBUG("http head request header length: %d", size);
        MG_OS_ThreadSleep(1000);
        if (size > 0)
        {
            // get response content value
            MG_UTILS_Memset(gBuffer, 0 , HTTP_REQUESR_BUFFER_SIZE);
            MG_HTTP_GetResponseInfoValue(response, HTTP_RESPONSE_HEADER, (u8 *)gBuffer, HTTP_REQUESR_BUFFER_SIZE);
            APP_DEBUG("http head request header: %s", gBuffer);
            MG_OS_ThreadSleep(1000);
        }
    }
    // close request
    MG_HTTP_Close(request);
    // delete response context
    MG_HTTP_ResponseDelete(response);
    // destoty request context
    MG_HTTP_Destroy(request);

    return;
}

static void ex_http_get_test(u8 cid)
{
    s32 iret = MG_RET_OK;
    char *url = "http://httpbin.org/get";
    // create request context

#if (HTTP_SELECT_CID_EN == 0)
    //use current cid
    ST_HttpRequestCtx *request = MG_HTTP_Create(NULL);
#else
    //select cid
    ST_HttpInfo httpinfo = {0};
    httpinfo.cid = cid;
    ST_HttpRequestCtx *request = MG_HTTP_Create(&httpinfo);
#endif

    if (request == NULL)
    {
        APP_DEBUG("http get request create fail [%d]", __LINE__);
        return;
    }
    // set optVal
    s32 usr_header_en = HTTP_OPT_ENABLE;
    MG_HTTP_SetOpt(request, HTTP_OPT_USR_REQUEST_HEADER, &usr_header_en, sizeof(s32));
    // add header
    MG_HTTP_RequestHeaderClear(request);
    MG_HTTP_RequestHeaderAdd(request, "Accept", "*/*");
    MG_HTTP_RequestHeaderAdd(request, "Host", "httpbin.org");
    // new response context
    ST_HttpResponseInfo *response = MG_HTTP_ResponseNew();
    // request get
    iret = MG_HTTP_Get(request, url, response, HTTP_REQUESR_TIMOUT_MS);
    if (iret != MG_RET_OK)
    {
        APP_DEBUG("http get request fail %d", iret);
    }
    else
    {
        // get response status code
        APP_DEBUG("http get request success, status code: %d", MG_HTTP_GetResponseStatusCode(response));
        MG_OS_ThreadSleep(1000);
        // get response content size
        s32 size = MG_HTTP_GetResponseInfoSize(response, HTTP_RESPONSE_CONTENT);
        APP_DEBUG("http get request content length: %d", size);
        MG_OS_ThreadSleep(1000);
        if (size > 0)
        {
            // get response content value
            MG_UTILS_Memset(gBuffer, 0 , HTTP_REQUESR_BUFFER_SIZE);
            MG_HTTP_GetResponseInfoValue(response, HTTP_RESPONSE_CONTENT, (u8 *)gBuffer, HTTP_REQUESR_BUFFER_SIZE);
            APP_DEBUG("http get request content: %s", gBuffer);
            MG_OS_ThreadSleep(1000);
        }
    }
    // close request
    MG_HTTP_Close(request);
    // delete response context
    MG_HTTP_ResponseDelete(response);
    // destoty request context
    MG_HTTP_Destroy(request);

    return;
}

static void ex_http_post_test(u8 cid)
{
    s32 iret = MG_RET_OK;
    char *url = "http://httpbin.org/post";
    char *data = "\"key1\": \"value1\", \"key2\": \"value2\", \"key3\": \"value3\"";
    s32 dataLen = MG_UTILS_Strlen(data);
    char dataLenString[16] = {0};
    MG_UTILS_Snprintf((u8 *)dataLenString, 16, "%d", dataLen);
    // create request context
#if (HTTP_SELECT_CID_EN == 0)
    //use current cid
    ST_HttpRequestCtx *request = MG_HTTP_Create(NULL);
#else
    //select cid
    ST_HttpInfo httpinfo = {0};
    httpinfo.cid = cid;
    ST_HttpRequestCtx *request = MG_HTTP_Create(&httpinfo);
#endif

    if (request == NULL)
    {
        APP_DEBUG("http post request create fail [%d]", __LINE__);
        return;
    }
    // set optVal
    s32 usr_header_en = HTTP_OPT_ENABLE;
    MG_HTTP_SetOpt(request, HTTP_OPT_USR_REQUEST_HEADER, &usr_header_en, sizeof(s32));
    // add header
    MG_HTTP_RequestHeaderClear(request);
    MG_HTTP_RequestHeaderAdd(request, "Accept", "*/*");
    MG_HTTP_RequestHeaderAdd(request, "Host", "httpbin.org");
    MG_HTTP_RequestHeaderAdd(request, "Content-length", dataLenString);
    // new response context
    ST_HttpResponseInfo *response = MG_HTTP_ResponseNew();
    // request post
    iret = MG_HTTP_Post(request, url, (u8 *)data, dataLen, response, HTTP_REQUESR_TIMOUT_MS);
    if (iret != MG_RET_OK)
    {
        APP_DEBUG("http post request fail %d", iret);
    }
    else
    {
        // get response status code
        APP_DEBUG("http post request success, status code: %d", MG_HTTP_GetResponseStatusCode(response));
        MG_OS_ThreadSleep(1000);
        // get response content size
        s32 size = MG_HTTP_GetResponseInfoSize(response, HTTP_RESPONSE_CONTENT);
        APP_DEBUG("http post request content length: %d", size);
        MG_OS_ThreadSleep(1000);
        if (size > 0)
        {
            // get response content value
            MG_UTILS_Memset(gBuffer, 0, HTTP_REQUESR_BUFFER_SIZE);
            MG_HTTP_GetResponseInfoValue(response, HTTP_RESPONSE_CONTENT, (u8 *)gBuffer, HTTP_REQUESR_BUFFER_SIZE);
            APP_DEBUG("http post request content: %s", gBuffer);
            MG_OS_ThreadSleep(1000);
        }
    }
    // close request
    MG_HTTP_Close(request);
    // delete response context
    MG_HTTP_ResponseDelete(response);
    // destoty request context
    MG_HTTP_Destroy(request);

    return;
}

static void prvThreadEntry(void *param)
{
    BOOL isAct = FALSE;
    u8 nCurcid = 0;
    APP_DEBUG("mghttp example entry line[%d]", __LINE__);

    //MG_TRACE_SetLogLevel(MG_TRACE_LOG_LEVEL_VERBOSE);
    MG_OS_ThreadSleep(10000);

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

    /*select current cid*/
#if (HTTP_SELECT_CID_EN == 0)
    nCurcid = PDP_CID_1;
#else
    nCurcid = PDP_CID_2;
#endif

    /*test*/
    APP_DEBUG("http test start line[%d] %d", __LINE__, nCurcid);
    ex_http_head_test(nCurcid);
    ex_http_post_test(nCurcid);
    ex_http_get_test(nCurcid);
    APP_DEBUG("http test end line[%d]", __LINE__);
    MG_OS_ThreadSleep(1000);

    APP_DEBUG("mg example exit line[%d]", __LINE__);
    MG_OS_ThreadSleep(1000);
    MG_OS_ThreadExit();
}

void proc_main_task(void)
{
    APP_DEBUG("mg example enter line[%d]", __LINE__);
    MG_OS_ThreadCreate("mghttp", prvThreadEntry, NULL, Thread_PRIORITY_NORMAL, 1024 * 2, 4);
}
