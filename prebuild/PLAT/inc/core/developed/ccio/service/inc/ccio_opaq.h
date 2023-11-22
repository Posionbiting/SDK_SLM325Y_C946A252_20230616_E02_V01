/******************************************************************************

*(C) Copyright 2018 EIGENCOMM International Ltd.

* All Rights Reserved

******************************************************************************
*  Filename: ccio_opaq.h
*
*  Description:
*
*  History: 2022/3/15 created by xuwang
*
*  Notes:
*
******************************************************************************/
#ifndef CCIO_OPAQ_H
#define CCIO_OPAQ_H

/*----------------------------------------------------------------------------*
 *                    INCLUDES                                                *
 *----------------------------------------------------------------------------*/
#include "ccio_misc.h"


#ifdef __cplusplus
extern "C" {
#endif


/*----------------------------------------------------------------------------*
 *                    MACROS                                                  *
 *----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*
 *                   DATA TYPE DEFINITION                                     *
 *----------------------------------------------------------------------------*/
//<!--[ODM][USRL][TaskID 33219]wbh@2023-03-07 Declare an application callback function type.
typedef void (*opaqCallback_t)(UlPduBlock_t *ulpdu);
//end-->

/*----------------------------------------------------------------------------*
 *                    GLOBAL FUNCTIONS DECLEARATION                           *
 *----------------------------------------------------------------------------*/

/**
  \fn     int32_t opaqDataInput(UlPduBlock_t *ulpdu, void *extras)
  \brief  to further handle opaq data received from serial device
  \return
  \note   invoked by RxTask automatically once opaq data is received
*/
int32_t opaqDataInput(UlPduBlock_t *ulpdu, void *extras);

/**
  \fn     int32_t opaqDataOutput(uint8_t chanNo, DlPduBlock_t *dlpdu, void *extras)
  \brief  to send opaq data out of serial device
  \return
  \note   invoked by customer's AppTask for outputting opaq data
*/
int32_t opaqDataOutput(uint8_t chanNo, DlPduBlock_t *dlpdu, void *extras);

//<!--[ODM][USRL][TaskID 33219]wbh@2023-03-07 Add an API to register application callback.
void opaqCallbackRegister(opaqCallback_t cb);
//end-->

#ifdef __cplusplus
}
#endif
#endif

