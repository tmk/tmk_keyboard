/*
             LUFA Library
     Copyright (C) Dean Camera, 2012.

  dean [at] fourwalledcubicle [dot] com
           www.lufa-lib.org
*/

/*
  Copyright 2012  Dean Camera (dean [at] fourwalledcubicle [dot] com)

  Permission to use, copy, modify, distribute, and sell this
  software and its documentation for any purpose is hereby granted
  without fee, provided that the above copyright notice appear in
  all copies and that both that the copyright notice and this
  permission notice and warranty disclaimer appear in supporting
  documentation, and that the name of the author not be used in
  advertising or publicity pertaining to distribution of the
  software without specific, written prior permission.

  The author disclaim all warranties with regard to this
  software, including all implied warranties of merchantability
  and fitness.  In no event shall the author be liable for any
  special, indirect or consequential damages or any damages
  whatsoever resulting from loss of use, data or profits, whether
  in an action of contract, negligence or other tortious action,
  arising out of or in connection with the use or performance of
  this software.
*/

/** \file
 *  \brief Board specific LED driver header for the PJRC Teensy 1.x/2.x boards.
 *  \copydetails Group_LEDs_TEENSY
 *
 *  \note This file should not be included directly. It is automatically included as needed by the LEDs driver
 *        dispatch header located in LUFA/Drivers/Board/LEDs.h.
 */

/** \ingroup Group_LEDs
 *  \defgroup Group_LEDs_TEENSY2 TEENSY2
 *  \brief Board specific LED driver header for the PJRC Teensy 2 boards.
 *
 *  See \ref Group_LEDs_TEENSY for more details.
 */

/** \ingroup Group_LEDs
 *  \defgroup Group_LEDs_TEENSY TEENSY
 *  \brief Board specific LED driver header for the PJRC Teensy 1.x/2.x boards.
 *
 *  \note For version 2 Teensy boards, compile with <code>BOARD = TEENSY2</code>.
 *
 *  Board specific LED driver header for the PJRC Teensy boards (http://www.pjrc.com/teensy/index.html).
 *
 *  <b>TEENSY</b>:
 *  <table>
 *    <tr><th>Name</th><th>Color</th><th>Info</th><th>Active Level</th><th>Port Pin</th></tr>
 *    <tr><td>LEDS_LED1</td><td>Green</td><td>General Indicator</td><td>High</td><td>PORTD.6</td></tr>
 *  </table>
 *
 *  <b>TEENSY2</b>:
 *  <table>
 *    <tr><th>Name</th><th>Color</th><th>Info</th><th>Active Level</th><th>Port Pin</th></tr>
 *    <tr><td>LEDS_LED1</td><td>Green</td><td>General Indicator</td><td>Low</td><td>PORTD.6</td></tr>
 *  </table>
 *
 *  @{
 */

#ifndef __LEDS_TEENSY_H__
#define __LEDS_TEENSY_H__

	/* Includes: */
		#include "../../../../Common/Common.h"

	/* Enable C linkage for C++ Compilers: */
		#if defined(__cplusplus)
			extern "C" {
		#endif

	/* Preprocessor Checks: */
		#if !defined(__INCLUDE_FROM_LEDS_H)
			#error Do not include this file directly. Include LUFA/Drivers/Board/LEDS.h instead.
		#endif

	/* Public Interface - May be used in end-application: */
		/* Macros: */
			/** LED mask for the first LED on the board. */
			#define LEDS_LED1        (1 << 6)

			/** LED mask for all the LEDs on the board. */
			#define LEDS_ALL_LEDS    LEDS_LED1

			/** LED mask for none of the board LEDs. */
			#define LEDS_NO_LEDS     0

		/* Inline Functions: */
		#if !defined(__DOXYGEN__)
			static inline void LEDs_Init(void)
			{
				DDRD  |= LEDS_ALL_LEDS;

				#if (BOARD == BOARD_TEENSY2)
				PORTD &= ~LEDS_ALL_LEDS;
				#else
				PORTD |=  LEDS_ALL_LEDS;
				#endif
			}

			static inline void LEDs_Disable(void)
			{
				DDRD  &= ~LEDS_ALL_LEDS;
				PORTD &= ~LEDS_ALL_LEDS;
			}

			static inline void LEDs_TurnOnLEDs(const uint8_t LEDMask)
			{
				#if (BOARD == BOARD_TEENSY2)
				PORTD |=  LEDMask;
				#else
				PORTD &= ~LEDMask;
				#endif
			}

			static inline void LEDs_TurnOffLEDs(const uint8_t LEDMask)
			{
				#if (BOARD == BOARD_TEENSY2)
				PORTD &= ~LEDMask;
				#else
				PORTD |=  LEDMask;
				#endif
			}

			static inline void LEDs_SetAllLEDs(const uint8_t LEDMask)
			{
				#if (BOARD == BOARD_TEENSY2)
				PORTD = ((PORTD & ~LEDS_ALL_LEDS) | LEDMask);
				#else
				PORTD = ((PORTD | LEDS_ALL_LEDS) & ~LEDMask);
				#endif
			}

			static inline void LEDs_ChangeLEDs(const uint8_t LEDMask,
			                                   const uint8_t ActiveMask)
			{
				#if (BOARD == BOARD_TEENSY2)
				PORTD = ((PORTD & ~LEDMask) | ActiveMask);
				#else
				PORTD = ((PORTD | LEDMask) & ~ActiveMask);
				#endif
			}

			static inline void LEDs_ToggleLEDs(const uint8_t LEDMask)
			{
				PIND  = LEDMask;
			}

			static inline uint8_t LEDs_GetLEDs(void) ATTR_WARN_UNUSED_RESULT;
			static inline uint8_t LEDs_GetLEDs(void)
			{
				#if (BOARD == BOARD_TEENSY2)
				return (PORTD & LEDS_ALL_LEDS);
				#else
				return (~PORTD & LEDS_ALL_LEDS);
				#endif
			}
		#endif

	/* Disable C linkage for C++ Compilers: */
		#if defined(__cplusplus)
			}
		#endif

#endif

/** @} */

