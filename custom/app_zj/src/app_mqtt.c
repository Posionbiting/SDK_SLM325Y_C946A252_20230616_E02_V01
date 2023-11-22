#include "app_include.h"
#include "cJSON.h"
#include "mqtt_time.h"
#include "mg_mqtt.h"
#include "mg_sim.h"
#include "mg_network.h"
#include "mg_timer.h"
#include "mg_pm.h"
#include "slpman.h"
#include "ps_lib_api.h"
#include "math.h"

#define LOG_TAG MAKE_LOG_TAG('U', 'S', 'E', 'R')

#define APP_PUB_TOPIC_DEV_SN    "tank/gpsMsg/"
#define APP_PUB_TOPIC_ALARM     "tank/gpsMsg/"
#define APP_SUB_DOWN_PARAM      "device/ota/"
#define HW_VERSION              "TTC411.0.0.1"

#define DEVICE_TYPE        "TTC411"
#define TOPIC_MAX_LEN      64
#define CHAR_MAX_LEN       20

#define remoteURL "47.89.13.131"
#define remotePort 3134
#define batConsume 350


char mTopic1[TOPIC_MAX_LEN] = {0};//平台下发配置
char mTopic2[TOPIC_MAX_LEN] = {0};//查询设备当前配置
char mTopic3[TOPIC_MAX_LEN] = {0};//平台下发升级指令
char mTopic4[TOPIC_MAX_LEN] = {0};//平台下发批量升级指令
char mTopic5[TOPIC_MAX_LEN] = {0};//平台查询设备固件
char mTopic6[TOPIC_MAX_LEN] = {0};//设备消息上报
char mTopic7[TOPIC_MAX_LEN] = {0};//设备返回配置结果
char mTopic8[TOPIC_MAX_LEN] = {0};//返回当前设备配置
char mTopic9[TOPIC_MAX_LEN] = {0};//设备返回升级结果
char mTopic10[TOPIC_MAX_LEN] = {0};//设备返回当前固件
char mTopica[TOPIC_MAX_LEN] = {0};//查询电量
char mTopicb[TOPIC_MAX_LEN] = {0};//上报电量

u8 settingId[CHAR_MAX_LEN];
u8 queryId[CHAR_MAX_LEN];
u8 qfirmwareId[CHAR_MAX_LEN];
u8 version[CHAR_MAX_LEN];


static char *_app_netTypeToString(app_net_type netType)
{
    switch(netType)
    {
    case NET_TYPE_GSM:
        return "2G";
    case NET_TYPE_LTE:
    default:
        return "4G";
    }
}

void app_getModuleInfo(app_cfg_t *ctx)
{
    int iRet = MG_RET_OK;
    u8 m_buff[64] = {0};
    //s32 m_vbat;

    iRet = MG_SIM_GetIMEI(ctx->netInfo.simId, m_buff, sizeof(m_buff));
    if (iRet > 0)
    {
        memset(ctx->devInfo.imei, 0, sizeof(ctx->devInfo.imei));
        memcpy(ctx->devInfo.imei, (char *)m_buff, sizeof(ctx->devInfo.imei));
    }

    {
        u8 signalLevel = 99;
        iRet = MG_NW_GetSignalQuality(ctx->netInfo.simId, &signalLevel);
        ctx->netInfo.signalLevel = signalLevel;
    }

    //m_vbat = MG_ADC_getChannelValue(MG_ADC_CHANNEL_VBAT);
    //ctx->batteryInfo.capacity = m_vbat;

}

static char* _app_gnss_degree2str(double val)
{   
    val = val / 1000000;
    int integerPart = (int)val;  // 转换为整数
    int decimalPart = (int)((val - integerPart) * 1000000);  // 保留小数位，并转换为整数

    static char integerStr[20];
    static char decimalStr[20];

    sprintf(integerStr, "%d", integerPart);  // 将整数位转换为字符串
    sprintf(decimalStr, "%d", decimalPart);  // 将小数位转换为字符串

    static char result[40];
    sprintf(result, "\"%s.%s\"", integerStr, decimalStr);  // 组合整数位和小数位
    
    return result;
}

