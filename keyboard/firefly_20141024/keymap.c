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
#include "keymap.h"

/*
 Matrix col/row mapping

 ,----.    ,-------------------. ,-------------------. ,-------------------. ,--------------.
 |06/6|    |07/4|08/4|08/2|08/6| |15/5|11/6|12/2|12/4| |14/4|14/5|14/6|14/0| |13/5|13/7|15/7|
 `----'    `-------------------' `-------------------' `-------------------' `--------------'
 ,-------------------------------------------------------------------------. ,--------------. ,-------------------.
 |06/4|06/5|07/5|08/5|09/5|09/4|10/4|10/5|11/5|12/5|05/5|05/4|11/4|    14/2| |17/4|02/4|04/4| |16/1|17/1|04/1|04/0|
 |-------------------------------------------------------------------------| |--------------| |-------------------|
 |06/2  |06/7|07/7|08/7|09/7|09/2|10/2|10/7|11/7|12/7|05/7|05/2|11/2|  14/3| |16/4|02/5|04/5| |16/7|17/7|04/7|    |
 |-------------------------------------------------------------------------| '--------------' |-------------- 02/7|
 |02/7   |06/3|07/3|08/3|09/3|09/6|10/6|10/3|11/3|12/3|05/3|05/6|      14/1|                  |16/2|17/2|04/2|    |
 |-------------------------------------------------------------------------|      ,----.      |-------------------|
 |01/2     |06/1|07/1|08/1|09/1|09/0|10/0|10/1|11/1|12/1|05/0|         01/3|      |02/6|      |16/3|17/3|04/3|    |
 |-------------------------------------------------------------------------| ,--------------. |-------------- 02/3|
 |15/4|03/2|13/6|                 16/6                  |13/0|0/3|12/0|15/1| |02/0|16/0|17/0| | 17/6    |04/6|    |
 `-------------------------------------------------------------------------' `--------------' `-------------------'
*/

#define KEYMAP(K1, K2, K3, K4, K5, K6, K7) \
  {{KC_##K1, KC_##K2, KC_##K3, KC_##K4, KC_##K5, KC_##K6, KC_##K7}}

#include "keymap_ansi.h"

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
