/*
Copyright 2017 Jun Wako <wakojun@gmail.com>

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
#include "debug.h"
#include "timer.h"
#include "matrix.h"


#ifndef DEBOUNCE
#   define DEBOUNCE	5
#endif
/* matrix state(1:on, 0:off) */
static matrix_row_t row_debounced = 0;
static matrix_row_t row_debouncing = 0;
static bool debouncing = false;
static uint16_t debouncing_time = 0;


void matrix_init(void)
{
    debug_enable = true;
    debug_matrix = true;
    debug_mouse = true;

    // PB0: Input with pull-up(DDR:0, PORT:1)
    DDRB  &= ~(1<<0);
    PORTB |=  (1<<0);
}

uint8_t matrix_scan(void)
{
    matrix_row_t r = (PINB&(1<<0) ? 0 : 1);
    if (row_debouncing != r) {
        row_debouncing = r;
        debouncing = true;
        debouncing_time = timer_read();
    }

    if (debouncing && timer_elapsed(debouncing_time) > DEBOUNCE) {
        row_debounced = row_debouncing;
        debouncing = false;
    }
    return 1;
}

inline
matrix_row_t matrix_get_row(uint8_t row)
{
    return row_debounced;
}
