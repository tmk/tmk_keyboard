#include "parser.h"
#include "usb_hid.h"

#include "print.h"


void KBDReportParser::Parse(USBHID *hid, bool is_rpt_id, uint8_t len, uint8_t *buf)
{
    xprintf("input %d:", hid->GetAddress());
    for (uint8_t i = 0; i < len; i++) {
        xprintf(" %02X", buf[i]);
    }
    xprintf("\r\n");

    /* Keyboard can send report in size other than 8 bytes
     * https://github.com/tmk/tmk_keyboard/issues/773
    // boot keyboard report length should be 8
    if (len != 8) {
        xprintf(" ignored\r\n");
        return;
    }
    */

    // Rollover error
    // Cherry: 0101010101010101
    // https://geekhack.org/index.php?topic=69169.msg2638223#msg2638223
    // Apple:  0000010101010101
    // https://geekhack.org/index.php?topic=69169.msg2760969#msg2760969
    if (buf[2] == 0x01) {
       xprintf("Rollover error: ignored\r\n");
       return;
    }

    ::memcpy(&report, buf, sizeof(report_keyboard_t));
    time_stamp = millis();
}
