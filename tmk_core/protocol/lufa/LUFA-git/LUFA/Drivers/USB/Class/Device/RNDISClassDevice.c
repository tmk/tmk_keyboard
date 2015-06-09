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

#define  __INCLUDE_FROM_RNDIS_DRIVER
#define  __INCLUDE_FROM_RNDIS_DEVICE_C
#include "RNDISClassDevice.h"

static const uint32_t PROGMEM AdapterSupportedOIDList[]  =
	{
		CPU_TO_LE32(OID_GEN_SUPPORTED_LIST),
		CPU_TO_LE32(OID_GEN_PHYSICAL_MEDIUM),
		CPU_TO_LE32(OID_GEN_HARDWARE_STATUS),
		CPU_TO_LE32(OID_GEN_MEDIA_SUPPORTED),
		CPU_TO_LE32(OID_GEN_MEDIA_IN_USE),
		CPU_TO_LE32(OID_GEN_MAXIMUM_FRAME_SIZE),
		CPU_TO_LE32(OID_GEN_MAXIMUM_TOTAL_SIZE),
		CPU_TO_LE32(OID_GEN_LINK_SPEED),
		CPU_TO_LE32(OID_GEN_TRANSMIT_BLOCK_SIZE),
		CPU_TO_LE32(OID_GEN_RECEIVE_BLOCK_SIZE),
		CPU_TO_LE32(OID_GEN_VENDOR_ID),
		CPU_TO_LE32(OID_GEN_VENDOR_DESCRIPTION),
		CPU_TO_LE32(OID_GEN_CURRENT_PACKET_FILTER),
		CPU_TO_LE32(OID_GEN_MAXIMUM_TOTAL_SIZE),
		CPU_TO_LE32(OID_GEN_MEDIA_CONNECT_STATUS),
		CPU_TO_LE32(OID_GEN_XMIT_OK),
		CPU_TO_LE32(OID_GEN_RCV_OK),
		CPU_TO_LE32(OID_GEN_XMIT_ERROR),
		CPU_TO_LE32(OID_GEN_RCV_ERROR),
		CPU_TO_LE32(OID_GEN_RCV_NO_BUFFER),
		CPU_TO_LE32(OID_802_3_PERMANENT_ADDRESS),
		CPU_TO_LE32(OID_802_3_CURRENT_ADDRESS),
		CPU_TO_LE32(OID_802_3_MULTICAST_LIST),
		CPU_TO_LE32(OID_802_3_MAXIMUM_LIST_SIZE),
		CPU_TO_LE32(OID_802_3_RCV_ERROR_ALIGNMENT),
		CPU_TO_LE32(OID_802_3_XMIT_ONE_COLLISION),
		CPU_TO_LE32(OID_802_3_XMIT_MORE_COLLISIONS),
	};

void RNDIS_Device_ProcessControlRequest(USB_ClassInfo_RNDIS_Device_t* const RNDISInterfaceInfo)
{
	if (!(Endpoint_IsSETUPReceived()))
	  return;

	if (USB_ControlRequest.wIndex != RNDISInterfaceInfo->Config.ControlInterfaceNumber)
	  return;

	switch (USB_ControlRequest.bRequest)
	{
		case RNDIS_REQ_SendEncapsulatedCommand:
			if (USB_ControlRequest.bmRequestType == (REQDIR_HOSTTODEVICE | REQTYPE_CLASS | REQREC_INTERFACE))
			{
				Endpoint_ClearSETUP();
				Endpoint_Read_Control_Stream_LE(RNDISInterfaceInfo->Config.MessageBuffer, USB_ControlRequest.wLength);
				Endpoint_ClearIN();

				RNDIS_Device_ProcessRNDISControlMessage(RNDISInterfaceInfo);
			}

			break;
		case RNDIS_REQ_GetEncapsulatedResponse:
			if (USB_ControlRequest.bmRequestType == (REQDIR_DEVICETOHOST | REQTYPE_CLASS | REQREC_INTERFACE))
			{
				RNDIS_Message_Header_t* MessageHeader = (RNDIS_Message_Header_t*)RNDISInterfaceInfo->Config.MessageBuffer;

				if (!(MessageHeader->MessageLength))
				{
					RNDISInterfaceInfo->Config.MessageBuffer[0] = 0;
					MessageHeader->MessageLength                = CPU_TO_LE32(1);
				}

				Endpoint_ClearSETUP();
				Endpoint_Write_Control_Stream_LE(RNDISInterfaceInfo->Config.MessageBuffer, le32_to_cpu(MessageHeader->MessageLength));
				Endpoint_ClearOUT();

				MessageHeader->MessageLength = CPU_TO_LE32(0);
			}

			break;
	}
}

