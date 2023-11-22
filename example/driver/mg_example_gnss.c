/**
  ******************************************************************************
  * @file    mg_example_gnss.c
  * @author  MEIG Development Team
  * @version V1.0.1
  * @date    07-Jun-2023
  * @brief   This file provides a simple example for GNSS application.
  ******************************************************************************
  * @attention
  *
  * THE SOFTWARE IS PROTECTED BY COPYRIGHT AND THE INFORMATION CONTAINED HEREIN
  * IS CONFIDENTIAL. THE SOFTWARE MAY NOT BE COPIED AND THE INFORMATION CONTAINED
  * HEREIN MAY NOT BE USED OR DISCLOSED EXCEPT WITH THE WRITTEN PERMISSION OF
  * MEIG SMART TECHNOLOGY CO., LTD.
  *
  * <h2><center>&copy; COPYRIGHT 2023 MEIG SMART</center></h2>
  ******************************************************************************
  */

#include "mg_trace.h"
#include <stdio.h>
#include <string.h>
#include "mg_gnss.h"
#include "mg_common.h"
#include "mg_os.h"
#include DEBUG_LOG_HEADER_FILE

#define LOG_TAG MAKE_LOG_TAG('G', 'N', 'S', 'S')

#define DEBUG_ENABLE 1
#if DEBUG_ENABLE
#define APP_DEBUG(FORMAT, ...)  MG_TRACE_Printf(LOG_TAG, FORMAT, ##__VA_ARGS__)
#else
#define APP_DEBUG(FORMAT, ...)
#endif

u8 nmea_buffer[1024];
u8 nmea_sentence[1024];
ST_nmeaINFO nmeaINFO;

void prvThreadEntry(void *p)
{
    MG_GNSS_Open();

    MG_OS_ThreadSleep(1000);
    MG_GNSS_DebugConfig(GNSS_DBG_USB_NMEA);

    while (1)
    {
        if (MG_GNSS_GetStatus() == TRUE)
        {
            MG_GNSS_GetNmeaInfo(nmea_buffer, sizeof(nmea_buffer));

            if (TRUE == MG_GNSS_NmeaParse(nmea_buffer, &nmeaINFO))
            {
                APP_DEBUG("FIX STATUS:%d, UTC:%d-%02d-%02d %02d:%02d:%02d\r\n",
                        nmeaINFO.sig, nmeaINFO.utc.year + 1900, nmeaINFO.utc.mon, nmeaINFO.utc.day,
                        nmeaINFO.utc.hour, nmeaINFO.utc.min, nmeaINFO.utc.sec);

                memset(nmea_sentence, 0x00, sizeof(nmea_sentence));
                MG_GNSS_NmeaSentenceFilter(nmea_buffer, nmea_sentence, NMEA_GGA);
                APP_DEBUG("GGA:%s", nmea_sentence);

                memset(nmea_sentence, 0x00, sizeof(nmea_sentence));
                MG_GNSS_NmeaSentenceFilter(nmea_buffer, nmea_sentence, NMEA_RMC);
                APP_DEBUG("RMC:%s", nmea_sentence);

                memset(nmea_sentence, 0x00, sizeof(nmea_sentence));
                MG_GNSS_NmeaSentenceFilter(nmea_buffer, nmea_sentence, NMEA_GSV);
                APP_DEBUG("GSV:%s", nmea_sentence);

                memset(nmea_sentence, 0x00, sizeof(nmea_sentence));
                MG_GNSS_NmeaSentenceFilter(nmea_buffer, nmea_sentence, NMEA_GSA);
                APP_DEBUG("GSA:%s", nmea_sentence);

                memset(nmea_sentence, 0x00, sizeof(nmea_sentence));
                MG_GNSS_NmeaSentenceFilter(nmea_buffer, nmea_sentence, NMEA_VTG);
                APP_DEBUG("VTG:%s", nmea_sentence);

                if (nmeaINFO.sig)
                    goto EXIT;
            }
        }
        MG_OS_ThreadSleep(1000 * 1);
    }

EXIT:
    MG_GNSS_Close();
    MG_GNSS_DebugConfig(GNSS_DBG_NONE);
    MG_OS_ThreadExit();
}

void proc_main_task(void)
{
    MG_OS_ThreadCreate("mg_gnss", prvThreadEntry, NULL, Thread_PRIORITY_NORMAL, 1024 * 8, 4);
}
/******************* (C) COPYRIGHT 2023 MEIG SMART*****END OF FILE*************/