/*
Copyright 2011 Jun Wako <wakojun@gmail.com>

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

/*
 * scan matrix
 */
#include <stdint.h>
#include <stdbool.h>
#include <avr/io.h>
#include <util/delay.h>
#include "print.h"
#include "util.h"
#include "debug.h"
#include "adb.h"
#include "matrix.h"
#include "report.h"
#include "host.h"
#include "led.h"
#include "timer.h"




static bool has_media_keys = false;
static bool is_iso_layout = false;
static uint8_t mouse_handler = ADB_HANDLER_CLASSIC1_MOUSE;

// matrix state buffer(1:on, 0:off)
static matrix_row_t matrix[MATRIX_ROWS];

static void register_key(uint8_t key);


void matrix_init(void)
{
    // LED on
    DDRD |= (1<<6); PORTD |= (1<<6);

    adb_host_init();
    // wait for keyboard to boot up and receive command
    _delay_ms(2000);

    // device scan
    xprintf("\nScan:\n");
    for (uint8_t addr = 1; addr < 16; addr++) {
        uint16_t reg3 = adb_host_talk(addr, ADB_REG_3);
        if (reg3) {
            xprintf(" addr:%d, reg3:%04X\n", addr, reg3);
        }
        _delay_ms(20);
    }


    //
    // Keyboard
    //
    xprintf("\nKeyboard:\n");
    // Determine ISO keyboard by handler id
    // http://lxr.free-electrons.com/source/drivers/macintosh/adbhid.c?v=4.4#L815
    uint8_t handler_id = (uint8_t) adb_host_talk(ADB_ADDR_KEYBOARD, ADB_REG_3);
    switch (handler_id) {
    case 0x04: case 0x05: case 0x07: case 0x09: case 0x0D:
    case 0x11: case 0x14: case 0x19: case 0x1D: case 0xC1:
    case 0xC4: case 0xC7:
        is_iso_layout = true;
        break;
    default:
        is_iso_layout = false;
        break;
    }
    xprintf("hadler: %02X, ISO: %s\n", handler_id, (is_iso_layout ? "yes" : "no"));

    // Adjustable keyboard media keys: address=0x07 and handlerID=0x02
    has_media_keys = (0x02 == (adb_host_talk(ADB_ADDR_APPLIANCE, ADB_REG_3) & 0xff));
    if (has_media_keys) {
        xprintf("Media keys\n");
    }

    // Enable keyboard left/right modifier distinction
    // Listen Register3
    //  upper byte: reserved bits 0000, keyboard address 0010
    //  lower byte: device handler 00000011
    adb_host_listen(ADB_ADDR_KEYBOARD, ADB_REG_3, ADB_ADDR_KEYBOARD, ADB_HANDLER_EXTENDED_KEYBOARD);


    #ifdef ADB_MOUSE_ENABLE
    //
    // Mouse
    //
    // https://developer.apple.com/library/archive/technotes/hw/hw_01.html#Extended
    xprintf("\nMouse:\n");

    // Some old mouses seems to need wait between commands.
    _delay_ms(20);
    adb_host_listen(ADB_ADDR_MOUSE, ADB_REG_3, ADB_ADDR_MOUSE, ADB_HANDLER_CLASSIC2_MOUSE);

    _delay_ms(20);
    adb_host_listen(ADB_ADDR_MOUSE, ADB_REG_3, ADB_ADDR_MOUSE, ADB_HANDLER_EXTENDED_MOUSE);

    _delay_ms(20);
    mouse_handler = adb_host_talk(ADB_ADDR_MOUSE, ADB_REG_3) & 0xff;
    mouse_handler = mouse_handler ? mouse_handler : ADB_HANDLER_CLASSIC1_MOUSE;

    // Extended Mouse Protocol
    if (mouse_handler == ADB_HANDLER_EXTENDED_MOUSE) {
        // Device info format(reg1 8-byte data)
        // 0-3: device id
        // 4-5: resolution in units/inch (0xC8=200upi)
        // 6  : device class      (0: Tablet, 1: Mouse, 2: Trackball)
        // 7  : num of buttons
        uint8_t len;
        uint8_t buf[8];
        len = adb_host_talk_buf(ADB_ADDR_MOUSE, ADB_REG_1, buf, sizeof(buf));
        if (len) {
            xprintf("Devinfo: [", len);
            for (int8_t i = 0; i < len; i++) xprintf("%02X ", buf[i]);
            xprintf("]\n");
        }

        // Kensington Turbo Mouse 5
        if (len == 8 && buf[0] == 0x4B && buf[1] == 0x4D && buf[2] == 0x4C && buf[3] == 0x31) {
            xprintf("Turbo Mouse 5\n");

            // Turbo Mouse command sequence to enable four buttons
            // https://elixir.bootlin.com/linux/v4.4/source/drivers/macintosh/adbhid.c#L1176
            // https://github.com/NetBSD/src/blob/64b8a48e1288eb3902ed73113d157af50b2ec596/sys/arch/macppc/dev/ams.c#L261
            static uint8_t cmd1[] = { 0xE7, 0x8C, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x94 };
            static uint8_t cmd2[] = { 0xA5, 0x14, 0x00, 0x00, 0x69, 0xFF, 0xFF, 0x27 };

            // configure with new address 9
            adb_host_listen(ADB_ADDR_MOUSE, ADB_REG_3, 0x69, 0xFE);
            adb_host_flush(9);

            adb_host_flush(ADB_ADDR_MOUSE);
            adb_host_listen_buf(ADB_ADDR_MOUSE, ADB_REG_2, cmd1, sizeof(cmd1));
            adb_host_flush(ADB_ADDR_MOUSE);
            adb_host_listen_buf(ADB_ADDR_MOUSE, ADB_REG_2, cmd2, sizeof(cmd2));
        }

        // Add device specific init here
    }
    mouse_handler = adb_host_talk(ADB_ADDR_MOUSE, ADB_REG_3) & 0xff;
    mouse_handler = mouse_handler ? mouse_handler : ADB_HANDLER_CLASSIC1_MOUSE;
    xprintf("handler: %d\n", mouse_handler);
    #endif


    xprintf("\nScan:\n");
    for (uint8_t addr = 1; addr < 16; addr++) {
        uint16_t reg3 = adb_host_talk(addr, ADB_REG_3);
        if (reg3) {
            xprintf(" addr:%d, reg3:%04X\n", addr, reg3);
        }
        _delay_ms(20);
    }

    // initialize matrix state: all keys off
    for (uint8_t i=0; i < MATRIX_ROWS; i++) matrix[i] = 0x00;

    led_set(host_keyboard_leds());

    debug_enable = true;
    //debug_matrix = true;
    //debug_keyboard = true;
    debug_mouse = true;

    // LED off
    DDRD |= (1<<6); PORTD &= ~(1<<6);
    return;
}

