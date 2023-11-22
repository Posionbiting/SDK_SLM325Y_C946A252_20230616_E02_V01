#include "mg_common.h"
#include "mg_type.h"
#include "mg_error.h"
#include "mg_os.h"
#include "mg_trace.h"
#include "mg_utils.h"
#include "mg_sys.h"
#include "mg_network.h"
#include "mg_http.h"

#define LOG_TAG MAKE_LOG_TAG('E', 'X', 'P', 'S')

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

static char *ca_cert_httpbin={  \
"-----BEGIN CERTIFICATE-----\r\n"
"MIIEDzCCAvegAwIBAgIBADANBgkqhkiG9w0BAQUFADBoMQswCQYDVQQGEwJVUzEl\r\n"
"MCMGA1UEChMcU3RhcmZpZWxkIFRlY2hub2xvZ2llcywgSW5jLjEyMDAGA1UECxMp\r\n"
"U3RhcmZpZWxkIENsYXNzIDIgQ2VydGlmaWNhdGlvbiBBdXRob3JpdHkwHhcNMDQw\r\n"
"NjI5MTczOTE2WhcNMzQwNjI5MTczOTE2WjBoMQswCQYDVQQGEwJVUzElMCMGA1UE\r\n"
"ChMcU3RhcmZpZWxkIFRlY2hub2xvZ2llcywgSW5jLjEyMDAGA1UECxMpU3RhcmZp\r\n"
"ZWxkIENsYXNzIDIgQ2VydGlmaWNhdGlvbiBBdXRob3JpdHkwggEgMA0GCSqGSIb3\r\n"
"DQEBAQUAA4IBDQAwggEIAoIBAQC3Msj+6XGmBIWtDBFk385N78gDGIc/oav7PKaf\r\n"
"8MOh2tTYbitTkPskpD6E8J7oX+zlJ0T1KKY/e97gKvDIr1MvnsoFAZMej2YcOadN\r\n"
"+lq2cwQlZut3f+dZxkqZJRRU6ybH838Z1TBwj6+wRir/resp7defqgSHo9T5iaU0\r\n"
"X9tDkYI22WY8sbi5gv2cOj4QyDvvBmVmepsZGD3/cVE8MC5fvj13c7JdBmzDI1aa\r\n"
"K4UmkhynArPkPw2vCHmCuDY96pzTNbO8acr1zJ3o/WSNF4Azbl5KXZnJHoe0nRrA\r\n"
"1W4TNSNe35tfPe/W93bC6j67eA0cQmdrBNj41tpvi/JEoAGrAgEDo4HFMIHCMB0G\r\n"
"A1UdDgQWBBS/X7fRzt0fhvRbVazc1xDCDqmI5zCBkgYDVR0jBIGKMIGHgBS/X7fR\r\n"
"zt0fhvRbVazc1xDCDqmI56FspGowaDELMAkGA1UEBhMCVVMxJTAjBgNVBAoTHFN0\r\n"
"YXJmaWVsZCBUZWNobm9sb2dpZXMsIEluYy4xMjAwBgNVBAsTKVN0YXJmaWVsZCBD\r\n"
"bGFzcyAyIENlcnRpZmljYXRpb24gQXV0aG9yaXR5ggEAMAwGA1UdEwQFMAMBAf8w\r\n"
"DQYJKoZIhvcNAQEFBQADggEBAAWdP4id0ckaVaGsafPzWdqbAYcaT1epoXkJKtv3\r\n"
"L7IezMdeatiDh6GX70k1PncGQVhiv45YuApnP+yz3SFmH8lU+nLMPUxA2IGvd56D\r\n"
"eruix/U0F47ZEUD0/CwqTRV/p2JdLiXTAAsgGh1o+Re49L2L7ShZ3U0WixeDyLJl\r\n"
"xy16paq8U4Zt3VekyvggQQto8PT7dL5WXXp59fkdheMtlb71cZBDzI0fmgAKhynp\r\n"
"VSJYACPq4xJDKVtHCN2MQWplBqjlIapBtJUhlbl90TSrE9atvNziPTnNvT51cKEY\r\n"
"WQPJIrSPnNVeKtelttQKbfi3QBFGmh95DmK/D5fs4C8fF5Q=\r\n"
"-----END CERTIFICATE-----\r\n"};

