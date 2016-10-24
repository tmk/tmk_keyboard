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
#ifndef UNIMAP_TRNAS_H
#define UNIMAP_TRNAS_H

#include <stdint.h>
#include <avr/pgmspace.h>
#include "unimap.h"


const uint8_t PROGMEM unimap_trans[MATRIX_ROWS][MATRIX_COLS] = { // 16*8
#ifndef IBM4704_ALPS
    /* IBM4740 Capcitive models(62-key/77-key/107-key)
     * ,-------. ,-----------------------------------------------------------. ,-----------. ,---------------.
     * | F1| F2| |  `|  1|  2|  3|  4|  5|  6|  7|  8|  9|  0|  -|  =|JPY|BSp| |Ins|Hom|PgU| |NmL|  /|  *|  -|
     * |-------| |-----------------------------------------------------------| |-----------| |---------------|
     * | F3| F4| |Tab  |  Q|  W|  E|  R|  T|  Y|  U|  I|  O|  P|  [|  ]|    \| |Del|End|PgD| |  7|  8|  9|  +|
     * |-------| |-----------------------------------------------------------| |-----------| |---------------|
     * | F5| F6| |Caps  |  A|  S|  D|  F|  G|  H|  J|  K|  L|  ;|  '|  #|Retn| |PrS|ScL|Pau| |  4|  5|  6|KP,|
     * |-------| |-----------------------------------------------------------| |-----------| |---------------|
     * | F7| F8| |Shif|  <|  Z|  X|  C|  V|  B|  N|  M|  ,|  .|  /| RO|Shift | |F11| Up|F12| |  1|  2|  3|Ent|
     * |-------| |-----------------------------------------------------------| |-----------| |---------------|
     * | F9|F10| |Ctrl |Gui|Alt  |         Space             |Alt* |Gui|Ctrl | |Lef|Dow|Rig| |Esc|  0|KP.|KP=|
     * `-------' `-----------------------------------------------------------' `-----------' `---------------'
     * scan codes
     * ,-------. ,-----------------------------------------------------------. ,-----------. ,---------------.
     * | 46| 64| | 00| 18| 19| 1A| 10| 11| 12| 08| 09| 0A| 0F| 1F| 0D| 0C| 0E| | 6A| 6B| 6C| | 47| 48| 49| 4A|
     * |-------| |-----------------------------------------------------------| |-----------| |---------------|
     * | 56| 66| | 04  | 05| 06| 13| 14| 15| 16| 17| 01| 02| 03| 1B| 1C|   1D| | 6D| 6E| 6F| | 4B| 4C| 4D| 4E|
     * |-------| |-----------------------------------------------------------| |-----------| |---------------|
     * | 71| 77| | 20   | 21| 22| 23| 24| 25| 26| 27| 28| 29| 2A| 2B| 2C|  2D| | 70| 65| 72| | 50| 51| 52| 53|
     * |-------| |-----------------------------------------------------------| |-----------| |---------------|
     * | 44| 45| | 30 | 3E| 32| 33| 34| 35| 36| 37| 38| 39| 3A| 3B| 3C|    3D| | 74| 75| 76| | 5E| 58| 59| 5A|
     * |-------| |-----------------------------------------------------------| |-----------| |---------------|
     * | 54| 55| |  31 | 41|  3F |          40               | *3F | 42|  2F | | 78| 67| 79| | 5B| 5C| 5D| 57|
     * `-------' `-----------------------------------------------------------' `-----------' `---------------'
     * Both Alt keys spit same scan code 3F.
     */
    { UNIMAP_GRV,  UNIMAP_I,    UNIMAP_O,    UNIMAP_P,    UNIMAP_TAB,  UNIMAP_Q,    UNIMAP_W,    UNIMAP_NO   }, // 00-07
    { UNIMAP_7,    UNIMAP_8,    UNIMAP_9,    UNIMAP_NO,   UNIMAP_JYEN, UNIMAP_EQL,  UNIMAP_BSPC, UNIMAP_0    }, // 08-0F
    { UNIMAP_4,    UNIMAP_5,    UNIMAP_6,    UNIMAP_E,    UNIMAP_R,    UNIMAP_T,    UNIMAP_Y,    UNIMAP_U    }, // 10-17
    { UNIMAP_1,    UNIMAP_2,    UNIMAP_3,    UNIMAP_LBRC, UNIMAP_RBRC, UNIMAP_BSLS, UNIMAP_NO,   UNIMAP_MINS }, // 18-1F
    { UNIMAP_CAPS, UNIMAP_A,    UNIMAP_S,    UNIMAP_D,    UNIMAP_F,    UNIMAP_G,    UNIMAP_H,    UNIMAP_J    }, // 20-27
    { UNIMAP_K,    UNIMAP_L,    UNIMAP_SCLN, UNIMAP_QUOT, UNIMAP_NUHS, UNIMAP_ENT,  UNIMAP_NO,   UNIMAP_RCTL }, // 28-2F
    { UNIMAP_LSFT, UNIMAP_LCTL, UNIMAP_Z,    UNIMAP_X,    UNIMAP_C,    UNIMAP_V,    UNIMAP_B,    UNIMAP_N,   }, // 30-37
    { UNIMAP_M,    UNIMAP_COMM, UNIMAP_DOT,  UNIMAP_SLSH, UNIMAP_RO,   UNIMAP_RSFT, UNIMAP_NUBS, UNIMAP_LALT }, // 38-3F
    { UNIMAP_SPC,  UNIMAP_LGUI, UNIMAP_RGUI, UNIMAP_NO,   UNIMAP_F7,   UNIMAP_F8,   UNIMAP_F1,   UNIMAP_NLCK }, // 40-47
    { UNIMAP_PSLS, UNIMAP_PAST, UNIMAP_PMNS, UNIMAP_P7,   UNIMAP_P8,   UNIMAP_P9,   UNIMAP_PPLS, UNIMAP_NO   }, // 48-4F
    { UNIMAP_P4,   UNIMAP_P5,   UNIMAP_P6,   UNIMAP_PCMM, UNIMAP_F9,   UNIMAP_F10,  UNIMAP_F3,   UNIMAP_PEQL }, // 50-57
    { UNIMAP_P2,   UNIMAP_P3,   UNIMAP_PENT, UNIMAP_ESC,  UNIMAP_P0,   UNIMAP_PDOT, UNIMAP_P1,   UNIMAP_NO   }, // 58-5F
    { UNIMAP_NO,   UNIMAP_NO,   UNIMAP_NO,   UNIMAP_NO,   UNIMAP_F2,   UNIMAP_SLCK, UNIMAP_F4,   UNIMAP_DOWN }, // 60-67
    { UNIMAP_NO,   UNIMAP_NO,   UNIMAP_INS,  UNIMAP_HOME, UNIMAP_PGUP, UNIMAP_DEL,  UNIMAP_END,  UNIMAP_PGDN }, // 68-6F
    { UNIMAP_PSCR, UNIMAP_F5,   UNIMAP_PAUS, UNIMAP_NO,   UNIMAP_F11,  UNIMAP_UP,   UNIMAP_F12,  UNIMAP_F6   }, // 70-77
    { UNIMAP_LEFT, UNIMAP_RGHT, UNIMAP_NO,   UNIMAP_NO,   UNIMAP_NO,   UNIMAP_NO,   UNIMAP_NO,   UNIMAP_NO   }  // 78-7F
#else
    /* IBM4740 Alps models(102-key)
     * ,---------------------------------------------------------------. ,-----------. ,---------------.
     * |  `|  1|  2|  3|  4|  5|  6|  7|  8|  9|  0|  -|  =|JPY|BSp| F1| |Ins|Hom|PgU| |NmL|  /|  *|  -|
     * |---------------------------------------------------------------| |-----------| |---------------|
     * |Tab  |  Q|  W|  E|  R|  T|  Y|  U|  I|  O|  P|  [|  ]|    \| F2| |Del|End|PgD| |  7|  8|  9|  +|
     * |---------------------------------------------------------------| |-----------| |---------------|
     * |Caps  |  A|  S|  D|  F|  G|  H|  J|  K|  L|  ;|  '|  #|Retn| F3| |PrS|ScL|Pau| |  4|  5|  6|KP,|
     * |---------------------------------------------------------------| |-----------| |---------------|
     * |Shift   |  Z|  X|  C|  V|  B|  N|  M|  ,|  .|  /| RO|Shift | F4| |F11| Up|F12| |  1|  2|  3|Ent|
     * |---------------------------------------------------------------| |-----------| |---------------|
     * |Ctrl |Gui|Alt  |         Space             |Alt  |Gui|  Ctrl   | |Lef|Dow|Rig| |Esc|  0|KP.|KP=|
     * `---------------------------------------------------------------' `-----------' `---------------'
     * ,---------------------------------------------------------------. ,-----------. ,---------------.
     * | 00| 0F| 01| 02| 03| 04| 05| 06| 07| 08| 09| 0A| 0B| 0C| 0D| 0E| | 44| 45| 46| | 43| 41| 42| 4A|
     * |---------------------------------------------------------------| |-----------| |---------------|
     * |  10 | 11| 12| 13| 14| 15| 16| 17| 18| 19| 1A| 1B| 1C|  1D | 1E| | 54| 55| 56| | 50| 51| 52| 5A|
     * |---------------------------------------------------------------| |-----------| |---------------|
     * | 20   | 21| 22| 23| 24| 25| 26| 27| 28| 29| 2A| 2B| 2C| 2D | 2E| | 64| 65| 66| | 60| 61| 62| 6B|
     * |---------------------------------------------------------------| |-----------| |---------------|
     * |   30   | 32| 33| 34| 35| 36| 37| 38| 39| 3A| 3B| 3C|  3D  | 3E| | 74| 75| 76| | 70| 71| 72| 7B|
     * |---------------------------------------------------------------| |-----------| |---------------|
     * |  31 | 47|  3F |          40               | 4F  | 48|     2F  | | 77| 67| 57| | 73| 63| 53| 4E|
     * `---------------------------------------------------------------' `-----------' `---------------'
     */
    { UNIMAP_GRV,  UNIMAP_2,    UNIMAP_3,    UNIMAP_4,    UNIMAP_5,    UNIMAP_6,    UNIMAP_7,    UNIMAP_8    }, // 00-07
    { UNIMAP_9,    UNIMAP_0,    UNIMAP_MINS, UNIMAP_EQL,  UNIMAP_JYEN, UNIMAP_BSPC, UNIMAP_F1,   UNIMAP_1    }, // 08-0F
    { UNIMAP_TAB,  UNIMAP_Q,    UNIMAP_W,    UNIMAP_E,    UNIMAP_R,    UNIMAP_T,    UNIMAP_Y,    UNIMAP_U    }, // 10-17
    { UNIMAP_I,    UNIMAP_O,    UNIMAP_P,    UNIMAP_LBRC, UNIMAP_RBRC, UNIMAP_BSLS, UNIMAP_F2,   UNIMAP_NO   }, // 18-1F
    { UNIMAP_CAPS, UNIMAP_A,    UNIMAP_S,    UNIMAP_D,    UNIMAP_F,    UNIMAP_G,    UNIMAP_H,    UNIMAP_J    }, // 20-27
    { UNIMAP_K,    UNIMAP_L,    UNIMAP_SCLN, UNIMAP_QUOT, UNIMAP_NUHS, UNIMAP_ENT,  UNIMAP_F3,   UNIMAP_RCTL }, // 28-2F
    { UNIMAP_LSFT, UNIMAP_LCTL, UNIMAP_Z,    UNIMAP_X,    UNIMAP_C,    UNIMAP_V,    UNIMAP_B,    UNIMAP_N    }, // 30-37
    { UNIMAP_M,    UNIMAP_COMM, UNIMAP_DOT,  UNIMAP_SLSH, UNIMAP_RO,   UNIMAP_RSFT, UNIMAP_F4,   UNIMAP_LALT }, // 38-3F
    { UNIMAP_SPC,  UNIMAP_PSLS, UNIMAP_PAST, UNIMAP_NLCK, UNIMAP_INS,  UNIMAP_HOME, UNIMAP_PGUP, UNIMAP_LGUI }, // 40-47
    { UNIMAP_RGUI, UNIMAP_NO,   UNIMAP_PMNS, UNIMAP_NO,   UNIMAP_NO,   UNIMAP_NO,   UNIMAP_PEQL, UNIMAP_RALT }, // 48-4F
    { UNIMAP_P7,   UNIMAP_P8,   UNIMAP_P9,   UNIMAP_PDOT, UNIMAP_DEL,  UNIMAP_END,  UNIMAP_PGDN, UNIMAP_RGHT }, // 50-57
    { UNIMAP_NO,   UNIMAP_NO,   UNIMAP_PPLS, UNIMAP_NO,   UNIMAP_NO,   UNIMAP_NO,   UNIMAP_NO,   UNIMAP_NO,  }, // 58-5F
    { UNIMAP_P4,   UNIMAP_P5,   UNIMAP_P6,   UNIMAP_P0,   UNIMAP_PSCR, UNIMAP_SLCK, UNIMAP_PAUS, UNIMAP_DOWN }, // 60-67
    { UNIMAP_NO,   UNIMAP_NO,   UNIMAP_NO,   UNIMAP_PCMM, UNIMAP_NO,   UNIMAP_NO,   UNIMAP_NO,   UNIMAP_NO,  }, // 68-6F
    { UNIMAP_P1,   UNIMAP_P2,   UNIMAP_P3,   UNIMAP_ESC,  UNIMAP_F11,  UNIMAP_UP,   UNIMAP_F12,  UNIMAP_LEFT }, // 70-77
    { UNIMAP_NO,   UNIMAP_NO,   UNIMAP_NO,   UNIMAP_PENT, UNIMAP_NO,   UNIMAP_NO,   UNIMAP_NO,   UNIMAP_NO,  }  // 78-7F
#endif
};

#endif
