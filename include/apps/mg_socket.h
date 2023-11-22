/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MeiG Smart Technology Co., Ltd. 2021
*
*****************************************************************************/
/*****************************************************************************
 *
 * Filename:
 * ---------
 *   mg_socket.h
 *
 * Project:
 * --------
 *   OpenCPU
 *
 * Description:
 * ------------
 *   File  API defines.
 *
 *============================================================================
 *             HISTORY
 *----------------------------------------------------------------------------
 *
 ****************************************************************************/

#ifndef _MG_SOCKET_H_
#define _MG_SOCKET_H_
#include "mg_type.h"
#include "mg_ip.h"

typedef struct _timeval {
	u32   tv_sec;
	u32   tv_usec;
}ST_TimeVal;

typedef enum
{
	/** A socket is created but not used */
	CLOSED      = 0,

	/** The socket is listening for a connection */
	LISTEN      = 1,

	/** The socket is trying to actively establish a connection,
	  * that is, it has not received an ACK after sending SYN */
	SYN_SENT    = 2,

	/** The socket is in the initial synchronization state of the connection,
	  * that is, it has received the SYN from the other party,
	  * but has not yet received the ACK of the SYN sent by itself */
	SYN_RCVD    = 3,

	/** Connection established */
	ESTABLISHED = 4,

	/** Socket closed, closing connection, sending FIN,
	  * neither ACK nor FIN received */
	FIN_WAIT_1  = 5,

	/** The socket is closed, waiting for the remote socket to close,
	  * that is, the ACK corresponding to the sent FIN is received in the FIN_WAIT_1 state */
	FIN_WAIT_2  = 6,

	/** The remote socket has been closed, waiting to close this socket,
	  * and the passively closed party receives FIN */
	CLOSE_WAIT  = 7,

	/** The socket is closed, the remote socket is closing, the closing acknowledgement is temporarily pending,
	  * that is, the FIN of the passive party is received in the FIN_WAIT_1 state */
	CLOSING     = 8,

	/** The remote socket is closed, waiting for the closing confirmation of the local socket,
	  * and the passive party sends FIN in the CLOSE_WAIT state */
	LAST_ACK    = 9,

	/** The socket has been closed, waiting for the remote socket to close,
	  * that is, FIN, ACK, FIN, ACK are all finished, and become CLOSED state after 2MSL time */
	TIME_WAIT   = 10
}Enum_SocketState;

typedef enum {
    /**
     *  Maximum number of retransmissions of SYN segments. (u32)
    */
    TCPIP_OPT_SYN_RETRY_MAX,

    TCPIP_OPT_MAX,
}Enum_TcpipOpt;

/******************************************************************************
* Function:     MG_SOCK_Create
*
* Description:
*               This function creates a socket.
*
* Parameters:
*               domain:
*                   [IN] domain Types
*               type:
*                   [IN] Socket protocol types.
*               protocol:
*                   [IN] protocol Type.
* Return:
*               >0, socket Id.
*               Other values, error code.
******************************************************************************/
s32 MG_SOCK_Create(s32 domain, s32 type, s32 protocol);

/******************************************************************************
* Function:     MG_SOCK_BindCid
*
* Description:
*               This function bind cid.
*               if the cid set as NULL, it will bind with deault pdn
*
* Parameters:
*               socketId:
*                   [IN] A socket Id.
*
*               remoteIP:
*                   [IN] A pointer to the cid.
* Return:
*               MG_RET_OK, this function succeeds.
*               Other values, error code.
******************************************************************************/
s32 MG_SOCK_BindCid(s32 socketId, u8 *cid);

/******************************************************************************
* Function:     MG_SOCK_Connect
*
* Description:
*               This function connects to TCP server. The server
*               is specified by an IP address and a port number.
*
* Parameters:
*               socketId:
*                   [IN] A socket Id.
*
*               remoteIP:
*                   [IN] A address pointer to the IP address.
*
*               remotePort:
*                   [IN] Socket port.
* Return:
*               MG_RET_OK, this function succeeds.
*               Other values, error code.
******************************************************************************/
 s32 MG_SOCK_Connect(s32 socketId,ST_ipAddr* remoteIP, u16 remotePort);
 
/******************************************************************************
* Function:     MG_SOCK_Connect
*
* Description:
*               This function closes a socket.
*
* Parameters:
*               socketId:
*                   [IN] A socket Id.
*
* Return:
*               MG_RET_OK, this function succeeds.
*               Other values, error code.
******************************************************************************/
s32 MG_SOCK_Close(s32 socketId);

