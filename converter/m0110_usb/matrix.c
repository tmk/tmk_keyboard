/*
Copyright 2011,2012 Jun Wako <wakojun@gmail.com>

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
#include <avr/io.h>
#include <util/delay.h>
#include "print.h"
#include "util.h"
#include "debug.h"
#include "host.h"
#include "led.h"
#include "m0110.h"
#include "matrix.h"


#define CAPS        0x39
#define CAPS_BREAK  (CAPS | 0x80)
#define ROW(key)    ((key)>>3&0x0F)
#define COL(key)    ((key)&0x07)


static bool is_modified = false;

// matrix state buffer(1:on, 0:off)
static uint8_t *matrix;
static uint8_t _matrix0[MATRIX_ROWS];

static void register_key(uint8_t key);


void matrix_init(void)
{
    debug_enable = true;
    m0110_init();
    // initialize matrix state: all keys off
    for (uint8_t i=0; i < MATRIX_ROWS; i++) _matrix0[i] = 0x00;
    matrix = _matrix0;

    // LED flash
    DDRD |= (1<<6); PORTD |= (1<<6);
    _delay_ms(500);
    DDRD |= (1<<6); PORTD &= ~(1<<6);

    return;
}

static bool m0110_intl = false;
static uint8_t m0110_model = 0xFF;
static uint8_t get_m0110_model(void)
{
    m0110_send(M0110_MODEL);
    return m0110_recv();
}

uint8_t matrix_scan(void)
{
    uint8_t key;

    if (m0110_model == 0xFF) {
        m0110_model = get_m0110_model();
        if (m0110_model == 0xFF) {
            return 0;
        }
        xprintf("model: %02X\n", m0110_model);
        // TODO: detect international M0110 and config m0110_intl
    }

    is_modified = false;
    key = m0110_recv_key();

    if (key == M0110_NULL) {
        return 0;
    } else if (key == M0110_ERROR) {
        m0110_model = 0xFF;
        return 0;
    } else {
        is_modified = true;
        register_key(key);
    }

    if (debug_enable) {
        print("["); phex(key); print("]\n");
    }
    return 1;
}

inline
uint8_t matrix_get_row(uint8_t row)
{
    return matrix[row];
}

static uint8_t intl_key(uint8_t key)
{
    switch (key) {
        // Intl code -> TMK matrix
        case 0x06: return 0x0A; // Non-US bslash
        case 0x07: return 0x06; // Z
        case 0x08: return 0x07; // X
        case 0x09: return 0x08; // C
        case 0x0B: return 0x09; // V
        case 0x2D: return 0x0B; // B
        case 0x2E: return 0x2D; // N
        case 0x2B: return 0x2E; // M
        case 0x2F: return 0x2B; // ,
        case 0x2C: return 0x2F; // .
        case 0x0A: return 0x2C; // /
        case 0x34: return 0x31; // Space
        case 0x31: return 0x34; // RGUI
        case 0x24: return 0x2A; // bslash
        case 0x2A: return 0x24; // Enter
    }
    return key;
}

inline
static void register_key(uint8_t key)
{
    if (m0110_intl) {
        key = (key & 0x80) | intl_key(key & 0x7F);
        dprintf("<%02X> ", key);
    }

    if (key&0x80) {
        matrix[ROW(key)] &= ~(1<<COL(key));
    } else {
        matrix[ROW(key)] |=  (1<<COL(key));
    }
}