//上报数据
static int app_mqtt_pub_uploadDevSN(app_cfg_t *ctx)
{
    int iRet = MG_RET_OK;
    unsigned char mEnd[6] ={0};
    unsigned int m_sendLen = 0;
    int m_iBuff = 0;

    memset(g_app_tmpBuff, 0, sizeof(g_app_tmpBuff));
    m_sendLen += sprintf(g_app_tmpBuff+m_sendLen, "{");

    // app_util_jsonDataPack(true, &m_sendLen, ATTR_STRING, "deviceSn", (unsigned char *)ctx->nvConfig.sn);
    // app_util_jsonDataPack(false, &m_sendLen, ATTR_STRING, "time", (unsigned char *)ctx->gnssData.utc);
    // app_util_jsonDataPack(false, &m_sendLen, ATTR_INT, "batteryLeft", (unsigned char *)&ctx->batteryInfo.capacity);
    float batteryLeft = 100*(g_app_cfg.nvConfig.batterySize-g_app_cfg.nvConfig.batteryCount*batConsume)/g_app_cfg.nvConfig.batterySize;
    int mbat = (int)(batteryLeft + 0.5);
    if(mbat>100)
        mbat = 100;
    if(mbat<0)
        mbat = 0;
    // memset(g_app_tmpBuff, 0, sizeof(g_app_tmpBuff));
    // m_sendLen += sprintf(g_app_tmpBuff+m_sendLen, "{");
    APP_DEBUG("[%d]hyj battery, consume:%.2f", __LINE__, batteryLeft);
    app_util_jsonDataPack(true, &m_sendLen, ATTR_STRING, "deviceSn", (unsigned char *)ctx->nvConfig.sn);
    app_util_jsonDataPack(false, &m_sendLen, ATTR_STRING, "time", (unsigned char *)ctx->gnssData.utc);
    app_util_jsonDataPack(false, &m_sendLen, ATTR_INT, "batteryLeft", (unsigned char *)&mbat);
    app_util_jsonDataPack(false, &m_sendLen, ATTR_STRING, "cellularType", (unsigned char *)_app_netTypeToString(ctx->netInfo.netType));
    app_util_jsonDataPack(false, &m_sendLen, ATTR_INT, "cellularStrength", (unsigned char *)&ctx->netInfo.signalLevel);
    // app_util_jsonDataPack(false, &m_sendLen, ATTR_DOUBLE, "lat", (unsigned char *)&ctx->gnssData.lat);
    // app_util_jsonDataPack(false, &m_sendLen, ATTR_DOUBLE, "lon", (unsigned char *)&ctx->gnssData.lon);
    app_util_jsonDataPack(false, &m_sendLen, ATTR_STRING, "lat", (unsigned char *)ctx->gnssData.lat);
    app_util_jsonDataPack(false, &m_sendLen, ATTR_STRING, "lon", (unsigned char *)ctx->gnssData.lon);
    // m_iBuff = ctx->gnssData.elv;
    app_util_jsonDataPack(false, &m_sendLen, ATTR_STRING, "altitude", (unsigned char *)ctx->gnssData.elv);
    // m_iBuff = ctx->gnssData.speed;
    app_util_jsonDataPack(false, &m_sendLen, ATTR_STRING, "speed", (unsigned char *)ctx->gnssData.speed);
    // app_util_jsonDataPack(false, &m_sendLen, ATTR_DOUBLE, "altitude", (unsigned char *)&ctx->gnssData.elv);
    // app_util_jsonDataPack(false, &m_sendLen, ATTR_DOUBLE, "speed", (unsigned char *)&ctx->gnssData.speed);
    app_util_jsonDataPack(false, &m_sendLen, ATTR_INT, "satellitesNum", (unsigned char *)&ctx->gnssData.view);
    app_util_jsonDataPack(false, &m_sendLen, ATTR_STRING, "gpsValid", (unsigned char *)"true");
    m_iBuff = ctx->nvConfig.sysState;
    app_util_jsonDataPack(false, &m_sendLen, ATTR_INT, "tamper", (unsigned char *)&m_iBuff);

    m_sendLen += sprintf(g_app_tmpBuff+m_sendLen, "%s}", mEnd);

    {
        iRet = MG_MQTT_Publish(ctx->mqtt.client, mTopic6,
                               (u8 *)g_app_tmpBuff, strlen(g_app_tmpBuff),
                               MQTT_PUBLISH_DUP_0, MQTT_QOS_0, MQTT_PUBLISH_RETAIN_0, ctx->mqtt.requestTimeout);

        if (iRet == MCI_RET_OK)
        {
            ctx->workState = 1;
            //save nv
            ctx->nvConfig.sysState = SYS_STATE_NOV;
            app_nvmSaveConfig();
        }
        else
        {
            ctx->workState = 0;
        }


    }

    APP_DEBUG("[%d]MQTT pub uploadDevSN, iRet:%d", __LINE__, iRet);
    return iRet;
}

void app_get_ParamSet(const u8 *data, u32 date_len)
{
    cJSON *root = NULL;
    cJSON *item = NULL;

    root = cJSON_Parse((char *)data);
    if(root == NULL)
    {
        APP_DEBUG("[%d]MQTT download param err", __LINE__);
        return;
    }

    item = cJSON_GetObjectItem(root, "settingId");
    if (item && item->type == cJSON_String)
    {
        memset(settingId, 0, sizeof(u8)*20);
        memcpy(settingId, item->valuestring, strlen(item->valuestring));
    }

    item = cJSON_GetObjectItem(root, "gpsInterval");
    if (item)
    {
        if (item->type == cJSON_String)
        {
            g_app_cfg.nvConfig.nReportCycle = atoi(item->valuestring);
        }
        else if (item->type == cJSON_Number)
        {
            g_app_cfg.nvConfig.nReportCycle = item->valueint;
            app_nvmSaveConfig();
        }
    }

    if(root != NULL)
    {
        cJSON_Delete(root);
    }

    APP_DEBUG("[%d]MQTT get paramSet samplingPeriod:%d", __LINE__, g_app_cfg.nvConfig.nReportCycle);
}


