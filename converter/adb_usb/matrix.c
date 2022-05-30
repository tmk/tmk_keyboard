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


// matrix state buffer(1:on, 0:off)
static matrix_row_t matrix[MATRIX_ROWS];

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


// ADB device table
static struct adb_device {
    uint8_t addr_default;
    uint8_t handler_default;
    uint8_t handler;
} device_table[16] = {0};

static void print_device_table(void)
{
    xprintf("\nTable:\n");
    xprintf("A:H  a:h\n");
    xprintf("---------\n");
    for (uint8_t addr = 0; addr < 16; addr++) {
        if (device_table[addr].addr_default == 0) continue;
        xprintf("%X:%02X %X:%02X\n", addr,
                device_table[addr].handler,
                device_table[addr].addr_default,
                device_table[addr].handler_default);
    }
    xprintf("\n");
}

static void device_scan(void)
{
    xprintf("\nScan:\n");
    for (uint8_t addr = 1; addr < 16; addr++) {
        uint16_t reg3 = adb_host_talk(addr, ADB_REG_3);
        if (reg3) {
            xprintf("$%X R3:%04X\n", addr, reg3);
        }
    }
    xprintf("\n");
}


////////////////////////////////////////////////////////////////////////////////
// Keyboard handler
////////////////////////////////////////////////////////////////////////////////
enum {
    ADB_LAYOUT_ANSI,
    ADB_LAYOUT_ISO,
    ADB_LAYOUT_JIS
};

static uint8_t keyboard_layout(uint8_t handler)
{
    // https://elixir.bootlin.com/linux/v5.17.4/source/drivers/macintosh/adbhid.c#L802
    switch (handler) {
    case 0x01: case 0x02: case 0x03: case 0x06: case 0x08:
    case 0x0C: case 0x10: case 0x18: case 0x1B: case 0x1C:
    case 0xC0: case 0xC3: case 0xC6:
        return ADB_LAYOUT_ANSI;
    case 0x04: case 0x05: case 0x07: case 0x09: case 0x0D:
    case 0x11: case 0x14: case 0x19: case 0x1D: case 0xC1:
    case 0xC4: case 0xC7:
        return ADB_LAYOUT_ISO;
    case 0x12: case 0x15: case 0x16: case 0x17: case 0x1A:
    case 0x1E: case 0xC2: case 0xC5: case 0xC8: case 0xC9:
        return ADB_LAYOUT_JIS;
    default:
        return ADB_LAYOUT_ANSI;
    }
}

static void keyboard_setup(uint8_t addr)
{
    uint16_t reg3;

    // Check if there is keyboard at default address
    reg3 = adb_host_talk(addr, ADB_REG_3);
    if (!reg3) {
        xprintf("K:$%X:Not found\n", addr);
        return;
    }
    xprintf("K:$%X:Setup. R3:%04X\n", addr, reg3);

    uint8_t adb_layout = keyboard_layout(reg3 & 0xFF);
    xprintf("K:$%X:Layout: %s\n", addr, (adb_layout == ADB_LAYOUT_ANSI ? "ANSI" : (
                                         adb_layout == ADB_LAYOUT_ISO  ? "ISO"  : (
                                         adb_layout == ADB_LAYOUT_JIS  ? "JIS"  : "???"))));

    // Enable Extended protocol
    adb_host_listen(addr, ADB_REG_3, (reg3 >> 8), ADB_HANDLER_EXTENDED_KEYBOARD);
    reg3 = adb_host_talk(addr, ADB_REG_3);
    device_table[addr].handler = reg3 & 0xFF;

    // Keyboard indicator
    adb_host_kbd_led(addr, ~(host_keyboard_leds()));
}

