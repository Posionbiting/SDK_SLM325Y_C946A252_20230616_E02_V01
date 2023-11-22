

#ifndef _HTTP_TCP_TLS_H_
#define _HTTP_TCP_TLS_H_
#include "http_common.h"

#ifdef __cplusplus
extern "C" {
#endif

#define HTTP_SOCK_TYPE_TCP            0
#define HTTP_SOCK_TYPE_TLS            1

#define HTTP_PDP_CID_MIN              PDP_CID_MIN
#define HTTP_PDP_CID_MAX              PDP_CID_MAX

#define HTTP_INVALID_CID              0xFF

#define HTTP_GetCurrentSimId()        (SIM0)

#define HTTP_GetIPByHostName(hostName, ipAddr, SimID, Cid)                 MG_IP_GetIPByHostName(hostName, ipAddr, SimID, Cid)


#define HTTP_TcpCreate(domain, type, protocol)                             MG_SOCK_Create(domain, type, protocol)
#define HTTP_TcpConnect(socketId, remoteIP, remotePort)                    MG_SOCK_Connect(socketId, remoteIP, remotePort)
#define HTTP_TcpClose(socketId)                                            MG_SOCK_Close(socketId)
#define HTTP_TcpSend(socketId, pData, dataLen)                             MG_SOCK_Send(socketId, pData, dataLen)
#define HTTP_TcpRecv(socketId, pBuffer, bufferLen)                         MG_SOCK_Recv(socketId, pBuffer, bufferLen)
#define HTTP_TcpSelect(maxfdp1, readSet, writeSet, exceptSet, timeout)     MG_SOCK_Select(maxfdp1, readSet, writeSet, exceptSet, timeout)
#define HTTP_TcpSetSockOpt(socketId, level, optName, optVal, optLen)       MG_SOCK_SetOpt(socketId, level, optName, optVal, optLen)
#define HTTP_TcpGetSockOpt(socketId, level, optName, optVal, optLen)       MG_SOCK_GetOpt(socketId, level, optName, optVal, optLen)
#define HTTP_TcpIoctl(socketId, cmd, argp)                                 MG_SOCK_Ioctl(socketId, cmd, argp)

#if HTTP_BIND_CID_EN
#define HTTP_TcpBindCid(socketId, cid)                                     MG_SOCK_BindCid(socketId, cid)
#define HTTP_TLS_OPT_CID                                                   SOCK_SSL_CID
#else
#define HTTP_TcpBindCid(socketId, cid)
#define HTTP_TLS_OPT_CID                                                   0xFF
#endif

typedef ST_TimeVal  ST_HttpTimeVal;
typedef ST_SslSock  ST_HttpTlsSock;
typedef ST_SslInfo  ST_HttpTlsInfo;
static inline ST_HttpTlsSock *HTTP_TlsCreate(ST_HttpTlsInfo *info)
{
    return (ST_HttpTlsSock *)MG_SOCK_SSL_Create((ST_SslInfo *)info);
}

static inline s32 HTTP_TlsDestroy(ST_HttpTlsSock *sock)
{
    return MG_SOCK_SSL_Destroy((ST_SslSock *)sock);
}

static inline s32 HTTP_TlsConnect(ST_HttpTlsSock *sock, const char *hostName, u16 port, s32 timeout)
{
    return MG_SOCK_SSL_Connect((ST_SslSock *)sock, hostName, port, timeout);
}

static inline s32 HTTP_TlsDisconnect(ST_HttpTlsSock *sock)
{
    return MG_SOCK_SSL_Disconnect((ST_SslSock *)sock);
}

static inline s32 HTTP_TlsWrite(ST_HttpTlsSock *sock, const u8 *pData, u32 dataLen)
{
    return MG_SOCK_SSL_Write((ST_SslSock *)sock, pData, dataLen);
}

static inline s32 HTTP_TlsRead(ST_HttpTlsSock *sock, u8 *pData, u32 size)
{
    return MG_SOCK_SSL_Read((ST_SslSock *)sock, pData, size);
}

static inline s32 HTTP_TlsGetSocketId(ST_HttpTlsSock *sock)
{
    return MG_SOCK_SSL_GetSocketId((ST_SslSock *)sock);
}

static inline s32 HTTP_TlsSetOpt(ST_HttpTlsSock *sock, s32 optName, void *optVal, u32 optLen)
{
    return MG_SOCK_SSL_SetOpt((ST_SslSock *)sock, optName, optVal, optLen);
    // return MCI_RET_OK;
}

static inline s32 HTTP_TlsGetOpt(ST_HttpTlsSock *sock, s32 optName, void *optVal, u32 optLen)
{
    return MG_SOCK_SSL_GetOpt((ST_SslSock *)sock, optName, optVal, optLen);
    // return MCI_RET_OK;
}


typedef s32 (*HttpNetCallbackConnect)(void *sock, const char *hostName, u16 port, s32 timeout);
typedef void (*HttpNetCallbackDisconnect)(void *sock);
typedef s32 (*HttpNetCallbackSend)(void *sock, const u8 *pdata, u32 DataLen, s32 timeout);
typedef s32 (*HttpNetCallbackRecv)(void *sock, u8 *buffer, u32 bufferLen, s32 timeout);
typedef s32 (*HttpNetCallbackGetSocketId)(void *sock);

typedef struct _httpNetCallback
{
    HttpNetCallbackConnect onConnect;
    HttpNetCallbackDisconnect onDisconnect;
    HttpNetCallbackSend onSend;
    HttpNetCallbackRecv onRecv;
    HttpNetCallbackGetSocketId onGetSocketId;
} httpNetCallback;

typedef struct _SockTcp
{
    s32 sock;
    u8 cid;
}ST_SockTcp;

typedef struct _SockTls
{
    ST_HttpTlsSock *sock;
    u8 cid;
}ST_SockTls;

typedef struct _httpSock
{
  u8 type;
  union {
    ST_SockTcp tcp;
    ST_SockTls tls;
  } u_sock;
  httpNetCallback handler;
} httpSock;

/**
 * \brief  This function new tcp context.
 */
httpSock *HttpNetSocketStreamNew(u8 cid);

/**
 * \brief  This function delete tcp context.
 */
void HttpNetSocketStreamDelete(httpSock *sock);

/**
 * \brief  This function new tls context.
 */
httpSock *HttpNetSocketTlsNew(ST_HttpTlsInfo *info, u8 cid);

/**
 * \brief  This function delete tls context.
 */
void HttpNetSocketTlsDelete(httpSock *sock);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif //End-of _HTTP_TCP_TLS_H_