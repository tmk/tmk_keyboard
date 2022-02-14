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
    xprintf("s%02X ", data);
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

    // Keyboards require RDY pulse >=37us to send next data
    // https://archive.org/stream/PC9800TechnicalDataBookHARDWARE1993/PC-9800TechnicalDataBook_HARDWARE1993#page/n157
    PC98_RDY_PORT |=  (1<<PC98_RDY_BIT);
    _delay_us(40);
    PC98_RDY_PORT &= ~(1<<PC98_RDY_BIT);

    xprintf("r%04X ", code);
    return code;
}

static void pc98_inhibit_repeat(void)
{
    uint16_t code;
RETRY:
    pc98_send(0x9C);
    code = pc98_wait_response();
    if (code != -1) dprintf("send 9C: %02X\n", code);
    if (code != 0xFA) return;

    pc98_send(0x70);
    code = pc98_wait_response();
    if (code != -1) dprintf("send 70: %02X\n", code);
    if (code != 0xFA) goto RETRY;
}

static bool pc98_is_newtype(void)
{
    uint16_t code;
    pc98_send(0x9F);
    code = pc98_wait_response();
    if (code != 0xFA) return false;

    code = pc98_wait_response();
    if (code != 0xA0) return false;

    code = pc98_wait_response();
    if (code != 0x80) return false;

    return true;
}

static uint8_t pc98_led = 0;
static void pc98_led_set(void)
{
    uint16_t code;
RETRY:
    pc98_send(0x9D);
    code = pc98_wait_response();
    if (code != -1) dprintf("send 9D: %02X\n", code);
    if (code != 0xFA) return;

    pc98_send(pc98_led);
    code = pc98_wait_response();
    if (code != -1) dprintf("send %02X: %02X\n", pc98_led, code);
    if (code != 0xFA) goto RETRY;
}

void matrix_init(void)
{
    PC98_RST_PORT |= (1<<PC98_RST_BIT);
    PC98_RST_DDR |= (1<<PC98_RST_BIT);
    PC98_RDY_PORT |= (1<<PC98_RDY_BIT);
    PC98_RDY_DDR |= (1<<PC98_RDY_BIT);
    PC98_RTY_PORT |= (1<<PC98_RTY_BIT);
    PC98_RTY_DDR |= (1<<PC98_RTY_BIT);

    // PC98 reset
    // https://archive.org/stream/PC9800TechnicalDataBookHARDWARE1993/PC-9800TechnicalDataBook_HARDWARE1993#page/n359
    PC98_RDY_PORT |=  (1<<PC98_RDY_BIT);    // RDY: high
    PC98_RST_PORT &= ~(1<<PC98_RST_BIT);    // RST: low
    _delay_us(15);                          // > 13us
    PC98_RST_PORT |= (1<<PC98_RST_BIT);     // RST: high

    serial_init();

    _delay_ms(50);
    if (pc98_is_newtype()) xprintf("new type\n"); else xprintf("old type\n");
    pc98_inhibit_repeat();

    // initialize matrix state: all keys off
    for (uint8_t i=0; i < MATRIX_ROWS; i++) matrix[i] = 0x00;

    // ready to receive from keyboard
    PC98_RDY_PORT &= ~(1<<PC98_RDY_BIT);    // RDY: low

    return;
}

uint8_t matrix_scan(void)
{
    uint16_t code;
    code = serial_recv2();
    if (code == -1) {
#ifdef PC98_LED_CONTROL
        // Before sending command  we have to make sure that there is no unprocessed key in queue
        // otherwise keys will be missed during sending command
        if (pc98_led) {
            pc98_led_set();
            pc98_led = 0;
        }
#endif
        return 0;
    }

    xprintf("r%02X ", code);

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

    // Keyboards require RDY pulse >=37us to send next data
    // https://archive.org/stream/PC9800TechnicalDataBookHARDWARE1993/PC-9800TechnicalDataBook_HARDWARE1993#page/n157
    PC98_RDY_PORT |=  (1<<PC98_RDY_BIT);
    _delay_us(40);
    PC98_RDY_PORT &= ~(1<<PC98_RDY_BIT);

    return code;
}

inline
uint8_t matrix_get_row(uint8_t row)
{
    return matrix[row];
}

void led_set(uint8_t usb_led)
{
    // https://archive.org/stream/PC9800TechnicalDataBookHARDWARE1993/PC-9800TechnicalDataBook_HARDWARE1993#page/n161
    // http://www.webtech.co.jp/company/doc/undocumented_mem/io_kb.txt
    pc98_led = 0x70;
    if (usb_led & (1<<USB_LED_NUM_LOCK))    pc98_led |= (1<<0);
    if (usb_led & (1<<USB_LED_CAPS_LOCK))   pc98_led |= (1<<2);
    dprintf("usb_led: %02X\n", usb_led);
    dprintf("pc98_led: %02X\n", pc98_led);
}
