/*
Copyright 2019 Jun Wako <wakojun@gmail.com>

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
#include "print.h"
#include "util.h"
#include "debug.h"
#include "ibmpc.h"
#include "host.h"
#include "led.h"
#include "matrix.h"
#include "timer.h"
#include "action.h"
#include "ibmpc_usb.h"


static void matrix_make(uint8_t code);
static void matrix_break(uint8_t code);

static int8_t process_cs1(void);
static int8_t process_cs2(void);
static int8_t process_cs3(void);


static uint8_t matrix[MATRIX_ROWS];
#define ROW(code)      ((code>>3)&0x0F)
#define COL(code)      (code&0x07)

static int16_t read_wait(uint16_t wait_ms)
{
    uint16_t start = timer_read();
    int16_t code;
    while ((code = ibmpc_host_recv()) == -1 && timer_elapsed(start) < wait_ms);
    return code;
}

static uint16_t read_keyboard_id(void)
{
    uint16_t id = 0;
    int16_t  code = 0;

    // Disable
    code = ibmpc_host_send(0xF5);

    // Read ID
    code = ibmpc_host_send(0xF2);
    if (code == -1)  return 0xFFFF;     // XT or No keyboard
    if (code != 0xFA) return 0xFFFE;    // Broken PS/2?

    code = read_wait(1000);
    if (code == -1)  return 0x0000;     // AT
    id = (code & 0xFF)<<8;

    code = read_wait(1000);
    id |= code & 0xFF;

    // Enable
    code = ibmpc_host_send(0xF4);

    return id;
}

void matrix_init(void)
{
    debug_enable = true;
    ibmpc_host_init();

    // hard reset for XT keyboard
    IBMPC_RESET();

    // initialize matrix state: all keys off
    for (uint8_t i=0; i < MATRIX_ROWS; i++) matrix[i] = 0x00;

    return;
}

/*
 * keyboard recognition
 *
 * 1. Send F2 to get keyboard ID
 *      a. no ACK(FA): XT keyobard
 *      b. ACK but no ID: 84-key AT keyboard CodeSet2
 *      c. ID is AB 83: PS/2 keyboard CodeSet2
 *      d. ID is BF BF: Terminal keyboard CodeSet3
 *      e. error on recv: maybe broken PS/2
 */
uint16_t keyboard_id = 0x0000;
keyboard_kind_t keyboard_kind = NONE;
uint8_t matrix_scan(void)
{
    // scan code reading states
    static enum {
        INIT,
        WAIT_STARTUP,
        READ_ID,
        LED_SET,
        LOOP,
        END
    } state = INIT;
    static uint16_t last_time;


    if (ibmpc_error) {
        xprintf("err: %02X\n", ibmpc_error);

        // when recv error, neither send error nor buffer full
        if (!(ibmpc_error & (IBMPC_ERR_SEND | IBMPC_ERR_FULL))) {
            // keyboard init again
            if (state == LOOP) {
                xprintf("init\n");
                state = INIT;
            }
        }

        // clear or process error
        ibmpc_error = IBMPC_ERR_NONE;
    }

    switch (state) {
        case INIT:
            ibmpc_protocol = IBMPC_PROTOCOL_AT;
            keyboard_kind = NONE;
            keyboard_id = 0x0000;
            last_time = timer_read();
            state = WAIT_STARTUP;
            matrix_clear();
            clear_keyboard();
            break;
        case WAIT_STARTUP:
            // read and ignore BAT code and other codes when power-up
            ibmpc_host_recv();
            if (timer_elapsed(last_time) > 1000) {
                state = READ_ID;
            }
            break;
        case READ_ID:
            keyboard_id = read_keyboard_id();
            if (ibmpc_error) {
                xprintf("err: %02X\n", ibmpc_error);
                ibmpc_error = IBMPC_ERR_NONE;
            }
            xprintf("ID: %04X\n", keyboard_id);
            if (0xAB00 == (keyboard_id & 0xFF00)) {
                // CodeSet2 PS/2
                keyboard_kind = PC_AT;
            } else if (0xBF00 == (keyboard_id & 0xFF00)) {
                // CodeSet3 Terminal
                keyboard_kind = PC_TERMINAL;
            } else if (0x0000 == keyboard_id) {
                // CodeSet2 AT
                keyboard_kind = PC_AT;
            } else if (0xFFFF == keyboard_id) {
                // CodeSet1 XT
                keyboard_kind = PC_XT;
            } else if (0xFFFE == keyboard_id) {
                // CodeSet2 PS/2 fails to response?
                keyboard_kind = PC_AT;
            } else if (0x00FF == keyboard_id) {
                // Mouse is not supported
                xprintf("Mouse: not supported\n");
                keyboard_kind = NONE;
            } else {
                keyboard_kind = PC_AT;
            }

            // protocol
            if (keyboard_kind == PC_XT) {
                xprintf("kbd: XT\n");
                ibmpc_protocol = IBMPC_PROTOCOL_XT;
            } else if (keyboard_kind == PC_AT) {
                xprintf("kbd: AT\n");
                ibmpc_protocol = IBMPC_PROTOCOL_AT;
            } else if (keyboard_kind == PC_TERMINAL) {
                xprintf("kbd: Terminal\n");
                ibmpc_protocol = IBMPC_PROTOCOL_AT;
                // Set all keys - make/break [3]p.23
                ibmpc_host_send(0xF8);
            } else {
                xprintf("kbd: Unknown\n");
                ibmpc_protocol = IBMPC_PROTOCOL_AT;
            }
            state = LED_SET;
            break;
        case LED_SET:
            led_set(host_keyboard_leds());
            state = LOOP;
        case LOOP:
            switch (keyboard_kind) {
                case PC_XT:
                    process_cs1();
                    break;
                case PC_AT:
                    process_cs2();
                    break;
                case PC_TERMINAL:
                    process_cs3();
                    break;
                default:
                    break;
            }
            break;
        default:
            break;
    }
    return 1;
}

