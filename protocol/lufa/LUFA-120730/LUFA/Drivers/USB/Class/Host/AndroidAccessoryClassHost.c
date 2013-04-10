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

#define  __INCLUDE_FROM_USB_DRIVER
#include "../../Core/USBMode.h"

#if defined(USB_CAN_BE_HOST)

#define  __INCLUDE_FROM_AOA_DRIVER
#define  __INCLUDE_FROM_ANDROIDACCESSORY_HOST_C
#include "AndroidAccessoryClassHost.h"

bool AOA_Host_ValidateAccessoryDevice(USB_ClassInfo_AOA_Host_t* const AOAInterfaceInfo,
                                      const USB_Descriptor_Device_t* const DeviceDescriptor,
                                      bool* const NeedModeSwitch)
{
	(void)AOAInterfaceInfo;

	if (DeviceDescriptor->Header.Type != DTYPE_Device)
	  return false;

	*NeedModeSwitch = ((DeviceDescriptor->ProductID != ANDROID_ACCESSORY_PRODUCT_ID) &&
	                   (DeviceDescriptor->ProductID != ANDROID_ACCESSORY_ADB_PRODUCT_ID));

	return true;
}

uint8_t AOA_Host_ConfigurePipes(USB_ClassInfo_AOA_Host_t* const AOAInterfaceInfo,
                                uint16_t ConfigDescriptorSize,
                                void* ConfigDescriptorData)
{
	USB_Descriptor_Endpoint_t*  DataINEndpoint  = NULL;
	USB_Descriptor_Endpoint_t*  DataOUTEndpoint = NULL;
	USB_Descriptor_Interface_t* AOAInterface    = NULL;

	memset(&AOAInterfaceInfo->State, 0x00, sizeof(AOAInterfaceInfo->State));

	if (DESCRIPTOR_TYPE(ConfigDescriptorData) != DTYPE_Configuration)
	  return AOA_ENUMERROR_InvalidConfigDescriptor;
	
	if (USB_GetNextDescriptorComp(&ConfigDescriptorSize, &ConfigDescriptorData,
	                              DCOMP_AOA_Host_NextAndroidAccessoryInterface) != DESCRIPTOR_SEARCH_COMP_Found)
	{
		return AOA_ENUMERROR_NoCompatibleInterfaceFound;
	}
	
	AOAInterface = DESCRIPTOR_PCAST(ConfigDescriptorData, USB_Descriptor_Interface_t);

	while (!(DataINEndpoint) || !(DataOUTEndpoint))
	{
		if (USB_GetNextDescriptorComp(&ConfigDescriptorSize, &ConfigDescriptorData,
		                              DCOMP_AOA_Host_NextInterfaceBulkEndpoint) != DESCRIPTOR_SEARCH_COMP_Found)
		{
			return AOA_ENUMERROR_NoCompatibleInterfaceFound;
		}

		USB_Descriptor_Endpoint_t* EndpointData = DESCRIPTOR_PCAST(ConfigDescriptorData, USB_Descriptor_Endpoint_t);

		if ((EndpointData->EndpointAddress & ENDPOINT_DIR_MASK) == ENDPOINT_DIR_IN)
		  DataINEndpoint  = EndpointData;
		else
		  DataOUTEndpoint = EndpointData;
	}

	AOAInterfaceInfo->Config.DataINPipe.Size  = le16_to_cpu(DataINEndpoint->EndpointSize);
	AOAInterfaceInfo->Config.DataINPipe.EndpointAddress = DataINEndpoint->EndpointAddress;
	AOAInterfaceInfo->Config.DataINPipe.Type  = EP_TYPE_BULK;
	
	AOAInterfaceInfo->Config.DataOUTPipe.Size = le16_to_cpu(DataOUTEndpoint->EndpointSize);
	AOAInterfaceInfo->Config.DataOUTPipe.EndpointAddress = DataOUTEndpoint->EndpointAddress;
	AOAInterfaceInfo->Config.DataOUTPipe.Type = EP_TYPE_BULK;
	
	if (!(Pipe_ConfigurePipeTable(&AOAInterfaceInfo->Config.DataINPipe, 1)))
	  return false;
	
	if (!(Pipe_ConfigurePipeTable(&AOAInterfaceInfo->Config.DataOUTPipe, 1)))
	  return false;

	AOAInterfaceInfo->State.IsActive        = true;
	AOAInterfaceInfo->State.InterfaceNumber = AOAInterface->InterfaceNumber;

	return AOA_ENUMERROR_NoError;
}

