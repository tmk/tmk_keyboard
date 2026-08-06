/*
 * Matrix for IBM Twinax keyboard(1394312, german version)
 * (C) Copyright 2019
 * Dirk Eibach <dirk.eibach@gmail.com>
 * 
 * Based on Model M support by Les Orchard.
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <stdint.h>
#include <stdbool.h>
#include <avr/io.h>
#include <util/delay.h>
#include "print.h"
#include "debug.h"
#include "util.h"
#include "matrix.h"
#include "shootduino/joystick.h"

#ifndef DEBOUNCE
#   define DEBOUNCE	10
#endif
static uint8_t debouncing = DEBOUNCE;

// bit array of key state(1:on, 0:off)
static matrix_row_t matrix[MATRIX_ROWS];
static matrix_row_t matrix_debouncing[MATRIX_ROWS];

static uint32_t read_rows(void);
static void init_rows(void);
static void unselect_cols(void);
static void select_col(uint8_t col);

uint8_t matrix_rows(void)
{
    return MATRIX_ROWS;
}

uint8_t matrix_cols(void)
{
    return MATRIX_COLS;
}

void matrix_init(void)
{
    unselect_cols();
    init_rows();

    // initialize matrix state: all keys off
    for (uint8_t i = 0; i < MATRIX_ROWS; i++)  {
        matrix[i] = 0;
        matrix_debouncing[i] = 0;
    }

    debug_enable=true;
}

struct JoystickState joystick;

uint8_t matrix_scan(void)
{
    for (uint8_t col = 0; col < MATRIX_COLS; col++) {
        select_col(col);
        _delay_us(30);       // without this wait it won't read stable value.
        uint32_t rows = read_rows();
        for (uint8_t row = 0; row < MATRIX_ROWS; row++) {
            bool prev_bit = matrix_debouncing[row] & ((matrix_row_t)1<<col);
            bool curr_bit = rows & (1UL<<row);
#if 0
            if (curr_bit)
                xprintf("%u%c\n", col, 'A' + row);
#endif
            if ((col == 0) && (row == ('Q'-'A')))
                joystick.up = curr_bit;
            else if ((col == 0) && (row == ('P'-'A')))
                joystick.down = curr_bit;
            else if ((col == 6) && (row == ('A'-'A')))
                joystick.left = curr_bit;
            else if ((col == 1) && (row == ('T'-'A')))
                joystick.right = curr_bit;
            else if ((col == 0) && (row == ('A'-'A')))
                joystick.right_button = curr_bit;
            else if ((col == 7) && (row == ('B'-'A')))
                joystick.left_button = curr_bit;

            if (prev_bit != curr_bit) {
                matrix_debouncing[row] ^= ((matrix_row_t)1<<col);
                if (debouncing) {
                    dprint("bounce!: "); dprintf("%02X", debouncing); dprintln();
                }
                debouncing = DEBOUNCE;
            }
        }
        unselect_cols();
    }

    if (debouncing) {
        if (--debouncing) {
            _delay_ms(1);
        } else {
            for (uint8_t i = 0; i < MATRIX_ROWS; i++) {
                matrix[i] = matrix_debouncing[i];
            }
        }
    }

    return 1;
}

bool matrix_is_modified(void)
{
    if (debouncing) return false;
    return true;
}

bool matrix_is_on(uint8_t row, uint8_t col)
{
    return (matrix[row] & ((matrix_row_t)1<<col));
}

matrix_row_t matrix_get_row(uint8_t row)
{
    return matrix[row];
}

void matrix_print(void)
{
    print("\nr/c 01234567\n");
    for (uint8_t row = 0; row < matrix_rows(); row++) {
        xprintf(" %c: ", 'A'+row);
        pbin_reverse(matrix_get_row(row));
#ifdef MATRIX_HAS_GHOST
        if (matrix_has_ghost_in_row(row)) {
            print(" <ghost");
        }
#endif
        print("\n");
    }
}

uint8_t matrix_key_count(void)
{
    uint8_t count = 0;
    for (uint8_t i = 0; i < MATRIX_ROWS; i++) {
        count += bitpop32(matrix[i]);
    }
    return count;
}

/* Row pin configuration
 * row: 0  1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16 17 18 19
 * pin: B7 D0 D1 D2 D3 D4 D5 E7 D7 E0 E1 C0 C1 C2 C3 C4 C5 C6 C7 B6
 */
static void init_rows(void)
{
    // Input with pull-up(DDR:0, PORT:1)
    DDRB  &= ~0b10000011;
    PORTB |=  0b10000011;
    DDRC  &= ~0b11111111;
    PORTC |=  0b11111111;
    DDRD  &= ~0b10111100;
    PORTD |=  0b10111100;
    DDRE  &= ~0b11000011;
    PORTE |=  0b11000011;
}

static uint32_t read_rows(void)
{
    return (PINB&(1<<7) ? 0 : (1UL<<0)) |
           (PINB&(1<<0) ? 0 : (1UL<<1)) |
           (PINB&(1<<1) ? 0 : (1UL<<2)) |
           (PIND&(1<<2) ? 0 : (1UL<<3)) |
           (PIND&(1<<3) ? 0 : (1UL<<4)) |
           (PIND&(1<<4) ? 0 : (1UL<<5)) |
           (PIND&(1<<5) ? 0 : (1UL<<6)) |
           (PINE&(1<<7) ? 0 : (1UL<<7)) |
           (PIND&(1<<7) ? 0 : (1UL<<8)) |
           (PINE&(1<<0) ? 0 : (1UL<<9)) |
           (PINE&(1<<1) ? 0 : (1UL<<10)) |
           (PINC&(1<<0) ? 0 : (1UL<<11)) |
           (PINC&(1<<1) ? 0 : (1UL<<12)) |
           (PINC&(1<<2) ? 0 : (1UL<<13)) |
           (PINC&(1<<3) ? 0 : (1UL<<14)) |
           (PINC&(1<<4) ? 0 : (1UL<<15)) |
           (PINC&(1<<5) ? 0 : (1UL<<16)) |
           (PINC&(1<<6) ? 0 : (1UL<<17)) |
           (PINC&(1<<7) ? 0 : (1UL<<18)) |
           (PINE&(1<<6) ? 0 : (1UL<<19));
}

/* Column pin configuration
 * col:  0  1  2  3  4  5  6  7
 * pin: F0 F1 F2 F3 F4 F5 F6 F7
 */
static void unselect_cols(void)
{
    // Hi-Z(DDR:0, PORT:0) to unselect
    DDRF  &= ~0b11111111;
    PORTF |=  0b11111111;
}

static void select_col(uint8_t col)
{
    DDRF  |=  (1 << col);
    PORTF &= ~(1 << col);
}