static int app_pub_ParamSet(app_cfg_t *ctx)
{
    int iRet = MG_RET_OK;
    unsigned char mEnd[6] ={0};
    unsigned int m_sendLen = 0;
    //int m_iBuff = 0;

    app_gps_getData(&g_app_cfg);

    memset(g_app_tmpBuff, 0, sizeof(g_app_tmpBuff));
    m_sendLen += sprintf(g_app_tmpBuff+m_sendLen, "{");

    app_util_jsonDataPack(true, &m_sendLen, ATTR_STRING, "settingId", (unsigned char *)settingId);
    app_util_jsonDataPack(false, &m_sendLen, ATTR_STRING, "deviceSn", (unsigned char *)ctx->nvConfig.sn);
    app_util_jsonDataPack(false, &m_sendLen, ATTR_STRING, "confirm", (unsigned char *)"true");
    app_util_jsonDataPack(false, &m_sendLen, ATTR_STRING, "time", (unsigned char *)ctx->gnssData.utc);

    APP_DEBUG("[%d]MQTT sub: %s", __LINE__, mTopic7);

    m_sendLen += sprintf(g_app_tmpBuff+m_sendLen, "%s}", mEnd);

    {
        iRet = MG_MQTT_Publish(ctx->mqtt.client, mTopic7,
                               (u8 *)g_app_tmpBuff, strlen(g_app_tmpBuff),
                               MQTT_PUBLISH_DUP_0, MQTT_QOS_1, MQTT_PUBLISH_RETAIN_0, ctx->mqtt.requestTimeout);
    }

    APP_DEBUG("[%d]MQTT pub ParamSet, iRet:%d", __LINE__, iRet);
    return iRet;
}

void app_get_ParamQuery(const u8 *data, u32 date_len)
{
    cJSON *root = NULL;
    cJSON *item = NULL;

    root = cJSON_Parse((char *)data);
    if(root == NULL)
    {
        APP_DEBUG("[%d]MQTT get paramQuery err", __LINE__);
        return;
    }

    item = cJSON_GetObjectItem(root, "querySettingId");
    if (item && item->type == cJSON_String)
    {
        memset(queryId, 0, sizeof(u8)*20);
        memcpy(queryId, item->valuestring, strlen(item->valuestring));
    }

    if(root != NULL)
    {
        cJSON_Delete(root);
    }

    APP_DEBUG("[%d]MQTT get paramQuery ", __LINE__);
}

static int app_pub_paramQuery(app_cfg_t *ctx)
{
    int iRet = MG_RET_OK;
    unsigned char mEnd[6] ={0};
    unsigned int m_sendLen = 0;

    memset(g_app_tmpBuff, 0, sizeof(g_app_tmpBuff));
    m_sendLen += sprintf(g_app_tmpBuff+m_sendLen, "{");
    int m_iBuff = 0;
    app_gps_getData(&g_app_cfg);

    app_util_jsonDataPack(true, &m_sendLen, ATTR_STRING, "querySettingId", (unsigned char *)queryId);
    app_util_jsonDataPack(false, &m_sendLen, ATTR_STRING, "deviceSn", (unsigned char *)ctx->nvConfig.sn);
    app_util_jsonDataPack(false, &m_sendLen, ATTR_STRING, "remoteURL", (unsigned char *)remoteURL);
    m_iBuff = remotePort;
    app_util_jsonDataPack(false, &m_sendLen, ATTR_INT, "remotePort", (unsigned char *)&m_iBuff);
    m_iBuff = ctx->nvConfig.nReportCycle;
    app_util_jsonDataPack(false, &m_sendLen, ATTR_INT, "gpsInterval", (unsigned char *)&m_iBuff);
    app_util_jsonDataPack(false, &m_sendLen, ATTR_STRING, "time", (unsigned char *)ctx->gnssData.utc);

    APP_DEBUG("[%d]MQTT sub: %s", __LINE__, mTopic8);
    m_sendLen += sprintf(g_app_tmpBuff+m_sendLen, "%s}", mEnd);

    {
        iRet = MG_MQTT_Publish(ctx->mqtt.client, mTopic8,
                               (u8 *)g_app_tmpBuff, strlen(g_app_tmpBuff),
                               MQTT_PUBLISH_DUP_0, MQTT_QOS_1, MQTT_PUBLISH_RETAIN_0, ctx->mqtt.requestTimeout);
    }

    APP_DEBUG("[%d]MQTT pub paramQuery, iRet:%d", __LINE__, iRet);
    return iRet;
}

