/**
  ******************************************************************************
  * @file    mg_openAT_cmd_table.h
  * @author  MEIG Development Team
  * @version V1.0.0
  * @date    28-Mar-2022
  * @brief   This file provides all openAT cmd table APIs.
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

#ifndef _MG_OPENAT_TABLE_H_
#define _MG_OPENAT_TABLE_H_

#include "at_util.h"

/*
 * OPEN AT CMD table
*/
AtCmdPreDefInfoC *atcGetOpenAtCmdSeqPointer(void);

/*
 * OPEN AT CMD number
*/
UINT32 atcGetOpenAtCmdSeqNumb(void);

#endif
