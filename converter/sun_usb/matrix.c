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

#include <stdint.h>
#include <stdbool.h>
#include <avr/io.h>
#include <util/delay.h>
#include "print.h"
#include "util.h"
#include "matrix.h"
#include "debug.h"
#include "protocol/serial.h"
#include "led.h"
#include "host.h"


/*
 * Matrix Array usage:
 *
 * ROW: 16(4bits)
 * COL:  8(3bits)
 *
 *    8bit wide
 *   +---------+
 *  0|00 ... 07|
 *  1|08 ... 0F|
 *  :|   ...   |
 *  :|   ...   |
 *  E|70 ... 77|
 *  F|78 ... 7F|
 *   +---------+
 */
static uint8_t matrix[MATRIX_ROWS];
#define ROW(code)      ((code>>3)&0xF)
#define COL(code)      (code&0x07)


void matrix_init(void)
{
    DDRD |= (1<<6);
    PORTD |= (1<<6);
    //debug_enable = true;

    serial_init();

    // initialize matrix state: all keys off
    for (uint8_t i=0; i < MATRIX_ROWS; i++) matrix[i] = 0x00;

    // wait for keyboard coming up
    // otherwise LED status update fails
    print("Reseting ");
    while (1) {
        print(".");
        while (serial_recv());
        serial_send(0x01);
        _delay_ms(500);
        if (serial_recv() == 0xFF) {
            _delay_ms(500);
            if (serial_recv() == 0x04)
                break;
        }
    }
    print(" Done\n");

    PORTD &= ~(1<<6);
    return;
}

uint8_t matrix_scan(void)
{
    uint8_t code;
    code = serial_recv();
    if (!code) return 0;

    debug_hex(code); debug(" ");

    switch (code) {
        case 0xFF:  // reset success: FF 04
            print("reset: ");
            _delay_ms(500);
            code = serial_recv();
            xprintf("%02X\n", code);
            if (code == 0x04) {
                // LED status
                led_set(host_keyboard_leds());
            }
            return 0;
        case 0xFE:  // layout: FE <layout>
            print("layout: ");
            _delay_ms(500);
            xprintf("%02X\n", serial_recv());
            return 0;
        case 0x7E:  // reset fail: 7E 01
            print("reset fail: ");
            _delay_ms(500);
            xprintf("%02X\n", serial_recv());
            return 0;
        case 0x7F:
            // all keys up
            for (uint8_t i=0; i < MATRIX_ROWS; i++) matrix[i] = 0x00;
            return 0;
    }

    if (code&0x80) {
        // break code
        if (matrix_is_on(ROW(code), COL(code))) {
            matrix[ROW(code)] &= ~(1<<COL(code));
        }
    } else {
        // make code
        if (!matrix_is_on(ROW(code), COL(code))) {
            matrix[ROW(code)] |=  (1<<COL(code));
        }
    }
    return code;
}

inline
uint8_t matrix_get_row(uint8_t row)
{
    return matrix[row];
}
