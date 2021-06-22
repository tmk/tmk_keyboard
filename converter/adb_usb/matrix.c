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
#include "print.h"
#include "util.h"
#include "debug.h"
#include "adb.h"
#include "matrix.h"
#include "report.h"
#include "host.h"
#include "led.h"
#include "timer.h"
#include "wait.h"




static bool has_media_keys = false;
static bool is_iso_layout = false;

#if ADB_MOUSE_ENABLE
#define dmprintf(fmt, ...)  do { /* if (debug_mouse) */ xprintf("M:" fmt, ##__VA_ARGS__); } while (0)
static uint16_t mouse_cpi = 100;
static void mouse_init(uint8_t addr);
#endif

// matrix state buffer(1:on, 0:off)
static matrix_row_t matrix[MATRIX_ROWS];

static void register_key(uint8_t key);

static void device_scan(void)
{
    xprintf("\nScan:\n");
    for (uint8_t addr = 0; addr < 16; addr++) {
        uint16_t reg3 = adb_host_talk(addr, ADB_REG_3);
        if (reg3) {
            xprintf(" addr:%d, reg3:%04X\n", addr, reg3);
        }
    }
}

void matrix_init(void)
{
    debug_enable = true;
    //debug_matrix = true;
    //debug_keyboard = true;
    //debug_mouse = true;

    // LED on
    DDRD |= (1<<6); PORTD |= (1<<6);

    adb_host_init();
    adb_host_reset_hard();
    //adb_host_reset(); // some of devices done't recognize

    // AEK/AEKII(ANSI/ISO) startup is slower. Without proper delay
    // it would fail to recognize layout and enable Extended protocol.
    // 200ms seems to be enough for AEKs. 1000ms is used for safety.
    // Tested with devices:
    // M0115J(AEK), M3501(AEKII), M0116(Standard), M1242(Adjustable),
    // G5431(Mouse), 64210(Kensington Trubo Mouse 5)
    wait_ms(1000);

    device_scan();

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
    xprintf("handler: %02X, ISO: %s\n", handler_id, (is_iso_layout ? "yes" : "no"));

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

    // initialize matrix state: all keys off
    for (uint8_t i=0; i < MATRIX_ROWS; i++) matrix[i] = 0x00;

    led_set(host_keyboard_leds());

    device_scan();

    // LED off
    DDRD |= (1<<6); PORTD &= ~(1<<6);
    return;
}

