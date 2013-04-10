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
 *  \brief Board specific LED driver header for the Olimex AVR-USB-32U4.
 *  \copydetails Group_LEDs_OLIMEX32U4
 *
 *  \note This file should not be included directly. It is automatically included as needed by the LEDs driver
 *        dispatch header located in LUFA/Drivers/Board/LEDs.h.
 */

/** \ingroup Group_LEDs
 *  \defgroup Group_LEDs_OLIMEX32U4 OLIMEX32U4
 *  \brief Board specific LED driver header for the Olimex AVR-USB-32U4.
 *
 *  Board specific LED driver header for the Olimex AVR-USB-32U4 (http://www.olimex.com/dev/olimexino-32u4.html).
 *
 *  <table>
 *    <tr><th>Name</th><th>Color</th><th>Info</th><th>Active Level</th><th>Port Pin</th></tr>
 *    <tr><td>LEDS_LED1</td><td>Green</td><td>TX</td><td>High</td><td>PORTD.5</td></tr>
 *    <tr><td>LEDS_LED2</td><td>Yellow</td><td>RX</td><td>High</td><td>PORTB.0</td></tr>
 *    <tr><td>LEDS_LED3</td><td>Green</td><td>General Indicator (Default Unconnected)</td><td>High</td><td>PORTE.6</td></tr>
 *    <tr><td>LEDS_LED4</td><td>Yellow</td><td>General Indicator (Default Unconnected)</td><td>High</td><td>PORTB.5</td></tr>
 *  </table>
 *
 *  @{
 */

#ifndef __LEDS_OLIMEX32U4_H__
#define __LEDS_OLIMEX32U4_H__

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

	/* Private Interface - For use in library only: */
	#if !defined(__DOXYGEN__)
		/* Macros: */
			#define LEDS_PORTB_LEDS       (LEDS_LED2 | LEDS_LED4)
			#define LEDS_PORTD_LEDS       (LEDS_LED1)
			#define LEDS_PORTE_LEDS       (LEDS_LED3)

			#define LEDS_PORTD_MASK_SHIFT 1
	#endif
	
	/* Public Interface - May be used in end-application: */
		/* Macros: */
			/** LED mask for the first LED on the board. */
			#define LEDS_LED1        ((1 << 5) >> LEDS_PORTD_MASK_SHIFT)

			/** LED mask for the second LED on the board. */
			#define LEDS_LED2        (1 << 0)
			
			/** LED mask for the third LED on the board. */
			#define LEDS_LED3        (1 << 5)

			/** LED mask for the fourth LED on the board. */
			#define LEDS_LED4        (1 << 6)
			
			/** LED mask for all the LEDs on the board. */
			#define LEDS_ALL_LEDS    (LEDS_LED1 | LEDS_LED2 | LEDS_LED3 | LEDS_LED4)

			/** LED mask for none of the board LEDs. */
			#define LEDS_NO_LEDS     0

		/* Inline Functions: */
		#if !defined(__DOXYGEN__)
			static inline void LEDs_Init(void)
			{
				DDRB  |=  LEDS_PORTB_LEDS;
				PORTB &= ~LEDS_PORTB_LEDS;
				DDRD  |=  (LEDS_PORTD_LEDS << LEDS_PORTD_MASK_SHIFT);
				PORTD &= ~(LEDS_PORTD_LEDS << LEDS_PORTD_MASK_SHIFT);
				DDRE  |=  LEDS_PORTE_LEDS;
				PORTE &= ~LEDS_PORTE_LEDS;
			}

			static inline void LEDs_Disable(void)
			{
				DDRB  &= ~LEDS_PORTB_LEDS;
				PORTB &= ~LEDS_PORTB_LEDS;
				DDRD  &= ~(LEDS_PORTD_LEDS << LEDS_PORTD_MASK_SHIFT);
				PORTD &= ~(LEDS_PORTD_LEDS << LEDS_PORTD_MASK_SHIFT);
				DDRE  &= ~LEDS_PORTE_LEDS;
				PORTE &= ~LEDS_PORTE_LEDS;
			}

			static inline void LEDs_TurnOnLEDs(const uint8_t LEDMask)
			{
				PORTB |= (LEDMask & LEDS_PORTB_LEDS);
				PORTD |= ((LEDMask & LEDS_PORTD_LEDS) << LEDS_PORTD_MASK_SHIFT);
				PORTE |= (LEDMask & LEDS_PORTE_LEDS);
			}

			static inline void LEDs_TurnOffLEDs(const uint8_t LEDMask)
			{
				PORTB &= ~(LEDMask & LEDS_PORTB_LEDS);
				PORTD &= ~((LEDMask & LEDS_PORTD_LEDS) << LEDS_PORTD_MASK_SHIFT);
				PORTE &= ~(LEDMask & LEDS_PORTE_LEDS);
			}

			static inline void LEDs_SetAllLEDs(const uint8_t LEDMask)
			{
				PORTB = ((PORTB & ~LEDS_PORTB_LEDS) | (LEDMask & LEDS_PORTB_LEDS));
				PORTD = ((PORTD & ~(LEDS_PORTD_LEDS << LEDS_PORTD_MASK_SHIFT)) |
				         ((LEDMask & LEDS_PORTD_LEDS) << LEDS_PORTD_MASK_SHIFT));
				PORTE = ((PORTE & ~LEDS_PORTE_LEDS) | (LEDMask & LEDS_PORTE_LEDS));
			}

			static inline void LEDs_ChangeLEDs(const uint8_t LEDMask,
			                                   const uint8_t ActiveMask)
			{
				PORTB = ((PORTB & ~(LEDMask & LEDS_PORTB_LEDS)) | (ActiveMask & LEDS_PORTB_LEDS));
				PORTD = ((PORTD & ~((LEDMask & LEDS_PORTD_LEDS) << LEDS_PORTD_MASK_SHIFT)) |
				         ((ActiveMask & LEDS_PORTD_LEDS) << LEDS_PORTD_MASK_SHIFT));
				PORTE = ((PORTE & ~(LEDMask & LEDS_PORTE_LEDS)) | (ActiveMask & LEDS_PORTE_LEDS));
			}

			static inline void LEDs_ToggleLEDs(const uint8_t LEDMask)
			{
				PINB  = (LEDMask & LEDS_PORTB_LEDS);
				PIND  = ((LEDMask & LEDS_PORTD_LEDS) << LEDS_PORTD_MASK_SHIFT);
				PINE  = (LEDMask & LEDS_PORTE_LEDS);
			}

			static inline uint8_t LEDs_GetLEDs(void) ATTR_WARN_UNUSED_RESULT;
			static inline uint8_t LEDs_GetLEDs(void)
			{
				return ((PORTB & LEDS_PORTB_LEDS) | 
				        ((PORTD & (LEDS_PORTD_LEDS << LEDS_PORTD_MASK_SHIFT)) >> LEDS_PORTD_MASK_SHIFT) |
				        (PORTE & LEDS_PORTE_LEDS));
			}
		#endif

	/* Disable C linkage for C++ Compilers: */
		#if defined(__cplusplus)
			}
		#endif

#endif

/** @} */

