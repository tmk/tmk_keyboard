/*
Copyright 2019 Jun Wako <wakojun@gmail.com>

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
#ifndef UNIMAP_TRANS_H
#define UNIMAP_TRANS_H

#include <avr/pgmspace.h>
#include "unimap.h"


/* Acorn Archimedes A300
 * ,---.   ,-----------------------------------------------.     ,-----------.     ,-----------.
 * |Esc|   |F1 |F2 |F3 |F4 |F5 |F6 |F7 |F8 |F9 |F10|F11|F12|     |Prt|ScL|Brk|     |SW1|SW2|SW3|
 * `---'   `-----------------------------------------------'     `-----------'     `-----------'
 * ,-----------------------------------------------------------. ,-----------. ,---------------.
 * |  `|  1|  2|  3|  4|  5|  6|  7|  8|  9|  0|  -|  =|  £|Bsp| |Ins|Hom|PgU| |NmL|  /|  *|  #|
 * |-----------------------------------------------------------| |-----------| |---------------|
 * |Tab  |  Q|  W|  E|  R|  T|  Y|  U|  I|  O|  P|  [|  ]|  \  | |Del|Cpy|PgD| |  7|  8|  9|  -|
 * |-----------------------------------------------------------| `-----------' |---------------|
 * |CapsL |  A|  S|  D|  F|  G|  H|  J|  K|  L|  ;|  '|  Enter |               |  4|  5|  6|  +|
 * |-----------------------------------------------------------|     ,---.     |---------------|
 * |Shift   |  Z|  X|  C|  V|  B|  N|  M|  ,|  .|  /|   Shift  |     |Up |     |  1|  2|  3|   |
 * |-----------------------------------------------------------| ,-----------. |-----------|Ent|
 * | Ctrl|   | Alt |         Space             | Alt |   | Ctrl| |Lef|Dow|Rig| |      0|  .|   |
 * `-----'   `---------------------------------------'   `-----' `-----------' `---------------'
 *
 * ,---.   ,-----------------------------------------------.     ,-----------.     ,-----------.
 * | 00|   | 01| 02| 03| 04| 05| 06| 07| 08| 09| 0A| 0B| 0C|     | 0D| 0E| 0F|     | 70| 72| 73|
 * `---'   `-----------------------------------------------'     `-----------'     `-----------'
 * ,-----------------------------------------------------------. ,-----------. ,---------------.
 * | 10| 11| 12| 13| 14| 15| 16| 17| 18| 19| 1A| 1B| 1C| 1D| 1E| | 1F| 20| 21| | 22| 23| 24| 25|
 * |-----------------------------------------------------------| |-----------| |---------------|
 * | 26  | 27| 28| 29| 2A| 2B| 2C| 2D| 2E| 2F| 30| 31| 32|  33 | | 34| 35| 36| | 37| 38| 39| 3A|
 * |-----------------------------------------------------------| `-----------' |---------------|
 * | 3B   | 3C| 3D| 3E| 3F| 40| 41| 42| 43| 44| 45| 46|   47   |               | 48| 49| 4A| 4B|
 * |-----------------------------------------------------------|     ,---.     |---------------|
 * | 4C     | 4E| 4F| 50| 51| 52| 53| 54| 55| 56| 57|    58    |     | 59|     | 5A| 5B| 5C|   |
 * |-----------------------------------------------------------| ,-----------. |-----------| 67|
 * | 5D  |   | 5E  |         5F                |  60 |   |  61 | | 62| 63| 64| |     65| 66|   |
 * `-----'   `---------------------------------------'   `-----' `-----------' `---------------'
 */
const uint8_t PROGMEM unimap_trans[MATRIX_ROWS][MATRIX_COLS] = {
    { UNIMAP_ESC,  UNIMAP_F1,   UNIMAP_F2,   UNIMAP_F3,   UNIMAP_F4,   UNIMAP_F5,   UNIMAP_F6,   UNIMAP_F7,     /* 00-07 */
      UNIMAP_F8,   UNIMAP_F9,   UNIMAP_F10,  UNIMAP_F11,  UNIMAP_F12,  UNIMAP_PSCR, UNIMAP_SLCK, UNIMAP_PAUS }, /* 08-0F */
    { UNIMAP_GRV,  UNIMAP_1,    UNIMAP_2,    UNIMAP_3,    UNIMAP_4,    UNIMAP_5,    UNIMAP_6,    UNIMAP_7,      /* 10-17 */
      UNIMAP_8,    UNIMAP_9,    UNIMAP_0,    UNIMAP_MINS, UNIMAP_EQL,  UNIMAP_JPY,  UNIMAP_BSPC, UNIMAP_INS  }, /* 18-1F */
    { UNIMAP_HOME, UNIMAP_PGUP, UNIMAP_NLCK, UNIMAP_PSLS, UNIMAP_PAST, UNIMAP_PEQL, UNIMAP_TAB,  UNIMAP_Q,      /* 20-27 */
      UNIMAP_W,    UNIMAP_E,    UNIMAP_R,    UNIMAP_T,    UNIMAP_Y,    UNIMAP_U,    UNIMAP_I,    UNIMAP_O    }, /* 28-2F */
    { UNIMAP_P,    UNIMAP_LBRC, UNIMAP_RBRC, UNIMAP_BSLS, UNIMAP_DEL,  UNIMAP_END,  UNIMAP_PGDN, UNIMAP_P7,     /* 30-37 */
      UNIMAP_P8,   UNIMAP_P9,   UNIMAP_PMNS, UNIMAP_LCTL, UNIMAP_A,    UNIMAP_S,    UNIMAP_D,    UNIMAP_F    }, /* 38-3F */
    { UNIMAP_G,    UNIMAP_H,    UNIMAP_J,    UNIMAP_K,    UNIMAP_L,    UNIMAP_SCLN, UNIMAP_QUOT, UNIMAP_ENT,    /* 40-47 */
      UNIMAP_P4,   UNIMAP_P5,   UNIMAP_P6,   UNIMAP_PPLS, UNIMAP_LSFT, UNIMAP_NUBS, UNIMAP_Z,    UNIMAP_X    }, /* 48-4F */
    { UNIMAP_C,    UNIMAP_V,    UNIMAP_B,    UNIMAP_N,    UNIMAP_M,    UNIMAP_COMM, UNIMAP_DOT,  UNIMAP_SLSH,   /* 50-57 */
      UNIMAP_RSFT, UNIMAP_UP,   UNIMAP_P1,   UNIMAP_P2,   UNIMAP_P3,   UNIMAP_CAPS, UNIMAP_LALT, UNIMAP_SPC  }, /* 58-5F */
    { UNIMAP_RALT, UNIMAP_RCTL, UNIMAP_LEFT, UNIMAP_DOWN, UNIMAP_RIGHT,UNIMAP_P0,   UNIMAP_PDOT, UNIMAP_PENT,   /* 60-67 */
      UNIMAP_NO,   UNIMAP_NO,   UNIMAP_NO,   UNIMAP_NO,   UNIMAP_NO,   UNIMAP_NO,   UNIMAP_NO,   UNIMAP_NO   }, /* 68-6F */
    { UNIMAP_NO,   UNIMAP_NO,   UNIMAP_NO,   UNIMAP_NO,   UNIMAP_NO,   UNIMAP_NO,   UNIMAP_NO,   UNIMAP_NO,     /* 70-77 */
      UNIMAP_NO,   UNIMAP_NO,   UNIMAP_NO,   UNIMAP_NO,   UNIMAP_NO,   UNIMAP_NO,   UNIMAP_NO,   UNIMAP_NO   }, /* 78-7F */
};

#endif
