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
 *  Main source file for the MIDI input demo. This file contains the main tasks of the demo and
 *  is responsible for the initial application hardware configuration.
 */

#include "MIDI.h"

/** Main program entry point. This routine configures the hardware required by the application, then
 *  enters a loop to run the application tasks in sequence.
 */
int main(void)
{
	SetupHardware();

	LEDs_SetAllLEDs(LEDMASK_USB_NOTREADY);
	GlobalInterruptEnable();

	for (;;)
	{
		MIDI_Task();
		USB_USBTask();
	}
}

/** Configures the board hardware and chip peripherals for the demo's functionality. */
void SetupHardware(void)
{
#if (ARCH == ARCH_AVR8)
	/* Disable watchdog if enabled by bootloader/fuses */
	MCUSR &= ~(1 << WDRF);
	wdt_disable();

	/* Disable clock division */
	clock_prescale_set(clock_div_1);
#elif (ARCH == ARCH_XMEGA)
	/* Start the PLL to multiply the 2MHz RC oscillator to 32MHz and switch the CPU core to run from it */
	XMEGACLK_StartPLL(CLOCK_SRC_INT_RC2MHZ, 2000000, F_CPU);
	XMEGACLK_SetCPUClockSource(CLOCK_SRC_PLL);

	/* Start the 32MHz internal RC oscillator and start the DFLL to increase it to 48MHz using the USB SOF as a reference */
	XMEGACLK_StartInternalOscillator(CLOCK_SRC_INT_RC32MHZ);
	XMEGACLK_StartDFLL(CLOCK_SRC_INT_RC32MHZ, DFLL_REF_INT_USBSOF, F_USB);

	PMIC.CTRL = PMIC_LOLVLEN_bm | PMIC_MEDLVLEN_bm | PMIC_HILVLEN_bm;
#endif

	/* Hardware Initialization */
	Joystick_Init();
	LEDs_Init();
	Buttons_Init();
	USB_Init();
}

/** Event handler for the USB_Connect event. This indicates that the device is enumerating via the status LEDs. */
void EVENT_USB_Device_Connect(void)
{
	/* Indicate USB enumerating */
	LEDs_SetAllLEDs(LEDMASK_USB_ENUMERATING);
}

/** Event handler for the USB_Disconnect event. This indicates that the device is no longer connected to a host via
 *  the status LEDs, disables the sample update and PWM output timers and stops the USB and MIDI management tasks.
 */
void EVENT_USB_Device_Disconnect(void)
{
	/* Indicate USB not ready */
	LEDs_SetAllLEDs(LEDMASK_USB_NOTREADY);
}

/** Event handler for the USB_ConfigurationChanged event. This is fired when the host set the current configuration
 *  of the USB device after enumeration - the device endpoints are configured and the MIDI management task started.
 */
void EVENT_USB_Device_ConfigurationChanged(void)
{
	bool ConfigSuccess = true;

	/* Setup MIDI Data Endpoints */
	ConfigSuccess &= Endpoint_ConfigureEndpoint(MIDI_STREAM_IN_EPADDR, EP_TYPE_BULK, MIDI_STREAM_EPSIZE, 1);
	ConfigSuccess &= Endpoint_ConfigureEndpoint(MIDI_STREAM_OUT_EPADDR, EP_TYPE_BULK, MIDI_STREAM_EPSIZE, 1);

	/* Indicate endpoint configuration success or failure */
	LEDs_SetAllLEDs(ConfigSuccess ? LEDMASK_USB_READY : LEDMASK_USB_ERROR);
}

/** Task to handle the generation of MIDI note change events in response to presses of the board joystick, and send them
 *  to the host.
 */
