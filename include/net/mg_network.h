/**
  ******************************************************************************
  * @file    mg_network.h
  * @author  MEIG Development Team
  * @version V1.0.0
  * @date    28-Mar-2022
  * @brief   This file provides all the network function APIs.
  ******************************************************************************
  *  @attention
  *
  * THE SOFTWARE IS PROTECTED BY COPYRIGHT AND THE INFORMATION CONTAINED HEREIN
  * IS CONFIDENTIAL. THE SOFTWARE MAY NOT BE COPIED AND THE INFORMATION CONTAINED
  * HEREIN MAY NOT BE USED OR DISCLOSED EXCEPT WITH THE WRITTEN PERMISSION OF
  * MEIG SMART TECHNOLOGY CO., LTD.
  *
  * <h2><center>&copy; COPYRIGHT 2022 MEIG SMART</center></h2>
  ******************************************************************************
  */

#ifndef _MG_NETWORK_H_
#define _MG_NETWORK_H_
#include "mg_type.h"
#include "mg_ip.h"
#include "mg_opt_event.h"

#ifdef __cplusplus
extern "C" {
#endif

/****************************************************************************
 *Definitons and Declarations
 ***************************************************************************/
#define NETWORK_APN_LEN_MAX 99
#define NETWORK_APN_USER_LEN_MAX 64
#define NETWORK_APN_PWD_LEN_MAX 64

/* IP type*/
#define NETWORK_PDP_TYPE_IP 1         ///<  IP
#define NETWORK_PDP_TYPE_IPV6 2       ///<  IP Version 6
#define NETWORK_PDP_TYPE_IPV4V6 3     ///< DualStack

/* DNS IP type*/
#define NETWORK_PDP_TYPE_PRIMARY_DNS_IP 1         ///<  Primary DNS
#define NETWORK_PDP_TYPE_SECONDARY_DNS_IP 2       ///<  Secondary DNS

/* Authentication protocol used for PDP context */
#define NETWORK_AUTHTYPE_NULL 0    ///<  NULL
#define NETWORK_AUTHTYPE_PAP 1     ///<  PAP
#define NETWORK_AUTHTYPE_CHAP 2    ///<  CHAP

/**
 * The minimum timeout for switching flight mode in millisecond.
 */
#define NETWORK_SWITCH_FLIGHT_MODE_TIMEOUT_MIN_MS (30*1000)

/* flight mode */
#define NETWORK_FLIGHT_MODE 0     ///<  enter flight mode
#define NETWORK_NORMAL_MODE 1     ///<  exit flight mode

/* net type */
#define NETWORK_RAT_TYPE_GSM 2     ///<  GSM
#define NETWORK_RAT_TYPE_LTE 4     ///<  LTE

/* The maximum number for neighbor cell. */
#define NETWORK_NEIGHBOR_CELL_NUM_MAX 6

/* The operator name format */
#define OPERATOR_LONG_FORMAT_ALPHANUMERIC           0   ///<  long format alphanumeric
#define OPERATOR_SHORT_FORMAT_ALPHANUMERIC          1   ///<  short format alphanumeric
#define OPERATOR_NUMERIC                            2   ///<  numeric
#define OPERATOR_LONG_FORMAT_ALPHANUMERIC_MAX_LEN   17  ///<  long format alphanumeric string Maximum length
#define OPERATOR_SHORT_FORMAT_ALPHANUMERIC_MAX_LEN  9   ///<  short format alphanumeric string Maximum length
#define OPERATOR_NUMERIC_MAX_LEN                    6   ///<  numeric string Maximum length

typedef struct _NwRegInfo
{
    u8 nSim;
    u8 nCid;
} ST_NwRegInfo;

typedef enum
{
    NW_STATE_NO_REGISTERED           = 0,
    NW_STATE_GSM_REGISTERED,
    NW_STATE_LTE_REGISTERED,
    NW_STATE_GSM_ACTIVED,
    NW_STATE_LTE_ACTIVED,
}Enum_NwState;

typedef struct _NwConfig
{
    u8 nIpType;
    char apnName[NETWORK_APN_LEN_MAX];
    u8 nApnSize;
    char apnUserId[NETWORK_APN_USER_LEN_MAX];
    u8 nApnUserIdSize;
    char apnPasswd[NETWORK_APN_PWD_LEN_MAX];
    u8 nApnPasswdSize;
    u8 authType; //PAP or CHAP
} ST_NwConfig;

typedef struct _NwGSMCurrCellInfo
{
    u8 mcc[3]; //Mobile Country Code
    u8 mnc[3]; //Mobile Network Code
    u16 lac;   //Location Area Code
    u32 cellID; //Cell ID
    u8 bsic; //Base Station Identification Code
    u8 rxLev; //RX level value for base station selection in dB
    u8 rssi; //received signal strength indication
    u8 bandinfo; //current band used, 9--GSM900  18--DCS1800  19--PCS1900
} ST_NwGSMCurrCellInfo;

typedef struct _NwLTECurrCellInfo
{
    u8 mcc[3]; //Mobile Country Code
    u8 mnc[3]; //Mobile Network Code
    u8 plmn[3]; //PLMN code
    u32 cellID; //Cell ID
    s16 rsrp; //Reference Signal Received Power
    s16 rsrq; //Reference Signal Received Quality
    u16 tac; //Tracking Area Code
    u16 pcid; //Physical Cell ID
    u8 rssi; //received signal strength indication
    u16 bandinfo; //current band used, band1-band256
    s16 sinr; //synchronization signal based signal to noise and interference ratio
    u16 netMode; //0--tdd  1--fdd
    s16 Srxlev; //Cell selection RX level value
} ST_NwLTECurrCellInfo;

typedef struct _NwGSMNeighborCellInfo
{
    u8 mcc[3]; //Mobile Country Code
    u8 mnc[3]; //Mobile Network Code
    u16 lac;   //Location Area Code
    u32 cellID; //Cell ID
    u8 bsic; //Base Station Identification Code
    u8 rxLev; //RX level value for base station selection in dB
    u8 rssi; //received signal strength indication
} ST_NwGSMNeighborCellInfo;

typedef struct _NwLTENeighborCellInfo
{
    u8 mcc[3]; //Mobile Country Code
    u8 mnc[3]; //Mobile Network Code
    u8 plmn[3]; //PLMN code
    u32 cellID; //Cell ID
    s16 rsrp; //Reference Signal Received Power
    s16 rsrq; //Reference Signal Received Quality
    u16 tac; //Tracking Area Code
    u16 pcid; //Physical Cell ID
} ST_NwLTENeighborCellInfo;

typedef struct _NwCurrCellInfo
{
    union
    {
        ST_NwGSMCurrCellInfo gsm;
        ST_NwLTECurrCellInfo lte;
    } u_CellInfo;
    u8 netType; //NETWORK_RAT_TYPE_GSM or NETWORK_RAT_TYPE_LTE
} ST_NwCurrCellInfo;

typedef struct _NwNeighborCellInfo
{
    u8 cellNum;
    union
    {
        ST_NwGSMNeighborCellInfo gsm[NETWORK_NEIGHBOR_CELL_NUM_MAX];
        ST_NwLTENeighborCellInfo lte[NETWORK_NEIGHBOR_CELL_NUM_MAX];
    } u_CellInfo;
    u8 netType; //NETWORK_RAT_TYPE_GSM or NETWORK_RAT_TYPE_LTE
} ST_NwNeighborCellInfo;

typedef struct _NwUeExtStatusInfo
{
    u16         rrcStatus_plmn_mcc;
    u16         rrcStatus_plmn_mncWithAddInfo;

    u16         rrcStatus_tac;
    u32         rrcStatus_cellId;

    //value in dBm, real RSRP = this value / 100, example: -10.5 dbm, sRsrp = -10.5*100 = -1050
    //value range: -15600 ~ -4400
    s16         phyStatus_sRsrp;
} ST_NwUeExtStatusInfo;

typedef void(*MG_NET_Callback)(s32 eventId, ST_NwRegInfo *info, void *param);

/******************************************************************************
* Function:     MG_NW_Register
*
* Description:
*               This function registers the related callback functions.
*
* Parameters:
*               callback:
*                       [IN] the callback function.
*               param:
*                       [IN] the callback function parameter.
* Return:
*               MG_RET_OK, this function succeeds.
*               Other values, error code.
******************************************************************************/
s32 MG_NW_Register(MG_NET_Callback callback, void *param);

/******************************************************************************
* Function:     MG_NW_Config
*
* Description:
*               This function configures net parameters, including
*               APN name, user name, password and authentication type
*               for the specified PDP context.
*
* Parameters:
*               simId:
*                       [IN] the sim card id.
*               cid:
*                       [IN] the PDP context id.
*               cfg:
*                       [IN] PDP configuration structure.
* Return:
*               MG_RET_OK, this function succeeds.
*               Other values, error code.
******************************************************************************/
s32 MG_NW_Config(u8 simId, u8 cid, ST_NwConfig *cfg);

/******************************************************************************
* Function:     MG_NW_GetNetWorkState
*
* Description:
*               This function gets net status.
*
* Parameters:
*               simId:
*                       [IN] the sim card id.
*               cid:
*                       [IN] the PDP context id.
*               stat:
*                       [OUT] net status.
* Return:
*               MG_RET_OK, this function succeeds.
*               Other values, error code.
******************************************************************************/
s32 MG_NW_GetNetWorkState(u8 simId, u8 cid, Enum_NwState *stat);

/******************************************************************************
* Function:     MG_NW_Activate
*
* Description:
*               This function activates the specified PDP context.
*
* Parameters:
*               simId:
*                       [IN] the sim card id.
*               cid:
*                       [IN] the PDP context id.
*               timeout:
*                       [IN] Activate request timeout.
* Return:
*               MG_RET_OK, this function succeeds.
*               Other values, error code.
******************************************************************************/
s32 MG_NW_Activate(u8 simId, u8 cid, u32 timeout);

/******************************************************************************
* Function:     MG_NW_Deactivate
*
* Description:
*               This function deactivates the specified PDP context.
*
* Parameters:
*               simId:
*                       [IN] the sim card id.
*               cid:
*                       [IN] the PDP context id.
*               timeout:
*                       [IN] Activate request timeout.
* Return:
*               MG_RET_OK, this function succeeds.
*               Other values, error code.
******************************************************************************/
s32 MG_NW_Deactivate(u8 simId, u8 cid, u32 timeout);

/******************************************************************************
* Function:     MG_NW_GetLocalIPAddress
*
* Description:
*               This function gets the local IP address.
*
* Parameters:
*               simId:
*                       [IN] the sim card id.
*               cid:
*                       [IN] the PDP context id.
*               ipAddr:
*                       [OUT] IP address.
*               ipType:
*                       [IN] IP type.
* Return:
*               MG_RET_OK, this function succeeds.
*               Other values, error code.
******************************************************************************/
s32 MG_NW_GetLocalIPAddress(u8 simId, u8 cid, ST_ipAddr *ipAddr, Enum_ipType ipType);

/******************************************************************************
* Function:     MG_NW_GetDnsIpAddress
*
* Description:
*               This function retrieves the DNS server's IP address from network.
*
* Parameters:
*               simId:
*                       [IN] the sim card id.
*               cid:
*                       [IN] the PDP context id.
*               ipAddr:
*                       [OUT] IP address.
*               ipType:
*                       [IN] IP type.
*               dnsIpType:
*                       [IN] DNS type. such as
*                            NETWORK_PDP_TYPE_PRIMARY_DNS_IP
*                            NETWORK_PDP_TYPE_SECONDARY_DNS_IP.
* Return:
*               MG_RET_OK, this function succeeds.
*               Other values, error code.
******************************************************************************/
s32 MG_NW_GetDnsIpAddress(u8 simId, u8 cid, ST_ipAddr *ipAddr, Enum_ipType ipType, s32 dnsIpType);

/******************************************************************************
* Function:     MG_NW_FlightModeEnter
*
* Description:
*               This function enters flight mode.
*
* Parameters:
*               simId:
*                       [IN] the sim card id.
*               timeout:
*                       [IN] request timeout.
* Return:
*               MG_RET_OK, this function succeeds.
*               Other values, error code.
******************************************************************************/
s32 MG_NW_FlightModeEnter(u8 simId, u32 timeout);

/******************************************************************************
* Function:     MG_NW_FlightModeExit
*
* Description:
*               This function exits flight mode.
*
* Parameters:
*               simId:
*                       [IN] the sim card id.
*               timeout:
*                       [IN] request timeout.
* Return:
*               MG_RET_OK, this function succeeds.
*               Other values, error code.
******************************************************************************/
s32 MG_NW_FlightModeExit(u8 simId, u32 timeout);

/******************************************************************************
* Function:     MG_NW_GetFlightModeState
*
* Description:
*               This function gets flight mode status.
*
* Parameters:
*               simId:
*                       [IN] the sim card id.
*               state:
*                       [OUT] point to the flight mode status.
* Return:
*               MG_RET_OK, this function succeeds.
*               Other values, error code.
******************************************************************************/
s32 MG_NW_GetFlightModeState(u8 simId, u8 *state);

/******************************************************************************
* Function:     MG_NW_GetSignalQuality
*
* Description:
*               This function gets signal quality.
*
* Parameters:
*               simId:
*                       [IN] the sim card id.
*               SignalLevel:
*                       [OUT] point to the signal quality level.
* Return:
*               MG_RET_OK, this function succeeds.
*               Other values, error code.
******************************************************************************/
s32 MG_NW_GetSignalQuality(u8 simId, u8 *SignalLevel);

/******************************************************************************
* Function:     MG_NW_GetCurrentCellInfo
*
* Description:
*               This function gets current cell info.
*
* Parameters:
*               simId:
*                       [IN] the sim card id.
*               curCellInfo:
*                       [OUT] point to the current cell info.
*               timeout:
*                       [IN] request timeout.
* Return:
*               MG_RET_OK, this function succeeds.
*               Other values, error code.
******************************************************************************/
s32 MG_NW_GetCurrentCellInfo(u8 simId, ST_NwCurrCellInfo *curCellInfo, u32 timeout);

/******************************************************************************
* Function:     MG_NW_GetNeighborCellInfo
*
* Description:
*               This function gets neighbor cell info.
*
* Parameters:
*               simId:
*                       [IN] the sim card id.
*               neiCellInfo:
*                       [OUT] point to the neighbor cell info.
*               timeout:
*                       [IN] request timeout.
* Return:
*               MG_RET_OK, this function succeeds.
*               Other values, error code.
******************************************************************************/
s32 MG_NW_GetNeighborCellInfo(u8 simId, ST_NwNeighborCellInfo *neiCellInfo, u32 timeout);



s32 MG_NW_GetUeStatusInfo(u8 simId, ST_NwUeExtStatusInfo *statuInfo);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif // End-of _MG_NETWORK_H_