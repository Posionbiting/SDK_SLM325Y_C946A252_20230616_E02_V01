/**
  ******************************************************************************
  * @file    mg_error.h
  * @author  MEIG Development Team
  * @version V1.0.0
  * @date    28-Mar-2022
  * @brief   This file provides all error code.
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
#ifndef __MG_ERROR_H
#define __MG_ERROR_H

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup ERROR_CODE_OpenCPU
  * @{
  */

/** @defgroup ERROR_CODE
  * @{
  */

enum
{
    MG_RET_OK                       = 0,
    MG_RET_ERR                      = -1,
    MG_RET_ERR_PARAM                = -2,
    MG_RET_ERR_ENOMEM               = -3,
    MG_RET_ERR_QUEUE_FULL           = -4,
    MG_RET_ERR_QUEUE_EMPTY          = -5,
    MG_RET_ERR_TIMEOUT              = -6,
    MG_RET_ERR_EINPROGRESS          = -7,
    MG_RET_ERR_NOSUPPORT            = -8,

    /*reserved to -100 */
    MG_RET_ERR_ALARM_ERROR           = -100,
    MG_RET_ERR_ALARM_EINVAL          = -101,
    MG_RET_ERR_ALARM_ENOMEM          = -102,
    MG_RET_ERR_ALARM_EREG            = -103,
    MG_RET_ERR_ALARM_EINVALDATE      = -104,

    /*reserved to -150 */
    MG_RET_ERR_SIM_ERROR             = -150,
    MG_RET_ERR_SIM_PARAM             = -151,
    MG_RET_ERR_SIM_ENOMEM            = -152,
    MG_RET_ERR_SIM_ETIMEOUT          = -153,
    MG_RET_ERR_SIM_NOINSERTED        = -154,
    MG_RET_ERR_SIM_NOALLOWED         = -155,
    MG_RET_ERR_SIM_EUTIPARAM         = -156,
    MG_RET_ERR_SIM_EUTIMATCH         = -157,
    MG_RET_ERR_SIM_EUTITYPE          = -158,
    MG_RET_ERR_SIM_EUTIMEM           = -159,
    MG_RET_ERR_SIM_ENOSESSION        = -160,

    /*reserved to -200 */
    MG_RET_ERR_SOCK_ERROR           = -200,
    MG_RET_ERR_SOCK_ENOMEM          = -201,
    MG_RET_ERR_SOCK_ENOBUFS         = -202,
    MG_RET_ERR_SOCK_EWOULDBLOCK     = -203,
    MG_RET_ERR_SOCK_EHOSTUNREACH    = -204,
    MG_RET_ERR_SOCK_EINPROGRESS     = -205,
    MG_RET_ERR_SOCK_EINVAL          = -206,
    MG_RET_ERR_SOCK_EADDRINUSE      = -207,
    MG_RET_ERR_SOCK_EALREADY        = -208,
    MG_RET_ERR_SOCK_EISCONN         = -209,
    MG_RET_ERR_SOCK_ENOTCONN        = -210,
    MG_RET_ERR_SOCK_ECONNABORTED    = -211,
    MG_RET_ERR_SOCK_ECONNRESET      = -212,
    MG_RET_ERR_SOCK_EIO             = -213,
    MG_RET_ERR_SOCK_EIPADDR         = -214,
    MG_RET_ERR_DNS_ERROR            = -215,
    MG_RET_ERR_DNS_EINPROGRESS      = -216,
    MG_RET_ERR_DNS_EINVAL           = -217,
    MG_RET_ERR_DNS_EREQFAIL         = -218,

