/* This source file is part of the ATMEL AVR-UC3-SoftwareFramework-1.7.0 Release */

/**
 * @file
 * Modules initialization
 *
 */

/*
 * Copyright (c) 2001-2004 Swedish Institute of Computer Science.
 * All rights reserved. 
 * 
 * Redistribution and use in source and binary forms, with or without modification, 
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission. 
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED 
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT 
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT 
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING 
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 * 
 * Author: Adam Dunkels <adam@sics.se>
 *
 */

#include "lwip/opt.h"

#include "lwip/init.h"
#include "lwip/stats.h"
#include "lwip/sys.h"
#include "lwip/mem.h"
#include "lwip/memp.h"
#include "lwip/pbuf.h"
#include "lwip/netif.h"
#include "lwip/sockets.h"
#include "lwip/ip.h"
#include "lwip/raw.h"
#include "lwip/udp.h"
#include "lwip/tcp.h"
#include "lwip/snmp_msg.h"
#include "lwip/autoip.h"
#include "lwip/igmp.h"
#include "lwip/dns.h"
#include "netif/etharp.h"

/* Compile-time sanity checks for configuration errors.
 * These can be done independently of LWIP_DEBUG, without penalty.
 */
#ifndef BYTE_ORDER
  #error "BYTE_ORDER is not defined, you have to define it in your cc.h"
#endif
#if (!IP_SOF_BROADCAST && IP_SOF_BROADCAST_RECV)
  #error "If you want to use broadcast filter per pcb on recv operations, you have to define IP_SOF_BROADCAST=1 in your lwipopts.h"
#endif
#if (!LWIP_ARP && ARP_QUEUEING)
  #error "If you want to use ARP Queueing, you have to define LWIP_ARP=1 in your lwipopts.h"
#endif
#if (!LWIP_UDP && LWIP_UDPLITE)
  #error "If you want to use UDP Lite, you have to define LWIP_UDP=1 in your lwipopts.h"
#endif
#if (!LWIP_UDP && LWIP_SNMP)
  #error "If you want to use SNMP, you have to define LWIP_UDP=1 in your lwipopts.h"
#endif
#if (!LWIP_UDP && LWIP_DHCP)
  #error "If you want to use DHCP, you have to define LWIP_UDP=1 in your lwipopts.h"
#endif
#if (!LWIP_UDP && LWIP_IGMP)
  #error "If you want to use IGMP, you have to define LWIP_UDP=1 in your lwipopts.h"
#endif
#if (!LWIP_UDP && LWIP_DNS)
  #error "If you want to use DNS, you have to define LWIP_UDP=1 in your lwipopts.h"
#endif
#if (LWIP_ARP && (ARP_TABLE_SIZE > 0x7f))
  #error "If you want to use ARP, ARP_TABLE_SIZE must fit in an s8_t, so, you have to reduce it in your lwipopts.h"
#endif
#if (LWIP_ARP && ARP_QUEUEING && (MEMP_NUM_ARP_QUEUE<=0))
  #error "If you want to use ARP Queueing, you have to define MEMP_NUM_ARP_QUEUE>=1 in your lwipopts.h"
#endif
#if (LWIP_RAW && (MEMP_NUM_RAW_PCB<=0))
  #error "If you want to use RAW, you have to define MEMP_NUM_RAW_PCB>=1 in your lwipopts.h"
#endif
#if (LWIP_UDP && (MEMP_NUM_UDP_PCB<=0))
  #error "If you want to use UDP, you have to define MEMP_NUM_UDP_PCB>=1 in your lwipopts.h"
#endif
#if (LWIP_TCP && (MEMP_NUM_TCP_PCB<=0))
  #error "If you want to use TCP, you have to define MEMP_NUM_TCP_PCB>=1 in your lwipopts.h"
#endif
#if (LWIP_TCP && (TCP_WND > 0xffff))
  #error "If you want to use TCP, TCP_WND must fit in an u16_t, so, you have to reduce it in your lwipopts.h"
#endif
#if (LWIP_TCP && (TCP_SND_QUEUELEN > 0xffff))
  #error "If you want to use TCP, TCP_SND_QUEUELEN must fit in an u16_t, so, you have to reduce it in your lwipopts.h"
#endif
#if (LWIP_TCP && ((TCP_MAXRTX > 12) || (TCP_SYNMAXRTX > 12)))
  #error "If you want to use TCP, TCP_MAXRTX and TCP_SYNMAXRTX must less or equal to 12 (due to tcp_backoff table), so, you have to reduce them in your lwipopts.h"
#endif
#if (LWIP_TCP && TCP_LISTEN_BACKLOG && (TCP_DEFAULT_LISTEN_BACKLOG < 0) || (TCP_DEFAULT_LISTEN_BACKLOG > 0xff))
  #error "If you want to use TCP backlog, TCP_DEFAULT_LISTEN_BACKLOG must fit into an u8_t"
