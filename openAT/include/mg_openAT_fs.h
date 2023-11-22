/**
  ******************************************************************************
  * @file    mg_openAT_fs.h
  * @author  MEIG Development Team
  * @version V1.0.0
  * @date    28-Mar-2022
  * @brief   This file provides all openAT fs APIs.
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

#ifndef _MG_OPENAT_FS_H_
#define _MG_OPENAT_FS_H_

#include "at_util.h"
#include "mg_type.h"

#define OPEN_AT_FS_FILE_NAME_MAX_LEN            64

#define OPEN_AT_FS_SIZE_MAX                     20*1024

CmsRetId openFSDWNFILE(const AtCmdInputContext *pAtCmdReq);
CmsRetId openFSRDFILE(const AtCmdInputContext *pAtCmdReq);
CmsRetId openFSDELFILE(const AtCmdInputContext *pAtCmdReq);
CmsRetId openFSLSTFILE(const AtCmdInputContext *pAtCmdReq);

#endif
