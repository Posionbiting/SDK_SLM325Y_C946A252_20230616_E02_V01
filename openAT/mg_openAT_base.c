
#include "at_util.h"
#include "atc_decoder.h"

#include "mg_openAT_param.h"
#include "mg_openAT_reply.h"
#include "mg_openAT_engine.h"
#include "mg_common.h"
#include "mg_trace.h"
#include "mg_utils.h"
#include "mg_sys.h"

static void cmd_MINFO_process(void *argument)
{
    s32 nRet = -1;
    u8 nModuleInfo[MODULE_INFO_MIN_LENGTH] = {0};
    CHAR rspBuf[ATEC_IND_RESP_128_STR_LEN] = {0};
    UINT32 atHandle = (UINT32)argument;
    nRet = MG_SYSTEM_GetModuleInfo(nModuleInfo, MODULE_INFO_MIN_LENGTH);
    if (nRet < 0)
    {
        mg_openAT_cmdRespError(atHandle, OPEN_AT_ERROR);
    }
    MG_UTILS_Snprintf((u8 *)rspBuf, ATEC_IND_RESP_128_STR_LEN, "+MINFO: %s", nModuleInfo);
    mg_openAT_cmdRespTextAndOK(atHandle, rspBuf);
}

CmsRetId openMINFO(const AtCmdInputContext *pAtCmdReq)
{
    CmsRetId    rc = CMS_FAIL;
    UINT32      atHandle = AT_SET_SRC_HANDLER(pAtCmdReq->tid, CMS_REF_SUB_AT_5_ID, pAtCmdReq->chanId);
    UINT32      operaType = (UINT32)pAtCmdReq->operaType;
    CHAR        rspBuf[ATEC_IND_RESP_128_STR_LEN] = {0};
    BOOL        paramok = TRUE;
    s32         nRet = -1;

    switch (operaType)
    {
        case AT_TEST_REQ:             /* AT+MINFO=? */
        {
            MG_UTILS_Snprintf((u8 *)rspBuf, ATEC_IND_RESP_128_STR_LEN, "+MINFO: <type>");
            rc = mg_openAT_cmdRespTextAndOK(atHandle, rspBuf);
            break;
        }

        /* AT synchronization process */
        case AT_SET_REQ:              /* AT+MINFO=0 */
        {
            u8 nModuleInfo[MODULE_INFO_MIN_LENGTH] = {0};
            s32 type = 0;

            nRet = mg_openAT_cmdParamUintInRange(pAtCmdReq->pParamList, 0, 0, 1, 0, &paramok);
            if (!paramok)
            {
                rc = mg_openAT_cmdRespError(atHandle, OPEN_AT_ERROR_PARAM);
                break;
            }

            type = nRet;

            if (type == 0)
            {
                nRet = MG_SYSTEM_GetModuleInfo(nModuleInfo, MODULE_INFO_MIN_LENGTH);
                if (nRet < 0)
                {
                    rc = mg_openAT_cmdRespError(atHandle, OPEN_AT_ERROR);
                    break;
                }

                MG_UTILS_Snprintf((u8 *)rspBuf, ATEC_IND_RESP_128_STR_LEN, "+MINFO: %s", nModuleInfo);
                rc = mg_openAT_cmdRespTextAndOK(atHandle, rspBuf);
                break;
            }

            rc = mg_openAT_cmdRespError(atHandle, OPEN_AT_ERROR_NOT_SUPPORT);
            break;
        }

        /* AT asynchronous processing */
        case AT_READ_REQ:             /* AT+MINFO? */
        {
            if ( mg_openAT_EngineCallback(cmd_MINFO_process, (void *)atHandle) )
            {
                rc = CMS_RET_SUCC;
            }
            else
            {
                rc = mg_openAT_cmdRespError(atHandle, OPEN_AT_ERROR);
            }
            break;
        }

        case AT_EXEC_REQ:            /* AT+MINFO */
        {
            rc = mg_openAT_cmdRespOK(atHandle);
            break;
        }

        default:
        {
            rc = mg_openAT_cmdRespError(atHandle, OPEN_AT_ERROR_NOT_SUPPORT);
            break;
        }
    }

    return rc;
}