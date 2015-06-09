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
 *  USB Device Descriptor processing routines, to determine the overall device parameters. Descriptors are special
 *  computer-readable structures which the host requests upon device enumeration, to determine information about
 *  the attached device.
 */

#include "DeviceDescriptor.h"

/** Reads and processes an attached device's Device Descriptor, to determine compatibility
 *
 *  This routine checks to ensure that the attached device's VID and PID matches Google's for Android devices.
 *
 *  \return An error code from the \ref AndroidHost_GetDeviceDescriptorDataCodes_t enum.
 */
uint8_t ProcessDeviceDescriptor(void)
{
	USB_Descriptor_Device_t DeviceDescriptor;

	/* Send the request to retrieve the device descriptor */
	if (USB_Host_GetDeviceDescriptor(&DeviceDescriptor) != HOST_SENDCONTROL_Successful)
	  return DevControlError;

	/* Validate returned data - ensure the returned data is a device descriptor */
	if (DeviceDescriptor.Header.Type != DTYPE_Device)
	  return InvalidDeviceDataReturned;

	/* Check the product ID to determine if the Android device is in accessory mode */
	if ((DeviceDescriptor.ProductID != ANDROID_ACCESSORY_PRODUCT_ID) &&
	    (DeviceDescriptor.ProductID != ANDROID_ACCESSORY_ADB_PRODUCT_ID))
	{
		return NonAccessoryModeAndroidDevice;
	}

	return AccessoryModeAndroidDevice;
}

