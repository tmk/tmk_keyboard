/*
Copyright 2011 Jun Wako <wakojun@gmail.com>

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

#define LAYERS \
    /* Layer 0: Default Layer \
     * ,-------. \
     * | 1 | 2 | \
     * |---+---| \
     * | 3 | 4 | \
     * `-------' \
     */ \
    KEYMAP(KB_1, KB_2, \
           KB_3, KB_4)

/* matrix size */
#define MATRIX_ROWS 2
#define MATRIX_COLS 2

// Convert physical keyboard layout to matrix array.
// This is a macro to define keymap easily in keyboard layout form.
#define KEYMAP( \
      R0C0, R0C1, \
      R1C0, R1C1 \
) { \
    { R0C0, R0C1 }, \
    { R1C0, R1C1 } \
}

#define VENDOR_ID       0xFEED
#define PRODUCT_ID      0xBEE0
#define MANUFACTURER    t.m.k.
#define PRODUCT         Macway mod
#define DESCRIPTION     t.m.k. keyboard firmware for Fourway mod


/* Set 0 if need no debouncing */
#define DEBOUNCE    5


/* key combination for command */
#define IS_COMMAND() ( \
    keyboard_report->mods == (MOD_BIT(KB_LSHIFT) | MOD_BIT(KB_LCTRL) | MOD_BIT(KB_LALT) | MOD_BIT(KB_LGUI)) || \
    keyboard_report->mods == (MOD_BIT(KB_LSHIFT) | MOD_BIT(KB_RSHIFT)) \
)


#endif