/******************************************************************************
* Function:     MG_SOCK_Send
*
* Description:
*               The function sends data to a connected TCP socket.
*
* Parameters:
*               socketId:
*                   [IN] Socket Id.
*
*               pData:
*                   [IN] Pointer to the data to send.
*
*               dataLen:
*                   [IN] Number of bytes to send.
* Return:
*               >=0, the total number of bytes sent, which can
*                    be less than the number requested to be sent
*                    in the dataLen parameter
*               Other values, error code.
******************************************************************************/
s32 MG_SOCK_Send(s32 socketId, const u8 *pData, u32 dataLen);

/******************************************************************************
* Function:     MG_SOCK_SendTo
*
* Description:
*               The function sends data to a connected TCP socket.
*
* Parameters:
*               socketId:
*                   [IN] Socket Id.
*               pData:
*                   [IN] Pointer to the data to send.
*               dataLen:
*                   [IN] Number of bytes to send.
*               remoteIP:
*                   [IN] Point to the address of the target socket
*               remotePort:
*                   [IN] The target port number
* Return:
*               >=0, the total number of bytes sent, which can
*                    be less than the number requested to be sent
*                    in the dataLen parameter
*               Other values, error code.
******************************************************************************/
s32 MG_SOCK_SendTo(s32 socketId, const u8* pData, u32 dataLen, ST_ipAddr* remoteIP, u16 remotePort);

/******************************************************************************
* Function:     MG_SOCK_Recv
*
* Description:
*               The function receives data.
*
* Parameters:
*               socketId:
*                   [IN] A socket Id.
*               pBuffer:
*                   [OUT] Point to a buffer that is the storage space
*                        for the received data.
*               bufferLen:
*                   [IN] Length of pData, in bytes.
* Return:
*               >=0, the total number of bytes received.
*               Other values, error code.
******************************************************************************/
s32 MG_SOCK_Recv(s32 socketId, u8* pBuffer, u32 bufferLen);

/******************************************************************************
* Function:     MG_SOCK_Recv
*
* Description:
*               The function receives data.
*
* Parameters:
*               socketId:
*                   [IN] A socket Id.
*               pBuffer:
*                   [OUT] Point to a buffer that is the storage space
*                        for the received data.
*               bufferLen:
*                   [IN] Length of pData, in bytes.
*               remoteIP:
*                   [IN] Point to the address of the target socket
*               remotePort:
*                   [IN] Point to the target port number
* Return:
*               >=0, the total number of bytes received.
*               Other values, error code.
******************************************************************************/
s32 MG_SOCK_RecvFrom(s32 socketId, u8* pBuffer, u32 bufferLen, ST_ipAddr* remoteIP, u16 *remotePort);

/******************************************************************************
* Function:     MG_SOCK_Select
*
* Description:
*               The function listens for changes in the state of one or more sockets
*
* Parameters:
*               maxfdp1:
*                   [IN] Number of the socket to check.
*               readSet:
*                   [IN] the descriptors used to check readability.
*               writeSet:
*                   [IN] the descriptors used to check writability.
*               exceptSet:
*                   [IN] the descriptors used to check abnormal conditions.
*               timeout:
*                   [IN] listening timeout
* Return:
*               >=0, the total number of changed socket
*               Other values, error code.
******************************************************************************/
s32 MG_SOCK_Select(s32 maxfdp1, fd_set *readSet, fd_set *writeSet, fd_set *exceptSet, ST_TimeVal *timeout);

/******************************************************************************
* Function:     MG_SOCK_Bind
*
* Description:
*               This function associates a local address with a socket.
*
* Parameters:
*               socketId:
*                   [IN] A socket Id.
*               remoteIP:
*                   [IN] local IP
*               remotePort:
*                   [IN] local port
* Return:
*               MG_RET_OK, this function succeeds.
*               Other values, error code.
******************************************************************************/
s32 MG_SOCK_Bind(s32 socketId, ST_ipAddr *localIP, u16 localPort);

/******************************************************************************
* Function:     MG_SOCK_Accept
*
* Description:
*               The function permits a connection attempt on a socket.
*
* Parameters:
*               socketId:
*                   [IN] The listened socket id.
*               remoteIP:
*                   [OUT] An optional pointer to a buffer that
*                         receives the address of the connecting entity.
*               remotePort:
*                   [OUT] An optional pointer to an integer that
*                         contains the port number of the connecting entity.
* Return:
*               MG_RET_OK, this function succeeds.
*               Other values, error code.
******************************************************************************/
s32 MG_SOCK_Accept(s32 socketId, ST_ipAddr *remoteIP, u16 *remotePort);

/******************************************************************************
* Function:     MG_SOCK_Listen
*
* Description:
*               The function places a socket in a state in which
*               it is listening for an incoming connection.
*
* Parameters:
*               socketId:
*                   [IN] The listened socket id.
*               backlog:
*                   [IN] backlog (ATTENTION: needs TCP_LISTEN_BACKLOG=1)
* Return:
*               MG_RET_OK, this function succeeds.
*               Other values, error code.
******************************************************************************/
s32 MG_SOCK_Listen(s32 socketId, s32 backlog);


