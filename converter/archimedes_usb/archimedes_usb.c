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
#include <avr/interrupt.h>
#include "protocol/serial.h"
#include "matrix.h"
#include "wait.h"
#include "timer.h"
#include "host.h"
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
#define KDDA    0xC0
#define KUDA    0xD0
// mouse data: 0b0xxx xxxx(0x00 - 0x7F)
// mouse delta: -64 - 63
#define MDAT_MIN    0x00
#define MDAT_MAX    0x7F

// Archimedes LED
#define ARC_LED_CAPS_LOCK      0
#define ARC_LED_NUM_LOCK       1
#define ARC_LED_SCROLL_LOCK    2

/* key matrix 8x16
 * |R/C|  0|  1|  2|  3|  4|  5|  6|  7|  8|  9|  A|  B|  C|  D|  E|  F|
 * |---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|
 * |  0|ESC| F1| F2| F3| F4| F5| F6| F7| F8| F9|F10|F11|F12|Prt|ScL|Brk|
 * |  1| `~|  1|  2|  3|  4|  5|  6|  7|  8|  9|  0| -_| =+|  £| BS|Ins|
 * |  2|Hom|PgU|NmL| P/| P*| P#|Tab|  Q|  W|  E|  R|  T|  Y|  U|  I|  O|
 * |  3|  P| [{| ]}| \||Del|Cpy|PgD| P7| P8| P9| P-|LCt|  A|  S|  D|  F|
 * |  4|  G|  H|  J|  K|  L| ;:| '"|Rtn| P4| P5| P6| P+|LSh|   |  Z|  X|
 * |  5|  C|  V|  B|  N|  M| ,<| .>| /?|RSh| Up| P1| P2| P3|Cap|LAl|Spc|
 * |  6|Ral|RCt|Lef|Dow|Rig| P0| P.|PEn|   |   |   |   |   |   |   |   |
 * |  7|SW1|SW2|SW3|   |   |   |   |   |   |   |   |   |   |   |   |   |
 */
#define ROW(key)    ((key & 0x70) >> 4)
#define COL(key)    (key & 0x0F)
static matrix_row_t matrix[MATRIX_ROWS];

void matrix_clear(void)
{
    for (uint8_t i=0; i < MATRIX_ROWS; i++) matrix[i] = 0x00;
}

matrix_row_t matrix_get_row(uint8_t row)
{
    return matrix[row];
}


void matrix_init(void)
{
    //debug_enable = true;
    //debug_matrix = true;
    //debug_keyboard = true;
    //debug_mouse = true;

    matrix_clear();
    serial_init();

    // wait for keyboard coming up
    // otherwise LED status update fails
    print("Archimedes starts.\n");
    //xprintf("EIMSK: %02X\n", EIMSK);
    //xprintf("EICRA: %02X\n", EICRA);

    return;
}

// LED status
static uint8_t arc_led = 0;
static uint8_t arc_led_prev = 0;

static enum  {
    INIT,
    SCAN,
    WAIT_KEY_COL,
    WAIT_MDAT_Y,
} state = INIT;

static int16_t check_reply(void)
{
    int16_t d;
    while ((d = serial_recv2()) != -1) {
        xprintf("r%02X ", d & 0xFF);
        if (d == HRST) state = INIT;
        return d;
    }
    return -1;
}

static void send_cmd(uint8_t cmd)
{
    xprintf("s%02X ", cmd);
    uint8_t sreg = SREG;
    cli();
    serial_send(cmd);
    SREG = sreg;
}

