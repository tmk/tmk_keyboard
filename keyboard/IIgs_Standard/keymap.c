/*
Copyright 2012 Jeffrey Sung <nattyman@gmail.com>

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

/* 
 * Keymap for Apple IIgs/Standard Keyboard
 */
#include <stdint.h>
#include <stdbool.h>
#include <avr/pgmspace.h>
#include "usb_keycodes.h"
#include "print.h"
#include "debug.h"
#include "util.h"
#include "keymap.h"


// Convert physical keyboard layout to matrix array.
// This is a macro to define keymap easily in keyboard layout form.
#define KEYMAP( R10C5, \
    R3C7, R3C6, R3C5, R3C4, R3C3, R3C2, R3C1, R8C1, R8C0, R3C0, R0C0, R0C1, R0C2, R0C3, R4C4, R4C5, R4C6, R4C7, \
    R9C7, R9C6, R9C5, R9C4, R9C3, R9C2, R9C1, R9C0, R1C0, R1C1, R1C2, R1C3, R1C4,       R2C4, R2C5, R2C6, R2C3, \
    R10C0,R7C7, R7C6, R7C5, R7C4, R7C3, R7C2, R7C1, R7C0, R0C4, R1C6, R1C7, R1C5,       R2C0, R2C1, R2C2, R2C7, \
    R10C1,R6C7, R6C6, R6C5, R6C4, R6C3, R6C2, R6C1, R6C0, R0C5, R0C6, 					R4C0, R4C1, R4C2, \
    R10C4,R10C2,R10C3,R5C4, R5C7, R5C5, R5C6, R5C0, R5C2, R0C7,							R5C1, R5C3, R4C3 \
) { \
    { R0C0, R0C1,  R0C2,  R0C3,  R0C4, R0C5,  R0C6,  R0C7 }, \
    { R1C0, R1C1,  R1C2,  R1C3,  R1C4, R1C5,  R1C6,  R1C7 }, \
    { R2C0, R2C1,  R2C2,  R2C3,  R2C4, R2C5,  R2C6,  R2C7 }, \
    { R3C0, R3C1,  R3C2,  R3C3,  R3C4, R3C5,  R3C6,  R3C7 }, \
    { R4C0, R4C1,  R4C2,  R4C3,  R4C4, R4C5,  R4C6,  R4C7 }, \
    { R5C0, R5C1,  R5C2,  R5C3,  R5C4, R5C5,  R5C6,  R5C7 }, \
    { R6C0, R6C1,  R6C2,  R6C3,  R6C4, R6C5,  R6C6,  R6C7 }, \
    { R7C0, R7C1,  R7C2,  R7C3,  R7C4, R7C5,  R7C6,  R7C7 }, \
    { R8C0, R8C1,  KB_NO, KB_NO, KB_NO,KB_NO, KB_NO, KB_NO }, \
    { R9C0, R9C1,  R9C2,  R9C3,  R9C4, R9C5,  R9C6,  R9C7 }, \
	{ R10C0,R10C1, R10C2, R10C3, R10C4,R10C5, KB_NO, KB_NO} \
}

#define KEYCODE(layer, row, col) (pgm_read_byte(&keymaps[(layer)][(row)][(col)]))


// Assign Fn key(0-7) to a layer to which switch with the Fn key pressed.
static const uint8_t PROGMEM fn_layer[] = {
    0,              // Fn0
    1,              // Fn1
    2,              // Fn2
    3,              // Fn3
    4,              // Fn4
    0,              // Fn5
    3,              // Fn6
    3               // Fn7
};

// Assign Fn key(0-7) to a keycode sent when release Fn key without use of the layer.
// See layer.c for details.
static const uint8_t PROGMEM fn_keycode[] = {
    KB_NO,          // Fn0
    KB_NO,          // Fn1
    KB_SLSH,        // Fn2
    KB_SCLN,        // Fn3
    KB_SPC,         // Fn4
    KB_NO,          // Fn5
    KB_NO,          // Fn6
    KB_NO           // Fn7
};

