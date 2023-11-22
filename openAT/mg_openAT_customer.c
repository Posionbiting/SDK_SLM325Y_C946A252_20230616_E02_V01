
#include "at_util.h"
#include "atc_decoder.h"

#include "mg_openAT_param.h"
#include "mg_openAT_reply.h"
#include "mg_openAT_engine.h"
#include "app_include.h"

#define LOG_TAG MAKE_LOG_TAG('U', 'S', 'E', 'R')

CmsRetId openAT_USGSW(const AtCmdInputContext *pAtCmdReq)
{
    CmsRetId    rc = CMS_FAIL;
    UINT32      atHandle = AT_SET_SRC_HANDLER(pAtCmdReq->tid, CMS_REF_SUB_AT_5_ID, pAtCmdReq->chanId);
    UINT32      operaType = (UINT32)pAtCmdReq->operaType;
    CHAR mBuff[256] = {0};
    u8 mVer[64];
    int iLen = 0;

    switch (operaType)
    {
    case AT_EXEC_REQ:
        // soft ver
        iLen = 0;
        memset(mBuff, 0, sizeof(mBuff));
        iLen += sprintf(mBuff+iLen, "APP Version: %s\r\n", APP_VER);

        // sdk ver
        memset(mVer, 0, sizeof(mVer));
        MG_SYSTEM_GetModuleInfo(mVer, sizeof(mVer));
        iLen += sprintf(mBuff+iLen, "SDK Version: %s", mVer);
        rc = mg_openAT_cmdRespTextAndOK(atHandle, mBuff);
        break;
    default:
        rc = mg_openAT_cmdRespError(atHandle, OPEN_AT_ERROR_NOT_SUPPORT);
        break;
    }

    return rc;
}

CmsRetId openAT_USN(const AtCmdInputContext *pAtCmdReq)
{
    CmsRetId    rc = CMS_FAIL;
    UINT32      atHandle = AT_SET_SRC_HANDLER(pAtCmdReq->tid, CMS_REF_SUB_AT_5_ID, pAtCmdReq->chanId);
    UINT32      operaType = (UINT32)pAtCmdReq->operaType;
    BOOL        paramok = TRUE;
    s32 mLen = 0;
    CHAR mBuff[64+1] = {0};

    switch (operaType)
    {
    case AT_TEST_REQ:
        rc = mg_openAT_cmdRespTextAndOK(atHandle, "+USN: <SN>");
        break;

    case AT_READ_REQ:
        memset(mBuff, 0, sizeof(mBuff));
        sprintf(mBuff, "%s: %s", pAtCmdReq->pName, g_app_cfg.nvConfig.sn);
        rc = mg_openAT_cmdRespTextAndOK(atHandle, mBuff);
        break;

    case AT_SET_REQ:
        mLen = mg_openAT_cmdParamStr(pAtCmdReq->pParamList, 0, (u8 *)mBuff, 20, &paramok);
        if(!paramok || mLen <= 0)
        {
            rc = mg_openAT_cmdRespError(atHandle, OPEN_AT_ERROR_PARAM);
            break;
        }
        g_app_cfg.nvConfig.batterySize=15200000;
        g_app_cfg.nvConfig.batteryCount=0;

        memset(g_app_cfg.nvConfig.sn, 0, sizeof(g_app_cfg.nvConfig.sn));
        memcpy(g_app_cfg.nvConfig.sn, mBuff, mLen);
        app_nvmSaveConfig();
        rc = mg_openAT_cmdRespOK(atHandle);
        break;

    default:
        rc = mg_openAT_cmdRespError(atHandle, OPEN_AT_ERROR_NOT_SUPPORT);
        break;
    }

    return rc;
}

CmsRetId openAT_UKEY(const AtCmdInputContext *pAtCmdReq)
{
    CmsRetId    rc = CMS_FAIL;
    UINT32      atHandle = AT_SET_SRC_HANDLER(pAtCmdReq->tid, CMS_REF_SUB_AT_5_ID, pAtCmdReq->chanId);
    UINT32      operaType = (UINT32)pAtCmdReq->operaType;
    BOOL        paramok = TRUE;
    s32 mLen = 0;
    CHAR mBuff[64+1] = {0};

    switch (operaType)
    {
    case AT_TEST_REQ:
        rc = mg_openAT_cmdRespTextAndOK(atHandle, "+UKEY: <KEY>");
        break;

    case AT_READ_REQ:
        memset(mBuff, 0, sizeof(mBuff));
        sprintf(mBuff, "%s: %s", pAtCmdReq->pName, g_app_cfg.nvConfig.key);
        rc = mg_openAT_cmdRespTextAndOK(atHandle, mBuff);
        break;

    case AT_SET_REQ:
        mLen = mg_openAT_cmdParamStr(pAtCmdReq->pParamList, 0, (u8 *)mBuff, 20, &paramok);
        if(!paramok || mLen <= 0)
        {
            rc = mg_openAT_cmdRespError(atHandle, OPEN_AT_ERROR_PARAM);
            break;
        }

        memset(g_app_cfg.nvConfig.key, 0, sizeof(g_app_cfg.nvConfig.key));
        memcpy(g_app_cfg.nvConfig.key, mBuff, mLen);
        app_nvmSaveConfig();
        rc = mg_openAT_cmdRespOK(atHandle);
        break;

    default:
        rc = mg_openAT_cmdRespError(atHandle, OPEN_AT_ERROR_NOT_SUPPORT);
        break;
    }

    return rc;
}

