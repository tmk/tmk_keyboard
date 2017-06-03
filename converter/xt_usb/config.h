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

#ifndef CONFIG_H
#define CONFIG_H

#include <avr/interrupt.h>

#define VENDOR_ID       0xFEED
#define PRODUCT_ID      0x6512
#define DEVICE_VER      0x0001
#define MANUFACTURER    t.m.k.
#define PRODUCT         XT keyboard converter
#define DESCRIPTION     convert XT keyboard to USB


/* matrix size */
#define MATRIX_ROWS 16  // keycode bit: 3-0
#define MATRIX_COLS 8   // keycode bit: 6-4


/* key combination for command */
#define IS_COMMAND() ( \
    keyboard_report->mods == (MOD_BIT(KC_LSHIFT) | MOD_BIT(KC_RSHIFT)) || \
    keyboard_report->mods == (MOD_BIT(KC_LCTRL) | MOD_BIT(KC_RSHIFT)) \
)


//#define NO_SUSPEND_POWER_DOWN

/*
 * XT Pin interrupt
 */
#ifdef XT_USE_INT
/* uses INT1 for clock line(ATMega32U4) */
#define XT_CLOCK_PORT  PORTD
#define XT_CLOCK_PIN   PIND
#define XT_CLOCK_DDR   DDRD
#define XT_CLOCK_BIT   1
#define XT_DATA_PORT   PORTD
#define XT_DATA_PIN    PIND
#define XT_DATA_DDR    DDRD
#define XT_DATA_BIT    0
/* optional */
#ifdef XT_ZENITH_BLACK
#define XT_RST_PORT    PORTB
#define XT_RST_PIN     PINB
#define XT_RST_DDR     DDRB
#define XT_RST_BIT     7
#define XT_RST_INIT() do { \
    /* set to low */                  \
    XT_RST_PORT &= ~(1<<XT_RST_BIT);  \
    XT_RST_DDR  |=  (1<<XT_RST_BIT);  \
                                      \
    /* wait 0.2s */                   \
    _delay_ms(200);                   \
                                      \
    /* pull up */                     \
    XT_RST_DDR  &= ~(1<<XT_RST_BIT);  \
    XT_RST_PORT |=  (1<<XT_RST_BIT);  \
} while (0)
#endif /* XT_ZENITH_BLACK */
#ifdef XT_ZENITH_BLACK
#define XT_INT_INIT()  do {    \
    EICRA |= (1<<ISC11);       \
} while (0)
#else
#define XT_INT_INIT()  do {    \
    EICRA |= ((1<<ISC11) |      \
              (1<<ISC10));      \
} while (0)
#endif /* XT_ZENITH_BLACK */
#define XT_INT_ON()  do {      \
    EIMSK |= (1<<INT1);         \
} while (0)
#define XT_INT_OFF() do {      \
    EIMSK &= ~(1<<INT1);        \
} while (0)
#define XT_INT_VECT    INT1_vect
#endif

#endif