inline
bool matrix_is_on(uint8_t row, uint8_t col)
{
    return (matrix[row] & (1<<col));
}

inline
uint8_t matrix_get_row(uint8_t row)
{
    return matrix[row];
}

uint8_t matrix_key_count(void)
{
    uint8_t count = 0;
    for (uint8_t i = 0; i < MATRIX_ROWS; i++) {
        count += bitpop(matrix[i]);
    }
    return count;
}


inline
static void matrix_make(uint8_t code)
{
    if (!matrix_is_on(ROW(code), COL(code))) {
        matrix[ROW(code)] |= 1<<COL(code);
    }
}

inline
static void matrix_break(uint8_t code)
{
    if (matrix_is_on(ROW(code), COL(code))) {
        matrix[ROW(code)] &= ~(1<<COL(code));
    }
}

void matrix_clear(void)
{
    for (uint8_t i=0; i < MATRIX_ROWS; i++) matrix[i] = 0x00;
}

void led_set(uint8_t usb_led)
{
    if (keyboard_kind != PC_AT) return;

    uint8_t ibmpc_led = 0;
    if (usb_led &  (1<<USB_LED_SCROLL_LOCK))
        ibmpc_led |= (1<<IBMPC_LED_SCROLL_LOCK);
    if (usb_led &  (1<<USB_LED_NUM_LOCK))
        ibmpc_led |= (1<<IBMPC_LED_NUM_LOCK);
    if (usb_led &  (1<<USB_LED_CAPS_LOCK))
        ibmpc_led |= (1<<IBMPC_LED_CAPS_LOCK);
    ibmpc_host_set_led(ibmpc_led);
}


/*******************************************************************************
 * XT: Scan Code Set 1
 *
 * See [3], [a]
 *
 * E0-escaped scan codes are translated into unused range of the matrix.(54-7F)
 *
 *     01-53: Normal codes used in original XT keyboard
 *     54-7F: Not used in original XT keyboard
 *
 *         0   1   2   3   4   5   6   7   8   9   A   B   C   D   E   F
 *     50  -   -   -   -   *   *   x   x   x   x   *   *   *   *   *   *
 *     60  *   *   *   *   x   x   x   x   x   x   x   x   x   x   x   *
 *     70  x   *   *   x   *   *   x   *   *   x   *   x   *   x   x   *
 *
 * -: codes existed in original XT keyboard
 * *: E0-escaped codes translated
 * x: Non-espcaped codes(Some are not used in real keyboards probably)
 *
 * Codes assigned in range 54-7F:
 *
 *     50  -                60  Up*                 70  KANAx
 *     51  -                61  Left*               71  Insert*
 *     52  -                62  Down*               72  Delete*
 *     53  -                63  Right*              73  ROx
 *     54  PrintScr*        64  F13x                74  Home*
 *     55  Pause*           65  F14x                75  End*
 *     56  Euro2x           66  F15x                76  F24x
 *     57  F11x             67  F16x                77  PageUp*
 *     58  F12x             68  F17x                78  PageDown*
 *     59  Keypad=x         69  F18x                79  HENKANx
 *     5A  LGUI*            6A  F19x                7A  RCTL*
 *     5B  RGUI*            6B  F20x                7B  MUHENKANx
 *     5C  APP*             6C  F21x                7C  RALT*
 *     5D  Mute*            6D  F22x                7D  JPYx
 *     5E  Volume Down*     6E  F23x                7E  Keypad,x
 *     5F  Volume Up*       6F  Keypad Enter*       7F  Keypad/ *
 */
