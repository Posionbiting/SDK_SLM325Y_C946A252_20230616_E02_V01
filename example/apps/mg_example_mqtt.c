#include "mg_common.h"
#include "mg_type.h"
#include "mg_error.h"
#include "mg_os.h"
#include "mg_trace.h"
#include "mg_utils.h"
#include "mg_sys.h"
#include "mg_network.h"
#include "mg_mqtt.h"

#define LOG_TAG MAKE_LOG_TAG('E', 'X', 'M', 'Q')

#define DEBUG_ENABLE 1
#if DEBUG_ENABLE
#define APP_DEBUG(FORMAT, ...)  MG_TRACE_Printf(LOG_TAG, FORMAT, ##__VA_ARGS__)
#else
#define APP_DEBUG(FORMAT, ...)
#endif

#define MQTT_CONNECT_TIMEOUT_MS (100*1000)
#define MQTT_REQUEST_TIMEOUT_MS (10*1000)

#define MQTT_CLIENT_SUBSCRIBE_TOPIC  "/public/TEST/mPython"
#define MQTT_CLIENT_PUBLISH_TOPIC  "/public/TEST/mPython"
#define MQTT_CLIENT_PUBLISH_MSG  "This is test data, hello MQTT"

#define MQTT_BUFFER_MAX_LEN       1024
u8 g_buf[MQTT_BUFFER_MAX_LEN] = {0};

// auto activate (cid = 1)
static BOOL _doAct(void)
{
    s32 iResult = 0;

    /*get csq*/
    u8 nSignalLevel = 99;
    iResult = MG_NW_GetSignalQuality(SIM0, &nSignalLevel);
    APP_DEBUG("get csq[%d] iResult:%d, nSignalLevel:%d", __LINE__, iResult, nSignalLevel);
    if (iResult != MG_RET_OK)
    {
        return FALSE;
    }
    MG_OS_ThreadSleep(100);

    /*get network state*/
    Enum_NwState nNwState = NW_STATE_NO_REGISTERED;
    iResult = MG_NW_GetNetWorkState(SIM0, PDP_CID_1, &nNwState);
    APP_DEBUG("get network state[%d] iResult:%d, nNwState:%d", __LINE__, iResult, nNwState);
    if (nNwState == NW_STATE_NO_REGISTERED)
    {
        return FALSE;
    }
    else if (nNwState == NW_STATE_LTE_ACTIVED)
    {
        return TRUE;
    }

    return FALSE;
}

// Activate manually
static BOOL _doActEx(u8 cid)
{
    s32 iResult = 0;

    if ( cid == 1 )
    {
        APP_DEBUG("cid 1 is automatically activated[%d]", __LINE__);
        return FALSE;
    }

    /*get csq*/
    u8 nSignalLevel = 99;
    iResult = MG_NW_GetSignalQuality(SIM0, &nSignalLevel);
    APP_DEBUG("get csq[%d] iResult:%d, nSignalLevel:%d", __LINE__, iResult, nSignalLevel);
    if (iResult != MG_RET_OK)
    {
        return FALSE;
    }
    MG_OS_ThreadSleep(100);

    /*get network state*/
    Enum_NwState nNwState = NW_STATE_NO_REGISTERED;
    iResult = MG_NW_GetNetWorkState(SIM0, cid, &nNwState);
    APP_DEBUG("get network state[%d] iResult:%d, nNwState:%d", __LINE__, iResult, nNwState);
    if (nNwState == NW_STATE_NO_REGISTERED)
    {
        return FALSE;
    }
    else if (nNwState == NW_STATE_LTE_ACTIVED)
    {
        return TRUE;
    }

    /*set default network cofig*/
    iResult = MG_NW_Config(SIM0, cid, NULL);
    APP_DEBUG("cid %d set default network cofig[%d] iResult:%d", cid, __LINE__, iResult);
    if (iResult != MG_RET_OK)
    {
        return FALSE;
    }
    MG_OS_ThreadSleep(100);

    /*do act, timeout 10s*/
    iResult = MG_NW_Activate(SIM0, cid, 10*1000);
    APP_DEBUG("cid %d do act[%d] iResult:%d", cid, __LINE__, iResult);
    if (iResult != MG_RET_OK)
    {
        return FALSE;
    }
    MG_OS_ThreadSleep(100);

    return TRUE;
}