    /* reserved to -250 */
    MG_RET_ERR_SSL_ERROR            = -250,
    MG_RET_ERR_SSL_EINVAL           = -251,
    MG_RET_ERR_SSL_ENOMEM           = -252,
    MG_RET_ERR_SSL_EHOST            = -253,
    MG_RET_ERR_SSL_ESOCKET          = -254,
    MG_RET_ERR_SSL_ECONN            = -255,
    MG_RET_ERR_SSL_TIMEOUT          = -256,
    MG_RET_ERR_SSL_WANTREAD         = -257,
    MG_RET_ERR_SSL_WANTWRITE        = -258,
    MG_RET_ERR_SSL_CRYPTOINPROGRESS = -259,
    MG_RET_ERR_SSL_ASYNCINPROGRESS  = -260,
    MG_RET_ERR_SSL_PEERCLOSENOTIFY  = -261,
    MG_RET_ERR_SSL_CONNEOF          = -262,
    MG_RET_ERR_SSL_SESSIONEXPIRED   = -263,
    MG_RET_ERR_SSL_NONFATAL         = -264,

    /* reserved to -300 */
    MG_RET_ERR_SYS_FAILED           = -300,

    /* reserved to -320 */
    MG_RET_ERR_FILE_ERR             = -320,
    MG_RET_ERR_FILE_ENAMETOOLENGTH  = -321,
    MG_RET_ERR_FILE_EINVAL          = -322,
    MG_RET_ERR_FILE_ENOMEM          = -323,
    MG_RET_ERR_FILE_ESYNC           = -324,
    MG_RET_ERR_DIR_ENOSUPPORT       = -325,

    /* reserved to -500 */
    MG_RET_ERR_MQTT_ERROR           = -500,
    MG_RET_ERR_MQTT_EINVAL          = -501,
    MG_RET_ERR_MQTT_ENOMEM          = -502,
    MG_RET_ERR_MQTT_ENOCREATE       = -503,
    MG_RET_ERR_MQTT_EDNSREQ         = -504,
    MG_RET_ERR_MQTT_ECONREQ         = -505,
    MG_RET_ERR_MQTT_EDNSTIMEOUT     = -506,
    MG_RET_ERR_MQTT_ECONTIMEOUT     = -507,
    MG_RET_ERR_MQTT_EPROTOVERSION   = -508,
    MG_RET_ERR_MQTT_EIDENTIFIER     = -509,
    MG_RET_ERR_MQTT_ESERVER         = -510,
    MG_RET_ERR_MQTT_EUSERNAME_PASS  = -511,
    MG_RET_ERR_MQTT_ENOTAUTHORIZED  = -512,
    MG_RET_ERR_MQTT_EDISCONNECTED   = -513,
    MG_RET_ERR_MQTT_EALREADYCONN    = -514,
    MG_RET_ERR_MQTT_ENOCREATECONN   = -515,
    MG_RET_ERR_MQTT_EMSGREQ         = -516,
    MG_RET_ERR_MQTT_EMSGREQTIMEOUT  = -517,
    MG_RET_ERR_MQTT_ECONNECTED      = -518,
    MG_RET_ERR_MQTT_EBUFSHORT       = -519,
    MG_RET_ERR_MQTT_EPACKETREAD     = -520,
    MG_RET_ERR_MQTT_EPACKETSEND     = -521,
    MG_RET_ERR_MQTT_EPACKETSERIAL   = -522,
    MG_RET_ERR_MQTT_EPACKETDESERIAL = -523,
    MG_RET_ERR_MQTT_EACKTYPE        = -524,
    MG_RET_ERR_MQTT_EACKTOOMUCH     = -525,
    MG_RET_ERR_MQTT_EREQNODEEXIST   = -526,
    MG_RET_ERR_MQTT_EREQRECORED     = -527,
    MG_RET_ERR_MQTT_ECREQSUBACK     = -528,
    MG_RET_ERR_MQTT_ESOCKID         = -529,
    MG_RET_ERR_MQTT_ESOCKCONFAIL    = -530,
    MG_RET_ERR_MQTT_ESOCKREAD       = -531,
    MG_RET_ERR_MQTT_ESOCKSEND       = -532,

