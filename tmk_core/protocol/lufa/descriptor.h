/* 
 * Copyright 2012,2013 Jun Wako <wakojun@gmail.com>
 * This file is based on:
 *     LUFA-120219/Demos/Device/Lowlevel/KeyboardMouse
 *     LUFA-120219/Demos/Device/Lowlevel/GenericHID
 */

/*
             LUFA Library
     Copyright (C) Dean Camera, 2012.

  dean [at] fourwalledcubicle [dot] com
           www.lufa-lib.org
*/

/*
  Copyright 2012  Dean Camera (dean [at] fourwalledcubicle [dot] com)
  Copyright 2010  Denver Gingerich (denver [at] ossguy [dot] com)

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
 *  Header file for Descriptors.c.
 */

#ifndef _DESCRIPTORS_H_
#define _DESCRIPTORS_H_

#include <LUFA/Drivers/USB/USB.h>
#include <avr/pgmspace.h>


typedef struct
{
    USB_Descriptor_Configuration_Header_t Config;

    // Keyboard HID Interface
    USB_Descriptor_Interface_t            Keyboard_Interface;
    USB_HID_Descriptor_HID_t              Keyboard_HID;
    USB_Descriptor_Endpoint_t             Keyboard_INEndpoint;

#ifdef MOUSE_ENABLE
    // Mouse HID Interface
    USB_Descriptor_Interface_t            Mouse_Interface;
    USB_HID_Descriptor_HID_t              Mouse_HID;
    USB_Descriptor_Endpoint_t             Mouse_INEndpoint;
#endif

#ifdef EXTRAKEY_ENABLE
    // Extrakey HID Interface
    USB_Descriptor_Interface_t            Extrakey_Interface;
    USB_HID_Descriptor_HID_t              Extrakey_HID;
    USB_Descriptor_Endpoint_t             Extrakey_INEndpoint;
#endif

#ifdef CONSOLE_ENABLE
    // Console HID Interface
    USB_Descriptor_Interface_t            Console_Interface;
    USB_HID_Descriptor_HID_t              Console_HID;
    USB_Descriptor_Endpoint_t             Console_INEndpoint;
    USB_Descriptor_Endpoint_t             Console_OUTEndpoint;
#endif

#ifdef NKRO_ENABLE
    // NKRO HID Interface
    USB_Descriptor_Interface_t            NKRO_Interface;
    USB_HID_Descriptor_HID_t              NKRO_HID;
    USB_Descriptor_Endpoint_t             NKRO_INEndpoint;
#endif

#ifdef SERIAL_ENABLE
    // CDC Command Interface
    USB_Descriptor_Interface_t            CDC_CCI_Interface;
    USB_CDC_Descriptor_FunctionalHeader_t CDC_Functional_Header;
    USB_CDC_Descriptor_FunctionalACM_t    CDC_Functional_ACM;
    USB_CDC_Descriptor_FunctionalUnion_t  CDC_Functional_Union;
    USB_Descriptor_Endpoint_t             CDC_NotificationEndpoint;

    // CDC Data Interface
    USB_Descriptor_Interface_t            CDC_DCI_Interface;
    USB_Descriptor_Endpoint_t             CDC_DataOutEndpoint;
    USB_Descriptor_Endpoint_t             CDC_DataInEndpoint;
#endif

} USB_Descriptor_Configuration_t;


/* index of interface */
#define KEYBOARD_INTERFACE          0

#ifdef MOUSE_ENABLE
#   define MOUSE_INTERFACE          (KEYBOARD_INTERFACE + 1)
#else
#   define MOUSE_INTERFACE          KEYBOARD_INTERFACE
#endif 

#ifdef EXTRAKEY_ENABLE
#   define EXTRAKEY_INTERFACE       (MOUSE_INTERFACE + 1)
#else
#   define EXTRAKEY_INTERFACE       MOUSE_INTERFACE
#endif 

#ifdef CONSOLE_ENABLE
#   define CONSOLE_INTERFACE        (EXTRAKEY_INTERFACE + 1)
#else
#   define CONSOLE_INTERFACE        EXTRAKEY_INTERFACE
#endif

#ifdef NKRO_ENABLE
#   define NKRO_INTERFACE           (CONSOLE_INTERFACE + 1)
#else
#   define NKRO_INTERFACE           CONSOLE_INTERFACE
#endif

#ifdef SERIAL_ENABLE
#   define SERIAL_CCI_INTERFACE     (NKRO_INTERFACE + 1)
#   define SERIAL_DCI_INTERFACE     (NKRO_INTERFACE + 2)
#else
#   define SERIAL_DCI_INTERFACE     NKRO_INTERFACE
#endif

/* nubmer of interfaces */
#define TOTAL_INTERFACES            (SERIAL_DCI_INTERFACE + 1)


// Endopoint number and size
#define KEYBOARD_IN_EPNUM           1

#ifdef MOUSE_ENABLE
#   define MOUSE_IN_EPNUM           (KEYBOARD_IN_EPNUM + 1) 
#else
#   define MOUSE_IN_EPNUM           KEYBOARD_IN_EPNUM
#endif

#ifdef EXTRAKEY_ENABLE
#   define EXTRAKEY_IN_EPNUM        (MOUSE_IN_EPNUM + 1)
#else
#   define EXTRAKEY_IN_EPNUM        MOUSE_IN_EPNUM 
#endif

#ifdef CONSOLE_ENABLE
#   define CONSOLE_IN_EPNUM         (EXTRAKEY_IN_EPNUM + 1)
#   define CONSOLE_OUT_EPNUM        (EXTRAKEY_IN_EPNUM + 1)
#else
#   define CONSOLE_OUT_EPNUM        EXTRAKEY_IN_EPNUM
#endif

#ifdef NKRO_ENABLE
#   define NKRO_IN_EPNUM            (CONSOLE_OUT_EPNUM + 1)
#else
#   define NKRO_IN_EPNUM            CONSOLE_OUT_EPNUM
#endif

#ifdef SERIAL_ENABLE
#   define SERIAL_NOTIF_EPNUM       (NKRO_IN_EPNUM + 1)
#   define SERIAL_TX_EPNUM          (NKRO_IN_EPNUM + 2)
#   define SERIAL_RX_EPNUM          (NKRO_IN_EPNUM + 3)
#else
#   define SERIAL_RX_EPNUM          NKRO_IN_EPNUM
#endif

/* Check number of endpoints. ATmega32u2 has only four except for control endpoint. */
#if defined(__AVR_ATmega32U2__) && NKRO_IN_EPNUM > 4
#   error "Endpoints are not available enough to support all functions. Disable some of build options in Makefile.(MOUSEKEY, EXTRAKEY, CONSOLE, NKRO)"
#endif


#define KEYBOARD_EPSIZE             8
#define MOUSE_EPSIZE                8
#define EXTRAKEY_EPSIZE             8
#define CONSOLE_EPSIZE              32
#define NKRO_EPSIZE                 32
#define SERIAL_NOTIF_EPSIZE         8
#define SERIAL_TXRX_EPSIZE          16

uint16_t CALLBACK_USB_GetDescriptor(const uint16_t wValue,
                                    const uint16_t wIndex,
                                    const void** const DescriptorAddress)
                                    ATTR_WARN_UNUSED_RESULT ATTR_NON_NULL_PTR_ARG(3);

#endif
