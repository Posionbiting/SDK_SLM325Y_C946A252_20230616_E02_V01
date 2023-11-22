#include "mg_common.h"
#include "mg_type.h"
#include "mg_error.h"
#include "mg_os.h"
#include "mg_trace.h"
#include "mg_utils.h"
#include "mg_sys.h"
#include "mg_network.h"
#include "mg_opt_socket.h"
#include "mg_socket.h"

#define LOG_TAG MAKE_LOG_TAG('E', 'X', 'T', 'C')

#define DEBUG_ENABLE 1
#if DEBUG_ENABLE
#define APP_DEBUG(FORMAT, ...)  MG_TRACE_Printf(LOG_TAG, FORMAT, ##__VA_ARGS__)
#else
#define APP_DEBUG(FORMAT, ...)
#endif

#define SERVICE_IP "116.247.69.94"
#define SERVICE_PORT 4222
#define SEND_MSG "MeiG Smart Technology"
#define RECV_BUFF_SIZE_OFSSET 1
#define RECV_BUFF_SIZE 64+RECV_BUFF_SIZE_OFSSET

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

static s32 _status_is_in_progress(s32 ret)
{
    return ((ret == MG_RET_ERR_SOCK_EINPROGRESS) ||
        (ret == MG_RET_ERR_SOCK_EWOULDBLOCK)) ? TRUE : FALSE;
}

static void prvThreadEntry(void *param)
{
    s32 iResult = 0;
    BOOL isAct = FALSE;
    u8 nCurcid = 0;
    APP_DEBUG("mgtcp example entry line[%d]", __LINE__);

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
    nCurcid = PDP_CID_1; //PDP_CID_2

    /*create socket*/
    s32 fd = -1;
    do
    {
        fd = MG_SOCK_Create(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        APP_DEBUG("TCP Create line[%d] fd:%d", __LINE__, fd);
        MG_OS_ThreadSleep(1000);
    }while(fd < 0);
    /*IP address conversion*/
    ST_ipAddr ServiceIPAddr = {0};
    do
    {
        iResult = MG_IP_ConvertIpAddr((const char *)SERVICE_IP, &ServiceIPAddr);
        APP_DEBUG("IP Convert line[%d] iResult:%d", __LINE__, iResult);
        MG_OS_ThreadSleep(1000);
    }while(iResult != MG_RET_OK);
    if(ServiceIPAddr.type == IP_ADDR_TYPE_V4)
    {
        u8 to4[4]= {0};
        to4[0] = (u8)((ServiceIPAddr.u_addr.ip4.addr >> 24) & 0xff);
        to4[1] = (u8)((ServiceIPAddr.u_addr.ip4.addr >> 16) & 0xff);
        to4[2] = (u8)((ServiceIPAddr.u_addr.ip4.addr >> 8) & 0xff);
        to4[3] = (u8)((ServiceIPAddr.u_addr.ip4.addr >> 0) & 0xff);
        APP_DEBUG("IP line[%d]:%d.%d.%d.%d", __LINE__, to4[0], to4[1], to4[2], to4[3]);
    }
    /*keeplive*/
    s32 keepalive = 1;
    s32 keepidle = 7200;
    s32 keepinterval = 75;
    s32 keepcount = 9;
    if(keepalive == 1)
    {
        MG_SOCK_SetOpt(fd, SOL_SOCKET, SO_KEEPALIVE, (void *)&keepalive, sizeof(keepalive));
        MG_SOCK_SetOpt(fd, IPPROTO_TCP, TCP_KEEPIDLE, (void *)&keepidle, sizeof(keepidle));
        MG_SOCK_SetOpt(fd, IPPROTO_TCP, TCP_KEEPINTVL, (void *)&keepinterval, sizeof(keepinterval));
        MG_SOCK_SetOpt(fd, IPPROTO_TCP, TCP_KEEPCNT, (void *)&keepcount, sizeof(keepcount));
    }
    else
    {
        MG_SOCK_SetOpt(fd, SOL_SOCKET, SO_KEEPALIVE, (void *)&keepalive, sizeof(keepalive));
    }

    /*bind cid*/
    iResult = MG_SOCK_BindCid(fd, &nCurcid);
    APP_DEBUG("fd %d bind cid %d line[%d] iResult %d", fd, nCurcid, __LINE__, iResult);

    /*Connect*/
    do
    {
        MG_OS_ThreadSleep(1000);
        iResult = MG_SOCK_Connect(fd, &ServiceIPAddr, (u16)SERVICE_PORT);
        APP_DEBUG("TCP Connect line[%d] iResult:%d", __LINE__, iResult);
    }while(iResult != MG_RET_OK);

    /*noblock*/
    u32 NBIO = 1;
    MG_SOCK_Ioctl(fd, FIONBIO, &NBIO);
    /*send*/
    s32 nFlag = 0;
    do
    {
        iResult = MG_SOCK_Send(fd, (const u8 *)SEND_MSG, (u32)MG_UTILS_Strlen(SEND_MSG));
        APP_DEBUG("TCP SEND line[%d] iResult:%d", __LINE__, iResult);
        nFlag = iResult;
        if(iResult < 0)
        {
            if(_status_is_in_progress(iResult))
            {
                nFlag = MG_RET_OK;
            }
        }
    }while(nFlag == MG_RET_OK);

    /*recv*/
    s32 nRecvLen = 0;
    s32 maxfd = fd + 1;
    fd_set readfd;
    ST_TimeVal stSelectTimeout;
    stSelectTimeout.tv_sec  = 0;
    stSelectTimeout.tv_usec = 200*1000;
    while(1)
    {
        FD_ZERO(&readfd);
        FD_SET(fd, &readfd);
        MG_UTILS_Memset(RecvBuff, 0, RECV_BUFF_SIZE);

        iResult = MG_SOCK_Select(maxfd, &readfd, NULL, NULL, &stSelectTimeout);
        APP_DEBUG("tcp line[%d] maxfd %d,select_val:%d,read_set:%x", __LINE__, maxfd, iResult, readfd);
        if(iResult < 0)
        {
            APP_DEBUG("TCP Select fail line[%d] iResult:%d", __LINE__, iResult);
            break;
        }
        else
        {
            if(FD_ISSET(fd, &readfd))
            {
                nRecvLen = MG_SOCK_Recv(fd, RecvBuff, RECV_BUFF_SIZE - RECV_BUFF_SIZE_OFSSET);
                if(nRecvLen < 0)
                {
                    APP_DEBUG("TCP recv fail line[%d] nRecvLen:%d", __LINE__, nRecvLen);
                    break;
                }
                else if(nRecvLen == 0)
                {
                    APP_DEBUG("TCP recv msg len line[%d] nRecvLen:%d", __LINE__, nRecvLen);
                    continue;
                }
                else
                {
                    APP_DEBUG("TCP Recv msg line[%d] :%s", __LINE__, RecvBuff);
                    break;
                }
            }
        }
    }

    /*close*/
    MG_SOCK_Close(fd);

    APP_DEBUG("mg example exit line[%d]", __LINE__);
    MG_OS_ThreadSleep(1000);
    MG_OS_ThreadExit();
}

void proc_main_task(void)
{
    APP_DEBUG("mg example enter line[%d]", __LINE__);
    MG_OS_ThreadCreate("mgtcp", prvThreadEntry, NULL, Thread_PRIORITY_NORMAL, 1024 * 2, 4);
}