CmsRetId openAT_USECRET(const AtCmdInputContext *pAtCmdReq)
{
    CmsRetId    rc = CMS_FAIL;
    UINT32      atHandle = AT_SET_SRC_HANDLER(pAtCmdReq->tid, CMS_REF_SUB_AT_5_ID, pAtCmdReq->chanId);
    UINT32      operaType = (UINT32)pAtCmdReq->operaType;
    BOOL        paramok = TRUE;
    s32 mLen = 0;
    CHAR mBuff[64+1] = {0};

    switch (operaType)
    {
    case AT_TEST_REQ:
        rc = mg_openAT_cmdRespTextAndOK(atHandle, "+USECRET: <SECRET>");
        break;

    case AT_READ_REQ:
        memset(mBuff, 0, sizeof(mBuff));
        sprintf(mBuff, "%s: %s", pAtCmdReq->pName, g_app_cfg.nvConfig.secret);
        rc = mg_openAT_cmdRespTextAndOK(atHandle, mBuff);
        break;

    case AT_SET_REQ:
        mLen = mg_openAT_cmdParamStr(pAtCmdReq->pParamList, 0, (u8 *)mBuff, 20, &paramok);
        if(!paramok || mLen <= 0)
        {
            rc = mg_openAT_cmdRespError(atHandle, OPEN_AT_ERROR_PARAM);
            break;
        }

        memset(g_app_cfg.nvConfig.secret, 0, sizeof(g_app_cfg.nvConfig.secret));
        memcpy(g_app_cfg.nvConfig.secret, mBuff, mLen);
        app_nvmSaveConfig();
        rc = mg_openAT_cmdRespOK(atHandle);
        break;

    default:
        rc = mg_openAT_cmdRespError(atHandle, OPEN_AT_ERROR_NOT_SUPPORT);
        break;
    }

    return rc;
}

CmsRetId openAT_UTYPE(const AtCmdInputContext *pAtCmdReq)
{
    CmsRetId    rc = CMS_FAIL;
    UINT32      atHandle = AT_SET_SRC_HANDLER(pAtCmdReq->tid, CMS_REF_SUB_AT_5_ID, pAtCmdReq->chanId);
    UINT32      operaType = (UINT32)pAtCmdReq->operaType;
    BOOL        paramok = TRUE;
    s32 mLen = 0;
    CHAR mBuff[64+1] = {0};

    switch (operaType)
    {
    case AT_TEST_REQ:
        rc = mg_openAT_cmdRespTextAndOK(atHandle, "+UTYPE: <TYPE>");
        break;

    case AT_READ_REQ:
        memset(mBuff, 0, sizeof(mBuff));
        sprintf(mBuff, "%s: %s", pAtCmdReq->pName, g_app_cfg.nvConfig.type);
        rc = mg_openAT_cmdRespTextAndOK(atHandle, mBuff);
        break;

    case AT_SET_REQ:
        mLen = mg_openAT_cmdParamStr(pAtCmdReq->pParamList, 0, (u8 *)mBuff, 20, &paramok);
        if(!paramok || mLen <= 0)
        {
            rc = mg_openAT_cmdRespError(atHandle, OPEN_AT_ERROR_PARAM);
            break;
        }

        memset(g_app_cfg.nvConfig.type, 0, sizeof(g_app_cfg.nvConfig.type));
        memcpy(g_app_cfg.nvConfig.type, mBuff, mLen);
        app_nvmSaveConfig();
        rc = mg_openAT_cmdRespOK(atHandle);
        break;

    default:
        rc = mg_openAT_cmdRespError(atHandle, OPEN_AT_ERROR_NOT_SUPPORT);
        break;
    }

    return rc;
}