void MG_mqtt_mqtt_incoming_topic_and_data_cb(void *arg, const u8 *topic, u32 topic_len, const u8 *data, u32 date_len)
{
    MG_UTILS_Memset(g_buf, 0, MQTT_BUFFER_MAX_LEN);
    MG_UTILS_Memcpy(g_buf, topic, topic_len);
    APP_DEBUG("mqtt incoming topic line[%d]topic_len:%d, topic:%s", __LINE__, topic_len , g_buf);
    MG_UTILS_Memset(g_buf, 0, MQTT_BUFFER_MAX_LEN);
    MG_UTILS_Memcpy(g_buf, data, date_len);
    APP_DEBUG("mqtt incoming topic line[%d]date_len:%d, date:%s", __LINE__, date_len , g_buf);
}

static void prvThreadEntry(void *param)
{
    s32 iResult = 0;
    BOOL isAct = FALSE;
    u8 nCurcid = 0;
    APP_DEBUG("mgmqtt example entry line[%d]", __LINE__);

    //MG_TRACE_SetLogLevel(MG_TRACE_LOG_LEVEL_VERBOSE);
    MG_OS_ThreadSleep(10000);

    /*do act 1 */
    isAct = FALSE;
    while (!isAct)
    {
        isAct = _doAct();
        APP_DEBUG("_doAct[%d] cid:1, isAct:%d", __LINE__, isAct);
        MG_OS_ThreadSleep(1000);
    }
    /*do act 2 */
    isAct = FALSE;
    while (!isAct)
    {
        isAct = _doActEx(PDP_CID_2);
        APP_DEBUG("_doAct[%d] cid:2, isAct:%d", __LINE__, isAct);
        MG_OS_ThreadSleep(1000);
    }

    /*select current cid*/
    nCurcid = PDP_CID_1; //PDP_CID_2

    /* create mqtt client*/
    ST_MqttClient * MqttClient = NULL;
    ST_MqttClientInfo *MqttClientInfo = NULL;
    do
    {
        MqttClient = MG_MQTT_Create(MqttClientInfo);
        APP_DEBUG("mqtt Create line[%d] MqttClient :%x", __LINE__, MqttClient);
        MG_OS_ThreadSleep(1000);
    }while(!MqttClient);

    //enable tls
    iResult = MG_MQTT_SetOpt(MqttClient, MQTT_OPT_BIND_CID_SET, &nCurcid, sizeof(u8));
    APP_DEBUG("Set OPT cid %d line[%d] iResult :%d", nCurcid, __LINE__, iResult);
    MG_OS_ThreadSleep(1000);

    bool isErr = FALSE;
    bool isConnect = FALSE;
    u32 nPulishMsgCount = 0;
    ST_MqttConnectCtx MqttClientConnectCtx = {0};
    while(1)
    {
        MG_OS_ThreadSleep(1000);
        if(!isConnect)
        {
            /* mqtt Register Subscribe callback */
            iResult = MG_MQTT_RegisterEx(MqttClient,
                        (mqtt_incoming_topic_and_data_cb)MG_mqtt_mqtt_incoming_topic_and_data_cb,
                        NULL);
            if(iResult != MG_RET_OK)
            {
                APP_DEBUG("mqtt Register fail line[%d] iResult:%d", __LINE__, iResult);
                isErr = TRUE;
            }

            /* mqtt connect */
            MG_UTILS_Memset(&MqttClientConnectCtx ,0, sizeof(ST_MqttConnectCtx));
            MqttClientConnectCtx.client_id = "umqtt_client";
            MqttClientConnectCtx.hostname = "mq.tongxinmao.com";
            MqttClientConnectCtx.port = 18830;
            MqttClientConnectCtx.username = NULL;
            MqttClientConnectCtx.password = NULL;
            MqttClientConnectCtx.keep_alive = 300;
            MqttClientConnectCtx.clean_session = 0,

            iResult = MG_MQTT_Connect(MqttClient, &MqttClientConnectCtx, MQTT_CONNECT_TIMEOUT_MS);
            if(iResult != MG_RET_OK)
            {
                APP_DEBUG("mqtt Connect fail line[%d] iResult:%d", __LINE__, iResult);
                continue;
            }
            isConnect = TRUE;

            /* mqtt Subscribe topic*/
            if(isConnect && !isErr)
            {
                iResult = MG_MQTT_Subscribe(MqttClient,
                            (char *)MQTT_CLIENT_SUBSCRIBE_TOPIC,
                            MQTT_QOS_0,
                            MQTT_REQUEST_TIMEOUT_MS);
                if(iResult != MG_RET_OK)
                {
                    APP_DEBUG("mqtt Subscribe fail line[%d] iResult:%d", __LINE__, iResult);
                    isErr = TRUE;
                }
            }
            nPulishMsgCount = 0;
        }

        /* ckeck mqtt connect state*/
        if(isConnect && !isErr)
        {
            if(!MG_MQTT_ClientIsConnected(MqttClient))
            {
                APP_DEBUG("mqtt is disconnected line[%d]", __LINE__);
                isConnect = FALSE;
            }
        }

        /* mqtt publish msg*/
        if(isConnect && !isErr)
        {
            iResult = MG_MQTT_Publish(MqttClient,
                        (char *)MQTT_CLIENT_PUBLISH_TOPIC,
                        (u8 *)MQTT_CLIENT_PUBLISH_MSG,
                        MG_UTILS_Strlen((char *)MQTT_CLIENT_PUBLISH_MSG),
                        MQTT_PUBLISH_DUP_0,
                        MQTT_QOS_2,
                        MQTT_PUBLISH_RETAIN_0,
                        MQTT_REQUEST_TIMEOUT_MS);
            if(iResult != MG_RET_OK)
            {
                APP_DEBUG("mqtt pulish fail line[%d] iResult:%d", __LINE__, iResult);
                isErr = TRUE;
            }
            nPulishMsgCount++;
        }

        /* mqtt Unsubscribe and reconnect when nPulishMsgCount Up to 5*/
        if(isConnect && !isErr)
        {
            if(nPulishMsgCount == 5)
            {
                iResult = MG_MQTT_Unsubscribe(MqttClient,
                            (char *)MQTT_CLIENT_SUBSCRIBE_TOPIC,
                            MQTT_REQUEST_TIMEOUT_MS);
                if(iResult != MG_RET_OK)
                {
                    APP_DEBUG("mqtt Unsubscribe fail line[%d] iResult:%d", __LINE__, iResult);
                    isErr = TRUE;
                }
                isConnect = FALSE;
            }
        }

        APP_DEBUG("mqtt pulish count line[%d] nPulishMsgCount:%d", __LINE__, nPulishMsgCount);

        /*Check whether to reconnect*/
        if(!isConnect || isErr)
        {
            MG_MQTT_Disconnect(MqttClient);
            isErr = FALSE;
            isConnect = FALSE;
        }
    }

    MG_MQTT_Destroy(MqttClient);

    APP_DEBUG("mg example exit line[%d]", __LINE__);
    MG_OS_ThreadSleep(1000);
    MG_OS_ThreadExit();
}

void proc_main_task(void)
{
    APP_DEBUG("mg example enter line[%d]", __LINE__);
    MG_OS_ThreadCreate("mgmqtt", prvThreadEntry, NULL, Thread_PRIORITY_NORMAL, 1024 * 4, 4);
}
