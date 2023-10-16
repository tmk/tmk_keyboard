/*
Copyright 2023 Jun Wako <wakojun@gmail.com>

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the “Software”), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
of the Software, and to permit persons to whom the Software is furnished to do
so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include <stdint.h>
#include <util/delay.h>
#include "protocol/serial.h"
#include "debug.h"
#include "print.h"

// Archimedes commands
#define HRST    0xFF
#define RAK1    0xFE
#define RAK2    0xFD
#define RQMP    0x22
#define PRST    0x21
#define RQID    0x20
#define LEDS(stat)    (0x00 | (stat & 0x7))
#define RQPD(data)    (0x40 | (data & 0xF))

// Archimedes Replies
#define BACK    0x3F
#define NACK    0x30
#define SACK    0x31
#define MACK    0x32
#define SMAK    0x33


void matrix_init(void)
{
    //debug_enable = true;

    serial_init();

    // wait for keyboard coming up
    // otherwise LED status update fails
    print("Archimedes starts.\n");
    print("Sending HRST(0xFF): ");

    return;
}

uint8_t matrix_scan(void)
{
    //while (1) {
        print(".");
        serial_send(0xFF);
        _delay_ms(10);
        serial_send(RAK1);
        _delay_ms(10);
        serial_send(RAK2);
        _delay_ms(10);
        serial_send(SACK);

        _delay_ms(10);
        //serial_send(LEDS(0x5));  // ScrollLock and CapsLock
        //serial_send(LEDS(0x3));  // NumLock and CapsLock
        serial_send(LEDS(0x6));  // NumLock and CapsLock

        //serial_send(RQID);

        _delay_ms(500);
    //}
}

uint8_t matrix_get_row(uint8_t row)
{
    // TODO
    return 0;
}