static char *ca_cert_mail163 ={  \
"-----BEGIN CERTIFICATE-----\r\n"\
"MIIDrzCCApegAwIBAgIQCDvgVpBCRrGhdWrJWZHHSjANBgkqhkiG9w0BAQUFADBh\r\n"\
"MQswCQYDVQQGEwJVUzEVMBMGA1UEChMMRGlnaUNlcnQgSW5jMRkwFwYDVQQLExB3\r\n"\
"d3cuZGlnaWNlcnQuY29tMSAwHgYDVQQDExdEaWdpQ2VydCBHbG9iYWwgUm9vdCBD\r\n"\
"QTAeFw0wNjExMTAwMDAwMDBaFw0zMTExMTAwMDAwMDBaMGExCzAJBgNVBAYTAlVT\r\n"\
"MRUwEwYDVQQKEwxEaWdpQ2VydCBJbmMxGTAXBgNVBAsTEHd3dy5kaWdpY2VydC5j\r\n"\
"b20xIDAeBgNVBAMTF0RpZ2lDZXJ0IEdsb2JhbCBSb290IENBMIIBIjANBgkqhkiG\r\n"\
"9w0BAQEFAAOCAQ8AMIIBCgKCAQEA4jvhEXLeqKTTo1eqUKKPC3eQyaKl7hLOllsB\r\n"\
"CSDMAZOnTjC3U/dDxGkAV53ijSLdhwZAAIEJzs4bg7/fzTtxRuLWZscFs3YnFo97\r\n"\
"nh6Vfe63SKMI2tavegw5BmV/Sl0fvBf4q77uKNd0f3p4mVmFaG5cIzJLv07A6Fpt\r\n"\
"43C/dxC//AH2hdmoRBBYMql1GNXRor5H4idq9Joz+EkIYIvUX7Q6hL+hqkpMfT7P\r\n"\
"T19sdl6gSzeRntwi5m3OFBqOasv+zbMUZBfHWymeMr/y7vrTC0LUq7dBMtoM1O/4\r\n"\
"gdW7jVg/tRvoSSiicNoxBN33shbyTApOB6jtSj1etX+jkMOvJwIDAQABo2MwYTAO\r\n"\
"BgNVHQ8BAf8EBAMCAYYwDwYDVR0TAQH/BAUwAwEB/zAdBgNVHQ4EFgQUA95QNVbR\r\n"\
"TLtm8KPiGxvDl7I90VUwHwYDVR0jBBgwFoAUA95QNVbRTLtm8KPiGxvDl7I90VUw\r\n"\
"DQYJKoZIhvcNAQEFBQADggEBAMucN6pIExIK+t1EnE9SsPTfrgT1eXkIoyQY/Esr\r\n"\
"hMAtudXH/vTBH1jLuG2cenTnmCmrEbXjcKChzUyImZOMkXDiqw8cvpOp/2PV5Adg\r\n"\
"06O/nVsJ8dWO41P0jmP6P6fbtGbfYmbW0W5BjfIttep3Sp+dWOIrWcBAI+0tKIJF\r\n"\
"PnlUkiaY4IBIqDfv8NZ5YBberOgOzW6sRBc4L0na4UU+Krk2U886UAb3LujEV0ls\r\n"\
"YSEY1QSteDwsOoBrp+uvFRTp2InBuThs4pFsiv9kuXclVzDAGySj4dzp30d8tbQk\r\n"\
"CAUw7C29C79Fv1C5qfPrmAESrciIxpg0X40KPMbp1ZWVbd4=\r\n"\
"-----END CERTIFICATE-----\r\n"};

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