static uint8_t DCOMP_AOA_Host_NextAndroidAccessoryInterface(void* const CurrentDescriptor)
{
	USB_Descriptor_Header_t* Header = DESCRIPTOR_PCAST(CurrentDescriptor, USB_Descriptor_Header_t);

	if (Header->Type == DTYPE_Interface)
	{
		USB_Descriptor_Interface_t* Interface = DESCRIPTOR_PCAST(CurrentDescriptor, USB_Descriptor_Interface_t);

		if ((Interface->Class    == AOA_CSCP_AOADataClass)    &&
		    (Interface->SubClass == AOA_CSCP_AOADataSubclass) &&
		    (Interface->Protocol == AOA_CSCP_AOADataProtocol))
		{
			return DESCRIPTOR_SEARCH_Found;
		}
	}

	return DESCRIPTOR_SEARCH_NotFound;
}

static uint8_t DCOMP_AOA_Host_NextInterfaceBulkEndpoint(void* const CurrentDescriptor)
{
	USB_Descriptor_Header_t* Header = DESCRIPTOR_PCAST(CurrentDescriptor, USB_Descriptor_Header_t);

	if (Header->Type == DTYPE_Endpoint)
	{
		USB_Descriptor_Endpoint_t* Endpoint = DESCRIPTOR_PCAST(CurrentDescriptor, USB_Descriptor_Endpoint_t);

		uint8_t EndpointType = (Endpoint->Attributes & EP_TYPE_MASK);

		if ((EndpointType == EP_TYPE_BULK) && (!(Pipe_IsEndpointBound(Endpoint->EndpointAddress))))
		  return DESCRIPTOR_SEARCH_Found;
	}
	else if (Header->Type == DTYPE_Interface)
	{
		return DESCRIPTOR_SEARCH_Fail;
	}

	return DESCRIPTOR_SEARCH_NotFound;
}

void AOA_Host_USBTask(USB_ClassInfo_AOA_Host_t* const AOAInterfaceInfo)
{
	if ((USB_HostState != HOST_STATE_Configured) || !(AOAInterfaceInfo->State.IsActive))
	  return;

	#if !defined(NO_CLASS_DRIVER_AUTOFLUSH)
	AOA_Host_Flush(AOAInterfaceInfo);
	#endif
}

uint8_t AOA_Host_StartAccessoryMode(USB_ClassInfo_AOA_Host_t* const AOAInterfaceInfo)
{
	uint8_t ErrorCode;
	
	uint16_t AccessoryProtocol;
	if ((ErrorCode = AOA_Host_GetAccessoryProtocol(&AccessoryProtocol)) != HOST_WAITERROR_Successful)
	  return ErrorCode;

	if (AccessoryProtocol != CPU_TO_LE16(AOA_PROTOCOL_AccessoryV1))
	  return AOA_ERROR_LOGICAL_CMD_FAILED;

	for (uint8_t PropertyIndex = 0; PropertyIndex < AOA_STRING_TOTAL_STRINGS; PropertyIndex++)
	{
		if ((ErrorCode = AOA_Host_SendPropertyString(AOAInterfaceInfo, PropertyIndex)) != HOST_WAITERROR_Successful)
		  return ErrorCode;
	}

	USB_ControlRequest = (USB_Request_Header_t)
	{
		.bmRequestType = (REQDIR_HOSTTODEVICE | REQTYPE_VENDOR | REQREC_DEVICE),
		.bRequest      = AOA_REQ_StartAccessoryMode,
		.wValue        = 0,
		.wIndex        = 0,
		.wLength       = 0,
	};

	Pipe_SelectPipe(PIPE_CONTROLPIPE);
	return USB_Host_SendControlRequest(NULL);	
}

