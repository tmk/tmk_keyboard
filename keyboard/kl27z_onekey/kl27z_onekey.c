/*
Copyright 2012 Jun Wako <wakojun@gmail.com>

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

#include "ch.h"
#include "hal.h"

/*
 * scan matrix
 */
#include "print.h"
#include "debug.h"
#include "util.h"
#include "wait.h"
#include "matrix.h"
#include "led.h"
#include "keymap.h"


inline
uint8_t matrix_rows(void)
{
    return MATRIX_ROWS;
}

inline
uint8_t matrix_cols(void)
{
    return MATRIX_COLS;
}

void matrix_init(void)
{
    // internal pull-up
    palSetPadMode(GPIOA, 4, PAL_MODE_INPUT_PULLUP);

    // LED blink
    palSetPadMode(GPIOD, 7, PAL_MODE_OUTPUT_PUSHPULL);
    palSetPad(GPIOD, 7);
    chThdSleepMilliseconds(200);
    palClearPad(GPIOD, 7);
    chThdSleepMilliseconds(200);
    palSetPad(GPIOD, 7);
    chThdSleepMilliseconds(200);
    palClearPad(GPIOD, 7);
}

uint8_t matrix_scan(void)
{
    return 1;
}

inline
bool matrix_is_on(uint8_t row, uint8_t col)
{
    return !palReadPad(GPIOA, 4);
}

inline
matrix_row_t matrix_get_row(uint8_t row)
{
    return !palReadPad(GPIOA, 4);
}

void matrix_print(void)
{
}

void led_set(uint8_t usb_led) {
    if (usb_led & (1<<USB_LED_CAPS_LOCK)) {
        // output high
        palSetPadMode(GPIOD, 7, PAL_MODE_OUTPUT_PUSHPULL);
        palSetPad(GPIOD, 7);
    } else {
        // Hi-Z
        palSetPadMode(GPIOD, 7, PAL_MODE_INPUT);
    }
}


//
// Keymap
//
const uint8_t keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    {{KC_A}},
};

const action_t fn_actions[] = {
};
