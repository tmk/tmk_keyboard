/* This header file is part of the ATMEL AVR-UC3-SoftwareFramework-1.7.0 Release */

/*****************************************************************************
* ppp_oe.h - PPP Over Ethernet implementation for lwIP.
*
* Copyright (c) 2006 by Marc Boucher, Services Informatiques (MBSI) inc.
*
* The authors hereby grant permission to use, copy, modify, distribute,
* and license this software and its documentation for any purpose, provided
* that existing copyright notices are retained in all copies and that this
* notice and the following disclaimer are included verbatim in any 
* distributions. No written agreement, license, or royalty fee is required
* for any of the authorized uses.
*
* THIS SOFTWARE IS PROVIDED BY THE CONTRIBUTORS *AS IS* AND ANY EXPRESS OR
* IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
* OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
* IN NO EVENT SHALL THE CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
* INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
* NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
* DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
* THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
* THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
******************************************************************************
* REVISION HISTORY
*
* 06-01-01 Marc Boucher <marc@mbsi.ca>
*   Ported to lwIP.
*****************************************************************************/



/* based on NetBSD: if_pppoe.c,v 1.64 2006/01/31 23:50:15 martin Exp */

/*-
 * Copyright (c) 2002 The NetBSD Foundation, Inc.
 * All rights reserved.
 *
 * This code is derived from software contributed to The NetBSD Foundation
 * by Martin Husemann <martin@NetBSD.org>.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *        This product includes software developed by the NetBSD
 *        Foundation, Inc. and its contributors.
 * 4. Neither the name of The NetBSD Foundation nor the names of its
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE NETBSD FOUNDATION, INC. AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE FOUNDATION OR CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */
#ifndef PPP_OE_H
#define PPP_OE_H

#include "lwip/opt.h"

#if PPPOE_SUPPORT > 0

#ifdef PACK_STRUCT_USE_INCLUDES
#  include "arch/bpstruct.h"
#endif
PACK_STRUCT_BEGIN
struct pppoehdr {
	PACK_STRUCT_FIELD(u8_t vertype);
	PACK_STRUCT_FIELD(u8_t code);
	PACK_STRUCT_FIELD(u16_t session);
	PACK_STRUCT_FIELD(u16_t plen);
} PACK_STRUCT_STRUCT;
PACK_STRUCT_END
#ifdef PACK_STRUCT_USE_INCLUDES
#  include "arch/epstruct.h"
#endif

#ifdef PACK_STRUCT_USE_INCLUDES
#  include "arch/bpstruct.h"
#endif
PACK_STRUCT_BEGIN
struct pppoetag {
	PACK_STRUCT_FIELD(u16_t tag);
	PACK_STRUCT_FIELD(u16_t len);
} PACK_STRUCT_STRUCT;
PACK_STRUCT_END
#ifdef PACK_STRUCT_USE_INCLUDES
#  include "arch/epstruct.h"
#endif


#define PPPOE_STATE_INITIAL	0
#define PPPOE_STATE_PADI_SENT	1
#define	PPPOE_STATE_PADR_SENT	2
#define	PPPOE_STATE_SESSION	3
#define	PPPOE_STATE_CLOSING	4
/* passive */
#define	PPPOE_STATE_PADO_SENT	1

#define PPPOE_HEADERLEN	sizeof(struct pppoehdr)
#define	PPPOE_VERTYPE	0x11	/* VER=1, TYPE = 1 */

#define	PPPOE_TAG_EOL		0x0000		/* end of list */
#define	PPPOE_TAG_SNAME		0x0101		/* service name */
#define	PPPOE_TAG_ACNAME	0x0102		/* access concentrator name */
#define	PPPOE_TAG_HUNIQUE	0x0103		/* host unique */
#define	PPPOE_TAG_ACCOOKIE	0x0104		/* AC cookie */
#define	PPPOE_TAG_VENDOR	0x0105		/* vendor specific */
#define	PPPOE_TAG_RELAYSID	0x0110		/* relay session id */
#define	PPPOE_TAG_SNAME_ERR	0x0201		/* service name error */
#define	PPPOE_TAG_ACSYS_ERR	0x0202		/* AC system error */
#define	PPPOE_TAG_GENERIC_ERR	0x0203		/* gerneric error */

#define PPPOE_CODE_PADI		0x09		/* Active Discovery Initiation */
#define	PPPOE_CODE_PADO		0x07		/* Active Discovery Offer */
#define	PPPOE_CODE_PADR		0x19		/* Active Discovery Request */
#define	PPPOE_CODE_PADS		0x65		/* Active Discovery Session confirmation */
#define	PPPOE_CODE_PADT		0xA7		/* Active Discovery Terminate */

#ifndef ETHERMTU
#define ETHERMTU 1500
#endif

/* two byte PPP protocol discriminator, then IP data */
#define	PPPOE_MAXMTU	(ETHERMTU-PPPOE_HEADERLEN-2)

struct pppoe_softc;


void pppoe_init(void);

err_t pppoe_create(struct netif *ethif, int pd, void (*linkStatusCB)(int pd, int up), struct pppoe_softc **scptr);
err_t pppoe_destroy(struct netif *ifp);

int pppoe_connect(struct pppoe_softc *sc);
void pppoe_disconnect(struct pppoe_softc *sc);

void pppoe_disc_input(struct netif *netif, struct pbuf *p);
void pppoe_data_input(struct netif *netif, struct pbuf *p);

err_t pppoe_xmit(struct pppoe_softc *sc, struct pbuf *pb);

extern int pppoe_hdrlen;

#endif /* PPPOE_SUPPORT */

#endif /* PPP_OE_H */
