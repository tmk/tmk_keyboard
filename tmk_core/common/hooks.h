/*
Copyright 2016 Jun Wako <wakojun@gmail.com>

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

#include "hooks.h"

/* -------------------------------------------------
 * Definitions of hardware-independent default hooks
 * ------------------------------------------------- */

/* Called on layer state change event. */
/* Default behaviour: do nothing. */
__attribute__((weak))
void hook_layer_state_change(uint8_t layer_state) {
    (void)layer_state;
}

/* Called on default layer state change event. */
/* Default behaviour: do nothing. */
__attribute__((weak))
void hook_default_layer_state_change(uint8_t default_layer_state) {
    (void)default_layer_state;
}

/* Called periodically from the matrix scan loop (very often!) */
/* Default behaviour: do nothing. */
__attribute__((weak))
void hook_scan_loop(void) {}

/* Called on matrix state change event (every keypress => often!) */
/* Default behaviour: do nothing. */
__attribute__((weak))
void hook_matrix_change(keyevent_t event) {
	(void)event;
}

/* Called on indicator LED update event (when reported from host). */
/* Default behaviour: calls led_set (for compatibility). */
__attribute__((weak))
void hook_led_update(uint8_t led_status) {
    led_set(led_status);
}

/* Called when indicator LEDs need updating from firmware. */
/* Default behaviour: calls led_set (for compatibility). */
void led_restore_hook(uint8_t led_status);
__attribute__((weak))
void hook_led_restore(uint8_t led_status) {
    led_set(led_status);
}

/* Called once, on checking the bootmagic combos. */
/* Default behaviour: do nothing. */
__attribute__((weak))
void hook_bootmagic(void) {
	/* An example: */
	// #include "bootmagic.h"
	// #include "keymap.h"
	// if(bootmagic_scan_keycode(KC_W)) {
	// 	// do something
	// }
}