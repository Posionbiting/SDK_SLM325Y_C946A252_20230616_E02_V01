/**
  ******************************************************************************
  * @file    mg_common.h
  * @author  MEIG Development Team
  * @version V1.0.0
  * @date    28-Mar-2022
  * @brief   This file is common header file.
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

#ifndef __MG_COMMON_H__
#define __MG_COMMON_H__

#include "mg_type.h"
#define _COMPILE_WITH_C_LIB_ 1
#if _COMPILE_WITH_C_LIB_
#include <stdbool.h>
#else
typedef BOOL bool;
#endif

#include DEBUG_LOG_HEADER_FILE

#include "mg_error.h"
#include "mg_opt_net.h"

typedef void osiThread_t;

typedef void osiMutex_t;

typedef void osiSemaphore_t;

typedef void osiTimer_t;

typedef void (*osiThreadEntry_t)(void *argument);

#include "mg_trace.h"
extern Enum_TRACE_LogLevel g_traceLogLevel;
#define MCI_TRACE_LOG_LEVEL g_traceLogLevel

#define MCI_TRACE_PrintfEX(level, tag, fmt, ...)                                                                       \
    do                                                                                                                 \
    {                                                                                                                  \
        if (MCI_TRACE_LOG_LEVEL >= level)                                                                              \
        {                                                                                                              \
            ECPLAT_PRINTF(UNILOG_MCI, tag, P_INFO, fmt, ##__VA_ARGS__);                                                \
        }                                                                                                              \
    } while (0)

#define MCI_TRACE_LOGE(tag, fmt, ...)  MCI_TRACE_PrintfEX(MG_TRACE_LOG_LEVEL_ERROR, tag, fmt, ##__VA_ARGS__)
#define MCI_TRACE_LOGW(tag, fmt, ...)  MCI_TRACE_PrintfEX(MG_TRACE_LOG_LEVEL_WARN, tag, fmt, ##__VA_ARGS__)
#define MCI_TRACE_LOGI(tag, fmt, ...)  MCI_TRACE_PrintfEX(MG_TRACE_LOG_LEVEL_INFO, tag, fmt, ##__VA_ARGS__)
#define MCI_TRACE_LOGD(tag, fmt, ...)  MCI_TRACE_PrintfEX(MG_TRACE_LOG_LEVEL_DEBUG, tag, fmt, ##__VA_ARGS__)
#define MCI_TRACE_LOGV(tag, fmt, ...)  MCI_TRACE_PrintfEX(MG_TRACE_LOG_LEVEL_VERBOSE, tag, fmt, ##__VA_ARGS__)

#endif  // End-of __MG_COMMON_H__
