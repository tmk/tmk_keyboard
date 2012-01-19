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
#include <util/delay.h>
#include "print.h"
#include "debug.h"
#include "util.h"
#include "matrix.h"
#include "ports.h"

// need to fix the row/col limit to support full C(22,2)
#if (MATRIX_COLS > 16)
#   error "MATRIX_COLS must not exceed 16"
#endif
#if (MATRIX_ROWS > 8)
#   error "MATRIX_ROWS must not exceed 8"
#endif


#ifndef DEBOUNCE
#   define DEBOUNCE	0
#endif
static uint8_t debouncing = DEBOUNCE;

static const bool matrix_has_diodes_in_row = DIODE_DIRECTION;


// Maps the switch matrix to ports on the teensy
// The index of portmap is teensy's port reading from left to right:
/* GND-----Vcc */
/* B0   T   F0 */
/* B1   E   F1 */
/* B2   E   F4 */
/* B3   N   F5 */
/* B7   S   F6 */
/* D0   Y   F7 */
/* D1       B6 */
/* D2   2   B5 */
/* D3   .   B4 */
/* C6   0   D7 */
/* C7-------D6 */
/* Or in other words portmap is:
 * Index:  0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21
 * Port:  B0 F0 B1 F1 B2 F4 B3 F5 B7 F6 D0 F7 D1 B6 D2 B5 D3 B4 C6 D7 C7 D6
 */
static const uint8_t portmap[] = {
    PORTMAP
};

static const uint8_t pinmap[] = {
	0, 0, 1, 1, 2, 4, 3, 5, 7, 6, 0, 7, 1, 6, 2, 5, 3, 4, 6, 7, 7, 6
};

// matrix state buffer(1:on, 0:off)
#if (MATRIX_COLS <= 8)
static uint8_t *matrix;
static uint8_t *matrix_prev;
static uint8_t _matrix0[MATRIX_ROWS];
static uint8_t _matrix1[MATRIX_ROWS];
#else
static uint16_t *matrix;
static uint16_t *matrix_prev;
static uint16_t _matrix0[MATRIX_ROWS];
static uint16_t _matrix1[MATRIX_ROWS];
#endif

#ifdef MATRIX_HAS_GHOST
static bool matrix_has_ghost_in_row(uint8_t row);
#endif
#if (MATRIX_COLS <= 8)
static uint8_t read(void);
#else
static uint16_t read(void);
#endif
static uint8_t read_pin(uint8_t index);
static void unselect(void);
static void unselect_pin(uint8_t index);
static void select(uint8_t index);


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

#define CLEAR_PORT(index) (|= (1 << pinmap[index]));
#define SET_PORT(index) (&= ~(1 << pinmap[index]));
#define IS_INPUT(port) ((matrix_has_diodes_in_row && IS_COL(port)) || (!matrix_has_diodes_in_row && IS_ROW(port)))
#define IS_OUTPUT(port) ((matrix_has_diodes_in_row && IS_ROW(port)) || (!matrix_has_diodes_in_row && IS_COL(port)))

// Input with pull-up(DDR:0, PORT:1)
void pull_up(uint8_t index)
{
	switch (index) {
		case 0:
		case 2:
		case 4:
		case 6:
		case 8:
		case 13:
		case 15:
		case 17:
			print("pull up port B pin ");
			phex(pinmap[index]);
			print("\n");
			//DDRB CLEAR_PORT(index);
			//PORTB SET_PORT(index);
			break;
		case 18:
		case 20:
			print("pull up port C pin ");
			phex(pinmap[index]);
			print("\n");
			//DDRC CLEAR_PORT(index);
			//PORTC SET_PORT(index);
			break;
		case 10:
		case 12:
		case 14:
		case 16:
		case 19:
		case 21:
			print("pull up port D pin ");
			phex(pinmap[index]);
			print("\n");
			//DDRD CLEAR_PORT(index);
			//PORTD SET_PORT(index);
			break;
		case 1:
		case 3:
		case 5:
		case 7:
		case 9:
		case 11:
			print("pull up port F pin ");
			phex(pinmap[index]);
			print("\n");
			//DDRF CLEAR_PORT(index);
			//PORTF SET_PORT(index);
			break;
	}
}