static uint8_t cs1_e0code(uint8_t code) {
    switch(code) {
        // Original IBM XT keyboard doesn't use E0-codes probably
        // Some XT compatilble keyobards need these keys?
        case 0x37: return 0x54; // Print Screen
        case 0x46: return 0x55; // Ctrl + Pause
        case 0x5B: return 0x5A; // Left  GUI
        case 0x5C: return 0x5B; // Right GUI
        case 0x5D: return 0x5C; // Application
        case 0x20: return 0x5D; // Mute
        case 0x2E: return 0x5E; // Volume Down
        case 0x30: return 0x5F; // Volume Up
        case 0x48: return 0x60; // Up
        case 0x4B: return 0x61; // Left
        case 0x50: return 0x62; // Down
        case 0x4D: return 0x63; // Right
        case 0x1C: return 0x6F; // Keypad Enter
        case 0x52: return 0x71; // Insert
        case 0x53: return 0x72; // Delete
        case 0x47: return 0x74; // Home
        case 0x4F: return 0x75; // End
        case 0x49: return 0x77; // Page Up
        case 0x51: return 0x78; // Page Down
        case 0x1D: return 0x7A; // Right Ctrl
        case 0x38: return 0x7C; // Right Alt
        case 0x35: return 0x7F; // Keypad /

        // Shared matrix cell with other keys
        case 0x5E: return 0x70; // Power (KANA)
        case 0x5F: return 0x79; // Sleep (HENKAN)
        case 0x63: return 0x7B; // Wake  (MUHENKAN)

        default:
           xprintf("!CS1_?!\n");
           return code;
    }
    return 0x00;
}

static int8_t process_cs1(void)
{
    static enum {
        INIT,
        E0,
        // Pause: E1 1D 45, E1 9D C5
        E1,
        E1_1D,
        E1_9D,
    } state = INIT;

    uint16_t code = ibmpc_host_recv();
    if (code == -1) {
        return 0;
    }

    switch (state) {
        case INIT:
            switch (code) {
                case 0xE0:
                    state = E0;
                    break;
                case 0xE1:
                    state = E1;
                    break;
                default:
                    if (code < 0x80)
                        matrix_make(code);
                    else
                        matrix_break(code & 0x7F);
                    break;
            }
            break;
        case E0:
            switch (code) {
                case 0x2A:
                case 0xAA:
                case 0x36:
                case 0xB6:
                    //ignore fake shift
                    state = INIT;
                    break;
                default:
                    if (code < 0x80)
                        matrix_make(cs1_e0code(code));
                    else
                        matrix_break(cs1_e0code(code & 0x7F));
                    state = INIT;
                    break;
            }
            break;
        case E1:
            switch (code) {
                case 0x1D:
                    state = E1_1D;
                    break;
                case 0x9D:
                    state = E1_9D;
                    break;
                default:
                    state = INIT;
                    break;
            }
            break;
        case E1_1D:
            switch (code) {
                case 0x45:
                    matrix_make(0x55);
                    break;
                default:
                    state = INIT;
                    break;
            }
            break;
        case E1_9D:
            switch (code) {
                case 0x45:
                    matrix_break(0x55);
                    break;
                default:
                    state = INIT;
                    break;
            }
            break;
        default:
            state = INIT;
    }
    return 0;
}