#endif
#if (LWIP_IGMP && (MEMP_NUM_IGMP_GROUP<=1))
  #error "If you want to use IGMP, you have to define MEMP_NUM_IGMP_GROUP>1 in your lwipopts.h"
#endif
#if (PPP_SUPPORT && (NO_SYS==1))
  #error "If you want to use PPP, you have to define NO_SYS=0 in your lwipopts.h"
#endif 
#if (LWIP_NETIF_API && (NO_SYS==1))
  #error "If you want to use NETIF API, you have to define NO_SYS=0 in your lwipopts.h"
#endif
#if ((LWIP_SOCKET || LWIP_NETCONN) && (NO_SYS==1))
  #error "If you want to use Sequential API, you have to define NO_SYS=0 in your lwipopts.h"
#endif
#if ((LWIP_NETCONN || LWIP_SOCKET) && (MEMP_NUM_TCPIP_MSG_API<=0))
  #error "If you want to use Sequential API, you have to define MEMP_NUM_TCPIP_MSG_API>=1 in your lwipopts.h"
#endif
#if (!LWIP_NETCONN && LWIP_SOCKET)
  #error "If you want to use Socket API, you have to define LWIP_NETCONN=1 in your lwipopts.h"
#endif
#if (((!LWIP_DHCP) || (!LWIP_AUTOIP)) && LWIP_DHCP_AUTOIP_COOP)
  #error "If you want to use DHCP/AUTOIP cooperation mode, you have to define LWIP_DHCP=1 and LWIP_AUTOIP=1 in your lwipopts.h"
#endif
#if (((!LWIP_DHCP) || (!LWIP_ARP)) && DHCP_DOES_ARP_CHECK)
  #error "If you want to use DHCP ARP checking, you have to define LWIP_DHCP=1 and LWIP_ARP=1 in your lwipopts.h"
#endif
#if (!LWIP_ARP && LWIP_AUTOIP)
  #error "If you want to use AUTOIP, you have to define LWIP_ARP=1 in your lwipopts.h"
#endif
#if (LWIP_SNMP && (SNMP_CONCURRENT_REQUESTS<=0))
  #error "If you want to use SNMP, you have to define SNMP_CONCURRENT_REQUESTS>=1 in your lwipopts.h"
#endif
#if (LWIP_SNMP && (SNMP_TRAP_DESTINATIONS<=0))
  #error "If you want to use SNMP, you have to define SNMP_TRAP_DESTINATIONS>=1 in your lwipopts.h"
#endif
#if (LWIP_TCP && ((LWIP_EVENT_API && LWIP_CALLBACK_API) || (!LWIP_EVENT_API && !LWIP_CALLBACK_API)))
  #error "One and exactly one of LWIP_EVENT_API and LWIP_CALLBACK_API has to be enabled in your lwipopts.h"
#endif
/* There must be sufficient timeouts, taking into account requirements of the subsystems. */
#if ((NO_SYS==0) && (MEMP_NUM_SYS_TIMEOUT < (LWIP_TCP + IP_REASSEMBLY + LWIP_ARP + (2*LWIP_DHCP) + LWIP_AUTOIP + LWIP_IGMP + LWIP_DNS + PPP_SUPPORT)))
  #error "MEMP_NUM_SYS_TIMEOUT is too low to accomodate all required timeouts"
#endif
#if (IP_REASSEMBLY && (MEMP_NUM_REASSDATA > IP_REASS_MAX_PBUFS))
  #error "MEMP_NUM_REASSDATA > IP_REASS_MAX_PBUFS doesn't make sense since each struct ip_reassdata must hold 2 pbufs at least!"
#endif
#if (MEM_LIBC_MALLOC && MEM_USE_POOLS)
  #error "MEM_LIBC_MALLOC and MEM_USE_POOLS may not both be simultaneously enabled in your lwipopts.h"
#endif
#if (MEM_USE_POOLS && !MEMP_USE_CUSTOM_POOLS)
  #error "MEM_USE_POOLS requires custom pools (MEMP_USE_CUSTOM_POOLS) to be enabled in your lwipopts.h"
#endif
#if (PBUF_POOL_BUFSIZE <= MEM_ALIGNMENT)
  #error "PBUF_POOL_BUFSIZE must be greater than MEM_ALIGNMENT or the offset may take the full first pbuf"
#endif
#if (TCP_QUEUE_OOSEQ && !LWIP_TCP)
  #error "TCP_QUEUE_OOSEQ requires LWIP_TCP"
#endif
#if (DNS_LOCAL_HOSTLIST && !DNS_LOCAL_HOSTLIST_IS_DYNAMIC && !(defined(DNS_LOCAL_HOSTLIST_INIT)))
  #error "you have to define define DNS_LOCAL_HOSTLIST_INIT {{'host1', 0x123}, {'host2', 0x234}} to initialize DNS_LOCAL_HOSTLIST"
#endif
#if PPP_SUPPORT && !PPPOS_SUPPORT & !PPPOE_SUPPORT
  #error "PPP_SUPPORT needs either PPPOS_SUPPORT or PPPOE_SUPPORT turned on"