#ifdef ADB_MOUSE_ENABLE

#ifdef MAX
#undef MAX
#endif
#define MAX(X, Y) ((X) > (Y) ? (X) : (Y))

static report_mouse_t mouse_report = {};

void adb_mouse_task(void)
{
    uint8_t len;
    uint8_t buf[5];
    int16_t x, y;
    static int8_t mouseacc;

    /* tick of last polling */
    static uint16_t tick_ms;

    // polling with 12ms interval
    if (timer_elapsed(tick_ms) < 12) return;
    tick_ms = timer_read();

    // Extended Mouse Protocol data can be 2-5 bytes
    // https://developer.apple.com/library/archive/technotes/hw/hw_01.html#Extended
    //
    //   Byte 0: b00 y06 y05 y04 y03 y02 y01 y00
    //   Byte 1: b01 x06 x05 x04 x03 x02 x01 x00
    //   Byte 2: b02 y09 y08 y07 b03 x09 x08 x07
    //   Byte 3: b04 y12 y11 y10 b05 x12 x11 x10
    //   Byte 4: b06 y15 y14 y13 b07 x15 x14 x13
    //
    //   b--: Button state.(0: on, 1: off)
    //   x--: X axis movement.
    //   y--: Y axis movement.
    len = adb_host_talk_buf(ADB_ADDR_MOUSE, ADB_REG_0, buf, sizeof(buf));

    // If nothing received reset mouse acceleration, and quit.
    if (len < 2) {
        mouseacc = 1;
        return;
    };

    // Store off-buttons and 0-movements in unused bytes
    bool xneg = false;
    bool yneg = false;
    if (len == 2) {
        if (buf[0] & 0x40) yneg = true;
        if (buf[1] & 0x40) xneg = true;
    } else {
        if (buf[len - 1] & 0x40) yneg = true;
        if (buf[len - 1] & 0x04) xneg = true;
    }

    for (int8_t i = len; i < sizeof(buf); i++) {
        buf[i] = 0x88;
        if (yneg) buf[i] |= 0x70;
        if (xneg) buf[i] |= 0x07;
    }

    // 8 buttons at max
    // TODO: Fix HID report descriptor for mouse to support button6-8
    uint8_t buttons = 0;
    if (!(buf[4] & 0x08)) buttons |= MOUSE_BTN8;
    if (!(buf[4] & 0x80)) buttons |= MOUSE_BTN7;
    if (!(buf[3] & 0x08)) buttons |= MOUSE_BTN6;
    if (!(buf[3] & 0x80)) buttons |= MOUSE_BTN5;
    if (!(buf[2] & 0x08)) buttons |= MOUSE_BTN4;
    if (!(buf[2] & 0x80)) buttons |= MOUSE_BTN3;
    if (!(buf[1] & 0x80)) buttons |= MOUSE_BTN2;
    if (!(buf[0] & 0x80)) buttons |= MOUSE_BTN1;
    mouse_report.buttons = buttons;

    int16_t xx, yy;
    yy = (buf[0] & 0x7F) | (buf[2] & 0x70) << 3 | (buf[3] & 0x70) << 6 | (buf[4] & 0x70) << 9;
    xx = (buf[1] & 0x7F) | (buf[2] & 0x07) << 7 | (buf[3] & 0x07) << 10 | (buf[4] & 0x07) << 13;

    // Accelerate mouse. (They weren't meant to be used on screens larger than 320x200).
    x = xx * mouseacc;
    y = yy * mouseacc;

    // TODO: Fix HID report descriptor for mouse to support finer resolution
    // Cap our two bytes per axis to one byte.
    // Easier with a MIN-function, but since -MAX(-a,-b) = MIN(a,b)...
    // I.E. MIN(MAX(x,-127),127) = -MAX(-MAX(x, -127), -127) = MIN(-MIN(-x,127),127)
    mouse_report.x = -MAX(-MAX(x, -127), -127);
    mouse_report.y = -MAX(-MAX(y, -127), -127);

    if (debug_mouse) {
        xprintf("Mouse raw: [");
        for (int8_t i = 0; i < len; i++) xprintf("%02X ", buf[i]);
        xprintf("]\n");

        xprintf("Mouse info[");
        xprintf("B:%02X, X:%d(%d), Y:%d(%d), A:%d]\n", buttons, x, xx, y, yy, mouseacc);
    }

    // Send result by usb.
    host_mouse_send(&mouse_report);

    // increase acceleration of mouse
    mouseacc += ( mouseacc < ADB_MOUSE_MAXACC ? 1 : 0 );

    return;
}
#endif