/*******************************************************************************
 * AT, PS/2: Scan Code Set 2
 *
 * Exceptional Handling
 * --------------------
 * Some keys should be handled exceptionally. See [b].
 *
 * Scan codes are varied or prefix/postfix'd depending on modifier key state.
 *
 * 1) Insert, Delete, Home, End, PageUp, PageDown, Up, Down, Right, Left
 *     a) when Num Lock is off
 *     modifiers | make                      | break
 *     ----------+---------------------------+----------------------
 *     Ohter     |                    <make> | <break>
 *     LShift    | E0 F0 12           <make> | <break>  E0 12
 *     RShift    | E0 F0 59           <make> | <break>  E0 59
 *     L+RShift  | E0 F0 12  E0 F0 59 <make> | <break>  E0 59 E0 12
 *
 *     b) when Num Lock is on
 *     modifiers | make                      | break
 *     ----------+---------------------------+----------------------
 *     Other     | E0 12              <make> | <break>  E0 F0 12
 *     Shift'd   |                    <make> | <break>
 *
 *     Handling: These prefix/postfix codes are ignored.
 *
 *
 * 2) Keypad /
 *     modifiers | make                      | break
 *     ----------+---------------------------+----------------------
 *     Ohter     |                    <make> | <break>
 *     LShift    | E0 F0 12           <make> | <break>  E0 12
 *     RShift    | E0 F0 59           <make> | <break>  E0 59
 *     L+RShift  | E0 F0 12  E0 F0 59 <make> | <break>  E0 59 E0 12
 *
 *     Handling: These prefix/postfix codes are ignored.
 *
 *
 * 3) PrintScreen
 *     modifiers | make         | break
 *     ----------+--------------+-----------------------------------
 *     Other     | E0 12  E0 7C | E0 F0 7C  E0 F0 12
 *     Shift'd   |        E0 7C | E0 F0 7C
 *     Control'd |        E0 7C | E0 F0 7C
 *     Alt'd     |           84 | F0 84
 *
 *     Handling: These prefix/postfix codes are ignored, and both scan codes
 *               'E0 7C' and 84 are seen as PrintScreen.
 *
 * 4) Pause
 *     modifiers | make(no break code)
 *     ----------+--------------------------------------------------
 *     Other     | E1 14 77 E1 F0 14 F0 77
 *     Control'd | E0 7E E0 F0 7E
 *
 *     Handling: Both code sequences are treated as a whole.
 *               And we need a ad hoc 'pseudo break code' hack to get the key off
 *               because it has no break code.
 *
 * Notes:
 * 'Hanguel/English'(F1) and 'Hanja'(F2) have no break code. See [a].
 * These two Korean keys need exceptional handling and are not supported for now.
 *
 */
static uint8_t cs2_e0code(uint8_t code) {
    switch(code) {
        // E0 prefixed codes translation See [a].
        case 0x11: return 0x0F; // right alt
        case 0x14: return 0x17; // right control
        case 0x1F: return 0x19; // left GUI
        case 0x27: return 0x1F; // right GUI
        case 0x2F: return 0x5C; // apps
        case 0x4A: return 0x60; // keypad /
        case 0x5A: return 0x62; // keypad enter
        case 0x69: return 0x27; // end
        case 0x6B: return 0x53; // cursor left
        case 0x6C: return 0x2F; // home
        case 0x70: return 0x39; // insert
        case 0x71: return 0x37; // delete
        case 0x72: return 0x3F; // cursor down
        case 0x74: return 0x47; // cursor right
        case 0x75: return 0x4F; // cursor up
        case 0x7A: return 0x56; // page down
        case 0x7D: return 0x5E; // page up
        case 0x7C: return 0x6F; // Print Screen
        case 0x7E: return 0x00; // Control'd Pause

        case 0x21: return 0x65; // volume down
        case 0x32: return 0x6E; // volume up
        case 0x23: return 0x7F; // mute
        case 0x10: return 0x08; // (WWW search)     -> F13
        case 0x18: return 0x10; // (WWW favourites) -> F14
        case 0x20: return 0x18; // (WWW refresh)    -> F15
        case 0x28: return 0x20; // (WWW stop)       -> F16
        case 0x30: return 0x28; // (WWW forward)    -> F17
        case 0x38: return 0x30; // (WWW back)       -> F18
        case 0x3A: return 0x38; // (WWW home)       -> F19
        case 0x40: return 0x40; // (my computer)    -> F20
        case 0x48: return 0x48; // (email)          -> F21
        case 0x2B: return 0x50; // (calculator)     -> F22
        case 0x34: return 0x08; // (play/pause)     -> F13
        case 0x3B: return 0x10; // (stop)           -> F14
        case 0x15: return 0x18; // (previous track) -> F15
        case 0x4D: return 0x20; // (next track)     -> F16
        case 0x50: return 0x28; // (media select)   -> F17
        case 0x5E: return 0x50; // (ACPI wake)      -> F22
        case 0x3F: return 0x57; // (ACPI sleep)     -> F23
        case 0x37: return 0x5F; // (ACPI power)     -> F24

        // https://github.com/tmk/tmk_keyboard/pull/636
        case 0x03: return 0x18; // Help        DEC LK411 -> F15
        case 0x04: return 0x08; // F13         DEC LK411
        case 0x0B: return 0x20; // Do          DEC LK411 -> F16
        case 0x0C: return 0x10; // F14         DEC LK411
        case 0x0D: return 0x19; // LCompose    DEC LK411 -> LGUI
        case 0x79: return 0x6D; // KP-         DEC LK411 -> PCMM
        case 0x83: return 0x28; // F17         DEC LK411
        default: return (code & 0x7F);
    }
}

