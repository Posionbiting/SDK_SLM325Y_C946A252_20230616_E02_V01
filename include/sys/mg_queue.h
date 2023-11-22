/**
  ******************************************************************************
  * @file    mg_queue.h
  * @author  MEIG Development Team
  * @version V1.0.0
  * @date    28-Mar-2022
  * @brief   This file provides all the queue function APIs.
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

#ifndef _MG_QUEUE_H_
#define _MG_QUEUE_H_
#include "mg_type.h"


/****************************************************************************
 * opaque data structure for queue
 ***************************************************************************/
typedef struct _Queue ST_queue;

/******************************************************************************
* Function:     MG_QUEUE_Create
*
* Description:
*               Create queue.
*
* Parameters:
*               count:
*                       [IN] maximum message count can be hold in queue.
*               size:
*                       [IN] size of each message in bytes.
* Return:
*               If no error occurs, MG_QUEUE_Create returns the queue
*               pointer. Otherwise, returns NULL.
******************************************************************************/
ST_queue* MG_QUEUE_Create(u32 count, u32 size);

/******************************************************************************
* Function:     MG_QUEUE_Delete
*
* Description:
*               Delete queue.
*
* Parameters:
*               queue:
*                       [IN] the queue pointer.
* Return:
*               MG_RET_OK, this function succeeds.
*               Other values, error code.
******************************************************************************/
s32 MG_QUEUE_Delete(ST_queue* queue);

/******************************************************************************
* Function:     MG_QUEUE_Write
*
* Description:
*               Write data message to queue.
*
* Parameters:
*               queue:
*                       [IN] the queue pointer.
*               data:
*                       [IN] the data message.
*               len:
*                       [IN] the data message length.
* Return:
*               >=0, written data length.
*               Other values, error code.
******************************************************************************/
s32 MG_QUEUE_Write(ST_queue* queue, const u8* data, s32 len);

/******************************************************************************
* Function:     MG_QUEUE_Read
*
* Description:
*               Read data message from queue.
*
* Parameters:
*               queue:
*                       [IN] the queue pointer.
*               data:
*                       [OUT] the data buffer.
*               size:
*                       [IN] the data buffer size.
* Return:
*               >=0, read data length.
*               Other values, error code.
******************************************************************************/
s32 MG_QUEUE_Read(ST_queue* queue, u8* data, s32 size);

/******************************************************************************
* Function:     MG_QUEUE_IsFull
*
* Description:
*               Check whether the queue is full.
*
* Parameters:
*               queue:
*                       [IN] the queue pointer.
* Return:
*               TRUE, queue is full.
*               FALSE, queue is not full.
******************************************************************************/
bool MG_QUEUE_IsFull(ST_queue* queue);

/******************************************************************************
* Function:     MG_QUEUE_IsEmpty
*
* Description:
*               Check whether the queue is empty.
*
* Parameters:
*               queue:
*                       [IN] the queue pointer.
* Return:
*               TRUE, queue is empty.
*               FALSE, queue is not empty.
******************************************************************************/
bool MG_QUEUE_IsEmpty(ST_queue* queue);

#endif // End-of _MG_QUEUE_H_



