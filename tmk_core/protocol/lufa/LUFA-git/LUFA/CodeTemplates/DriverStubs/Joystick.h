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
 *  \brief LUFA Custom Board Joystick Hardware Driver (Template)
 *
 *  This is a stub driver header file, for implementing custom board
 *  layout hardware with compatible LUFA board specific drivers. If
 *  the library is configured to use the BOARD_USER board mode, this
 *  driver file should be completed and copied into the "/Board/" folder
 *  inside the application's folder.
 *
 *  This stub is for the board-specific component of the LUFA Joystick
 *  driver, for a digital four-way (plus button) joystick.
*/

#ifndef __JOYSTICK_USER_H__
#define __JOYSTICK_USER_H__

	/* Includes: */
		// TODO: Add any required includes here

	/* Enable C linkage for C++ Compilers: */
		#if defined(__cplusplus)
			extern "C" {
		#endif

	/* Preprocessor Checks: */
		#if !defined(__INCLUDE_FROM_JOYSTICK_H)
			#error Do not include this file directly. Include LUFA/Drivers/Board/Joystick.h instead.
		#endif

	/* Public Interface - May be used in end-application: */
		/* Macros: */
			/** Mask for the joystick being pushed in the left direction. */
			#define JOY_LEFT                  // TODO: Add mask to indicate joystick left position here

			/** Mask for the joystick being pushed in the right direction. */
			#define JOY_RIGHT                 // TODO: Add mask to indicate joystick right position here

			/** Mask for the joystick being pushed in the upward direction. */
			#define JOY_UP                    // TODO: Add mask to indicate joystick up position here

			/** Mask for the joystick being pushed in the downward direction. */
			#define JOY_DOWN                  // TODO: Add mask to indicate joystick down position here

			/** Mask for the joystick being pushed inward. */
			#define JOY_PRESS                 // TODO: Add mask to indicate joystick pressed position here

		/* Inline Functions: */
		#if !defined(__DOXYGEN__)
			static inline void Joystick_Init(void)
			{
				// TODO: Initialize joystick port pins as inputs with pull-ups
			}

			static inline void Joystick_Disable(void)
			{
				// TODO: Clear the joystick pins as high impedance inputs here
			}

			static inline uint8_t Joystick_GetStatus(void) ATTR_WARN_UNUSED_RESULT;
			static inline uint8_t Joystick_GetStatus(void)
			{
				// TODO: Return current joystick position data which can be obtained by masking against the JOY_* macros
			}
		#endif

	/* Disable C linkage for C++ Compilers: */
		#if defined(__cplusplus)
			}
		#endif

#endif

