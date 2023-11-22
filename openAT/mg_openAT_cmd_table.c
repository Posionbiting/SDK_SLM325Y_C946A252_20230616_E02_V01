
#include "at_util.h"

#ifdef FEATURE_OPENCPU_AT_ENABLE

#include "mg_openAT_base.h"
#include "mg_openAT_fs.h"
#include "mg_openAT_param.h"
#include "mg_openAT_config.h"

#define CONFIG_CUSTOMER_ZJ
#ifdef CONFIG_CUSTOMER_ZJ
CmsRetId openAT_USGSW(const AtCmdInputContext *pAtCmdReq);
CmsRetId openAT_USN(const AtCmdInputContext *pAtCmdReq);               //设置设备SN
CmsRetId openAT_USETUPLOAD(const AtCmdInputContext *pAtCmdReq); //周期值设置
CmsRetId openAT_UWORKSTATE(const AtCmdInputContext *pAtCmdReq); //工作状态查询
CmsRetId openAT_UALARM(const AtCmdInputContext *pAtCmdReq); //防拆报警上报
CmsRetId openAT_UKEY(const AtCmdInputContext *pAtCmdReq);               //设置设备Key
CmsRetId openAT_UTYPE(const AtCmdInputContext *pAtCmdReq);               //设置设备TYPE
CmsRetId openAT_USECRET(const AtCmdInputContext *pAtCmdReq);               //设置设备Secret
//CmsRetId openAT_UTest(const AtCmdInputContext *pAtCmdReq);

const static AtValueAttr USN_PARAM_TABLE[] = { AT_PARAM_DEF(1, AT_PARAM_TYPE_STRING, AT_PARAM_ATTR_MUST) };
const static AtValueAttr UKEY_PARAM_TABLE[] = { AT_PARAM_DEF(1, AT_PARAM_TYPE_STRING, AT_PARAM_ATTR_MUST) };
const static AtValueAttr USECRET_PARAM_TABLE[] = { AT_PARAM_DEF(1, AT_PARAM_TYPE_STRING, AT_PARAM_ATTR_MUST) };
const static AtValueAttr UTYPE_PARAM_TABLE[] = { AT_PARAM_DEF(1, AT_PARAM_TYPE_STRING, AT_PARAM_ATTR_MUST) };
const static AtValueAttr USETUPLOAD_PARAM_TABLE[] = { AT_PARAM_DEF(1, AT_PARAM_TYPE_INT32, AT_PARAM_ATTR_MUST) };
const static AtValueAttr UALARM_PARAM_TABLE[] = { AT_PARAM_DEF(1, AT_PARAM_TYPE_INT32, AT_PARAM_ATTR_MUST) };
//const static AtValueAttr UTEST_PARAM_TABLE[] = { AT_PARAM_DEF(1, AT_PARAM_TYPE_INT32, AT_PARAM_ATTR_MUST) };
#endif

#if OPENAT_BASE_ENABLE
//AT+MINFO
const static AtValueAttr MINFO_PARAM_TABLE[]     = { AT_PARAM_DEF(1, AT_PARAM_TYPE_INT32, AT_PARAM_ATTR_MUST)};
#endif

#if OPENAT_FS_ENABLE
//AT+FSDWNFILE
const static AtValueAttr FSDWNFILE_PARAM_TABLE[] = { AT_PARAM_DEF(1, AT_PARAM_TYPE_STRING, AT_PARAM_ATTR_MUST),
                                                     AT_PARAM_DEF(2, AT_PARAM_TYPE_INT32, AT_PARAM_ATTR_MUST),
                                                     AT_PARAM_DEF(3, AT_PARAM_TYPE_INT32, AT_PARAM_ATTR_OPTIONAL)};
//AT+FSRDFILE
const static AtValueAttr FSRDFILE_PARAM_TABLE[]  = { AT_PARAM_DEF(1, AT_PARAM_TYPE_STRING, AT_PARAM_ATTR_MUST)};
//AT+FSDELFILE
const static AtValueAttr FSDELFILE_PARAM_TABLE[] = { AT_PARAM_DEF(1,AT_PARAM_TYPE_STRING, AT_PARAM_ATTR_MUST)};
//AT+FSLSTFILE
const static AtValueAttr FSLSTFILE_PARAM_TABLE[] = { AT_PARAM_DEF(1,AT_PARAM_TYPE_STRING, AT_PARAM_ATTR_MUST)};
#endif