#endif


/* Compile-time checks for deprecated options.
 */
#ifdef MEMP_NUM_TCPIP_MSG
  #error "MEMP_NUM_TCPIP_MSG option is deprecated. Remove it from your lwipopts.h."
#endif
#ifdef MEMP_NUM_API_MSG
  #error "MEMP_NUM_API_MSG option is deprecated. Remove it from your lwipopts.h."
#endif
#ifdef TCP_REXMIT_DEBUG
  #error "TCP_REXMIT_DEBUG option is deprecated. Remove it from your lwipopts.h."
#endif
#ifdef RAW_STATS
  #error "RAW_STATS option is deprecated. Remove it from your lwipopts.h."
#endif
#ifdef ETHARP_QUEUE_FIRST
  #error "ETHARP_QUEUE_FIRST option is deprecated. Remove it from your lwipopts.h."
#endif
#ifdef ETHARP_ALWAYS_INSERT
  #error "ETHARP_ALWAYS_INSERT option is deprecated. Remove it from your lwipopts.h."
#endif
#if SO_REUSE
/* I removed the lot since this was an ugly hack. It broke the raw-API.
   It also came with many ugly goto's, Christiaan Simons. */
  #error "SO_REUSE currently unavailable, this was a hack"
#endif

#ifdef LWIP_DEBUG
static void
lwip_sanity_check(void)
{
  /* Warnings */
#if LWIP_NETCONN
  if (MEMP_NUM_NETCONN > (MEMP_NUM_TCP_PCB+MEMP_NUM_TCP_PCB_LISTEN+MEMP_NUM_UDP_PCB+MEMP_NUM_RAW_PCB))
    LWIP_PLATFORM_DIAG(("lwip_sanity_check: WARNING: MEMP_NUM_NETCONN should be less than the sum of MEMP_NUM_{TCP,RAW,UDP}_PCB+MEMP_NUM_TCP_PCB_LISTEN\n"));
#endif /* LWIP_NETCONN */
#if LWIP_TCP
  if (MEMP_NUM_TCP_SEG < TCP_SND_QUEUELEN)
    LWIP_PLATFORM_DIAG(("lwip_sanity_check: WARNING: MEMP_NUM_TCP_SEG should be at least as big as TCP_SND_QUEUELEN\n"));
  if (TCP_SND_BUF < 2 * TCP_MSS)
    LWIP_PLATFORM_DIAG(("lwip_sanity_check: WARNING: TCP_SND_BUF must be at least as much as (2 * TCP_MSS) for things to work smoothly\n"));
  if (TCP_SND_QUEUELEN < (2 * (TCP_SND_BUF/TCP_MSS)))
    LWIP_PLATFORM_DIAG(("lwip_sanity_check: WARNING: TCP_SND_QUEUELEN must be at least as much as (2 * TCP_SND_BUF/TCP_MSS) for things to work\n"));
  if (TCP_SNDLOWAT > TCP_SND_BUF)
    LWIP_PLATFORM_DIAG(("lwip_sanity_check: WARNING: TCP_SNDLOWAT must be less than or equal to TCP_SND_BUF.\n"));
  if (TCP_WND > (PBUF_POOL_SIZE*PBUF_POOL_BUFSIZE))
    LWIP_PLATFORM_DIAG(("lwip_sanity_check: WARNING: TCP_WND is larger than space provided by PBUF_POOL_SIZE*PBUF_POOL_BUFSIZE\n"));
  if (TCP_WND < TCP_MSS)
    LWIP_PLATFORM_DIAG(("lwip_sanity_check: WARNING: TCP_WND is smaller than MSS\n"));
#endif /* LWIP_TCP */
}
#else  /* LWIP_DEBUG */
#define lwip_sanity_check()
#endif /* LWIP_DEBUG */

/**
 * Perform Sanity check of user-configurable values, and initialize all modules.
 */
void
lwip_init(void)
{
  /* Sanity check user-configurable values */
  lwip_sanity_check();

  /* Modules initialization */
  stats_init();
  sys_init();
  mem_init();
  memp_init();
  pbuf_init();
  netif_init();
#if LWIP_SOCKET
  lwip_socket_init();
#endif /* LWIP_SOCKET */
  ip_init();
#if LWIP_ARP
  etharp_init();
#endif /* LWIP_ARP */
#if LWIP_RAW
  raw_init();
#endif /* LWIP_RAW */
#if LWIP_UDP
  udp_init();
#endif /* LWIP_UDP */
#if LWIP_TCP
  tcp_init();
#endif /* LWIP_TCP */
#if LWIP_SNMP
  snmp_init();
#endif /* LWIP_SNMP */
#if LWIP_AUTOIP
  autoip_init();
#endif /* LWIP_AUTOIP */
#if LWIP_IGMP
  igmp_init();
#endif /* LWIP_IGMP */
#if LWIP_DNS
  dns_init();
#endif /* LWIP_DNS */
}