#ifdef ADB_MOUSE_ENABLE
static void mouse_init(uint8_t orig_addr)
{
    uint16_t reg3;
    uint8_t mouse_handler;
    uint8_t addr;

again:
    // Move to tmp address 15 to setup mouse function
    mouse_handler = (reg3  = adb_host_talk(orig_addr, ADB_REG_3)) & 0xFF;
    if (!reg3) return;
    dmprintf("addr%d reg3: %04X\n", orig_addr, reg3);

    // Move device to tmp address
    adb_host_flush(orig_addr);
    adb_host_listen(orig_addr, ADB_REG_3, ((reg3 >> 8) & 0xF0) | ADB_ADDR_TMP, 0xFE);
    adb_host_flush(ADB_ADDR_TMP);

    mouse_handler = (reg3  = adb_host_talk(ADB_ADDR_TMP, ADB_REG_3)) & 0xFF;
    if (!reg3) {
        dmprintf("move fail\n");
        goto again;
    }
    addr = ADB_ADDR_TMP;


detect_again:
    // Try to escalate into extended/classic2 protocol
    if (mouse_handler == ADB_HANDLER_CLASSIC1_MOUSE || mouse_handler == ADB_HANDLER_CLASSIC2_MOUSE) {
        adb_host_flush(addr);
        adb_host_listen(addr, ADB_REG_3, (reg3 >> 8), ADB_HANDLER_EXTENDED_MOUSE);

        mouse_handler = (reg3  = adb_host_talk(addr, ADB_REG_3)) & 0xFF;


        if (mouse_handler == ADB_HANDLER_CLASSIC1_MOUSE) {
            adb_host_flush(addr);
            adb_host_listen(addr, ADB_REG_3, (reg3 >> 8), ADB_HANDLER_CLASSIC2_MOUSE);

            mouse_handler = (reg3  = adb_host_talk(addr, ADB_REG_3)) & 0xFF;
        }
        dmprintf("addr%d reg3: %04X\n", addr, reg3);

    }

    // Classic Protocol 100cpi
    if (mouse_handler == ADB_HANDLER_CLASSIC1_MOUSE) {
        xprintf("Classic 100cpi\n");
        mouse_cpi = 100;
    }

    // Classic Protocol 200cpi
    if (mouse_handler == ADB_HANDLER_CLASSIC2_MOUSE) {
        xprintf("Classic 200cpi\n");
        mouse_cpi = 200;
    }

    // Extended Mouse Protocol
    if (mouse_handler == ADB_HANDLER_EXTENDED_MOUSE) {
        // Device info format(reg1 8-byte data)
        // 0-3: device id
        // 4-5: resolution in units/inch (0xC8=200upi)
        // 6  : device class      (0: Tablet, 1: Mouse, 2: Trackball)
        // 7  : num of buttons
        uint8_t len;
        uint8_t buf[8];
        len = adb_host_talk_buf(addr, ADB_REG_1, buf, sizeof(buf));

        if (len > 5) {
            mouse_cpi = (buf[4]<<8) | buf[5];
        } else {
            mouse_cpi = 100;
        }

        if (len) {
            xprintf("Ext: [", len);
            for (int8_t i = 0; i < len; i++) xprintf("%02X ", buf[i]);
            xprintf("] cpi=%d\n", mouse_cpi);
        }


        // Kensington Turbo Mouse 5: default device
        if (buf[0] == 0x4B && buf[1] == 0x4D && buf[2] == 0x4C && buf[3] == 0x31) {
            xprintf("TM5: default\n");
            // Move it to addr0 to remove this device and get new device with handle id 50 on addr 3
            // and the new device on address 3 should be handled with command sequence later.
            //
            // Turbo Mouse 5 has one default device on addr3 as normal mouse at first, and another device
            // with hander id 50 appears after the default device is moved from addr3.
            // The mouse has the two devices at same time transiently in the result. The default device is
            // removed automatically after the another device receives command sequence.
            // NOTE: The mouse hangs if you try moving the two deivces to same address.
            adb_host_flush(addr);
            adb_host_listen(addr, ADB_REG_3, ((reg3 >> 8) & 0xF0) | ADB_ADDR_0, 0xFE);
        } else {
            xprintf("Unknown\n");
        }
    }

    // Kensington Turbo Mouse 5: setup
    if (mouse_handler == ADB_HANDLER_TURBO_MOUSE) {
        xprintf("TM5: ext\n");

        /* byte0: speed
         *        0xa0, 0xa5, 0xb0 and 0xb5 seem to work
         *        uppper nibble:
         *          0x00-70, 0xc0-f0  no movement and button event
         *          0x80  enables mouse output    speed slow
         *          0x90  enables mouse output
         *          0xa0  enables mouse output
         *          0xb0  enables mouse output    speed fast -126 to 126
         *        lower nibble:
         *          0x08  makes cursor not smooth, bit4 should be 0
         *          0x02  disables button4, bit1 should be 0
         *          how other bits work is not clear.
         * byte1: button mapping    - upper nibble for button1 and lower for button2
         *        0x14  button1 and button2 mapped as themselves
         *          0x0   disabled
         *          0x1   button1
         *          0x2   button1 toggle
         *          0x3   no effect key event FFFF
         *          0x4   button2
         *          0x5   button2 toggle
         *          0x6   button3
         *          0x7   button3 toggle
         *          0x8   ?toggle weirdly?
         *          0x9   button4
         *          0xa   button4 toggle
         *          0xb   ?disabled?
         *          0xc   Left
         *          0xd   Right
         *          0xe   Alt+Left
         *          0xf   Alt+Right
         * byte2: 0x00  - 0x40 on powerup, seems to do nothing
         * byte3: 0x00  - 0x01 on powerup, seems to do nothing
         * byte4: button mapping    - upper nibble for button3 and lower for button4
         *        0x69  button3 and button4 mapped as themselves(see byte1)
         * byte5: 0xff  unknown
         * byte6: 0xff  unknown
         * byte7: 0xff  checksum  - must be 0xff before calculating
         * https://github.com/NetBSD/src/blob/8966d5b1cf335756dd9bba3331e84c659bf917e1/sys/dev/adb/adb_ktm.c#L181
         */
        //static uint8_t cmd[] = { 0xA5, 0x14, 0x00, 0x00, 0x69, 0xFF, 0xFF, 0xFF };
        static uint8_t cmd[] = { 0xB5, 0x14, 0x00, 0x00, 0x69, 0xFF, 0xFF, 0xFF };
        cmd[7] = cmd[0] ^ cmd[1] ^ cmd[2] ^ cmd[3] ^ cmd[4] ^ cmd[5] ^ cmd[6] ^ cmd[7];

        adb_host_flush(addr);
        adb_host_listen_buf(addr, ADB_REG_2, cmd, sizeof(cmd));
    }


    // Move to address 10 for mouse polling
    adb_host_flush(addr);
    adb_host_listen(addr, ADB_REG_3, ((reg3 >> 8) & 0xF0) | ADB_ADDR_MOUSE_POLL, 0xFE);
    adb_host_flush(ADB_ADDR_MOUSE_POLL);

    mouse_handler = (reg3  = adb_host_talk(addr, ADB_REG_3)) & 0xFF;
    if (reg3) {
        dmprintf("detect again\n");
        goto detect_again;
    }

    goto again;
}

