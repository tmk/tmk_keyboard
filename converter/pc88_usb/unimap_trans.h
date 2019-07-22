/*
Copyright 2018 Jun Wako <wakojun@gmail.com>

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
#ifndef _TRANS_H
#define _TRANS_H

#include <stdint.h>
#include <avr/pgmspace.h>
#include "unimap.h"


/* Mapping PC88 scan codes to UNIMAP layout codes
 *
 * PC88 scan codes
 * ,---. ,---. ,------------------------. ,------------------------.  ,-------. 
 * | 60| | 61| | 62 | 63 | 64 | 65 | 66 | | 67 | 68 | 69 | 6A | 6B |  | 36| 37| 
 * `---' `---' `------------------------' `------------------------'  `-------' 
 * ,---------------------------------------------------------------.  ,-------.  ,---------------.
 * |  00 | 01| 02| 03| 04| 05| 06| 07| 08| 09| 0A| 0B| 0C| 0D| 0E  |  | 38| 39|  | 3E| 3F| 4D| 41|
 * |---------------------------------------------------------------|  `-------'  |---------------|
 * |   0F  | 10| 11| 12| 13| 14| 15| 16| 17| 18| 19| 1A| 1B|       |             | 42| 43| 44| 45|
 * |--------------------------------------------------------`  1C  |  ,-------.  |---------------|
 * |  74| 71| 1D| 1E| 1F| 20| 21| 22| 23| 24| 25| 26| 27| 28|      |  |   3A  |  | 46| 47| 48| 49|
 * |---------------------------------------------------------------|  |-------|  |---------------|
 * |    70    | 29| 2A| 2B| 2C| 2D| 2E| 2F| 30| 31| 32| 33|  78    |  | 3B| 3C|  | 4A| 4B| 4C| 4D|
 * `---------------------------------------------------------------'  |-------|  |---------------|
 *        | 72| 73|   51  |   34    |   35    | 7A| 7B|               |   3D  |  | 4E| 4F| 50| 79|
 *        `-------------------------------------------'               `-------'  `---------------'
 *
 *
 */
const uint8_t PROGMEM unimap_trans[MATRIX_ROWS][MATRIX_COLS] = {
    { UNIMAP_ESC,  UNIMAP_1,    UNIMAP_2,    UNIMAP_3,    UNIMAP_4,    UNIMAP_5,    UNIMAP_6,    UNIMAP_7     }, /* 00-07 */
    { UNIMAP_8,    UNIMAP_9,    UNIMAP_0,    UNIMAP_MINS, UNIMAP_EQL,  UNIMAP_JYEN, UNIMAP_BSPC, UNIMAP_TAB   }, /* 08-0F */
    { UNIMAP_Q,    UNIMAP_W,    UNIMAP_E,    UNIMAP_R,    UNIMAP_T,    UNIMAP_Y,    UNIMAP_U,    UNIMAP_I     }, /* 10-17 */
    { UNIMAP_O,    UNIMAP_P,    UNIMAP_LBRC, UNIMAP_RBRC, UNIMAP_ENT,  UNIMAP_A,    UNIMAP_S,    UNIMAP_D     }, /* 18-1F */
    { UNIMAP_F,    UNIMAP_G,    UNIMAP_H,    UNIMAP_J,    UNIMAP_K,    UNIMAP_L,    UNIMAP_SCLN, UNIMAP_QUOT  }, /* 20-27 */
    { UNIMAP_BSLS, UNIMAP_Z,    UNIMAP_X,    UNIMAP_C,    UNIMAP_V,    UNIMAP_B,    UNIMAP_N,    UNIMAP_M     }, /* 28-2F */
    { UNIMAP_COMM, UNIMAP_DOT,  UNIMAP_SLSH, UNIMAP_RO,   UNIMAP_SPC,  UNIMAP_HENK, UNIMAP_PGDN, UNIMAP_PGUP  }, /* 30-37 */
    { UNIMAP_INS,  UNIMAP_DEL,  UNIMAP_UP,   UNIMAP_LEFT, UNIMAP_RGHT, UNIMAP_DOWN, UNIMAP_HOME, UNIMAP_END   }, /* 38-3F */
    { UNIMAP_PMNS, UNIMAP_PSLS, UNIMAP_P7,   UNIMAP_P8,   UNIMAP_P9,   UNIMAP_PAST, UNIMAP_P4,   UNIMAP_P5    }, /* 40-47 */
    { UNIMAP_P6,   UNIMAP_PPLS, UNIMAP_P1,   UNIMAP_P2,   UNIMAP_P3,   UNIMAP_PEQL, UNIMAP_P0,   UNIMAP_PCMM  }, /* 48-4F */
    { UNIMAP_PDOT, UNIMAP_MHEN, UNIMAP_F11,  UNIMAP_F12,  UNIMAP_F13,  UNIMAP_F14,  UNIMAP_F15,  UNIMAP_NO    }, /* 50-57 */
    { UNIMAP_F21,  UNIMAP_F22,  UNIMAP_F23,  UNIMAP_F24,  UNIMAP_NO,   UNIMAP_NO,   UNIMAP_APP,  UNIMAP_NO    }, /* 58-5F */
    { UNIMAP_PAUS, UNIMAP_PSCR, UNIMAP_F1,   UNIMAP_F2,   UNIMAP_F3,   UNIMAP_F4,   UNIMAP_F5,   UNIMAP_F6    }, /* 60-67 */
    { UNIMAP_F7,   UNIMAP_F8,   UNIMAP_F9,   UNIMAP_F10,  UNIMAP_NO,   UNIMAP_NO,   UNIMAP_NO,   UNIMAP_NO    }, /* 68-6F */
    { UNIMAP_LSFT, UNIMAP_CAPS, UNIMAP_KANA, UNIMAP_LALT, UNIMAP_LCTL, UNIMAP_NO,   UNIMAP_NO,   UNIMAP_NO    }, /* 70-77 */
    { UNIMAP_RSFT, UNIMAP_PENT, UNIMAP_RGUI, UNIMAP_GRV,  UNIMAP_NO,   UNIMAP_NO,   UNIMAP_NO,   UNIMAP_NO    }  /* 78-7F */
};

#endif
