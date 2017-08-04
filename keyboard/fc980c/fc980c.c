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
#include "fc980c.h"


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
    debug_enable = true;
    debug_matrix = true;

    KEY_INIT();

    // LEDs on NumLock, CapsLock and ScrollLock(PB4, PB5, PB6)
    DDRB  |= (1<<4) | (1<<5) | (1<<6);
    PORTB |= (1<<4) | (1<<5) | (1<<6);

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
    if (usb_led & (1<<USB_LED_NUM_LOCK)) {
        PORTB |=  (1<<4);
    } else {
        PORTB &= ~(1<<4);
    }
    if (usb_led & (1<<USB_LED_CAPS_LOCK)) {
        PORTB |=  (1<<5);
    } else {
        PORTB &= ~(1<<5);
    }
    if (usb_led & (1<<USB_LED_SCROLL_LOCK)) {
        PORTB |=  (1<<6);
    } else {
        PORTB &= ~(1<<6);
    }
}


#ifdef UNIMAP_ENABLE
#include <avr/pgmspace.h>
#include "unimap.h"

const uint8_t PROGMEM unimap_trans[MATRIX_ROWS][MATRIX_COLS] = {
    { UNIMAP_LEFT, UNIMAP_RCTL, UNIMAP_RALT, UNIMAP_NO  , UNIMAP_DOWN, UNIMAP_PDOT, UNIMAP_RGHT, UNIMAP_P0  ,
      UNIMAP_X   , UNIMAP_LGUI, UNIMAP_GRV , UNIMAP_V   , UNIMAP_NO  , UNIMAP_ESC , UNIMAP_M   , UNIMAP_SPC   },
    { UNIMAP_RGUI, UNIMAP_DOT , UNIMAP_NO  , UNIMAP_NO  , UNIMAP_P1  , UNIMAP_PENT, UNIMAP_P2  , UNIMAP_P3  ,
      UNIMAP_Z   , UNIMAP_LALT, UNIMAP_LCTL, UNIMAP_C   , UNIMAP_K   , UNIMAP_NO  , UNIMAP_N   , UNIMAP_B     },
    { UNIMAP_QUOT, UNIMAP_SLSH, UNIMAP_COMM, UNIMAP_NO  , UNIMAP_P4  , UNIMAP_PPLS, UNIMAP_P5  , UNIMAP_P6  ,
      UNIMAP_D   , UNIMAP_A   , UNIMAP_LSFT, UNIMAP_F   , UNIMAP_J   , UNIMAP_F1  , UNIMAP_H   , UNIMAP_G     },
    { UNIMAP_RSFT, UNIMAP_SCLN, UNIMAP_L   , UNIMAP_RBRC, UNIMAP_UP  , UNIMAP_NO  , UNIMAP_NO  , UNIMAP_NO  ,
      UNIMAP_S   , UNIMAP_Q   , UNIMAP_CAPS, UNIMAP_R   , UNIMAP_I   , UNIMAP_F3  , UNIMAP_U   , UNIMAP_T     },
    { UNIMAP_NO  , UNIMAP_NO  , UNIMAP_NO  , UNIMAP_NO  , UNIMAP_NO  , UNIMAP_NO  , UNIMAP_NO  , UNIMAP_NO  ,
      UNIMAP_NO  , UNIMAP_NO  , UNIMAP_NO  , UNIMAP_NO  , UNIMAP_NO  , UNIMAP_NO  , UNIMAP_NO  , UNIMAP_NO    },
    { UNIMAP_EQL , UNIMAP_MINS, UNIMAP_0   , UNIMAP_BSLS, UNIMAP_NLCK, UNIMAP_BSPC, UNIMAP_PSLS, UNIMAP_PAST,
      UNIMAP_3   , UNIMAP_2   , UNIMAP_NO  , UNIMAP_4   , UNIMAP_9   , UNIMAP_F2  , UNIMAP_7   , UNIMAP_6     },
    { UNIMAP_LBRC, UNIMAP_P   , UNIMAP_O   , UNIMAP_ENT , UNIMAP_P7  , UNIMAP_PMNS, UNIMAP_P8  , UNIMAP_P9  ,
      UNIMAP_W   , UNIMAP_1   , UNIMAP_TAB , UNIMAP_E   , UNIMAP_8   , UNIMAP_F4  , UNIMAP_Y   , UNIMAP_5     },
    { UNIMAP_F11 , UNIMAP_F10 , UNIMAP_F9  , UNIMAP_F12 , UNIMAP_DEL , UNIMAP_PGDN, UNIMAP_INS , UNIMAP_PGUP,
      UNIMAP_NO  , UNIMAP_NO  , UNIMAP_NO  , UNIMAP_NO  , UNIMAP_F8  , UNIMAP_F5  , UNIMAP_F7  , UNIMAP_F6    }

};
#endif