void app_get_ParamQF(const u8 *data, u32 date_len)
{
    cJSON *root = NULL;
    cJSON *item = NULL;

    root = cJSON_Parse((char *)data);
    if(root == NULL)
    {
        APP_DEBUG("[%d]MQTT get paramQF err", __LINE__);
        return;
    }

    item = cJSON_GetObjectItem(root, "queryFirmwareId");
    if (item && item->type == cJSON_String)
    {
        memset(qfirmwareId, 0, sizeof(u8)*20);
        memcpy(qfirmwareId, item->valuestring, strlen(item->valuestring));
    }

    if(root != NULL)
    {
        cJSON_Delete(root);
    }

    APP_DEBUG("[%d]MQTT get paramQF", __LINE__);
}

static int app_pub_paramQF(app_cfg_t *ctx)
{
    int iRet = MG_RET_OK;
    unsigned char mEnd[6] ={0};
    unsigned int m_sendLen = 0;
    //int m_iBuff = 0;
    u8 iccid[21]={0};
    iRet = MG_SIM_GetICCID(SIM0,iccid,21);

    app_gps_getData(&g_app_cfg);

    memset(g_app_tmpBuff, 0, sizeof(g_app_tmpBuff));
    m_sendLen += sprintf(g_app_tmpBuff+m_sendLen, "{");

    app_util_jsonDataPack(true, &m_sendLen, ATTR_STRING, "qfirmwareId", (unsigned char *)qfirmwareId);
    app_util_jsonDataPack(false, &m_sendLen, ATTR_STRING, "deviceSn", (unsigned char *)ctx->nvConfig.sn);
    app_util_jsonDataPack(false, &m_sendLen, ATTR_STRING, "deviceType", (unsigned char *)DEVICE_TYPE);
    app_util_jsonDataPack(false, &m_sendLen, ATTR_STRING, "hwVersion", (unsigned char *)HW_VERSION);
    app_util_jsonDataPack(false, &m_sendLen, ATTR_STRING, "IMEI", (unsigned char *)ctx->devInfo.imei);
    app_util_jsonDataPack(false, &m_sendLen, ATTR_STRING, "phone", (unsigned char *)iccid);
    app_util_jsonDataPack(false, &m_sendLen, ATTR_STRING, "version", (unsigned char *)APP_VER);
    app_util_jsonDataPack(false, &m_sendLen, ATTR_STRING, "time", (unsigned char *)ctx->gnssData.utc);

    APP_DEBUG("[%d]MQTT sub10: %s", __LINE__, mTopic10);

    m_sendLen += sprintf(g_app_tmpBuff+m_sendLen, "%s}", mEnd);

    {
        iRet = MG_MQTT_Publish(ctx->mqtt.client, mTopic10,
                               (u8 *)g_app_tmpBuff, strlen(g_app_tmpBuff),
                               MQTT_PUBLISH_DUP_0, MQTT_QOS_2, MQTT_PUBLISH_RETAIN_0, ctx->mqtt.requestTimeout);
    }

    APP_DEBUG("[%d]MQTT pub paramQF, iRet:%d", __LINE__, iRet);
    return iRet;
}

void app_get_ParamBattery(const u8 *data, u32 date_len)
{
    cJSON *root = NULL;
    cJSON *item = NULL;

    root = cJSON_Parse((char *)data);
    if(root == NULL)
    {
        APP_DEBUG("[%d]MQTT get paramQF err", __LINE__);
        return;
    }

    item = cJSON_GetObjectItem(root, "batterySettingId");

    if (item && item->type == cJSON_String)
    {
        memset(settingId, 0, sizeof(u8)*20);
        memcpy(settingId, item->valuestring, strlen(item->valuestring));
    }

    item = cJSON_GetObjectItem(root, "batterySize");

    if (item)
    {
        if (item->type == cJSON_String)
        {
              g_app_cfg.nvConfig.batterySize = atoi(item->valuestring);
        }
        else if (item->type == cJSON_Number)
        {
              g_app_cfg.nvConfig.batterySize = item->valueint;
        }
     }

    item = cJSON_GetObjectItem(root, "batteryCount");
    if (item)
    {
        if (item->type == cJSON_String)
        {
              g_app_cfg.nvConfig.batteryCount = atoi(item->valuestring);
        }
        else if (item->type == cJSON_Number)
        {
              g_app_cfg.nvConfig.batteryCount = item->valueint;
        }
     }

    app_nvmSaveConfig();

    if(root != NULL)
    {
        cJSON_Delete(root);
    }

    APP_DEBUG("[%d]MQTT get paramBattery", __LINE__);
}