static uint8_t AOA_Host_GetAccessoryProtocol(uint16_t* const Protocol)
{
	USB_ControlRequest = (USB_Request_Header_t)
	{
		.bmRequestType = (REQDIR_DEVICETOHOST | REQTYPE_VENDOR | REQREC_DEVICE),
		.bRequest      = AOA_REQ_GetAccessoryProtocol,
		.wValue        = 0,
		.wIndex        = 0,
		.wLength       = sizeof(uint16_t),
	};

	Pipe_SelectPipe(PIPE_CONTROLPIPE);
	return USB_Host_SendControlRequest(Protocol);
}

static uint8_t AOA_Host_SendPropertyString(USB_ClassInfo_AOA_Host_t* const AOAInterfaceInfo,
                                           const uint8_t StringIndex)
{	
	const char* String = AOAInterfaceInfo->Config.PropertyStrings[StringIndex];
	
	if (String == NULL)
	  String = "";

	USB_ControlRequest = (USB_Request_Header_t)
	{
		.bmRequestType = (REQDIR_HOSTTODEVICE | REQTYPE_VENDOR | REQREC_DEVICE),
		.bRequest      = AOA_REQ_SendString,
		.wValue        = 0,
		.wIndex        = StringIndex,
		.wLength       = (strlen(String) + 1),
	};

	Pipe_SelectPipe(PIPE_CONTROLPIPE);
	return USB_Host_SendControlRequest((char*)String);
}

uint8_t AOA_Host_SendData(USB_ClassInfo_AOA_Host_t* const AOAInterfaceInfo,
                          const uint8_t* const Buffer,
                          const uint16_t Length)
{
	if ((USB_HostState != HOST_STATE_Configured) || !(AOAInterfaceInfo->State.IsActive))
	  return PIPE_READYWAIT_DeviceDisconnected;

	uint8_t ErrorCode;

	Pipe_SelectPipe(AOAInterfaceInfo->Config.DataOUTPipe.Address);

	Pipe_Unfreeze();
	ErrorCode = Pipe_Write_Stream_LE(Buffer, Length, NULL);
	Pipe_Freeze();

	return ErrorCode;
}

uint8_t AOA_Host_SendString(USB_ClassInfo_AOA_Host_t* const AOAInterfaceInfo,
                            const char* const String)
{
	if ((USB_HostState != HOST_STATE_Configured) || !(AOAInterfaceInfo->State.IsActive))
	  return PIPE_READYWAIT_DeviceDisconnected;

	uint8_t ErrorCode;

	Pipe_SelectPipe(AOAInterfaceInfo->Config.DataOUTPipe.Address);

	Pipe_Unfreeze();
	ErrorCode = Pipe_Write_Stream_LE(String, strlen(String), NULL);
	Pipe_Freeze();

	return ErrorCode;
}

uint8_t AOA_Host_SendByte(USB_ClassInfo_AOA_Host_t* const AOAInterfaceInfo,
                          const uint8_t Data)
{
	if ((USB_HostState != HOST_STATE_Configured) || !(AOAInterfaceInfo->State.IsActive))
	  return PIPE_READYWAIT_DeviceDisconnected;

	uint8_t ErrorCode;

	Pipe_SelectPipe(AOAInterfaceInfo->Config.DataOUTPipe.Address);
	Pipe_Unfreeze();

	if (!(Pipe_IsReadWriteAllowed()))
	{
		Pipe_ClearOUT();

		if ((ErrorCode = Pipe_WaitUntilReady()) != PIPE_READYWAIT_NoError)
		  return ErrorCode;
	}

	Pipe_Write_8(Data);
	Pipe_Freeze();

	return PIPE_READYWAIT_NoError;
}

uint16_t AOA_Host_BytesReceived(USB_ClassInfo_AOA_Host_t* const AOAInterfaceInfo)
{
	if ((USB_HostState != HOST_STATE_Configured) || !(AOAInterfaceInfo->State.IsActive))
	  return 0;

	Pipe_SelectPipe(AOAInterfaceInfo->Config.DataINPipe.Address);
	Pipe_Unfreeze();

	if (Pipe_IsINReceived())
	{
		if (!(Pipe_BytesInPipe()))
		{
			Pipe_ClearIN();
			Pipe_Freeze();
			return 0;
		}
		else
		{
			Pipe_Freeze();
			return Pipe_BytesInPipe();
		}
	}
	else
	{
		Pipe_Freeze();

		return 0;
	}
}

