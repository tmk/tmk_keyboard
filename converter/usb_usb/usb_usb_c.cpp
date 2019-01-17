/*
Copyright 2016 Jun Wako <wakojun@gmail.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <stdint.h>
#include <stdbool.h>

// USB HID host
#include "Usb.h"
#ifdef USBHUB_ENABLE
#   include "usbhub.h"
#endif
#include "usbhid.h"
#include "hidboot.h"
#include "parser.h"

#include "keycode.h"
#include "util.h"
#include "print.h"
#include "debug.h"
#include "timer.h"
#include "matrix.h"
#include "led.h"
#include "host.h"
#include "keyboard.h"


/* KEY CODE to Matrix
 *
 * HID keycode(1 byte):
 * Higher 5 bits indicates ROW and lower 3 bits COL.
 *
 *  7 6 5 4 3 2 1 0
 * +---------------+
 * |  ROW  |  COL  |
 * +---------------+
 *
 * Matrix space(16 * 16):
 *   r\c0123456789ABCDEF
 *   0 +----------------+
 *   : |                |
 *   : |                |
 *  16 +----------------+
 */
#define ROW_MASK 0xF0
#define COL_MASK 0x0F
#define CODE(row, col)  (((row) << 4) | (col))
#define ROW(code)       (((code) & ROW_MASK) >> 4)
#define COL(code)       ((code) & COL_MASK)
#define ROW_BITS(code)  (1 << COL(code))


// Integrated key state of all keyboards
static report_keyboard_t keyboard_report;

static bool matrix_is_mod =false;

/*
 * USB Host Shield HID keyboards
 * This supports two cascaded hubs and four keyboards
 */
USB usb_host;
#ifdef USBHUB_ENABLE
USBHub hub1(&usb_host);
USBHub hub2(&usb_host);
#endif
HIDBoot<USB_HID_PROTOCOL_KEYBOARD>    HidKeyboard1(&usb_host);
KBDReportParser kbd_parser1;
#ifdef MULTI_KBD_ENABLE
HIDBoot<USB_HID_PROTOCOL_KEYBOARD>    HidKeyboard2(&usb_host);
HIDBoot<USB_HID_PROTOCOL_KEYBOARD>    HidKeyboard3(&usb_host);
HIDBoot<USB_HID_PROTOCOL_KEYBOARD>    HidKeyboard4(&usb_host);
KBDReportParser kbd_parser2;
KBDReportParser kbd_parser3;
KBDReportParser kbd_parser4;
#endif
#ifdef COMPOSITE_ENABLE
HIDBoot<USB_HID_PROTOCOL_KEYBOARD | USB_HID_PROTOCOL_MOUSE> HidComposite1(&usb_host);
HIDBoot<USB_HID_PROTOCOL_MOUSE>       HidMouse1(&usb_host);
MOUSEReportParser mouse_parser1;
#endif


uint8_t matrix_rows(void) { return MATRIX_ROWS; }
uint8_t matrix_cols(void) { return MATRIX_COLS; }
bool matrix_has_ghost(void) { return false; }
void matrix_init(void) {
    debug_enable = true;
    // USB Host Shield setup
    usb_host.Init();
    #ifdef COMPOSITE_ENABLE
    HidComposite1.SetReportParser(0, (HIDReportParser*)&kbd_parser1);
    HidComposite1.SetReportParser(1, (HIDReportParser*)&mouse_parser1);
    HidMouse1.SetReportParser(0, (HIDReportParser*)&mouse_parser1);
    #endif
    HidKeyboard1.SetReportParser(0, (HIDReportParser*)&kbd_parser1);
    #ifdef MULTI_KBD_ENABLE
    HidKeyboard2.SetReportParser(0, (HIDReportParser*)&kbd_parser2);
    HidKeyboard3.SetReportParser(0, (HIDReportParser*)&kbd_parser3);
    HidKeyboard4.SetReportParser(0, (HIDReportParser*)&kbd_parser4);
    #endif
}

//#ifdef MULTI_KBD_ENABLE
static void or_report(report_keyboard_t report) {
    // integrate reports into keyboard_report
    keyboard_report.mods |= report.mods;
    for (uint8_t i = 0; i < KEYBOARD_REPORT_KEYS; i++) {
        if (IS_ANY(report.keys[i])) {
            for (uint8_t j = 0; j < KEYBOARD_REPORT_KEYS; j++) {
                if (! keyboard_report.keys[j]) {
                    keyboard_report.keys[j] = report.keys[i];
                    break;
                }
            }
        }
    }
}
//#endif

