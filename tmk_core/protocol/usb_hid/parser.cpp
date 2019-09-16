#include "parser.h"
#include "usb_hid.h"

#include "debug.h"


void KBDReportParser::Parse(USBHID *hid, bool is_rpt_id, uint8_t len, uint8_t *buf)
{
    dprintf("input %d:", hid->GetAddress());
    for (uint8_t i = 0; i < len; i++) {
        dprintf(" %02X", buf[i]);
    }
    dprint("\r\n");

    // Rollover error
    // Cherry: 0101010101010101
    // https://geekhack.org/index.php?topic=69169.msg2638223#msg2638223
    // Apple:  0000010101010101
    // https://geekhack.org/index.php?topic=69169.msg2760969#msg2760969
    if (buf[2] == 0x01) {
       dprint("Rollover error: ignored\r\n");
       return;
    }

    ::memcpy(&report, buf, sizeof(report_keyboard_t));
    time_stamp = millis();
}


void MOUSEReportParser::Parse(USBHID *hid, bool is_rpt_id, uint8_t len, uint8_t *buf)
{
    dprintf("input %d:", hid->GetAddress());
    for (uint8_t i = 0; i < len; i++) {
        dprintf(" %02X", buf[i]);
    }
    dprint("\r\n");

    ::memcpy(&report, buf, sizeof(report_mouse_t));
    time_stamp = millis();
}
