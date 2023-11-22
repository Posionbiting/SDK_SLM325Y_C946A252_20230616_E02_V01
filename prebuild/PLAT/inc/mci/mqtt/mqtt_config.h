#ifndef __MQTT_CONFIG_H_
#define __MQTT_CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

/**
 * MQTT_BIND_CID_EN=0: Use the cid of default network card.
 * MQTT_BIND_CID_EN=1: Use the set cid.
 */
#define MQTT_BIND_CID_EN                        (1)

#define MQTT_MAX_PACKET_ID                      (0xFFFF - 1)

#define MQTT_DEFAULT_VERSION                    MQTT_OPT_VERSION_3_1_1

#define MQTT_DEFAULT_KEEP_ALIVE_INTERVAL_SEC    (120)

#define MQTT_DEFAULT_BUF_SIZE                   (4096)

#define MQTT_DEFAULT_YIELD_TIMEOUT_MS           (5000)

#define MQTT_DEFAULT_REQ_LIST_NUM_MAX           (16)

#define MQTT_DEFAULT_PING_REQ_MAX_NUMBRE        (3)

/* thread */
#define MQTT_DEFAULT_THREAD_STACK_SIZE          (4096)

#define MQTT_DEFAULT_THREAD_PRIO                 MCI_Thread_PRIORITY_NORMAL

#define MQTT_DEFAULT_THREAD_EVENT                (4)

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif // End-of __MQTT_CONFIG_H_