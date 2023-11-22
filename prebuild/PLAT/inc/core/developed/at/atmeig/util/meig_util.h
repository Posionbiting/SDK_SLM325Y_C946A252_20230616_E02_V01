/******************************************************************************

*(C) Copyright 2018 EIGENCOMM International Ltd.

* All Rights Reserved

******************************************************************************
*  Filename: meig_util.h
*
*  Description:
*
*  History:
*
*  Notes:
*
******************************************************************************/
#ifndef _MEIG_UTIL_H
#define _MEIG_UTIL_H

#include DEBUG_LOG_HEADER_FILE

#define MG_LOGE(tag, fmt, ...)    ECPLAT_PRINTF(UNILOG_MCI, tag, P_ERROR, fmt, ##__VA_ARGS__)
#define MG_LOGW(tag, fmt, ...)    ECPLAT_PRINTF(UNILOG_MCI, tag, P_WARNING, fmt, ##__VA_ARGS__)
#define MG_LOGI(tag, fmt, ...)    ECPLAT_PRINTF(UNILOG_MCI, tag, P_INFO, fmt, ##__VA_ARGS__)
#define MG_LOGV(tag, fmt, ...)    ECPLAT_PRINTF(UNILOG_MCI, tag, P_VALUE, fmt, ##__VA_ARGS__)
#define MG_LOGD(tag, fmt, ...)    ECPLAT_PRINTF(UNILOG_MCI, tag, P_DEBUG, fmt, ##__VA_ARGS__)

extern INT32 gsktmode;
extern INT32 ghttptype;
extern INT32 gfotasize;

//outdata: projectName_buildGitRevision_projectDate_xxx_ver
int MEIG_Util_GetSGSW_ID(char *outData, int outDataSize);

//outdata: projectName_xxx_ver
int MEIG_Util_GetGMR_ID(char *outData, int outDataSize);

int MEIG_Util_IP_Filter(const char *sIp, char *tIp, int tIpSize);

//<!--[ODM][TaskID 28909]liulei@2022-11-25 add AT+MGSYS
int MEIG_Util_SimHotplugIsEnable(void);
//end-->

#endif