static int app_pub_ParamBattery(app_cfg_t *ctx)
{
    int iRet = MG_RET_OK;
    unsigned char mEnd[6] ={0};
    unsigned int m_sendLen = 0;
    //int m_iBuff = 0;
    float batteryLeft = 100*(g_app_cfg.nvConfig.batterySize-g_app_cfg.nvConfig.batteryCount*batConsume)/g_app_cfg.nvConfig.batterySize;
    int mbat = (int)batteryLeft;
    app_gps_getData(&g_app_cfg);
    mbat+=1;
    if(mbat>100)
        mbat = 100;
    if(mbat<0)
        mbat = 0;
    int consume = ctx->nvConfig.batteryCount*batConsume;
    memset(g_app_tmpBuff, 0, sizeof(g_app_tmpBuff));
    m_sendLen += sprintf(g_app_tmpBuff+m_sendLen, "{");
    APP_DEBUG("[%d]hyj battery, consume:%.2f", __LINE__, batteryLeft);

    app_util_jsonDataPack(true, &m_sendLen, ATTR_STRING, "settingId", (unsigned char *)settingId);
    app_util_jsonDataPack(false, &m_sendLen, ATTR_INT, "batteryLeft", (unsigned char *)&mbat);
    app_util_jsonDataPack(false, &m_sendLen, ATTR_INT, "batterySize", (unsigned char *)&ctx->nvConfig.batterySize);
    app_util_jsonDataPack(false, &m_sendLen, ATTR_INT, "batteryCount", (unsigned char *)&consume);
    app_util_jsonDataPack(false, &m_sendLen, ATTR_STRING, "confirm", (unsigned char *)"true");
    app_util_jsonDataPack(false, &m_sendLen, ATTR_STRING, "time", (unsigned char *)ctx->gnssData.utc);

    m_sendLen += sprintf(g_app_tmpBuff+m_sendLen, "%s}", mEnd);

    {
        iRet = MG_MQTT_Publish(ctx->mqtt.client, mTopicb,
                               (u8 *)g_app_tmpBuff, strlen(g_app_tmpBuff),
                               MQTT_PUBLISH_DUP_0, MQTT_QOS_1, MQTT_PUBLISH_RETAIN_0, ctx->mqtt.requestTimeout);
    }

    APP_DEBUG("[%d]MQTT pub ParamBattery, iRet:%d", __LINE__, iRet);
    return iRet;
}

void app_get_downloadParam(const u8 *data, u32 date_len)
{
    cJSON *root = NULL;
    cJSON *item = NULL;
    bool bFota = false;

    root = cJSON_Parse((char *)data);
    if(root == NULL)
    {
        APP_DEBUG("[%d]MQTT download param err", __LINE__);
        return;
    }

    item = cJSON_GetObjectItem(root, "otaId");
    if (item && item->type == cJSON_String)
    {
        memset(g_app_cfg.nvConfig.otaId, 0, sizeof(u8)*20);
        memcpy(g_app_cfg.nvConfig.otaId, item->valuestring, strlen(item->valuestring));
    }

    item = cJSON_GetObjectItem(root, "version");
    if (item && item->type == cJSON_String)
    {
        memset(version, 0, sizeof(u8)*20);
        memcpy(version, item->valuestring, strlen(item->valuestring));
    }

    item = cJSON_GetObjectItem(root, "url");
    if (item && item->type == cJSON_String)
    {
        memset((char *)g_app_cfg.fotaFileDownloadUrl, 0, sizeof(g_app_cfg.fotaFileDownloadUrl));
        memcpy((char *)g_app_cfg.fotaFileDownloadUrl, item->valuestring, strlen(item->valuestring));
        bFota = true;
    }

    if(root != NULL)
    {
        cJSON_Delete(root);
    }

    if (bFota)
    {
        g_app_cfg.workState = 2;
        g_app_cfg.nvConfig.isSysUpdate = 1;
        app_nvmSaveConfig();
        app_fota_start();
    }

    APP_DEBUG("[%d]MQTT download param fota_state:%d, url:%s", __LINE__, g_app_cfg.nvConfig.isSysUpdate, g_app_cfg.fotaFileDownloadUrl);
    APP_DEBUG("[%d]MQTT download param samplingPeriod:%d", __LINE__, g_app_cfg.nvConfig.nReportCycle);
}


