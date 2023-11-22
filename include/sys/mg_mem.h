/**
  ******************************************************************************
  * @file    mg_mem.h
  * @author  MEIG Development Team
  * @version V1.0.0
  * @date    28-Mar-2022
  * @brief   This file provides all the mem function APIs.
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

#ifndef _MG_MEM_H_
#define _MG_MEM_H_

/*****************************************************************************
 *Include Files
 ****************************************************************************/
#include "mg_type.h"

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************
 *Definitons and Declarations
 ****************************************************************************/

/*****************************************************************************
 *Function Declarations
 ****************************************************************************/

/*****************************************************************************
* Function:     MG_MEM_Calloc
*
* Description:
*   1. function allocates memory for an array of <count> elements of <size> 
*   bytes each and returns a pointer to the allocated memory.
*   2. The memory is set to zero.
*
* Parameters:
*   1. count: the number of elements in the array.
*   2. size: size of an element.
*
* Return:
*   If <count> or <size> is 0, then MG_MEM_Calloc() returns either NULL, or a
*   unique pointer value that can later be successfully passed to 
*   MG_MEM_Free().
*
*****************************************************************************/
void *MG_MEM_Calloc(u32 count, u32 size);

/*****************************************************************************
* Function:     MG_MEM_Malloc
*
* Description:
*   function allocates size bytes memory.
*
* Parameters:
*   size: size of memory to be allocated.
*
* Return:
*   1. If <size> is 0, then MG_MEM_Malloc() returns either NULL, or a unique
*   pointer value that can later be successfully passed to MG_MEM_Free().
*   2. The memory is not initialized.
*
*****************************************************************************/
void *MG_MEM_Malloc(u32 size);

/*****************************************************************************
* Function:     MG_MEM_Free
*
* Description:
*   1. function frees the memory space pointed to by ptr, which must have 
*   been returned by a previous call to MG_MEM_Malloc() or MG_MEM_Calloc().
*   2. Otherwise, or if MG_MEM_Free(ptr) has already been called before,
*   undefined behavior occurs.
*   3. If ptr is NULL, no operation is performed.
*
* Parameters:
*   ptr: pointer to the memory to be free.
*
* Return:
*
*****************************************************************************/
void MG_MEM_Free(void *ptr);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif // End-of _MG_MEM_H_