bool RNDIS_Device_ConfigureEndpoints(USB_ClassInfo_RNDIS_Device_t* const RNDISInterfaceInfo)
{
	memset(&RNDISInterfaceInfo->State, 0x00, sizeof(RNDISInterfaceInfo->State));

	RNDISInterfaceInfo->Config.DataINEndpoint.Type       = EP_TYPE_BULK;
	RNDISInterfaceInfo->Config.DataOUTEndpoint.Type      = EP_TYPE_BULK;
	RNDISInterfaceInfo->Config.NotificationEndpoint.Type = EP_TYPE_INTERRUPT;

	if (RNDISInterfaceInfo->Config.MessageBuffer == NULL)
	  return false;

	if (RNDISInterfaceInfo->Config.MessageBufferLength < RNDIS_DEVICE_MIN_MESSAGE_BUFFER_LENGTH)
	  return false;

	if (!(Endpoint_ConfigureEndpointTable(&RNDISInterfaceInfo->Config.DataINEndpoint, 1)))
	  return false;

	if (!(Endpoint_ConfigureEndpointTable(&RNDISInterfaceInfo->Config.DataOUTEndpoint, 1)))
	  return false;

	if (!(Endpoint_ConfigureEndpointTable(&RNDISInterfaceInfo->Config.NotificationEndpoint, 1)))
	  return false;

	return true;
}

void RNDIS_Device_USBTask(USB_ClassInfo_RNDIS_Device_t* const RNDISInterfaceInfo)
{
	if (USB_DeviceState != DEVICE_STATE_Configured)
	  return;

	Endpoint_SelectEndpoint(RNDISInterfaceInfo->Config.NotificationEndpoint.Address);

	if (Endpoint_IsINReady() && RNDISInterfaceInfo->State.ResponseReady)
	{
		USB_Request_Header_t Notification = (USB_Request_Header_t)
			{
				.bmRequestType = (REQDIR_DEVICETOHOST | REQTYPE_CLASS | REQREC_INTERFACE),
				.bRequest      = RNDIS_NOTIF_ResponseAvailable,
				.wValue        = CPU_TO_LE16(0),
				.wIndex        = CPU_TO_LE16(0),
				.wLength       = CPU_TO_LE16(0),
			};

		Endpoint_Write_Stream_LE(&Notification, sizeof(USB_Request_Header_t), NULL);

		Endpoint_ClearIN();

		RNDISInterfaceInfo->State.ResponseReady = false;
	}
}