static int8_t process_cs2(void)
{
    // scan code reading states
    static enum {
        INIT,
        F0,
        E0,
        E0_F0,
        // Pause
        E1,
        E1_14,
        E1_F0,
        E1_F0_14,
        E1_F0_14_F0,
    } state = INIT;

    uint16_t code = ibmpc_host_recv();
    if (code == -1) {
        return 0;
    }

    switch (state) {
        case INIT:
            switch (code) {
                case 0xE0:
                    state = E0;
                    break;
                case 0xF0:
                    state = F0;
                    break;
                case 0xE1:
                    state = E1;
                    break;
                case 0x83:  // F7
                    matrix_make(0x02);
                    state = INIT;
                    break;
                case 0x84:  // Alt'd PrintScreen
                    matrix_make(0x6F);
                    state = INIT;
                    break;
                case 0x00:  // Overrun [3]p.26
                    matrix_clear();
                    xprintf("!CS2_OVERRUN!\n");
                    state = INIT;
                    break;
                case 0xAA:  // Self-test passed
                case 0xFC:  // Self-test failed
                    // reset or plugin-in new keyboard
                    state = INIT;
                    return -1;
                    break;
                default:    // normal key make
                    if (code < 0x80) {
                        matrix_make(code);
                    } else {
                        matrix_clear();
                        xprintf("!CS2_INIT!\n");
                    }
                    state = INIT;
            }
            break;
        case E0:    // E0-Prefixed
            switch (code) {
                case 0x12:  // to be ignored
                case 0x59:  // to be ignored
                    state = INIT;
                    break;
                case 0xF0:
                    state = E0_F0;
                    break;
                default:
                    if (code < 0x80) {
                        matrix_make(cs2_e0code(code));
                    } else {
                        matrix_clear();
                        xprintf("!CS2_E0!\n");
                    }
                    state = INIT;
            }
            break;
        case F0:    // Break code
            switch (code) {
                case 0x83:  // F7
                    matrix_break(0x02);
                    state = INIT;
                    break;
                case 0x84:  // Alt'd PrintScreen
                    matrix_break(0x6F);
                    state = INIT;
                    break;
                default:
                    if (code < 0x80) {
                        matrix_break(code);
                    } else {
                        matrix_clear();
                        xprintf("!CS2_F0!\n");
                    }
                    state = INIT;
            }
            break;
        case E0_F0: // Break code of E0-prefixed
            switch (code) {
                case 0x12:  // to be ignored
                case 0x59:  // to be ignored
                    state = INIT;
                    break;
                default:
                    if (code < 0x80) {
                        matrix_break(cs2_e0code(code));
                    } else {
                        matrix_clear();
                        xprintf("!CS2_E0_F0!\n");
                    }
                    state = INIT;
            }
            break;
        // Pause make: E1 14 77
        case E1:
            switch (code) {
                case 0x14:
                    state = E1_14;
                    break;
                case 0xF0:
                    state = E1_F0;
                    break;
                default:
                    state = INIT;
            }
            break;
        case E1_14:
            switch (code) {
                case 0x77:
                    matrix_make(0x00);
                    state = INIT;
                    break;
                default:
                    state = INIT;
            }
            break;
        // Pause break: E1 F0 14 F0 77
        case E1_F0:
            switch (code) {
                case 0x14:
                    state = E1_F0_14;
                    break;
                default:
                    state = INIT;
            }
            break;
        case E1_F0_14:
            switch (code) {
                case 0xF0:
                    state = E1_F0_14_F0;
                    break;
                default:
                    state = INIT;
            }
            break;
        case E1_F0_14_F0:
            switch (code) {
                case 0x77:
                    matrix_break(0x00);
                    state = INIT;
                    break;
                default:
                    state = INIT;
            }
            break;
        default:
            state = INIT;
    }
    return 0;
}

