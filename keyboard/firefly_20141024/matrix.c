/*
Copyright 2014 Ralf Schmitt <ralf@bunkertor.net>

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
#include "debug.h"
#include "util.h"
#include "matrix.h"

#ifndef DEBOUNCE
#   define DEBOUNCE 0
#endif
static uint8_t debouncing = DEBOUNCE;

static matrix_row_t matrix[MATRIX_ROWS];
static matrix_row_t matrix_debouncing[MATRIX_ROWS];

static uint8_t read_rows(void);
static void init_rows(void);
static void unselect_cols(void);
static void select_col(uint8_t col);

inline uint8_t matrix_rows(void)
{
    return MATRIX_ROWS;
}

inline uint8_t matrix_cols(void)
{
    return MATRIX_COLS;
}

void matrix_init(void)
{
    unselect_cols();
    init_rows();
    for (uint8_t i=0; i < MATRIX_ROWS; i++)  {
        matrix[i] = 0;
        matrix_debouncing[i] = 0;
    }
}

uint8_t matrix_scan(void)
{
    for (uint8_t col = 0; col < MATRIX_COLS; col++) {
        select_col(col);
        _delay_us(3);
        uint8_t rows = read_rows();
        for (uint8_t row = 0; row < MATRIX_ROWS; row++) {
            bool prev_bit = matrix_debouncing[row] & ((matrix_row_t)1<<col);
            bool curr_bit = rows & (1<<row);
            if (prev_bit != curr_bit) {
                matrix_debouncing[row] ^= ((matrix_row_t)1<<col);
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

inline bool matrix_is_on(uint8_t row, uint8_t col)
{
    return (matrix[row] & ((matrix_row_t)1<<col));
}

inline matrix_row_t matrix_get_row(uint8_t row)
{
    return matrix[row];
}

void matrix_print(void)
{
    print("\nr/c 0123456789ABCDEF\n");
    for (uint8_t row = 0; row < MATRIX_ROWS; row++) {
        xprintf("%02X: %032lb\n", row, bitrev32(matrix_get_row(row)));
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
 *
 * row: 0    1    2    3    4    5    6
 * pin: PC4  PB3  PB2  PB1  PD3  PD2  PC2
 *
 */
static void init_rows(void)
{
    DDRB &= ~0b00001110;
    DDRC &= ~0b00010100;
    DDRD &= ~0b00001100;
    PORTB |= 0b00001110;
    PORTC |= 0b00010100;
    PORTD |= 0b00001110;
}

static uint8_t read_rows(void)
{
    return (PINC&(1<<4) ? 0 : (1<<0)) |
           (PINB&(1<<3) ? 0 : (1<<1)) |
           (PINB&(1<<2) ? 0 : (1<<2)) |
           (PINB&(1<<1) ? 0 : (1<<3)) |
           (PIND&(1<<3) ? 0 : (1<<4)) |
           (PIND&(1<<2) ? 0 : (1<<5)) |
           (PINC&(1<<2) ? 0 : (1<<6));
}

/*  The are no columns */
static void unselect_cols(void) {}

static void select_col(uint8_t col) {}
