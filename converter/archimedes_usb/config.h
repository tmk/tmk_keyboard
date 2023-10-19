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

#ifndef CONFIG_H
#define CONFIG_H

#define VENDOR_ID       0xFEED
#define PRODUCT_ID      0xACAC
#define DEVICE_VER      0x0100
#define MANUFACTURER    TMK
#define PRODUCT         Archimedes keyboard converter
#define DESCRIPTION     converts Archimedes keyboard into USB

/* matrix size */
#define MATRIX_ROWS 8
#define MATRIX_COLS 16

/* key combination for command */
#define IS_COMMAND() ( \
    keyboard_report->mods == (MOD_BIT(KC_LALT) | MOD_BIT(KC_RALT)) || \
    keyboard_report->mods == (MOD_BIT(KC_LGUI) | MOD_BIT(KC_RGUI)) || \
    keyboard_report->mods == (MOD_BIT(KC_LSHIFT) | MOD_BIT(KC_RSHIFT)) \
)

/* Software Serial configuration
 *     RX: PD1, TX: PD3
 *     asynchronous, negative logic, 31250 baud
 *     start bit(0), 8-bit data(LSB first), stop bit(1)
 */
#define SERIAL_SOFT_BAUD            31250
#define SERIAL_SOFT_PARITY_NONE
#define SERIAL_SOFT_BIT_ORDER_LSB
#define SERIAL_SOFT_LOGIC_NEGATIVE

/* debug for signal timing, see debug pin with oscilloscope */
#define SERIAL_SOFT_DEBUG_INIT()    (DDRD |= 1<<2)
#define SERIAL_SOFT_DEBUG_TGL()     (PIND |= 1<<2)

/* RXD Port */
#define SERIAL_SOFT_RXD_ENABLE
#define SERIAL_SOFT_RXD_DDR         DDRD
#define SERIAL_SOFT_RXD_PORT        PORTD
#define SERIAL_SOFT_RXD_PIN         PIND
#define SERIAL_SOFT_RXD_BIT         1
#define SERIAL_SOFT_RXD_VECT        INT1_vect

// XXX: phantom interrupt(INT1) occrus on rising edge of PD3/TXD for some reason.
/* RXD Interupt */
#ifdef SERIAL_SOFT_LOGIC_NEGATIVE
/* enable interrupt: INT1(rising edge) */
#define INTR_TRIG_EDGE   ((1<<ISC11)|(1<<ISC10))
#else
/* enable interrupt: INT1(falling edge) */
#define INTR_TRIG_EDGE   ((1<<ISC11)|(0<<ISC10))
#endif

#define SERIAL_SOFT_RXD_INIT()      do { \
    /* pin configuration: input with pull-up */ \
    SERIAL_SOFT_RXD_DDR &= ~(1<<SERIAL_SOFT_RXD_BIT); \
    SERIAL_SOFT_RXD_PORT |= (1<<SERIAL_SOFT_RXD_BIT); \
    EICRA |= INTR_TRIG_EDGE; \
    EIMSK |= (1<<INT1); \
    sei(); \
} while (0)

#define SERIAL_SOFT_RXD_INT_ENTER()

#define SERIAL_SOFT_RXD_INT_EXIT()  do { \
    /* clear interrupt  flag */ \
    EIFR = (1<<INTF1); \
} while (0)

#define SERIAL_SOFT_RXD_READ()      (SERIAL_SOFT_RXD_PIN&(1<<SERIAL_SOFT_RXD_BIT))

/* TXD Port */
#define SERIAL_SOFT_TXD_ENABLE
#define SERIAL_SOFT_TXD_DDR         DDRD
#define SERIAL_SOFT_TXD_PORT        PORTD
#define SERIAL_SOFT_TXD_PIN         PIND
#define SERIAL_SOFT_TXD_BIT         3

#define SERIAL_SOFT_TXD_HI()        do { SERIAL_SOFT_TXD_PORT |=  (1<<SERIAL_SOFT_TXD_BIT); } while (0)
#define SERIAL_SOFT_TXD_LO()        do { SERIAL_SOFT_TXD_PORT &= ~(1<<SERIAL_SOFT_TXD_BIT); } while (0)
#define SERIAL_SOFT_TXD_INIT()      do { \
    /* pin configuration: output */ \
    SERIAL_SOFT_TXD_DDR |= (1<<SERIAL_SOFT_TXD_BIT); \
    /* idle */ \
    SERIAL_SOFT_TXD_ON(); \
} while (0)

#endif //config.h
