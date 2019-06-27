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
#include "timer.h"


/* Matrix
 * COL: input with pullup to sense
 *  PTD6 PTD5 PTD4 PTD3 PTA19 PTA18 PTA4 PTA2 PTA1 PTE25 PTE24 PTE30 PTE29 PTE21
 *
 * ROW: output low to strobe
 *  PTB3 PTB16 PTB17 PTC0 PTC1
 *
 * State(1:on, 0:off) 
 */
static matrix_row_t matrix[MATRIX_ROWS];
static matrix_row_t matrix_debouncing[MATRIX_ROWS];
static bool debouncing = false;
static uint16_t debouncing_time = 0;


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
    debug_matrix = true;
    memset(matrix, 0, MATRIX_ROWS);
    memset(matrix_debouncing, 0, MATRIX_ROWS);

    // COL: internal pull-up
    palSetPadMode(GPIOD, 6,  PAL_MODE_INPUT_PULLUP);
    palSetPadMode(GPIOD, 5,  PAL_MODE_INPUT_PULLUP);
    palSetPadMode(GPIOD, 4,  PAL_MODE_INPUT_PULLUP);
    palSetPadMode(GPIOD, 3,  PAL_MODE_INPUT_PULLUP);

    palSetPadMode(GPIOA, 19, PAL_MODE_INPUT_PULLUP);
    palSetPadMode(GPIOA, 18, PAL_MODE_INPUT_PULLUP);
    palSetPadMode(GPIOA, 4,  PAL_MODE_INPUT_PULLUP);
    palSetPadMode(GPIOA, 2,  PAL_MODE_INPUT_PULLUP);
    palSetPadMode(GPIOA, 1,  PAL_MODE_INPUT_PULLUP);

    palSetPadMode(GPIOE, 25, PAL_MODE_INPUT_PULLUP);
    palSetPadMode(GPIOE, 24, PAL_MODE_INPUT_PULLUP);
    palSetPadMode(GPIOE, 30, PAL_MODE_INPUT_PULLUP);
    palSetPadMode(GPIOE, 29, PAL_MODE_INPUT_PULLUP);
    palSetPadMode(GPIOE, 21, PAL_MODE_INPUT_PULLUP);

    // ROW: Output high
    palSetPadMode(GPIOB, 3,  PAL_MODE_OUTPUT_PUSHPULL);
    palSetPadMode(GPIOB, 16, PAL_MODE_OUTPUT_PUSHPULL);
    palSetPadMode(GPIOB, 17, PAL_MODE_OUTPUT_PUSHPULL);
    palSetPadMode(GPIOC, 0,  PAL_MODE_OUTPUT_PUSHPULL);
    palSetPadMode(GPIOC, 1,  PAL_MODE_OUTPUT_PUSHPULL);
    palSetPad(GPIOB, 3);
    palSetPad(GPIOB, 16);
    palSetPad(GPIOB, 17);
    palSetPad(GPIOC, 0);
    palSetPad(GPIOC, 1);


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
    for (int row = 0; row < MATRIX_ROWS; row++) {
        matrix_row_t data = 0;

        // strobe row
        switch (row) {
            case 0: palClearPad(GPIOB, 3);    break;
            case 1: palClearPad(GPIOB, 16);   break;
            case 2: palClearPad(GPIOB, 17);   break;
            case 3: palClearPad(GPIOC, 0);    break;
            case 4: palClearPad(GPIOC, 1);    break;
        }

        //wait_us(1);
        //chThdSleepMicroseconds(1); // TODO: sleep around 1ms for some reason
        //chThdSleepMilliseconds(1); // seems to work correctly

        data = (!palReadPad(GPIOD, 6)  << 0UL)  |
               (!palReadPad(GPIOD, 5)  << 1UL)  |
               (!palReadPad(GPIOD, 4)  << 2UL)  |
               (!palReadPad(GPIOD, 3)  << 3UL)  |
               (!palReadPad(GPIOA, 19) << 4UL)  |
               (!palReadPad(GPIOA, 18) << 5UL)  |
               (!palReadPad(GPIOA, 4)  << 6UL)  |
               (!palReadPad(GPIOA, 2)  << 7UL)  |
               (!palReadPad(GPIOA, 1)  << 8UL)  |
               (!palReadPad(GPIOE, 25) << 9UL)  |
               (!palReadPad(GPIOE, 24) << 10UL) |
               (!palReadPad(GPIOE, 30) << 11UL) |
               (!palReadPad(GPIOE, 29) << 12UL) |
               (!palReadPad(GPIOE, 21) << 13UL);

        // unstrobe row
        switch (row) {
            case 0: palSetPad(GPIOB, 3);    break;
            case 1: palSetPad(GPIOB, 16);   break;
            case 2: palSetPad(GPIOB, 17);   break;
            case 3: palSetPad(GPIOC, 0);    break;
            case 4: palSetPad(GPIOC, 1);    break;
        }

        if (matrix_debouncing[row] != data) {
            matrix_debouncing[row] = data;
            debouncing = true;
            debouncing_time = timer_read();
        }
    }

    if (debouncing && timer_elapsed(debouncing_time) > DEBOUNCE) {
        for (int row = 0; row < MATRIX_ROWS; row++) {
            matrix[row] = matrix_debouncing[row];
        }
        debouncing = false;
    }
    return 1;
}

inline
bool matrix_is_on(uint8_t row, uint8_t col)
{
    return (matrix[row] & (1<<col));
}

inline
matrix_row_t matrix_get_row(uint8_t row)
{
    return matrix[row];
}

void matrix_print(void)
{
    return;
    xprintf("\nr/c 0123456789ABCDEF\n");
    for (uint8_t row = 0; row < MATRIX_ROWS; row++) {
        xprintf("%02X: ", row);
        matrix_row_t data = matrix_get_row(row);
        for (int col = 0; col < MATRIX_COLS; col++) {
            if (data & (1<<col))
                xprintf("1");
            else
                xprintf("0");
        }
        xprintf("\n");
    }
}


//
// LED
//
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
    [0] = {
        { KC_ESC,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,     KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_BSLS },
        { KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,     KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_BSPC },
        { KC_LCTL, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,     KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT, KC_ENT,  KC_NO   },
        { KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,     KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT, KC_NO,   KC_FN0  },
        { KC_LCTL, KC_LGUI, KC_LALT, KC_NO,   KC_SPC,  KC_NO,   KC_NO,    KC_NO,   KC_NLCK, KC_CAPS, KC_MENU, KC_RALT, KC_RGUI, KC_RCTL },
    },
    [1] = {
        { KC_ESC,  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,    KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  KC_DEL  },
        { KC_CAPS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,  KC_TRNS, KC_PSCR, KC_SLCK, KC_PAUS, KC_UP,   KC_INS,  KC_TRNS },
        { KC_TRNS, KC_VOLD, KC_VOLU, KC_MUTE, KC_TRNS, KC_TRNS, KC_PAST,  KC_PSLS, KC_HOME, KC_PGUP, KC_LEFT, KC_RGHT, KC_PENT, KC_TRNS },
        { KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_PPLS,  KC_PMNS, KC_END,  KC_PGDN, KC_DOWN, KC_TRNS, KC_TRNS, KC_TRNS },
        { KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS },
    },
};

const action_t fn_actions[] = {
    [0]  = ACTION_LAYER_TAP_KEY(1, KC_GRV),
};
