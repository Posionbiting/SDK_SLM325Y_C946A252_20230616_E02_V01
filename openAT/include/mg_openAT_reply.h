/**
  ******************************************************************************
  * @file    mg_openAT_reply.h
  * @author  MEIG Development Team
  * @version V1.0.0
  * @date    28-Mar-2022
  * @brief   This file provides all openAT reply APIs.
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

#ifndef _MG_OPENAT_REPLY_H_
#define _MG_OPENAT_REPLY_H_

#include "at_util.h"

#define OPEN_AT_OK                   0
#define OPEN_AT_ERROR                701
#define OPEN_AT_ERROR_PARAM          702
#define OPEN_AT_ERROR_NO_MEMORY      703
#define OPEN_AT_ERROR_NOT_SUPPORT    704

/**
 * response OK
 */
CmsRetId mg_openAT_cmdRespOK(UINT32 atHandle);

/**
 * response ERROR
 */
CmsRetId mg_openAT_cmdRespError(UINT32 atHandle, UINT32 errCode);

/**
 * response text and ok
 * 
 * add "\r\n" at start&end of text
 * add "\r\nOK\r\n" after text
 * 
 */
CmsRetId mg_openAT_cmdRespTextAndOK(UINT32 atHandle, const CHAR *text);

/**
 * response URC text
 */
CmsRetId mg_openAT_cmdRespUrcText(UINT32 atHandle, const CHAR *text);

/**
 * response prompt '>'
 */
CmsRetId mg_openAT_cmdRespOutputPrompt(UINT32 atHandle);

/**
 * response line break "\r\n"
 */
CmsRetId mg_openAT_cmdRespOutputLineBreak(UINT32 atHandle);

#endif