uint8_t matrix_scan(void) {
    static uint16_t last_time_stamp1 = 0;
    #ifdef MULTI_KBD_ENABLE
    static uint16_t last_time_stamp2 = 0;
    static uint16_t last_time_stamp3 = 0;
    static uint16_t last_time_stamp4 = 0;
    #endif
    #ifdef COMPOSITE_ENABLE
    static uint16_t last_time_stamp1m = 0;
    #endif

    // check report came from keyboards
    if (   kbd_parser1.time_stamp != last_time_stamp1
        #ifdef MULTI_KBD_ENABLE
        || kbd_parser2.time_stamp != last_time_stamp2
        || kbd_parser3.time_stamp != last_time_stamp3
        || kbd_parser4.time_stamp != last_time_stamp4
        #endif
        ) {

        last_time_stamp1 = kbd_parser1.time_stamp;
        #ifdef MULTI_KBD_ENABLE
        last_time_stamp2 = kbd_parser2.time_stamp;
        last_time_stamp3 = kbd_parser3.time_stamp;
        last_time_stamp4 = kbd_parser4.time_stamp;
        #endif

        // clear and integrate all reports
        keyboard_report = {};
        or_report(kbd_parser1.report);
        #ifdef MULTI_KBD_ENABLE
        or_report(kbd_parser2.report);
        or_report(kbd_parser3.report);
        or_report(kbd_parser4.report);
        //#else
        //keyboard_report = kbd_parser1.report;
        #endif

        matrix_is_mod = true;

        dprintf("state:  %02X %02X", keyboard_report.mods, keyboard_report.reserved);
        for (uint8_t i = 0; i < KEYBOARD_REPORT_KEYS; i++) {
            dprintf(" %02X", keyboard_report.keys[i]);
        }
        dprint("\r\n");
    } else {
        matrix_is_mod = false;
    }

    #ifdef COMPOSITE_ENABLE
    //report that comes from mouse
    if ( mouse_parser1.time_stamp != last_time_stamp1m ) {
        last_time_stamp1m = mouse_parser1.time_stamp;
        // disable horizontal wheel for Unifying, work pending.
        mouse_parser1.report.h = 0;
        host_mouse_send(&mouse_parser1.report);
        // clear report
        mouse_parser1.report.x = 0;
        mouse_parser1.report.y = 0;
        mouse_parser1.report.v = 0;
        mouse_parser1.report.h = 0;
        mouse_parser1.report.buttons = 0;
    }
    #endif

    uint16_t timer;
    timer = timer_read();
    usb_host.Task();
    timer = timer_elapsed(timer);
    if (timer > 100) {
        dprintf("host.Task: %d\n", timer);
    }

    static uint8_t usb_state = 0;
    if (usb_state != usb_host.getUsbTaskState()) {
        usb_state = usb_host.getUsbTaskState();
        dprintf("usb_state: %02X\n", usb_state);

        // restore LED state when keyboard comes up
        if (usb_state == USB_STATE_RUNNING) {
            dprintf("speed: %s\n", usb_host.getVbusState()==FSHOST ? "full" : "low");
            keyboard_set_leds(host_keyboard_leds());
        }
    }
    return 1;
}

bool matrix_is_modified(void) {
    return matrix_is_mod;
}

bool matrix_is_on(uint8_t row, uint8_t col) {
    uint8_t code = CODE(row, col);

    if (IS_MOD(code)) {
        if (keyboard_report.mods & ROW_BITS(code)) {
            return true;
        }
    }
    for (uint8_t i = 0; i < KEYBOARD_REPORT_KEYS; i++) {
        if (keyboard_report.keys[i] == code) {
            return true;
        }
    }
    return false;
}

matrix_row_t matrix_get_row(uint8_t row) {
    uint16_t row_bits = 0;

    if (IS_MOD(CODE(row, 0)) && keyboard_report.mods) {
        row_bits |= keyboard_report.mods;
    }

    for (uint8_t i = 0; i < KEYBOARD_REPORT_KEYS; i++) {
        if (IS_ANY(keyboard_report.keys[i])) {
            if (row == ROW(keyboard_report.keys[i])) {
                row_bits |= ROW_BITS(keyboard_report.keys[i]);
            }
        }
    }
    return row_bits;
}

uint8_t matrix_key_count(void) {
    uint8_t count = 0;

    count += bitpop(keyboard_report.mods);
    for (uint8_t i = 0; i < KEYBOARD_REPORT_KEYS; i++) {
        if (IS_ANY(keyboard_report.keys[i])) {
            count++;
        }
    }
    return count;
}

void matrix_print(void) {
    print("\nr/c 0123456789ABCDEF\n");
    for (uint8_t row = 0; row < matrix_rows(); row++) {
        xprintf("%02d: ", row);
        print_bin_reverse16(matrix_get_row(row));
        print("\n");
    }
}

void led_set(uint8_t usb_led)
{
    if (HidKeyboard1.isReady()) HidKeyboard1.SetReport(0, 0, 2, 0, 1, &usb_led);
    #ifdef MULTI_KBD_ENABLE
    if (HidKeyboard2.isReady()) HidKeyboard2.SetReport(0, 0, 2, 0, 1, &usb_led);
    if (HidKeyboard3.isReady()) HidKeyboard3.SetReport(0, 0, 2, 0, 1, &usb_led);
    if (HidKeyboard4.isReady()) HidKeyboard4.SetReport(0, 0, 2, 0, 1, &usb_led);
    #endif
}
