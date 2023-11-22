/**
  ******************************************************************************
  * @file    mg_mqtt.h
  * @author  MEIG Development Team
  * @version V1.0.0
  * @date    28-Mar-2022
  * @brief   This file provides all the http function APIs.
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

#ifndef _MG_MQTT_H_
#define _MG_MQTT_H_
#include "mg_type.h"

#ifdef __cplusplus
extern "C" {
#endif

#define MQTT_CLIENT_ID_MAXLEN (256)
#define MQTT_HOSTNAME_MAXLEN (128)
#define MQTT_USERNAME_MAXLEN (256)
#define MQTT_PASSWORD_MAXLEN (256)
#define MQTT_TOPIC_MAXLEN (512)

#define MQTT_QOS_0 (0)
#define MQTT_QOS_1 (1)
#define MQTT_QOS_2 (2)

#define MQTT_PUBLISH_DUP_0 (0)
#define MQTT_PUBLISH_DUP_1 (1)

#define MQTT_PUBLISH_RETAIN_0 (0)
#define MQTT_PUBLISH_RETAIN_1 (1)

#define MQTT_OPT_CLIENT_DISABLE (0)
#define MQTT_OPT_CLIENT_ENABLE  (1)

#define MQTT_OPT_CLIENT_TLS_VERIFY_NONE     (0)
#define MQTT_OPT_CLIENT_TLS_VERIFY_OPTIONAL (1)
#define MQTT_OPT_CLIENT_TLS_VERIFY_REQUIRED (2)

#define MQTT_TLS_VERSION_SSL3_0   (0)
#define MQTT_TLS_VERSION_TLS1_0   (1)
#define MQTT_TLS_VERSION_TLS1_1   (2)
#define MQTT_TLS_VERSION_TLS1_2   (3)
#define MQTT_TLS_VERSION_MAX      (4)

typedef enum
{
    MQTT_STATE_IDLE         = 0,
    MQTT_STATE_DISCONNECT,
    MQTT_STATE_CONNECT,
    MQTT_STATE_DISCONNECTING,
    MQTT_STATE_CLEAN_SESSION
}Enum_MqttWorkState;

typedef struct
{
    /** will_topic, set to NULL if will is not to be used,
      will_msg, will_qos and will_retain are then ignored */
    char *will_topic;
    /** will_msg, see will_topic*/
    char *will_msg;
    /** will_qos, see will_topic, range(0~2)*/
    u8 will_qos;
    /** will_retain, see will_topic, range(0~1)*/
    u8 will_retain;
}ST_MqttClientWill;

typedef enum {

    /**  whether to enable tls
     *   type : u8
     *   default : MQTT_OPT_CLIENT_DISABLE
    */
    MQTT_OPT_CLIENT_TLS = 0,

    /** config tls verify type
     *  type : u8
    */
    MQTT_OPT_CLIENT_TLS_VERIFY,

    /** whether to enable will
     *  type : u8
     *  default : MQTT_OPT_CLIENT_DISABLE
    */
    MQTT_OPT_LAST_WILL_ENABLE,

    /** set will
     *  type : ST_MqttClientWill
    */
    MQTT_OPT_LAST_WILL_SET,

    /** set cid
     *  type : u8
    */
    MQTT_OPT_BIND_CID_SET,

    /** set send buffer size (topic + msg)
     *  type : u32
     *  default : 4096
    */
    MQTT_OPT_SEND_BUFFER_SET,

    /** set receive buffer size (topic + msg)
     *  type : u32
     *  default : 4096
    */
    MQTT_OPT_RECEVICE_BUFFER_SET,

    MQTT_OPT_MAX,
} Enum_MqttOpt;

typedef struct
{
    /** Client identifier, must be set by caller */
    char *client_id;
    /** Server domain name*/
    char *hostname;
    /** Server port*/
    u16 port;
    /** User name, set to NULL if not used */
    char* username;
    /** Password, set to NULL if not used */
    char* password;
    /** keep alive time in seconds, 0 to disable keep alive functionality*/
    u16 keep_alive;
    /** Processing flag for session state, range(0~1)*/
    u8 clean_session;
}ST_MqttConnectCtx;

typedef struct
{
    /** tls version*/
    s32 vsn;
    /** ca certificate*/
    char *cacert;
    /** client certificate*/
    char *clicert;
    /** client key*/
    char *pkey;
}ST_MqttClientInfo;

typedef struct _MqttClient ST_MqttClient;
typedef void (*mqtt_incoming_data_cb)(void *arg, const u8 *data, u16 len, u8 flags);
typedef void (*mqtt_incoming_topic_cb)(void *arg, const char *topic, u32 len);
typedef void (*mqtt_incoming_topic_and_data_cb)(void *arg, const u8 *topic, u32 topic_len, const u8 *data, u32 date_len);

/******************************************************************************
* Function:     MG_MQTT_GetClientState
*
* Description:
*               get mqtt client state.
*
* Parameters:
*               client:
*                       [IN] mqtt context.
* Return:
*               Enum_MqttWorkState
******************************************************************************/
Enum_MqttWorkState MG_MQTT_GetClientState(ST_MqttClient *client);

/******************************************************************************
* Function:     MG_MQTT_Create
*
* Description:
*               Create mqtt context.
*
* Parameters:
*               info:
*                       [IN] mqtt initial parameter configuration.
* Return:
*               If no error occurs, MG_MQTT_Create returns the mqtt
*               context. Otherwise, returns NULL.
******************************************************************************/
ST_MqttClient *MG_MQTT_Create(ST_MqttClientInfo *info);