static uint8_t keyboard_proc(uint8_t addr)
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

    codes = extra_key;
    extra_key = 0xFFFF;

    if ( codes == 0xFFFF ) {
        codes = adb_host_kbd_recv(addr);
        if (codes) xprintf("K:$%X:%02X:%04X ", addr, device_table[addr].handler_default, codes);
    }
    key0 = codes>>8;
    key1 = codes&0xFF;

    if (codes == 0) {               // no keys
        return 0;
    } else if (codes == 0x7F7F) {   // power key press
        register_key(0x7F);
    } else if (codes == 0xFFFF) {   // power key release
        register_key(0xFF);

        // for debug
        device_scan();
        print_device_table();
    } else {
        // Macally keyboard sends keys inversely against ADB protocol
        // https://deskthority.net/workshop-f7/macally-mk96-t20116.html
        if (key0 == 0xFF) {
            key0 = key1;
            key1 = 0xFF;
        }

        /* Keyboard code translation
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
         * |Shift   |     |Shift     |
         * |-----------    ----------|
         *
         * ISO
         * ,-----------    ----------.
         * | *a|  1|  2     =|Backspa|
         * |-----------    ----------|
         * |Tab  |  Q|     |  ]|Retur|
         * |-----------    -----`    |
         * |CapsLo|  A|    '| *c|    |
         * |-----------    ----------|
         * |Shif| *b|     |Shift     |
         * |-----------    ----------|
         *
         * JIS
         * ,-----------    ----------.
         * | *a|  1|  2     =| *d| BS|
         * |-----------    ----------|
         * |Tab  |  Q|     |  [|Retur|
         * |-----------    -----`    |
         * |CapsLo|  A|    :| *c|    |
         * |-----------    ----------|
         * |Shift   |     | *e| Shift|
         * |-----------    ----------|
         *
         *         ADB scan code        USB usage
         *         -------------        ---------
         * Key     ANSI   ISO    JIS    ANSI    ISO     JIS
         * ---------------------------------------------------
         * *a      0x32   0x0A   0x32   GRAVE   GRAVE   GRAVE
         * *b      ----   0x32   ----   ----    NUBS    ----
         * *c      0x2A   0x70   0x2A   BSLASH  NUHS    NUHS
         * *d      ----   ----   0x5D   ----    ----    JPY
         * *e      ----   ----   0x5E   ----    ----    RO
         */
        uint8_t adb_layout = keyboard_layout(device_table[addr].handler_default);
        if (adb_layout == ADB_LAYOUT_ISO) {
            // 32 <-> 0A
            if ((key0 & 0x7F) == 0x32) {
                key0 = (key0 & 0x80) | 0x0A;
            } else if ((key0 & 0x7F) == 0x0A) {
                key0 = (key0 & 0x80) | 0x32;
            }
            // 2A -> 70
            if ((key0 & 0x7F) == 0x2A) {
                key0 = (key0 & 0x80) | 0x70;
            }
        }
        if (adb_layout == ADB_LAYOUT_JIS) {
            // 2A -> 70
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


////////////////////////////////////////////////////////////////////////////////
// Mouse handler
////////////////////////////////////////////////////////////////////////////////
#ifdef ADB_MOUSE_ENABLE
static void mouse_setup(uint8_t addr)
{
    uint16_t mouse_cpi = 100;
    uint8_t mouse_handler;
    uint16_t reg3;

    // Check if there is mouse device to setup
    reg3 = adb_host_talk(addr, ADB_REG_3);
    if (!reg3) {
        xprintf("M:$%X:Not found\n", addr);
        return;
    }
    xprintf("M:$%X:Setup. R3:%04X\n", addr, reg3);
    mouse_handler = reg3 & 0xFF;

    if (mouse_handler == ADB_HANDLER_MICROSPEED_MACTRAC ||
        mouse_handler == ADB_HANDLER_MICROSPEED_UNKNOWN ||
        mouse_handler == ADB_HANDLER_CONTOUR_MOUSE ||
        mouse_handler == ADB_HANDLER_CHPRODUCTS_PRO) {
        // https://github.com/NetBSD/src/blob/netbsd-9/sys/arch/macppc/dev/ams.c#L226-L255
        // https://github.com/torvalds/linux/blob/v5.17/drivers/macintosh/adbhid.c#L1007-L1018
        // https://github.com/torvalds/linux/blob/v5.17/drivers/macintosh/adbhid.c#L1204-L1239
        uint8_t cmd[] = { 0x00,     // alt speed max
                          0x00,     // speed max
                          0x10,     // ext protocol enabled
                          0x07 };   // buttons without locking
        adb_host_listen_buf(addr, ADB_REG_1, cmd, sizeof(cmd));
    }

    // Try to escalate into extended/classic2 protocol
    if (mouse_handler == ADB_HANDLER_CLASSIC1_MOUSE || mouse_handler == ADB_HANDLER_CLASSIC2_MOUSE) {
        adb_host_listen(addr, ADB_REG_3, (reg3 >> 8), ADB_HANDLER_EXTENDED_MOUSE);
        mouse_handler = (reg3 = adb_host_talk(addr, ADB_REG_3)) & 0xFF;

        if (mouse_handler != ADB_HANDLER_EXTENDED_MOUSE) {
            adb_host_listen(addr, ADB_REG_3, (reg3 >> 8), ADB_HANDLER_MOUSESYSTEMS_A3);
            mouse_handler = (reg3 = adb_host_talk(addr, ADB_REG_3)) & 0xFF;

            if (mouse_handler == ADB_HANDLER_MOUSESYSTEMS_A3) {
                adb_host_listen(addr, ADB_REG_2, 0x00, 0x07);
            }
        }

        if (mouse_handler == ADB_HANDLER_CLASSIC1_MOUSE) {
            adb_host_listen(addr, ADB_REG_3, (reg3 >> 8), ADB_HANDLER_CLASSIC2_MOUSE);
            mouse_handler = (reg3 = adb_host_talk(addr, ADB_REG_3)) & 0xFF;
        }
    }

    // Classic Protocol 100cpi
    if (mouse_handler == ADB_HANDLER_CLASSIC1_MOUSE) {
        xprintf("M:$%X:Classic1 100cpi\n", addr);
        mouse_cpi = 100;
    }

    // Classic Protocol 200cpi
    if (mouse_handler == ADB_HANDLER_CLASSIC2_MOUSE) {
        xprintf("M:$%X:Classic2 200cpi\n", addr);
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
            xprintf("M:$%X:EXT: [%02X %02X %02X %02X %02X %02X %02X %02X] cpi=%d btn=%d len=%d\n",
                    addr, buf[0], buf[1], buf[2], buf[3], buf[4], buf[5], buf[6], buf[7], mouse_cpi, buf[7], len);
        }

        if (buf[0] == 0x4B && buf[1] == 0x4D && buf[2] == 0x4C && buf[3] == 0x31) {
            xprintf("M:$%X:Kensington Ext\n", addr);
            // Kensington Turbo Mouse 5/Thinking Mouse: Extended device
            // Another device, whose handler ID is $32, becomes available in address 3
            // when this device is moved from address 3.
            // This device will disappear automatically once another device is setup.
        } else if (buf[0] == 0x4B && buf[1] == 0x4F && buf[2] == 0x49 && buf[3] == 0x54) {
            // Macally 2-button Mouse
            // https://elixir.bootlin.com/linux/v5.17/source/drivers/macintosh/adbhid.c#L1068
            adb_host_flush(addr);
            adb_host_listen(addr, ADB_REG_3, (reg3 >> 8), ADB_HANDLER_MACALLY2_MOUSE);
            mouse_handler = (reg3 = adb_host_talk(addr, ADB_REG_3)) & 0xFF;
            xprintf("M:$%X:Macally-2btn\n", addr);
        } else if (buf[0] == 0x9A && (buf[1] == 0x20 || buf[1] == 0x21)) {
            // Logitech MouseMan/TrackMan proprietary
            // buf[1] = 0x20:MouseMan, 0x21:TrackMan
            xprintf("M:$%X:Logitech\n", addr);

            // https://elixir.bootlin.com/linux/v5.17/source/drivers/macintosh/adbhid.c#L1047
            adb_host_listen(addr, ADB_REG_1, 0x00, 0x81);
            adb_host_listen(addr, ADB_REG_1, 0x01, 0x81);
            adb_host_listen(addr, ADB_REG_1, 0x02, 0x81);
            adb_host_listen(addr, ADB_REG_1, 0x03, 0x38);

            // set pseudo handler
            mouse_handler = ADB_HANDLER_LOGITECH;
        } else if (buf[0] == 0x4C && buf[1] == 0x54) {
            // Logitech MouaseMan/TrackMan Extended
            // MouseMan - FCCID:DZLMAH32 'LT01'
            // MouseMan Cordless - FCCID:DZLMRC33T 'LTW1'
            xprintf("M:$%X:Logitech-Extended\n", addr);

            // set pseudo handler
            mouse_handler = ADB_HANDLER_LOGITECH_EXT;
        } else {
            xprintf("M:$%X:Extended\n", addr);
        }
    }

    // Kensington Turbo Mouse 5: setup
    if (mouse_handler == ADB_HANDLER_TURBO_MOUSE) {
        xprintf("M:$%X:Kensington\n", addr);

        /* byte0: 0xb5  speed - 0xa0, 0xa5, 0xb0 and 0xb5 seem to work
         *              uppper nibble:
         *                  0x00-70, 0xc0-f0  no movement and button event
         *                  0x80  enables mouse output    speed slow
         *                  0x90  enables mouse output
         *                  0xa0  enables mouse output
         *                  0xb0  enables mouse output    speed fast -126 to 126
         *              lower nibble:
         *                  0x08  makes cursor not smooth, bit3 should be 0
         *                  0x02  disables button4, bit1 should be 0
         *                  how other bits work is not clear.
         * byte1: 0x14  button mapping - upper nibble for button1 and lower for button2
         *              button1 and button2 mapped as themselves
         *                  0x0   disabled
         *                  0x1   button1
         *                  0x2   button1 toggle
         *                  0x3   no effect key event FFFF
         *                  0x4   button2
         *                  0x5   button2 toggle
         *                  0x6   button3
         *                  0x7   button3 toggle
         *                  0x8   ?toggle weirdly?
         *                  0x9   button4
         *                  0xa   button4 toggle
         *                  0xb   ?disabled?
         *                  0xc   Left
         *                  0xd   Right
         *                  0xe   Alt+Left
         *                  0xf   Alt+Right
         * byte2: 0x00  - 0x40 on powerup, seems to do nothing
         * byte3: 0x00  - 0x01 on powerup, seems to do nothing
         * byte4: 0x69  button mapping - upper nibble for button3 and lower for button4
         *              button3 and button4 mapped as themselves(see byte1)
         * byte5: 0xff  unknown
         * byte6: 0xff  unknown
         * byte7: 0x??  checksum
         *              byte7 = byte0 ^ byte1 ^ byte2 ^ byte3 ^ byte4 ^ byte5 ^ byte6 ^ 0xFF;
         * https://github.com/NetBSD/src/blob/8966d5b1cf335756dd9bba3331e84c659bf917e1/sys/dev/adb/adb_ktm.c#L181
         */
        uint8_t cmd[] = { 0xB5, 0x14, 0x00, 0x00, 0x69, 0xFF, 0xFF, 0x37 };
        // cmd[7] = cmd[0] ^ cmd[1] ^ cmd[2] ^ cmd[3] ^ cmd[4] ^ cmd[5] ^ cmd[6] ^ 0xFF;
        adb_host_flush(addr);
        adb_host_listen_buf(addr, ADB_REG_2, cmd, sizeof(cmd));
    }

    device_table[addr].handler = mouse_handler;
    return;
}

static report_mouse_t mouse_report = {};
static int32_t scroll_state = 0;
static uint8_t scroll_speed = ADB_MOUSE_SCROLL_SPEED;
static uint8_t scroll_button_mask = (1 << ADB_MOUSE_SCROLL_BUTTON) >> 1;

static uint8_t mouse_proc(uint8_t addr)
{
    uint8_t len;
    uint8_t buf[5];
    int16_t x, y;
    uint8_t mouse_handler;

    //if (device_table[addr].addr_default != ADB_ADDR_MOUSE) return;
    mouse_handler = device_table[addr].handler;

    len = adb_host_talk_buf(addr, ADB_REG_0, buf, sizeof(buf));
    if (len < 2) {
        return 0;
    };

    xprintf("M:$%X:%02X:[ ", addr, mouse_handler);
    for (uint8_t i = 0; i < len; i++)
        xprintf("%02X ", buf[i]);
    xprintf("] ");

    bool xneg = false;
    bool yneg = false;
    // Convert data into Apple Extended Mouse format
    //
    // Apple Extended Mouse:
    //   Byte0: b00 y06 y05 y04 y03 y02 y01 y00
    //   Byte1: b01 x06 x05 x04 x03 x02 x01 x00
    //   Byte2: b02 y09 y08 y07 b03 x09 x08 x07
    //   Byte3: b04 y12 y11 y10 b05 x12 x11 x10
    //   Byte4: b06 y15 y14 y13 b07 x15 x14 x13
    //     b--: button state(0:pressed, 1:released)
    //     Data can be 2-5 bytes.
    //     L=b00, R=b01, M=b02
    if (mouse_handler == ADB_HANDLER_LOGITECH) {
        // Logitech:
        //   Byte0: bbb y06 y05 y04 y03 y02 y01 y00
        //   Byte1: 1   x06 x05 x04 x03 x02 x01 x00
        //   Byte2: 0   0   0   0   0   BL  BM  BR
        //     Bx: button state(1:pressed, 0:released)
        //     bbb: 0 when either BL, BR or BM is pressed
        if (buf[0] & 0x40) yneg = true;
        if (buf[1] & 0x40) xneg = true;
        if (buf[2] & 0x04) buf[0] &= 0x7F; else buf[0] |= 0x80;
        if (buf[2] & 0x01) buf[1] &= 0x7F; else buf[1] |= 0x80;
        if (buf[2] & 0x02) buf[2] = 0x08;  else buf[2] = 0x88;
        if (yneg) buf[2] |= 0x70;
        if (xneg) buf[2] |= 0x07;
        len = 3;
    } else if (mouse_handler == ADB_HANDLER_LOGITECH_EXT) {
        // Logitech Extended:
        //   Byte0: b00 y06 y05 y04 y03 y02 y01 y00
        //   Byte1: b02 x06 x05 x04 x03 x02 x01 x00
        //   Byte2: b01 y09 y08 y07 b03 x09 x08 x07
        //     L=b00, R=b01, M=b02
        uint8_t tmp = buf[2];
        if (buf[1] & 0x80) buf[2] |= 0x80; else buf[2] &= 0x7F;
        if (tmp    & 0x80) buf[1] |= 0x80; else buf[1] &= 0x7F;
        if (buf[len - 1] & 0x40) yneg = true;
        if (buf[len - 1] & 0x04) xneg = true;
    } else if (mouse_handler == ADB_HANDLER_MACALLY2_MOUSE && len == 4) {
        // Macally 2-button mouse:
        //   Byte0: b00 y06 y05 y04 y03 y02 y01 y00
        //   Byte1: b01 x06 x05 x04 x03 x02 x01 x00
        //   Byte2: 1   0   0   0   1   0   0   0
        //   Byte3: 1   0   0   0   1   0   0   0
        //     b--: button state(0:pressed, 1:released)
        if (buf[0] & 0x40) yneg = true;
        if (buf[1] & 0x40) xneg = true;
        // Ignore Byte2 and 3
        len = 2;
    } else if (mouse_handler == ADB_HANDLER_MICROSPEED_MACTRAC ||
               mouse_handler == ADB_HANDLER_MICROSPEED_UNKNOWN ||
               mouse_handler == ADB_HANDLER_CONTOUR_MOUSE) {
        // Microspeed:
        //   Byte0: ??? y06 y05 y04 y03 y02 y01 y00
        //   Byte1: ??? x06 x05 x04 x03 x02 x01 x00
        //   Byte2: ??? ??? ??? ??? ??? bM  bR  bL
        // Contour Mouse:
        //   Byte0: bbb y06 y05 y04 y03 y02 y01 y00
        //   Byte1: 1   x06 x05 x04 x03 x02 x01 x00
        //   Byte2: 0   0   0   0   1   bM  bR  bL
        //   Byte3: 0   0   0   0   1   bM  bR  bL
        //     b--: button state(0:pressed, 1:released)
        if (buf[0] & 0x40) yneg = true;
        if (buf[1] & 0x40) xneg = true;
        buf[0] = ((buf[2] & 1) << 7) | (buf[0] & 0x7F);
        buf[1] = ((buf[2] & 2) << 6) | (buf[1] & 0x7F) ;
        buf[2] = ((buf[2] & 4) << 5) | (buf[2] & 8) | (yneg ? 0x70 : 0x00) | (xneg ? 0x07 : 0x00);
        len = 3;
    } else if (mouse_handler == ADB_HANDLER_CHPRODUCTS_PRO) {
        // CH Products Trackball Pro:
        //   Byte0: ??? y06 y05 y04 y03 y02 y01 y00
        //   Byte1: ??? x06 x05 x04 x03 x02 x01 x00
        //   Byte2: ??? ??? ??? ??? bL0 bL1 bR  bM
        //     b--: button state(0:pressed, 1:released)
        //     L=(bL0 & bL1)
        if (buf[0] & 0x40) yneg = true;
        if (buf[1] & 0x40) xneg = true;
        buf[0] = (((buf[2] & 4) << 5) & ((buf[2] & 8) << 4)) | (buf[0] & 0x7F);
        buf[1] = ((buf[2] & 2) << 6) | (buf[1] & 0x7F) ;
        buf[2] = ((buf[2] & 1) << 7) | (yneg ? 0x70 : 0x00) | (xneg ? 0x0F : 0x08);
        len = 3;
    } else if (mouse_handler == ADB_HANDLER_MOUSESYSTEMS_A3) {
        // Mouse Systems A3: 3-button mouse/trackball:
        //   Byte0: ??? y06 y05 y04 y03 y02 y01 y00
        //   Byte1: ??? x06 x05 x04 x03 x02 x01 x00
        //   Byte2: ??? ??? ??? ??? ??? bR  bM  bL
        //     b--: button state(0:pressed, 1:released)
        if (buf[0] & 0x40) yneg = true;
        if (buf[1] & 0x40) xneg = true;
        buf[0] = ((buf[2] & 1) << 7) | (buf[0] & 0x7F);
        buf[1] = ((buf[2] & 4) << 5) | (buf[1] & 0x7F) ;
        buf[2] = ((buf[2] & 2) << 6) | (yneg ? 0x70 : 0x00) | (xneg ? 0x0F : 0x08);
        len = 3;
    } else if (mouse_handler == ADB_HANDLER_EXTENDED_MOUSE ||
               mouse_handler == ADB_HANDLER_TURBO_MOUSE) {
        // Apple Extended Mouse:
        if (buf[len - 1] & 0x40) yneg = true;
        if (buf[len - 1] & 0x04) xneg = true;
    } else {
        // Apple Classic Mouse and Unknown devices:
        //   Byte0: b00 y06 y05 y04 y03 y02 y01 y00
        //   Byte1: b01 x06 x05 x04 x03 x02 x01 x00
        if (buf[0] & 0x40) yneg = true;
        if (buf[1] & 0x40) xneg = true;
        len = 2;

        #ifdef ADB_MOUSE_2ND_BUTTON_QUIRK
        // Ignore b01('optional second button') as OSX/MacOS9 does.
        // Some mouses misuse the bit and make it unusable.
        // https://github.com/tmk/tmk_keyboard/issues/724
        buf[1] |= 0x80;
        #endif
    }

    // Make unused buf bytes compatible with Extended Mouse Protocol
    for (int8_t i = len; i < sizeof(buf); i++) {
        buf[i] = 0x88;
        if (yneg) buf[i] |= 0x70;
        if (xneg) buf[i] |= 0x07;
    }

    /* for debug
    xprintf("[ ");
    for (uint8_t i = 0; i < 5; i++)
        xprintf("%02X ", buf[i]);
    xprintf("] ");
    */

    uint8_t buttons = 0;
    if (!(buf[4] & 0x08)) buttons |= MOUSE_BTN8;
    if (!(buf[4] & 0x80)) buttons |= MOUSE_BTN7;
    if (!(buf[3] & 0x08)) buttons |= MOUSE_BTN6;
    if (!(buf[3] & 0x80)) buttons |= MOUSE_BTN5;
    if (!(buf[2] & 0x08)) buttons |= MOUSE_BTN4;
    if (!(buf[2] & 0x80)) buttons |= MOUSE_BTN3;    // Middle
    if (!(buf[1] & 0x80)) buttons |= MOUSE_BTN2;    // Right
    if (!(buf[0] & 0x80)) buttons |= MOUSE_BTN1;    // Left

    // check if the scroll enable button is pressed
    bool scroll_enable = (bool)(buttons & scroll_button_mask);
    // mask out the scroll button so it isn't reported
    buttons &= ~scroll_button_mask;

    mouse_report.buttons = buttons;

    int16_t xx, yy;
    y = yy = (buf[0] & 0x7F) | (buf[2] & 0x70) << 3 | (buf[3] & 0x70) << 6 | (buf[4] & 0x70) << 9;
    x = xx = (buf[1] & 0x7F) | (buf[2] & 0x07) << 7 | (buf[3] & 0x07) << 10 | (buf[4] & 0x07) << 13;

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

    xprintf("[ B:%02X X:%d(%d) Y:%d(%d) V:%d ]\n",
            mouse_report.buttons, mouse_report.x, xx, mouse_report.y, yy, mouse_report.v);

    // Send result by usb.
    host_mouse_send(&mouse_report);

    return 1;
}

uint8_t adb_mouse_buttons(void)
{
    return mouse_report.buttons;
}
#endif


////////////////////////////////////////////////////////////////////////////////
// Appliance handler
////////////////////////////////////////////////////////////////////////////////
static void appliance_setup(uint8_t addr)
{
    // Adjustable keyboard M1242 media keys: address=7 and handler=2
    xprintf("A:$%X:Appliance(Media keys)\n", addr);
}

static uint8_t appliance_keymap(uint8_t code)
{
    switch (code & 0x7f ) {
    case 0x00: return (code & 0x80) | 0x42; // Mic -> F13
    case 0x01: return (code & 0x80) | 0x4a; // Mute -> MUTE
    case 0x02: return (code & 0x80) | 0x49; // Volume down -> VOLD
    case 0x03: return (code & 0x80) | 0x48; // Volume Up -> VOLU
    case 0x7F: return 0;                    // no code
    default:
        xprintf("m:Unknown: %02X\n", code);
        return 0;
    }
}

static uint8_t appliance_proc(uint8_t addr)
{
    // Adjustable keybaord M1242 media keys: handler=2
    uint16_t codes = adb_host_kbd_recv(addr);
    if (!codes) return 0;
    xprintf("m:$%X:%04X ", addr, codes);

    uint8_t key;
    if ((codes >> 8) != 0xFF) {
        key = appliance_keymap(codes >> 8);
        if (key) {
            register_key(key);
        }
    }
    if ((codes & 0xFF) != 0xFF) {
        key = appliance_keymap(codes & 0xFF);
        if (key) {
            register_key(key);
        }
    }

    return 1;
}


////////////////////////////////////////////////////////////////////////////////
// Address Resolution - hot-plug support
////////////////////////////////////////////////////////////////////////////////
static bool update_device_table(void)
{
    bool changed = false;
    // remove entry for non-existent device
    for (uint8_t addr = 8; addr < 16; addr++) {
        uint16_t reg3 = adb_host_talk(addr, ADB_REG_3);
        if (!reg3) {
            if (device_table[addr].addr_default) {
                changed = true;
                device_table[addr] = (struct adb_device){0};
            }
        }
    }
    return changed;
}

static uint8_t free_address(void)
{
    // address for dynamic assignment
    for (uint8_t addr = 15; addr > 7; addr--) {
        if (device_table[addr].addr_default == 0)
            return addr;
    }
    return 0;
}

static void resolve_address(void)
{
    bool changed = false;
    uint16_t reg3;
    // Find new device at address 1 to 7
    for (uint8_t addr = 1; addr < 8; addr++) {
again:
        reg3 = adb_host_talk(addr, ADB_REG_3);
        if (!reg3) {
            if (device_table[addr].addr_default) {
                changed = true;
                device_table[addr] = (struct adb_device){0};
            }
            continue;
        }

        changed = true;
        xprintf("R:$%X:Found. R3:%04X\n", addr, reg3);
        uint8_t new_addr = free_address();
        if (!new_addr) {
            xprintf("R:$%X:No free address\n", addr);
            continue;
        }

        // Move device to higher address 8 to 15
        adb_host_listen(addr, ADB_REG_3, ((reg3 >> 8) & 0xF0) | new_addr, 0xFE);
        reg3 = adb_host_talk(new_addr, ADB_REG_3);
        if (!reg3) {
            xprintf("R:$%X:Fail to move\n", addr);
            continue;
        }
        device_table[new_addr].addr_default = addr;
        device_table[new_addr].handler_default = reg3 & 0xFF;
        device_table[new_addr].handler = reg3 & 0xFF;
        xprintf("R:$%X:Move to $%X. R3:%04X\n", addr, new_addr, reg3);

        // Setup supported device
        if (addr == ADB_ADDR_KEYBOARD)  keyboard_setup(new_addr);
        if (addr == ADB_ADDR_MOUSE)     mouse_setup(new_addr);
        if (addr == ADB_ADDR_APPLIANCE) appliance_setup(new_addr);

        goto again; // if addr still has another deivce
    }

    // update table entry
    changed |= update_device_table();

    if (changed) {
        print_device_table();
    }
}


////////////////////////////////////////////////////////////////////////////////
// Core API
////////////////////////////////////////////////////////////////////////////////
void hook_late_init(void)
{
    debug_enable = true;
    //debug_matrix = true;
    //debug_keyboard = true;
    //debug_mouse = true;

    for (uint8_t addr = 0; addr < 16; addr++) {
        device_table[addr].addr_default = 0;
        device_table[addr].handler_default = 0;
        device_table[addr].handler = 0;
    }

    // initialize matrix state: all keys off
    for (uint8_t i=0; i < MATRIX_ROWS; i++) matrix[i] = 0x00;

    // LED on
    DDRD |= (1<<6); PORTD |= (1<<6);

    adb_host_init();
    adb_host_reset_hard();
    //adb_host_reset(); // some of devices done't recognize

    device_scan();

    // LED off
    DDRD |= (1<<6); PORTD &= ~(1<<6);
    return;
}

void matrix_init(void)
{
}

uint8_t matrix_scan(void)
{
    return 0;
}

inline
matrix_row_t matrix_get_row(uint8_t row)
{
    return matrix[row];
}

void led_set(uint8_t usb_led)
{
    for (uint8_t addr = 15; addr > 7; addr--) {
        if (device_table[addr].addr_default == ADB_ADDR_KEYBOARD) {
            adb_host_kbd_led(addr, ~usb_led);
        }
    }
    adb_host_kbd_led(ADB_ADDR_KEYBOARD, ~usb_led);
}

void hook_main_loop(void)
{
    static uint16_t poll_ms;
    static uint16_t detect_ms;
    static uint8_t active_addr = 3;
    static bool psw_state = false;

    uint8_t len;
    uint8_t buf[8];
    uint8_t addr = active_addr;
    uint8_t busy = 0;

    // Polling with 11ms interval
    if (timer_elapsed(poll_ms) >= 11) {
        poll_ms = timer_read();
        do {
            addr %= 16;

            // Ignore Address 0
            if (addr == 0) continue;

            switch (device_table[addr].addr_default) {
            case ADB_ADDR_KEYBOARD:
                busy = keyboard_proc(addr);
                break;
            case ADB_ADDR_MOUSE:
                busy = mouse_proc(addr);
                break;
            case ADB_ADDR_APPLIANCE:
                busy = appliance_proc(addr);
                break;
            case 0:
                // 'Dumb' device may exist at 2, 3 and 7 #733
                switch (addr) {
                case ADB_ADDR_KEYBOARD:
                    busy = keyboard_proc(addr);
                    break;
                case ADB_ADDR_MOUSE:
                    busy = mouse_proc(addr);
                    break;
                case ADB_ADDR_APPLIANCE:
                    busy = appliance_proc(addr);
                    break;
                default:
                    // No device - Poll next address
                    continue;
                }
                break;
            default:
                // Unsupported device
                len = adb_host_talk_buf(addr, ADB_REG_0, buf, sizeof(buf));
                if (len) {
                    xprintf("U:$%X:%02X:[ ", addr, device_table[addr].handler);
                    for (uint8_t i = 0; i < len; i++) {
                        xprintf("%02X ", buf[i]);
                    }
                    xprintf("]\n");
                    busy = 1;
                }
                break;
            }

            if (!adb_service_request()) {
                // No SRQ - Done.
                break;
            }
        } while (++addr != active_addr);
        active_addr = addr % 16;

        // Check PSW pin for NeXT keyboard
        // https://github.com/tmk/tmk_keyboard/issues/735
        if (!psw_state) {
            if (!adb_host_psw()) {
                register_key(0x7F); // power key press
                psw_state = true;
            }
        } else {
            if (adb_host_psw()) {
                register_key(0xFF); // power key release
                psw_state = false;
            }
        }
    }

    // Address Resolution
    if (!busy) {
        if (timer_elapsed(detect_ms) >= 1000) {
            detect_ms = timer_read();
            resolve_address();
        }
    } else {
        detect_ms = timer_read();
    }
}