void RNDIS_Device_ProcessRNDISControlMessage(USB_ClassInfo_RNDIS_Device_t* const RNDISInterfaceInfo)
{
	/* Note: Only a single buffer is used for both the received message and its response to save SRAM. Because of
	         this, response bytes should be filled in order so that they do not clobber unread data in the buffer. */

	RNDIS_Message_Header_t* MessageHeader = (RNDIS_Message_Header_t*)RNDISInterfaceInfo->Config.MessageBuffer;

	switch (le32_to_cpu(MessageHeader->MessageType))
	{
		case REMOTE_NDIS_INITIALIZE_MSG:
			RNDISInterfaceInfo->State.ResponseReady     = true;

			RNDIS_Initialize_Message_t*  INITIALIZE_Message  =
			               (RNDIS_Initialize_Message_t*)RNDISInterfaceInfo->Config.MessageBuffer;
			RNDIS_Initialize_Complete_t* INITIALIZE_Response =
			               (RNDIS_Initialize_Complete_t*)RNDISInterfaceInfo->Config.MessageBuffer;

			INITIALIZE_Response->MessageType            = CPU_TO_LE32(REMOTE_NDIS_INITIALIZE_CMPLT);
			INITIALIZE_Response->MessageLength          = CPU_TO_LE32(sizeof(RNDIS_Initialize_Complete_t));
			INITIALIZE_Response->RequestId              = INITIALIZE_Message->RequestId;
			INITIALIZE_Response->Status                 = CPU_TO_LE32(REMOTE_NDIS_STATUS_SUCCESS);

			INITIALIZE_Response->MajorVersion           = CPU_TO_LE32(REMOTE_NDIS_VERSION_MAJOR);
			INITIALIZE_Response->MinorVersion           = CPU_TO_LE32(REMOTE_NDIS_VERSION_MINOR);
			INITIALIZE_Response->DeviceFlags            = CPU_TO_LE32(REMOTE_NDIS_DF_CONNECTIONLESS);
			INITIALIZE_Response->Medium                 = CPU_TO_LE32(REMOTE_NDIS_MEDIUM_802_3);
			INITIALIZE_Response->MaxPacketsPerTransfer  = CPU_TO_LE32(1);
			INITIALIZE_Response->MaxTransferSize        = CPU_TO_LE32(sizeof(RNDIS_Packet_Message_t) + ETHERNET_FRAME_SIZE_MAX);
			INITIALIZE_Response->PacketAlignmentFactor  = CPU_TO_LE32(0);
			INITIALIZE_Response->AFListOffset           = CPU_TO_LE32(0);
			INITIALIZE_Response->AFListSize             = CPU_TO_LE32(0);

			RNDISInterfaceInfo->State.CurrRNDISState    = RNDIS_Initialized;
			break;
		case REMOTE_NDIS_HALT_MSG:
			RNDISInterfaceInfo->State.ResponseReady     = false;

			MessageHeader->MessageLength                = CPU_TO_LE32(0);

			RNDISInterfaceInfo->State.CurrRNDISState    = RNDIS_Uninitialized;
			break;
		case REMOTE_NDIS_QUERY_MSG:
			RNDISInterfaceInfo->State.ResponseReady     = true;

			RNDIS_Query_Message_t*  QUERY_Message       = (RNDIS_Query_Message_t*)RNDISInterfaceInfo->Config.MessageBuffer;
			RNDIS_Query_Complete_t* QUERY_Response      = (RNDIS_Query_Complete_t*)RNDISInterfaceInfo->Config.MessageBuffer;
			uint32_t                Query_Oid           = CPU_TO_LE32(QUERY_Message->Oid);

			void*    QueryData    = &RNDISInterfaceInfo->Config.MessageBuffer[sizeof(RNDIS_Message_Header_t) +
			                                                                  le32_to_cpu(QUERY_Message->InformationBufferOffset)];
			void*    ResponseData = &RNDISInterfaceInfo->Config.MessageBuffer[sizeof(RNDIS_Query_Complete_t)];
			uint16_t ResponseSize;

			QUERY_Response->MessageType                 = CPU_TO_LE32(REMOTE_NDIS_QUERY_CMPLT);

			if (RNDIS_Device_ProcessNDISQuery(RNDISInterfaceInfo, Query_Oid, QueryData, le32_to_cpu(QUERY_Message->InformationBufferLength),
			                                  ResponseData, &ResponseSize))
			{
				QUERY_Response->Status                  = CPU_TO_LE32(REMOTE_NDIS_STATUS_SUCCESS);
				QUERY_Response->MessageLength           = cpu_to_le32(sizeof(RNDIS_Query_Complete_t) + ResponseSize);

				QUERY_Response->InformationBufferLength = CPU_TO_LE32(ResponseSize);
				QUERY_Response->InformationBufferOffset = CPU_TO_LE32(sizeof(RNDIS_Query_Complete_t) - sizeof(RNDIS_Message_Header_t));
			}
			else
			{
				QUERY_Response->Status                  = CPU_TO_LE32(REMOTE_NDIS_STATUS_NOT_SUPPORTED);
				QUERY_Response->MessageLength           = CPU_TO_LE32(sizeof(RNDIS_Query_Complete_t));

				QUERY_Response->InformationBufferLength = CPU_TO_LE32(0);
				QUERY_Response->InformationBufferOffset = CPU_TO_LE32(0);
			}

			break;
		case REMOTE_NDIS_SET_MSG:
			RNDISInterfaceInfo->State.ResponseReady     = true;

			RNDIS_Set_Message_t*  SET_Message           = (RNDIS_Set_Message_t*)RNDISInterfaceInfo->Config.MessageBuffer;
			RNDIS_Set_Complete_t* SET_Response          = (RNDIS_Set_Complete_t*)RNDISInterfaceInfo->Config.MessageBuffer;
			uint32_t              SET_Oid               = le32_to_cpu(SET_Message->Oid);

			SET_Response->MessageType                   = CPU_TO_LE32(REMOTE_NDIS_SET_CMPLT);
			SET_Response->MessageLength                 = CPU_TO_LE32(sizeof(RNDIS_Set_Complete_t));
			SET_Response->RequestId                     = SET_Message->RequestId;

			void* SetData = &RNDISInterfaceInfo->Config.MessageBuffer[sizeof(RNDIS_Message_Header_t) +
			                                                              le32_to_cpu(SET_Message->InformationBufferOffset)];

			SET_Response->Status = RNDIS_Device_ProcessNDISSet(RNDISInterfaceInfo, SET_Oid, SetData,
			                                                   le32_to_cpu(SET_Message->InformationBufferLength)) ?
			                                                   REMOTE_NDIS_STATUS_SUCCESS : REMOTE_NDIS_STATUS_NOT_SUPPORTED;
			break;
		case REMOTE_NDIS_RESET_MSG:
			RNDISInterfaceInfo->State.ResponseReady     = true;

			RNDIS_Reset_Complete_t* RESET_Response      = (RNDIS_Reset_Complete_t*)RNDISInterfaceInfo->Config.MessageBuffer;

			RESET_Response->MessageType                 = CPU_TO_LE32(REMOTE_NDIS_RESET_CMPLT);
			RESET_Response->MessageLength               = CPU_TO_LE32(sizeof(RNDIS_Reset_Complete_t));
			RESET_Response->Status                      = CPU_TO_LE32(REMOTE_NDIS_STATUS_SUCCESS);
			RESET_Response->AddressingReset             = CPU_TO_LE32(0);

			break;
		case REMOTE_NDIS_KEEPALIVE_MSG:
			RNDISInterfaceInfo->State.ResponseReady     = true;

			RNDIS_KeepAlive_Message_t*  KEEPALIVE_Message  =
			                (RNDIS_KeepAlive_Message_t*)RNDISInterfaceInfo->Config.MessageBuffer;
			RNDIS_KeepAlive_Complete_t* KEEPALIVE_Response =
			                (RNDIS_KeepAlive_Complete_t*)RNDISInterfaceInfo->Config.MessageBuffer;

			KEEPALIVE_Response->MessageType             = CPU_TO_LE32(REMOTE_NDIS_KEEPALIVE_CMPLT);
			KEEPALIVE_Response->MessageLength           = CPU_TO_LE32(sizeof(RNDIS_KeepAlive_Complete_t));
			KEEPALIVE_Response->RequestId               = KEEPALIVE_Message->RequestId;
			KEEPALIVE_Response->Status                  = CPU_TO_LE32(REMOTE_NDIS_STATUS_SUCCESS);

			break;
	}
}

