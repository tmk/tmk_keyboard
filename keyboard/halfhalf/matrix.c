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
#include <avr/wdt.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "print.h"
#include "debug.h"
#include "util.h"
#include "timer.h"
#include "matrix.h"
#include "i2c.h"
#include "serial.h"
#include "split-util.h"
#include "pro-micro.h"
#include "config.h"

#include "pin_defs.h"

#ifndef DEBOUNCE
#   define DEBOUNCE	5
#endif

#define ERROR_DISCONNECT_COUNT 5

#define I2C_MATRIX_ADDR 0x00
#define I2C_LED_ADDR ROWS_PER_HAND

static uint8_t debouncing = DEBOUNCE;
static uint8_t error_count = 0;

/* matrix state(1:on, 0:off) */
static matrix_row_t matrix[MATRIX_ROWS];
static matrix_row_t matrix_debouncing[MATRIX_ROWS];

static const uint8_t row_pins[MATRIX_ROWS] = MATRIX_ROW_PINS;
static const uint8_t col_pins[MATRIX_COLS] = MATRIX_COL_PINS;

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

    debug_enable = true;
    debug_matrix = true;
    debug_mouse = true;
    // initialize row and col
    unselect_rows();
    init_cols();

    TX_RX_LED_INIT;
    //Turn LEDs off by default
    RXLED0;
    TXLED0;

    // initialize matrix state: all keys off
    for (uint8_t i=0; i < MATRIX_ROWS; i++) {
        matrix[i] = 0;
        matrix_debouncing[i] = 0;
    }
}

uint8_t _matrix_scan(void)
{
    // Right hand is stored after the left in the matirx so, we need to offset it
    int offset = isLeftHand ? 0 : (ROWS_PER_HAND);

    for (uint8_t i = 0; i < ROWS_PER_HAND; i++) {
        select_row(i);
        _delay_us(30);  // without this wait read unstable value.
        matrix_row_t cols = read_cols();
        if (matrix_debouncing[i+offset] != cols) {
            matrix_debouncing[i+offset] = cols;
            debouncing = DEBOUNCE;
        }
        unselect_rows();
    }

    if (debouncing) {
        if (--debouncing) {
            _delay_ms(1);
        } else {
            for (uint8_t i = 0; i < ROWS_PER_HAND; i++) {
                matrix[i+offset] = matrix_debouncing[i+offset];
            }
        }
    }

    return 1;
}

// Get rows from other half over i2c
int i2c_transaction(void) {
    bool err = false;
    int slaveOffset = (isLeftHand) ? (ROWS_PER_HAND) : 0;

    err = i2c_master_read(
            SLAVE_I2C_ADDRESS, // i2c address of other half
            I2C_MATRIX_ADDR, // read the slaves matrix data
            matrix+slaveOffset, // store in correct position in master's matrix
            ROWS_PER_HAND // number of bytes to read
        );

#ifdef I2C_WRITE_TEST_CODE
    // controls the RX led on the slave and toggles it every second
    uint8_t test_data = (timer_read() / 1000) % 2;

    err |= i2c_master_write(
            SLAVE_I2C_ADDRESS, // i2c address of other half
            I2C_LED_ADDR, // address for led control
            &test_data, // data to send
            sizeof(test_data) // size of test data
        );
#endif

    return err;
}

int serial_transaction(void) {
    int slaveOffset = (isLeftHand) ? (ROWS_PER_HAND) : 0;

    if (serial_update_buffers()) {
        return 1;
    }

    for (int i = 0; i < ROWS_PER_HAND; ++i) {
        matrix[slaveOffset+i] = serial_slave_buffer[i];
    }
    return 0;
}

uint8_t matrix_scan(void)
{
    int ret = _matrix_scan();



#ifdef USE_I2C
    if( i2c_transaction() ) {
#else
    if( serial_transaction() ) {
#endif
        // turn on the indicator led when halves are disconnected
        TXLED1;

        error_count++;

        if (error_count > ERROR_DISCONNECT_COUNT) {
            // reset other half if disconnected
            int slaveOffset = (isLeftHand) ? (ROWS_PER_HAND) : 0;
            for (int i = 0; i < ROWS_PER_HAND; ++i) {
                matrix[slaveOffset+i] = 0;
            }
        }
    } else {
        // turn off the indicator led on no error
        TXLED0;
        error_count = 0;
    }

    return ret;
}

void matrix_slave_scan(void) {
    _matrix_scan();

    int offset = (isLeftHand) ? 0 : (MATRIX_ROWS / 2);

#ifdef USE_I2C
    for (int i = 0; i < ROWS_PER_HAND; ++i) {
        i2c_slave_write(I2C_MATRIX_ADDR+i, matrix[offset+i]);
    }

#ifdef I2C_WRITE_TEST_CODE
    // control the pro micro RX LED based on what the
    // i2c master has sent us
    uint8_t led_state = i2c_slave_read(I2C_LED_ADDR);
    if (led_state == 1) {
        RXLED1;
    } else if(led_state == 0) {
        RXLED0;
    }
#endif

#else
    for (int i = 0; i < ROWS_PER_HAND; ++i) {
        serial_slave_buffer[i] = matrix[offset+i];
    }
#endif

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


static void  init_cols(void)
{
    for(int x = 0; x < MATRIX_COLS; x++) {
        _SFR_IO8((col_pins[x] >> 4) + 1) &=  ~_BV(col_pins[x] & 0xF);
        _SFR_IO8((col_pins[x] >> 4) + 2) |= _BV(col_pins[x] & 0xF);
    }
}

static matrix_row_t read_cols(void)
{
    matrix_row_t result = 0;
    for(int x = 0; x < MATRIX_COLS; x++) {
        result |= (_SFR_IO8(col_pins[x] >> 4) & _BV(col_pins[x] & 0xF)) ? 0 : (1 << x);
    }
    return result;
}

static void unselect_rows(void)
{
    for(int x = 0; x < ROWS_PER_HAND; x++) {
        _SFR_IO8((row_pins[x] >> 4) + 1) &=  ~_BV(row_pins[x] & 0xF);
        _SFR_IO8((row_pins[x] >> 4) + 2) |= _BV(row_pins[x] & 0xF);
    }
}

static void select_row(uint8_t row)
{
    _SFR_IO8((row_pins[row] >> 4) + 1) |=  _BV(row_pins[row] & 0xF);
    _SFR_IO8((row_pins[row] >> 4) + 2) &= ~_BV(row_pins[row] & 0xF);
}