int16_t AOA_Host_ReceiveByte(USB_ClassInfo_AOA_Host_t* const AOAInterfaceInfo)
{
	if ((USB_HostState != HOST_STATE_Configured) || !(AOAInterfaceInfo->State.IsActive))
	  return -1;

	int16_t ReceivedByte = -1;

	Pipe_SelectPipe(AOAInterfaceInfo->Config.DataINPipe.Address);
	Pipe_Unfreeze();

	if (Pipe_IsINReceived())
	{
		if (Pipe_BytesInPipe())
		  ReceivedByte = Pipe_Read_8();

		if (!(Pipe_BytesInPipe()))
		  Pipe_ClearIN();
	}

	Pipe_Freeze();

	return ReceivedByte;
}

uint8_t AOA_Host_Flush(USB_ClassInfo_AOA_Host_t* const AOAInterfaceInfo)
{
	if ((USB_HostState != HOST_STATE_Configured) || !(AOAInterfaceInfo->State.IsActive))
	  return PIPE_READYWAIT_DeviceDisconnected;

	uint8_t ErrorCode;

	Pipe_SelectPipe(AOAInterfaceInfo->Config.DataOUTPipe.Address);
	Pipe_Unfreeze();

	if (!(Pipe_BytesInPipe()))
	  return PIPE_READYWAIT_NoError;

	bool BankFull = !(Pipe_IsReadWriteAllowed());

	Pipe_ClearOUT();

	if (BankFull)
	{
		if ((ErrorCode = Pipe_WaitUntilReady()) != PIPE_READYWAIT_NoError)
		  return ErrorCode;

		Pipe_ClearOUT();
	}

	Pipe_Freeze();

	return PIPE_READYWAIT_NoError;
}

#if defined(FDEV_SETUP_STREAM)
void AOA_Host_CreateStream(USB_ClassInfo_AOA_Host_t* const AOAInterfaceInfo,
                           FILE* const Stream)
{
	*Stream = (FILE)FDEV_SETUP_STREAM(AOA_Host_putchar, AOA_Host_getchar, _FDEV_SETUP_RW);
	fdev_set_udata(Stream, AOAInterfaceInfo);
}

void AOA_Host_CreateBlockingStream(USB_ClassInfo_AOA_Host_t* const AOAInterfaceInfo,
                                   FILE* const Stream)
{
	*Stream = (FILE)FDEV_SETUP_STREAM(AOA_Host_putchar, AOA_Host_getchar_Blocking, _FDEV_SETUP_RW);
	fdev_set_udata(Stream, AOAInterfaceInfo);
}

static int AOA_Host_putchar(char c,
                            FILE* Stream)
{
	return AOA_Host_SendByte((USB_ClassInfo_AOA_Host_t*)fdev_get_udata(Stream), c) ? _FDEV_ERR : 0;
}

static int AOA_Host_getchar(FILE* Stream)
{
	int16_t ReceivedByte = AOA_Host_ReceiveByte((USB_ClassInfo_AOA_Host_t*)fdev_get_udata(Stream));

	if (ReceivedByte < 0)
	  return _FDEV_EOF;

	return ReceivedByte;
}

static int AOA_Host_getchar_Blocking(FILE* Stream)
{
	int16_t ReceivedByte;

	while ((ReceivedByte = AOA_Host_ReceiveByte((USB_ClassInfo_AOA_Host_t*)fdev_get_udata(Stream))) < 0)
	{
		if (USB_HostState == HOST_STATE_Unattached)
		  return _FDEV_EOF;

		AOA_Host_USBTask((USB_ClassInfo_AOA_Host_t*)fdev_get_udata(Stream));
		USB_USBTask();
	}

	return ReceivedByte;
}
#endif

#endif