static int app_pub_ParamOta(app_cfg_t *ctx)
{
    int iRet = MG_RET_OK;
    unsigned char mEnd[6] ={0};
    unsigned int m_sendLen = 0;
    //int m_iBuff = 0;

    app_gps_getData(&g_app_cfg);

    memset(g_app_tmpBuff, 0, sizeof(g_app_tmpBuff));
    m_sendLen += sprintf(g_app_tmpBuff+m_sendLen, "{");

    app_util_jsonDataPack(true, &m_sendLen, ATTR_STRING, "otaId", (unsigned char *)ctx->nvConfig.otaId);
    app_util_jsonDataPack(false, &m_sendLen, ATTR_STRING, "deviceSn", (unsigned char *)ctx->nvConfig.sn);
    app_util_jsonDataPack(false, &m_sendLen, ATTR_STRING, "confirm", (unsigned char *)"true");
    app_util_jsonDataPack(false, &m_sendLen, ATTR_STRING, "hwVersion", (unsigned char *)HW_VERSION);
    app_util_jsonDataPack(false, &m_sendLen, ATTR_STRING, "version", (unsigned char *)APP_VER);
    app_util_jsonDataPack(false, &m_sendLen, ATTR_STRING, "errorMessage", (unsigned char *)"");
    app_util_jsonDataPack(false, &m_sendLen, ATTR_STRING, "time", (unsigned char *)ctx->gnssData.utc);

    m_sendLen += sprintf(g_app_tmpBuff+m_sendLen, "%s}", mEnd);
    {
        iRet = MG_MQTT_Publish(ctx->mqtt.client, mTopic9,
                               (u8 *)g_app_tmpBuff, strlen(g_app_tmpBuff),
                               MQTT_PUBLISH_DUP_0, MQTT_QOS_2, MQTT_PUBLISH_RETAIN_0, ctx->mqtt.requestTimeout);
    }

    APP_DEBUG("[%d]MQTT pub ParamOta, iRet:%d", __LINE__, iRet);
    return iRet;
}

//订阅FOTA主题
static int app_mqtt_sub_paramDown(app_cfg_t *ctx)
{
    int iRet = MG_RET_OK;
    iRet += MG_MQTT_Subscribe(ctx->mqtt.client, mTopic1, MQTT_QOS_1, ctx->mqtt.requestTimeout);

    APP_DEBUG("[%d]MQTT sub: %s", __LINE__, mTopic1);
    iRet += MG_MQTT_Subscribe(ctx->mqtt.client, mTopic2, MQTT_QOS_1, ctx->mqtt.requestTimeout);
    APP_DEBUG("[%d]MQTT sub: %s", __LINE__, mTopic2);
    iRet += MG_MQTT_Subscribe(ctx->mqtt.client, mTopic3, MQTT_QOS_2, ctx->mqtt.requestTimeout);
    APP_DEBUG("[%d]MQTT sub: %s", __LINE__, mTopic3);
    iRet += MG_MQTT_Subscribe(ctx->mqtt.client, mTopic4, MQTT_QOS_2, ctx->mqtt.requestTimeout);
    APP_DEBUG("[%d]MQTT sub: %s", __LINE__, mTopic4);
    iRet += MG_MQTT_Subscribe(ctx->mqtt.client, mTopic5, MQTT_QOS_1, ctx->mqtt.requestTimeout);
    APP_DEBUG("[%d]MQTT sub: %s", __LINE__, mTopic5);
    iRet += MG_MQTT_Subscribe(ctx->mqtt.client, mTopica, MQTT_QOS_1, ctx->mqtt.requestTimeout);
    APP_DEBUG("[%d]MQTT sub: %s", __LINE__, mTopica);

    APP_DEBUG("[%d]MQTT sub topic, iRet:%d", __LINE__, iRet);
    return MG_RET_OK;
}


void app_mqtt_incoming_cb(void *arg, const u8 *topic, u32 topic_len, const u8 *data, u32 date_len)
{
    char mTopic[TOPIC_MAX_LEN+1] = {0};
    app_cfg_t *pApp = &g_app_cfg;
    APP_DEBUG("[%d]MQTT incoming", __LINE__);
    memcpy(mTopic, (char *)topic, topic_len > TOPIC_MAX_LEN ? TOPIC_MAX_LEN : topic_len);

    APP_DEBUG("[%d]MQTT date_len:%d, data:%s", __LINE__, date_len , data);
    if(strstr(mTopic, mTopic1))//平台发送设置指令
    {
        APP_DEBUG("[%d]MQTT topic_len:%d, topic:%s", __LINE__, topic_len , mTopic1);
        app_get_ParamSet(data, date_len);
        app_pub_ParamSet(pApp);
    }
    else if(strstr(mTopic, mTopic2))//平台发送查询指令
    {
        APP_DEBUG("[%d]MQTT topic_len:%d, topic:%s", __LINE__, topic_len , mTopic2);
        app_get_ParamQuery(data, date_len);
        app_pub_paramQuery(pApp);
    }
    else if(strstr(mTopic, mTopic3) || strstr(mTopic, mTopic4))//平台发送升级指令
    {
        //app_pub_ParamOta(pApp);
        APP_DEBUG("[%d]MQTT topic_len:%d, topic:%s", __LINE__, topic_len , mTopic3);
        APP_DEBUG("[%d]MQTT topic_len:%d, topic:%s", __LINE__, topic_len , mTopic4);
        app_get_downloadParam(data, date_len);

    }
    else if(strstr(mTopic, mTopic5))//平台查询固件版本
    {
        APP_DEBUG("[%d]MQTT topic_len:%d, topic:%s", __LINE__, topic_len , mTopic5);
        app_get_ParamQF(data, date_len);
        app_pub_paramQF(pApp);

    }
    else if(strstr(mTopic, mTopica))//平台查询电量
    {
        APP_DEBUG("[%d]MQTT topic_len:%d, topic:%s", __LINE__, topic_len , mTopica);
        app_get_ParamBattery(data, date_len);
        app_pub_ParamBattery(pApp);

    }
}

