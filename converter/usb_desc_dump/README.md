USB Descriptor Dumper
=====================
2021-07-20

`Usb_desc_dump` gets USB descriptors and shows in HEX dump and human readable form.
This works on TMK USB-USB converter and USB Host Shield with Arduino Leonardo.


Following descriptors are supported.

- Device Descriptor
- Configuration Descriptor
    - Interface Descriptor
    - Endpoint Descriptor
    - HID Descriptor
- HID Report descriptor
- String Descriptor
- Device Qualifier*
- Other Speed*
- Audio/MIDI Class*
*: partly supported


See source code for the detail.

- https://github.com/tmk/USB_Host_Shield_2.0/tree/master/examples/USB_desc_dump



Example optput:

```
usb_state: 90

//////////////////////////////////////////////////////////////////////
// USB_desc_dump
// Address: 01
// Lowspeed: 00

// Devicer dump:
12 01 00 02 00 00 00 08 6A 04 11 00 00 01 00 00
00 01

// Device:
bLength:                12
bDescriptorType:        01
bcdUSB:                 0200
bDeviceClass:           00
bDeviceSubClass:        00
bDeviceProtocol:        00
bMaxPacketSize0:        08
idVendor:               046A
idProduct:              0011
bcdDevice:              0100
iManufacturer:          00
iProduct:               00
iSerialNumber:          00
bNumConfigurations:     01

// Config0 dump:        len: 0022
09 02 22 00 01 01 00 A0 32 09 04 00 00 01 03 01
01 00 09 21 11 01 00 01 22 40 00 07 05 81 03 08
00 0A

// Config:
bLength:                09
bDescriptorType:        02
wTotalLength:           0022
bNumInterfaces:         01
bConfigurationValue:    01
iConfiguration:         00
bmAttributes:           A0
bMaxPower:              32

// Interface0.0:
bLength:                09
bDescriptorType:        04
bInterfaceNumber:       00
bAlternateSetting:      00
bNumEndpoints:          01
bInterfaceClass:        03
bInterfaceSubClass:     01
bInterfaceProtocol:     01
iInterface:             00

// HID:
bLength:                09
bDescriptorType:        21
bcdHID:                 0111
bCountryCode:           00
bNumDescriptors:        01
bDescrType:             22
wDescriptorLength:      0040

// Report0 dump:        len: 0040
05 01 09 06 A1 01 05 07 19 E0 29 E7 15 00 25 01
75 01 95 08 81 02 95 01 75 08 81 01 95 03 75 01
05 08 19 01 29 03 91 02 95 05 75 01 91 01 95 06
75 08 15 00 26 DD 00 05 07 19 00 29 DD 81 00 C0

// Endpoint:
bLength:                07
bDescriptorType:        05
bEndpointAddress:       81
bmAttributes:           03
wMaxPacketSize:         0008
bInterval:              0A

// Parse data here: http://eleccelerator.com/usbdescreqparser/
```



To inspect descriptor content closely use 'USB Descriptor and Request Parser' on line.

- https://eleccelerator.com/usbdescreqparser/


Also you can use command line tool `hidrd-convert` like below.

- https://github.com/DIGImend/hidrd

```
$ cat | hidrd-convert -i hex -o spec
05  01  09  06  A1  01  05  07  19  E0  29  E7  15  00  25  01
75  01  95  08  81  02  95  01  75  08  81  01  95  03  75  01
05  08  19  01  29  03  91  02  95  05  75  01  91  01  95  06
75  08  15  00  26  DD  00  05  07  19  00  29  DD  81  00  C0
^D

Usage Page (Desktop),               ; Generic desktop controls (01h)
Usage (Keyboard),                   ; Keyboard (06h, application collection)
Collection (Application),
    Usage Page (Keyboard),          ; Keyboard/keypad (07h)
    Usage Minimum (KB Leftcontrol), ; Keyboard left control (E0h, dynamic value)
    Usage Maximum (KB Right GUI),   ; Keyboard right GUI (E7h, dynamic value)
    Logical Minimum (0),
    Logical Maximum (1),
    Report Size (1),
    Report Count (8),
    Input (Variable),
    Report Count (1),
    Report Size (8),
    Input (Constant),
    Report Count (3),
    Report Size (1),
    Usage Page (LED),               ; LEDs (08h)
    Usage Minimum (01h),
    Usage Maximum (03h),
    Output (Variable),
    Report Count (5),
    Report Size (1),
    Output (Constant),
    Report Count (6),
    Report Size (8),
    Logical Minimum (0),
    Logical Maximum (221),
    Usage Page (Keyboard),          ; Keyboard/keypad (07h)
    Usage Minimum (None),           ; No event (00h, selector)
    Usage Maximum (KP Hexadecimal), ; Keypad Hexadecimal (DDh, selector)
    Input,
End Collection
```
