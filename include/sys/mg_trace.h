/**
  ******************************************************************************
  * @file    mg_trace.h
  * @author  MEIG Development Team
  * @version V1.0.0
  * @date    28-Mar-2022
  * @brief   This file provides all the trace function APIs.
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

#ifndef _MG_TRACE_H_
#define _MG_TRACE_H_
#include "mg_type.h"

#ifdef __cplusplus
extern "C" {
#endif
typedef enum
{
	/** only used in control, for not to output trace */
	MG_TRACE_LOG_LEVEL_NEVER,
	/** error */
	MG_TRACE_LOG_LEVEL_ERROR,
	/** warning */
	MG_TRACE_LOG_LEVEL_WARN,
	/** information */
	MG_TRACE_LOG_LEVEL_INFO,
	/** debug */
	MG_TRACE_LOG_LEVEL_DEBUG,
	/** verbose */
	MG_TRACE_LOG_LEVEL_VERBOSE,
} Enum_TRACE_LogLevel;

#define MG_TRACE_LOG_DEFAULT_LEVEL    MG_TRACE_LOG_LEVEL_INFO

/**
 * macro for trace tag
 */
#define MAKE_LOG_TAG(a, b, c, d)     (u32)((u32)(a) | ((u32)(b) << 7) | ((u32)(c) << 14) | ((u32)(d) << 21))

#define MG_TRACE_LOGE(tag, fmt, ...)  MG_TRACE_PrintfEX(MG_TRACE_LOG_LEVEL_ERROR, tag, fmt, ##__VA_ARGS__)
#define MG_TRACE_LOGW(tag, fmt, ...)  MG_TRACE_PrintfEX(MG_TRACE_LOG_LEVEL_WARN, tag, fmt, ##__VA_ARGS__)
#define MG_TRACE_LOGI(tag, fmt, ...)  MG_TRACE_PrintfEX(MG_TRACE_LOG_LEVEL_INFO, tag, fmt, ##__VA_ARGS__)
#define MG_TRACE_LOGD(tag, fmt, ...)  MG_TRACE_PrintfEX(MG_TRACE_LOG_LEVEL_DEBUG, tag, fmt, ##__VA_ARGS__)
#define MG_TRACE_LOGV(tag, fmt, ...)  MG_TRACE_PrintfEX(MG_TRACE_LOG_LEVEL_VERBOSE, tag, fmt, ##__VA_ARGS__)

/******************************************************************************
* Function:     MG_TRACE_Printf
*
* Description: print log info
*
* Parameters:
*               tag:
*                       [IN] log tag
*               fmt:
*                       [IN] log format string
*
* Return:
*               void.
*
******************************************************************************/
void MG_TRACE_Printf(u32 tag, const char *fmt, ...);

/******************************************************************************
* Function:     MG_TRACE_PrintfEX
*
* Description: print log info
*
* Parameters:
*               level:
*                       [IN] log level
*               tag:
*                       [IN] log tag
*               fmt:
*                       [IN] log format string
*
* Return:
*               void.
*
******************************************************************************/
void MG_TRACE_PrintfEX(Enum_TRACE_LogLevel level, u32 tag, const char *fmt, ...);

/******************************************************************************
* Function:     MG_TRACE_SetLogLevel
*
* Description: set log level
*
* Parameters:
*               level:
*                       [IN] log level
*
* Return:
*               void.
*
******************************************************************************/
void MG_TRACE_SetLogLevel(Enum_TRACE_LogLevel level);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif // End-of _MG_TRACE_H_