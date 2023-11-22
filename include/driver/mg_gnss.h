/**
  ******************************************************************************
  * @file    mg_gnss.h
  * @author  MEIG Development Team
  * @version V1.0.1
  * @date    07-Jun-2023
  * @brief   This file contains all the functions prototypes for the GNSS system.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MG_GNSS_H
#define __MG_GNSS_H

/* Includes ------------------------------------------------------------------*/
#include "mg_type.h"

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup SLM3xx_OpenCPU_GNSS
  * @{
  */

/** @addtogroup GNSS
  * @brief GNSS driver modules
  * @{
  */

/** @defgroup GNSS_Exported_Constants
  * @{
  */

/* Enable debug function for GNSS */
#define GNSS_CFG_DBG_EN     TRUE

#define NMEA_SIG_BAD        (0)
#define NMEA_SIG_LOW        (1)
#define NMEA_SIG_MID        (2)
#define NMEA_SIG_HIGH       (3)

#define NMEA_FIX_BAD        (1)
#define NMEA_FIX_2D         (2)
#define NMEA_FIX_3D         (3)

#define NMEA_MAXSAT         (12)
#define NMEA_SATINPACK      (4)
#define NMEA_NSATPACKS      (NMEA_MAXSAT / NMEA_SATINPACK)

#define NMEA_DEF_LAT        (5001.2621)
#define NMEA_DEF_LON        (3613.0595)

/**
  * @}
  */

/** @defgroup GNSS_Exported_Types
  * @{
  */

/* Output port of NMEA sentences */
typedef enum
{
    GNSS_DBG_NONE = 0,
    GNSS_DBG_USB_NMEA,
    GNSS_DBG_LOG
} Enum_DebugPort;


typedef enum
{
    NMEA_GGA = 0,
    NMEA_RMC,
    NMEA_GSV,
    NMEA_GSA,
    NMEA_VTG,
    NMEA_GLL,
} Enum_nmeaTYPE;

/**
  * @brief  Date and time data
  *
  * @see nmea_time_now
  */
typedef struct
{
    u32     year;       /**< Years since 1900 */
    u32     mon;        /**< Months since January - [0,11] */
    u32     day;        /**< Day of the month - [1,31] */
    u32     hour;       /**< Hours since midnight - [0,23] */
    u32     min;        /**< Minutes after the hour - [0,59] */
    u32     sec;        /**< Seconds after the minute - [0,59] */
    u32     hsec;       /**< Hundredth part of second - [0,99] */
} ST_nmeaTIME;

/**
  * @brief  Position data in fractional degrees or radians
  */
typedef struct
{
    double lat;         /**< Latitude */
    double lon;         /**< Longitude */
} ST_nmeaPOS;

/**
  * @brief  Information about satellite
  * @see nmeaSATINFO
  * @see nmeaGPGSV
  */
typedef struct
{
    u32     id;         /**< Satellite PRN number */
    u32     in_use;     /**< Used in position fix */
    u32     elv;        /**< Elevation in degrees, 90 maximum */
    u32     azimuth;    /**< Azimuth, degrees from true north, 000 to 359 */
    u32     sig;        /**< Signal, 00-99 dB */
} ST_nmeaSATELLITE;

/**
  * @brief  Information about all satellites in view
  * @see nmeaINFO
  * @see nmeaGPGSV
  */
typedef struct
{
    u32     inuse;      /**< Number of satellites in use (not those in view) */
    u32     inview;     /**< Total number of satellites in view */
    ST_nmeaSATELLITE sat[NMEA_MAXSAT]; /**< Satellites information */
} ST_nmeaSATINFO;

/**
  * @brief  Summary GPS information from all parsed packets, used also for generating NMEA stream
  * @see nmea_parse
  * @see nmea_GPGGA2info,  nmea_...2info
  */
typedef struct
{
    u32         smask;          /**< Mask specifying types of packages from which data have been obtained */

    ST_nmeaTIME utc;            /**< UTC of position */

    u32         sig;            /**< GPS quality indicator (0 = Invalid; 1 = Fix; 2 = Differential, 3 = Sensitive) */
    u32         fix;            /**< Operating mode, used for navigation (1 = Fix not available; 2 = 2D; 3 = 3D) */

    double      PDOP;           /**< Position Dilution Of Precision */
    double      HDOP;           /**< Horizontal Dilution Of Precision */
    double      VDOP;           /**< Vertical Dilution Of Precision */

    double      lat;            /**< Latitude in NDEG - +/-[degree][min].[sec/60] */
    double      lon;            /**< Longitude in NDEG - +/-[degree][min].[sec/60] */
    double      elv;            /**< Antenna altitude above/below mean sea level (geoid) in meters */
    double      speed;          /**< Speed over the ground in kilometers/hour */
    double      direction;      /**< Track angle in degrees True */
    double      declination;    /**< Magnetic variation degrees (Easterly var. subtracts from true course) */

    ST_nmeaSATINFO satinfo;     /**< Satellites information */
} ST_nmeaINFO;

/**
  * @}
  */

/** @defgroup GNSS_Exported_Functions
  * @{
  */

/**
  * @brief  This function will open the gnss system.
  *
  * @param  mode            GPS work mode.
  *
  * @retval FALSE           Open gnss system fail.
  * @retval TRUE            Open gnss system success.
  */
BOOL MG_GNSS_Open(void);

/**
  * @brief  This function will close the gnss system.
  *
  * @param  None.
  *
  * @retval FALSE           Close gnss system fail.
  * @retval TRUE            Close gnss system success.
  */
BOOL MG_GNSS_Close(void);

/**
  * @brief  This function will return the status of gnss system.
  *
  * @param  None.
  *
  * @retval 0               GNSS system is closed.
  * @retval 1               GNSS system is running.
  */
s32 MG_GNSS_GetStatus(void);

/**
  * @brief  This function will read nmea information from the GPS chip.
  *
  * @param  buf             Buffer to store the data read from the GPS chip.
  * @param  size            Size of data to read.
  *
  * @retval
  *         Return the actual length of data read from the GPS chip.
  */
s32 MG_GNSS_GetNmeaInfo(u8 *buf, u32 size);

/**
  * @brief  This function is used to parse the NMEA information.
  *
  * @param  src             The data to parse.
  * @param  info            Pointer to parse result.
  *
  * @retval FALSE           Parse the NMEA information fail.
  * @retval TRUE            Parse the NMEA information success.
  */
BOOL MG_GNSS_NmeaParse(u8 *src, ST_nmeaINFO *info);

/**
  * @brief  This function will get parse specific statement form the NMEA information.
  *
  * @param  src             Pointer to the date to parse.
  * @param  dest            The buffer to store specific nmea data.
  * @param  type            The specific statement type.
  *
  * @retval FALSE           Filter the NMEA sentence fail.
  * @retval TRUE            Filter the NMEA sentence success.
  */
BOOL MG_GNSS_NmeaSentenceFilter(const u8 *src, u8 *dest, Enum_nmeaTYPE type);

void MG_GNSS_DebugConfig(Enum_DebugPort port);

#ifdef __cplusplus
}
#endif

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

#endif /* __MG_GNSS_H */
/******************* (C) COPYRIGHT 2023 MEIG SMART*****END OF FILE*************/