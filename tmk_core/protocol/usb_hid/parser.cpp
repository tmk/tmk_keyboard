#include "parser.h"
#include "usb_hid.h"

#include "debug.h"


void KBDReportParser::Parse(HID *hid, bool is_rpt_id, uint8_t len, uint8_t *buf)
{
    dprintf("input %d:", hid->GetAddress());
    for (uint8_t i = 0; i < len; i++) {
        dprintf(" %02X", buf[i]);
    }
    dprint("\r\n");

    // ignore Cherry 0101010101010101 bug report
    // https://geekhack.org/index.php?topic=69169.msg2638223#msg2638223
    if (buf[1] == 0x01) {
       dprint("Cherry bug: ignored\r\n");
       return;
    }

    ::memcpy(&report, buf, sizeof(report_keyboard_t));
    time_stamp = millis();
}