static void ex_https_get_test(u8 cid)
{
    s32 iret = MG_RET_OK;
    char *url = "https://httpbin.org/get";
    (void)iret;
    (void)url;
    // create request context
#if (HTTP_SELECT_CID_EN == 0)
    //use current cid
    ST_HttpInfo httpinfo = {0};
    httpinfo.vsn = HTTP_TLS_VERSION_TLS1_2;
    httpinfo.verify = HTTP_TLS_VERIFY_REQUIRED;
    httpinfo.cacert = ca_cert_httpbin;
    ST_HttpRequestCtx *request = MG_HTTP_Create(&httpinfo);
#else
    //select cid
    ST_HttpInfo httpinfo = {0};
    httpinfo.vsn = HTTP_TLS_VERSION_TLS1_2;
    httpinfo.verify = HTTP_TLS_VERIFY_REQUIRED;
    httpinfo.cacert = ca_cert_httpbin;
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
    iret = MG_HTTP_Get(request, url, response, HTTP_REQUESR_TIMOUT_MS);

    if (iret != MG_RET_OK)
    {
        APP_DEBUG("https get request fail %d", iret);
    }
    else
    {
        // get response status code
        APP_DEBUG("https get request success, status code: %d", MG_HTTP_GetResponseStatusCode(response));
        MG_OS_ThreadSleep(1000);
        // get response header size
        s32 size = MG_HTTP_GetResponseInfoSize(response, HTTP_RESPONSE_CONTENT);
        APP_DEBUG("https get request content length: %d", size);
        MG_OS_ThreadSleep(1000);
        if (size > 0)
        {
            // get response content value
            MG_UTILS_Memset(gBuffer, 0 , HTTP_REQUESR_BUFFER_SIZE);
            MG_HTTP_GetResponseInfoValue(response, HTTP_RESPONSE_CONTENT, (u8 *)gBuffer, HTTP_REQUESR_BUFFER_SIZE);
            APP_DEBUG("https get request content: %s", gBuffer);
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

static void ex_https_get_test_mail163(u8 cid)
{
    s32 iret = MG_RET_OK;
    char *url = "https://mail.163.com/";

    // create request context
#if (HTTP_SELECT_CID_EN == 0)
    //use current cid
    ST_HttpInfo httpinfo = {0};
    httpinfo.vsn = HTTP_TLS_VERSION_TLS1_2;
    httpinfo.verify = HTTP_TLS_VERIFY_REQUIRED;
    httpinfo.cacert = ca_cert_mail163;
    ST_HttpRequestCtx *request = MG_HTTP_Create(&httpinfo);
#else
    //select cid
    ST_HttpInfo httpinfo = {0};
    httpinfo.vsn = HTTP_TLS_VERSION_TLS1_2;
    httpinfo.verify = HTTP_TLS_VERIFY_REQUIRED;
    httpinfo.cacert = ca_cert_mail163;
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
    MG_HTTP_RequestHeaderAdd(request, "Host", "mail.163.com");
    // new response context
    ST_HttpResponseInfo *response = MG_HTTP_ResponseNew();
    // request get
    iret = MG_HTTP_Get(request, url, response, HTTP_REQUESR_TIMOUT_MS);
    if (iret != MG_RET_OK)
    {
        APP_DEBUG("https get request fail %d", iret);
    }
    else
    {
        // get response status code
        APP_DEBUG("https get request success, status code: %d", MG_HTTP_GetResponseStatusCode(response));
        MG_OS_ThreadSleep(1000);
        // get response content size
        s32 size = MG_HTTP_GetResponseInfoSize(response, HTTP_RESPONSE_CONTENT);
        APP_DEBUG("https get request content length: %d", size);
        MG_OS_ThreadSleep(1000);
        if (size > 0)
        {
            // get response content value
            MG_UTILS_Memset(gBuffer, 0 , HTTP_REQUESR_BUFFER_SIZE);
            MG_HTTP_GetResponseInfoValue(response, HTTP_RESPONSE_CONTENT, (u8 *)gBuffer, HTTP_REQUESR_BUFFER_SIZE);
            APP_DEBUG("https get request content: %s", gBuffer);
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

static void ex_https_post_test(u8 cid)
{
    s32 iret = MG_RET_OK;
    char *url = "https://httpbin.org/post";
    char *data = "\"key1\": \"value1\", \"key2\": \"value2\", \"key3\": \"value3\"";
    s32 dataLen = MG_UTILS_Strlen(data);
    char dataLenString[16] = {0};
    MG_UTILS_Snprintf((u8 *)dataLenString, 16, "%d", dataLen);

    // create request context
#if (HTTP_SELECT_CID_EN == 0)
    //use current cid
    ST_HttpInfo httpinfo = {0};
    httpinfo.vsn = HTTP_TLS_VERSION_TLS1_2;
    httpinfo.verify = HTTP_TLS_VERIFY_REQUIRED;
    httpinfo.cacert = ca_cert_httpbin;
    ST_HttpRequestCtx *request = MG_HTTP_Create(&httpinfo);
#else
    //select cid
    ST_HttpInfo httpinfo = {0};
    httpinfo.vsn = HTTP_TLS_VERSION_TLS1_2;
    httpinfo.verify = HTTP_TLS_VERIFY_REQUIRED;
    httpinfo.cacert = ca_cert_httpbin;
    httpinfo.cid = cid;
    ST_HttpRequestCtx *request = MG_HTTP_Create(&httpinfo);
#endif

    if (request == NULL)
    {
        APP_DEBUG("https post request create fail [%d]", __LINE__);
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
        APP_DEBUG("https post request fail %d", iret);
    }
    else
    {
        // get response status code
        APP_DEBUG("https post request success, status code: %d", MG_HTTP_GetResponseStatusCode(response));
        MG_OS_ThreadSleep(1000);
        // get response content size
        s32 size = MG_HTTP_GetResponseInfoSize(response, HTTP_RESPONSE_CONTENT);
        APP_DEBUG("https post request content length: %d", size);
        MG_OS_ThreadSleep(1000);
        if (size > 0)
        {
            // get response content value
            MG_UTILS_Memset(gBuffer, 0, HTTP_REQUESR_BUFFER_SIZE);
            MG_HTTP_GetResponseInfoValue(response, HTTP_RESPONSE_CONTENT, (u8 *)gBuffer, HTTP_REQUESR_BUFFER_SIZE);
            APP_DEBUG("https post request content: %s", gBuffer);
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
    APP_DEBUG("https test start line[%d] %d", __LINE__, nCurcid);
    ex_https_get_test(nCurcid);
    ex_https_get_test_mail163(nCurcid);
    ex_https_post_test(nCurcid);
    APP_DEBUG("https test end line[%d]", __LINE__);
    MG_OS_ThreadSleep(1000);

    APP_DEBUG("mg example exit line[%d]", __LINE__);
    MG_OS_ThreadSleep(1000);
    MG_OS_ThreadExit();
}

void proc_main_task(void)
{
    APP_DEBUG("mg example enter line[%d]", __LINE__);
    MG_OS_ThreadCreate("mghttps", prvThreadEntry, NULL, Thread_PRIORITY_NORMAL, 1024 * 4, 4);
}
