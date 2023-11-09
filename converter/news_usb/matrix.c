/*
Copyright 2012,2023 Jun Wako <wakojun@gmail.com>

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
#include "serial.h"
#include "matrix.h"
#include "debug.h"
#include "led.h"
#include "hook.h"
#include "wait.h"


/*
 * Matrix Array usage:
 *
 * ROW: 16
 * COL:8
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
 *
 */
static uint8_t matrix[MATRIX_ROWS];
#define ROW(code)      ((code>>3)&0xF)
#define COL(code)      (code&0x07)

static uint8_t news_led = 0;


void matrix_init(void)
{
    serial_init();

    // initialize matrix state: all keys off
    for (uint8_t i=0; i < MATRIX_ROWS; i++) matrix[i] = 0x00;

    return;
}

static uint16_t send_cmd(uint8_t cmd)
{
    int16_t ret = 0;

    xprintf("s%02X ", cmd);
    serial_send(cmd);
    wait_ms(10);

    int16_t c;
    while ((c = serial_recv2()) != -1) {
        if ((c != 0x7B) && (c != 0xFB)) {
            ret <<= 8;
            ret |= c & 0xFF;
        }
        xprintf("r%02X ", c);
        if (c == 0xFB) {
            xprintf("\n");
            return ret;
        }
    }
    return -1;
}

void hook_late_init(void)
{
    /* Commands for starup
     * 82 is needed to enable LED command at least
     *
     *  80      Reset? turns LEDs off
     *          FB
     *  81      replies whether LED command is enabled
     *          7B [00|01] FB
     *  82      enable LED command
     *  83      replies DIP switches status
     *          7B 00 0X FB
     */
    send_cmd(0x80);
    send_cmd(0x82);
    send_cmd(0x81);
    send_cmd(0x83);
}

void tone(unsigned int frequency, unsigned long duration);
void noTone(void);
uint8_t matrix_scan(void)
{
    static uint8_t sent_led = 0;
    int16_t code;

    code = serial_recv2();
    if (code == -1) {
        // update LED
        if (news_led != sent_led) {
            send_cmd(0xB0);
            send_cmd(news_led);
            sent_led = news_led;
        }
        return 0;
    }

    xprintf("%02X ", code);
    if (code&0x80) {
        // break code
        if (matrix_is_on(ROW(code), COL(code))) {
            matrix[ROW(code)] &= ~(1<<COL(code));
        }
    } else {
        // make code
        if (!matrix_is_on(ROW(code), COL(code))) {
            matrix[ROW(code)] |=  (1<<COL(code));
            //tone(440, 100);
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
    news_led = 0;
    if (usb_led & (1<<USB_LED_CAPS_LOCK))
        news_led |= 4;
    if (usb_led & (1<<USB_LED_NUM_LOCK))
        news_led |= 8;
}
