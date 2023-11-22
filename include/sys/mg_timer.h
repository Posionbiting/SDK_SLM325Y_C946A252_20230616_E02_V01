/**
  ******************************************************************************
  * @file    mg_timer.h
  * @author  MEIG Development Team
  * @version V1.0.0
  * @date    28-Mar-2022
  * @brief   This file provides all the timer function APIs.
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
#ifndef __MG_TIMER_H__
#define __MG_TIMER_H__

#include "mg_type.h"

/****************************************************************************
 * Def config
 ***************************************************************************/
typedef osiTimer_t ST_Timer;

typedef void (*FU_timterCallback)(void *argument);

/*****************************************************************
* Function:     MG_TIMER_Create
*
* Description:
*               Create a timer with specified callback and callback parameter,
*               the callback will be executed in timer service thread.
*
* Parameters:
*              timerCb:
*                       [IN] callback to be executed after timer expire.
*              arg:
*                       [IN] callback parameter.
*
* Return:
*               the created timer instance;
*               NULL at out of memory, or invalid parameter.
*****************************************************************/
ST_Timer *MG_TIMER_Create(FU_timterCallback timerCb, void *arg);

/*****************************************************************
* Function:     MG_TIMER_IrqCreate
*
* Description:
*               Create a timer with specified callback and callback parameter,
*               the callback will be executed in timer ISR. So, the callback
*               should follow ISR programming guide.
*
* Parameters:
*              timerCb:
*                       [IN] callback to be executed after timer expire..
*              arg:
*                       [IN] callback parameter.
*
* Return:
*               the created timer instance;
*               NULL at out of memory, or invalid parameter.
*****************************************************************/
ST_Timer *MG_TIMER_IrqCreate(FU_timterCallback timerCb, void *arg);

/*****************************************************************
* Function:     MG_TIMER_Start
*
* Description:
*               Start up the specified timer.
*
* Parameters:
*               timer:
*                       [IN] the timer to be started.
*               ms:
*                       [IN] Set the interval of the timer, unit: ms.
*               autoRepeat:
*                       [IN] TRUE for periodic, FALSE for one shot.
* Return:
*               MG_RET_OK, this function succeeds.
*               Other values, error code.
*****************************************************************/
s32 MG_TIMER_Start(ST_Timer *timer, u32 ms, bool autoRepeat);

/*****************************************************************
* Function:     MG_TIMER_StartLowPower
*
* Description:
*               the function is a power optimization version of MG_TIMER_Start.
*
* Parameters:
*               timer:
*                       [IN] the timer to be started.
*               ms:
*                       [IN] Set the interval of the timer, unit: ms.
*               autoRepeat:
*                       [IN] TRUE for periodic, FALSE for one shot.
* Return:
*               MG_RET_OK, this function succeeds.
*               Other values, error code.
*****************************************************************/
s32 MG_TIMER_StartLowPower(ST_Timer *timer, u32 ms, bool autoRepeat);

/*****************************************************************
* Function:     MG_TIMER_Stop
*
* Description:
*               Stop the specified timer.
*
* Parameters:
*               timer:
*                       [IN] the timer to be stopped.
* Return:
*               MG_RET_OK, this function succeeds.
*               Other values, error code.
*****************************************************************/
s32 MG_TIMER_Stop(ST_Timer *timer);

/*****************************************************************
* Function:     MG_TIMER_Delete
*
* Description:
*               delete the specified timer.
*
* Parameters:
*               timer:
*                       [IN] the timer to be deleted.
* Return:
*               MG_RET_OK, this function succeeds.
*               Other values, error code.
*****************************************************************/
s32 MG_TIMER_Delete(ST_Timer *timer);

#endif /* __MG_TIMER_H__ */
