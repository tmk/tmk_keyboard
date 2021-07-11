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


#define AUDIO_CLASS_INTERFACE               0x24
#define AUDIO_CLASS_ENDPOINT                0x25


USB Usb;
USBHub hub1(&Usb);

#define BUF_SIZE 512
uint8_t buf[BUF_SIZE];
uint16_t langid = 0;
EpInfo epInfo[1];


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

void dumpBuf(int len, uint8_t* buf, bool commented = false)
{
    for (int i = 0; i < len; i++) {
        if (i % 16)
            print(" ");
        else {
            if (i != 0) print("\r\n");
            if (commented) print("// ");
        }

        xprintf("%02X ", buf[i]);
    }
    print("\r\n");
}

class dumpCallback : public USBReadParser {
public:
  void Parse(const uint16_t len, const uint8_t *pbuf, const uint16_t &offset) {
    for (int i = 0; i < len; i++) {
      if ((offset + i) % 16)
        print(" ");
      else {
        if ((offset + i) != 0) print("\r\n");
      }

      printHEX(pbuf[i]);
    }
  }
} dumper;

void printStringDescriptor(UsbDevice *pdev, uint8_t index, uint16_t langid)
{
    uint8_t sbuf[256];
    uint8_t rcode, len;

    if (index == 0) return;

    rcode = Usb.getStrDescr(pdev->address.devAddress, 0, 1, index, langid, sbuf);
    if (rcode) { printError(rcode); return; }
    len = (sbuf[0] > sizeof(sbuf) ? sizeof(sbuf) : sbuf[0]);

    rcode = Usb.getStrDescr(pdev->address.devAddress, 0, len, index, langid, sbuf);
    if (rcode) { printError(rcode); return; }

    xprintf("String%d: ", index);

    for (uint8_t i = 2; (i + 1) < len; i += 2) {
        if (sbuf[i + 1] != 0) continue;
        //xputc(sbuf[i + 1]);
        xputc(sbuf[i]);
    }
    print("\r\n");

    dumpBuf(sbuf[0], sbuf, true);
}

uint8_t dumpReportDesc(UsbDevice *pdev, uint16_t iface, uint16_t len)
{
    uint8_t rbuf[64];
    uint8_t rcode = 0;
    rcode = Usb.ctrlReq(pdev->address.devAddress, 0x00, bmREQ_HID_REPORT, USB_REQUEST_GET_DESCRIPTOR, 0x00,
                HID_DESCRIPTOR_REPORT, iface, len, sizeof(rbuf), rbuf, &dumper);
    print("\r\n");
    if (rcode) { printError(rcode); }
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

                // String Descriptor
                if (pI->iInterface) {
                  print("\r\n// iInterface: ");
                  printStringDescriptor(pdev, pI->iInterface, langid);
                }
                break;
            }

            case HID_DESCRIPTOR_HID: {
                USB_HID_DESCRIPTOR *pH = (USB_HID_DESCRIPTOR *)pB;
                xprintf("\r\n// Report%d: len:%04X\r\n", iface, pH->wDescriptorLength);
                if (pH->bDescrType == HID_DESCRIPTOR_REPORT) {
                    dumpReportDesc(pdev, iface, pH->wDescriptorLength);
                } else {
                    printVal("// Unknown HID Desc Type:", pH->bDescrType);
                }
                break;
            }

            case USB_DESCRIPTOR_DEBUG:
            case USB_DESCRIPTOR_INTERFACE_ASSOCIATION:
            case USB_DESCRIPTOR_BOS:
            case USB_DESCRIPTOR_DEVICE_CAPABILITY:
            case USB_DESCRIPTOR_SS_USB_EP_COMPANION:
            case USB_DESCRIPTOR_SSP_ISO_EP_COMPANION:
            // Class specific
            case AUDIO_CLASS_INTERFACE:
            case AUDIO_CLASS_ENDPOINT:
                break;
            default:
                printVal("// Unknown Desc Type:", *(pB+1));
                break;
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
        printVal(" len", pC->wTotalLength);
        uint16_t conf_len = (pC->wTotalLength < BUF_SIZE ? pC->wTotalLength : BUF_SIZE);

        rcode = Usb.getConfDescr(pdev->address.devAddress, 0, conf_len, i, buf);
        if (rcode) return rcode;
        dumpBuf(conf_len, buf);
        if (pC->wTotalLength > BUF_SIZE) println("Buffer is not enough!");

        // String Descriptor
        if (pC->iConfiguration) {
            print("\r\n// iConfiguration: ");
            printStringDescriptor(pdev, pC->iConfiguration, langid);
        }

        // Set Configuration
        Usb.setConf(pdev->address.devAddress, 0, pC->bConfigurationValue);

        scanConfigDesc(pdev, conf_len, buf);
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

    // set maxpacketsize0
    epInfo[0].epAddr = 0;
    epInfo[0].maxPktSize = pD->bMaxPacketSize0;
    epInfo[0].bmNakPower = USB_NAK_MAX_POWER;
    UsbDevice *p = Usb.GetAddressPool().GetUsbDevicePtr(pdev->address.devAddress);
    p->epinfo = epInfo;

    // String Descriptor
    if (pD->iManufacturer || pD->iProduct || pD->iSerialNumber) {
        uint8_t sbuf[4];
        rcode = Usb.getStrDescr(pdev->address.devAddress, 0, 4, 0, 0, sbuf);
        if (rcode == 0) {
            langid = (sbuf[3] << 8) | sbuf[2];
            printVal("// langid", langid);

            println("\r\n// String0:");
            dumpBuf(sbuf[0], sbuf, true);
        }
    }
    if (pD->iManufacturer) {
        print("\r\n// iManufacturer: ");
        printStringDescriptor(pdev, pD->iManufacturer, langid);
    }
    if (pD->iProduct) {
        print("\r\n// iProduct: ");
        printStringDescriptor(pdev, pD->iProduct, langid);
    }
    if (pD->iSerialNumber) {
        print("\r\n// iSerialNumber: ");
        printStringDescriptor(pdev, pD->iSerialNumber, langid);
    }

    print("\r\n// Device Descriptor:\r\n");
    dumpBuf(sizeof(USB_DEVICE_DESCRIPTOR), buf);


    //
    // Configuration Descriptor
    //
    uint8_t numConf = pD->bNumConfigurations;
    rcode = dumpConfigDesc(pdev, numConf);
    if (rcode) { printError(rcode); }

    Usb.GetAddressPool().FreeAddress(pdev->address.devAddress);

    println("\r\n// Parse data here: http://eleccelerator.com/usbdescreqparser/");
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
