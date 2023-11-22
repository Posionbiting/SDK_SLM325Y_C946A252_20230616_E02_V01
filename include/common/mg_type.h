/**
  ******************************************************************************
  * @file    mg_type.h
  * @author  MEIG Development Team
  * @version V1.0.0
  * @date    28-Mar-2022
  * @brief   This file is type definition.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MG_TYPE_H
#define __MG_TYPE_H

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup DATA_TYPE_OpenCPU
  * @{
  */

/** @defgroup DATA_TYPE_Exported_Types
  * @{
  */

#ifndef FALSE
#define FALSE    0
#endif

#ifndef TRUE
#define TRUE     1
#endif

#ifndef NULL
#define NULL    ((void *) 0)
#endif

/**
  * @brief  Data type definitions
  */
typedef unsigned char       BOOL;
typedef unsigned char       u8;
typedef signed   char       s8;
typedef unsigned short      u16;
typedef          short      s16;
typedef unsigned int        u32;
typedef          int        s32;
typedef unsigned long long  u64;
typedef          long long  s64;

#ifdef __cplusplus
}
#endif

/**
  * @}
  */

/**
  * @}
  */

#endif /* __MG_TYPE_H */
/******************* (C) COPYRIGHT 2022 MEIG SMART*****END OF FILE*************/
