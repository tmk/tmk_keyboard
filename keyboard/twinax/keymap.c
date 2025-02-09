/*
 * Keymap for IBM Twinax keyboard(1394312, german version)
 * (C) Copyright 2019
 * Dirk Eibach <dirk.eibach@gmail.com>
 *
 * Based on Model M support by Les Orchard.
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdint.h>
#include <stdbool.h>
#include "keycode.h"
#include "action.h"
#include "action_macro.h"
#include "report.h"
#include "host.h"
#include "debug.h"
#include "keymap.h"

#define KEYMAP( \
                  K0J,K1J,K1K,K2J,K3J,K3K,K4J,K5J,K5K,K6J,K7J,K7K,                                          \
                  K0K,K0L,K1L,K2K,K2L,K3L,K4K,K4L,K5L,K6K,K6L,K7L,                                          \
                                                                                                            \
    K3B,K3C,  K3E, K2E,K2F,K2G,K2H,K3H,K3I,K2I,K2M,K2N,K2O,K3O,K3M,    K3P,  K3Q,K3R,K2Q,  K2P,K2R,K2S,K2A, \
    K2B,K4B,  K4C, K4E,K4F,K4G,K4H,K5H,K5I,K4I,K4M,K4N,K4O,K5O,K5M,          K1Q,K5Q,K4Q,  K4P,K4R,K4S,K4A, \
    K5B,K5C,  K6C, K1E,K1F,K1G,K1H,K0H,K0I,K1I,K1M,K1N,K1O,K0O,K6O,    K6P,      K0Q,      K1P,K1R,K1S,K1A, \
    K1B,K1C,  K7D, K7E,K6E,K6F,K6G,K6H,K7H,K7I,K6I,K6M,K6N,K7O,        K6D,  K6A,K0T,K1T,  K6B,K6R,K6S,K7T, \
    K0C,K0B,  K0A,    K7A,            K7B,                    K0D,     K7C,      K0P,          K7R,K7S      \
) { \
    { KC_##K0A, KC_##K1A, KC_##K2A, KC_NO,    KC_##K4A, KC_NO,    KC_##K6A, KC_##K7A }, \
    { KC_##K0B, KC_##K1B, KC_##K2B, KC_##K3B, KC_##K4B, KC_##K5B, KC_##K6B, KC_##K7B }, \
    { KC_##K0C, KC_##K1C, KC_NO,    KC_##K3C, KC_##K4C, KC_##K5C, KC_##K6C, KC_##K7C }, \
    { KC_##K0D, KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_##K6D, KC_##K7D }, \
    { KC_NO,    KC_##K1E, KC_##K2E, KC_##K3E, KC_##K4E, KC_NO,    KC_##K6E, KC_##K7E }, \
    { KC_NO,    KC_##K1F, KC_##K2F, KC_NO,    KC_##K4F, KC_NO,    KC_##K6F, KC_NO    }, \
    { KC_NO,    KC_##K1G, KC_##K2G, KC_NO,    KC_##K4G, KC_NO,    KC_##K6G, KC_NO    }, \
    { KC_##K0H, KC_##K1H, KC_##K2H, KC_##K3H, KC_##K4H, KC_##K5H, KC_##K6H, KC_##K7H }, \
    { KC_##K0I, KC_##K1I, KC_##K2I, KC_##K3I, KC_##K4I, KC_##K5I, KC_##K6I, KC_##K7I }, \
    { KC_##K0J, KC_##K1J, KC_##K2J, KC_##K3J, KC_##K4J, KC_##K5J, KC_##K6J, KC_##K7J }, \
    { KC_##K0K, KC_##K1K, KC_##K2K, KC_##K3K, KC_##K4K, KC_##K5K, KC_##K6K, KC_##K7K }, \
    { KC_##K0L, KC_##K1L, KC_##K2L, KC_##K3L, KC_##K4L, KC_##K5L, KC_##K6L, KC_##K7L }, \
    { KC_NO,    KC_##K1M, KC_##K2M, KC_##K3M, KC_##K4M, KC_##K5M, KC_##K6M, KC_NO    }, \
    { KC_NO,    KC_##K1N, KC_##K2N, KC_NO,    KC_##K4N, KC_NO,    KC_##K6N, KC_NO    }, \
    { KC_##K0O, KC_##K1O, KC_##K2O, KC_##K3O, KC_##K4O, KC_##K5O, KC_##K6O, KC_##K7O }, \
    { KC_##K0P, KC_##K1P, KC_##K2P, KC_##K3P, KC_##K4P, KC_NO,    KC_##K6P, KC_NO    }, \
    { KC_##K0Q, KC_##K1Q, KC_##K2Q, KC_##K3Q, KC_##K4Q, KC_##K5Q, KC_NO,    KC_NO    }, \
    { KC_NO,    KC_##K1R, KC_##K2R, KC_##K3R, KC_##K4R, KC_NO,    KC_##K6R, KC_##K7R }, \
    { KC_NO,    KC_##K1S, KC_##K2S, KC_NO,    KC_##K4S, KC_NO,    KC_##K6S, KC_##K7S }, \
    { KC_##K0T, KC_##K1T, KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_##K7T }, \
}

const uint8_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    KEYMAP(
                         F13, F14, F15, F16, F17, F18, F19, F20, F21, F22, F23, F24,
                         F1,  F2,  F3,  F4,  F5,  F6,  F7,  F8,  F9,  F10, F11, F12,

        PSCR,ESC,   GRV, 1,   2,   3,   4,   5,   6,   7,   8,   9,   0,   MINS,EQL,      BSPC,  INS, HOME,PGUP,  NLCK,PSLS,PAST,PMNS,
        SLCK,INT4,  TAB, Q,   W,   E,   R,   T,   Y,   U,   I,   O,   P,   LBRC,RBRC,            DEL, END, PGDN,  P7,  P8,  P9,  PPLS,
        PAUS,INT5,  CAPS,A,   S,   D,   F,   G,   H,   J,   K,   L,   SCLN,QUOT,BSLS,     ENT,        UP,         P4,  P5,  P6,  PCMM,
        APP, INT6,  LSFT,NUBS,Z,   X,   C,   V,   B,   N,   M,   COMM,DOT, SLSH,          RSFT,  LEFT,INT2,RGHT,  P1,  P2,  P3,  PENT,
        RGUI,LGUI,  LCTL,     LALT,               SPC,                          RALT,     RCTL,       DOWN,            P0,  PDOT
    ),
};

#define KEYCODE(layer, row, col) (pgm_read_byte(&keymaps[(layer)][(row)][(col)]))

const action_t PROGMEM fn_actions[] = {};

#define KEYMAPS_SIZE    (sizeof(keymaps) / sizeof(keymaps[0]))

uint8_t keymap_get_keycode(uint8_t layer, uint8_t row, uint8_t col)
{
    return KEYCODE(layer, row, col);
}
