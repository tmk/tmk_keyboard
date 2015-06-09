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
 *  Header file for XPROGProtocol.c.
 */

#ifndef _XPROG_PROTOCOL_
#define _XPROG_PROTOCOL_

	/* Includes: */
		#include <avr/io.h>
		#include <util/delay.h>
		#include <stdio.h>

		#include <LUFA/Drivers/USB/USB.h>

		#include "../V2Protocol.h"
		#include "XMEGANVM.h"
		#include "TINYNVM.h"
		#include "Config/AppConfig.h"

	/* Preprocessor Checks: */
		#if ((BOARD == BOARD_XPLAIN) || (BOARD == BOARD_XPLAIN_REV1))
			/* On the XPLAIN board, we only need PDI programming
			   for the ATXMEGA128A1 - disable ISP to prevent hardware
			   damage and force-enable XPROG.
			 */
			#undef ENABLE_ISP_PROTOCOL

			#if !defined(ENABLE_XPROG_PROTOCOL)
				#define ENABLE_XPROG_PROTOCOL
			#endif
		#endif

	/* Macros: */
		#define XPROG_CMD_ENTER_PROGMODE             0x01
		#define XPROG_CMD_LEAVE_PROGMODE             0x02
		#define XPROG_CMD_ERASE                      0x03
		#define XPROG_CMD_WRITE_MEM                  0x04
		#define XPROG_CMD_READ_MEM                   0x05
		#define XPROG_CMD_CRC                        0x06
		#define XPROG_CMD_SET_PARAM                  0x07

		#define XPROG_MEM_TYPE_APPL                  1
		#define XPROG_MEM_TYPE_BOOT                  2
		#define XPROG_MEM_TYPE_EEPROM                3
		#define XPROG_MEM_TYPE_FUSE                  4
		#define XPROG_MEM_TYPE_LOCKBITS              5
		#define XPROG_MEM_TYPE_USERSIG               6
		#define XPROG_MEM_TYPE_FACTORY_CALIBRATION   7

		#define XPROG_ERASE_CHIP                     1
		#define XPROG_ERASE_APP                      2
		#define XPROG_ERASE_BOOT                     3
		#define XPROG_ERASE_EEPROM                   4
		#define XPROG_ERASE_APP_PAGE                 5
		#define XPROG_ERASE_BOOT_PAGE                6
		#define XPROG_ERASE_EEPROM_PAGE              7
		#define XPROG_ERASE_USERSIG                  8

		#define XPROG_MEM_WRITE_ERASE                0
		#define XPROG_MEM_WRITE_WRITE                1

		#define XPROG_CRC_APP                        1
		#define XPROG_CRC_BOOT                       2
		#define XPROG_CRC_FLASH                      3

		#define XPROG_ERR_OK                         0
		#define XPROG_ERR_FAILED                     1
		#define XPROG_ERR_COLLISION                  2
		#define XPROG_ERR_TIMEOUT                    3

		#define XPROG_PARAM_NVMBASE                  0x01
		#define XPROG_PARAM_EEPPAGESIZE              0x02
		#define XPROG_PARAM_NVMCMD_REG               0x03
		#define XPROG_PARAM_NVMCSR_REG               0x04
		#define XPROG_PARAM_UNKNOWN_1                0x05

		#define XPROG_PROTOCOL_PDI                   0x00
		#define XPROG_PROTOCOL_JTAG                  0x01
		#define XPROG_PROTOCOL_TPI                   0x02

		#define XPROG_PAGEMODE_WRITE                 (1 << 1)
		#define XPROG_PAGEMODE_ERASE                 (1 << 0)

	/* External Variables: */
		extern uint32_t XPROG_Param_NVMBase;
		extern uint16_t XPROG_Param_EEPageSize;
		extern uint8_t  XPROG_Param_NVMCSRRegAddr;
		extern uint8_t  XPROG_Param_NVMCMDRegAddr;

	/* Function Prototypes: */
		void XPROGProtocol_SetMode(void);
		void XPROGProtocol_Command(void);

		#if (defined(INCLUDE_FROM_XPROGPROTOCOL_C) && defined(ENABLE_XPROG_PROTOCOL))
			static void XPROGProtocol_EnterXPROGMode(void);
			static void XPROGProtocol_LeaveXPROGMode(void);
			static void XPROGProtocol_SetParam(void);
			static void XPROGProtocol_Erase(void);
			static void XPROGProtocol_WriteMemory(void);
			static void XPROGProtocol_ReadMemory(void);
			static void XPROGProtocol_ReadCRC(void);
		#endif

#endif

