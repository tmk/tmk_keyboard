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
#define PRODUCT_ID      0x1bee
#define DEVICE_VER      0x0001
#define MANUFACTURER    t.m.k.
#define PRODUCT         IBM PC keyboard converter
#define DESCRIPTION     convert IBM PC keyboard to USB


/* matrix size */
#define MATRIX_ROWS 16  // keycode bit: 6-3
#define MATRIX_COLS 8   // keycode bit: 2-0


/* key combination for command */
#define IS_COMMAND() ( \
    keyboard_report->mods == (MOD_BIT(KC_LSHIFT) | MOD_BIT(KC_RSHIFT)) || \
    keyboard_report->mods == (MOD_BIT(KC_LALT) | MOD_BIT(KC_RALT)) \
)


/*
 * Pin and interrupt configuration
 */
#if defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega32U2__) || defined(__AVR_AT90USB1286__)
/* uses INT1 for clock line */
#define IBMPC_CLOCK_PORT  PORTD
#define IBMPC_CLOCK_PIN   PIND
#define IBMPC_CLOCK_DDR   DDRD
#define IBMPC_CLOCK_BIT   1
#define IBMPC_DATA_PORT   PORTD
#define IBMPC_DATA_PIN    PIND
#define IBMPC_DATA_DDR    DDRD
#define IBMPC_DATA_BIT    0
#define IBMPC_RST_PORT    PORTB
#define IBMPC_RST_PIN     PINB
#define IBMPC_RST_DDR     DDRB
#define IBMPC_RST_BIT1    6
#define IBMPC_RST_BIT2    7

/* reset for XT keyboard: low pulse for 500ms and after that HiZ for safety */
#define IBMPC_RESET() do { \
    IBMPC_RST_PORT &= ~(1<<IBMPC_RST_BIT1);  \
    IBMPC_RST_DDR  |=  (1<<IBMPC_RST_BIT1);  \
    IBMPC_RST_PORT &= ~(1<<IBMPC_RST_BIT2);  \
    IBMPC_RST_DDR  |=  (1<<IBMPC_RST_BIT2);  \
    _delay_ms(500);                   \
    IBMPC_RST_DDR  &= ~(1<<IBMPC_RST_BIT1);  \
    IBMPC_RST_DDR  &= ~(1<<IBMPC_RST_BIT2);  \
} while (0)

#define IBMPC_INT_INIT()  do {  \
    EICRA |= ((1<<ISC11) |      \
              (0<<ISC10));      \
} while (0)

#define IBMPC_INT_ON()  do {    \
    EIMSK |= (1<<INT1);         \
} while (0)

#define IBMPC_INT_OFF() do {    \
    EIMSK &= ~(1<<INT1);        \
} while (0)

#define IBMPC_INT_VECT    INT1_vect

#else
#error "No pin configuration in config.h"
#endif

#endif