static report_mouse_t mouse_report = {};
static int32_t scroll_state = 0;
static uint8_t scroll_speed = ADB_MOUSE_SCROLL_SPEED;
static uint8_t scroll_button_mask = (1 << ADB_MOUSE_SCROLL_BUTTON) >> 1;

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

    static uint16_t detect_ms;
    if (timer_elapsed(detect_ms) > 1000) {
        detect_ms = timer_read();
        // check new device on addr3
        mouse_init(ADB_ADDR_MOUSE);
    }

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
    len = adb_host_talk_buf(ADB_ADDR_MOUSE_POLL, ADB_REG_0, buf, sizeof(buf));

    // If nothing received reset mouse acceleration, and quit.
    if (len < 2) {
        mouseacc = 1;
        return;
    };
    dmprintf("[%02X %02X %02X %02X %02X]\n", buf[0], buf[1], buf[2], buf[3], buf[4]);

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

    // check if the scroll enable button is pressed
    bool scroll_enable = (bool)(buttons & scroll_button_mask);
    // mask out the scroll button so it isn't reported
    buttons &= ~scroll_button_mask;

    mouse_report.buttons = buttons;

    int16_t xx, yy;
    yy = (buf[0] & 0x7F) | (buf[2] & 0x70) << 3 | (buf[3] & 0x70) << 6 | (buf[4] & 0x70) << 9;
    xx = (buf[1] & 0x7F) | (buf[2] & 0x07) << 7 | (buf[3] & 0x07) << 10 | (buf[4] & 0x07) << 13;

    // Accelerate mouse. (They weren't meant to be used on screens larger than 320x200).
    x = xx * mouseacc;
    y = yy * mouseacc;

    #ifndef MOUSE_EXT_REPORT
    x = (x > 127) ? 127 : ((x < -127) ? -127 : x);
    y = (y > 127) ? 127 : ((y < -127) ? -127 : y);
    #endif

    if (scroll_enable) {
        scroll_state -= y;
        mouse_report.v = scroll_state / scroll_speed;
        scroll_state %= scroll_speed;

        mouse_report.x = 0;
        mouse_report.y = 0;
    } else {
        scroll_state = 0;
        mouse_report.v = 0;

        mouse_report.x = x;
        mouse_report.y = y;
    }

    dmprintf("[B:%02X X:%d(%d) Y:%d(%d) V:%d A:%d]\n", mouse_report.buttons, mouse_report.x, xx, mouse_report.y, yy, mouse_report.v, mouseacc);

    // Send result by usb.
    host_mouse_send(&mouse_report);

    // TODO: acceleration curve is needed for precise operation?
    // increase acceleration of mouse
    mouseacc += ( mouseacc < (mouse_cpi < 200 ? ADB_MOUSE_MAXACC : ADB_MOUSE_MAXACC/2) ? 1 : 0 );

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
        if (codes) xprintf("%04X ", codes);

        // Adjustable keybaord media keys
        if (codes == 0 && has_media_keys &&
                (codes = adb_host_kbd_recv(ADB_ADDR_APPLIANCE))) {
            xprintf("m:%04X ", codes);
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

    if (codes == 0) {                           // no keys
        return 0;
    } else if (codes == 0x7F7F) {   // power key press
        register_key(0x7F);
    } else if (codes == 0xFFFF) {   // power key release
        register_key(0xFF);
    } else {
        // Macally keyboard sends keys inversely against ADB protocol
        // https://deskthority.net/workshop-f7/macally-mk96-t20116.html
        if (key0 == 0xFF) {
            key0 = key1;
            key1 = 0xFF;
        }

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
         * *a      0x32    0x0A    GRAVE   GRAVE
         * *b      ----    0x32    ----    NUBS
         * *c      0x2A    0x70    BSLASH  NUHS
         */
        if (is_iso_layout) {
            if ((key0 & 0x7F) == 0x32) {
                key0 = (key0 & 0x80) | 0x0A;
            } else if ((key0 & 0x7F) == 0x0A) {
                key0 = (key0 & 0x80) | 0x32;
            }
            if ((key0 & 0x7F) == 0x2A) {
                key0 = (key0 & 0x80) | 0x70;
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
