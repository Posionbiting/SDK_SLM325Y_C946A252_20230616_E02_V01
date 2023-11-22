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
 *   mg_opt_socket.h
 *
 * Project:
 * --------
 *   OpenCPU
 *
 * Description:
 * ------------
 *  OpenCPU Type Definitions
 *
 *============================================================================
 *             HISTORY
 *----------------------------------------------------------------------------
 *
 ****************************************************************************/

#ifndef __MG_OPT_SOCKET_H__
#define __MG_OPT_SOCKET_H__

#include LWIP_CONFIG_FILE

/**
 * macro for socket
 */
/*Number of socket connections*/
#define NUM_NETCONN MEMP_NUM_NETCONN

/* domain Type */
#define AF_INET         2
#define AF_INET6        10

/* Socket protocol types (TCP/UDP/RAW) */
#define SOCK_STREAM     1
#define SOCK_DGRAM      2
#define SOCK_RAW        3

/* protocol Type */
#define IPPROTO_IP      0
#define IPPROTO_ICMP    1
#define IPPROTO_TCP     6
#define IPPROTO_UDP     17
#define IPPROTO_IPV6    41
#define IPPROTO_ICMPV6  58
#define IPPROTO_RAW     255

/* Level number*/
#define  SOL_SOCKET  0xfff    /* options for socket level */

/* Option flags*/
#define SO_REUSEADDR    0x0004 /* Allow local address reuse */
#define SO_KEEPALIVE    0x0008 /* keep connections alive */
#define SO_LINGER       0x0080 /* linger on close if data present */
#define SO_RCVBUF       0x1002 /* receive buffer size */
#define SO_SNDTIMEO     0x1005 /* send timeout */
#define SO_RCVTIMEO     0x1006 /* receive timeout */
#define SO_ERROR        0x1007 /* get error status and clear */
#define SO_TYPE         0x1008 /* get socket type */

/*Options for level IPPROTO_TCP*/
#define TCP_NODELAY    0x01    /* don't delay send to coalesce packets */
#define TCP_KEEPALIVE  0x02    /* send KEEPALIVE probes when idle for pcb->keep_idle milliseconds */
#define TCP_KEEPIDLE   0x03    /* set pcb->keep_idle  - Same as TCP_KEEPALIVE, but use seconds for get/setsockopt */
#define TCP_KEEPINTVL  0x04    /* set pcb->keep_intvl - Use seconds for get/setsockopt */
#define TCP_KEEPCNT    0x05    /* set pcb->keep_cnt   - Use number of probes sent for get/setsockopt */

/*Structure used for manipulating linger option*/
struct linger {
  s32 l_onoff;                /* option on/off */
  s32 l_linger;               /* linger time in seconds */
};

/* set/clear non-blocking*/
#define IOCPARM_MASK    0x7fU           /* parameters must be < 128 bytes */
#define IOC_IN          0x80000000UL    /* copy in parameters */
#define _IOW(x,y,t)     (IOC_IN|(((long)sizeof(t)&IOCPARM_MASK)<<16)|((x)<<8)|(y))
#define FIONBIO          _IOW('f', 126, unsigned long)


#undef FD_SET
#undef FD_CLR
#undef FD_ISSET
#undef FD_ZERO
#undef fd_set
#undef FD_SETSIZE

/*FD_SET*/
#define FD_SETSIZE    NUM_NETCONN
#define SOCKET_OFFSET LWIP_SOCKET_OFFSET
#define FDSETSAFESET(n, code) do { \
  if (((n) - SOCKET_OFFSET < NUM_NETCONN) && (((s32)(n) - SOCKET_OFFSET) >= 0)) { \
  code; }} while(0)
#define FDSETSAFEGET(n, code) (((n) - SOCKET_OFFSET < NUM_NETCONN) && (((s32)(n) - SOCKET_OFFSET) >= 0) ?\
  (code) : 0)
#define FD_SET(n, p)  FDSETSAFESET(n, (p)->fd_bits[((n)-SOCKET_OFFSET)/8] |=  (1 << (((n)-SOCKET_OFFSET) & 7)))
#define FD_CLR(n, p)  FDSETSAFESET(n, (p)->fd_bits[((n)-SOCKET_OFFSET)/8] &= ~(1 << (((n)-SOCKET_OFFSET) & 7)))
#define FD_ISSET(n,p) FDSETSAFEGET(n, (p)->fd_bits[((n)-SOCKET_OFFSET)/8] &   (1 << (((n)-SOCKET_OFFSET) & 7)))
#define	FD_ZERO(p)	(__extension__ (void)({ \
     s32 __i; \
     char *__tmp = (char *)p; \
     for (__i = 0; __i < sizeof (*(p)); ++__i) \
       *__tmp++ = 0; \
}))

/*Structure fd_set*/
typedef struct fd_set
{
  u8 fd_bits [(FD_SETSIZE+7)/8];
} fd_set;

#endif  // End-of __MG_OPT_SOCKET_H__