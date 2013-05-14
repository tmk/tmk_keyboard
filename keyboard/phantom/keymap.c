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

/* 
 * Keymap for Phantom controller
 */
#include <stdint.h>
#include <stdbool.h>
#include <avr/pgmspace.h>
#include "keycode.h"
#include "action.h"
#include "action_macro.h"
#include "report.h"
#include "host.h"
#include "print.h"
#include "debug.h"
#include "keymap.h"

// Convert physical keyboard layout to matrix array.
// This is a macro to define keymap easily in keyboard layout form.
// Use this for 7bit layout.
#define KEYMAP( \
    K5A, K5B, K5C, K5D, K5E, K5F, K5G, K5H, K5I, K5J, K5K, K5L, K5M, K5N, K5O, K5P, K5Q, K5R, \
    K4A, K4B, K4C, K4D, K4E, K4F, K4G, K4H, K4I, K4J, K4K, K4L, K4M, K4N, K4O, K4P, K4Q, K4R, \
    K3A, K3B, K3C, K3D, K3E, K3F, K3G, K3H, K3I, K3J, K3K, K3L, K3M, K3N,      K3P, K3Q, K3R, \
    K2A, K2B, K2C, K2D, K2E, K2F, K2G, K2H, K2I, K2J, K2K, K2L, K2M, K2N,      K2P, K2Q, K2R, \
    K1A, K1B, K1C, K1D, K1E, K1F, K1G, K1H, K1I, K1J, K1K, K1L, K1M, K1N,      K1P, K1Q, K1R, \
    K0A, K0B, K0C, K0D,      K0F, K0G, K0H,           K0K, K0L, K0M, K0N,      K0P, K0Q, K0R  \
) { \
/*             0         1         2         3         4         5         6         7         8         9         10        11        12        13        14        15        16   */  \
/* 0 */   { KC_##K0A, KC_##K0B, KC_##K0C, KC_##K5B, KC_##K0D, KC_##K0F, KC_##K5G, KC_##K0G, KC_##K0H, KC_NO,    KC_##K0K, KC_##K0L, KC_##K0M, KC_##K0N, KC_##K0P, KC_##K0Q, KC_##K0R}, \
/* 1 */   { KC_##K1A, KC_##K1B, KC_##K1C, KC_##K1D, KC_##K1E, KC_##K1F, KC_##K1G, KC_##K1H, KC_##K1I, KC_##K1J, KC_##K1K, KC_##K1L, KC_##K1M, KC_##K1N, KC_##K1P, KC_##K1Q, KC_##K1R}, \
/* 2 */   { KC_##K2A, KC_##K2B, KC_##K2C, KC_##K2D, KC_##K2E, KC_##K2F, KC_##K2G, KC_##K2H, KC_##K2I, KC_##K2J, KC_##K2K, KC_##K2L, KC_##K2M, KC_##K2N, KC_##K2P, KC_##K2Q, KC_##K2R}, \
/* 3 */   { KC_##K3A, KC_##K3B, KC_##K3C, KC_##K3D, KC_##K3E, KC_##K3F, KC_##K3G, KC_##K3H, KC_##K3I, KC_##K3J, KC_##K3K, KC_##K3L, KC_##K3M, KC_##K3N, KC_##K3P, KC_##K3Q, KC_##K3R}, \
/* 4 */   { KC_##K4B, KC_##K4C, KC_##K4D, KC_##K4E, KC_##K4F, KC_##K4G, KC_##K4H, KC_##K4I, KC_##K4J, KC_##K4K, KC_##K4L, KC_##K4M, KC_##K4N, KC_##K4O, KC_##K4P, KC_##K4Q, KC_##K4R}, \
/* 5 */   { KC_##K5A, KC_##K4A, KC_##K5C, KC_##K5D, KC_##K5E, KC_##K5F, KC_##K5H, KC_##K5I, KC_##K5J, KC_##K5K, KC_##K5L, KC_##K5M, KC_##K5N, KC_##K5O, KC_##K5P, KC_##K5Q, KC_##K5R}  \
}

#define KEYMAP_ANSI( \
    K5A,      K5C, K5D, K5E, K5F, K5H, K5I, K5J, K5K, K5L, K5M, K5N, K5O,      K5P, K5Q, K5R, \
    K4A, K4B, K4C, K4D, K4E, K4F, K4G, K4H, K4I, K4J, K4K, K4L, K4M, K4O,      K4P, K4Q, K4R, \
    K3A, K3B, K3C, K3D, K3E, K3F, K3G, K3H, K3I, K3J, K3K, K3L, K3M, K3N,      K3P, K3Q, K3R, \
    K2A, K2B, K2C, K2D, K2E, K2F, K2G, K2H, K2I, K2J, K2K, K2L,      K2N,                     \
    K1A,      K1C, K1D, K1E, K1F, K1G, K1H, K1I, K1J, K1K, K1L,      K1N,           K1Q,      \
    K0A, K0B, K0C,                K0G,                K0K, K0L, K0M, K0N,      K0P, K0Q, K0R  \
) KEYMAP( \
    K5A, NO,  K5C, K5D, K5E, K5F, NO,  K5H, K5I, K5J, K5K, K5L, K5M, K5N, K5O, K5P, K5Q, K5R, \
    K4A, K4B, K4C, K4D, K4E, K4F, K4G, K4H, K4I, K4J, K4K, K4L, K4M, NO,  K4O, K4P, K4Q, K4R, \
    K3A, K3B, K3C, K3D, K3E, K3F, K3G, K3H, K3I, K3J, K3K, K3L, K3M, K3N,      K3P, K3Q, K3R, \
    K2A, K2B, K2C, K2D, K2E, K2F, K2G, K2H, K2I, K2J, K2K, K2L, NO,  K2N,      NO,  NO,  NO,  \
    K1A, NO,  K1C, K1D, K1E, K1F, K1G, K1H, K1I, K1J, K1K, K1L, NO,  K1N,      NO,  K1Q, NO,  \
    K0A, K0B, K0C, NO,       NO,  K0G, NO,            K0K, K0L, K0M, K0N,      K0P, K0Q, K0R  \
)

#define KEYMAP_ANSI_150( \
    K5A,      K5C, K5D, K5E, K5F, K5H, K5I, K5J, K5K, K5L, K5M, K5N, K5O,      K5P, K5Q, K5R, \
    K4A, K4B, K4C, K4D, K4E, K4F, K4G, K4H, K4I, K4J, K4K, K4L, K4M, K4O,      K4P, K4Q, K4R, \
    K3A, K3B, K3C, K3D, K3E, K3F, K3G, K3H, K3I, K3J, K3K, K3L, K3M, K3N,      K3P, K3Q, K3R, \
    K2A, K2B, K2C, K2D, K2E, K2F, K2G, K2H, K2I, K2J, K2K, K2L,      K2N,                     \
    K1A,      K1C, K1D, K1E, K1F, K1G, K1H, K1I, K1J, K1K, K1L,      K1N,           K1Q,      \
    K0A, K0B, K0C,                K0G,                     K0L, K0M, K0N,      K0P, K0Q, K0R  \
) KEYMAP( \
    K5A, NO,  K5C, K5D, K5E, K5F, NO,  K5H, K5I, K5J, K5K, K5L, K5M, K5N, K5O, K5P, K5Q, K5R, \
    K4A, K4B, K4C, K4D, K4E, K4F, K4G, K4H, K4I, K4J, K4K, K4L, K4M, NO,  K4O, K4P, K4Q, K4R, \
    K3A, K3B, K3C, K3D, K3E, K3F, K3G, K3H, K3I, K3J, K3K, K3L, K3M, K3N,      K3P, K3Q, K3R, \
    K2A, K2B, K2C, K2D, K2E, K2F, K2G, K2H, K2I, K2J, K2K, K2L, NO,  K2N,      NO,  NO,  NO,  \
    K1A, NO,  K1C, K1D, K1E, K1F, K1G, K1H, K1I, K1J, K1K, K1L, NO,  K1N,      NO,  K1Q, NO,  \
    K0A, K0B, K0C, NO,       NO,  K0G, NO,            NO,  K0L, K0M, K0N,      K0P, K0Q, K0R  \
)

#define KEYMAP_ISO( \
    K5A,      K5C, K5D, K5E, K5F, K5H, K5I, K5J, K5K, K5L, K5M, K5N, K5O,      K5P, K5Q, K5R, \
    K4A, K4B, K4C, K4D, K4E, K4F, K4G, K4H, K4I, K4J, K4K, K4L, K4M, K4O,      K4P, K4Q, K4R, \
    K3A, K3B, K3C, K3D, K3E, K3F, K3G, K3H, K3I, K3J, K3K, K3L, K3M,           K3P, K3Q, K3R, \
    K2A, K2B, K2C, K2D, K2E, K2F, K2G, K2H, K2I, K2J, K2K, K2L, K2M, K2N,                     \
    K1A, K1B, K1C, K1D, K1E, K1F, K1G, K1H, K1I, K1J, K1K, K1L,      K1N,           K1Q,      \
    K0A, K0B, K0C,                K0G,                K0K, K0L, K0M, K0N,      K0P, K0Q, K0R  \
) KEYMAP( \
    K5A, NO,  K5C, K5D, K5E, K5F, NO,  K5H, K5I, K5J, K5K, K5L, K5M, K5N, K5O, K5P, K5Q, K5R, \
    K4A, K4B, K4C, K4D, K4E, K4F, K4G, K4H, K4I, K4J, K4K, K4L, K4M, NO,  K4O, K4P, K4Q, K4R, \
    K3A, K3B, K3C, K3D, K3E, K3F, K3G, K3H, K3I, K3J, K3K, K3L, K3M, NO,       K3P, K3Q, K3R, \
    K2A, K2B, K2C, K2D, K2E, K2F, K2G, K2H, K2I, K2J, K2K, K2L, K2M, K2N,      NO,  NO,  NO,  \
    K1A, K1B, K1C, K1D, K1E, K1F, K1G, K1H, K1I, K1J, K1K, K1L, NO,  K1N,      NO,  K1Q, NO,  \
    K0A, K0B, K0C, NO,       NO,  K0G, NO,            K0K, K0L, K0M, K0N,      K0P, K0Q, K0R  \
)

#define KEYMAP_ISO_150( \
    K5A,      K5C, K5D, K5E, K5F, K5H, K5I, K5J, K5K, K5L, K5M, K5N, K5O,      K5P, K5Q, K5R, \
    K4A, K4B, K4C, K4D, K4E, K4F, K4G, K4H, K4I, K4J, K4K, K4L, K4M, K4O,      K4P, K4Q, K4R, \
    K3A, K3B, K3C, K3D, K3E, K3F, K3G, K3H, K3I, K3J, K3K, K3L, K3M,           K3P, K3Q, K3R, \
    K2A, K2B, K2C, K2D, K2E, K2F, K2G, K2H, K2I, K2J, K2K, K2L, K2M, K2N,                     \
    K1A, K1B, K1C, K1D, K1E, K1F, K1G, K1H, K1I, K1J, K1K, K1L,      K1N,           K1Q,      \
    K0A, K0B, K0C,                K0G,                     K0L, K0M, K0N,      K0P, K0Q, K0R  \
) KEYMAP( \
    K5A, NO,  K5C, K5D, K5E, K5F, NO,  K5H, K5I, K5J, K5K, K5L, K5M, K5N, K5O, K5P, K5Q, K5R, \
    K4A, K4B, K4C, K4D, K4E, K4F, K4G, K4H, K4I, K4J, K4K, K4L, K4M, NO,  K4O, K4P, K4Q, K4R, \
    K3A, K3B, K3C, K3D, K3E, K3F, K3G, K3H, K3I, K3J, K3K, K3L, K3M, NO,       K3P, K3Q, K3R, \
    K2A, K2B, K2C, K2D, K2E, K2F, K2G, K2H, K2I, K2J, K2K, K2L, K2M, K2N,      NO,  NO,  NO,  \
    K1A, K1B, K1C, K1D, K1E, K1F, K1G, K1H, K1I, K1J, K1K, K1L, NO,  K1N,      NO,  K1Q, NO,  \
    K0A, K0B, K0C, NO,       NO,  K0G, NO,            NO,  K0L, K0M, K0N,      K0P, K0Q, K0R  \
)

static const uint8_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /* Layer 0: Default Layer
     *
     * ANSI:
     *
     * ,---.   ,---------------. ,---------------. ,---------------. ,-----------.
     * |Esc|   |F1 |F2 |F3 |F4 | |F5 |F6 |F7 |F8 | |F9 |F10|F11|F12| |PrS|ScL|Pau|
     * `---'   `---------------' `---------------' `---------------' `-----------'
     * ,-----------------------------------------------------------. ,-----------.
     * |~  |  1|  2|  3|  4|  5|  6|  7|  8|  9|  0|  -|  =|Backsp | |Ins|Hom|PgU|
     * |-----------------------------------------------------------| |-----------|
     * |Tab  |  Q|  W|  E|  R|  T|  Y|  U|  I|  O|  P|  [|  ]|    \| |Del|End|PgD|
     * |-----------------------------------------------------------| '-----------'
     * |Caps  |  A|  S|  D|  F|  G|  H|  J|  K|  L|  ;|  '|Return  |
     * |-----------------------------------------------------------|     ,---.
     * |Shift   |  Z|  X|  C|  V|  B|  N|  M|  ,|  .|  /|Shift     |     |Up |
     * |-----------------------------------------------------------| ,-----------.
     * |Ctl|Gui|Alt|             Space             |Alt|Gui|App|Ctl| |Lef|Dow|Rig|
     * `-----------------------------------------------------------' `-----------'
     */

    KEYMAP_ANSI(\
        ESC,      F1,  F2,  F3,  F4,  F5,  F6,  F7,  F8,  F9,  F10, F11, F12,       PSCR,SLCK,BRK,  \
        GRV, 1,   2,   3,   4,   5,   6,   7,   8,   9,   0,   MINS,EQL, BSPC,      INS, HOME,PGUP, \
        TAB, Q,   W,   E,   R,   T,   Y,   U,   I,   O,   P,   LBRC,RBRC,BSLS,      DEL, END, PGDN, \
        FN0, A,   S,   D,   F,   G,   H,   J,   K,   L,   SCLN,QUOT,     ENT,                       \
        LSFT,     Z,   X,   C,   V,   B,   N,   M,   COMM,DOT, SLSH,     RSFT,           UP,        \
        LCTL,LGUI,LALT,               SPC,                RALT,RGUI,APP, RCTL,      LEFT,DOWN,RGHT),

    /* Overlay 1
     *
     * ,---.   ,---------------. ,---------------. ,---------------. ,-----------.
     * |   |   |   |   |   |   | |   |   |   |   | |   |   |   |   | |   |   |Slp|
     * `---'   `---------------' `---------------' `---------------' `-----------'
     * ,-----------------------------------------------------------. ,-----------.
     * |   |   |   |   |   |   |   |   |   |   |Mut|V- |V+ |       | |   |   |   |
     * |-----------------------------------------------------------| |-----------|
     * |     |   |   |   |   |   |   |   |   |MSt|Ply|Prv|Nxt|Media| |   |   |   |
     * |-----------------------------------------------------------| '-----------'
     * |      |   |   |   |   |   |   |   |   |   |   |   |        |
     * |-----------------------------------------------------------|     ,---.
     * |        |   |   |Clc|   |   |   |   |   |   |   |Caps      |     |   |
     * |-----------------------------------------------------------| ,-----------.
     * |   |   |   |                               |   |   |   |   | |   |   |   |
     * `-----------------------------------------------------------' `-----------'
     */

    KEYMAP_ANSI(\
        TRNS,     TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,      TRNS,TRNS,SLEP, \
        TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,MUTE,VOLD,VOLU,TRNS,      TRNS,TRNS,TRNS, \
        TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,MSTP,MPLY,MPRV,MNXT,MSEL,      TRNS,TRNS,TRNS, \
        TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,     TRNS,                      \
        TRNS,     TRNS,TRNS,CALC,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,     CAPS,           TRNS,      \
        TRNS,TRNS,TRNS,               TRNS,               TRNS,TRNS,TRNS,TRNS,      TRNS,TRNS,TRNS)
};

/*
 * Fn action definition
 */
static const uint16_t PROGMEM fn_actions[] = {
    [0] = ACTION_LAYER_MOMENTARY(1)
};

#define KEYMAPS_SIZE    (sizeof(keymaps) / sizeof(keymaps[0]))
#define FN_ACTIONS_SIZE (sizeof(fn_actions) / sizeof(fn_actions[0]))

/* translates key to keycode */
uint8_t keymap_key_to_keycode(uint8_t layer, key_t key)
{
    if (layer < KEYMAPS_SIZE) {
        return pgm_read_byte(&keymaps[(layer)][(key.row)][(key.col)]);
    } else {
        // fall back to layer 0
        return pgm_read_byte(&keymaps[0][(key.row)][(key.col)]);
    }
}

/* translates Fn keycode to action */
action_t keymap_fn_to_action(uint8_t keycode)
{
    action_t action;
    if (FN_INDEX(keycode) < FN_ACTIONS_SIZE) {
        action.code = pgm_read_word(&fn_actions[FN_INDEX(keycode)]);
    } else {
        action.code = ACTION_NO;
    }
    return action;
}
