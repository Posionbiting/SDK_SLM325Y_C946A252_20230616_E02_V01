#ifndef __PS_DIAL_NET_H__
#define __PS_DIAL_NET_H__
/******************************************************************************
 ******************************************************************************
 Copyright:      - 2017- Copyrights of EigenComm Ltd.
 File name:      - psdial_net.h
 Description:    - psdial net header
 History:        - 2021/11/17, Originated by xlhu
 ******************************************************************************
******************************************************************************/
#include "commontypedef.h"
#include "cms_api.h"
#include "networkmgr.h"

/******************************************************************************
 *****************************************************************************
 * MARCO
 *****************************************************************************
******************************************************************************/



/******************************************************************************
 *****************************************************************************
 * STRUCT
 *****************************************************************************
******************************************************************************/



/******************************************************************************
 ******************************************************************************
 * External global variable
 ******************************************************************************
******************************************************************************/



/******************************************************************************
 *****************************************************************************
 * Functions
 *****************************************************************************
******************************************************************************/
/*
* psdial net unbind cid for lwip
*/
//void psDialNetDevCtrlUnBindCid(UINT8 cid);

/*
* psdial net bind cid for lwip
*/
//void psDialNetDevCtrlBindCid(UINT8 newCid);

/*
 * psdial net process device control
*/
void psDialNetDevCtl(UINT8 op, UINT8 cid, BOOL bUrcEn);

/*
* psdial net proc appl Ind (NM_ATI_LAN_INFO_IND)
*/
void psDialNetProcApplInd(CmsApplInd *pAppInd);

/*
* psdial net proc Act Pdn Ctx Failure by sending appl Ind (NM_ATI_LAN_INFO_IND)
*/
void psDialNetProcActPdnCtxFailure(void);

/*
 * psdial net WAN link up and lan bind
*/
void psDialNetLinkupCtrl(UINT8 cid, NmIfConfiguration *pIfCfg);

/*
 * psdial net auto dial mode check
*/
void psDialNetAutoDialCheck(void);

/*
* psdial net link down
*/
void psDialNetLinkDown(UINT8 cid);

/*
 * psdial net init and lanconfig for lwip
*/
void psDialNetInit(void);


#endif

