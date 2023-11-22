
/******************************************************************************

*(C) Copyright 2018 EIGENCOMM International Ltd.

* All Rights Reserved

******************************************************************************
*  Filename: ccio_monitor.h
*
*  Description:
*
*  History: 2021/1/19 created by xuwang
*
*  Notes:
*
******************************************************************************/
#ifndef CCIO_MONITOR_H
#define CCIO_MONITOR_H

/*----------------------------------------------------------------------------*
 *                    INCLUDES                                                *
 *----------------------------------------------------------------------------*/


#ifdef __cplusplus
extern "C" {
#endif


/*----------------------------------------------------------------------------*
 *                    MACROS                                                  *
 *----------------------------------------------------------------------------*/
#define CCIO_CHAN_MSGQ_DELAY_NONE       0
#define CCIO_CHAN_MSGQ_DELAT_TIME       500


/*----------------------------------------------------------------------------*
 *                   DATA TYPE DEFINITION                                     *
 *----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*
 *                    GLOBAL FUNCTIONS DECLEARATION                           *
 *----------------------------------------------------------------------------*/
void  ccioInitMonitor(uint8_t bmTaskCreateFlag);
void  ccioDeinitMonitor(uint8_t bmTaskKillFlag);

void  ccioReportUlfcMemEvent(OsaUlfcMemEvtFlags_e flags, OsaUlfcMemEvtArgs_t *args);
void  ccioReportDlfcMemEvent(OsaDlfcMemEvtFlags_e flags, OsaDlfcMemEvtArgs_t *args);

/**
 * @brief ccioSetRxFlowCtrlState(uint8_t isFlowCtrl)
 * @details set Rx channel flow control state.
 *
 * @param hasFlowCtrl has flow controlled or not
 * @return void
 */
int32_t ccioSetRxFlowCtrlState(uint8_t isFlowCtrl);
/**
 * @brief ccioGetRxFlowCtrlState(void)
 * @details get Rx channel flow control state.
 *
 * @return isFlowCtrl has flow controlled or not
 */
int32_t ccioGetRxFlowCtrlState(void);

int32_t ccioSetUpChannels(uint8_t chanType, void *chent);
int32_t ccioPullDownChannels(uint8_t chanType, void *chent);

int32_t ccioHandleDevStatus(uint32_t flags, void *args);
int32_t ccioHandleEntStatus(uint32_t flags, void *args);

uint32_t ccioQueryChanPendTccm(void);

uint8_t  ccioIsUldpPppSchemAvlb(void);

CcioEntity_t* ccioGetEutraServEnt(void);
CcioEntity_t* ccioGetEtherServEnt(void);
CcioEntity_t* ccioGetPppServEnt(void);

int32_t ccioDisablePmuSleep1(uint32_t lanMedia);
int32_t ccioEnablePmuSleep1(uint32_t lanMedia);

#ifdef __cplusplus
}
#endif
#endif