void matrix_init(void)
{
	_delay_ms(1000);
	print_enable = true;
	print("init matrix\n");
	// initialize row and col
    unselect();
	for (uint8_t i = 0; i < 22; i++) {
		if (IS_INPUT(portmap[i])) {
			pull_up(i);
		}
	}

    // initialize matrix state: all keys off
    for (uint8_t i = 0; i < MATRIX_ROWS; i++) _matrix0[i] = 0x00;
    for (uint8_t i = 0; i < MATRIX_ROWS; i++) _matrix1[i] = 0x00;
    matrix = _matrix0;
    matrix_prev = _matrix1;
}

uint8_t matrix_scan(void)
{
    if (!debouncing) {
        uint8_t *tmp = matrix_prev;
        matrix_prev = matrix;
        matrix = tmp;
    }

	uint8_t row = 0;
	for (uint8_t i = 0; i < 22; i++) {
		if (IS_OUTPUT(portmap[i])) {
			select(i);
			_delay_ms(300);  // without this wait read unstable value.
#if (MATRIX_COLS <= 8)
			uint8_t stripe = ~read();
#else
			uint16_t stripe = ~read();
#endif
			if (matrix[row] != stripe) {
				matrix[row] = stripe;
				row++;
				if (debouncing) {
					debug("bounce!: "); debug_hex(debouncing); print("\n");
				}
				debouncing = DEBOUNCE;
			}
			unselect_pin(i);
		}
    }

    if (debouncing) {
        debouncing--;
    }

    return 1;
}

bool matrix_is_modified(void)
{
    if (debouncing) return false;
    for (uint8_t i = 0; i < MATRIX_ROWS; i++) {
        if (matrix[i] != matrix_prev[i]) {
            return true;
        }
    }
    return false;
}

inline
bool matrix_has_ghost(void)
{
#ifdef MATRIX_HAS_GHOST
    for (uint8_t i = 0; i < MATRIX_ROWS; i++) {
        if (matrix_has_ghost_in_row(i))
            return true;
    }
#endif
    return false;
}

inline
bool matrix_is_on(uint8_t row, uint8_t col)
{
    return (matrix[row] & (1<<col));
}

inline
#if (MATRIX_COLS <= 8)
uint8_t matrix_get_row(uint8_t row)
#else
uint16_t matrix_get_row(uint8_t row)
#endif
{
    return matrix[row];
}

void matrix_print(void)
{
    print("\nr/c 01234567\n");
    for (uint8_t row = 0; row < matrix_rows(); row++) {
        phex(row); print(": ");
#if (MATRIX_COLS <= 8)
        pbin_reverse(matrix_get_row(row));
#else
        pbin_reverse16(matrix_get_row(row));
#endif
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
#if (MATRIX_COLS <= 8)
        count += bitpop(matrix[i]);
#else
        count += bitpop16(matrix[i]);
#endif
    }
    return count;
}

#ifdef MATRIX_HAS_GHOST
inline
static bool matrix_has_ghost_in_row(uint8_t row)
{
    // no ghost exists in case less than 2 keys on
    if (((matrix[row] - 1) & matrix[row]) == 0)
        return false;

    // ghost exists in case same state as other row
    for (uint8_t i=0; i < MATRIX_ROWS; i++) {
        if (i != row && (matrix[i] & matrix[row]) == matrix[row])
            return true;
    }
    return false;
}
#endif