int app_mqtt_connect(app_cfg_t *ctx)
{
    int iRet = MG_RET_OK;
    u8 cid = ctx->netInfo.cid;
    char name[20];
    char password[20];
    strcpy(name, (char *)ctx->nvConfig.key);
    strcpy(password, (char *)ctx->nvConfig.secret);
    APP_DEBUG("[%d]MQTT connect", __LINE__);
    APP_DEBUG("name:%s", name);
    APP_DEBUG("password:%s", password);
    if (ctx->mqtt.client == NULL)
    {
        ctx->mqtt.client = MG_MQTT_Create(&ctx->mqtt.clientInfo);
        if (!ctx->mqtt.client)
            return MG_RET_ERR_MQTT_ENOMEM;
    }

    if (MG_MQTT_GetClientState(ctx->mqtt.client) != MQTT_STATE_CONNECT
     && MG_MQTT_GetClientState(ctx->mqtt.client) != MQTT_STATE_DISCONNECTING)
    {
        APP_DEBUG("[%d]MQTT connect, iRet:%d", __LINE__, iRet);
        {
            ctx->mqtt.connectCtx.client_id        = (char *)ctx->nvConfig.sn;
            ctx->mqtt.connectCtx.hostname         = remoteURL;
            ctx->mqtt.connectCtx.port             = remotePort;
            ctx->mqtt.connectCtx.username         = name;
            ctx->mqtt.connectCtx.password         = password;
            ctx->mqtt.connectCtx.keep_alive       = 300;
            ctx->mqtt.connectCtx.clean_session    = 0;

            ctx->mqtt.connectTimeout = 60*1000;
            ctx->mqtt.requestTimeout = 60*1000;
        }

        iRet = MG_MQTT_SetOpt(ctx->mqtt.client, MQTT_OPT_BIND_CID_SET, &cid, sizeof(cid));
        if (MG_RET_OK == iRet)
        {
            APP_DEBUG("[%d]MQTT connect, iRet:%d", __LINE__, iRet);
            iRet = MG_MQTT_RegisterEx(ctx->mqtt.client, app_mqtt_incoming_cb, NULL);
        }

        if (MG_RET_OK == iRet)
        {
            APP_DEBUG("[%d]MQTT connect, iRet:%d", __LINE__, iRet);
            iRet = MG_MQTT_Connect(ctx->mqtt.client, &ctx->mqtt.connectCtx, ctx->mqtt.connectTimeout);
            if (MG_RET_OK == iRet)
            {
                iRet = app_mqtt_sub_paramDown(ctx);
            }
        }

        ctx->mqtt.state = iRet != MG_RET_OK ? APP_MQTT_DISCONNECTED : APP_MQTT_CONNECTED;
        if(iRet != MG_RET_OK)
        {
            APP_DEBUG("[%d]MQTT connect, iRet:%d", __LINE__, iRet);
            MG_MQTT_Disconnect(ctx->mqtt.client);
        }
    }

    return iRet;
}

void app_mqtt_timer_cb(void *param)
{
    ST_Semaphore *semaphore = (ST_Semaphore *)param;
    MG_OS_SemaphoreRelease(semaphore);
}

void app_mqtt_getData(app_cfg_t *pApp)
{
    app_getModuleInfo(pApp);
    app_gps_getData(pApp);
}

static void app_slp_beforeSleep(void *pdata, slpManLpState state)
{
    // enable aonio latch
    slpManAONIOLatchEn(true);

    switch(state)
    {
        case SLPMAN_SLEEP1_STATE:
            APP_DEBUG("[%d]slp_beforeSleep, state:%d", __LINE__, state);
            //ECPLAT_PRINTF(UNILOG_PLA_APP, appBeforeSleep_1, P_SIG, "Before Sleep1");
            break;
        case SLPMAN_SLEEP2_STATE:
            APP_DEBUG("[%d]slp_beforeSleep, state:%d", __LINE__, state);
            //ECPLAT_PRINTF(UNILOG_PLA_APP, appBeforeSleep_2, P_SIG, "Before Sleep2");
            break;
        case SLPMAN_HIBERNATE_STATE:
            APP_DEBUG("[%d]slp_beforeSleep, state:%d", __LINE__, state);
            //ECPLAT_PRINTF(UNILOG_PLA_APP, appBeforeSleep_3, P_SIG, "Before Hibernate");
            break;
        default:
            break;
    }
}


#define E_SLEEP_TIMER  0

