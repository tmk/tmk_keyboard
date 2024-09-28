/*
Copyright 2024 Jun Wako <wakojun@gmail.com>

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
#include <avr/interrupt.h>
#include <util/delay.h>
#include "print.h"
#include "util.h"
#include "matrix.h"
#include "debug.h"
#include "software_uart.h"
#include "led.h"
#include "host.h"


#if !defined(SUN_MOUSE_ENABLE)
#   define SUN_MOUSE_ENABLE 1
#endif

// Timer count
#define BAUD                1200
#define PRESCALE            8
#define TCNT_PER_BIT        TICK_PER_BIT(BAUD, PRESCALE)
#define TCNT_PER_0_5_BIT    TICK_PER_0_5_BIT(BAUD, PRESCALE)
#define TCNT_PER_1_5_BIT    TICK_PER_1_5_BIT(BAUD, PRESCALE)


//
// keyboard uart
//
static uint8_t kb_rb[16];
static ringbuf_t kb_rbuf = {
    .buffer = kb_rb,
    .size_mask = 15
};
static uint8_t kb_sb[16];
static ringbuf_t kb_sbuf = {
    .buffer = kb_sb,
    .size_mask = 15
};
static suart_t kb_suart = {
    .recv_pin = { &PORTD, 2 },
    .send_pin = { &PORTD, 3 },
/*
    .parity = PARITY_NONE,
    .order = ORDER_LSB,
    .logic = LOGIC_NEGATIVE,
*/
    .rbuf = &kb_rbuf,
    .sbuf = &kb_sbuf
};

static void kb_send(uint8_t data)
{
    suart_send(&kb_suart, data);
    xprintf("s%02X ", data);
}

// receive keyboard data
ISR(INT2_vect)
{
    // start bit rising edge
    // setup TIMER1_COMPB interrupt to sample at mid of bit width
    OCR1B = (TCNT1 + TCNT_PER_0_5_BIT) % TCNT_PER_BIT;

    TIFR1  |=  (1 << OCF1B);    // clear TIMER1_COMPB interrupt flag
    TIMSK1 |=  (1 << OCIE1B);   // enable TIMER1_COMPB interrupt
    EIMSK  &= ~(1 << INT2);     // disable INT2 interrupt
    EIFR   |=  (1 << INTF2);    // clear INT2 interrupt flag
}

ISR(TIMER1_COMPB_vect)
{
    suart_t _suart = kb_suart;

    static uint8_t bit = 0;
    static uint8_t data = 0;

    switch (bit) {
    case 0:
        // start bit
        bit++;
        break;
    case 1: case 2: case 3: case 4: case 5: case 6: case 7: case 8:
        // data
        data >>= 1;
        if (!PIN_STATE(_suart.recv_pin)) {
            data |= 0x80;
        }
        bit++;
        break;
    case 9:
        // stop bit
        if (!PIN_STATE(_suart.recv_pin)) {
            ringbuf_put(_suart.rbuf, data);
        }

        TIMSK1 &= ~(1 << OCIE1B);   // disable TIMER1_COMPB interrupt
        TIFR1  |=  (1 << OCF1B);    // clear TIMER1_COMPB interrupt flag
        EIFR   |=  (1 << INTF2);    // clear INT2 interrupt flag
        EIMSK  |=  (1 << INT2);     // enable INT2 interrupt
        bit = 0;
        data = 0;
        break;
    }
}

// send keyboard data
ISR(TIMER1_COMPA_vect)
{
    suart_t *_suart = &kb_suart;
    static uint8_t bit = 0;
    static uint8_t data = 0;

    switch (bit) {
    case 0:
        // start bit
        if (ringbuf_is_empty(_suart->sbuf)) return;
        data = ringbuf_get(_suart->sbuf);
        ASSERT0(_suart);
        bit++;
        break;
    case 1: case 2: case 3: case 4: case 5: case 6: case 7: case 8:
        // data
        if (data & 1) {
            ASSERT1(_suart);
        } else {
            ASSERT0(_suart);
        }
        data >>= 1;
        bit++;
        break;
    case 9:
        // stop bit
        ASSERT1(_suart);
        bit++;
        break;
    case 10:
        // done
        bit = 0;
        data = 0;
        break;
    }
}