uint8_t read_pin(uint8_t index)
{
	switch (index) {
		case 0:
		case 2:
		case 4:
		case 6:
		case 8:
		case 13:
		case 15:
		case 17:
			print("read port B pin ");
			phex(pinmap[index]);
			print("\n");
			break;
			//return PINB & (1 << pinmap[index]);
		case 18:
		case 20:
			print("read port C pin ");
			phex(pinmap[index]);
			print("\n");
			break;
			//return PINC & (1 << pinmap[index]);
		case 10:
		case 12:
		case 14:
		case 16:
		case 19:
		case 21:
			print("read port D pin ");
			phex(pinmap[index]);
			print("\n");
			break;
			//return PIND & (1 << pinmap[index]);
		case 1:
		case 3:
		case 5:
		case 7:
		case 9:
		case 11:
			print("read port F pin ");
			phex(pinmap[index]);
			print("\n");
			break;
			//return PINF & (1 << pinmap[index]);
	}
	return 0;
}

//inline
#if (MATRIX_COLS <= 8)
static uint8_t read(void)
#else
static uint16_t read(void)
#endif
{
#if (MATRIX_COLS <= 8)
	uint8_t row = 0xFF;
#else
	uint16_t row = 0xFFFF;
#endif
	uint8_t bit = 0;
	for (uint8_t i = 0; i < 22; i++) {
		if (IS_INPUT(portmap[i])) {
			row |= ((read_pin(i) & 0x01) << bit);
			bit++;
		}
	}
	return row;
}

// Hi-Z(DDR:0, PORT:0) to unselect
void unselect_pin(uint8_t index)
{
	switch (index) {
		case 0:
		case 2:
		case 4:
		case 6:
		case 8:
		case 13:
		case 15:
		case 17:
			print("unselect port B pin ");
			phex(pinmap[index]);
			print("\n");
			//DDRB CLEAR_PORT(index);
			//PORTB CLEAR_PORT(index);
			break;
		case 18:
		case 20:
			print("unselect port C pin ");
			phex(pinmap[index]);
			print("\n");
			//DDRC CLEAR_PORT(index);
			//PORTC CLEAR_PORT(index);
			break;
		case 10:
		case 12:
		case 14:
		case 16:
		case 19:
		case 21:
			print("unselect port D pin ");
			phex(pinmap[index]);
			print("\n");
			//DDRD CLEAR_PORT(index);
			//PORTD CLEAR_PORT(index);
			break;
		case 1:
		case 3:
		case 5:
		case 7:
		case 9:
		case 11:
			print("unselect port F pin ");
			phex(pinmap[index]);
			print("\n");
			//DDRF CLEAR_PORT(index);
			//PORTF CLEAR_PORT(index);
			break;
	}
}

//inline
static void unselect(void)
{
	for (uint8_t i = 0; i < 22; i++) {
		if (IS_OUTPUT(portmap[i])) {
			unselect_pin(i);
		}
	}
}

// Output low(DDR:1, PORT:0) to select
static void select(uint8_t index)
{
	switch (index) {
		case 0:
		case 2:
		case 4:
		case 6:
		case 8:
		case 13:
		case 15:
		case 17:
			print("select port B pin ");
			phex(pinmap[index]);
			print("\n");
			//DDRB SET_PORT(index);
			//PORTB CLEAR_PORT(index);
			break;
		case 18:
		case 20:
			print("select port C pin ");
			phex(pinmap[index]);
			print("\n");
			//DDRC SET_PORT(index);
			//PORTC CLEAR_PORT(index);
			break;
		case 10:
		case 12:
		case 14:
		case 16:
		case 19:
		case 21:
			print("select port D pin ");
			phex(pinmap[index]);
			print("\n");
			//DDRD SET_PORT(index);
			//PORTD CLEAR_PORT(index);
			break;
		case 1:
		case 3:
		case 5:
		case 7:
		case 9:
		case 11:
			print("select port F pin ");
			phex(pinmap[index]);
			print("\n");
			//DDRF SET_PORT(index);
			//PORTF CLEAR_PORT(index);
			break;
	}
}