static bool RNDIS_Device_ProcessNDISQuery(USB_ClassInfo_RNDIS_Device_t* const RNDISInterfaceInfo,
                                          const uint32_t OId,
                                          void* const QueryData,
                                          const uint16_t QuerySize,
                                          void* ResponseData,
                                          uint16_t* const ResponseSize)
{
	(void)QueryData;
	(void)QuerySize;

	switch (OId)
	{
		case OID_GEN_SUPPORTED_LIST:
			*ResponseSize = sizeof(AdapterSupportedOIDList);

			memcpy_P(ResponseData, AdapterSupportedOIDList, sizeof(AdapterSupportedOIDList));

			return true;
		case OID_GEN_PHYSICAL_MEDIUM:
			*ResponseSize = sizeof(uint32_t);

			/* Indicate that the device is a true ethernet link */
			*((uint32_t*)ResponseData) = CPU_TO_LE32(0);

			return true;
		case OID_GEN_HARDWARE_STATUS:
			*ResponseSize = sizeof(uint32_t);

			*((uint32_t*)ResponseData) = CPU_TO_LE32(NDIS_HardwareStatus_Ready);

			return true;
		case OID_GEN_MEDIA_SUPPORTED:
		case OID_GEN_MEDIA_IN_USE:
			*ResponseSize = sizeof(uint32_t);

			*((uint32_t*)ResponseData) = CPU_TO_LE32(REMOTE_NDIS_MEDIUM_802_3);

			return true;
		case OID_GEN_VENDOR_ID:
			*ResponseSize = sizeof(uint32_t);

			/* Vendor ID 0x0xFFFFFF is reserved for vendors who have not purchased a NDIS VID */
			*((uint32_t*)ResponseData) = CPU_TO_LE32(0x00FFFFFF);

			return true;
		case OID_GEN_MAXIMUM_FRAME_SIZE:
		case OID_GEN_TRANSMIT_BLOCK_SIZE:
		case OID_GEN_RECEIVE_BLOCK_SIZE:
			*ResponseSize = sizeof(uint32_t);

			*((uint32_t*)ResponseData) = CPU_TO_LE32(ETHERNET_FRAME_SIZE_MAX);

			return true;
		case OID_GEN_VENDOR_DESCRIPTION:
			*ResponseSize = (strlen(RNDISInterfaceInfo->Config.AdapterVendorDescription) + 1);

			memcpy(ResponseData, RNDISInterfaceInfo->Config.AdapterVendorDescription, *ResponseSize);

			return true;
		case OID_GEN_MEDIA_CONNECT_STATUS:
			*ResponseSize = sizeof(uint32_t);

			*((uint32_t*)ResponseData) = CPU_TO_LE32(REMOTE_NDIS_MEDIA_STATE_CONNECTED);

			return true;
		case OID_GEN_LINK_SPEED:
			*ResponseSize = sizeof(uint32_t);

			/* Indicate 10Mb/s link speed */
			*((uint32_t*)ResponseData) = CPU_TO_LE32(100000);

			return true;
		case OID_802_3_PERMANENT_ADDRESS:
		case OID_802_3_CURRENT_ADDRESS:
			*ResponseSize = sizeof(MAC_Address_t);

			memcpy(ResponseData, &RNDISInterfaceInfo->Config.AdapterMACAddress, sizeof(MAC_Address_t));

			return true;
		case OID_802_3_MAXIMUM_LIST_SIZE:
			*ResponseSize = sizeof(uint32_t);

			/* Indicate only one multicast address supported */
			*((uint32_t*)ResponseData) = CPU_TO_LE32(1);

			return true;
		case OID_GEN_CURRENT_PACKET_FILTER:
			*ResponseSize = sizeof(uint32_t);

			*((uint32_t*)ResponseData) = cpu_to_le32(RNDISInterfaceInfo->State.CurrPacketFilter);

			return true;
		case OID_GEN_XMIT_OK:
		case OID_GEN_RCV_OK:
		case OID_GEN_XMIT_ERROR:
		case OID_GEN_RCV_ERROR:
		case OID_GEN_RCV_NO_BUFFER:
		case OID_802_3_RCV_ERROR_ALIGNMENT:
		case OID_802_3_XMIT_ONE_COLLISION:
		case OID_802_3_XMIT_MORE_COLLISIONS:
			*ResponseSize = sizeof(uint32_t);

			/* Unused statistic OIDs - always return 0 for each */
			*((uint32_t*)ResponseData) = CPU_TO_LE32(0);

			return true;
		case OID_GEN_MAXIMUM_TOTAL_SIZE:
			*ResponseSize = sizeof(uint32_t);

			/* Indicate maximum overall buffer (Ethernet frame and RNDIS header) the adapter can handle */
			*((uint32_t*)ResponseData) = CPU_TO_LE32(RNDISInterfaceInfo->Config.MessageBufferLength + ETHERNET_FRAME_SIZE_MAX);

			return true;
		default:
			return false;
	}
}

