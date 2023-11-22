/**
  ******************************************************************************
  * @file    mg_openAT_param.h
  * @author  MEIG Development Team
  * @version V1.0.0
  * @date    28-Mar-2022
  * @brief   This file provides all openAT parameter APIs.
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

#ifndef _MG_OPENAT_PARAM_H_
#define _MG_OPENAT_PARAM_H_

#include "at_util.h"
#include "mg_type.h"

/*
input parameters, could be "numeric" or "string"
AT_PARAM_TYPE_INT32       numeric
AT_PARAM_TYPE_STRING      string
*/
#define AT_PARAM_TYPE_INT32            AT_DEC_VAL
#define AT_PARAM_TYPE_STRING           AT_STR_VAL

/* Attributes
 AT_PARAM_ATTR_MUST           mandatory
 AT_PARAM_ATTR_OPTIONAL       optional
*/
#define AT_PARAM_ATTR_MUST             AT_MUST_VAL
#define AT_PARAM_ATTR_OPTIONAL         AT_OPT_VAL

#define AT_PARAM_DEF(INDEX, AT_PARAM_TYPE, AT_PARAM_ATTR)    AT_PARAM_ATTR_DEF(AT_PARAM_TYPE, AT_PARAM_ATTR)

/*
 * parse uint paramter
*/
s32 mg_openAT_cmdParamUintInRange(AtParamValueCP pAtParaList, u32 index, s32 minValue, s32 maxValue, s32 defaultValue, BOOL *result);

/*
 * parse string paramter
*/
s32 mg_openAT_cmdParamStr(AtParamValueCP pAtParaList, u32 index, u8 *pOutStrBuf, u16 maxOutBufLen, BOOL *result);

#endif