/*
 * Terminal: Scan Code Set 3
 *
 * See [3], [7]
 *
 * Scan code 0x83 and 0x84 are handled exceptioanally to fit into 1-byte range index.
 */
static int8_t process_cs3(void)
{
    static enum {
        READY,
        F0,
    } state = READY;

    uint16_t code = ibmpc_host_recv();
    if (code == -1) {
        return 0;
    }

    switch (state) {
        case READY:
            switch (code) {
                case 0x00:
                case 0xff:
                    xprintf("!CS3_%02X!\n", code);
                    break;
                case 0xF0:
                    state = F0;
                    break;
                case 0x83:  // F7
                    matrix_make(0x02);
                    break;
                case 0x84:  // keypad -
                    matrix_make(0x7F);
                    break;
                default:    // normal key make
                    if (code < 0x80) {
                        matrix_make(code);
                    } else {
                        xprintf("!CS3_%02X!\n", code);
                    }
                    state = READY;
            }
            break;
        case F0:    // Break code
            switch (code) {
                case 0x00:
                case 0xff:
                    xprintf("!CS3_F0_%02X!\n", code);
                    state = READY;
                    break;
                case 0x83:  // F7
                    matrix_break(0x02);
                    state = READY;
                    break;
                case 0x84:  // keypad -
                    matrix_break(0x7F);
                    state = READY;
                    break;
                default:
                    if (code < 0x80) {
                        matrix_break(code);
                    } else {
                        xprintf("!CS3_F0_%02X!\n", code);
                    }
                    state = READY;
            }
            break;
    }
    return 0;
}

/*
 * IBM PC Keyboard Protocol Resources:
 *
 * [a] Microsoft USB HID to PS/2 Translation Table - Scan Code Set 1 and 2
 * http://download.microsoft.com/download/1/6/1/161ba512-40e2-4cc9-843a-923143f3456c/translate.pdf
 *
 * [b] Microsoft Keyboard Scan Code Specification - Special rules of Scan Code Set 1 and 2
 * http://download.microsoft.com/download/1/6/1/161ba512-40e2-4cc9-843a-923143f3456c/scancode.doc
 *
 * [1] PS/2 Reference Manuals - Collection of IBM Personal System/2 documents.
 * http://www.mcamafia.de/pdf/pdfref.htm
 *
 * [2] Keyboard and Auxiliary Device Controller - Signal Timing and Format
 * http://www.mcamafia.de/pdf/ibm_hitrc07.pdf
 *
 * [3] Keyboards(101- and 102-key) - Keyboard Layout, Scan Code Set, POR, and Commands.
 * http://www.mcamafia.de/pdf/ibm_hitrc11.pdf
 *
 * [4] IBM PC XT Keyboard Protocol
 * https://github.com/tmk/tmk_keyboard/wiki/IBM-PC-XT-Keyboard-Protocol
 *
 * [5] IBM Keyboard Scan Code by John Elliott - 83-key, 84-key, 102-key and 122-key
 * https://www.seasip.info/VintagePC/index.html
 *
 * [6] IBM 1391406 Keyboard - Scan Code Set 2 of 102-key PS/2 keyboard
 * https://www.seasip.info/VintagePC/ibm_1391406.html
 *
 * [7] The IBM 6110344 Keyboard - Scan Code Set 3 of 122-key terminal keyboard
 * https://www.seasip.info/VintagePC/ibm_6110344.html
 *
 * [y] TrackPoint Engineering Specifications for version 3E
 * https://web.archive.org/web/20100526161812/http://wwwcssrv.almaden.ibm.com/trackpoint/download.html
 *
 * [z] [Soarer's XT/AT/PS2/Terminal to USB converter]
 * https://geekhack.org/index.php?topic=17458.0
 *
 */
