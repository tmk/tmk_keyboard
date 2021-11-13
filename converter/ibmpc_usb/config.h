/*
Copyright 2019 Jun Wako <wakojun@gmail.com>

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

#ifndef CONFIG_H
#define CONFIG_H

#include <avr/interrupt.h>

#define VENDOR_ID       0xFEED
#define PRODUCT_ID      0x1BEE
#define DEVICE_VER      0x0201
#define MANUFACTURER    TMK
#define PRODUCT         IBM PC keyboard converter


/* matrix size */
#define MATRIX_ROWS 8
#define MATRIX_COLS 16


/* key combination for command */
#define IS_COMMAND() ( \
    keyboard_report->mods == (MOD_BIT(KC_LSHIFT) | MOD_BIT(KC_RSHIFT)) || \
    keyboard_report->mods == (MOD_BIT(KC_LALT) | MOD_BIT(KC_RALT)) \
)


// CS2 80-prefixed codes support
#define CS2_80CODE_SUPPORT

// G80-2551 terminal keyboard support
#define G80_2551_SUPPORT

// Mouse Extended Report
//#define MOUSE_EXT_REPORT


/*
 * Pin and interrupt configuration
 */
#if defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega32U2__) || defined(__AVR_AT90USB1286__)
// clock requires External Interrupt pin(INT*)
#define IBMPC_CLOCK_PORT  PORTD
#define IBMPC_CLOCK_PIN   PIND
#define IBMPC_CLOCK_DDR   DDRD
#define IBMPC_DATA_PORT   PORTD
#define IBMPC_DATA_PIN    PIND
#define IBMPC_DATA_DDR    DDRD

// primary interface
#define IBMPC_CLOCK_BIT   1
#define IBMPC_DATA_BIT    0

#define IBMPC_INT_INIT()  do {  \
    EICRA |= ((1<<ISC11) |      \
              (0<<ISC10));      \
} while (0)
#define IBMPC_INT_ON()  do {    \
    EIFR  |= (1<<INTF1);        \
    EIMSK |= (1<<INT1);         \
} while (0)
#define IBMPC_INT_OFF() do {    \
    EIMSK &= ~(1<<INT1);        \
} while (0)
#define IBMPC_INT_VECT    INT1_vect

// secondary interface
#ifdef IBMPC_SECONDARY
#define IBMPC_CLOCK_BIT1  3
#define IBMPC_DATA_BIT1   2

#define IBMPC_INT_INIT1()  do { \
    EICRA |= ((1<<ISC31) |      \
              (0<<ISC30));      \
} while (0)
#define IBMPC_INT_ON1()  do {   \
    EIFR  |= (1<<INTF3);        \
    EIMSK |= (1<<INT3);         \
} while (0)
#define IBMPC_INT_OFF1() do {   \
    EIMSK &= ~(1<<INT3);        \
} while (0)
#define IBMPC_INT_VECT1   INT3_vect
#endif


/* reset line */
#define IBMPC_RST_PORT    PORTB
#define IBMPC_RST_PIN     PINB
#define IBMPC_RST_DDR     DDRB
#define IBMPC_RST_BIT0    6
#define IBMPC_RST_BIT1    7

/* reset for XT Type-1 keyboard: low pulse for 500ms */
#define IBMPC_RST_HIZ() do { \
    IBMPC_RST_PORT &= ~(1<<IBMPC_RST_BIT0);  \
    IBMPC_RST_DDR  &= ~(1<<IBMPC_RST_BIT0);  \
    IBMPC_RST_PORT &= ~(1<<IBMPC_RST_BIT1);  \
    IBMPC_RST_DDR  &= ~(1<<IBMPC_RST_BIT1);  \
} while (0)

#define IBMPC_RST_LO() do { \
    IBMPC_RST_PORT &= ~(1<<IBMPC_RST_BIT0);  \
    IBMPC_RST_DDR  |=  (1<<IBMPC_RST_BIT0);  \
    IBMPC_RST_PORT &= ~(1<<IBMPC_RST_BIT1);  \
    IBMPC_RST_DDR  |=  (1<<IBMPC_RST_BIT1);  \
} while (0)

// for debug
#define LED_ON()    do { DDRD |= (1<<6); PORTD |=  (1<<6); } while (0)
#define LED_OFF()   do { DDRD |= (1<<6); PORTD &= ~(1<<6); } while (0)

#else
#error "No pin configuration in config.h"
#endif

#endif
