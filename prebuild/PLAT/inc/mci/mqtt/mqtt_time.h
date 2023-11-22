#ifndef __MQTT_TIME_H_
#define __MQTT_TIME_H_

#include "mqtt_common.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _MqttTimeCtx
{
    u32 time;
} ST_MqttTimeCtx;

u32 mci_mqtt_time_now_ms(void);

void mci_mqtt_time_init(ST_MqttTimeCtx *tc);

void mci_mqtt_time_set_timeout(ST_MqttTimeCtx *tc, u32 timeout);

s32 mci_mqtt_time_is_expired(ST_MqttTimeCtx *tc);

s32 mci_mqtt_time_remain_ms(ST_MqttTimeCtx *tc);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif // End-of __MQTT_TIME_H_