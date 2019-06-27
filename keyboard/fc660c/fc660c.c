/*
Copyright 2017 Jun Wako <wakojun@gmail.com>

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
 * scan matrix
 */
#include <stdint.h>
#include <stdbool.h>
#include <util/delay.h>
#include "print.h"
#include "debug.h"
#include "util.h"
#include "timer.h"
#include "matrix.h"
#include "led.h"
#include "fc660c.h"


static uint32_t matrix_last_modified = 0;

// matrix state buffer(1:on, 0:off)
static matrix_row_t *matrix;
static matrix_row_t *matrix_prev;
static matrix_row_t _matrix0[MATRIX_ROWS];
static matrix_row_t _matrix1[MATRIX_ROWS];


void matrix_init(void)
{
#if 0
    debug_enable = true;
    debug_keyboard = true;
    debug_matrix = true;
#endif

    KEY_INIT();

    // LEDs on CapsLock and Insert
    DDRB  |= (1<<5) | (1<<6);
    PORTB |= (1<<5) | (1<<6);

    // initialize matrix state: all keys off
    for (uint8_t i=0; i < MATRIX_ROWS; i++) _matrix0[i] = 0x00;
    for (uint8_t i=0; i < MATRIX_ROWS; i++) _matrix1[i] = 0x00;
    matrix = _matrix0;
    matrix_prev = _matrix1;
}

uint8_t matrix_scan(void)
{
    matrix_row_t *tmp;

    tmp = matrix_prev;
    matrix_prev = matrix;
    matrix = tmp;

    uint8_t row, col;
    for (col = 0; col < MATRIX_COLS; col++) {
        SET_COL(col);
        for (row = 0; row < MATRIX_ROWS; row++) {
            //KEY_SELECT(row, col);
            SET_ROW(row);
            _delay_us(2);

            // Not sure this is needed. This just emulates HHKB controller's behaviour.
            if (matrix_prev[row] & (1<<col)) {
                KEY_HYS_ON();
            }
            _delay_us(10);

            // NOTE: KEY_STATE is valid only in 20us after KEY_ENABLE.
            // If V-USB interrupts in this section we could lose 40us or so
            // and would read invalid value from KEY_STATE.
            uint8_t last = TIMER_RAW;

            KEY_ENABLE();

            // Wait for KEY_STATE outputs its value.
            _delay_us(2);

            if (KEY_STATE()) {
                matrix[row] &= ~(1<<col);
            } else {
                matrix[row] |= (1<<col);
            }

            // Ignore if this code region execution time elapses more than 20us.
            // MEMO: 20[us] * (TIMER_RAW_FREQ / 1000000)[count per us]
            // MEMO: then change above using this rule: a/(b/c) = a*1/(b/c) = a*(c/b)
            if (TIMER_DIFF_RAW(TIMER_RAW, last) > 20/(1000000/TIMER_RAW_FREQ)) {
                matrix[row] = matrix_prev[row];
            }

            _delay_us(5);
            KEY_HYS_OFF();
            KEY_UNABLE();

            // NOTE: KEY_STATE keep its state in 20us after KEY_ENABLE.
            // This takes 25us or more to make sure KEY_STATE returns to idle state.
            _delay_us(75);
        }
        if (matrix[row] ^ matrix_prev[row]) {
            matrix_last_modified = timer_read32();
        }
    }
    return 1;
}

inline
matrix_row_t matrix_get_row(uint8_t row)
{
    return matrix[row];
}

void led_set(uint8_t usb_led)
{
    if (usb_led & (1<<USB_LED_CAPS_LOCK)) {
        PORTB &= ~(1<<6);
    } else {
        PORTB |=  (1<<6);
    }
}


#ifdef UNIMAP_ENABLE
#include <avr/pgmspace.h>
#include "unimap.h"

const uint8_t PROGMEM unimap_trans[MATRIX_ROWS][MATRIX_COLS] = {
    { UNIMAP_Q,     UNIMAP_W,     UNIMAP_E,     UNIMAP_TAB,   UNIMAP_R,     UNIMAP_U,     UNIMAP_T,     UNIMAP_Y,
      UNIMAP_O,     UNIMAP_P,     UNIMAP_LBRC,  UNIMAP_I,     UNIMAP_RBRC,  UNIMAP_NO,    UNIMAP_BSLS,  UNIMAP_DEL   },
    { UNIMAP_1,     UNIMAP_2,     UNIMAP_3,     UNIMAP_GRV,   UNIMAP_4,     UNIMAP_7,     UNIMAP_5,     UNIMAP_6,
      UNIMAP_9,     UNIMAP_0,     UNIMAP_MINS,  UNIMAP_8,     UNIMAP_EQL,   UNIMAP_NO,    UNIMAP_BSPC,  UNIMAP_INS   },
    { UNIMAP_NO,    UNIMAP_LGUI,  UNIMAP_LALT,  UNIMAP_LCTL,  UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO,    UNIMAP_SPC,
      UNIMAP_RALT,  UNIMAP_NO,    UNIMAP_RCTL,  UNIMAP_NO,    UNIMAP_RGUI,  UNIMAP_DOWN,  UNIMAP_LEFT,  UNIMAP_RGHT  },
    { UNIMAP_NO,    UNIMAP_Z,     UNIMAP_X,     UNIMAP_LSFT,  UNIMAP_C,     UNIMAP_N,     UNIMAP_V,     UNIMAP_B,
      UNIMAP_COMM,  UNIMAP_DOT,   UNIMAP_SLSH,  UNIMAP_M,     UNIMAP_RSFT,  UNIMAP_UP,    UNIMAP_NO,    UNIMAP_NO    },
    { UNIMAP_A,     UNIMAP_S,     UNIMAP_D,     UNIMAP_CAPS,  UNIMAP_F,     UNIMAP_J,     UNIMAP_G,     UNIMAP_H,
      UNIMAP_L,     UNIMAP_SCLN,  UNIMAP_QUOT,  UNIMAP_K,     UNIMAP_NO,    UNIMAP_NO,    UNIMAP_ENT,   UNIMAP_NO    },
    { UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO,
      UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO    },
    { UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO,
      UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO    },
    { UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO,
      UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO    }
};
#endif
