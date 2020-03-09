/*
Copyright 2012 Jun Wako <wakojun@gmail.com>

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
#include "debug.h"
#include "util.h"
#include "matrix.h"


#ifndef DEBOUNCE
#   define DEBOUNCE	5
#endif
static uint8_t debouncing = DEBOUNCE;

/* matrix state(1:on, 0:off) */
static matrix_row_t matrix[MATRIX_ROWS];
static matrix_row_t matrix_debouncing[MATRIX_ROWS];

static matrix_row_t read_cols(void);
static void init_cols(void);
static void unselect_rows(void);
static void select_row(uint8_t row);

void matrix_init(void)
{
    // initialize row and col
    unselect_rows();
    init_cols();

    // initialize matrix state: all keys off
    for (uint8_t i=0; i < MATRIX_ROWS; i++) {
        matrix[i] = 0;
        matrix_debouncing[i] = 0;
    }
}

uint8_t matrix_scan(void)
{
    for (uint8_t i = 0; i < MATRIX_ROWS; i++) {
        select_row(i);
        _delay_us(30);  // without this wait read unstable value.
        matrix_row_t cols = read_cols();
        if (matrix_debouncing[i] != cols) {
            matrix_debouncing[i] = cols;
            if (debouncing) {
                debug("bounce!: "); debug_hex(debouncing); debug("\n");
            }
            debouncing = DEBOUNCE;
        }
        unselect_rows();
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

inline
matrix_row_t matrix_get_row(uint8_t row)
{
    return matrix[row];
}

/* Column pin configuration
 * col: 0   1   2   3   4   5   6   7   8   9   10  11  12  13
 * pin: F0  F1  E6  C7  C6  B6  D4  B1  B0  B5  B4  D7  D6  B3  (Rev.A)
 * pin:                                 B7                      (Rev.B)
 */
static void  init_cols(void)
{
    // Input with pull-up(DDR:0, PORT:1)
    DDRD  &= ~(1<<0 | 1<<1 | 1<<2 | 1<<3 | 1<<4 | 1<<5);
    PORTD |=  (1<<0 | 1<<1 | 1<<2 | 1<<3 | 1<<4 | 1<<5);
    DDRB  &= ~(1<<0 | 1<<1 | 1<<2 | 1<<3 | 1<<4 | 1<<5 | 1<<6);
    PORTB |=  (1<<0 | 1<<1 | 1<<2 | 1<<3 | 1<<4 | 1<<5 | 1<<6);
}

static matrix_row_t read_cols(void)
{
    return (PIND&(1<<5) ? 0 : (1<<0))  |
           (PIND&(1<<4) ? 0 : (1<<1))  |
           (PIND&(1<<3) ? 0 : (1<<2))  |
           (PIND&(1<<2) ? 0 : (1<<3))  |
           (PIND&(1<<1) ? 0 : (1<<4))  |
           (PIND&(1<<0) ? 0 : (1<<5))  |
           (PINB&(1<<6) ? 0 : (1<<6))  |
           (PINB&(1<<5) ? 0 : (1<<7))  |
           (PINB&(1<<4) ? 0 : (1<<8))  |
           (PINB&(1<<3) ? 0 : (1<<9))  |
           (PINB&(1<<2) ? 0 : (1<<10)) |
           (PINB&(1<<1) ? 0 : (1<<11)) |
           (PINB&(1<<0) ? 0 : (1<<12));
}

/* Row pin configuration
 * row: 0   1   2   3   4
 * pin: D0  D1  D2  D3  D5
 */
static void unselect_rows(void)
{
    // Hi-Z(DDR:0, PORT:0) to unselect
    DDRC  &= ~0b01111111;
    PORTC &= ~0b01111111;
    DDRF  &= ~0b01111111;
    PORTF &= ~0b01111111;
}

static void select_row(uint8_t row)
{
    // Output low(DDR:1, PORT:0) to select
    switch (row) {
        case 0:
            DDRC  |= (1<<0);
            PORTC &= ~(1<<0);
            break;
        case 1:
            DDRC  |= (1<<1);
            PORTC &= ~(1<<1);
            break;
        case 2:
            DDRC  |= (1<<2);
            PORTC &= ~(1<<2);
            break;
        case 3:
            DDRC  |= (1<<3);
            PORTC &= ~(1<<3);
            break;
        case 4:
            DDRC  |= (1<<4);
            PORTC &= ~(1<<4);
            break;
        case 5:
            DDRC  |= (1<<6);
            PORTC &= ~(1<<6);
            break;
        case 6:
            DDRC  |= (1<<5);
            PORTC &= ~(1<<5);
            break;
        case 7:
            DDRF  |= (1<<0);
            PORTF &= ~(1<<0);
            break;
        case 8:
            DDRF  |= (1<<1);
            PORTF &= ~(1<<1);
            break;
        case 9:
            DDRF  |= (1<<2);
            PORTF &= ~(1<<2);
            break;
        case 10:
            DDRF  |= (1<<3);
            PORTF &= ~(1<<3);
            break;
        case 11:
            DDRF  |= (1<<4);
            PORTF &= ~(1<<4);
            break;
        // mind that 6 and 5 are swapped because there was a soldering error 
        case 12:
            DDRF  |= (1<<6);
            PORTF &= ~(1<<6);
            break;
        case 13:
            DDRF  |= (1<<5);
            PORTF &= ~(1<<5);
            break;
    }
}
