#ifndef __MQTT_CLIENT_H_
#define __MQTT_CLIENT_H_

#include "mqtt_common.h"
#include "mqtt_packet.h"

#ifdef __cplusplus
extern "C" {
#endif

#define MQTT_OPT_VERSION_3_1                  3
#define MQTT_OPT_VERSION_3_1_1                4

#define MQTT_ACK_SUBFAIL                      0x80

//#define MCI_PACKET_DATA_REVERSED

enum msgTypes
{
    MCI_MQTT_CONNECT = 1,
    MCI_MQTT_CONNACK,
    MCI_MQTT_PUBLISH,
    MCI_MQTT_PUBACK,
    MCI_MQTT_PUBREC,
    MCI_MQTT_PUBREL,
    MCI_MQTT_PUBCOMP,
    MCI_MQTT_SUBSCRIBE,
    MCI_MQTT_SUBACK,
    MCI_MQTT_UNSUBSCRIBE,
    MCI_MQTT_UNSUBACK,
    MCI_MQTT_PINGREQ,
    MCI_MQTT_PINGRESP,
    MCI_MQTT_DISCONNECT
};

/**
 * Bitfields for the MQTT header byte.
 */
typedef union
{
	unsigned char byte;	                /**< the whole byte */
#if defined(MCI_PACKET_DATA_REVERSED)
	struct
	{
		unsigned int type : 4;			/**< message type nibble */
		unsigned int dup : 1;				/**< DUP flag bit */
		unsigned int qos : 2;				/**< QoS value, 0, 1 or 2 */
		unsigned int retain : 1;		/**< retained flag bit */
	} bits;
#else
	struct
	{
		unsigned int retain : 1;		/**< retained flag bit */
		unsigned int qos : 2;				/**< QoS value, 0, 1 or 2 */
		unsigned int dup : 1;				/**< DUP flag bit */
		unsigned int type : 4;			/**< message type nibble */
	} bits;
#endif
} ST_MCI_MQTTHeader;

typedef union
{
	unsigned char all;	/**< all connect flags */
#if defined(MCI_PACKET_DATA_REVERSED)
	struct
	{
		unsigned int username : 1;			/**< 3.1 user name */
		unsigned int password : 1; 			/**< 3.1 password */
		unsigned int willRetain : 1;		/**< will retain setting */
		unsigned int willQoS : 2;				/**< will QoS value */
		unsigned int will : 1;			    /**< will flag */
		unsigned int cleansession : 1;	  /**< clean session flag */
		unsigned int : 1;	  	          /**< unused */
	} bits;
#else
	struct
	{
		unsigned int : 1;	     					/**< unused */
		unsigned int cleansession : 1;	  /**< cleansession flag */
		unsigned int will : 1;			    /**< will flag */
		unsigned int willQoS : 2;				/**< will QoS value */
		unsigned int willRetain : 1;		/**< will retain setting */
		unsigned int password : 1; 			/**< 3.1 password */
		unsigned int username : 1;			/**< 3.1 user name */
	} bits;
#endif
} ST_MCI_MQTTConnectFlags;	/**< connect flags byte */

typedef union
{
	unsigned char all;	/**< all connack flags */
#if defined(MCI_PACKET_DATA_REVERSED)
	struct
	{
        unsigned int reserved : 7;	  	    /**< unused */
        unsigned int sessionpresent : 1;    /**< session present flag */
	} bits;
#else
	struct
	{
        unsigned int sessionpresent : 1;    /**< session present flag */
        unsigned int reserved: 7;	     			/**< unused */
	} bits;
#endif
} ST_MCI_MQTTConnackFlags;	/**< connack flags byte */

typedef struct
{
    s32 mqttVsn;

    ST_MCI_MQTTString clientID;

    ST_MCI_MQTTString username;

    ST_MCI_MQTTString password;

    u16 keepAliveInterval;

    u8 cleansession;

    u8 willFlag;

    /** will_topic, set to NULL if will is not to be used,
         will_msg, will_qos and will_retain are then ignored */
    ST_MCI_MQTTString will_topic;
    /** will_msg, see will_topic*/
    ST_MCI_MQTTString will_msg;
    /** will_qos, see will_topic, range(0~2)*/
    u8 will_qos;
    /** will_retain, see will_topic, range(0~1)*/
    u8 will_retain;
} ST_MCI_MQTTPacket_connectData;


/**
 * @ingroup mqtt
 * Function prototype for mqtt connection status callback. Called when
 * client has connected to the server after initiating a mqtt connection attempt by
 * calling mqtt_client_connect() or when connection is closed by server or an error
 *
 * @param arg Additional argument to pass to the callback function
 * @param err MCI_RET_OK on success
 *
 */
typedef void (*mqtt_connection_cb_t)(void *arg, s32 err);

/**
 * @ingroup mqtt
 * Function prototype for mqtt request callback. Called when a subscribe, unsubscribe
 * or publish request has completed
 * @param arg Pointer to user data supplied when invoking request
 * @param err MCI_RET_OK on success
 */
typedef void (*mqtt_request_cb_t)(void *arg, s32 err);


s32 mci_MQTTSerialize_connect(u8* buf, u32 buflen, ST_MCI_MQTTPacket_connectData* options);

s32 mci_MQTTDeserialize_connack(u8 *sessionPresent, u8* connack_rc, u8* buf, u32 buflen);

s32 mci_MQTTSerialize_disconnect(u8 *buf, u32 buflen);

s32 mci_MQTTSerialize_pingreq(u8 *buf, u32 buflen);

s32 mci_MQTTSerialize_publish(u8* buf, u32 buflen, u8 dup, u8 qos, u8 retained, u16 packetid,
        ST_MCI_MQTTString topicName, u8 *payload, s32 payloadlen);

s32 mci_mqtt_set_publish_dup(u8* buf, u8 dup);

s32 mci_MQTTSerialize_ack(u8* buf, u32 buflen, u8 packettype, u8 dup, u16 packetid);

s32 mci_MQTTSerialize_puback(u8* buf, u32 buflen, u16 packetid);

s32 mci_MQTTSerialize_pubrel(u8* buf, u32 buflen, u8 packettype, u8 dup, u16 packetid);

s32 mci_MQTTSerialize_pubcomp(u8* buf, u32 buflen, u16 packetid);

s32 mci_MQTTDeserialize_ack(u8* packettype, u8* dup, u16* packetid, u8* buf, u32 buflen);

s32 mci_MQTTDeserialize_publish(u8* dup, u8* qos, u8* retained, u16* packetid, ST_MCI_MQTTString* topicName,
        u8** payload, u32* payloadlen, u8* buf, u32 buflen);

s32 mci_MQTTSerialize_subscribe(u8* buf, u32 buflen, u8 dup, u16 packetid, s32 count,
        ST_MCI_MQTTString topicFilters[], s32 requestedQoSs[]);

s32 mci_MQTTDeserialize_suback(u16 *packetid, s32 maxcount, s32 *count, s32 grantedQoSs[], u8* buf, u32 buflen);

s32 mci_MQTTSerialize_unsubscribe(u8* buf, u32 buflen, u8 dup, u16 packetid,
        s32 count, ST_MCI_MQTTString topicFilters[]);

s32 mci_MQTTDeserialize_unsuback(u16 *packetid, u8* buf, u32 buflen);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif // End-of __MQTT_CLIENT_H_
