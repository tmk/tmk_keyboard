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

#ifndef _HOOKS_H_
#define _HOOKS_H_

#include "keyboard.h"
#include "led.h"

/* -------------------------------------
 *       Hardware / one-off hooks
 * ------------------------------------- */

/* Called once, before initialising USB. */
/* Default behaviour: do nothing. */
void hook_init_early(void);

/* Called once, after USB is connected and keyboard initialised. */
/* Default behaviour: do nothing. */
void hook_init_late(void);

/* Called once, on getting SUSPEND event from USB. */
/* Default behaviour: enables sleep LED breathing. */
void hook_usb_suspend(void);

/* Called repeatedly during the SUSPENDed state. */
/* Default behaviour: power down and periodically check
 * the matrix, cause wakeup if needed. */
void hook_suspend_loop(void);

/* Called once, on getting WAKE event from USB. */
/* Default behaviour: disables sleep LED breathing and restores 
 * the "normal" indicator LED status by default. */
void hook_usb_wakeup(void);

/* Called once, on checking the bootmagic combos. */
/* Default behaviour: do nothing. */
void hook_bootmagic(void);

/* -------------------------------------
 *       Keyboard / periodic hooks
 * ------------------------------------- */

/* Called periodically from the matrix scan loop (very often!) */
/* Default behaviour: do nothing. */
void hook_scan_loop(void);

/* Called on matrix state change event (every keypress => often!) */
/* Default behaviour: do nothing. */
void hook_matrix_change(keyevent_t event);

/* Called on layer state change event. */
/* Default behaviour: do nothing. */
void hook_layer_state_change(uint8_t layer_state);

/* Called on default layer state change event. */
/* Default behaviour: do nothing. */
void hook_default_layer_state_change(uint8_t default_layer_state);

/* Called on tap event. */
/* Default behaviour: do nothing. */
// void hook_tap_event(void);
/* TODO */

/* Called on hold event. */
/* Default behaviour: do nothing. */
// void hook_hold_event(void);
/* TODO */

/* Called on indicator LED update event (when reported from host). */
/* Default behaviour: calls keyboard_set_leds() (for compatibility). */
void hook_led_update(uint8_t led_status);

/* Time interval-based hooks */
/* Default behaviour: do nothing. */
void hook_interval_1ms(void);
void hook_interval_10ms(void);
void hook_interval_100ms(void);
void hook_interval_1000ms(void);

#endif /* _HOOKS_H_ */