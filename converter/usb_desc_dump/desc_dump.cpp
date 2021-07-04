/*
Copyright 2021 Jun Wako <wakojun@gmail.com>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
/*
 * USB Host Shield HID keyboards
 */
#include "Usb.h"
#include "usbhub.h"
#include "usbhid.h"

#include "timer.h"
#include "matrix.h"
#include "keymap.h"
#include "hook.h"
// This should be placed after Host Shield lib headers to override
#include "print.h"


USB Usb;
USBHub hub1(&Usb);


#define BUF_SIZE 256
uint8_t buf[BUF_SIZE];

#define RBUF_SIZE 256
uint8_t rbuf[RBUF_SIZE];


void printHEX(uint8_t hex)
{
    xprintf("%02X", hex);
}

void printHEX(uint16_t hex)
{
    xprintf("%02X", hex>>8);
    xprintf("%02X", hex);
}

void printVal(const char* name, uint8_t hex)
{
    xprintf("%s:%02X\r\n", name, hex);
}

void printVal(const char* name, uint16_t hex)
{
    xprintf("%s:%04X\r\n", name, hex);
}

void printError(uint8_t rcode)
{
    xprintf("\r\nERROR:%02X\r\n", rcode);
}

void dumpBuf(int len, uint8_t* buf)
{
    for (int i = 0; i < len; i++) {
        if (i % 16)
            print(" ");
        else if (i != 0)
            print("\r\n");
        xprintf("%02X ", buf[i]);
    }
    print("\r\n");
}

uint8_t dumpReportDesc(UsbDevice *pdev, uint16_t iface, uint16_t len)
{
    uint8_t rcode = 0;
    Usb.ctrlReq(pdev->address.devAddress, 0x00, bmREQ_HID_REPORT, USB_REQUEST_GET_DESCRIPTOR, 0x00,
                HID_DESCRIPTOR_REPORT, iface, len, RBUF_SIZE, rbuf, NULL);
    if (rcode) return rcode;
    dumpBuf(len, rbuf);
    return rcode;
}

void scanConfigDesc(UsbDevice *pdev, uint16_t tl, uint8_t* pB)
{
    uint8_t iface = 0;
    while (pB < buf + tl) {
        switch (*(pB+1)) {
            case USB_DESCRIPTOR_DEVICE:
                break;
            case USB_DESCRIPTOR_CONFIGURATION:
                break;
            case USB_DESCRIPTOR_STRING:
                break;
            case USB_DESCRIPTOR_ENDPOINT:
                break;
            case USB_DESCRIPTOR_INTERFACE: {
                USB_INTERFACE_DESCRIPTOR *pI = (USB_INTERFACE_DESCRIPTOR *)pB;
                iface = pI->bInterfaceNumber;
                break;
                }
            case HID_DESCRIPTOR_HID: {
                USB_HID_DESCRIPTOR *pH = (USB_HID_DESCRIPTOR *)pB;
                xprintf("\r\n// Report%d: len:%04X\r\n", iface, pH->wDescriptorLength);
                if (pH->bDescrType == HID_DESCRIPTOR_REPORT) {
                    dumpReportDesc(pdev, iface, pH->wDescriptorLength);
                } else {
                    printVal("Unknown HID Desc Type:", pH->bDescrType);
                }
                break;
            }
            default:
                printVal("Unknown Desc Type:", *(pB+1));
                return;
        }
        pB += *pB;
    }
}

uint8_t dumpConfigDesc(UsbDevice *pdev, uint8_t numConf)
{
    uint8_t rcode = 0;
    for (uint8_t i = 0; i < numConf; i++) {
        xprintf("\r\n// Config%d:", i);

        rcode = Usb.getConfDescr(pdev->address.devAddress, 0, sizeof(USB_CONFIGURATION_DESCRIPTOR), i, buf);
        if (rcode) return rcode;
        USB_CONFIGURATION_DESCRIPTOR *pC = (USB_CONFIGURATION_DESCRIPTOR *)&buf;
        uint16_t tl = pC->wTotalLength;
        printVal(" len", tl);

        rcode = Usb.getConfDescr(pdev->address.devAddress, 0, (tl < BUF_SIZE ? tl : BUF_SIZE), i, buf);
        if (rcode) return rcode;
        dumpBuf(tl, buf);

        scanConfigDesc(pdev, tl, buf);
    }
    return rcode;
}

void dumpDescriptors(UsbDevice *pdev)
{
    uint8_t rcode = 0;

    xprintf("\r\n// Address:%02X\r\n", pdev->address.devAddress);
    xprintf("// lowspeed:%02X\r\n", pdev->lowspeed);

    //
    // Device Descriptor
    //
    rcode = Usb.getDevDescr(pdev->address.devAddress, 0, sizeof(USB_DEVICE_DESCRIPTOR), buf);
    if (rcode) { printError(rcode); }
    USB_DEVICE_DESCRIPTOR *pD = (USB_DEVICE_DESCRIPTOR *)&buf;
    printVal("// idVendor", pD->idVendor);
    printVal("// idProduct", pD->idProduct);
    print("\r\n// Device Descriptor:\r\n");
    dumpBuf(sizeof(USB_DEVICE_DESCRIPTOR), buf);


    //
    // Configuration Descriptor
    //
    uint8_t numConf = pD->bNumConfigurations;
    rcode = dumpConfigDesc(pdev, numConf);
    if (rcode) { printError(rcode); }

    Usb.GetAddressPool().FreeAddress(pdev->address.devAddress);
    return;
}





uint16_t timer;
void matrix_init(void) {
}

void hook_early_init(void) {
    Usb.Init();
}

void hook_late_init(void) {
    timer = timer_read();
}

uint8_t matrix_scan(void) {
    return 1;
}

void hook_keyboard_loop(void) {
    static bool waiting = true;
    static uint8_t usb_state = 0;


    Usb.Task();

    if (usb_state != Usb.getUsbTaskState()) {
        usb_state = Usb.getUsbTaskState();
        xprintf("usb_state: %02X\n", usb_state);
        if (!waiting) waiting = true;
    }

    // Wait for console startutp
    if (timer_elapsed(timer) < 3000) {
        return;
    }

    if (Usb.getUsbTaskState() == USB_STATE_RUNNING && waiting) {
        Usb.ForEachUsbDevice(&dumpDescriptors);
        waiting = false;
    }
}


// Keymap unused
const uint8_t keymaps[][MATRIX_ROWS][MATRIX_COLS] PROGMEM = {};
const action_t fn_actions[] PROGMEM = {0};
