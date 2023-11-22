/**
  ******************************************************************************
  * @file    mg_openAT_engine.h
  * @author  MEIG Development Team
  * @version V1.0.0
  * @date    28-Mar-2022
  * @brief   This file provides all openAT engine APIs.
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

#ifndef _MG_OPENAT_ENGINE_H_
#define _MG_OPENAT_ENGINE_H_

#include "at_util.h"

#include "mg_common.h"
#include "mg_type.h"
#include "mg_os.h"

#define EV_OPEN_AT_EV_BASE       0x00110000
#define EV_OPEN_AT_EV_CALLBACK   (EV_OPEN_AT_EV_BASE + 1)

typedef void (*openAtCallback_t)(void *argument);

typedef s32 (*openAtCmdBypassCB_t)(void *ctx, u8 chanId, const void *data, u32 size);

/**
 * create openAT engine
 */
void mg_openAt_EngineCreate(void);

/**
 * get openAT engine handle
 */
ST_Thread *mg_openAT_getOpenAtEngine(void);

/**
 * send EV_OPEN_AT_EV_CALLBACK event to openAT engine
 */
BOOL mg_openAT_EngineCallback(openAtCallback_t cb, void *cb_ctx);

/**
 * set command mode engine to command line mode
 */
CmsRetId mg_openAT_SetLineMode(UINT8 chanId);

/**
 * set command mode engine to bypass mode
 */
CmsRetId mg_openAT_SetBypassMode(UINT8 chanId, openAtCmdBypassCB_t cb, void *cb_ctx);

#endif