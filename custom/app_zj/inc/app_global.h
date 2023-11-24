#ifndef _APP_GLOBAL_H
#define _APP_GLOBAL_H
#include "mg_mqtt.h"

#ifndef _GLOBAL__
#define _GLOBAL__ extern
#endif

// #define APP_VER         "0.1.1"
// #define APP_VER         "0.1.2"
#define APP_VER         "0.1.3"
typedef enum
{
    APP_MQTT_DISCONNECTED = 0,
    APP_MQTT_CONNECTED
} app_mqtt_state;

typedef enum
{
    NET_TYPE_GSM = 0,
    NET_TYPE_LTE,
} app_net_type;

typedef enum
{
    SYS_STATE_NOV = 0,  //正常模式
    SYS_STATE_LOCK      //触发防拆
} app_sys_state;

typedef struct APP_MQTT_CFG_T
{
    int connectTimeout;
    int requestTimeout;

    app_mqtt_state      state;
    ST_MqttClient       *client;
    ST_MqttConnectCtx   connectCtx;
    ST_MqttClientInfo   clientInfo;
} app_mqtt_cfg_t;

// 固件信息
typedef struct APP_DEV_INFO_T
{
    u8 sysVer[48];
    u8 imei[24];
} app_dev_info_t;

// 电池信息
typedef struct APP_BATTERY_INFO_T
{
    s32 capacity;       //电池电量
} app_battery_info_t;

// 网络动态数据
typedef struct APP_NET_INFO_T
{
    u8 simId;
    u8 cid;
    app_net_type netType;
    int signalLevel;

} app_net_info_t;

// GNSS采集数据
typedef struct APP_GNSS_DATA_T
{
    u8 utc[64];         //GPS UTC时间
    u32 view;           //可见卫星数量
    char lat[20];         //经度
    char lon[20];         //纬度
    char elv[10];         //海拔
    char speed[10];       //速度，kilometers/hour
    char mode[10];
} app_gnss_data_t;

typedef struct
{
    u8      sn[20];
    u32     nReportCycle;
    app_sys_state    sysState;
    u8      key[20];  //username
    u8      secret[20];  //password
    u8      version[20]; 
    u8      type[20]; 
    u8    isSysUpdate;  //fota
    u8    otaId[20];
    u32   batterySize;
    u32   batteryCount;
    u8    mbuff;
} app_nv_config;

typedef struct APP_CFG_T
{
    u8                      netState;
    u8                      workState;          //1-已上报定位
    bool                    bLowPowerModeEn;    //低功耗模式

    app_dev_info_t          devInfo;        //固件信息
    app_battery_info_t      batteryInfo;    //电池信息
    app_net_info_t          netInfo;        //网络数据
    app_gnss_data_t         gnssData;       //GNSS采集数据

    app_mqtt_cfg_t  mqtt;

    app_nv_config           nvConfig;

    //fota
    bool    isSysUpdate;
    u8 fotaFileDownloadUrl[256];
} app_cfg_t;

_GLOBAL__ char g_app_tmpBuff[1024*10];
_GLOBAL__ app_cfg_t g_app_cfg;

#endif