const AtCmdPreDefInfo  openAtCmdTable[] =
{
#if OPENAT_BASE_ENABLE
    AT_CMD_PRE_DEFINE("+MINFO",              openMINFO,            MINFO_PARAM_TABLE,            AT_EXT_PARAM_CMD,        AT_DEFAULT_TIMEOUT_SEC),
#endif

#if OPENAT_FS_ENABLE
    AT_CMD_PRE_DEFINE("+FSDWNFILE",          openFSDWNFILE,        FSDWNFILE_PARAM_TABLE,        AT_EXT_PARAM_CMD,        (6*AT_DEFAULT_TIMEOUT_SEC)),
    AT_CMD_PRE_DEFINE("+FSRDFILE",           openFSRDFILE,         FSRDFILE_PARAM_TABLE,         AT_EXT_PARAM_CMD,        AT_DEFAULT_TIMEOUT_SEC),
    AT_CMD_PRE_DEFINE("+FSDELFILE",          openFSDELFILE,        FSDELFILE_PARAM_TABLE,        AT_EXT_PARAM_CMD,        AT_DEFAULT_TIMEOUT_SEC),
    AT_CMD_PRE_DEFINE("+FSLSTFILE",          openFSLSTFILE,        FSLSTFILE_PARAM_TABLE,        AT_EXT_PARAM_CMD,        AT_DEFAULT_TIMEOUT_SEC),
#endif

#ifdef CONFIG_CUSTOMER_ZJ
    AT_CMD_PRE_DEFINE("+USGSW",             openAT_USGSW,            NULL, AT_EXT_PARAM_CMD, AT_DEFAULT_TIMEOUT_SEC),
    AT_CMD_PRE_DEFINE("+USN",               openAT_USN,              USN_PARAM_TABLE, AT_EXT_PARAM_CMD, AT_DEFAULT_TIMEOUT_SEC),
    AT_CMD_PRE_DEFINE("+UKEY",              openAT_UKEY,             UKEY_PARAM_TABLE, AT_EXT_PARAM_CMD, AT_DEFAULT_TIMEOUT_SEC),
    AT_CMD_PRE_DEFINE("+UTYPE",             openAT_UTYPE,            UTYPE_PARAM_TABLE, AT_EXT_PARAM_CMD, AT_DEFAULT_TIMEOUT_SEC),
    AT_CMD_PRE_DEFINE("+USECRET",           openAT_USECRET,          USECRET_PARAM_TABLE, AT_EXT_PARAM_CMD, AT_DEFAULT_TIMEOUT_SEC),
    AT_CMD_PRE_DEFINE("+USETUPLOAD",        openAT_USETUPLOAD,       USETUPLOAD_PARAM_TABLE, AT_EXT_PARAM_CMD, AT_DEFAULT_TIMEOUT_SEC),
    AT_CMD_PRE_DEFINE("+UWORKSTATE",        openAT_UWORKSTATE,       NULL, AT_EXT_PARAM_CMD, AT_DEFAULT_TIMEOUT_SEC),
    AT_CMD_PRE_DEFINE("+UALARM",            openAT_UALARM,           UALARM_PARAM_TABLE, AT_EXT_PARAM_CMD, AT_DEFAULT_TIMEOUT_SEC),
    //AT_CMD_PRE_DEFINE("+UTEST",             openAT_UTest,          UTEST_PARAM_TABLE, AT_EXT_PARAM_CMD, AT_DEFAULT_TIMEOUT_SEC*20),
#endif
};

AtCmdPreDefInfoC *atcGetOpenAtCmdSeqPointer(void)
{
   return  (AtCmdPreDefInfoC *)openAtCmdTable;
}

UINT32 atcGetOpenAtCmdSeqNumb(void)
{
    return AT_NUM_OF_ARRAY(openAtCmdTable);
}

#else
AtCmdPreDefInfoC *atcGetOpenAtCmdSeqPointer(void)
{
   return  NULL;
}

UINT32 atcGetOpenAtCmdSeqNumb(void)
{
    return 0;
}

#endif
