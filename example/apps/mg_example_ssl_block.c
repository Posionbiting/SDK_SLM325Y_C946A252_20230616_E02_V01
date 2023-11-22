/* Copyright (C) 2019 RDA Technologies Limited and/or its affiliates("RDA").
 * All rights reserved.
 *
 * This software is supplied "AS IS" without any warranties.
 * RDA assumes no responsibility or liability for the use of the software,
 * conveys no license or title under any patent, copyright, or mask work
 * right to the product. RDA reserves the right to make changes in the
 * software without notification.  RDA also make no representation or
 * warranty that such application will be suitable for the specified use
 * without further testing or modification.
 */
#include "mg_common.h"
#include "mg_type.h"
#include "mg_error.h"
#include "mg_os.h"
#include "mg_trace.h"
#include "mg_utils.h"
#include "mg_sys.h"
#include "mg_ssl.h"
#include "mg_network.h"

#define LOG_TAG MAKE_LOG_TAG('E', 'S', 'S', 'L')

#define DEBUG_ENABLE 1
#if DEBUG_ENABLE
#define APP_DEBUG(FORMAT, ...)  MG_TRACE_Printf(LOG_TAG, FORMAT, ##__VA_ARGS__)
#else
#define APP_DEBUG(FORMAT, ...)
#endif

static char *ca_cert ={ \
"-----BEGIN CERTIFICATE-----\r\n" \
"MIICOzCCAaQCCQCkrOCNp5c1dTANBgkqhkiG9w0BAQsFADBiMQswCQYDVQQGEwJD\r\n" \
"TjENMAsGA1UECAwETUdDQTENMAsGA1UEBwwETUdDQTENMAsGA1UECgwETUdDQTEN\r\n" \
"MAsGA1UECwwETUdDQTEXMBUGA1UEAwwOMTA0LjIyNC4xNzYuMzEwHhcNMjAwODI0\r\n" \
"MDc0ODI4WhcNMzAwODIyMDc0ODI4WjBiMQswCQYDVQQGEwJDTjENMAsGA1UECAwE\r\n" \
"TUdDQTENMAsGA1UEBwwETUdDQTENMAsGA1UECgwETUdDQTENMAsGA1UECwwETUdD\r\n" \
"QTEXMBUGA1UEAwwOMTA0LjIyNC4xNzYuMzEwgZ8wDQYJKoZIhvcNAQEBBQADgY0A\r\n" \
"MIGJAoGBAKlyqC7NucBkGrXzfduX0r37ZV6XqEkVAbaUKp4ifekWYtU2ccG7j2jf\r\n" \
"GeVKciZlWNOBCx5NmQdj6dZHhtKjZmGI7PRz3qbHvImIsgKIerkSS8F++OuFNARF\r\n" \
"SxpQORjCFda77R9S8i6RmnU459dtv/aRz+WOUhZx3ZQdeqhdovFFAgMBAAEwDQYJ\r\n" \
"KoZIhvcNAQELBQADgYEALmBf3oqVVt6m9lObMs9bmPn45BQuZZjXjsosK8sV7F4d\r\n" \
"x5h0SvOW17eFLWx0D4J52loE8S0CK/KYYBEH8elqa+1JwqwcgF90ANsXY1gpfCJU\r\n" \
"hr7KWw5s/oP+vsoFV6dSIlplIWp/ZCKGRdHrC2YNrw1hwqJxsT39cVmCdLOG8sE=\r\n" \
"-----END CERTIFICATE-----\r\n"};

#define SERVICE_IP "116.247.69.94"
#define SERVICE_PORT 9103
#define CONNECT_TIMEOUT_MS 30000
#define SEND_TIMEOUT_MS 3000
#define RECV_TIMEOUT_MS 3000

#define SEND_MSG "GET / HTTP/1.1\r\nConnection: keep-alive\r\nUser-Agent: 8910\r\nHOST: 116.247.69.94:8080\r\n\r\n"
#define RECV_BUFF_SIZE_OFSSET 1
#define RECV_BUFF_SIZE 1500+RECV_BUFF_SIZE_OFSSET

u8 RecvBuff[RECV_BUFF_SIZE] = {0};

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