CmsRetId openAT_USETUPLOAD(const AtCmdInputContext *pAtCmdReq)
{
    CmsRetId    rc = CMS_FAIL;
    UINT32      atHandle = AT_SET_SRC_HANDLER(pAtCmdReq->tid, CMS_REF_SUB_AT_5_ID, pAtCmdReq->chanId);
    UINT32      operaType = (UINT32)pAtCmdReq->operaType;
    BOOL        paramok = TRUE;
    s32 mLen = 0;
    CHAR mBuff[64+1] = {0};

    switch (operaType)
    {
    case AT_TEST_REQ:
        rc = mg_openAT_cmdRespTextAndOK(atHandle, "+USETUPLOAD: <interval time>");
        break;

    case AT_READ_REQ:
        memset(mBuff, 0, sizeof(mBuff));
        sprintf(mBuff, "%s: %d", pAtCmdReq->pName, g_app_cfg.nvConfig.nReportCycle);
        rc = mg_openAT_cmdRespTextAndOK(atHandle, mBuff);
        break;

    case AT_SET_REQ:
        mLen = mg_openAT_cmdParamUintInRange(pAtCmdReq->pParamList, 0, 1, 86400, 8640000, &paramok);
        if(!paramok || mLen <= 0)
        {
            rc = mg_openAT_cmdRespError(atHandle, OPEN_AT_ERROR_PARAM);
            break;
        }

        g_app_cfg.nvConfig.nReportCycle = mLen;
        app_nvmSaveConfig();
        rc = mg_openAT_cmdRespOK(atHandle);
        break;

    default:
        rc = mg_openAT_cmdRespError(atHandle, OPEN_AT_ERROR_NOT_SUPPORT);
        break;
    }


    return rc;
}

CmsRetId openAT_UWORKSTATE(const AtCmdInputContext *pAtCmdReq)
{
    CmsRetId    rc = CMS_FAIL;
    UINT32      atHandle = AT_SET_SRC_HANDLER(pAtCmdReq->tid, CMS_REF_SUB_AT_5_ID, pAtCmdReq->chanId);
    UINT32      operaType = (UINT32)pAtCmdReq->operaType;
    CHAR mBuff[64+1] = {0};

    switch (operaType)
    {
    case AT_EXEC_REQ:
        memset(mBuff, 0, sizeof(mBuff));
        sprintf(mBuff, "%s: %d", pAtCmdReq->pName, g_app_cfg.workState);
        rc = mg_openAT_cmdRespTextAndOK(atHandle, mBuff);
        break;

    default:
        rc = mg_openAT_cmdRespError(atHandle, OPEN_AT_ERROR_NOT_SUPPORT);
        break;
    }

    return rc;
}

CmsRetId openAT_UALARM(const AtCmdInputContext *pAtCmdReq)
{
    CmsRetId    rc = CMS_FAIL;
    UINT32      atHandle = AT_SET_SRC_HANDLER(pAtCmdReq->tid, CMS_REF_SUB_AT_5_ID, pAtCmdReq->chanId);
    UINT32      operaType = (UINT32)pAtCmdReq->operaType;
    BOOL        paramok = TRUE;
    s32         mSysState = 0;

    switch (operaType)
    {
    case AT_SET_REQ:
        mSysState = mg_openAT_cmdParamUintInRange(pAtCmdReq->pParamList, 0, 0, 1, 0, &paramok);
        if(!paramok)
        {
            rc = mg_openAT_cmdRespError(atHandle, OPEN_AT_ERROR_PARAM);
            break;
        }

        if (g_app_cfg.nvConfig.sysState != SYS_STATE_LOCK)
        {
            g_app_cfg.nvConfig.sysState = (app_sys_state)mSysState;
            app_nvmSaveConfig();
        }
        rc = mg_openAT_cmdRespOK(atHandle);
        break;

    default:
        rc = mg_openAT_cmdRespError(atHandle, OPEN_AT_ERROR_NOT_SUPPORT);
        break;
    }

    return rc;
}

int app_gps_openGpsANT(void);
int app_gps_closeGpsANT(void);
CmsRetId openAT_UTest(const AtCmdInputContext *pAtCmdReq)
{
    CmsRetId    rc = CMS_FAIL;
    UINT32      atHandle = AT_SET_SRC_HANDLER(pAtCmdReq->tid, CMS_REF_SUB_AT_5_ID, pAtCmdReq->chanId);
    UINT32      operaType = (UINT32)pAtCmdReq->operaType;
    BOOL        paramok = TRUE;
    s32 mLen = 0;

    switch (operaType)
    {
    case AT_SET_REQ:
        mLen = mg_openAT_cmdParamUintInRange(pAtCmdReq->pParamList, 0, 0, 1, 1, &paramok);
        if(!paramok || mLen < 0)
        {
            rc = mg_openAT_cmdRespError(atHandle, OPEN_AT_ERROR_PARAM);
            break;
        }

        if (mLen == 1)
            app_gps_openGpsANT();
        else
            app_gps_closeGpsANT();

        rc = mg_openAT_cmdRespOK(atHandle);
        break;

    default:
        rc = mg_openAT_cmdRespError(atHandle, OPEN_AT_ERROR_NOT_SUPPORT);
        break;
    }

    return rc;
}


