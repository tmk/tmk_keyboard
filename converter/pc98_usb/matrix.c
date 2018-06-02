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
#include "led.h"
#include "debug.h"
#include "protocol/serial.h"


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


static void pc98_send(uint8_t data)
{
    PC98_RDY_PORT |= (1<<PC98_RDY_BIT);
    _delay_ms(1);
    serial_send(data);
    _delay_ms(1);
    PC98_RDY_PORT &= ~(1<<PC98_RDY_BIT);
}

static int16_t pc98_wait_response(void)
{
    int16_t code = -1;
    uint8_t timeout = 255;
    while (timeout-- && (code = serial_recv2()) == -1) _delay_ms(1);
    return code;
}

static void pc98_inhibit_repeat(void)
{
    uint16_t code;

    // clear recv buffer
    while (serial_recv()) ;
RETRY:
    _delay_ms(100);
    pc98_send(0x9C);
    code = pc98_wait_response();
    if (code != -1) xprintf("PC98: send 9C: %02X\n", code);
    if (code != 0xFA) goto RETRY;

    _delay_ms(100);
    pc98_send(0x70);
    code = pc98_wait_response();
    if (code != -1) xprintf("PC98: send 70: %02X\n", code);
    if (code != 0xFA) goto RETRY;
}

void matrix_init(void)
{
    debug_keyboard = true;
    PC98_RST_DDR |= (1<<PC98_RST_BIT);
    PC98_RDY_DDR |= (1<<PC98_RDY_BIT);
    PC98_RTY_DDR |= (1<<PC98_RTY_BIT);
    PC98_RST_PORT |= (1<<PC98_RST_BIT);
    PC98_RDY_PORT |= (1<<PC98_RDY_BIT);
    PC98_RTY_PORT |= (1<<PC98_RTY_BIT);


    serial_init();

    // PC98 reset
/*
    PC98_RST_PORT &= ~(1<<PC98_RST_BIT);
    _delay_us(15);
    PC98_RST_PORT |= (1<<PC98_RST_BIT);
    _delay_us(13);
    PC98_RDY_PORT &= ~(1<<PC98_RDY_BIT);
*/

    _delay_ms(500);
    pc98_inhibit_repeat();


    // PC98 ready
    PC98_RDY_PORT &= ~(1<<PC98_RDY_BIT);

    // initialize matrix state: all keys off
    for (uint8_t i=0; i < MATRIX_ROWS; i++) matrix[i] = 0x00;

    debug("init\n");
    return;
}

uint8_t matrix_scan(void)
{
    uint16_t code;
    PC98_RDY_PORT |= (1<<PC98_RDY_BIT);
    //_delay_us(30);
    code = serial_recv2();
    PC98_RDY_PORT &= ~(1<<PC98_RDY_BIT);
    if (code == -1) return 0;

    print_hex8(code); print(" ");

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

void led_set(uint8_t usb_led)
{
#ifdef PC98_LED_CONTROL
    uint8_t led_state = 0x70;
    if (usb_led & (1<<USB_LED_NUM_LOCK))    led_state |= (1<<0);
    if (usb_led & (1<<USB_LED_CAPS_LOCK))   led_state |= (1<<2);
    xprintf("led_set: %02X\n", led_state);

    pc98_send(0x9D);
    _delay_ms(100);
    pc98_send(led_state);
    // responses(FA or FC) will be ignored in matrix_scan()
#endif
}