static bool RNDIS_Device_ProcessNDISSet(USB_ClassInfo_RNDIS_Device_t* const RNDISInterfaceInfo,
                                        const uint32_t OId,
                                        const void* SetData,
                                        const uint16_t SetSize)
{
	(void)SetSize;

	switch (OId)
	{
		case OID_GEN_CURRENT_PACKET_FILTER:
			RNDISInterfaceInfo->State.CurrPacketFilter = le32_to_cpu(*((uint32_t*)SetData));
			RNDISInterfaceInfo->State.CurrRNDISState   = (RNDISInterfaceInfo->State.CurrPacketFilter) ? RNDIS_Data_Initialized : RNDIS_Initialized;

			return true;
		case OID_802_3_MULTICAST_LIST:
			/* Do nothing - throw away the value from the host as it is unused */

			return true;
		default:
			return false;
	}
}

bool RNDIS_Device_IsPacketReceived(USB_ClassInfo_RNDIS_Device_t* const RNDISInterfaceInfo)
{
	if ((USB_DeviceState != DEVICE_STATE_Configured) ||
	    (RNDISInterfaceInfo->State.CurrRNDISState != RNDIS_Data_Initialized))
	{
		return false;
	}

	Endpoint_SelectEndpoint(RNDISInterfaceInfo->Config.DataOUTEndpoint.Address);
	return Endpoint_IsOUTReceived();
}

