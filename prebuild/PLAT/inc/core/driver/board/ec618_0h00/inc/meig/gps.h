/**
  ******************************************************************************
  * @file    gps.h
  * @author  MEIG Development Team
  * @version V1.0.4
  * @date    07-Jun-2023
  * @brief   This file contains all the functions prototypes for the GPS.
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
#ifndef __DRV_GPS_H
#define __DRV_GPS_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "stdbool.h"

/** @addtogroup SLM3xx_Driver
  * @{
  */

/** @addtogroup GPS
  * @brief GPS driver modules
  * @{
  */

/** @defgroup GPS_Exported_Types
  * @{
  */

enum GNSS_MODE
{
    GNSS_MODE_BDS       = 1,
    GNSS_MODE_GPS,
    GNSS_MODE_BDS_GPS,
    GNSS_MODE_GLONASS,
    GNSS_MODE_BDS_GLONASS,
    GNSS_MODE_GPS_GLONASS
};

enum GPS_NMEA_CMD
{
    GPS_NMEA_CMD_ID_GGA_ON     = 0,
    GPS_NMEA_CMD_ID_GGA_OFF,
    GPS_NMEA_CMD_ID_GSA_ON,
    GPS_NMEA_CMD_ID_GSA_OFF,
    GPS_NMEA_CMD_ID_GSV_ON,
    GPS_NMEA_CMD_ID_GSV_OFF,
    GPS_NMEA_CMD_ID_RMC_ON,
    GPS_NMEA_CMD_ID_RMC_OFF,
    GPS_NMEA_CMD_ID_VTG_ON,
    GPS_NMEA_CMD_ID_VTG_OFF,
    GPS_NMEA_CMD_ID_GLL_ON,
    GPS_NMEA_CMD_ID_GLL_OFF,
    GPS_NMEA_CMD_ID_ZDA_ON,
    GPS_NMEA_CMD_ID_ZDA_OFF,
    GPS_NMEA_CMD_ID_TXT_ON,
    GPS_NMEA_CMD_ID_TXT_OFF
};

typedef enum
{
    CONFIG_ID_STATLLITE = 0,
    CONFIG_ID_SLEEP,
    CONFIG_ID_NMEA,
    CONFIG_ID_INTERVAL,
    CONFIG_ID_START_MODE,
    CONFIG_ID_AID_TIME,
} gps_config_id_e;

typedef struct
{
    uint16_t        year;
    uint8_t         mon;
    uint8_t         day;
    uint8_t         hour;
    uint8_t         min;
    uint8_t         sec;
    uint8_t         milsec;
} utc_time_t;

typedef struct
{
    char latitude[15];
    char longitude[15];
    char altitude[15];
} gps_loc_msg;
typedef enum
{
    GPS_CMD_R = 0,
    GPS_CMD_W,
} gps_cmd_opt;

typedef enum
{
    GPS_CONFIG_CFGPRT = 0,
    GPS_CONFIG_CFGMSG,
    GPS_CONFIG_CFGTP,
    GPS_CONFIG_CFGNMEA,
    GPS_CONFIG_CFGSYS,
    GPS_CONFIG_CFGDYN,
    GPS_CONFIG_CFGGEOID,
    GPS_CONFIG_CFGSAVE,
    GPS_CONFIG_CFGCLR,
    GPS_CONFIG_CFGCWOUT,
} gps_config_id;

/** @defgroup GPS_Exported_Constants
  * @{
  */

/**
  * @}
  */

/** @defgroup GPS_Exported_Functions
  * @{
  */
/**
 * @brief  gps state function.
 *
 * @param  None.
 *
 * @retval false           gps hasn't open
 * @retval true            gps has open
 */
bool drvGpsState(void);

/**
  * @brief  Open gps function.
  *
  * @param  None.
  *
  * @retval false           Open gps fail.
  * @retval true            Open gps success.
  */
bool drvGpsOpen(void);

/**
  * @brief  Close gps function.
  *
  * @param  None.
  *
  * @retval None.
  */
void drvGpsClose(void);

/**
  * @brief  Wite data to gps chip.
  *
  * @param  buf             Pointer to the data to write.
  * @param  size            Size of date to write.
  *
  * @retval false           Data write fail.
  * @retval true            Data write success.
  */
bool drvGpsWriteData(uint8_t *buf, uint32_t size);

/**
  * @brief  Read data from gps chip.
  *
  * @param  buf             Pointer to the buf to store data read from gps chip.
  * @param  size            Size of date to read from gps chip.
  *
  * @retval false           Data read fail.
  * @retval true            Data read success.
  */
bool drvGpsReadData(uint8_t *buf, uint32_t size);

/**
  * @brief  Configure the gps chip.
  *
  * @param  id              Config command id, one value in @enum gps_config_id_e.
  * @param  param           Parameter of the config command.
  *
  * @retval false           Configure fail.
  * @retval true            Configure success.
  */
bool drvGpsConfig(gps_config_id id, gps_cmd_opt opt, char *src, char *dst);

/**
  * @brief  Aid time to gps chip.
  *
  * @param  time           Pointer to the utc time to aid into gps chip.
  *
  * @retval false          Aid time fail.
  * @retval true           Aid time success.
  */
bool drvGpsAidTime(gps_cmd_opt opt, utc_time_t *time);

/**
 * @brief  writer gps location message
 *
 * @param opt     Read or write cmd
 * @param lat     Latitude
 * @param lat_dir Latitude
 * @param lon     Longitude
 * @param lon_dir    Latitude
 * @param alt    altitude
 * @retval true
 * @retval false
 */
void nmea_dtostr(double data, int bit, char *buf);
bool drvGpsAidPos(gps_cmd_opt opt, double lat, char lat_dir, double lon, char lon_dir, double alt);

/**
 * @brief Set reset mode
 *
 * @param type
 * @param clr_mask
 * @retval true
 * @retval false
 */
bool drvGpsSetReset(gps_cmd_opt opt, uint8_t type, uint8_t clr_mask);

/**
 * @brief  Get product message
 *
 * @retval true
 * @retval false
 */
bool drvGpsGetPdtInfo(gps_cmd_opt opt);

/**
 * @brief  check star data  state
 *
 * @param opt    Read or write cmd
 * @param sys    0:GPS, 1:BDS
 * @retval true
 * @retval false
 */
bool drvGpsAidCfg(gps_cmd_opt opt, uint8_t sys);

/**
 * @brief  ANT Check
 *
 * @param opt
 * @param antType
 * @retval true
 * @retval false
 */
bool drvGpsAntState(gps_cmd_opt opt, uint8_t antType);

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

#endif /* __DRV_GPS_H */
/******************* (C) COPYRIGHT 2023 MEIG SMART*****END OF FILE*************/