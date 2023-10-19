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
#include "wait.h"
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

// Archimedes LED
#define ARC_LED_CAPS_LOCK      0
#define ARC_LED_NUM_LOCK       1
#define ARC_LED_SCROLL_LOCK    2


void matrix_init(void)
{
    //debug_enable = true;

    serial_init();

    // wait for keyboard coming up
    // otherwise LED status update fails
    print("Archimedes starts.\n");
    xprintf("EIMSK: %02X\n", EIMSK);
    xprintf("EICRA: %02X\n", EICRA);

    return;
}

static uint8_t arc_led = 1<<ARC_LED_SCROLL_LOCK;
static enum  {
    INIT,
    SCAN,
    WAIT_KEY_COL,
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
    serial_send(cmd);
}

uint8_t matrix_scan(void)
{
    static uint8_t key;

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
                case -1:
                    // no reply
                    break;
                case KDDA ... KDDA+15:
                case KUDA ... KUDA+15:
                    // key row
                    key = (d & 0xF) << 4;
                    wait_us(100);

                    // ack for key data first byte
                    send_cmd(BACK);
                    state = WAIT_KEY_COL;
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
                                                                //
                    // ack
                    wait_us(100);
                    send_cmd(SMAK);
                    state = SCAN;

                    // TODO: make/brak key
                    xprintf("[k%02X] ", key);
                    break;
                default:
                    // error
                    state = INIT;
                    break;
            }
            break;
        }
    }

    // TODO
    // toggle ScrollLock LED
/*
    wait_ms(10);
    arc_led = arc_led ^ 1<<ARC_LED_SCROLL_LOCK;
    serial_send(LEDS(arc_led % 8));
    wait_ms(500);
*/

    return 0;
}

uint8_t matrix_get_row(uint8_t row)
{
    // TODO
    return 0;
}

#include "led.h"
void led_set(uint8_t usb_led)
{
    arc_led = 0;
    if (usb_led & (1<<USB_LED_NUM_LOCK))    arc_led |= (1<<ARC_LED_NUM_LOCK);
    if (usb_led & (1<<USB_LED_CAPS_LOCK))   arc_led |= (1<<ARC_LED_CAPS_LOCK);
    if (usb_led & (1<<USB_LED_SCROLL_LOCK)) arc_led |= (1<<ARC_LED_SCROLL_LOCK);
    xprintf("LED: %02X %02X\n", usb_led, arc_led);
}
