/*
Copyright 2011,2012 Jun Wako <wakojun@gmail.com>

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

/* controller configuration */
#include "controller_teensy.h"


#define VENDOR_ID       0xFEED
#define PRODUCT_ID      0x0110
#define DEVICE_VER      0x0100
#define MANUFACTURER    t.m.k.
#define PRODUCT         M0110 keyboard converter
#define DESCRIPTION     convert M0110 keyboard to USB


/* matrix size */
#define MATRIX_ROWS 14
#define MATRIX_COLS 8

/* Locking Caps Lock support */
//#define MATRIX_HAS_LOCKING_CAPS

/* magic key */
#define IS_COMMAND() ( \
    keyboard_report->mods == (MOD_BIT(KC_LSHIFT) | MOD_BIT(KC_LALT) | MOD_BIT(KC_LGUI)) || \
    keyboard_report->mods == (MOD_BIT(KC_LSHIFT) | MOD_BIT(KC_LALT) | MOD_BIT(KC_LCTL)) \
)


/* mouse keys */
#ifdef MOUSEKEY_ENABLE
#   define MOUSEKEY_DELAY_TIME 192
#endif


/* ports */
#define M0110_CLOCK_PORT        PORTF
#define M0110_CLOCK_PIN         PINF
#define M0110_CLOCK_DDR         DDRF
#define M0110_CLOCK_BIT         0
#define M0110_DATA_PORT         PORTF
#define M0110_DATA_PIN          PINF
#define M0110_DATA_DDR          DDRF
#define M0110_DATA_BIT          1

#endif
