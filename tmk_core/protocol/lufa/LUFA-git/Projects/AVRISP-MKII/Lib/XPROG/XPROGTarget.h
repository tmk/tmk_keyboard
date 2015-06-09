/*
             LUFA Library
     Copyright (C) Dean Camera, 2014.

  dean [at] fourwalledcubicle [dot] com
           www.lufa-lib.org
*/

/*
  Copyright 2014  Dean Camera (dean [at] fourwalledcubicle [dot] com)

  Permission to use, copy, modify, distribute, and sell this
  software and its documentation for any purpose is hereby granted
  without fee, provided that the above copyright notice appear in
  all copies and that both that the copyright notice and this
  permission notice and warranty disclaimer appear in supporting
  documentation, and that the name of the author not be used in
  advertising or publicity pertaining to distribution of the
  software without specific, written prior permission.

  The author disclaims all warranties with regard to this
  software, including all implied warranties of merchantability
  and fitness.  In no event shall the author be liable for any
  special, indirect or consequential damages or any damages
  whatsoever resulting from loss of use, data or profits, whether
  in an action of contract, negligence or other tortious action,
  arising out of or in connection with the use or performance of
  this software.
*/

/** \file
 *
 *  Header file for XPROGTarget.c.
 */

#ifndef _XPROG_TARGET_
#define _XPROG_TARGET_

	/* Includes: */
		#include <avr/io.h>
		#include <avr/interrupt.h>
		#include <stdbool.h>

		#include <LUFA/Common/Common.h>

		#include "../V2Protocol.h"
		#include "XPROGProtocol.h"
		#include "Config/AppConfig.h"

	/* Preprocessor Checks: */
		#if ((BOARD == BOARD_XPLAIN) || (BOARD == BOARD_XPLAIN_REV1))
			#undef ENABLE_ISP_PROTOCOL

			#if !defined(ENABLE_XPROG_PROTOCOL)
				#define ENABLE_XPROG_PROTOCOL
			#endif
		#endif

		/** Serial carrier TPI/PDI speed in Hz, when hardware TPI/PDI mode is used. */
		#define XPROG_HARDWARE_SPEED       2000000

		/** Total number of bits in a single USART frame. */
		#define BITS_IN_USART_FRAME        12

 		/** \name PDI Related Constants
 		 * @{
 		 */
		#define PDI_CMD_LDS(AddressSize, DataSize)  (0x00 | (  AddressSize << 2) | DataSize)
		#define PDI_CMD_LD(PointerAccess, DataSize) (0x20 | (PointerAccess << 2) | DataSize)
		#define PDI_CMD_STS(AddressSize, DataSize)  (0x40 | (  AddressSize << 2) | DataSize)
		#define PDI_CMD_ST(PointerAccess, DataSize) (0x60 | (PointerAccess << 2) | DataSize)
		#define PDI_CMD_LDCS(PDIReg)                (0x80 | PDIReg)
		#define PDI_CMD_REPEAT(DataSize)            (0xA0 | DataSize)
		#define PDI_CMD_STCS(PDIReg)                (0xC0 | PDIReg)
		#define PDI_CMD_KEY                         0xE0

		#define PDI_REG_STATUS             0
		#define PDI_REG_RESET              1
		#define PDI_REG_CTRL               2

		#define PDI_STATUS_NVM             (1 << 1)

		#define PDI_RESET_KEY              0x59
		#define PDI_NVMENABLE_KEY          (uint8_t[]){0x12, 0x89, 0xAB, 0x45, 0xCD, 0xD8, 0x88, 0xFF}

		#define PDI_DATASIZE_1BYTE         0
		#define PDI_DATASIZE_2BYTES        1
		#define PDI_DATASIZE_3BYTES        2
		#define PDI_DATASIZE_4BYTES        3

		#define PDI_POINTER_INDIRECT       0
		#define PDI_POINTER_INDIRECT_PI    1
		#define PDI_POINTER_DIRECT         2
 		/** @} */

 		/** \name TPI Related Constants
 		 * @{
 		 */
 		#define TPI_CMD_SLD(PointerAccess) (0x20 | PointerAccess)
		#define TPI_CMD_SST(PointerAccess) (0x60 | PointerAccess)
		#define TPI_CMD_SSTPR              0x68
		#define TPI_CMD_SIN(Address)       (0x10 | ((Address & 0x30) << 1) | (Address & 0x0F))
		#define TPI_CMD_SOUT(Address)      (0x90 | ((Address & 0x30) << 1) | (Address & 0x0F))
		#define TPI_CMD_SLDCS(TPIReg)      (0x80 | TPIReg)
		#define TPI_CMD_SSTCS(TPIReg)      (0xC0 | TPIReg)
		#define TPI_CMD_SKEY               0xE0

		#define TPI_REG_STATUS             0x00
		#define TPI_REG_CTRL               0x02
		#define TPI_REG_ID                 0x0F

		#define TPI_STATUS_NVM             (1 << 1)

		#define TPI_NVMENABLE_KEY          (uint8_t[]){0x12, 0x89, 0xAB, 0x45, 0xCD, 0xD8, 0x88, 0xFF}

		#define TPI_POINTER_INDIRECT       0
		#define TPI_POINTER_INDIRECT_PI    4
 		/** @} */

	/* Function Prototypes: */
		void    XPROGTarget_EnableTargetPDI(void);
		void    XPROGTarget_EnableTargetTPI(void);
		void    XPROGTarget_DisableTargetPDI(void);
		void    XPROGTarget_DisableTargetTPI(void);
		void    XPROGTarget_SendByte(const uint8_t Byte);
		uint8_t XPROGTarget_ReceiveByte(void);
		void    XPROGTarget_SendIdle(void);
		bool    XPROGTarget_WaitWhileNVMBusBusy(void);

		#if (defined(INCLUDE_FROM_XPROGTARGET_C) && defined(ENABLE_XPROG_PROTOCOL))
			static void XPROGTarget_SetTxMode(void);
			static void XPROGTarget_SetRxMode(void);
		#endif

#endif

