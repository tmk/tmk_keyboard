/*
Copyright 2013 Mathias Andersson <wraul@dbox.se>

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
 * Keymap for KMAC controller
 */
#include <stdint.h>
#include <stdbool.h>
#include <avr/pgmspace.h>
#include "keycode.h"
#include "action.h"
#include "action_macro.h"
#include "report.h"
#include "host.h"
#include "debug.h"
#include "keymap.h"

// Convert physical keyboard layout to matrix array.
// This is a macro to define keymap easily in keyboard layout form.
#define KEYMAP( \
    K5A,      K5C, K5D, K5E, K5F, K5G, K5H, K5I, K5J, K5K, K5L, K5M, K5N,      K5O, K5P, K5Q, \
    K4A, K4B, K4C, K4D, K4E, K4F, K4G, K4H, K4I, K4J, K4K, K4L, K4M, K4N,      K4O, K4P, K4Q, \
    K3A, K3B, K3C, K3D, K3E, K3F, K3G, K3H, K3I, K3J, K3K, K3L, K3M, K3N,      K3O, K3P, K3Q, \
    K2A, K2B, K2C, K2D, K2E, K2F, K2G, K2H, K2I, K2J, K2K, K2L,      K2N,                     \
    K1A,      K1C, K1D, K1E, K1F, K1G, K1H, K1I, K1J, K1K, K1L,      K1N,           K1P,      \
    K0A, K0B, K0C,                K0G,                K0K, K0L, K0M, K0N,      K0O, K0P, K0Q  \
) { \
/*             0         1         2         3         4         5         6         7         8         9         10        11        12        13        14        15        16   */ \
/* 5 */   { KC_##K5A, KC_NO,    KC_##K5C, KC_##K5D, KC_##K5E, KC_##K5F, KC_##K5G, KC_##K5H, KC_##K5I, KC_##K5J, KC_##K5K, KC_##K5L, KC_##K5M, KC_##K5N, KC_##K5O, KC_##K5P, KC_##K5Q}, \
/* 4 */   { KC_##K4A, KC_##K4B, KC_##K4C, KC_##K4D, KC_##K4E, KC_##K4F, KC_##K4G, KC_##K4H, KC_##K4I, KC_##K4J, KC_##K4K, KC_##K4L, KC_##K4M, KC_##K4N, KC_##K4O, KC_##K4P, KC_##K4Q}, \
/* 3 */   { KC_##K3A, KC_##K3B, KC_##K3C, KC_##K3D, KC_##K3E, KC_##K3F, KC_##K3G, KC_##K3H, KC_##K3I, KC_##K3J, KC_##K3K, KC_##K3L, KC_##K3M, KC_##K3N, KC_##K3O, KC_##K3P, KC_##K3Q}, \
/* 2 */   { KC_##K2A, KC_##K2B, KC_##K2C, KC_##K2D, KC_##K2E, KC_##K2F, KC_##K2G, KC_##K2H, KC_##K2I, KC_##K2J, KC_##K2K, KC_##K2L, KC_NO,    KC_##K2N, KC_NO,    KC_NO,    KC_NO   }, \
/* 1 */   { KC_##K1A, KC_##K1C, KC_##K1D, KC_##K1E, KC_##K1F, KC_##K1G, KC_##K1H, KC_##K1I, KC_##K1J, KC_##K1K, KC_##K1L, KC_NO,    KC_NO,    KC_##K1N, KC_NO,    KC_##K1P, KC_NO   }, \
/* 0 */   { KC_##K0A, KC_##K0B, KC_##K0C, KC_NO,    KC_NO,    KC_##K0G, KC_NO,    KC_NO,    KC_##K0K, KC_NO,    KC_##K0L, KC_NO,    KC_##K0M, KC_##K0N, KC_##K0O, KC_##K0P, KC_##K0Q} \
}

#define KEYMAP_WINKEYLESS( \
    K5A,      K5C, K5D, K5E, K5F, K5G, K5H, K5I, K5J, K5K, K5L, K5M, K5N,      K5O, K5P, K5Q, \
    K4A, K4B, K4C, K4D, K4E, K4F, K4G, K4H, K4I, K4J, K4K, K4L, K4M, K4N,      K4O, K4P, K4Q, \
    K3A, K3B, K3C, K3D, K3E, K3F, K3G, K3H, K3I, K3J, K3K, K3L, K3M, K3N,      K3O, K3P, K3Q, \
    K2A, K2B, K2C, K2D, K2E, K2F, K2G, K2H, K2I, K2J, K2K, K2L,      K2N,                     \
    K1A,      K1C, K1D, K1E, K1F, K1G, K1H, K1I, K1J, K1K, K1L,      K1N,           K1P,      \
    K0A, K0B, K0C,                K0G,                     K0L, K0M, K0N,      K0O, K0P, K0Q  \
) KEYMAP( \
    K5A,      K5C, K5D, K5E, K5F, K5G, K5H, K5I, K5J, K5K, K5L, K5M, K5N,      K5O, K5P, K5Q, \
    K4A, K4B, K4C, K4D, K4E, K4F, K4G, K4H, K4I, K4J, K4K, K4L, K4M, K4N,      K4O, K4P, K4Q, \
    K3A, K3B, K3C, K3D, K3E, K3F, K3G, K3H, K3I, K3J, K3K, K3L, K3M, K3N,      K3O, K3P, K3Q, \
    K2A, K2B, K2C, K2D, K2E, K2F, K2G, K2H, K2I, K2J, K2K, K2L,      K2N,                     \
    K1A,      K1C, K1D, K1E, K1F, K1G, K1H, K1I, K1J, K1K, K1L,      K1N,           K1P,      \
    K0A, K0B, K0C,                K0G,                K0L, K0M, NO,  K0N,      K0O, K0P, K0Q  \
)

#define KEYMAP_HAPPY( \
    R13, R18, R23, R28, R32, R41, R46, R50, R54, R58, R67, R72, R75, R79, R84, \
    R14, R19, R24, R29, R33, R42, R47, R51, R55, R59, R68, R73, R76,    R80,   \
    R36, R20, R25, R30, R34, R43, R48, R52, R56, R60, R69, R74,        R81,    \
    R15, R21, R26, R31, R35, R44, R49, R53, R57, R61, R70,        R77,     R82,\
    R16, R22, R27,           R45,                                R71, R78, R83 \
) { \
/* 4 */   { KC_##R13, KC_##R18, KC_##R23, KC_##R28, KC_##R32, KC_##R41, KC_##R46, KC_##R50, KC_##R54, KC_##R58, KC_##R67, KC_##R72, KC_##R75, KC_##R79,KC_##R84 }, \
/* 3 */   { KC_##R14, KC_##R19, KC_##R24, KC_##R29, KC_##R33, KC_##R42, KC_##R47, KC_##R51, KC_##R55, KC_##R59, KC_##R68, KC_##R73, KC_##R76, KC_##R80,KC_NO }, \
/* 2 */   { KC_##R36, KC_##R20, KC_##R25, KC_##R30, KC_##R34, KC_##R43, KC_##R48, KC_##R52, KC_##R56, KC_##R60, KC_##R69, KC_##R74, KC_NO,    KC_##R81,KC_NO }, \
/* 1 */   { KC_##R15, KC_##R21, KC_##R26, KC_##R31, KC_##R35, KC_##R44, KC_##R49, KC_##R53, KC_##R57, KC_##R61, KC_##R70, KC_NO,    KC_##R77, KC_##R82,KC_NO }, \
/* 0 */   { KC_##R16, KC_##R22, KC_##R27, KC_NO,    KC_##R36, KC_##R45, KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_##R71, KC_NO,    KC_##R78, KC_##R83,KC_NO } \
}

#if defined(LAYOUT_WINKEYLESS)
    #include "keymap_winkeyless.h"
#elif defined(LAYOUT_HAPPY)
    #include "keymap_happy.h"
#else
    #include "keymap_winkey.h"
#endif

#define KEYMAPS_SIZE    (sizeof(keymaps) / sizeof(keymaps[0]))
#define FN_ACTIONS_SIZE (sizeof(fn_actions) / sizeof(fn_actions[0]))

/* translates key to keycode */
uint8_t keymap_key_to_keycode(uint8_t layer, keypos_t key)
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
