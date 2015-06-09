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
 *  Header file for ConfigDescriptor.c.
 */

#ifndef _CONFIGDESCRIPTOR_H_
#define _CONFIGDESCRIPTOR_H_

	/* Includes: */
		#include <LUFA/Drivers/USB/USB.h>

		#include "AudioInputHost.h"

	/* Macros: */
		/** Pipe address for the Audio data IN pipe. */
		#define AUDIO_DATA_IN_PIPE          (PIPE_DIR_IN | 1)

	/* Enums: */
		/** Enum for the possible return codes of the \ref ProcessConfigurationDescriptor() function. */
		enum AudioHost_GetConfigDescriptorDataCodes_t
		{
			SuccessfulConfigRead            = 0, /**< Configuration Descriptor was processed successfully */
			ControlError                    = 1, /**< A control request to the device failed to complete successfully */
			DescriptorTooLarge              = 2, /**< The device's Configuration Descriptor is too large to process */
			InvalidConfigDataReturned       = 3, /**< The device returned an invalid Configuration Descriptor */
			NoCompatibleInterfaceFound      = 4, /**< A compatible interface with the required endpoints was not found */
		};

	/* External Variables: */
		extern uint8_t StreamingInterfaceIndex;
		extern uint8_t StreamingInterfaceAltSetting;
		extern uint8_t StreamingEndpointAddress;

	/* Function Prototypes: */
		uint8_t ProcessConfigurationDescriptor(void);

		uint8_t DComp_NextAudioControlInterface(void* CurrentDescriptor);
		uint8_t DComp_NextAudioStreamInterface(void* CurrentDescriptor);
		uint8_t DComp_NextAudioInterfaceDataEndpoint(void* CurrentDescriptor);

#endif