    /* reserved to -600 */
    MG_RET_ERR_HTTP_ERROR           = -600,
    MG_RET_ERR_HTTP_EINVAL          = -601,
    MG_RET_ERR_HTTP_ENOMEM          = -602,
    MG_RET_ERR_HTTP_ETIMEOUT        = -603,
    MG_RET_ERR_HTTP_EINPROGRESS     = -604,
    MG_RET_ERR_HTTP_EURL            = -605,
    MG_RET_ERR_HTTP_ECON            = -606,
    MG_RET_ERR_HTTP_ESEND           = -607,
    MG_RET_ERR_HTTP_ERECV           = -608,
    MG_RET_ERR_HTTP_ECALLBACK       = -609,

    /*reserved to -700 */
    MG_RET_ERR_FTP_EXE                = -701,
    MG_RET_ERR_FTP_INITIALIZE         = -702,
    MG_RET_ERR_FTP_NOT_CONNECTED      = -703,
    MG_RET_ERR_FTP_HAD_CONNECTED      = -704,
    MG_RET_ERR_FTP_BUSY               = -705,
    MG_RET_ERR_FTP_OPEN               = -706,
    MG_RET_ERR_FTP_TIMEOUT            = -707,
    MG_RET_ERR_FTP_SIZE               = -708,

    /* reserved to -900 */
    MG_RET_ERR_FOTA_ERROR           = -900,
    MG_RET_ERR_FOTA_EINVAL          = -901,
    MG_RET_ERR_FOTA_ENOMEM          = -902,
    MG_RET_ERR_FOTA_EBUSY           = -903,
    MG_RET_ERR_FOTA_EFILE           = -904,
    MG_RET_ERR_FOTA_EVALID          = -905,
    MG_RET_ERR_FOTA_ENOREADY        = -906,
    MG_RET_ERR_FOTA_EIO             = -907,

    /* reserved to -950 */
    MG_RET_ERR_LBS_ERROR            = -950,
    MG_RET_ERR_LBS_EINVAL           = -951,
    MG_RET_ERR_LBS_ENOMEM           = -952,
    MG_RET_ERR_LBS_ENOACT           = -953,
    MG_RET_ERR_LBS_ECELL            = -954,
    MG_RET_ERR_LBS_ENOLTE           = -955,
    MG_RET_ERR_LBS_EHTTPREQ         = -956,
    MG_RET_ERR_LBS_EHTTPCODE        = -957,
    MG_RET_ERR_LBS_EHTTPCONTENT     = -958,
    MG_RET_ERR_LBS_ENOLOCINFO       = -959,

    /* reserved to -1000 */
    MG_RET_ERR_NETWORK_ERROR        = -1000,
    MG_RET_ERR_NETWORK_EINVAL       = -1001,
    MG_RET_ERR_NETWORK_ENOMEM       = -1002,
    MG_RET_ERR_NETWORK_ENOSIM       = -1003,
    MG_RET_ERR_NETWORK_EINCALL      = -1004,
    MG_RET_ERR_NETWORK_EINCFUN      = -1005,
    MG_RET_ERR_NETWORK_ENOALLOWED   = -1006,
    MG_RET_ERR_NETWORK_ENOREG       = -1007,
    MG_RET_ERR_NETWORK_ETIMEOUT     = -1008,
    MG_RET_ERR_NETWORK_EINPROGRESS  = -1009,
    MG_RET_ERR_NETWORK_EATTREQ      = -1010,
    MG_RET_ERR_NETWORK_EACTREQ      = -1011,
    MG_RET_ERR_NETWORK_ENOATT       = -1012,
    MG_RET_ERR_NETWORK_ENOADDR      = -1013,
    MG_RET_ERR_NETWORK_ECFUNREQ     = -1014,
    MG_RET_ERR_NETWORK_ECELLINFOREQ = -1015,