uint8_t matrix_scan(void)
{
    static uint8_t key;
    static uint8_t mouse_btn = 0;
    static int8_t mouse_x = 0;
    static int8_t mouse_y = 0;

    switch (state) {
        case INIT:
            // ignore unprocessed replies
            check_reply();

            // reset sequence
            send_cmd(HRST);
            wait_ms(1);
            if (HRST != check_reply()) {
                wait_ms(1000);
                break;
            }

            send_cmd(RAK1);
            wait_ms(1);
            if (RAK1 != check_reply()) {
                wait_ms(1000);
                break;
            }

            send_cmd(RAK2);
            wait_ms(1);
            if (RAK2 != check_reply()) {
                wait_ms(1000);
                break;
            }

            // ack to scan now
            send_cmd(SMAK);
            check_reply();
            state = SCAN;
            break;
        case SCAN: {
            int16_t d;
            d = check_reply();
            switch (d) {
                case -1: // no input
                    // update LED
                    if (arc_led != arc_led_prev) {
                        wait_ms(1);
                        send_cmd(LEDS(arc_led));
                        arc_led_prev = arc_led;
                    }
                    break;
                case KDDA ... KDDA+15:
                case KUDA ... KUDA+15:
                    // key row
                    key = (d & 0x7) << 4;
                    wait_us(100);

                    // ack
                    send_cmd(BACK);
                    state = WAIT_KEY_COL;
                    break;
                case MDAT_MIN ... MDAT_MAX:
                    // sign bit for int8_t
                    if (d & 0x40) d |= 0x80;
                    mouse_x = d;
                    // ack
                    send_cmd(BACK);
                    state = WAIT_MDAT_Y;
                    break;
                default:
                    state = INIT;
                    break;
            }
            break;
        }
        case WAIT_KEY_COL: {
            int16_t d;
            d = check_reply();
            switch (d) {
                case -1:
                    // no reply
                    break;
                case KDDA ... KDDA+15:
                case KUDA ... KUDA+15:
                    // key col
                    key |= d & 0xF;
                    if ((d & KUDA) == KUDA) { key |= 0x80; }    // key up flag

                    // ack
                    wait_us(100);
                    send_cmd(SMAK);
                    state = SCAN;

                    if (key & 0x80) {
                        // break
                        switch (key & 0x7F) {
                            case 0x70:  // mouse SW1
                            case 0x71:  // mouse SW2
                            case 0x72:  // mouse SW3
                                mouse_btn &= ~(1 << (key & 3));

                                report_mouse_t mouse_report = {};
                                mouse_report.buttons = mouse_btn;
                                host_mouse_send(&mouse_report);
                                break;
                            default:
                                matrix[ROW(key)] &= ~(1 << COL(key));
                                break;
                        }
                    } else {
                        // make
                        switch (key & 0x7F) {
                            case 0x70:  // mouse SW1
                            case 0x71:  // mouse SW2
                            case 0x72:  // mouse SW3
                                mouse_btn |= (1 << (key & 3));

                                report_mouse_t mouse_report = {};
                                mouse_report.buttons = mouse_btn;
                                host_mouse_send(&mouse_report);
                                break;
                            default:
                                matrix[ROW(key)] |=  (1 << COL(key));
                                break;
                        }
                    }
                    xprintf("[k%02X] ", key);
                    break;
                default:
                    // error
                    state = INIT;
                    break;
            }
            break;
        }
        case WAIT_MDAT_Y: {
            int16_t d;
            d = check_reply();
            switch (d) {
                case -1:
                    // no reply
                    break;
                case MDAT_MIN ... MDAT_MAX:
                    // sign bit for int8_t
                    if (d & 0x40) d |= 0x80;
                    mouse_y = d;
                    xprintf("[m%02d,%02d] ", mouse_x, mouse_y);

                    report_mouse_t mouse_report = {};
                    mouse_report.buttons = mouse_btn;
                    // TODO: move direction is not confirmed
                    mouse_report.x = mouse_x;
                    mouse_report.y = mouse_y;
                    host_mouse_send(&mouse_report);

                    // ack
                    wait_us(100);
                    send_cmd(SMAK);
                    state = SCAN;
                    break;
                default:
                    state = INIT;
                    break;
            }
            break;
        }
    }

    // DEBUG
    // toggle ScrollLock LED
/*
    static uint16_t time_last;
    if (timer_elapsed(time_last) > 1000) {
        arc_led = arc_led ^ 1<<ARC_LED_SCROLL_LOCK;
        time_last = timer_read();
    }
*/

    return 0;
}

#include "led.h"
void led_set(uint8_t usb_led)
{
    arc_led = 0;
    if (usb_led & (1<<USB_LED_NUM_LOCK))    arc_led |= (1<<ARC_LED_NUM_LOCK);
    if (usb_led & (1<<USB_LED_CAPS_LOCK))   arc_led |= (1<<ARC_LED_CAPS_LOCK);
    if (usb_led & (1<<USB_LED_SCROLL_LOCK)) arc_led |= (1<<ARC_LED_SCROLL_LOCK);
    xprintf("[LED:%02X:%02X] ", usb_led, arc_led);
}
