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
#include "mg_opt_socket.h"
#include "mg_socket.h"
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


static s32 _set_config(ST_SslSock *ssl)
{
    if(ssl == NULL)
    {
        return MG_RET_ERR_SSL_EINVAL;
    }
    s32 fd = -1;
    fd = MG_SOCK_SSL_GetSocketId(ssl);
    APP_DEBUG("ssl Get SocketId line[%d]:%d", __LINE__, fd);
    if(fd < 0)
    {
        return fd;
    }

    u32 NBIO = 1;
    MG_SOCK_Ioctl(fd, FIONBIO, &NBIO);

    return 0;
}

static s32 _status_is_in_progress(s32 ret)
{
    return( ret == MG_RET_ERR_SSL_WANTREAD ||
        ret == MG_RET_ERR_SSL_WANTWRITE ||
        ret == MG_RET_ERR_SSL_CRYPTOINPROGRESS ||
        ret == MG_RET_ERR_SSL_TIMEOUT ||
        ret == MG_RET_ERR_SSL_ASYNCINPROGRESS );
}

static s32 _sslSend(ST_SslSock *ssl, const u8 *pdata, u32 DataLen, u32 *sendLen, u32 timeout)
{
    if((ssl == NULL) || (pdata == NULL) || (DataLen == 0) || (sendLen == NULL))
    {
        return MG_RET_ERR_SSL_EINVAL;
    }

    s32 iResult = 0;
    u64 nCurTime_ms = MG_SYSTEM_GetChipTickTimeToMs();
    u64 nCurTime1_ms = 0;
    u32 nSendLen = 0;
    while(1)
    {
        iResult = MG_SOCK_SSL_Write(ssl, (pdata+nSendLen), (DataLen-nSendLen));
        APP_DEBUG("ssl send iResult line[%d]:%d", __LINE__,iResult);
        if(iResult >= 0)
        {
            nSendLen += iResult;
            if(nSendLen >= DataLen)
            {
                break;
            }
        }
        else
        {
            if(!(_status_is_in_progress(iResult)))
            {
                *sendLen = nSendLen;
                return iResult;
            }
        }

        nCurTime1_ms = MG_SYSTEM_GetChipTickTimeToMs();
        if(nCurTime1_ms > nCurTime_ms + timeout)
        {
            APP_DEBUG("ssl send timeout line[%d]", __LINE__);
            *sendLen = nSendLen;
            return MG_RET_ERR_SSL_TIMEOUT;
        }
    }

    *sendLen = nSendLen;
    return 0;
}

static s32 _sslrecv(ST_SslSock *ssl, u8 *buffer, u32 bufferLen, u32 *RecvLen, u32 timeout)
{
    if((ssl == NULL) || (buffer == NULL) || (bufferLen == 0) || (RecvLen == NULL))
    {
        return MG_RET_ERR_SSL_EINVAL;
    }

    s32 fd = MG_SOCK_SSL_GetSocketId(ssl);
    if(fd < 0)
    {
        return -1;
    }

    s32 iResult = 0;
    u64 nCurTime_ms = MG_SYSTEM_GetChipTickTimeToMs();
    u64 nCurTime1_ms = 0;
    s32 nRecvLen = 0;
    s32 select_val = 0;
    s32 maxfd = fd + 1;
    fd_set read_set, error_set;
    ST_TimeVal stSelectTimeout;
    s32 nSelectTime_ms = 200;
    stSelectTimeout.tv_sec  = 0;
    stSelectTimeout.tv_usec = nSelectTime_ms*1000;

    while(1)
    {
        FD_ZERO(&read_set);
        FD_ZERO(&error_set);
        FD_SET(fd, &read_set);
        FD_SET(fd, &error_set);
        select_val = MG_SOCK_Select(maxfd, &read_set, NULL, &error_set, &stSelectTimeout);
        APP_DEBUG("ssl line[%d] maxfd %d,select_val:%d,read_set:%x,error_set:%x", __LINE__, maxfd, select_val, read_set, error_set);
        if(select_val < 0)
        {
            *RecvLen = nRecvLen;
            return select_val;
        }
        else
        {
            if(FD_ISSET(fd, &error_set))
            {
                s32 error = 0;
                s32 len = 4;
                iResult = MG_SOCK_GetOpt(fd, SOL_SOCKET, SO_ERROR, &error, &len);
                APP_DEBUG("ssl error_set line[%d] iResult:%d,error:%d", __LINE__, iResult, error);
                if(iResult == 0)
                {
                    iResult = error;
                }
                *RecvLen = nRecvLen;
                return iResult;
            }
            if(FD_ISSET(fd, &read_set))
            {
                iResult = MG_SOCK_SSL_Read(ssl, (buffer+nRecvLen), (bufferLen-nRecvLen));
                if(iResult < 0)
                {
                    if(!(_status_is_in_progress(iResult)))
                    {
                        *RecvLen = nRecvLen;
                        return iResult;
                    }
                }
                else if(iResult > 0)
                {
                    nRecvLen += iResult;
#if 0
                    if(nRecvLen == bufferLen)
#endif
                    {
                        break;
                    }
                }
            }
        }


        nCurTime1_ms = MG_SYSTEM_GetChipTickTimeToMs();
        if(nCurTime1_ms > nCurTime_ms + timeout)
        {
            APP_DEBUG("ssl recv timeout line[%d]", __LINE__);
            *RecvLen = nRecvLen;
            return MG_RET_ERR_SSL_TIMEOUT;
        }
    }

    *RecvLen = nRecvLen;
    return 0;
}

static void prvThreadEntry(void *param)
{
    s32 iResult = 0;
    BOOL isAct = FALSE;
    u8 nCurcid = 0;

    MG_OS_ThreadSleep(10000);
    APP_DEBUG("mgssl example entry line[%d]", __LINE__);

    ST_SslInfo Info = {0};
    ST_SslSock *ssl = NULL;
    Info.vsn = VERSION_TLS1_2;
    Info.verify = VERIFY_OPTIONAL;//VERIFY_REQUIRED
    Info.cacert = ca_cert;

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

    do
    {
        /*create ssl context*/
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
        MG_OS_ThreadSleep(1000);
        if (!isConnect)
        {
            u8 cidValue = 0;
            iResult =MG_SOCK_SSL_GetOpt(ssl, SOCK_SSL_CID, &cidValue, sizeof(cidValue));
            APP_DEBUG("ssl GetCid line[%d] iResult:%d getcid:%d", __LINE__, iResult,cidValue);

            /*ssl connect*/
            iResult = MG_SOCK_SSL_Connect(ssl, SERVICE_IP, SERVICE_PORT, CONNECT_TIMEOUT_MS);
            APP_DEBUG("ssl Connect line[%d] iResult:%d", __LINE__, iResult);
            if(iResult != MG_RET_OK)
            {
                continue;
            }
            isConnect = TRUE;
            iResult = _set_config(ssl);
            if(iResult < 0)
            {
                isErr = TRUE;
            }
        }
        /*ssl send*/
        u32 SendLen = 0;
        if(isConnect && !isErr)
        {
            iResult = _sslSend(ssl, (const u8 *)SEND_MSG, (u32)MG_UTILS_Strlen(SEND_MSG), &SendLen, SEND_TIMEOUT_MS);
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
            iResult = _sslrecv(ssl, RecvBuff, (RECV_BUFF_SIZE - RECV_BUFF_SIZE_OFSSET), &RecvLen, RECV_TIMEOUT_MS);
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
    MG_OS_ThreadCreate("mgtcp", prvThreadEntry, NULL, Thread_PRIORITY_NORMAL, 1024 * 2, 4);
}