/******************************************************************************
* Function:     MG_IP_ConvertIpAddr
*
* Description:
*               The function convert IP string
*
* Parameters:
*               ipAddrString:
*                   [IN] IP string.
*               ipAddr:
*                   [OUT] the Point to ip struct.
* Return:
*               MG_RET_OK, this function succeeds.
*               Other values, error code.
******************************************************************************/
s32 MG_IP_ConvertIpAddr(const char *ipAddrString, ST_ipAddr* ipAddr);

/******************************************************************************
* Function:     MG_IP_GetIPByHostName
*
* Description:
*               The function gets the IP of the given domain name.
*               The function will blocked until dns sucess/fail.
*
* Parameters:
*               hostName:
*                   [IN] The host name.
*               ipAddr:
*                   [IN] The IP address.
*               SimID:
*                   [IN] The sim card id.
*               Cid:
*                   [IN] The PDP context id
* Return:
*               MG_RET_OK, this function succeeds.
*               Other values, error code.
******************************************************************************/
s32 MG_IP_GetIPByHostName(const char *hostName, ST_ipAddr* ipAddr, u8 SimID, u8 Cid);

/******************************************************************************
* Function:     MG_SOCK_SetOpt
*
* Description:
*               Set the socket options.
*
* Parameters:
*               socketId:
*                       [IN] A socket Id.
*               level:
*                       [IN] the socket option level.
*               optName:
*                       [IN] the socket option name.
*               optVal:
*                       [IN] the pointer to the socket option value.
*               optLen:
*                       [IN] the socket option value length.
* Return:
*               MG_RET_OK, this function succeeds.
*               Other values, error code.
******************************************************************************/
s32 MG_SOCK_SetOpt(s32 socketId, s32 level, s32 optName, const void *optVal, s32 optLen);

/******************************************************************************
* Function:     MG_SOCK_GetOpt
*
* Description:
*               Set the socket options.
*
* Parameters:
*               socketId:
*                       [IN] A socket Id.
*               level:
*                       [IN] the socket option level.
*               optName:
*                       [IN] the socket option name.
*               optVal:
*                       [OUT] the pointer to the socket option value.
*               optLen:
*                       [IN] the socket option value length.
* Return:
*               MG_RET_OK, this function succeeds.
*               Other values, error code.
******************************************************************************/
s32 MG_SOCK_GetOpt(s32 socketId, s32 level, s32 optName, void *optVal, s32* optLen);

/******************************************************************************
* Function:     MG_SOCK_Ioctl
*
* Description:
*               the socket cmd operation.
*
* Parameters:
*               socketId:
*                       [IN] A socket Id.
*               cmd:
*                       [IN] the socket cmd.
*               argp:
*                       [IN] the socket cmd parameter.
* Return:
*               MG_RET_OK, this function succeeds.
*               Other values, error code.
******************************************************************************/
s32 MG_SOCK_Ioctl(s32 socketId, s32 cmd, void * argp);

/******************************************************************************
* Function:     MG_SOCK_GetSocketState
*
* Description:
*               get the socket state.
*
* Parameters:
*               socketId:
*                       [IN] A socket Id.
*               state:
*                       [OUT] the socket state.
* Return:
*               MG_RET_OK, this function succeeds.
*               Other values, error code.
******************************************************************************/
s32 MG_SOCK_GetSocketState(s32 socketId, Enum_SocketState *state);

/******************************************************************************
* Function:     MG_TCPIP_SetOpt
*
* Description:
*               Set the TCPIP options.
*
* Parameters:
*               optName:
*                       [IN] the request option name.
*               optVal:
*                       [IN] the pointer to the request option value.
*               optLen:
*                       [IN] the request option value length.
* Return:
*               MG_RET_OK, this function succeeds.
*               Other values, error code.
******************************************************************************/
s32 MG_TCPIP_SetOpt(Enum_TcpipOpt optName, void *optVal, u32 optLen);

/******************************************************************************
* Function:     MG_TCPIP_GetOpt
*
* Description:
*               Get the TCPIP options.
*
* Parameters:
*               optName:
*                       [IN] the request option name.
*               optVal:
*                       [OUT] the pointer to the request option value.
*               optLen:
*                       [IN] the request option value length.
* Return:
*               MG_RET_OK, this function succeeds.
*               Other values, error code.
******************************************************************************/
s32 MG_TCPIP_GetOpt(Enum_TcpipOpt optName, void *optVal, u32 optLen);

#endif // End-of _MG_SOCKET_H_