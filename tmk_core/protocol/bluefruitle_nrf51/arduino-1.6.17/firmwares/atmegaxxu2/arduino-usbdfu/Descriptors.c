/*
             LUFA Library
     Copyright (C) Dean Camera, 2010.
              
  dean [at] fourwalledcubicle [dot] com
      www.fourwalledcubicle.com
*/

/*
  Copyright 2010  Dean Camera (dean [at] fourwalledcubicle [dot] com)

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
 *
 *  USB Device Descriptors, for library use when in USB device mode. Descriptors are special 
 *  computer-readable structures which the host requests upon device enumeration, to determine
 *  the device's capabilities and functions.  
 */

#include "Descriptors.h"

/** Device descriptor structure. This descriptor, located in FLASH memory, describes the overall
 *  device characteristics, including the supported USB version, control endpoint size and the
 *  number of device configurations. The descriptor is read out by the USB host when the enumeration
 *  process begins.
 */
USB_Descriptor_Device_t DeviceDescriptor =
{
	.Header                 = {.Size = sizeof(USB_Descriptor_Device_t), .Type = DTYPE_Device},
		
	.USBSpecification       = VERSION_BCD(01.10),
	.Class                  = 0x00,
	.SubClass               = 0x00,
	.Protocol               = 0x00,
				
	.Endpoint0Size          = FIXED_CONTROL_ENDPOINT_SIZE,
		
	.VendorID               = 0x03EB, // Atmel
	.ProductID              = PRODUCT_ID_CODE, // MCU-dependent
	.ReleaseNumber          = 0x0000,
		
	.ManufacturerStrIndex   = NO_DESCRIPTOR,
	.ProductStrIndex        = 0x01,
	.SerialNumStrIndex      = NO_DESCRIPTOR,
		
	.NumberOfConfigurations = FIXED_NUM_CONFIGURATIONS
};

/** Configuration descriptor structure. This descriptor, located in FLASH memory, describes the usage
 *  of the device in one of its supported configurations, including information about any device interfaces
 *  and endpoints. The descriptor is read out by the USB host during the enumeration process when selecting
 *  a configuration so that the host may correctly communicate with the USB device.
 */
USB_Descriptor_Configuration_t ConfigurationDescriptor =
{
	.Config = 
		{
			.Header                   = {.Size = sizeof(USB_Descriptor_Configuration_Header_t), .Type = DTYPE_Configuration},

			.TotalConfigurationSize   = sizeof(USB_Descriptor_Configuration_t),
			.TotalInterfaces          = 1,

			.ConfigurationNumber      = 1,
			.ConfigurationStrIndex    = NO_DESCRIPTOR,
				
			.ConfigAttributes         = USB_CONFIG_ATTR_BUSPOWERED,
			
			.MaxPowerConsumption      = USB_CONFIG_POWER_MA(100)
		},
		
	.DFU_Interface = 
		{
			.Header                 = {.Size = sizeof(USB_Descriptor_Interface_t), .Type = DTYPE_Interface},

			.InterfaceNumber        = 0,
			.AlternateSetting       = 0,
			
			.TotalEndpoints         = 0,
				
			.Class                  = 0xFE,
			.SubClass               = 0x01,
			.Protocol               = 0x02,

			.InterfaceStrIndex      = NO_DESCRIPTOR
		},
		
	.DFU_Functional = 
		{
			.Header                 = {.Size = sizeof(USB_DFU_Functional_Descriptor_t), .Type = DTYPE_DFUFunctional},
			
			.Attributes             = (ATTR_CAN_UPLOAD | ATTR_CAN_DOWNLOAD),

			.DetachTimeout          = 0x0000,
			.TransferSize           = 0x0c00,
		
			.DFUSpecification       = VERSION_BCD(01.01)
		}
};

/** Language descriptor structure. This descriptor, located in FLASH memory, is returned when the host requests
 *  the string descriptor with index 0 (the first index). It is actually an array of 16-bit integers, which indicate
 *  via the language ID table available at USB.org what languages the device supports for its string descriptors.
 */ 
USB_Descriptor_String_t LanguageString =
{
	.Header                 = {.Size = USB_STRING_LEN(1), .Type = DTYPE_String},
		
	.UnicodeString          = {LANGUAGE_ID_ENG}
};

/** Product descriptor string. This is a Unicode string containing the product's details in human readable form,
 *  and is read out upon request by the host when the appropriate string ID is requested, listed in the Device
 *  Descriptor.
 */
USB_Descriptor_String_t ProductString =
{
	#if (ARDUINO_MODEL_PID == ARDUINO_UNO_PID)
		.Header                 = {.Size = USB_STRING_LEN(15), .Type = DTYPE_String},
			
		.UnicodeString          = L"Arduino Uno DFU"
	#elif (ARDUINO_MODEL_PID == ARDUINO_MEGA2560_PID)
		.Header                 = {.Size = USB_STRING_LEN(21), .Type = DTYPE_String},
			
		.UnicodeString          = L"Arduino Mega 2560 DFU"
	#endif
};

/** This function is called by the library when in device mode, and must be overridden (see library "USB Descriptors"
 *  documentation) by the application code so that the address and size of a requested descriptor can be given
 *  to the USB library. When the device receives a Get Descriptor request on the control endpoint, this function
 *  is called so that the descriptor details can be passed back and the appropriate descriptor sent back to the
 *  USB host.
 */
uint16_t CALLBACK_USB_GetDescriptor(const uint16_t wValue,
                                    const uint8_t wIndex,
                                    void** const DescriptorAddress)
{
	const uint8_t  DescriptorType   = (wValue >> 8);
	const uint8_t  DescriptorNumber = (wValue & 0xFF);

	void*    Address = NULL;
	uint16_t Size    = NO_DESCRIPTOR;

	switch (DescriptorType)
	{
		case DTYPE_Device:
			Address = &DeviceDescriptor;
			Size    = sizeof(USB_Descriptor_Device_t);
			break;
		case DTYPE_Configuration: 
			Address = &ConfigurationDescriptor;
			Size    = sizeof(USB_Descriptor_Configuration_t);
			break;
		case DTYPE_String: 
			if (!(DescriptorNumber))
			{
				Address = &LanguageString;
				Size    = LanguageString.Header.Size;
			}
			else
			{
				Address = &ProductString;
				Size    = ProductString.Header.Size;
			}
			
			break;
	}
	
	*DescriptorAddress = Address;
	return Size;
}
