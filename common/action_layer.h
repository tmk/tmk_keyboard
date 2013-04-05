/*
Copyright 2013 Jun Wako <wakojun@gmail.com>

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
#ifndef ACTION_LAYER_H
#define ACTION_LAYER_H

#include <stdint.h>
#include "keyboard.h"
#include "action.h"


/*
 * Default Layer
 */
extern uint32_t default_layer_state;
void default_layer_debug(void);
void default_layer_set(uint8_t layer);

#ifndef NO_ACTION_LAYER
/* bitwise operation */
void default_layer_or(uint32_t state);
void default_layer_and(uint32_t state);
void default_layer_xor(uint32_t state);
#else
#define default_layer_or(state)
#define default_layer_and(state)
#define default_layer_xor(state)
#endif


/*
 * Keymap Layer
 */
#ifndef NO_ACTION_LAYER
extern uint32_t layer_state;
void layer_debug(void);
void layer_clear(void);
void layer_move(uint8_t layer);
void layer_on(uint8_t layer);
void layer_off(uint8_t layer);
void layer_invert(uint8_t layer);
/* bitwise operation */
void layer_or(uint32_t state);
void layer_and(uint32_t state);
void layer_xor(uint32_t state);
#else
#define layer_state             0
#define layer_clear()
#define layer_move(layer)
#define layer_on(layer)
#define layer_off(layer)
#define layer_invert(layer)

#define layer_or(state)
#define layer_and(state)
#define layer_xor(state)
#define layer_debug()
#endif


/* return action depending on current layer status */
action_t layer_switch_get_action(key_t key);

#endif