static void app_mqtt_thread(void *param)
{
    int iRet = MG_RET_OK;
    app_cfg_t *pApp = &g_app_cfg;
#if E_SLEEP_TIMER
    ST_Semaphore *semaphore = MG_OS_SemaphoreCreate(1, 0);
    ST_Timer *stTimer = MG_TIMER_IrqCreate(app_mqtt_timer_cb, (void *)semaphore);
#endif
    uint8_t slpmanSlp1Handler = 0xff;
    bool bFirst = true;

    sprintf(mTopic1, "devices/%s/%s/settings/set", DEVICE_TYPE, g_app_cfg.nvConfig.sn);
    sprintf(mTopic2, "devices/%s/%s/settings/query", DEVICE_TYPE, g_app_cfg.nvConfig.sn);
    sprintf(mTopic3, "devices/%s/%s/ota/set", DEVICE_TYPE, g_app_cfg.nvConfig.sn);
    sprintf(mTopic4, "devices/%s/all/ota/set", DEVICE_TYPE);
    sprintf(mTopic5, "devices/%s/%s/ota/query", DEVICE_TYPE, g_app_cfg.nvConfig.sn);
    sprintf(mTopica, "devices/%s/%s/battery/set", DEVICE_TYPE, g_app_cfg.nvConfig.sn);

    sprintf(mTopic6, "devices/%s/%s/Msg", DEVICE_TYPE, g_app_cfg.nvConfig.sn);
    sprintf(mTopic7, "devices/%s/%s/settings/confirm", DEVICE_TYPE, g_app_cfg.nvConfig.sn);
    sprintf(mTopic8, "devices/%s/%s/settings/response", DEVICE_TYPE, g_app_cfg.nvConfig.sn);
    sprintf(mTopic9, "devices/%s/%s/ota/confirm", DEVICE_TYPE, g_app_cfg.nvConfig.sn);
    sprintf(mTopic10,"devices/%s/%s/ota/response", DEVICE_TYPE, g_app_cfg.nvConfig.sn);
    sprintf(mTopicb, "devices/%s/%s/battery/confirm", DEVICE_TYPE, g_app_cfg.nvConfig.sn);

    slpManSetPmuSleepMode(true, SLP_SLP1_STATE, false);
    slpManApplyPlatVoteHandle("appslp1", &slpmanSlp1Handler);

    slpManRegisterUsrdefinedBackupCb(app_slp_beforeSleep, NULL);
    slpManPlatVoteDisableSleep(slpmanSlp1Handler, SLP_HIB_STATE);

    while(1)
    {
        //激活网络
        app_net_doAct(60*1000, pApp);

        //跑业务
        if (strlen((const char *)pApp->nvConfig.sn) > 0 && pApp->netState == NET_ACTED)
        {
            app_mqtt_getData(pApp);
            iRet = app_mqtt_connect(pApp);
            if (MG_RET_OK == iRet)
            {
                g_app_cfg.nvConfig.batteryCount++;
                APP_DEBUG("[%d]MQTT upload info", __LINE__);
                APP_DEBUG("[%d]MQTT g_app_cfg.nvConfig.isSysUpdate:%d", __LINE__, g_app_cfg.nvConfig.isSysUpdate);
                if(g_app_cfg.nvConfig.isSysUpdate != 0){
                    app_pub_ParamOta(pApp);
                    g_app_cfg.nvConfig.isSysUpdate = 0;
                    app_nvmSaveConfig();
                }
                else
                {
                app_mqtt_pub_uploadDevSN(pApp);
                }

                do
                {
                    app_util_threadSleep(2000, pApp->bLowPowerModeEn);
                }while(g_app_cfg.isSysUpdate);

                //MG_MQTT_Disconnect(pApp->mqtt.client);
            }

            bFirst = false;
        }

        if (bFirst)
            continue;

#if E_SLEEP_TIMER
        MG_TIMER_Start(stTimer, 1000*pApp->nvConfig.nReportCycle, false);
        slpManPlatVoteEnableSleep(slpmanSlp1Handler, SLP_SLP1_STATE);

        //等唤醒
        MG_OS_SemaphoreAcquireLowPower(semaphore);
        APP_DEBUG("[%d]MQTT start", __LINE__);

        slpManPlatVoteDisableSleep(slpmanSlp1Handler, SLP_SLP1_STATE);
        //pApp->netState = NET_NO_ACT;
#else
        slpManPlatVoteEnableSleep(slpmanSlp1Handler, SLP_SLP1_STATE);
        while(1)
        {
            app_util_threadSleep(100, pApp->bLowPowerModeEn);
        }
#endif
    }

    if (pApp->mqtt.client)
    {
        MG_MQTT_Destroy(pApp->mqtt.client);
        pApp->mqtt.client = NULL;
    }

    APP_DEBUG("[%d]MQTT exit thread", __LINE__);
    app_util_threadSleep(1000, pApp->bLowPowerModeEn);
    MG_OS_ThreadExit();
}

void app_mqtt_task(void)
{
    APP_DEBUG("[%d]MQTT start thread", __LINE__);

    app_gps_init();
    MG_OS_ThreadCreate("appMqtt", app_mqtt_thread, NULL, Thread_PRIORITY_NORMAL, 1024*20, 50);
}