#if SUN_MOUSE_ENABLE
//
// mouse uart
//
static uint8_t ms_buf[16];
static ringbuf_t ms_rbuf = {
    .buffer = ms_buf,
    .size_mask = 15
};
static suart_t ms_suart = {
    .recv_pin = { &PORTD, 4 },
    .send_pin = {},
/*
    .parity = PARITY_NONE,
    .order = ORDER_LSB,
    .logic = LOGIC_NEGATIVE,
*/
    .rbuf = &ms_rbuf,
    .sbuf = 0
};

// receive mouse data
ISR(INT5_vect) // INT5 is used for PD4 on atmega32u2
{
    // start bit rising edge
    // setup TIMER1_COMPC interrupt to sample at mid of bit width
    OCR1C = (TCNT1 + TCNT_PER_0_5_BIT) % TCNT_PER_BIT;

    TIFR1  |=  (1 << OCF1C);    // clear TIMER1_COMPC interrupt flag
    TIMSK1 |=  (1 << OCIE1C);   // enable TIMER1_COMPC interrupt
    EIMSK  &= ~(1 << INT5);     // disable INT5 interrupt
    EIFR   |=  (1 << INTF5);    // clear INT5 interrupt flag
}

ISR(TIMER1_COMPC_vect)
{
    suart_t _suart = ms_suart;

    static uint8_t bit = 0;
    static uint8_t data = 0;

    switch (bit) {
    case 0:
        // start bit
        bit++;
        break;
    case 1: case 2: case 3: case 4: case 5: case 6: case 7: case 8:
        // data
        data >>= 1;
        if (!PIN_STATE(_suart.recv_pin)) {
            data |= 0x80;
        }
        bit++;
        break;
    case 9:
        // stop bit
        if (!PIN_STATE(_suart.recv_pin)) {
            ringbuf_put(_suart.rbuf, data);
        }

        TIMSK1 &= ~(1 << OCIE1C);   // disable TIMER1_COMPC interrupt
        TIFR1  |=  (1 << OCF1C);    // clear TIMER1_COMPC interrupt flag
        EIFR   |=  (1 << INTF5);    // clear INT5 interrupt flag
        EIMSK  |=  (1 << INT5);     // enable INT5 interrupt
        bit = 0;
        data = 0;
        break;
    }
}
#endif