static void prvThreadEntry(void *param)
{
    s32 iResult = 0;
    BOOL isAct = FALSE;
    u8 nCurcid = 0;

    MG_OS_ThreadSleep(10000);
    APP_DEBUG("mgssl example entry line[%d]", __LINE__);

    /*do act 1*/
    while (!isAct)
    {
        isAct = _doAct();
        APP_DEBUG("_doAct[%d] isAct:%d", __LINE__, isAct);
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
    nCurcid = PDP_CID_1; //PDP_CID_2

    /*get network state*/
    Enum_NwState nNwState = NW_STATE_NO_REGISTERED;
    iResult = MG_NW_GetNetWorkState(SIM0, nCurcid, &nNwState);
    if(nNwState == NW_STATE_LTE_ACTIVED)
    {
        APP_DEBUG("LTE ACT[%d]", __LINE__);
    }
    else
    {
        APP_DEBUG("get network state[%d] iResult:%d, nNwState:%d", __LINE__, iResult, nNwState);
    }
    MG_OS_ThreadSleep(1000);

    /*create ssl context*/
    ST_SslInfo Info = {0};
    ST_SslSock *ssl = NULL;
    Info.vsn = VERSION_TLS1_2;
    Info.verify = VERIFY_OPTIONAL;//VERIFY_REQUIRED
    Info.cacert = ca_cert;
    do
    {
        ssl = MG_SOCK_SSL_Create(&Info);
        APP_DEBUG("ssl Create line[%d] ssl :%x", __LINE__, ssl);
        MG_OS_ThreadSleep(1000);
    }while(!ssl);

    bool isErr = FALSE;
    bool isConnect = FALSE;
    s32 cnt = 0;
    iResult = MG_SOCK_SSL_SetOpt(ssl, SOCK_SSL_CID, &nCurcid, sizeof(nCurcid));
    APP_DEBUG("ssl SetCid line[%d] iResult:%d", __LINE__,  iResult);
    while(1)
    {
        if (!isConnect)
        {
            u32 cidValue = 0;
            /*ssl connect*/
            iResult =MG_SOCK_SSL_GetOpt(ssl, SOCK_SSL_CID, &cidValue, sizeof(cidValue));
            APP_DEBUG("ssl GetCid line[%d] iResult:%d getcid:%d", __LINE__, iResult,cidValue);
            iResult = MG_SOCK_SSL_Connect(ssl, SERVICE_IP, SERVICE_PORT, CONNECT_TIMEOUT_MS);
            APP_DEBUG("ssl Connect line[%d] iResult:%d", __LINE__, iResult);
            if(iResult != MG_RET_OK)
            {
                continue;
            }
            isConnect = TRUE;
        }
        /*ssl send*/
        u32 SendLen = 0;
        if(isConnect && !isErr)
        {
            iResult = MG_SOCK_SSL_Write(ssl, (const u8 *)SEND_MSG, (u32)MG_UTILS_Strlen(SEND_MSG));
            if(iResult < 0)
            {
                isErr = TRUE;
            }
            APP_DEBUG("ssl send line[%d] iResult:%d, SendLen:%d", __LINE__, iResult, SendLen);
        }
        /*ssl recv*/
        u32 RecvLen = 0;
        if(isConnect && !isErr)
        {
            MG_UTILS_Memset(RecvBuff, 0, RECV_BUFF_SIZE);
            iResult = MG_SOCK_SSL_Read(ssl, RecvBuff, (RECV_BUFF_SIZE - RECV_BUFF_SIZE_OFSSET));
            if(iResult < 0)
            {
                isErr = TRUE;
            }
            APP_DEBUG("ssl recv line[%d] iResult:%d, RecvLen:%d, RecvBuff:%s", __LINE__, iResult, RecvLen, RecvBuff);
        }
        /*Check whether to reconnect*/
        if(isErr)
        {
            APP_DEBUG("ssl disconnect line[%d]", __LINE__);
            MG_SOCK_SSL_Disconnect(ssl);
            isErr = FALSE;
            isConnect = FALSE;
            cnt = 0;
        }
        else
        {
            cnt++;
            if (cnt == 5)
                break;
        }
    }

    MG_SOCK_SSL_Destroy(ssl);

    APP_DEBUG("mg example exit line[%d]", __LINE__);
    MG_OS_ThreadSleep(1000);
    MG_OS_ThreadExit();
}

void proc_main_task(void)
{
    APP_DEBUG("mg example enter line[%d]", __LINE__);
    MG_OS_ThreadCreate("mgntp", prvThreadEntry, NULL, Thread_PRIORITY_NORMAL, 1024 * 2, 4);
}

