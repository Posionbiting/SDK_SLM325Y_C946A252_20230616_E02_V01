
#include "at_util.h"
#include "atc_decoder.h"

#include "mg_common.h"
#include "mg_trace.h"
#include "mg_utils.h"

#define OPEN_AT_PARAM_STR_DEF        NULL

s32 mg_openAT_cmdParamUintInRange(AtParamValueCP pAtParaList, u32 index, s32 minValue, s32 maxValue, s32 defaultValue, BOOL *result)
{
    AtParaRet iRet = AT_PARA_ERR;
    INT32 nValue = -1;

    if (!*result)
    {
        *result = FALSE;
        return 0;
    }

    iRet = atGetNumValue(pAtParaList, (UINT32)index, &nValue, (INT32)minValue, (INT32)maxValue, defaultValue);
    *result = (iRet != AT_PARA_ERR) ? (TRUE) : (FALSE);

    return (s32)nValue;
}

s32 mg_openAT_cmdParamStr(AtParamValueCP pAtParaList, u32 index, u8 *pOutStrBuf, u16 maxOutBufLen, BOOL *result)
{
    AtParaRet iRet = AT_PARA_ERR;
    UINT16 nLen = 0;

    if (!*result)
    {
        *result = FALSE;
        return 0;
    }

    iRet = atGetStrValue(pAtParaList, (UINT32)index, (UINT8  *)pOutStrBuf, (UINT16)maxOutBufLen, &nLen, (CHAR *)OPEN_AT_PARAM_STR_DEF);
    *result = (iRet != AT_PARA_ERR) ? (TRUE) : (FALSE);

    return (s32)nLen;
}