    /* reserved to -1100 */
    MG_RET_ERR_SMS_ERROR            = -1100,
    MG_RET_ERR_SMS_EINVAL           = -1101,
    MG_RET_ERR_SMS_ENOMEM           = -1002,
    MG_RET_ERR_SMS_EOPERATION       = -1103,
    MG_RET_ERR_SMS_ESEND            = -1104,
    MG_RET_ERR_SMS_ESENDRSP         = -1105,
    MG_RET_ERR_SMS_EINPROGRESS      = -1106,
    MG_RET_ERR_SMS_EGETSCA          = -1107,
    MG_RET_ERR_SMS_ESCALENLONG      = -1108,
    MG_RET_ERR_SMS_EPIDNOSUPPORT    = -1109,
    MG_RET_ERR_SMS_EDCSNOSUPPORT    = -1110,
    MG_RET_ERR_SMS_EPDUUDLENLONG    = -1111,
    MG_RET_ERR_SMS_EPDUTOTALLEN     = -1112,
    MG_RET_ERR_SMS_EISFDN           = -1113,

    /*reserved to -1200 */
    MG_RET_ERR_WIFI_ERROR           = -1200,
    MG_RET_ERR_WIFI_OPENED          = -1201,
    MG_RET_ERR_WIFI_SCAN            = -1202,
    MG_RET_ERR_WIFI_OPEN            = -1203,
    MG_RET_ERR_WIFI_EINVAL          = -1204,
    MG_RET_ERR_WIFI_ENOMEM          = -1205,
    MG_RET_ERR_WIFI_EINPROGRESS     = -1206,
    MG_RET_ERR_WIFI_ENOALLOWED      = -1207,

    /*reserved to -1300 */
    MG_RET_ERR_CALL_STATE            = -1301,
    MG_RET_ERR_CALL_VOICE            = -1302,
    MG_RET_ERR_CALL_EXE              = -1303,
    MG_RET_ERR_CALL_COMMGET          = -1304,
    MG_RET_ERR_CALL_COMMSTA          = -1305,
    MG_RET_ERR_CALL_DIALNUMLEN       = -1306,
    MG_RET_ERR_CALL_BCDMAXLEN        = -1307,
    MG_RET_ERR_CALL_BCDMINLEN        = -1308,
    MG_RET_ERR_CALL_CHANNEL          = -1309,
    MG_RET_ERR_CALL_PROHIBIT         = -1310,
    MG_RET_ERR_CALL_PARAMETER        = -1311,
    MG_RET_ERR_CALL_UTI              = -1312,

    /*reserved to -1400 */
    MG_RET_ERR_GNSS_ERROR            = -1400,
    MG_RET_ERR_GNSS_EPWR             = -1401,
    MG_RET_ERR_GNSS_EINVAL           = -1402,
    MG_RET_ERR_GNSS_ENOMEM           = -1403,
    MG_RET_ERR_GNSS_EINPROGRESS      = -1404,
    MG_RET_ERR_GNSS_ENOALLOWED       = -1405,

    /*reserved to -1500 */
    MG_RET_ERR_ABUP_ERROR            = -1500,
    MG_RET_ERR_ABUP_RUNNING          = -1501,
    MG_RET_ERR_ABUP_NETWORK          = -1502,
    MG_RET_ERR_ABUP_PARAM            = -1503,

    /*reserved to -1600 */
    MG_RET_ERR_NTP_ERROR             = -1600,
    MG_RET_ERR_NTP_ESOCK             = -1601,
    MG_RET_ERR_NTP_EINVAL            = -1602,
    MG_RET_ERR_NTP_EURL              = -1603,
    MG_RET_ERR_NTP_ENETNOREADY       = -1604,
    MG_RET_ERR_NTP_ERETRY            = -1605,
    MG_RET_ERR_NTP_EPDP              = -1606,
    MG_RET_ERR_NTP_ETIMEOUT          = -1607,
    MG_RET_ERR_NTP_ESYNC             = -1608,
    MG_RET_ERR_NTP_EINPROGRESS       = -1609,
    MG_RET_ERR_NTP_ENOMEM            = -1610,

};

#ifdef __cplusplus
}
#endif

/**
  * @}
  */

/**
  * @}
  */

#endif /* __MG_ERROR_H */
/******************* (C) COPYRIGHT 2022 MEIG SMART*****END OF FILE*************/
