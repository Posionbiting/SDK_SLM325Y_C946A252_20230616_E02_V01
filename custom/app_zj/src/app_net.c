#include "app_include.h"
#include "mg_network.h"

#define LOG_TAG MAKE_LOG_TAG('U', 'S', 'E', 'R')

static void app_net_status_cb(s32 eventId, ST_NwRegInfo *info, void *param)
{
    ST_NwRegInfo *pInfo = info;

    if (!pInfo)
        return;

    switch(eventId)
    {
    case EV_OPEN_SIM_CARD_INSERT:
        APP_DEBUG("EV_OPEN_SIM_CARD_INSERT line[%d]", __LINE__);
        break;
    case EV_OPEN_SIM_CARD_PULL_OUT:
        APP_DEBUG("EV_OPEN_SIM_CARD_PULL_OUT line[%d]", __LINE__);
        break;
    case EV_OPEN_PS_DOMAIN_NO_REGISTER:
        g_app_cfg.netState = NET_NO_ACT;
        APP_DEBUG("EV_OPEN_PS_DOMAIN_NO_REGISTER line[%d]", __LINE__);
        break;
    case EV_OPEN_PS_DOMAIN_REGISTER:
        APP_DEBUG("EV_OPEN_PS_DOMAIN_REGISTER line[%d]", __LINE__);
        break;
    case EV_OPEN_PDP_MANUAL_DEACTIVATION:
        APP_DEBUG("EV_OPEN_PDP_MANUAL_DEACTIVATION sim:%d cid:%d line[%d]", pInfo->nSim, pInfo->nCid, __LINE__);
        g_app_cfg.netState = NET_NO_ACT;
        break;
    case EV_OPEN_PDP_MANUAL_ACTIVATION:
        g_app_cfg.netState = NET_ACTED;
        APP_DEBUG("EV_OPEN_PDP_MANUAL_ACTIVATION sim:%d cid:%d line[%d]", pInfo->nSim, pInfo->nCid, __LINE__);
        break;
    case EV_OPEN_PDP_AUTO_DEACTIVATION:
        APP_DEBUG("EV_OPEN_PDP_AUTO_DEACTIVATION sim:%d cid:%d line[%d]", pInfo->nSim, pInfo->nCid, __LINE__);
        g_app_cfg.netState = NET_NO_ACT;
        //app_net_sendMsg(EV_ACT_NET, pInfo->nSim, pInfo->nCid);
        break;
    case EV_OPEN_PDP_AUTO_ACTIVATION:
        g_app_cfg.netState = NET_ACTED;
        APP_DEBUG("EV_OPEN_PDP_AUTO_ACTIVATION sim:%d cid:%d line[%d]", pInfo->nSim, pInfo->nCid, __LINE__);
        break;
    default:
        break;
    }
}

// Activate manually
static bool app_net_doActEx(u8 simId, u8 cid, u32 timeout)
{
    s32 iRet = 0;
    u8 nSignalLevel = 99;

    /*get csq*/
    iRet = MG_NW_GetSignalQuality(simId, &nSignalLevel);
    g_app_cfg.netInfo.signalLevel = nSignalLevel;
    APP_DEBUG("[%d]get csq-> iRet:%d, nSignalLevel:%d", __LINE__, iRet, nSignalLevel);
    if (MG_RET_OK == iRet)
    {
        /*get network state*/
        Enum_NwState nNwState = NW_STATE_NO_REGISTERED;
        iRet = MG_NW_GetNetWorkState(simId, cid, &nNwState);
        APP_DEBUG("[%d]get network state-> iRet:%d, nNwState:%d", __LINE__, iRet, nNwState);
        if (nNwState == NW_STATE_LTE_ACTIVED)
            return true;
    }

    /*set default network cofig*/
    if (cid != PDP_CID_1)
    {
        iRet = MG_NW_Config(simId, cid, NULL);
        APP_DEBUG("[%d]cid %d set default network cofig-> iRet:%d", __LINE__, cid, iRet);
        if (MG_RET_OK == iRet)
        {
            /*do act, timeout 10s*/
            iRet = MG_NW_Activate(simId, cid, timeout*1000);
            APP_DEBUG("[%d]cid %d do act-> iRet:%d", __LINE__, cid, iRet);
        }
    }

    return iRet != MG_RET_OK ? false : true;
}

void app_net_doAct(int iTimeout, app_cfg_t *pApp)
{
    bool isAct = false;
    int i = iTimeout/500;

    do
    {
        isAct = app_net_doActEx(pApp->netInfo.simId, pApp->netInfo.cid, 10);
        if (isAct == true && pApp->netState == NET_ACTED)
        {
            break;
        }
        app_util_threadSleep(500, pApp->bLowPowerModeEn);
    }while(--i > 0);
}

void app_net_task(void)
{
    MG_NW_Register(app_net_status_cb, NULL);
    g_app_cfg.netState = NET_ACTING;
}

