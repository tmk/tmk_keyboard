/*
Copyright 2014 Ralf Schmitt <ralf@bunkertor.net>

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

#include <stdint.h>
#include <stdbool.h>
#include <avr/pgmspace.h>
#include "keycode.h"
#include "action.h"
#include "action_macro.h"
#include "report.h"
#include "host.h"
#include "debug.h"

#define KEYMAP( \
    KJ6,      KI4, KH4, KH2, KH6, KA7, KE6, KD2, KD4, KB4, KB7, KB6, KB0,   KC7, KC5, KA5, \
    KJ4, KJ7, KI7, KH7, KG7, KG4, KF4, KF7, KE7, KD7, KR7, KR4, KE4, KB2,   KL4, KO4, KQ4, \
    KJ2, KJ5, KI5, KH5, KG5, KG2, KF2, KF5, KE5, KD5, KR5, KR2, KE2, KB3,   KK4, KO7, KQ7, \
    KI2, KJ3, KI3, KH3, KG3, KG6, KF6, KF3, KE3, KD3, KR3, KR6,      KB1,                  \
    KN2, KI6, KJ1, KI1, KH1, KG1, KG0, KF0, KF1, KE1, KD1, KR0,      KN3,        KO6,      \
    KA4, KP2, KC6,                KK6,                KC0, KM3, KD0, KA1,   KO0, KK0, KL0) \
{                                                                                       \
/*      0         1         2         3         4         5         6         7   */    \
/* A */ KC_NO,    KC_##KA1, KC_NO,    KC_NO,    KC_##KA4, KC_##KA5, KC_NO,    KC_##KA7, \
/* B */ KC_##KB0, KC_##KB1, KC_##KB2, KC_##KB3, KC_##KB4, KC_NO,    KC_##KB6, KC_##KB7, \
/* C */ KC_##KC0, KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_##KC5, KC_##KC6, KC_##KC7, \
/* D */ KC_##KD0, KC_##KD1, KC_##KD2, KC_##KD3, KC_##KD4, KC_##KD5, KC_NO,    KC_##KD7, \
/* E */ KC_NO,    KC_##KE1, KC_##KE2, KC_##KE3, KC_##KE4, KC_##KE5, KC_##KE6, KC_##KE7, \
/* F */ KC_##KF0, KC_##KF1, KC_##KF2, KC_##KF3, KC_##KF4, KC_##KF5, KC_##KF6, KC_##KF7, \
/* G */ KC_##KG0, KC_##KG1, KC_##KG2, KC_##KG3, KC_##KG4, KC_##KG5, KC_##KG6, KC_##KG7, \
/* H */ KC_NO,    KC_##KH1, KC_##KH2, KC_##KH3, KC_##KH4, KC_##KH5, KC_##KH6, KC_##KH7, \
/* I */ KC_NO,    KC_##KI1, KC_##KI2, KC_##KI3, KC_##KI4, KC_##KI5, KC_##KI6, KC_##KI7, \
/* J */ KC_NO,    KC_##KJ1, KC_##KJ2, KC_##KJ3, KC_##KJ4, KC_##KJ5, KC_##KJ6, KC_##KJ7, \
/* K */ KC_##KK0, KC_NO,    KC_NO,    KC_NO,    KC_##KK4, KC_NO,    KC_##KK6, KC_NO,    \
/* L */ KC_##KL0, KC_NO,    KC_NO,    KC_NO,    KC_##KL4, KC_NO,    KC_NO,    KC_NO,    \
/* M */ KC_NO,    KC_NO,    KC_NO,    KC_##KM3, KC_NO,    KC_NO,    KC_NO,    KC_NO,    \
/* N */ KC_NO,    KC_NO,    KC_##KN2, KC_##KN3, KC_NO,    KC_NO,    KC_NO,    KC_NO,    \
/* O */ KC_##KO0, KC_NO,    KC_NO,    KC_NO,    KC_##KO4, KC_NO,    KC_##KO6, KC_##KO7, \
/* P */ KC_NO,    KC_NO,    KC_##KP2, KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,    \
/* Q */ KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_##KQ4, KC_NO,    KC_NO,    KC_##KQ7, \
/* R */ KC_##KR0, KC_NO,    KC_##KR2, KC_##KR3, KC_##KR4, KC_##KR5, KC_##KR6, KC_##KR7  \
}

#if defined(LAYOUT_CUSTOM)
    #include "keymap_custom.h"
#else
    #include "keymap.h"
#endif

#define KEYMAPS_SIZE    (sizeof(keymaps) / sizeof(keymaps[0]))
#define FN_ACTIONS_SIZE (sizeof(fn_actions) / sizeof(fn_actions[0]))

/* translates key to keycode */
uint8_t keymap_key_to_keycode(uint8_t layer, keypos_t key)
{
    if (layer < KEYMAPS_SIZE) {
        return pgm_read_byte(&keymaps[(layer)][(key.col)][(key.row)]);
    } else {
        return pgm_read_byte(&keymaps[0][(key.col)][(key.row)]);
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
