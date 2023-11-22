/******************************************************************************

*(C) Copyright 2018 EIGENCOMM International Ltd.

* All Rights Reserved

******************************************************************************
*  Filename: atec_meig_utils.h
*
*  Description: Macro definition for network service related AT commands
*
*  History:
*
*  Notes:
*
******************************************************************************/

#ifndef _ATEC_MU_H
#define _ATEC_MU_H


CmsRetId  mgSGSW(const AtCmdInputContext *pAtCmdReq);
CmsRetId  mgSFHW(const AtCmdInputContext *pAtCmdReq);

CmsRetId atCmdHandleMGCFG(const AtCmdInputContext *pAtCmdReq);

CmsRetId atCmdHandleGTSET(const AtCmdInputContext *pAtCmdReq);

CmsRetId atCmdHandleSIMHOTSWAP(const AtCmdInputContext *pAtCmdReq);

CmsRetId atCmdHandleCCID(const AtCmdInputContext *pAtCmdReq);



#endif