void hook_early_init(void)
{
    // Timer1
    // cofigured as CTC mode.
    // OCR1A is set to 1-bit period at startup and not updated. OCR1A is used as TOP.
    // COMPA is triggered repeatedly to send data when there is data in send queue.
    // OCR1B/OCR1C is configured dynamically to sample data at center of bits when start
    // bit is detected.((TCNT1 + 0.5-bit) % TOP)
    // COMPB/COMPC is enabled at start bit and disabled at stop bit.
    //
    //     - - -|--------|---------|---------- - - - - |--------|--------|---------|- - COMPA(send)
    //    0      ________ _________ ______ _ _ _ _ _ _  ________ ________
    //    1 ____/        \_________X______ _ _ _ _ _ _ X________X________\_______________ _ _ _ _
    //          |  start   data0     data1               data6    data7    stop
    //          |    |---------|---------|-- - - - - - - ---|---------|--------| COMPB/COMPC(receive)
    //          \_ rising edge interrupt
    //
    //                     bit period
    //            - - -|------|------|------|------|------|- - - COMPA
    //      TOP        *      *      *      *      *      *   OCR1A(keyboard send)
    //                /|     /|     /|     /|     /|     /|
    //               / |    * |    * |    * |    * |    * |   OCR1B(keyboard receive)
    //    TCNT1     /  |   /  |   /  |   /  |   /  |   /  |   OCR1C(mouse receive)
    //             /   |  /   |  /   |  /   |  /   |  /   |
    //            /    | /    | /    | /    | /    | /    |
    //           /     |/     |/     |/     |/     |/     |/
    //        0 ---------------------------------------------
    //                      |------|------|------|------|- - - COMPB/COMPC
    TCCR1A =  0x00;
    TCCR1B =  (1 << WGM12) | (1 << CS11);   // CTC, clk/8
    OCR1A  =  TCNT_PER_BIT;                 // for send isr
    TIMSK1 =  (1 << OCIE1A);                // enable TIMER1_COMPA interrupt for sending

    // keyboard init
    suart_init(&kb_suart);
    // enable interrupt on rising edge for keyboard
    EICRA  |=  (1 << ISC21) | (1 << ISC20);
    EIMSK  |=  (1 << INT2);

#if SUN_MOUSE_ENABLE
    // mouse init
    suart_init(&ms_suart);
    // enable interrupt on rising edge for mouse
    EICRB  |=  (1 << ISC51) |(1 << ISC50);
    EIMSK  |=  (1 << INT5);
#endif

    sei();
}

void hook_late_init(void)
{
}


/*
 * Sun mouse
 * https://github.com/tmk/tmk_keyboard/wiki/Sun-Keyboard-Protocol#mouse-protocol
 *
 * Data format:
 *  Bit  |  7  6  5  4  3  2  1  0
 *  -----+------------------------
 *  Byte0|  1  0  0  0  0  L  M  R
 *  Byte1| x7 x6 x5 x4 x3 x2 x1 x0
 *  Byte2| y7 y6 y5 y4 y3 y2 y1 y0
 *  Byte3| X7 X6 X5 X4 X3 X2 X1 X0
 *  Byte4| Y7 Y6 Y5 Y4 Y3 Y2 Y1 Y0
 *
 *  confirmed with 'COMPACT 1' ball mouse(FCCID: DZLNSUN1).
 *  the device seems to fail to follow movement and send wrong value when dragging it rapidly.
 *  it causes opposite direction movement in the result. encoder speed limit? if not a bug in my code.
 */
#if SUN_MOUSE_ENABLE
void hook_main_loop(void)
{
    static uint8_t n = 0;
    static report_mouse_t mouse_report = {};

    uint16_t code = suart_receive(&ms_suart);
    if (code == -1) return;
    dprintf("%02X ", code);

    switch (n) {
    case 0:
        // first byte check
        if ((code & 0xF8) != 0x80) {
            // ignore
            dprint("\r\n");
            return;
        }
        mouse_report.buttons = 0;
        if ((code & (1 << 0)) == 0) { mouse_report.buttons |= MOUSE_BTN2; }
        if ((code & (1 << 1)) == 0) { mouse_report.buttons |= MOUSE_BTN3; }
        if ((code & (1 << 2)) == 0) { mouse_report.buttons |= MOUSE_BTN1; }
        break;
    case 1:
        mouse_report.x =  ((int8_t)code < -127 ? -127 : (int8_t)code);
        break;
    case 2:
        mouse_report.y = -((int8_t)code < -127 ? -127 : (int8_t)code);
        host_mouse_send(&mouse_report);
        break;
    case 3:
        mouse_report.x =  ((int8_t)code < -127 ? -127 : (int8_t)code);
        break;
    case 4:
        mouse_report.y = -((int8_t)code < -127 ? -127 : (int8_t)code);
        if (mouse_report.x || mouse_report.y) {
            host_mouse_send(&mouse_report);
        }
        dprint("\r\n");
        break;
    }
    n = (n + 1) % 5;
}
#endif