/******************************************************************************
* Function:     MG_MQTT_Destroy
*
* Description:
*               Destroy mqtt context.
*
* Parameters:
*               client:
*                       [IN] mqtt context.
* Return:
*               MG_RET_OK, this function succeeds.
*               Other values, error code.
******************************************************************************/
s32 MG_MQTT_Destroy(ST_MqttClient *client);

/******************************************************************************
* Function:     MG_MQTT_SetOpt
*
* Description:
*               Set the mqtt options.
*
* Parameters:
*               client:
*                       [IN] the mqtt context.
*               optName:
*                       [IN] the mqtt option name.
*               optVal:
*                       [IN] the pointer to the mqtt option value.
*               optLen:
*                       [IN] the mqtt option value length.
* Return:
*               MG_RET_OK, this function succeeds.
*               Other values, error code.
******************************************************************************/
s32 MG_MQTT_SetOpt(ST_MqttClient *client, Enum_MqttOpt optName, void *optVal, u32 optLen);

/******************************************************************************
* Function:     MG_MQTT_Connect
*
* Description:
*               mqtt connect.
*
* Parameters:
*               client:
*                       [IN] the mqtt context.
*               ctx:
*                       [IN] connect parameter.
*               timeout:
*                       [IN] timeout in milliseconds.
* Return:
*               MG_RET_OK, this function succeeds.
*               Other values, error code.
******************************************************************************/
s32 MG_MQTT_Connect(ST_MqttClient *client, ST_MqttConnectCtx *ctx, s32 timeout);

/******************************************************************************
* Function:     MG_MQTT_Disconnect
*
* Description:
*               mqtt disconnect.
*
* Parameters:
*               client:
*                       [IN] the mqtt context.
* Return:
*               MG_RET_OK, this function succeeds.
*               Other values, error code.
******************************************************************************/
s32 MG_MQTT_Disconnect(ST_MqttClient* client);

/******************************************************************************
* Function:     MG_MQTT_Register
*
* Description:
*               mqtt register.
*
* Parameters:
*               client:
*                       [IN] the mqtt context.
*               topicCb:
*                       [IN] incoming topic Callback of subscribed topics.
*               dataCb:
*                       [IN] incoming data Callback of subscribed topics.
*               arg:
*                       [IN] Callback Parameter.
* Return:
*               MG_RET_OK, this function succeeds.
*               Other values, error code.
******************************************************************************/
s32 MG_MQTT_Register(ST_MqttClient* client, mqtt_incoming_topic_cb topicCb, mqtt_incoming_data_cb dataCb, void *arg);

/******************************************************************************
* Function:     MG_MQTT_RegisterEx
*
* Description:
*               the function is an extension of the MG_MQTT_Register().
*
* Parameters:
*               client:
*                       [IN] the mqtt context.
*               topicDateCb:
*                       [IN] incoming topic and date Callback of subscribed topics.
*               arg:
*                       [IN] Callback Parameter.
* Return:
*               MG_RET_OK, this function succeeds.
*               Other values, error code.
******************************************************************************/
s32 MG_MQTT_RegisterEx(ST_MqttClient* client, mqtt_incoming_topic_and_data_cb topicDateCb, void *arg);

/******************************************************************************
* Function:     MG_MQTT_Subscribe
*
* Description:
*               mqtt subscribe.
*
* Parameters:
*               client:
*                       [IN] the mqtt context.
*               topic:
*                       [IN] the topic.
*               qos:
*                       [IN] Service quality.
*               timeout:
*                       [IN] timeout in milliseconds.
* Return:
*               MG_RET_OK, this function succeeds.
*               Other values, error code.
******************************************************************************/
s32 MG_MQTT_Subscribe(ST_MqttClient* client, const char *topic, u8 qos, s32 timeout);

/******************************************************************************
* Function:     MG_MQTT_Unsubscribe
*
* Description:
*               mqtt unsubscribe.
*
* Parameters:
*               client:
*                       [IN] the mqtt context.
*               topic:
*                       [IN] the topic.
*               timeout:
*                       [IN] timeout in milliseconds.
* Return:
*               MG_RET_OK, this function succeeds.
*               Other values, error code.
******************************************************************************/
s32 MG_MQTT_Unsubscribe(ST_MqttClient* client, const char *topic, s32 timeout);

/******************************************************************************
* Function:     MG_MQTT_Publish
*
* Description:
*               mqtt publish.
*
* Parameters:
*               client:
*                       [IN] the mqtt context.
*               topic:
*                       [IN] the topic.
*               msg:
*                       [IN] The message to be sent.
*               msgLen:
*                       [IN] the message length.
*               dup:
*                       [IN] 0 : First send an mqtt publish packet.
*                            1 ：the publish packet that was previously
*                                attempted to be sent.
*               qos:
*                       [IN] Service quality.
*               retain:
*                       [IN] 0 : broker will not retain the message
*                            1 ：broker will retain the message
*               timeout:
*                       [IN] timeout in milliseconds.
* Return:
*               MG_RET_OK, this function succeeds.
*               Other values, error code.
******************************************************************************/
s32 MG_MQTT_Publish(ST_MqttClient* client, const char *topic, const u8 *msg, u32 msgLen, u8 dup, u8 qos, u8 retain, s32 timeout);

/******************************************************************************
* Function:     MG_MQTT_ClientIsConnected
*
* Description:
*               mqtt connect state.
*
* Parameters:
*               client:
*                       [IN] the mqtt context.
* Return:
*               TRUE, Connection status.
*               FALSE, Non connected state.
******************************************************************************/
bool MG_MQTT_ClientIsConnected(ST_MqttClient* client);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif // End-of _MG_MQTT_H_