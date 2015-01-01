/*
Copyright 2013 Oleg Kostyuk <cub.uanic@gmail.com>

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
#include "action_layer.h"
#include "print.h"
#include "debug.h"
#include "util.h"
#include "matrix.h"
#include "ergodox.h"
#include "i2cmaster.h"

#ifndef DEBOUNCE
#   define DEBOUNCE	5
#endif
static uint8_t debouncing = DEBOUNCE;

/* matrix state(1:on, 0:off) */
static matrix_row_t matrix[MATRIX_ROWS];
static matrix_row_t matrix_debouncing[MATRIX_ROWS];

static matrix_row_t read_cols(uint8_t mcp23018_status, uint8_t row);
static void init_cols(void);
static void unselect_rows(uint8_t mcp23018_status);
static void select_row(uint8_t mcp23018_status, uint8_t row);


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
    // initialize row and col
    init_ergodox();
    uint8_t mcp23018_status;
    mcp23018_status = init_mcp23018();
    unselect_rows(mcp23018_status);
    init_cols();

    // initialize matrix state: all keys off
    for (uint8_t i=0; i < MATRIX_ROWS; i++) {
        matrix[i] = 0;
        matrix_debouncing[i] = 0;
    }
}

uint8_t matrix_scan(void)
{
#ifdef KEYMAP_CUB
    uint8_t layer = biton32(layer_state);

    if (layer == 1) {
        ergodox_left_led_1_on();
        ergodox_left_led_2_off();
        ergodox_left_led_3_off();
    } else if (layer == 2) {
        ergodox_left_led_1_off();
        ergodox_left_led_2_on();
        ergodox_left_led_3_off();
    } else if (layer == 3) {
        ergodox_left_led_1_off();
        ergodox_left_led_2_off();
        ergodox_left_led_3_on();
    } else if (layer == 4) {
        ergodox_left_led_1_on();
        ergodox_left_led_2_off();
        ergodox_left_led_3_on();
    } else if (layer == 5) {
        ergodox_left_led_1_on();
        ergodox_left_led_2_on();
        ergodox_left_led_3_off();
    } else if (layer == 6) {
        ergodox_left_led_1_off();
        ergodox_left_led_2_on();
        ergodox_left_led_3_on();
    } else if (layer == 7) {
        ergodox_left_led_1_on();
        ergodox_left_led_2_on();
        ergodox_left_led_3_on();
    } else {
        ergodox_left_led_1_off();
        ergodox_left_led_2_off();
        ergodox_left_led_3_off();
    }

    // not actually needed because we already calling init_mcp23018() in next line
    // ergodox_left_leds_update();

#endif

    uint8_t mcp23018_status = init_mcp23018();

    for (uint8_t i = 0; i < MATRIX_ROWS; i++) {
        select_row(mcp23018_status, i);
        _delay_us(30);  // without this wait read unstable value.
        matrix_row_t cols = read_cols(mcp23018_status, i);
        if (matrix_debouncing[i] != cols) {
            matrix_debouncing[i] = cols;
            if (debouncing) {
                debug("bounce!: "); debug_hex(debouncing); debug("\n");
            }
            debouncing = DEBOUNCE;
        }
        unselect_rows(mcp23018_status);
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
 *
 * Teensy
 * col: 0   1   2   3   4   5
 * pin: F0  F1  F4  F5  F6  F7 
 *
 * MCP23018
 * col: 0   1   2   3   4   5
 * pin: B5  B4  B3  B2  B1  B0 
 */
static void  init_cols(void)
{
    // init on mcp23018
    // not needed, already done as part of init_mcp23018()

    // init on teensy
    // Input with pull-up(DDR:0, PORT:1)
    DDRF  &= ~(1<<7 | 1<<6 | 1<<5 | 1<<4 | 1<<1 | 1<<0);
    PORTF |=  (1<<7 | 1<<6 | 1<<5 | 1<<4 | 1<<1 | 1<<0);
}

static matrix_row_t read_cols(uint8_t mcp23018_status, uint8_t row)
{
    if (row < 7) {
        if (mcp23018_status) { // if there was an error
            return 0;
        } else {
            uint8_t data = 0;
            uint8_t err = 0x20;
            err = i2c_start(I2C_ADDR_WRITE);    if (err) goto out;
            err = i2c_write(GPIOB);             if (err) goto out;
            err = i2c_start(I2C_ADDR_READ);     if (err) goto out;
            data = i2c_readNak();
            data = ~data;
        out:
            i2c_stop();
            return data;
        }
    } else {
        // read from teensy
        return
            (PINF&(1<<0) ? 0 : (1<<0)) |
            (PINF&(1<<1) ? 0 : (1<<1)) |
            (PINF&(1<<4) ? 0 : (1<<2)) |
            (PINF&(1<<5) ? 0 : (1<<3)) |
            (PINF&(1<<6) ? 0 : (1<<4)) |
            (PINF&(1<<7) ? 0 : (1<<5)) ;
    }
}

/* Row pin configuration
 *
 * Teensy
 * row: 7   8   9   10  11  12  13
 * pin: B0  B1  B2  B3  D2  D3  C6
 *
 * MCP23018
 * row: 0   1   2   3   4   5   6
 * pin: A0  A1  A2  A3  A4  A5  A6
 */
static void unselect_rows(uint8_t mcp23018_status)
{
    // unselect on mcp23018
    if (mcp23018_status) { // if there was an error
        // do nothing
    } else {
        // set all rows hi-Z : 1
        uint8_t err = 0x20;
        err = i2c_start(I2C_ADDR_WRITE);    if (err) goto out;
        err = i2c_write(GPIOA);             if (err) goto out;
        err = i2c_write( 0xFF
                & ~(ergodox_left_led_3<<LEFT_LED_3_SHIFT)
              );                            if (err) goto out;
    out:
        i2c_stop();
    }

    // unselect on teensy
    // Hi-Z(DDR:0, PORT:0) to unselect
    DDRB  &= ~(1<<0 | 1<<1 | 1<<2 | 1<<3);
    PORTB &= ~(1<<0 | 1<<1 | 1<<2 | 1<<3);
    DDRD  &= ~(1<<2 | 1<<3);
    PORTD &= ~(1<<2 | 1<<3);
    DDRC  &= ~(1<<6);
    PORTC &= ~(1<<6);
}

static void select_row(uint8_t mcp23018_status, uint8_t row)
{
    if (row < 7) {
        // select on mcp23018
        if (mcp23018_status) { // if there was an error
            // do nothing
        } else {
            // set active row low  : 0
            // set other rows hi-Z : 1
            uint8_t err = 0x20;
            err = i2c_start(I2C_ADDR_WRITE);        if (err) goto out;
            err = i2c_write(GPIOA);                 if (err) goto out;
            err = i2c_write( 0xFF & ~(1<<row) 
                    & ~(ergodox_left_led_3<<LEFT_LED_3_SHIFT)
                  );                                if (err) goto out;
        out:
            i2c_stop();
        }
    } else {
        // select on teensy
        // Output low(DDR:1, PORT:0) to select
        switch (row) {
            case 7:
                DDRB  |= (1<<0);
                PORTB &= ~(1<<0);
                break;
            case 8:
                DDRB  |= (1<<1);
                PORTB &= ~(1<<1);
                break;
            case 9:
                DDRB  |= (1<<2);
                PORTB &= ~(1<<2);
                break;
            case 10:
                DDRB  |= (1<<3);
                PORTB &= ~(1<<3);
                break;
            case 11:
                DDRD  |= (1<<2);
                PORTD &= ~(1<<3);
                break;
            case 12:
                DDRD  |= (1<<3);
                PORTD &= ~(1<<3);
                break;
            case 13:
                DDRC  |= (1<<6);
                PORTC &= ~(1<<6);
                break;
        }
    }
}

