#ifndef __MQTT_PACKET_H_
#define __MQTT_PACKET_H_

#include "mqtt_common.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
	s32 len;
	char* data;
} ST_MCI_MQTTLenString;

typedef struct
{
	char* cstring;
	ST_MCI_MQTTLenString lenstring;
} ST_MCI_MQTTString;

s32 mci_MQTTstrlen(ST_MCI_MQTTString mqttstring);
s32 mci_MQTTPacket_len(s32 rem_len);
void mci_writeChar(u8** pptr, char c);
void mci_writeInt(u8** pptr, s32 anInt);
void mci_writeCString(u8** pptr, const char* string);
void mci_writeMQTTString(unsigned char** pptr, ST_MCI_MQTTString mqttstring);
s32 mci_readInt(unsigned char** pptr);
u8 mci_readChar(unsigned char** pptr);
s32 mci_readMQTTLenString(ST_MCI_MQTTString *mqttstring, u8 **pptr, u8 *enddata);
s32 mci_MQTTPacket_encode(u8* buf, s32 length);
s32 mci_MQTTPacket_decode(unsigned char* buf, s32* value);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif // End-of __MQTT_PACKET_H_
