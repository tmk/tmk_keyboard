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

#define  __INCLUDE_FROM_USB_DRIVER
#include "../../Core/USBMode.h"

#if defined(USB_CAN_BE_DEVICE)

#define  __INCLUDE_FROM_AUDIO_DRIVER
#define  __INCLUDE_FROM_AUDIO_DEVICE_C
#include "AudioClassDevice.h"

void Audio_Device_ProcessControlRequest(USB_ClassInfo_Audio_Device_t* const AudioInterfaceInfo)
{
	if (!(Endpoint_IsSETUPReceived()))
	  return;

	if ((USB_ControlRequest.bmRequestType & CONTROL_REQTYPE_RECIPIENT) == REQREC_INTERFACE)
	{
		uint8_t InterfaceIndex = (USB_ControlRequest.wIndex & 0xFF);

		if ((InterfaceIndex != AudioInterfaceInfo->Config.ControlInterfaceNumber) &&
		    (InterfaceIndex != AudioInterfaceInfo->Config.StreamingInterfaceNumber))
		{
			return;
		}
	}
	else if ((USB_ControlRequest.bmRequestType & CONTROL_REQTYPE_RECIPIENT) == REQREC_ENDPOINT)
	{
		uint8_t EndpointAddress = (USB_ControlRequest.wIndex & 0xFF);

		if ((EndpointAddress != AudioInterfaceInfo->Config.DataINEndpoint.Address) &&
		    (EndpointAddress != AudioInterfaceInfo->Config.DataOUTEndpoint.Address))
		{
			return;
		}
	}

	switch (USB_ControlRequest.bRequest)
	{
		case REQ_SetInterface:
			if (USB_ControlRequest.bmRequestType == (REQDIR_HOSTTODEVICE | REQTYPE_STANDARD | REQREC_INTERFACE))
			{
				Endpoint_ClearSETUP();
				Endpoint_ClearStatusStage();

				AudioInterfaceInfo->State.InterfaceEnabled = ((USB_ControlRequest.wValue & 0xFF) != 0);
				EVENT_Audio_Device_StreamStartStop(AudioInterfaceInfo);
			}

			break;
		case AUDIO_REQ_GetStatus:
			if ((USB_ControlRequest.bmRequestType == (REQDIR_HOSTTODEVICE | REQTYPE_CLASS | REQREC_INTERFACE)) ||
			    (USB_ControlRequest.bmRequestType == (REQDIR_HOSTTODEVICE | REQTYPE_CLASS | REQREC_ENDPOINT)))
			{
				Endpoint_ClearSETUP();
				Endpoint_ClearStatusStage();
			}

			break;
		case AUDIO_REQ_SetCurrent:
		case AUDIO_REQ_SetMinimum:
		case AUDIO_REQ_SetMaximum:
		case AUDIO_REQ_SetResolution:
			if ((USB_ControlRequest.bmRequestType & CONTROL_REQTYPE_RECIPIENT) == REQREC_ENDPOINT)
			{
				uint8_t EndpointProperty = USB_ControlRequest.bRequest;
				uint8_t EndpointAddress  = (uint8_t)USB_ControlRequest.wIndex;
				uint8_t EndpointControl  = (USB_ControlRequest.wValue >> 8);

				if (CALLBACK_Audio_Device_GetSetEndpointProperty(AudioInterfaceInfo, EndpointProperty, EndpointAddress,
				                                                 EndpointControl, NULL, NULL))
				{
					uint16_t ValueLength = USB_ControlRequest.wLength;
					uint8_t  Value[ValueLength];

					Endpoint_ClearSETUP();
					Endpoint_Read_Control_Stream_LE(Value, ValueLength);
					Endpoint_ClearIN();

					CALLBACK_Audio_Device_GetSetEndpointProperty(AudioInterfaceInfo, EndpointProperty, EndpointAddress,
					                                             EndpointControl, &ValueLength, Value);
				}
			}
			else if ((USB_ControlRequest.bmRequestType & CONTROL_REQTYPE_RECIPIENT) == REQREC_INTERFACE)
			{
				uint8_t  Property  = USB_ControlRequest.bRequest;
				uint8_t  Entity    = (USB_ControlRequest.wIndex >> 8);
				uint16_t Parameter = USB_ControlRequest.wValue;

				if (CALLBACK_Audio_Device_GetSetInterfaceProperty(AudioInterfaceInfo, Property, Entity,
				                                                  Parameter, NULL, NULL))
				{
					uint16_t ValueLength = USB_ControlRequest.wLength;
					uint8_t  Value[ValueLength];

					Endpoint_ClearSETUP();
					Endpoint_Read_Control_Stream_LE(Value, ValueLength);
					Endpoint_ClearIN();

					CALLBACK_Audio_Device_GetSetInterfaceProperty(AudioInterfaceInfo, Property, Entity,
				                                                  Parameter, &ValueLength, Value);
				}
			}

			break;
		case AUDIO_REQ_GetCurrent:
		case AUDIO_REQ_GetMinimum:
		case AUDIO_REQ_GetMaximum:
		case AUDIO_REQ_GetResolution:
			if ((USB_ControlRequest.bmRequestType & CONTROL_REQTYPE_RECIPIENT) == REQREC_ENDPOINT)
			{
				uint8_t  EndpointProperty = USB_ControlRequest.bRequest;
				uint8_t  EndpointAddress  = (uint8_t)USB_ControlRequest.wIndex;
				uint8_t  EndpointControl  = (USB_ControlRequest.wValue >> 8);
				uint16_t ValueLength      = USB_ControlRequest.wLength;
				uint8_t  Value[ValueLength];

				if (CALLBACK_Audio_Device_GetSetEndpointProperty(AudioInterfaceInfo, EndpointProperty, EndpointAddress,
				                                                 EndpointControl, &ValueLength, Value))
				{
					Endpoint_ClearSETUP();
					Endpoint_Write_Control_Stream_LE(Value, ValueLength);
					Endpoint_ClearOUT();
				}
			}
			else if ((USB_ControlRequest.bmRequestType & CONTROL_REQTYPE_RECIPIENT) == REQREC_INTERFACE)
			{
				uint8_t  Property    = USB_ControlRequest.bRequest;
				uint8_t  Entity      = (USB_ControlRequest.wIndex >> 8);
				uint16_t Parameter   = USB_ControlRequest.wValue;
				uint16_t ValueLength = USB_ControlRequest.wLength;
				uint8_t  Value[ValueLength];

				if (CALLBACK_Audio_Device_GetSetInterfaceProperty(AudioInterfaceInfo, Property, Entity,
				                                                  Parameter, &ValueLength, Value))
				{
					Endpoint_ClearSETUP();
					Endpoint_Write_Control_Stream_LE(Value, ValueLength);
					Endpoint_ClearOUT();
				}
			}

			break;
	}
}

bool Audio_Device_ConfigureEndpoints(USB_ClassInfo_Audio_Device_t* const AudioInterfaceInfo)
{
	memset(&AudioInterfaceInfo->State, 0x00, sizeof(AudioInterfaceInfo->State));

	AudioInterfaceInfo->Config.DataINEndpoint.Type  = EP_TYPE_ISOCHRONOUS;
	AudioInterfaceInfo->Config.DataOUTEndpoint.Type = EP_TYPE_ISOCHRONOUS;

	if (!(Endpoint_ConfigureEndpointTable(&AudioInterfaceInfo->Config.DataINEndpoint, 1)))
	  return false;

	if (!(Endpoint_ConfigureEndpointTable(&AudioInterfaceInfo->Config.DataOUTEndpoint, 1)))
	  return false;

	return true;
}

void Audio_Device_Event_Stub(void)
{

}

#endif

