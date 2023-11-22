/******************************************************************************

*(C) Copyright 2018 EIGENCOMM International Ltd.

* All Rights Reserved

******************************************************************************
*  Filename: meig_nvm_config.h
*
*  Description:
*
*  History:
*
*  Notes:
*
******************************************************************************/
#ifndef _MEIG_NVM_CONFIG_H
#define _MEIG_NVM_CONFIG_H


typedef struct MG_NvmConfig_Tag
{
    UINT32  nvVer;
    BOOL    bSimHotplug;   //enable sim hotplug or not
    BOOL    bSimVoltTrigMode;
    BOOL    bsimSimStat;    //enable report qsim stat
    BOOL    URCpending;
}MGNvmConfig;

int MG_NV_SetMgSysParam(MGNvmConfig *pMgNvmCfg);
int MG_NV_GetMgSysParam(MGNvmConfig *pMgNvmCfg);

#endif