uint8_t RNDIS_Device_ReadPacket(USB_ClassInfo_RNDIS_Device_t* const RNDISInterfaceInfo,
                                void* Buffer,
                                uint16_t* const PacketLength)
{
	if ((USB_DeviceState != DEVICE_STATE_Configured) ||
	    (RNDISInterfaceInfo->State.CurrRNDISState != RNDIS_Data_Initialized))
	{
		return ENDPOINT_RWSTREAM_DeviceDisconnected;
	}

	Endpoint_SelectEndpoint(RNDISInterfaceInfo->Config.DataOUTEndpoint.Address);

	*PacketLength = 0;

	if (!(Endpoint_IsOUTReceived()))
		return ENDPOINT_RWSTREAM_NoError;

	RNDIS_Packet_Message_t RNDISPacketHeader;
	Endpoint_Read_Stream_LE(&RNDISPacketHeader, sizeof(RNDIS_Packet_Message_t), NULL);

	if (le32_to_cpu(RNDISPacketHeader.DataLength) > ETHERNET_FRAME_SIZE_MAX)
	{
		Endpoint_StallTransaction();

		return RNDIS_ERROR_LOGICAL_CMD_FAILED;
	}

	*PacketLength = (uint16_t)le32_to_cpu(RNDISPacketHeader.DataLength);

	Endpoint_Read_Stream_LE(Buffer, *PacketLength, NULL);
	Endpoint_ClearOUT();

	return ENDPOINT_RWSTREAM_NoError;
}

uint8_t RNDIS_Device_SendPacket(USB_ClassInfo_RNDIS_Device_t* const RNDISInterfaceInfo,
                                void* Buffer,
                                const uint16_t PacketLength)
{
	uint8_t ErrorCode;

	if ((USB_DeviceState != DEVICE_STATE_Configured) ||
	    (RNDISInterfaceInfo->State.CurrRNDISState != RNDIS_Data_Initialized))
	{
		return ENDPOINT_RWSTREAM_DeviceDisconnected;
	}

	Endpoint_SelectEndpoint(RNDISInterfaceInfo->Config.DataINEndpoint.Address);

	if ((ErrorCode = Endpoint_WaitUntilReady()) != ENDPOINT_READYWAIT_NoError)
	  return ErrorCode;

	RNDIS_Packet_Message_t RNDISPacketHeader;

	memset(&RNDISPacketHeader, 0, sizeof(RNDIS_Packet_Message_t));

	RNDISPacketHeader.MessageType   = CPU_TO_LE32(REMOTE_NDIS_PACKET_MSG);
	RNDISPacketHeader.MessageLength = cpu_to_le32(sizeof(RNDIS_Packet_Message_t) + PacketLength);
	RNDISPacketHeader.DataOffset    = CPU_TO_LE32(sizeof(RNDIS_Packet_Message_t) - sizeof(RNDIS_Message_Header_t));
	RNDISPacketHeader.DataLength    = cpu_to_le32(PacketLength);

	Endpoint_Write_Stream_LE(&RNDISPacketHeader, sizeof(RNDIS_Packet_Message_t), NULL);
	Endpoint_Write_Stream_LE(Buffer, PacketLength, NULL);
	Endpoint_ClearIN();

	return ENDPOINT_RWSTREAM_NoError;
}

#endif

