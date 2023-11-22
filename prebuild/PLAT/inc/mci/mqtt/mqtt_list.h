#ifndef __MQTT_LIST_H_
#define __MQTT_LIST_H_

#include "mqtt_common.h"
#include "mqtt_packet.h"
#include "mqtt_client.h"
#include "mqtt_time.h"

#ifdef __cplusplus
extern "C" {
#endif

#define MCI_MQTT_LIST_TYPE_INVAILD         0
#define MCI_MQTT_LIST_TYPE_REQ_PENDING     1
#define MCI_MQTT_LIST_TYPE_MAX             2

typedef struct _MciMqttReqPendDate {
    /** Callback to upper layer */
    mqtt_request_cb_t cb;
    void *arg;
    /**  req time */
    ST_MqttTimeCtx time;
} ST_MciMqttReqPendDate;

/**
 * \brief Container for mqtt list.
 */
typedef struct _MciMqttList {
    UNITY_LIST_STRUCT_MEMBERS

    u8 type;

    /** MQTT packet identifier */
    u32 pkt_id;

    ST_MciMqttReqPendDate reqPendDate;

} ST_MciMqttListNode, ST_MciMqttList;

typedef void (*mqtt_list_scan_handle)(void *arg, ST_MciMqttListNode *node);

ST_MciMqttList *mci_mqtt_list_new(u8 type);

s32 mci_mqtt_list_insert(ST_MciMqttList *list, u8 type, u32 pkt_id, void *content);

ST_MciMqttListNode *mci_mqtt_list_find(ST_MciMqttList *list, u8 type, u32 pkt_id);

void mci_mqtt_list_node_delete(ST_MciMqttListNode *node);

s32 mci_mqtt_list_remove(ST_MciMqttList *list, u8 type, u32 pkt_id);

void mci_mqtt_list_clear(ST_MciMqttList *list);

void mci_mqtt_list_delete(ST_MciMqttList *list);

void mci_mqtt_list_scan(ST_MciMqttList *list, u8 type, mqtt_list_scan_handle scan_cb, void *arg);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif // End-of __MQTT_LIST_H_