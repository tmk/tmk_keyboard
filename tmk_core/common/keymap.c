/*
Copyright 2013,2016,2020 Jun Wako <wakojun@gmail.com>

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
#include "keymap.h"
#include "report.h"
#include "keycode.h"
#include "action_layer.h"
#include "action.h"
#include "action_macro.h"
#include "wait.h"
#include "debug.h"
#include "bootloader.h"
#if defined(__AVR__)
#include <avr/pgmspace.h>
#endif

#ifdef BOOTMAGIC_ENABLE
extern keymap_config_t keymap_config;
#endif

static action_t keycode_to_action(uint8_t keycode);


/* converts key to action */
__attribute__ ((weak))
action_t action_for_key(uint8_t layer, keypos_t key)
{
    uint8_t keycode = keymap_key_to_keycode(layer, key);
    switch (keycode) {
        case KC_FN0 ... KC_FN31:
            return keymap_fn_to_action(keycode);
#ifdef BOOTMAGIC_ENABLE
        case KC_CAPSLOCK:
        case KC_LOCKING_CAPS:
            if (keymap_config.swap_control_capslock || keymap_config.capslock_to_control) {
                return keycode_to_action(KC_LCTL);
            }
            return keycode_to_action(keycode);
        case KC_LCTL:
            if (keymap_config.swap_control_capslock) {
                return keycode_to_action(KC_CAPSLOCK);
            }
            return keycode_to_action(KC_LCTL);
        case KC_LALT:
            if (keymap_config.swap_lalt_lgui) {
                if (keymap_config.no_gui) {
                    return keycode_to_action(KC_NO);
                }
                return keycode_to_action(KC_LGUI);
            }
            return keycode_to_action(KC_LALT);
        case KC_LGUI:
            if (keymap_config.swap_lalt_lgui) {
                return keycode_to_action(KC_LALT);
            }
            if (keymap_config.no_gui) {
                return keycode_to_action(KC_NO);
            }
            return keycode_to_action(KC_LGUI);
        case KC_RALT:
            if (keymap_config.swap_ralt_rgui) {
                if (keymap_config.no_gui) {
                    return keycode_to_action(KC_NO);
                }
                return keycode_to_action(KC_RGUI);
            }
            return keycode_to_action(KC_RALT);
        case KC_RGUI:
            if (keymap_config.swap_ralt_rgui) {
                return keycode_to_action(KC_RALT);
            }
            if (keymap_config.no_gui) {
                return keycode_to_action(KC_NO);
            }
            return keycode_to_action(KC_RGUI);
        case KC_GRAVE:
            if (keymap_config.swap_grave_esc) {
                return keycode_to_action(KC_ESC);
            }
            return keycode_to_action(KC_GRAVE);
        case KC_ESC:
            if (keymap_config.swap_grave_esc) {
                return keycode_to_action(KC_GRAVE);
            }
            return keycode_to_action(KC_ESC);
        case KC_BSLASH:
            if (keymap_config.swap_backslash_backspace) {
                return keycode_to_action(KC_BSPACE);
            }
            return keycode_to_action(KC_BSLASH);
        case KC_BSPACE:
            if (keymap_config.swap_backslash_backspace) {
                return keycode_to_action(KC_BSLASH);
            }
            return keycode_to_action(KC_BSPACE);
#endif
        default:
            return keycode_to_action(keycode);
    }
}


/* Macro */
__attribute__ ((weak))
const macro_t *action_get_macro(keyrecord_t *record, uint8_t id, uint8_t opt)
{
    (void)record;
    (void)id;
    (void)opt;
    return MACRO_NONE;
}

/* Function */
__attribute__ ((weak))
void action_function(keyrecord_t *record, uint8_t id, uint8_t opt)
{
    (void)record;
    (void)id;
    (void)opt;
}



/* translates keycode to action */
static action_t keycode_to_action(uint8_t keycode)
{
    switch (keycode) {
        case KC_A ... KC_EXSEL:
        case KC_LCTRL ... KC_RGUI:
            return (action_t)ACTION_KEY(keycode);
            break;
        case KC_SYSTEM_POWER ... KC_SYSTEM_WAKE:
            return (action_t)ACTION_USAGE_SYSTEM(KEYCODE2SYSTEM(keycode));
            break;
        case KC_AUDIO_MUTE ... KC_BRIGHTNESS_DEC:
            return (action_t)ACTION_USAGE_CONSUMER(KEYCODE2CONSUMER(keycode));
            break;
        case KC_MS_UP ... KC_MS_ACCEL2:
            return (action_t)ACTION_MOUSEKEY(keycode);
            break;
        case KC_TRNS:
            return (action_t)ACTION_TRANSPARENT;
            break;
        case KC_BOOTLOADER:
            return (action_t)ACTION_COMMAND(COMMAND_BOOTLOADER, 0);
            break;
        default:
            return (action_t)ACTION_NO;
            break;
    }
    return (action_t)ACTION_NO;
}



#ifdef USE_LEGACY_KEYMAP
/*
 * Legacy keymap support
 *      Consider using new keymap API instead.
 */
__attribute__ ((weak))
uint8_t keymap_get_keycode(uint8_t layer, uint8_t row, uint8_t col)
{
    return pgm_read_byte(&keymaps[(layer)][(row)][(col)]);
}

__attribute__ ((weak))
uint8_t keymap_fn_layer(uint8_t index)
{
    return pgm_read_byte(&fn_layer[index]);
}

__attribute__ ((weak))
uint8_t keymap_fn_keycode(uint8_t index)
{
    return pgm_read_byte(&fn_keycode[index]);
}

__attribute__ ((weak))
uint8_t keymap_key_to_keycode(uint8_t layer, keypos_t key)
{
    return keymap_get_keycode(layer, key.row, key.col);
}


/* Legacy keymap support */
__attribute__ ((weak))
action_t keymap_fn_to_action(uint8_t keycode)
{
    switch (keycode) {
        case KC_FN0 ... KC_FN31:
            {
                uint8_t layer = keymap_fn_layer(FN_INDEX(keycode));
                uint8_t key = keymap_fn_keycode(FN_INDEX(keycode));
                if (key) {
                    return (action_t)ACTION_LAYER_TAP_KEY(layer, key);
                } else {
                    return (action_t)ACTION_LAYER_MOMENTARY(layer);
                }
            }
            return (action_t)ACTION_NO;
        default:
            return (action_t)ACTION_NO;
    }
}

#else

__attribute__ ((weak))
uint8_t keymap_key_to_keycode(uint8_t layer, keypos_t key)
{
#if defined(__AVR__)
    return pgm_read_byte(&keymaps[(layer)][(key.row)][(key.col)]);
#else
    return keymaps[(layer)][(key.row)][(key.col)];
#endif
}

__attribute__ ((weak))
action_t keymap_fn_to_action(uint8_t keycode)
{
#if defined(__AVR__)
    return (action_t)pgm_read_word(&fn_actions[FN_INDEX(keycode)]);
#else
    return fn_actions[FN_INDEX(keycode)];
#endif
}

#endif