uint8_t matrix_scan(void)
{
    /* extra_key is volatile and more convoluted than necessary because gcc refused
    to generate valid code otherwise. Making extra_key uint8_t and constructing codes
    here via codes = extra_key<<8 | 0xFF; would consistently fail to even LOAD
    extra_key from memory, and leave garbage in the high byte of codes. I tried
    dozens of code variations and it kept generating broken assembly output. So
    beware if attempting to make extra_key code more logical and efficient. */
    static volatile uint16_t extra_key = 0xFFFF;
    uint16_t codes;
    uint8_t key0, key1;

    /* tick of last polling */
    static uint16_t tick_ms;

    codes = extra_key;
    extra_key = 0xFFFF;

    if ( codes == 0xFFFF )
    {
        // polling with 12ms interval
        if (timer_elapsed(tick_ms) < 12) return 0;
        tick_ms = timer_read();

        codes = adb_host_kbd_recv(ADB_ADDR_KEYBOARD);

        // Adjustable keybaord media keys
        if (codes == 0 && has_media_keys &&
                (codes = adb_host_kbd_recv(ADB_ADDR_APPLIANCE))) {
            // key1
            switch (codes & 0x7f ) {
            case 0x00:  // Mic
                codes = (codes & ~0x007f) | 0x42;
                break;
            case 0x01:  // Mute
                codes = (codes & ~0x007f) | 0x4a;
                break;
            case 0x02:  // Volume down
                codes = (codes & ~0x007f) | 0x49;
                break;
            case 0x03:  // Volume Up
                codes = (codes & ~0x007f) | 0x48;
                break;
            case 0x7F:  // no code
                break;
            default:
                xprintf("ERROR: media key1\n");
                return 0x11;
            }
            // key0
            switch ((codes >> 8) & 0x7f ) {
            case 0x00:  // Mic
                codes = (codes & ~0x7f00) | (0x42 << 8);
                break;
            case 0x01:  // Mute
                codes = (codes & ~0x7f00) | (0x4a << 8);
                break;
            case 0x02:  // Volume down
                codes = (codes & ~0x7f00) | (0x49 << 8);
                break;
            case 0x03:  // Volume Up
                codes = (codes & ~0x7f00) | (0x48 << 8);
                break;
            default:
                xprintf("ERROR: media key0\n");
                return 0x10;
            }
        }
    }
    key0 = codes>>8;
    key1 = codes&0xFF;

    if (debug_matrix && codes) {
        print("adb_host_kbd_recv: "); phex16(codes); print("\n");
    }

    if (codes == 0) {                           // no keys
        return 0;
    } else if (codes == 0x7F7F) {   // power key press
        register_key(0x7F);
    } else if (codes == 0xFFFF) {   // power key release
        register_key(0xFF);
    } else if (key0 == 0xFF) {      // error
        xprintf("adb_host_kbd_recv: ERROR(%d)\n", codes);
        // something wrong or plug-in
        matrix_init();
        return key1;
    } else {
        /* Swap codes for ISO keyboard
         * https://github.com/tmk/tmk_keyboard/issues/35
         *
         * ANSI
         * ,-----------    ----------.
         * | *a|  1|  2     =|Backspa|
         * |-----------    ----------|
         * |Tab  |  Q|     |  ]|   *c|
         * |-----------    ----------|
         * |CapsLo|  A|    '|Return  |
         * |-----------    ----------|
         * |Shift   |      Shift     |
         * `-----------    ----------'
         *
         * ISO
         * ,-----------    ----------.
         * | *a|  1|  2     =|Backspa|
         * |-----------    ----------|
         * |Tab  |  Q|     |  ]|Retur|
         * |-----------    -----`    |
         * |CapsLo|  A|    '| *c|    |
         * |-----------    ----------|
         * |Shif| *b|      Shift     |
         * `-----------    ----------'
         *
         *         ADB scan code   USB usage
         *         -------------   ---------
         * Key     ANSI    ISO     ANSI    ISO
         * ---------------------------------------------
         * *a      0x32    0x0A    0x35    0x35
         * *b      ----    0x32    ----    0x64
         * *c      0x2A    0x2A    0x31    0x31(or 0x32)
         */
        if (is_iso_layout) {
            if ((key0 & 0x7F) == 0x32) {
                key0 = (key0 & 0x80) | 0x0A;
            } else if ((key0 & 0x7F) == 0x0A) {
                key0 = (key0 & 0x80) | 0x32;
            }
        }
        register_key(key0);
        if (key1 != 0xFF)       // key1 is 0xFF when no second key.
            extra_key = key1<<8 | 0xFF; // process in a separate call
    }

    return 1;
}

inline
matrix_row_t matrix_get_row(uint8_t row)
{
    return matrix[row];
}

inline
static void register_key(uint8_t key)
{
    uint8_t col, row;
    col = key&0x07;
    row = (key>>3)&0x0F;
    if (key&0x80) {
        matrix[row] &= ~(1<<col);
    } else {
        matrix[row] |=  (1<<col);
    }
}

void led_set(uint8_t usb_led)
{
    adb_host_kbd_led(ADB_ADDR_KEYBOARD, ~usb_led);
}
