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

#include <stdint.h>
#include <stdbool.h>
#include <avr/io.h>
#include <util/delay.h>
#include "print.h"
#include "util.h"
#include "debug.h"
#include "ps2.h"
#include "matrix.h"


static void matrix_make(uint8_t code);
static void matrix_break(uint8_t code);


/*
 * Matrix Array usage:
 * 'Scan Code Set 3' is assigned into 17x8 cell matrix.
 *
 *    8bit wide
 *   +---------+
 *  0|         |
 *  :|         | 0x00-0x87
 *  ;|         |
 * 17|         |
 *   +---------+
 */
static uint8_t matrix[MATRIX_ROWS];
#define ROW(code)      (code>>3)
#define COL(code)      (code&0x07)


void matrix_init(void)
{
    debug_enable = true;
    //debug_matrix = true;
    //debug_keyboard = true;
    //debug_mouse = false;

    ps2_host_init();

    // initialize matrix state: all keys off
    for (uint8_t i=0; i < MATRIX_ROWS; i++) matrix[i] = 0x00;

    return;
}

uint8_t matrix_scan(void)
{

    // scan code reading states
    static enum {
        RESET,
        RESET_RESPONSE,
        KBD_ID0,
        KBD_ID1,
        CONFIG,
        READY,
        F0,
    } state = RESET;

    uint8_t code;
    if ((code = ps2_host_recv())) {
        debug("r"); debug_hex(code); debug(" ");
    }

    switch (state) {
        case RESET:
            debug("wFF ");
            if (ps2_host_send(0xFF) == 0xFA) {
                debug("[ack]\nRESET_RESPONSE: ");
                state = RESET_RESPONSE;
            }
            break;
        case RESET_RESPONSE:
            if (code == 0xAA) {
                debug("[ok]\nKBD_ID: ");
                state = KBD_ID0;
            } else if (code) {
                debug("err\nRESET: ");
                state = RESET;
            }
            break;
        // after reset receive keyboad ID(2 bytes)
        case KBD_ID0:
            if (code) {
                state = KBD_ID1;
            }
            break;
        case KBD_ID1:
            if (code) {
                debug("\nCONFIG: ");
                state = CONFIG;
            }
            break;
        case CONFIG:
            debug("wF8 ");
            if (ps2_host_send(0xF8) == 0xFA) {
                debug("[ack]\nREADY\n");
                state = READY;
            }
            break;
        case READY:
            switch (code) {
                case 0x00:
                    break;
                case 0xF0:
                    state = F0;
                    debug(" ");
                    break;
                default:    // normal key make
                    if (code < 0x88) {
                        matrix_make(code);
                    } else {
                        debug("unexpected scan code at READY: "); debug_hex(code); debug("\n");
                    }
                    state = READY;
                    debug("\n");
            }
            break;
        case F0:    // Break code
            switch (code) {
                case 0x00:
                    break;
                default:
                    if (code < 0x88) {
                        matrix_break(code);
                    } else {
                        debug("unexpected scan code at F0: "); debug_hex(code); debug("\n");
                    }
                    state = READY;
                    debug("\n");
            }
            break;
    }
    return 1;
}

inline
uint8_t matrix_get_row(uint8_t row)
{
    return matrix[row];
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