/*
 * Sun keyboard
 *
 * scan code format: 1 byte
 *
 *  76543210
 *  Rrrrrccc
 *  ||  |\_\___ column(3-bit) 0-7
 *  |\__\______ row(4-bit) 0-0xF
 *  \__________ release flag(1:relase)
 *
 * scan code matrix:
 *
 *   +---------+
 *  0|00 ... 07|
 *  1|08 ... 0F|
 *  :|   ...   |
 *  :|   ...   |
 *  E|70 ... 77|
 *  F|78 ... 7F|
 *   +---------+
 */
static matrix_row_t matrix[MATRIX_ROWS];
#define ROW(code)      ((code >> 3) & 0xF)
#define COL(code)      (code & 0x07)

void matrix_init(void)
{
    // initialize matrix state: all keys off
    for (uint8_t i=0; i < MATRIX_ROWS; i++) matrix[i] = 0x00;
}

uint8_t matrix_scan(void)
{
    uint16_t code;
    code = suart_receive(&kb_suart);
    if (code == -1) return 0;
    xprintf("%02X ", code);

    switch (code) {
        case 0xFF:  // reset success: FF 04
            print("reset:");
            _delay_ms(500);
            code = suart_receive(&kb_suart);
            if (code != -1) xprintf("%02X ", code);
            if (code == 0x04) {
                led_set(host_keyboard_leds());
            }
            return 0;
        case 0xFE:  // layout: FE <layout>
            print("layout:");
            _delay_ms(500);
            code = suart_receive(&kb_suart);
            if (code != -1) xprintf("%02X ", code);
            return 0;
        case 0x7E:  // reset fail: 7E 01
            print("reset fail:");
            _delay_ms(500);
            code = suart_receive(&kb_suart);
            if (code != -1) xprintf("%02X ", code);
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

uint8_t matrix_get_row(uint8_t row)
{
    return matrix[row];
}


void led_set(uint8_t usb_led)
{
    uint8_t sun_led = 0;
    if (usb_led & (1<<USB_LED_NUM_LOCK))    sun_led |= (1<<0);
    if (usb_led & (1<<USB_LED_COMPOSE))     sun_led |= (1<<1);
    if (usb_led & (1<<USB_LED_SCROLL_LOCK)) sun_led |= (1<<2);
    if (usb_led & (1<<USB_LED_CAPS_LOCK))   sun_led |= (1<<3);
    kb_send(0x0E);
    kb_send(sun_led);
}


/*
 * Sun keyboard commands
 */
bool sun_bell = false;
bool sun_click = false;
bool command_extra(uint8_t code)
{
    switch (code) {
        case KC_H:
        case KC_SLASH: /* ? */
            print("\n\n--- Sun keyboard commands ---\n");
            print("Home:        Toggle Bell\n");
            print("End:         Toggle Click\n");
            print("PgUp:        LED All On\n");
            print("PgDown:      LED All Off\n");
            print("Insert:      Layout\n");
            print("Delete:      Reset\n");
            return false;
        case KC_DEL:
            print("Reset ");
            kb_send(0x01);
            break;
        case KC_HOME:
	    sun_bell = !sun_bell;
	    if (sun_bell) {
                print("Bell On ");
	        kb_send(0x02);
	    } else {
	        print("Bell Off ");
	        kb_send(0x03);
	    }
            break;
        case KC_END:
	    sun_click = !sun_click;
	    if (sun_click) {
	        print("Click On ");
		kb_send(0x0A);
	    } else {
	        print("Click Off ");
                kb_send(0x0B);
	    }
	    break;
        case KC_PGUP:
            print("LED All On ");
            kb_send(0x0E);
            kb_send(0xFF);
            break;
        case KC_PGDOWN:
            print("LED All Off ");
            kb_send(0x0E);
            kb_send(0x00);
            break;
        case KC_INSERT:
            print("Layout ");
            kb_send(0x0F);
            break;
        default:
            return false;
    }
    return true;
}
