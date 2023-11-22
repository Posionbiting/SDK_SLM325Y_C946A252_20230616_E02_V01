#include "mg_common.h"
#include "mg_type.h"
#include "mg_error.h"
#include "mg_os.h"
#include "mg_trace.h"
#include "mg_utils.h"
#include "mg_sim.h"
#include "mg_network.h"

#define LOG_TAG MAKE_LOG_TAG('E', 'X', 'N', 'E')
#define DEBUG_ENABLE 1
#if DEBUG_ENABLE
#define APP_DEBUG(FORMAT, ...)  MG_TRACE_Printf(LOG_TAG, FORMAT, ##__VA_ARGS__)
#else
#define APP_DEBUG(FORMAT, ...)
#endif

#define IPV6_STRING_LEN    128
#define LOG_BUF_STRING_LEN 128

void _netStatus(s32 eventId, ST_NwRegInfo *info, void *param)
{
    ST_NwRegInfo *pInfo = info;
    switch(eventId)
    {
        case EV_OPEN_SIM_CARD_INSERT:
            APP_DEBUG("EV_OPEN_SIM_CARD_INSERT line[%d]", __LINE__);
            break;
        case EV_OPEN_SIM_CARD_PULL_OUT:
            APP_DEBUG("EV_OPEN_SIM_CARD_PULL_OUT line[%d]", __LINE__);
            break;
        case EV_OPEN_PS_DOMAIN_NO_REGISTER:
            APP_DEBUG("EV_OPEN_PS_DOMAIN_NO_REGISTER line[%d]", __LINE__);
            break;
        case EV_OPEN_PS_DOMAIN_REGISTER:
            APP_DEBUG("EV_OPEN_PS_DOMAIN_REGISTER line[%d]", __LINE__);
            break;
        case EV_OPEN_PDP_MANUAL_DEACTIVATION:
            APP_DEBUG("EV_OPEN_PDP_MANUAL_DEACTIVATION sim:%d cid:%d line[%d]", pInfo->nSim, pInfo->nCid, __LINE__);
            break;
        case EV_OPEN_PDP_MANUAL_ACTIVATION:
            APP_DEBUG("EV_OPEN_PDP_MANUAL_ACTIVATION sim:%d cid:%d line[%d]", pInfo->nSim, pInfo->nCid, __LINE__);
            break;
        case EV_OPEN_PDP_AUTO_DEACTIVATION:
            APP_DEBUG("EV_OPEN_PDP_AUTO_DEACTIVATION sim:%d cid:%d line[%d]", pInfo->nSim, pInfo->nCid, __LINE__);
            break;
        case EV_OPEN_PDP_AUTO_ACTIVATION:
            APP_DEBUG("EV_OPEN_PDP_AUTO_ACTIVATION sim:%d cid:%d line[%d]", pInfo->nSim, pInfo->nCid, __LINE__);
            break;

        default:
            break;
    }
    MG_OS_ThreadSleep(100);
}

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
    APP_DEBUG("mgnetwork example entry line[%d]", __LINE__);

    /*Register*/
    MG_NW_Register(_netStatus, NULL);

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

    s32 i = 0;
    u8 to4[4]= {0};
    u8 to6[16]= {0};
    u8 to6Str[IPV6_STRING_LEN] = {0};
    /*get local V4 addr*/
    ST_ipAddr localAddr = {0};
    iResult = MG_NW_GetLocalIPAddress(SIM0, nCurcid, &localAddr, IP_ADDR_TYPE_V4);
    if (iResult != MG_RET_OK)
    {
        APP_DEBUG("get local v4 addr[%d] iResult:%d", __LINE__, iResult);
    }
    else
    {
        if(localAddr.type == IP_ADDR_TYPE_V4)
        {
            to4[0] = (u8)((localAddr.u_addr.ip4.addr >> 24) & 0xff);
            to4[1] = (u8)((localAddr.u_addr.ip4.addr >> 16) & 0xff);
            to4[2] = (u8)((localAddr.u_addr.ip4.addr >> 8) & 0xff);
            to4[3] = (u8)((localAddr.u_addr.ip4.addr >> 0) & 0xff);
            APP_DEBUG("local addr v4 line[%d]:%d.%d.%d.%d", __LINE__, to4[3], to4[2], to4[1], to4[0]);
        }
    }
    MG_OS_ThreadSleep(1000);

    /*get local V6 addr*/
    MG_UTILS_Memset(&localAddr, 0, sizeof(ST_ipAddr));
    iResult = MG_NW_GetLocalIPAddress(SIM0, nCurcid, &localAddr, IP_ADDR_TYPE_V6);
    if (iResult != MG_RET_OK)
    {
        APP_DEBUG("get local v6 addr[%d] iResult:%d", __LINE__, iResult);
    }
    else
    {
        if(localAddr.type == IP_ADDR_TYPE_V6)
        {
            for ( i = 0; i < 4; i++)
            {
                to6[(i*4) + 0] = (u8)((localAddr.u_addr.ip6.addr[i] >> 24) & 0xff);
                to6[(i*4) + 1] = (u8)((localAddr.u_addr.ip6.addr[i] >> 16) & 0xff);
                to6[(i*4) + 2] = (u8)((localAddr.u_addr.ip6.addr[i] >> 8) & 0xff);
                to6[(i*4) + 3] = (u8)((localAddr.u_addr.ip6.addr[i] >> 0) & 0xff);
            }

            MG_UTILS_Memset(to6Str, 0, IPV6_STRING_LEN);
            MG_UTILS_Snprintf(to6Str, IPV6_STRING_LEN, "%d.%d.%d.%d.%d.%d.%d.%d.%d.%d.%d.%d.%d.%d.%d.%d",
                                           to6[3], to6[2], to6[1], to6[0],
                                           to6[7], to6[6], to6[5], to6[4],
                                           to6[11], to6[10], to6[9], to6[8],
                                           to6[15], to6[14], to6[13], to6[12]);

            APP_DEBUG("local addr v6 line[%d]:%s", __LINE__, to6Str);
        }
    }
    MG_OS_ThreadSleep(1000);

    /*get DNS1 V4 addr*/
    ST_ipAddr dnsAddr = {0};
    iResult = MG_NW_GetDnsIpAddress(SIM0, nCurcid, &dnsAddr, IP_ADDR_TYPE_V4, NETWORK_PDP_TYPE_PRIMARY_DNS_IP);
    if (iResult != MG_RET_OK)
    {
        APP_DEBUG("get dns1 v4 addr[%d] iResult:%d", __LINE__, iResult);
    }
    else
    {
        if(dnsAddr.type == IP_ADDR_TYPE_V4)
        {
            to4[0] = (u8)((dnsAddr.u_addr.ip4.addr >> 24) & 0xff);
            to4[1] = (u8)((dnsAddr.u_addr.ip4.addr >> 16) & 0xff);
            to4[2] = (u8)((dnsAddr.u_addr.ip4.addr >> 8) & 0xff);
            to4[3] = (u8)((dnsAddr.u_addr.ip4.addr >> 0) & 0xff);
            APP_DEBUG("dns1 v4 addr line[%d]:%d.%d.%d.%d", __LINE__, to4[3], to4[2], to4[1], to4[0]);
        }
    }
    MG_OS_ThreadSleep(1000);

    /*get DNS1 v6 addr*/
    MG_UTILS_Memset(&dnsAddr, 0, sizeof(ST_ipAddr));
    iResult = MG_NW_GetDnsIpAddress(SIM0, nCurcid, &dnsAddr, IP_ADDR_TYPE_V6, NETWORK_PDP_TYPE_PRIMARY_DNS_IP);
    if (iResult != MG_RET_OK)
    {
        APP_DEBUG("get dns1 v6 addr[%d] iResult:%d", __LINE__, iResult);
    }
    else
    {
        if(dnsAddr.type == IP_ADDR_TYPE_V6)
        {
            for ( i = 0; i < 4; i++)
            {
                to6[(i*4) + 0] = (u8)((dnsAddr.u_addr.ip6.addr[i] >> 24) & 0xff);
                to6[(i*4) + 1] = (u8)((dnsAddr.u_addr.ip6.addr[i] >> 16) & 0xff);
                to6[(i*4) + 2] = (u8)((dnsAddr.u_addr.ip6.addr[i] >> 8) & 0xff);
                to6[(i*4) + 3] = (u8)((dnsAddr.u_addr.ip6.addr[i] >> 0) & 0xff);
            }

            MG_UTILS_Memset(to6Str, 0, IPV6_STRING_LEN);
            MG_UTILS_Snprintf(to6Str, IPV6_STRING_LEN, "%d.%d.%d.%d.%d.%d.%d.%d.%d.%d.%d.%d.%d.%d.%d.%d",
                                           to6[3], to6[2], to6[1], to6[0],
                                           to6[7], to6[6], to6[5], to6[4],
                                           to6[11], to6[10], to6[9], to6[8],
                                           to6[15], to6[14], to6[13], to6[12]);

            APP_DEBUG("dns1 addr v6 line[%d]:%s", __LINE__, to6Str);
        }
    }
    MG_OS_ThreadSleep(1000);

    /*get DNS2 V4 addr*/
    MG_UTILS_Memset(&dnsAddr, 0, sizeof(ST_ipAddr));
    iResult = MG_NW_GetDnsIpAddress(SIM0, nCurcid, &dnsAddr, IP_ADDR_TYPE_V4, NETWORK_PDP_TYPE_SECONDARY_DNS_IP);
    if (iResult != MG_RET_OK)
    {
        APP_DEBUG("get dns2 v4 addr[%d] iResult:%d", __LINE__, iResult);
    }
    else
    {
        if(dnsAddr.type == IP_ADDR_TYPE_V4)
        {
            to4[0] = (u8)((dnsAddr.u_addr.ip4.addr >> 24) & 0xff);
            to4[1] = (u8)((dnsAddr.u_addr.ip4.addr >> 16) & 0xff);
            to4[2] = (u8)((dnsAddr.u_addr.ip4.addr >> 8) & 0xff);
            to4[3] = (u8)((dnsAddr.u_addr.ip4.addr >> 0) & 0xff);
            APP_DEBUG("dns2 v4 addr line[%d]:%d.%d.%d.%d", __LINE__, to4[3], to4[2], to4[1], to4[0]);
        }
    }
    MG_OS_ThreadSleep(1000);

    /*get DNS1 v6 addr*/
    MG_UTILS_Memset(&dnsAddr, 0, sizeof(ST_ipAddr));
    iResult = MG_NW_GetDnsIpAddress(SIM0, nCurcid, &dnsAddr, IP_ADDR_TYPE_V6, NETWORK_PDP_TYPE_SECONDARY_DNS_IP);
    if (iResult != MG_RET_OK)
    {
        APP_DEBUG("get dns2 v6 addr[%d] iResult:%d", __LINE__, iResult);
    }
    else
    {
        if(dnsAddr.type == IP_ADDR_TYPE_V6)
        {
            for ( i = 0; i < 4; i++)
            {
                to6[(i*4) + 0] = (u8)((dnsAddr.u_addr.ip6.addr[i] >> 24) & 0xff);
                to6[(i*4) + 1] = (u8)((dnsAddr.u_addr.ip6.addr[i] >> 16) & 0xff);
                to6[(i*4) + 2] = (u8)((dnsAddr.u_addr.ip6.addr[i] >> 8) & 0xff);
                to6[(i*4) + 3] = (u8)((dnsAddr.u_addr.ip6.addr[i] >> 0) & 0xff);
            }

            MG_UTILS_Memset(to6Str, 0, IPV6_STRING_LEN);
            MG_UTILS_Snprintf(to6Str, IPV6_STRING_LEN, "%d.%d.%d.%d.%d.%d.%d.%d.%d.%d.%d.%d.%d.%d.%d.%d",
                                           to6[3], to6[2], to6[1], to6[0],
                                           to6[7], to6[6], to6[5], to6[4],
                                           to6[11], to6[10], to6[9], to6[8],
                                           to6[15], to6[14], to6[13], to6[12]);

            APP_DEBUG("dns2 addr v6 line[%d]:%s", __LINE__, to6Str);
        }
    }
    MG_OS_ThreadSleep(1000);

    /*get curr cell info, timeout 20s*/
    ST_NwCurrCellInfo curCellInfo = {0};
    ST_NwLTECurrCellInfo *pLte = NULL;
    u8 PLogBuf[LOG_BUF_STRING_LEN] = {0};
    iResult = MG_NW_GetCurrentCellInfo(SIM0, &curCellInfo, 20*1000);
    if (iResult != MG_RET_OK)
    {
        APP_DEBUG("get curr cell info[%d] iResult:%d", __LINE__, iResult);
    }
    else
    {
        if(curCellInfo.netType == NETWORK_RAT_TYPE_LTE)
        {
            pLte = &(curCellInfo.u_CellInfo.lte);

            MG_UTILS_Memset(PLogBuf, 0, LOG_BUF_STRING_LEN);
            MG_UTILS_Snprintf(PLogBuf, LOG_BUF_STRING_LEN, "mcc:%d%d%d,mnc:%d%d,cellID:%d,"
                      "rsrp:%d,rsrq:%d,tac:%d,pcid:%d,rssi:%d,bandinfo:%d,"
                      "sinr:%d,netMode:%d,Srxlev:%d",
                        pLte->mcc[0], pLte->mcc[1], pLte->mcc[2],
                        pLte->mnc[0], pLte->mnc[1],
                        pLte->cellID,
                        pLte->rsrp,
                        pLte->rsrq,
                        pLte->tac,
                        pLte->pcid,
                        pLte->rssi,
                        pLte->bandinfo,
                        pLte->sinr,
                        pLte->netMode,
                        pLte->Srxlev);
            APP_DEBUG("LTE curr cell info line[%d]:%s", __LINE__, PLogBuf);
        }
        else
        {
            APP_DEBUG("no get curr cell info[%d]", __LINE__);
        }
    }
    MG_OS_ThreadSleep(1000);

    /*get Neighbor cell info, timeout 30s*/
    ST_NwNeighborCellInfo neiCellInfo = {0};
    u8 nCellNum = 0;
    ST_NwLTENeighborCellInfo *pNeiLte = NULL;
    iResult = MG_NW_GetNeighborCellInfo(SIM0, &neiCellInfo, 30*1000);
    if (iResult != MG_RET_OK)
    {
        APP_DEBUG("get Neighbor cell info[%d] iResult:%d", __LINE__, iResult);
    }
    else
    {
        nCellNum = neiCellInfo.cellNum;
        for (i = 0 ; i < nCellNum; i++)
        {
            if(neiCellInfo.netType == NETWORK_RAT_TYPE_LTE)
            {
                pNeiLte = &(neiCellInfo.u_CellInfo.lte[i]);
                MG_UTILS_Memset(PLogBuf, 0, LOG_BUF_STRING_LEN);
                MG_UTILS_Snprintf(PLogBuf, LOG_BUF_STRING_LEN, "mcc:%d%d%d,mnc:%d%d,cellID:%d,"
                          "rsrp:%d,rsrq:%d,tac:%d,pcid:%d",
                            pNeiLte->mcc[0], pNeiLte->mcc[1], pNeiLte->mcc[2],
                            pNeiLte->mnc[0], pNeiLte->mnc[1],
                            pNeiLte->cellID,
                            pNeiLte->rsrp,
                            pNeiLte->rsrq,
                            pNeiLte->tac,
                            pNeiLte->pcid);
                APP_DEBUG("LTE Neighbor cell info line[%d]:%s", __LINE__, PLogBuf);
            }
            MG_OS_ThreadSleep(1000);
        }
    }
    MG_OS_ThreadSleep(1000);

    /*flight mode*/
    u8 nflightModeState = NETWORK_NORMAL_MODE;
    iResult = MG_NW_GetFlightModeState(SIM0, &nflightModeState);
    APP_DEBUG("flight mode status[%d] iResult:%d, nflightModeState:%d", __LINE__, iResult, nflightModeState);
    MG_OS_ThreadSleep(1000);
    iResult = MG_NW_FlightModeEnter(SIM0, NETWORK_SWITCH_FLIGHT_MODE_TIMEOUT_MIN_MS);
    APP_DEBUG("enter flight mode[%d] iResult:%d", __LINE__, iResult);
    MG_OS_ThreadSleep(1000);
    iResult = MG_NW_GetFlightModeState(SIM0, &nflightModeState);
    APP_DEBUG("flight mode status[%d] iResult:%d, nflightModeState:%d", __LINE__, iResult, nflightModeState);
    MG_OS_ThreadSleep(1000);
    iResult = MG_NW_FlightModeExit(SIM0, NETWORK_SWITCH_FLIGHT_MODE_TIMEOUT_MIN_MS);
    APP_DEBUG("exit flight mode[%d] iResult:%d", __LINE__, iResult);
    MG_OS_ThreadSleep(1000);
    iResult = MG_NW_GetFlightModeState(SIM0, &nflightModeState);
    APP_DEBUG("flight mode status[%d] iResult:%d, nflightModeState:%d", __LINE__, iResult, nflightModeState);
    MG_OS_ThreadSleep(1000);

    APP_DEBUG("mgnetwork example exit line[%d]", __LINE__);
    MG_OS_ThreadSleep(1000);
    MG_OS_ThreadExit();
}

void proc_main_task(void *param)
{
    APP_DEBUG("mg example enter line[%d]", __LINE__);
    //MG_TRACE_SetLogLevel(MG_TRACE_LOG_LEVEL_VERBOSE);
    MG_OS_ThreadCreate("mgnetwork", prvThreadEntry, NULL, Thread_PRIORITY_NORMAL, 1024*2, 4);
}

