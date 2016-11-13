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


inline
uint8_t matrix_rows(void)
{
    return MATRIX_ROWS;
}

inline
uint8_t matrix_cols(void)
{
    return MATRIX_COLS;
}

void matrix_init(void)
{
    // To use PORTF disable JTAG with writing JTD bit twice within four cycles.
    MCUCR |= (1<<JTD);
    MCUCR |= (1<<JTD);

    backlight_init_ports();

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

bool matrix_is_modified(void)
{
    if (debouncing) return false;
    return true;
}

inline
bool matrix_is_on(uint8_t row, uint8_t col)
{
    return (matrix[row] & ((matrix_row_t)1<<col));
}

inline
matrix_row_t matrix_get_row(uint8_t row)
{
    return matrix[row];
}

void matrix_print(void)
{
    print("\nr/c 0123456789ABCDEF\n");
    for (uint8_t row = 0; row < MATRIX_ROWS; row++) {
        phex(row); print(": ");
        pbin_reverse16(matrix_get_row(row));
        print("\n");
    }
}

uint8_t matrix_key_count(void)
{
    uint8_t count = 0;
    for (uint8_t i = 0; i < MATRIX_ROWS; i++) {
        count += bitpop16(matrix[i]);
    }
    return count;
}

/* Column pin configuration
 * col: 0   1   2   3   4   5 
 * pin: D0, D4, C6, D7, E6, B4
 */
 
static void  init_cols(void)
{
    // Input with pull-up(DDR:0, PORT:1)
    DDRD  &= ~(1<<0 | 1<<4 | 1<<7 );
    PORTD |=  (1<<0 | 1<<4 | 1<<7 );
    DDRB  &= ~(1<<4);
    PORTB |=  (1<<4);
    DDRC  &= ~(1<<6);
    PORTC |=  (1<<6);
    DDRE  &= ~(1<<6);
    PORTE |=  (1<<6);
}

static matrix_row_t read_cols(void)
{
/* //LED on Bottom of switch
    return (PIND&(1<<0) ? 0 : (1<<0)) |
           (PIND&(1<<4) ? 0 : (1<<1)) |
           (PINC&(1<<6) ? 0 : (1<<2)) |
           (PIND&(1<<7) ? 0 : (1<<3)) |
           (PINE&(1<<6) ? 0 : (1<<4)) |
           (PINB&(1<<4) ? 0 : (1<<5));
*/
//LED on Top of switch
    return (PIND&(1<<0) ? 0 : (1<<5)) |
           (PIND&(1<<4) ? 0 : (1<<4)) |
           (PINC&(1<<6) ? 0 : (1<<3)) |
           (PIND&(1<<7) ? 0 : (1<<2)) |
           (PINE&(1<<6) ? 0 : (1<<1)) |
           (PINB&(1<<4) ? 0 : (1<<0));
}

/* Row pin configuration
 * row: 0   1   2   3   4 
 * pin: B6, B2, B3, B1, F7
 */
 
static void unselect_rows(void)
{
    // Hi-Z(DDR:0, PORT:0) to unselect
    DDRF  &= ~0b10000000;
    PORTF &= ~0b10000000;
	DDRB  &= ~0b01001110;
    PORTB &= ~0b01001110;
}

static void select_row(uint8_t row)
{
    // Output low(DDR:1, PORT:0) to select
/* //LED on Bottom of switch
    switch (row) {
        case 0:
            DDRB  |= (1<<6);
            PORTB &= ~(1<<6);
            break;
        case 1:
            DDRB  |= (1<<2);
            PORTB &= ~(1<<2);
            break;
        case 2:
            DDRB  |= (1<<3);
            PORTB &= ~(1<<3);
            break;
        case 3:
            DDRB  |= (1<<1);
            PORTB &= ~(1<<1);
            break;
        case 4:
            DDRF  |= (1<<7);
            PORTF &= ~(1<<7);
            break;
    }
*/
//LED on Top of switch
    switch (row) {
        case 4:
            DDRB  |= (1<<6);
            PORTB &= ~(1<<6);
            break;
        case 3:
            DDRB  |= (1<<2);
            PORTB &= ~(1<<2);
            break;
        case 2:
            DDRB  |= (1<<3);
            PORTB &= ~(1<<3);
            break;
        case 1:
            DDRB  |= (1<<1);
            PORTB &= ~(1<<1);
            break;
        case 0:
            DDRF  |= (1<<7);
            PORTF &= ~(1<<7);
            break;
    }

}
