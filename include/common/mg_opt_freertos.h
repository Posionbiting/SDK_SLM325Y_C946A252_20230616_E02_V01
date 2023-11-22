/**
  ******************************************************************************
  * @file    mg_opt_freertos.h
  * @author  MEIG Development Team
  * @version V1.0.0
  * @date    28-Mar-2022
  * @brief   This file provides freertos extend API.
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

#ifndef __MG_OPT_FREERTOS_H__
#define __MG_OPT_FREERTOS_H__

#ifdef __cplusplus
extern "C" {
#endif

#define SDK_THREAD_LOCAL_STORAGE_POINTER_ID 1

extern void MG_OS_vTaskDelete(void *xTaskToSet);
extern void MG_OS_vTaskSetThreadLocalStoragePointer(void *xTaskToSet, long xIndex, void *pvValue);
extern void *MG_OS_pvTaskGetThreadLocalStoragePointer(void *xTaskToSet, long xIndex);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif  // End-of __MG_OPT_FREERTOS_H__
