/*
Copyright 2011 Jun Wako <wakojun@gmail.com>
Copyright 2016 Ethan Apodaca <papodaca@gmail.com>

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
#include "action.h"
#include "print.h"
#include "util.h"
#include "debug.h"
#include "xt.h"
#include "matrix.h"


static void matrix_make(uint8_t code);
static void matrix_break(uint8_t code);

static uint8_t matrix[MATRIX_ROWS];
#define ROW(code)      (code>>3)
#define COL(code)      (code&0x07)

// matrix positions for exceptional keys
#define PRINT_SCREEN   (0x7C)
#define PAUSE          (0x7D)


void matrix_init(void)
{
    debug_enable = true;
    xt_host_init();

    // initialize matrix state: all keys off
    for (uint8_t i=0; i < MATRIX_ROWS; i++) matrix[i] = 0x00;

    return;
}

static uint8_t move_codes(uint8_t code) {
    switch(code) {
        case 0x10:
            code += 0x5E;
            break;
        case 0x19:
            code += 0x41;
            break;
        case 0x1C:
        case 0x1D:
            code += 0x38;
            break;
        case 0x20:
        case 0x21:
        case 0x22:
        case 0x24:
            code += 0x40;
            break;
        case 0x2E:
        case 0x30:
        case 0x32:
            code += 0x44;
            break;
        case 0x35:
        case 0x38:
            code += 0x21;
            break;
        case 0x47:
        case 0x48:
        case 0x49:
        case 0x4B:
        case 0x4D:
        case 0x4F:
        case 0x50:
        case 0x51:
        case 0x52:
        case 0x53:
            code += 0x28;
            break;
    }
    return code;
}

uint8_t matrix_scan(void)
{

    // scan code reading states
    static enum {
        INIT,
        E0,
        E0_2A,
        E0_2A_E0,
        E0_B7,
        E0_B7_E0,

        // print screen
        E1,
        E1_1D,
        E1_1D_45,
        E1_1D_45_E1,
        E1_1D_45_E1_9D,
        // pause
    } state = INIT;


    // 'pseudo break code' hack
    if (matrix_is_on(ROW(PAUSE), COL(PAUSE))) {
        matrix_break(PAUSE);
    }

    uint8_t code = xt_host_recv();
    switch (state) {
        case INIT:
            switch (code) {
                case 0xE0:
                    state = E0;
                    break;
                case 0xE1:
                    state = E1;
                    break;
                default:    // normal key make
                    if (code < 0x80 && code != 0x00) {
                        xprintf("make: %X\r\n", code);
                        matrix_make(code);
                    } else if (code > 0x80 && code < 0xFF && code != 0x00) {
                        xprintf("break %X\r\n", code);
                        matrix_break(code - 0x80);
                    }
                    state = INIT;
            }
            break;
        case E0:    // E0-Prefixed
            switch (code) { //move these codes to unused places on the matrix
                case 0x2A:
                    state = E0_2A;
                    break;
                case 0xB7:
                    state = E0_B7;
                    break;
                default:
                    if (code < 0x80 && code != 0x00) {
                        matrix_make(move_codes(code));
                    } else if (code > 0x80 && code < 0xFF && code != 0x00) {
                        matrix_break(move_codes(code - 0x80));
                    }
                    state = INIT;
            }
            break;
        case E0_2A:
            if(code == 0xE0)
                state = E0_2A_E0;
            else
                state = INIT;
            break;
        case E0_2A_E0:
            if(code == 0x37)
                matrix_make(PRINT_SCREEN);
            else
                state = INIT;
            break;
        case E0_B7:
            if(code == 0xE0)
                state = E0_B7;
            else
                state = INIT;
            break;
        case E0_B7_E0:
          if(code == 0xAA)
              matrix_break(PRINT_SCREEN);
          else
              state = INIT;
          break;
        case E1:
            if (code == 0x1D)
                state = E1_1D;
            else
                state = INIT;
            break;
        case E1_1D:
            if(code == 0x45)
                state = E1_1D_45;
            else
                state = INIT;
            break;
        case E1_1D_45:
            if(code == 0xE1)
                state = E1_1D_45_E1;
            else
                state = INIT;
            break;
        case E1_1D_45_E1:
            if(code == 0x9D)
                state = E1_1D_45_E1_9D;
            else
                state = INIT;
            break;
        case E1_1D_45_E1_9D:
            if(code == 0xC5)
                matrix_make(PAUSE);
            else
                state = INIT;
            break;
        default:
            state = INIT;
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

void matrix_clear(void)
{
    for (uint8_t i=0; i < MATRIX_ROWS; i++) matrix[i] = 0x00;
}
