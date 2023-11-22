
#include "at_util.h"
#include "atc_decoder.h"
#include "mg_openAT_reply.h"

CmsRetId mg_openAT_cmdRespOK(UINT32 atHandle)
{
    return atcReply(atHandle, AT_RC_OK, OPEN_AT_OK, NULL);
}

CmsRetId mg_openAT_cmdRespError(UINT32 atHandle, UINT32 errCode)
{
    return atcReply(atHandle, AT_RC_CME_ERROR, errCode, NULL);
}

CmsRetId mg_openAT_cmdRespTextAndOK(UINT32 atHandle, const CHAR *text)
{
    return atcReply(atHandle, AT_RC_OK, OPEN_AT_OK, text);
}

CmsRetId mg_openAT_cmdRespUrcText(UINT32 atHandle, const CHAR *text)
{
    return atcReply(atHandle, AT_RC_CONTINUE, OPEN_AT_OK, text);
}

CmsRetId mg_openAT_cmdRespOutputPrompt(UINT32 atHandle)
{
    return atcReply(atHandle, AT_RC_RAW_INFO_CONTINUE, OPEN_AT_OK, "\r\n> ");
}

CmsRetId mg_openAT_cmdRespOutputLineBreak(UINT32 atHandle)
{
    return atcReply(atHandle, AT_RC_RAW_INFO_CONTINUE, OPEN_AT_OK, "\r\n");
}
