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
#include "usbhub.h"
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

#include "hook.h"
#include "suspend.h"
#include "lufa.h"


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
HIDBoot<USB_HID_PROTOCOL_KEYBOARD>    kbd1(&usb_host);
HIDBoot<USB_HID_PROTOCOL_KEYBOARD>    kbd2(&usb_host);
HIDBoot<USB_HID_PROTOCOL_KEYBOARD>    kbd3(&usb_host);
HIDBoot<USB_HID_PROTOCOL_KEYBOARD>    kbd4(&usb_host);
KBDReportParser kbd_parser1;
KBDReportParser kbd_parser2;
KBDReportParser kbd_parser3;
KBDReportParser kbd_parser4;
USBHub hub1(&usb_host);
USBHub hub2(&usb_host);


uint8_t matrix_rows(void) { return MATRIX_ROWS; }
uint8_t matrix_cols(void) { return MATRIX_COLS; }
bool matrix_has_ghost(void) { return false; }
void matrix_init(void) {
    debug_enable = true;
    // USB Host Shield setup
    usb_host.Init();
    kbd1.SetReportParser(0, (HIDReportParser*)&kbd_parser1);
    kbd2.SetReportParser(0, (HIDReportParser*)&kbd_parser2);
    kbd3.SetReportParser(0, (HIDReportParser*)&kbd_parser3);
    kbd4.SetReportParser(0, (HIDReportParser*)&kbd_parser4);
}

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

uint8_t matrix_scan(void) {
    static uint16_t last_time_stamp1 = 0;
    static uint16_t last_time_stamp2 = 0;
    static uint16_t last_time_stamp3 = 0;
    static uint16_t last_time_stamp4 = 0;

    // check report came from keyboards
    if (kbd_parser1.time_stamp != last_time_stamp1 ||
        kbd_parser2.time_stamp != last_time_stamp2 ||
        kbd_parser3.time_stamp != last_time_stamp3 ||
        kbd_parser4.time_stamp != last_time_stamp4) {

        last_time_stamp1 = kbd_parser1.time_stamp;
        last_time_stamp2 = kbd_parser2.time_stamp;
        last_time_stamp3 = kbd_parser3.time_stamp;
        last_time_stamp4 = kbd_parser4.time_stamp;

        // clear and integrate all reports
        keyboard_report = {};
        or_report(kbd_parser1.report);
        or_report(kbd_parser2.report);
        or_report(kbd_parser3.report);
        or_report(kbd_parser4.report);

        matrix_is_mod = true;
    } else {
        matrix_is_mod = false;
    }

    uint16_t timer;
    timer = timer_read();
    usb_host.Task();
    timer = timer_elapsed(timer);
    if (timer > 100) {
        xprintf("host.Task: %d\n", timer);
    }

    static uint8_t usb_state = 0;
    if (usb_state != usb_host.getUsbTaskState()) {
        usb_state = usb_host.getUsbTaskState();
        xprintf("usb_state: %02X\n", usb_state);

        // restore LED state when keyboard comes up
        if (usb_state == USB_STATE_RUNNING) {
            xprintf("speed: %s\n", usb_host.getVbusState()==FSHOST ? "full" : "low");
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
}

void led_set(uint8_t usb_led)
{
    if (kbd1.isReady()) kbd1.SetLed(&usb_led);
    if (kbd2.isReady()) kbd2.SetLed(&usb_led);
    if (kbd3.isReady()) kbd3.SetLed(&usb_led);
    if (kbd4.isReady()) kbd4.SetLed(&usb_led);
}

static bool init_done = false;
void hook_late_init()
{
    dprintf("[i]");
    init_done = true;
}

void hook_usb_suspend_loop(void)
{
    dprintf("[s]");
#ifndef TMK_LUFA_DEBUG_UART
    // This corrupts debug print when suspend
    suspend_power_down();
#endif
    if (USB_Device_RemoteWakeupEnabled) {
        if (usb_host.checkRemoteWakeup()) {
            USB_Device_SendRemoteWakeup();
        }
    }
}

static uint8_t _led_stats = 0;
void hook_usb_suspend_entry(void)
{
    dprintf("[S]");
    if (!init_done) return;

    matrix_clear();
    clear_keyboard();

    usb_host.suspend();

#ifdef UHS2_POWER_SAVING
    // power down when remote wake is not enabled or no keyboard is connected
    if (!USB_Device_RemoteWakeupEnabled || usb_host.getUsbTaskState() != USB_STATE_RUNNING) {
        dprintf("[p]");
        usb_host.powerDown();
    }
#endif
}

void hook_usb_wakeup(void)
{
    dprintf("[W]");
    if (!init_done) return;

    suspend_wakeup_init();

#ifdef UHS2_POWER_SAVING
    // power down when remote wake is not enabled or no keyboard is connected
    if (!USB_Device_RemoteWakeupEnabled || usb_host.getUsbTaskState() != USB_STATE_RUNNING) {
        dprintf("[P]");
        usb_host.powerUp();

        // USB state cannot be retained through power down/up cycle
        // device should be enumerated and initialize from the beginning
        usb_host.ReleaseAllDevices();
        usb_host.setUsbTaskState(USB_STATE_DETACHED);
    }
#endif

    usb_host.resume();
}

void hook_usb_startup_wait_loop(void)
{
    usb_host.Task();

    static uint8_t usb_state = 0;
    if (usb_state != usb_host.getUsbTaskState()) {
        usb_state = usb_host.getUsbTaskState();
        dprintf("u:%02X\n", usb_state);
        if (usb_state == USB_STATE_RUNNING) {
            dprintf("s:%s\n", usb_host.getVbusState()==FSHOST ? "f" : "l");
        }
    }
}
