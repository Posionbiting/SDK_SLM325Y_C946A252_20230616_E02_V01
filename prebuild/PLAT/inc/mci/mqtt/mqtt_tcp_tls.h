

#ifndef __MQTT_TCP_TLS_H_
#define __MQTT_TCP_TLS_H_
#include "mqtt_common.h"

#ifdef __cplusplus
extern "C" {
#endif

#define MQTT_SOCK_TYPE_TCP            0
#define MQTT_SOCK_TYPE_TLS            1

#define MQTT_PDP_CID_MIN              PDP_CID_MIN
#define MQTT_PDP_CID_MAX              PDP_CID_MAX

#define MQTT_INVALID_CID              0xFF

#define MQTT_GetCurrentSimId()        (SIM0)

#define MQTT_GetIPByHostName(hostName, ipAddr, SimID, Cid)                 MG_IP_GetIPByHostName(hostName, ipAddr, SimID, Cid)

#define MQTT_TcpCreate(domain, type, protocol)                             MG_SOCK_Create(domain, type, protocol)
#define MQTT_TcpConnect(socketId, remoteIP, remotePort)                    MG_SOCK_Connect(socketId, remoteIP, remotePort)
#define MQTT_TcpClose(socketId)                                            MG_SOCK_Close(socketId)
#define MQTT_TcpSend(socketId, pData, dataLen)                             MG_SOCK_Send(socketId, pData, dataLen)
#define MQTT_TcpRecv(socketId, pBuffer, bufferLen)                         MG_SOCK_Recv(socketId, pBuffer, bufferLen)
#define MQTT_TcpSelect(maxfdp1, readSet, writeSet, exceptSet, timeout)     MG_SOCK_Select(maxfdp1, readSet, writeSet, exceptSet, timeout)
#define MQTT_TcpSetSockOpt(socketId, level, optName, optVal, optLen)       MG_SOCK_SetOpt(socketId, level, optName, optVal, optLen)
#define MQTT_TcpGetSockOpt(socketId, level, optName, optVal, optLen)       MG_SOCK_GetOpt(socketId, level, optName, optVal, optLen)
#define MQTT_TcpIoctl(socketId, cmd, argp)                                 MG_SOCK_Ioctl(socketId, cmd, argp)

#define MQTT_TcpisConnect(socketId, type, rc)       \
    do                                              \
    {                                               \
        Enum_SocketState __state = CLOSED;          \
        MG_SOCK_GetSocketState(socketId, &__state); \
        *(rc) = (__state == ESTABLISHED) ? ((type)1) : ((type)0);  \
    } while (0)

#if MQTT_BIND_CID_EN
#define MQTT_TcpBindCid(socketId, cid)                                     MG_SOCK_BindCid(socketId, cid)
#define MQTT_TLS_OPT_CID                                                   SOCK_SSL_CID
#else
#define MQTT_TcpBindCid(socketId, cid)
#define MQTT_TLS_OPT_CID                                                   0xFF
#endif

#define MQTT_MCI_OPT_CLIENT_TLS_VERIFY                                     0
#define MQTT_MCI_OPT_BIND_CID_SET                                          1

typedef ST_TimeVal  ST_MqttTimeVal;
typedef ST_SslSock  ST_MqttTlsSock;
typedef ST_SslInfo  ST_MqttTlsInfo;
static inline ST_MqttTlsSock *MQTT_TlsCreate(ST_MqttTlsInfo *info)
{
    return (ST_MqttTlsSock *)MG_SOCK_SSL_Create((ST_SslInfo *)info);
}

static inline s32 MQTT_TlsDestroy(ST_MqttTlsSock *sock)
{
    return MG_SOCK_SSL_Destroy((ST_SslSock *)sock);
}

static inline s32 MQTT_TlsConnect(ST_MqttTlsSock *sock, const char *hostName, u16 port, s32 timeout)
{
    return MG_SOCK_SSL_Connect((ST_SslSock *)sock, hostName, port, timeout);
}

static inline s32 MQTT_TlsDisconnect(ST_MqttTlsSock *sock)
{
    return MG_SOCK_SSL_Disconnect((ST_SslSock *)sock);
}

static inline s32 MQTT_TlsWrite(ST_MqttTlsSock *sock, const u8 *pData, u32 dataLen)
{
    return MG_SOCK_SSL_Write((ST_SslSock *)sock, pData, dataLen);
}

static inline s32 MQTT_TlsRead(ST_MqttTlsSock *sock, u8 *pData, u32 size)
{
    return MG_SOCK_SSL_Read((ST_SslSock *)sock, pData, size);
}

static inline s32 MQTT_TlsGetSocketId(ST_MqttTlsSock *sock)
{
    return MG_SOCK_SSL_GetSocketId((ST_SslSock *)sock);
}

static inline s32 MQTT_TlsSetOpt(ST_MqttTlsSock *sock, s32 optName, void *optVal, u32 optLen)
{
    return MG_SOCK_SSL_SetOpt((ST_SslSock *)sock, optName, optVal, optLen);
    // return MCI_RET_OK;
}

static inline s32 MQTT_TlsGetOpt(ST_MqttTlsSock *sock, s32 optName, void *optVal, u32 optLen)
{
    return MG_SOCK_SSL_GetOpt((ST_SslSock *)sock, optName, optVal, optLen);
    // return MCI_RET_OK;
}


typedef s32 (*MqttNetCallbackConnect)(void *sock, const char *hostName, u16 port, s32 timeout);
typedef void (*MqttNetCallbackDisconnect)(void *sock);
typedef s32 (*MqttNetCallbackSend)(void *sock, const u8 *pdata, u32 DataLen, s32 timeout);
typedef s32 (*MqttNetCallbackRecv)(void *sock, u8 *buffer, u32 bufferLen, s32 timeout);
typedef s32 (*MqttNetCallbackGetSocketId)(void *sock);
typedef s32 (*MqttNetCallbackSetOpt)(void *sock, s32 optName, void *optVal, u32 optLen);

typedef struct _mqttNetCallback
{
    MqttNetCallbackConnect onConnect;
    MqttNetCallbackDisconnect onDisconnect;
    MqttNetCallbackSend onSend;
    MqttNetCallbackRecv onRecv;
    MqttNetCallbackGetSocketId onGetSocketId;
    MqttNetCallbackSetOpt onSetOpt;
} mqttNetCallback;

typedef struct _SockTcp
{
    s32 sock;
    u8 cid;
}ST_SockTcp;

typedef struct _SockTls
{
    ST_MqttTlsSock *sock;
    u8 cid;
}ST_SockTls;

typedef struct _mqttSock
{
  u8 type;
  union {
    ST_SockTcp tcp;
    ST_SockTls tls;
  } u_sock;
  mqttNetCallback handler;
} mqttSock;

/**
 * \brief  This function new tcp context.
 */
mqttSock *MqttNetSocketStreamNew(u8 cid);

/**
 * \brief  This function delete tcp context.
 */
void MqttNetSocketStreamDelete(mqttSock *sock);

/**
 * \brief  This function new tls context.
 */
mqttSock *MqttNetSocketTlsNew(ST_MqttTlsInfo *info, u8 cid);

/**
 * \brief  This function delete tls context.
 */
void MqttNetSocketTlsDelete(mqttSock *sock);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif //End-of __MQTT_TCP_TLS_H_