static const uint8_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /* Layer 0: Default Layer
     * ,-----------------------------------------------------------. ,---------------,
     * |                    POWER                                  | |               |  
     * |-----------------------------------------------------------| |---------------|
     * |Esc|  1|  2|  3|  4|  5|  6|  7|  8|  9|  0|  -|  =|Backsp | |FN0| = | / | * |
     * |-----------------------------------------------------------| |---------------|
     * |Tab  |  Q|  W|  E|  R|  T|  Y|  U|  I|  O|  P|  [|  ]|     | | 7 | 8 | 9 | + |
     * |-----------------------------------------------------'     | |---------------|
     * |Contro|  A|  S|  D|  F|  G|  H|  J|  K|  L|Fn3|  '|Return  | | 4 | 5 | 6 | - |
     * |-----------------------------------------------------------| |---------------|
     * |Shift   |  Z|  X|  C|  V|  B|  N|  M|  ,|  .|   /   |Shift | | 1 | 2 | 3 | E |
     * |-----------------------------------------------------------| |-----------| N |
     * |CAPS|Alt |Gui  |`  |SPC               |BSLS  |LFT|RGT|DN|UP| |   0   | . | T |
     * `-----------------------------------------------------------' |---------------'
     */
    KEYMAP(							KB_PWR,
		   KB_ESC, KB_1,   KB_2,   KB_3,   KB_4,   KB_5,   KB_6,   KB_7,   KB_8,   KB_9,   KB_0,    KB_MINS,KB_EQL, KB_BSPC,	KB_FN1, KB_PEQL, KB_PSLS,  KB_PAST, \
           KB_TAB, KB_Q,   KB_W,   KB_E,   KB_R,   KB_T,   KB_Y,   KB_U,   KB_I,   KB_O,   KB_P,    KB_LBRC,KB_RBRC,			KB_P7,   KB_P8,   KB_P9,   KB_PPLS, \
           KB_LCTL,KB_A,   KB_S,   KB_D,   KB_F,   KB_G,   KB_H,   KB_J,   KB_K,   KB_L,   KB_SCLN, KB_QUOT,KB_ENT,				KB_P4,   KB_P5,   KB_P6,   KB_PMNS, \
           KB_LSFT,KB_Z,   KB_X,   KB_C,   KB_V,   KB_B,   KB_N,   KB_M,   KB_COMM,KB_DOT, KB_SLSH,								KB_P1,   KB_P2,   KB_P3, \
           KB_CAPS,KB_LALT,KB_LGUI,KB_GRV, KB_SPC, KB_BSLS,KB_LEFT,KB_RGHT,KB_DOWN,KB_UP,										KB_P0,   KB_PDOT, KB_PENT),


    /* Layer 1: Tenkey use Layer
     * ,-----------------------------------------------------------. ,---------------,
     * |                    POWER                                  | |               |  
     * |-----------------------------------------------------------| |---------------|
     * |Esc|  1|  2|  3|  4|  5|  6|  7|  8|  9|  0|  -|  =|Backsp | |NLK| = | / | * |
     * |-----------------------------------------------------------| |---------------|
     * |Tab  |  Q|  W|  E|  R|  T|  Y|  U|  I|  O|  P|  [|  ]|     | |INS| 8 |PGU|V+ |
     * |-----------------------------------------------------'     | |---------------|
     * |Contro|  A|  S|  D|  F|  G|  H|  J|  K|  L|Fn3|  '|Return  | |DEL|UP |PGD|V- |
     * |-----------------------------------------------------------| |---------------|
     * |Shift   |  Z|  X|  C|  V|  B|  N|  M|  ,|  .|   /   |Shift | |LFT|DN |RGT| E |
     * |-----------------------------------------------------------| |-----------| N |
     * |CAPS|Alt |Gui  |`  |SPC               |BSLS  |LFT|RGT|DN|UP| |   0   | . | T |
     * `-----------------------------------------------------------' `---------------'
     */
    KEYMAP(						KB_PWR, \
		   KB_ESC, KB_1,   KB_2,   KB_3,   KB_4,   KB_5,   KB_6,   KB_7,   KB_8,   KB_9,   KB_0,    KB_MINS,KB_EQL, KB_BSPC, KB_FN1,   KB_PEQL, KB_PSLS, KB_MUTE, \
           KB_TAB, KB_Q,   KB_W,   KB_E,   KB_R,   KB_T,   KB_Y,   KB_U,   KB_I,   KB_O,   KB_P,    KB_LBRC,KB_RBRC,         KB_INS,   KB_P8,   KB_PGUP,   KB_VOLU, \
           KB_LCTL,KB_A,   KB_S,   KB_D,   KB_F,   KB_G,   KB_H,   KB_J,   KB_K,   KB_L,   KB_SCLN, KB_QUOT,KB_ENT,          KB_DEL,   KB_UP,   KB_PGDN,   KB_VOLD, \
           KB_LSFT,KB_Z,   KB_X,   KB_C,   KB_V,   KB_B,   KB_N,   KB_M,   KB_COMM,KB_DOT, KB_SLSH, 						 KB_LEFT,  KB_DOWN,   KB_RIGHT, \
           KB_CAPS,KB_LALT,KB_LGUI,KB_GRV, KB_SPC, KB_BSLS,KB_LEFT,KB_RGHT,KB_DOWN,KB_UP,                                    KB_P0,    KB_PDOT, KB_PENT),
};


uint8_t keymap_get_keycode(uint8_t layer, uint8_t row, uint8_t col)
{
    return KEYCODE(layer, row, col);
}

uint8_t keymap_fn_layer(uint8_t fn_bits)
{
    return pgm_read_byte(&fn_layer[biton(fn_bits)]);
}

uint8_t keymap_fn_keycode(uint8_t fn_bits)
{
    return pgm_read_byte(&fn_keycode[(biton(fn_bits))]);
}