void MIDI_Task(void)
{
	static uint8_t PrevJoystickStatus;

	/* Device must be connected and configured for the task to run */
	if (USB_DeviceState != DEVICE_STATE_Configured)
	  return;

	Endpoint_SelectEndpoint(MIDI_STREAM_IN_EPADDR);

	if (Endpoint_IsINReady())
	{
		uint8_t MIDICommand = 0;
		uint8_t MIDIPitch;

		uint8_t JoystickStatus  = Joystick_GetStatus();
		uint8_t JoystickChanges = (JoystickStatus ^ PrevJoystickStatus);

		/* Get board button status - if pressed use channel 10 (percussion), otherwise use channel 1 */
		uint8_t Channel = ((Buttons_GetStatus() & BUTTONS_BUTTON1) ? MIDI_CHANNEL(10) : MIDI_CHANNEL(1));

		if (JoystickChanges & JOY_LEFT)
		{
			MIDICommand = ((JoystickStatus & JOY_LEFT)? MIDI_COMMAND_NOTE_ON : MIDI_COMMAND_NOTE_OFF);
			MIDIPitch   = 0x3C;
		}

		if (JoystickChanges & JOY_UP)
		{
			MIDICommand = ((JoystickStatus & JOY_UP)? MIDI_COMMAND_NOTE_ON : MIDI_COMMAND_NOTE_OFF);
			MIDIPitch   = 0x3D;
		}

		if (JoystickChanges & JOY_RIGHT)
		{
			MIDICommand = ((JoystickStatus & JOY_RIGHT)? MIDI_COMMAND_NOTE_ON : MIDI_COMMAND_NOTE_OFF);
			MIDIPitch   = 0x3E;
		}

		if (JoystickChanges & JOY_DOWN)
		{
			MIDICommand = ((JoystickStatus & JOY_DOWN)? MIDI_COMMAND_NOTE_ON : MIDI_COMMAND_NOTE_OFF);
			MIDIPitch   = 0x3F;
		}

		if (JoystickChanges & JOY_PRESS)
		{
			MIDICommand = ((JoystickStatus & JOY_PRESS)? MIDI_COMMAND_NOTE_ON : MIDI_COMMAND_NOTE_OFF);
			MIDIPitch   = 0x3B;
		}

		/* Check if a MIDI command is to be sent */
		if (MIDICommand)
		{
			MIDI_EventPacket_t MIDIEvent = (MIDI_EventPacket_t)
				{
					.Event       = MIDI_EVENT(0, MIDICommand),

					.Data1       = MIDICommand | Channel,
					.Data2       = MIDIPitch,
					.Data3       = MIDI_STANDARD_VELOCITY,
				};

			/* Write the MIDI event packet to the endpoint */
			Endpoint_Write_Stream_LE(&MIDIEvent, sizeof(MIDIEvent), NULL);

			/* Send the data in the endpoint to the host */
			Endpoint_ClearIN();
		}

		/* Save previous joystick value for next joystick change detection */
		PrevJoystickStatus = JoystickStatus;
	}

	/* Select the MIDI OUT stream */
	Endpoint_SelectEndpoint(MIDI_STREAM_OUT_EPADDR);

	/* Check if a MIDI command has been received */
	if (Endpoint_IsOUTReceived())
	{
		MIDI_EventPacket_t MIDIEvent;

		/* Read the MIDI event packet from the endpoint */
		Endpoint_Read_Stream_LE(&MIDIEvent, sizeof(MIDIEvent), NULL);

		/* Check to see if the sent command is a note on message with a non-zero velocity */
		if ((MIDIEvent.Event == MIDI_EVENT(0, MIDI_COMMAND_NOTE_ON)) && (MIDIEvent.Data3 > 0))
		{
			/* Change LEDs depending on the pitch of the sent note */
			LEDs_SetAllLEDs(MIDIEvent.Data2 > 64 ? LEDS_LED1 : LEDS_LED2);
		}
		else
		{
			/* Turn off all LEDs in response to non Note On messages */
			LEDs_SetAllLEDs(LEDS_NO_LEDS);
		}

		/* If the endpoint is now empty, clear the bank */
		if (!(Endpoint_BytesInEndpoint()))
		{
			/* Clear the endpoint ready for new packet */
			Endpoint_ClearOUT();
		}
	}
}

