/*
 * Copyright (c) 2004, Swedish Institute of Computer Science.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the Institute nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * This file is part of the Contiki operating system.
 *
 * Author: Adam Dunkels <adam@sics.se>
 *
 * $Id: uip-split.c,v 1.2 2008/10/14 13:39:12 julienabeille Exp $
 */

#include "uip-split.h"


#define BUF ((struct uip_tcpip_hdr *)&uip_buf[UIP_LLH_LEN])

/*-----------------------------------------------------------------------------*/
void
uip_split_output(void)
{
#if UIP_TCP
  u16_t tcplen, len1, len2;

  /* We only try to split maximum sized TCP segments. */
  if(BUF->proto == UIP_PROTO_TCP  && uip_len == UIP_BUFSIZE) {

    tcplen = uip_len - UIP_TCPIP_HLEN - UIP_LLH_LEN;
    /* Split the segment in two. If the original packet length was
       odd, we make the second packet one byte larger. */
    len1 = len2 = tcplen / 2;
    if(len1 + len2 < tcplen) {
      ++len2;
    }

    /* Create the first packet. This is done by altering the length
       field of the IP header and updating the checksums. */
    uip_len = len1 + UIP_TCPIP_HLEN + UIP_LLH_LEN;
#if UIP_CONF_IPV6
    /* For IPv6, the IP length field does not include the IPv6 IP header
       length. */
    BUF->len[0] = ((uip_len - UIP_IPH_LEN) >> 8);
    BUF->len[1] = ((uip_len - UIP_IPH_LEN) & 0xff);
#else /* UIP_CONF_IPV6 */
    BUF->len[0] = (uip_len - UIP_LLH_LEN) >> 8;
    BUF->len[1] = (uip_len - UIP_LLH_LEN) & 0xff;
#endif /* UIP_CONF_IPV6 */

    /* Recalculate the TCP checksum. */
    BUF->tcpchksum = 0;
    BUF->tcpchksum = ~(uip_tcpchksum());

#if !UIP_CONF_IPV6
    /* Recalculate the IP checksum. */
    BUF->ipchksum = 0;
    BUF->ipchksum = ~(uip_ipchksum());
#endif /* UIP_CONF_IPV6 */

    /* Transmit the first packet. */
#if UIP_CONF_IPV6
    tcpip_ipv6_output();
#else
	if (USB_CurrentMode == USB_MODE_Device)
	  RNDIS_Device_SendPacket(&Ethernet_RNDIS_Interface_Device, uip_buf, uip_len);
	else
	  RNDIS_Host_SendPacket(&Ethernet_RNDIS_Interface_Host, uip_buf, uip_len);
#endif /* UIP_CONF_IPV6 */

    /* Now, create the second packet. To do this, it is not enough to
       just alter the length field, but we must also update the TCP
       sequence number and point the uip_appdata to a new place in
       memory. This place is determined by the length of the first
       packet (len1). */
    uip_len = len2 + UIP_TCPIP_HLEN + UIP_LLH_LEN;
#if UIP_CONF_IPV6
    /* For IPv6, the IP length field does not include the IPv6 IP header
       length. */
    BUF->len[0] = ((uip_len - UIP_IPH_LEN) >> 8);
    BUF->len[1] = ((uip_len - UIP_IPH_LEN) & 0xff);
#else /* UIP_CONF_IPV6 */
    BUF->len[0] = (uip_len  - UIP_LLH_LEN) >> 8;
    BUF->len[1] = (uip_len - UIP_LLH_LEN) & 0xff;
#endif /* UIP_CONF_IPV6 */

    memcpy(uip_appdata, (u8_t *)uip_appdata + len1, len2);

    uip_add32(BUF->seqno, len1);
    BUF->seqno[0] = uip_acc32[0];
    BUF->seqno[1] = uip_acc32[1];
    BUF->seqno[2] = uip_acc32[2];
    BUF->seqno[3] = uip_acc32[3];

    /* Recalculate the TCP checksum. */
    BUF->tcpchksum = 0;
    BUF->tcpchksum = ~(uip_tcpchksum());

#if !UIP_CONF_IPV6
    /* Recalculate the IP checksum. */
    BUF->ipchksum = 0;
    BUF->ipchksum = ~(uip_ipchksum());
#endif /* UIP_CONF_IPV6 */

    /* Transmit the second packet. */
#if UIP_CONF_IPV6
    tcpip_ipv6_output();
#else
	if (USB_CurrentMode == USB_MODE_Device)
	  RNDIS_Device_SendPacket(&Ethernet_RNDIS_Interface_Device, uip_buf, uip_len);
	else
	  RNDIS_Host_SendPacket(&Ethernet_RNDIS_Interface_Host, uip_buf, uip_len);
#endif /* UIP_CONF_IPV6 */
    return;
  }
#endif /* UIP_TCP */

  /*    uip_fw_output();*/
#if UIP_CONF_IPV6
	tcpip_ipv6_output();
#else
	if (USB_CurrentMode == USB_MODE_Device)
	  RNDIS_Device_SendPacket(&Ethernet_RNDIS_Interface_Device, uip_buf, uip_len);
	else
	  RNDIS_Host_SendPacket(&Ethernet_RNDIS_Interface_Host, uip_buf, uip_len);
#endif /* UIP_CONF_IPV6 */
}

/*-----------------------------------------------------------------------------*/

