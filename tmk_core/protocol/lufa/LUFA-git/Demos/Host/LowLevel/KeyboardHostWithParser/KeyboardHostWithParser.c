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
 *  Main source file for the KeyboardHostWithParser demo. This file contains the main tasks of
 *  the demo and is responsible for the initial application hardware configuration.
 */

#include "KeyboardHostWithParser.h"

/** Main program entry point. This routine configures the hardware required by the application, then
 *  enters a loop to run the application tasks in sequence.
 */
int main(void)
{
	SetupHardware();

	puts_P(PSTR(ESC_FG_CYAN "Keyboard HID Parser Host Demo running.\r\n" ESC_FG_WHITE));

	LEDs_SetAllLEDs(LEDMASK_USB_NOTREADY);
	GlobalInterruptEnable();

	for (;;)
	{
		KeyboardHost_Task();

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
#endif

	/* Hardware Initialization */
	Serial_Init(9600, false);
	LEDs_Init();
	USB_Init();

	/* Create a stdio stream for the serial port for stdin and stdout */
	Serial_CreateStream(NULL);
}

/** Event handler for the USB_DeviceAttached event. This indicates that a device has been attached to the host, and
 *  starts the library USB task to begin the enumeration and USB management process.
 */
void EVENT_USB_Host_DeviceAttached(void)
{
	puts_P(PSTR(ESC_FG_GREEN "Device Attached.\r\n" ESC_FG_WHITE));
	LEDs_SetAllLEDs(LEDMASK_USB_ENUMERATING);
}

/** Event handler for the USB_DeviceUnattached event. This indicates that a device has been removed from the host, and
 *  stops the library USB task management process.
 */
void EVENT_USB_Host_DeviceUnattached(void)
{
	puts_P(PSTR(ESC_FG_GREEN "\r\nDevice Unattached.\r\n" ESC_FG_WHITE));
	LEDs_SetAllLEDs(LEDMASK_USB_NOTREADY);
}

/** Event handler for the USB_DeviceEnumerationComplete event. This indicates that a device has been successfully
 *  enumerated by the host and is now ready to be used by the application.
 */
void EVENT_USB_Host_DeviceEnumerationComplete(void)
{
	puts_P(PSTR("Getting Config Data.\r\n"));

	uint8_t ErrorCode;

	/* Get and process the configuration descriptor data */
	if ((ErrorCode = ProcessConfigurationDescriptor()) != SuccessfulConfigRead)
	{
		if (ErrorCode == ControlError)
		  puts_P(PSTR(ESC_FG_RED "Control Error (Get Configuration).\r\n"));
		else
		  puts_P(PSTR(ESC_FG_RED "Invalid Device.\r\n"));

		printf_P(PSTR(" -- Error Code: %d\r\n" ESC_FG_WHITE), ErrorCode);

		LEDs_SetAllLEDs(LEDMASK_USB_ERROR);
		return;
	}

	/* Set the device configuration to the first configuration (rarely do devices use multiple configurations) */
	if ((ErrorCode = USB_Host_SetDeviceConfiguration(1)) != HOST_SENDCONTROL_Successful)

	{
		puts_P(PSTR(ESC_FG_RED "Control Error (Set Configuration).\r\n"));
		printf_P(PSTR(" -- Error Code: %d\r\n" ESC_FG_WHITE), ErrorCode);

		LEDs_SetAllLEDs(LEDMASK_USB_ERROR);
		return;
	}

	printf_P(PSTR("Processing HID Report (Size %d Bytes).\r\n"), HIDReportSize);

	/* Get and process the device's first HID report descriptor */
	if ((ErrorCode = GetHIDReportData()) != ParseSuccessful)
	{
		puts_P(PSTR(ESC_FG_RED "Report Parse Error.\r\n"));

		if (!(HIDReportInfo.TotalReportItems))
			puts_P(PSTR("Not a valid Keyboard." ESC_FG_WHITE));
		else
			printf_P(PSTR(" -- Error Code: %d\r\n" ESC_FG_WHITE), ErrorCode);

		LEDs_SetAllLEDs(LEDMASK_USB_ERROR);
		USB_Host_SetDeviceConfiguration(0);
		return;
	}

	puts_P(PSTR("Keyboard Enumerated.\r\n"));
	LEDs_SetAllLEDs(LEDMASK_USB_READY);
}

/** Event handler for the USB_HostError event. This indicates that a hardware error occurred while in host mode. */
void EVENT_USB_Host_HostError(const uint8_t ErrorCode)
{
	USB_Disable();

	printf_P(PSTR(ESC_FG_RED "Host Mode Error\r\n"
	                         " -- Error Code %d\r\n" ESC_FG_WHITE), ErrorCode);

	LEDs_SetAllLEDs(LEDMASK_USB_ERROR);
	for(;;);
}

/** Event handler for the USB_DeviceEnumerationFailed event. This indicates that a problem occurred while
 *  enumerating an attached USB device.
 */
void EVENT_USB_Host_DeviceEnumerationFailed(const uint8_t ErrorCode,
                                            const uint8_t SubErrorCode)
{
	printf_P(PSTR(ESC_FG_RED "Dev Enum Error\r\n"
	                         " -- Error Code %d\r\n"
	                         " -- Sub Error Code %d\r\n"
	                         " -- In State %d\r\n" ESC_FG_WHITE), ErrorCode, SubErrorCode, USB_HostState);

	LEDs_SetAllLEDs(LEDMASK_USB_ERROR);
}

/** Task to read in and processes the next report from the attached device, displaying the report
 *  contents on the board LEDs and via the serial port.
 */
void KeyboardHost_Task(void)
{
	if (USB_HostState != HOST_STATE_Configured)
	  return;

	/* Select and unfreeze keyboard data pipe */
	Pipe_SelectPipe(KEYBOARD_DATA_IN_PIPE);
	Pipe_Unfreeze();

	/* Check to see if a packet has been received */
	if (Pipe_IsINReceived())
	{
		/* Check if data has been received from the attached keyboard */
		if (Pipe_IsReadWriteAllowed())
		{
			/* Create buffer big enough for the report */
			uint8_t KeyboardReport[Pipe_BytesInPipe()];

			/* Load in the keyboard report */
			Pipe_Read_Stream_LE(KeyboardReport, Pipe_BytesInPipe(), NULL);

			/* Process the read in keyboard report from the device */
			ProcessKeyboardReport(KeyboardReport);
		}

		/* Clear the IN endpoint, ready for next data packet */
		Pipe_ClearIN();
	}

	/* Freeze keyboard data pipe */
	Pipe_Freeze();
}

/** Processes a read HID report from an attached keyboard, extracting out elements via the HID parser results
 *  as required and prints pressed characters to the serial port. Each time a key is typed, a board LED is toggled.
 *
 *  \param[in] KeyboardReport  Pointer to a HID report from an attached keyboard device
 */
void ProcessKeyboardReport(uint8_t* KeyboardReport)
{
	/* Check each HID report item in turn, looking for keyboard scan code reports */
	for (uint8_t ReportNumber = 0; ReportNumber < HIDReportInfo.TotalReportItems; ReportNumber++)
	{
		/* Create a temporary item pointer to the next report item */
		HID_ReportItem_t* ReportItem = &HIDReportInfo.ReportItems[ReportNumber];

		/* Check if the current report item is a keyboard scan-code */
		if ((ReportItem->Attributes.Usage.Page      == USAGE_PAGE_KEYBOARD) &&
			(ReportItem->Attributes.BitSize         == 8)                   &&
			(ReportItem->Attributes.Logical.Maximum > 1)                    &&
			(ReportItem->ItemType                   == HID_REPORT_ITEM_In))
		{
			/* Retrieve the keyboard scan-code from the report data retrieved from the device */
			bool FoundData = USB_GetHIDReportItemInfo(KeyboardReport, ReportItem);

			/* For multi-report devices - if the requested data was not in the issued report, continue */
			if (!(FoundData))
			  continue;

			/* Key code is an unsigned char in length, cast to the appropriate type */
			uint8_t KeyCode = (uint8_t)ReportItem->Value;

			/* If scan-code is non-zero, a key is being pressed */
			if (KeyCode)
			{
				/* Toggle status LED to indicate keypress */
				LEDs_ToggleLEDs(LEDS_LED2);

				char PressedKey = 0;

				/* Retrieve pressed key character if alphanumeric */
				if ((KeyCode >= HID_KEYBOARD_SC_A) && (KeyCode <= HID_KEYBOARD_SC_Z))
				{
					PressedKey = (KeyCode - HID_KEYBOARD_SC_A) + 'A';
				}
				else if ((KeyCode >= HID_KEYBOARD_SC_1_AND_EXCLAMATION) &
						 (KeyCode  < HID_KEYBOARD_SC_0_AND_CLOSING_PARENTHESIS))
				{
					PressedKey = (KeyCode - HID_KEYBOARD_SC_1_AND_EXCLAMATION) + '1';
				}
				else if (KeyCode == HID_KEYBOARD_SC_0_AND_CLOSING_PARENTHESIS)
				{
					PressedKey = '0';
				}
				else if (KeyCode == HID_KEYBOARD_SC_SPACE)
				{
					PressedKey = ' ';
				}
				else if (KeyCode == HID_KEYBOARD_SC_ENTER)
				{
					PressedKey = '\n';
				}

				/* Print the pressed key character out through the serial port if valid */
				if (PressedKey)
				  putchar(PressedKey);
			}

			/* Once a scan-code is found, stop scanning through the report items */
			break;
		}
	}
}

