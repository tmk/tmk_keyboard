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
#ifndef UNIMAP_TRANS_H
#define UNIMAP_TRANS_H

#include <stdint.h>
#include <avr/pgmspace.h>
#include "unimap.h"


/* X68000 Keyboard Scan codes to Unimap
    ,---. ,---.    ,-------------------,    ,-------------------.  ,-----------. ,---------------.
    | 61| | 62|    | 63| 64| 65| 66| 67|    | 68| 69| 6A| 6B| 6C|  | 5A| 5B| 5C| | 5D| 52| 53| 54|
    `---' `---'    `-------------------'    `-------------------'  `-----------' `---------------'
    ,-----------------------------------------------------------.  ,-----------. ,---------------.
    | 01| 02| 03| 04| 05| 06| 07| 08| 09| 0A| 0B| 0C| 0D| 0E| 0F|  | 36| 5E| 37| | 3F| 40| 41| 42|
    |-----------------------------------------------------------|  |------------ |---------------|
    |  10 | 11| 12| 13| 14| 15| 16| 17| 18| 19| 1A| 1B| 1C|     |  | 38| 39| 3A| | 43| 44| 45| 46|
    |------------------------------------------------------. 1D |  `---=====---' |---------------|
    |  71  | 1E| 1F| 20| 21| 2l| 23| 24| 25| 26| 27| 28| 29|    |   ___| 3C|___  | 47| 48| 49| 4A|
    |-----------------------------------------------------------|  | 3B|---| 3D| |-----------|---|
    |  70    | 2A| 2B| 2C| 2D| 2E| 2F| 30| 31| 32| 33| 34|   70 |  `---| 3E|---' | 4B| 4C| 4D|   |
    `-----------------------------------------------------------'  .---=====---. |-----------| 4E|
           | 5F| 55 | 56 |     35     | 57 | 58 | 59 | 60|         |  72 |  73 | | 4F| 50| 51|   |
           `---------------------------------------------'         `-----------' `---------------'

    ,---. ,---.    ,-------------------,    ,-------------------.  ,-----------. ,---------------.
    |F11| |F12|    | F1| F2| F3| F4| F5|    | F6| F7| F8| F9|F10|  |PrS|ScL|Pau| |F15|F16|F17|F18|
    `---' `---'    `-------------------'    `-------------------'  `-----------' `---------------'
    ,-----------------------------------------------------------.  ,-----------. ,---------------.
    |Esc|  1|  2|  3|  4|  5|  6|  7|  8|  9|  0|  -|  =|  \|Bsp|  |Ins|Hom|PgU| |NmL|  /|  *|  -|
    |-----------------------------------------------------------|  |------------ |---------------|
    | Tab |  Q|  W|  E|  R|  T|  Y|  U|  I|  O|  P|  [|  ]|     |  |Del|End|PgD| |  7|  8|  9|  +|
    |------------------------------------------------------. Ent|  `---=====---' |---------------|
    | Ctrl |  A|  S|  D|  F|  G|  H|  J|  K|  L|  ;|  '|  `|    |   ___|Up |___  |  4|  5|  6|  =|
    |-----------------------------------------------------------|  |Lef|---|Rig| |-----------|---|
    |  Shift |  Z|  X|  C|  V|  B|  N|  M|  ,|  .|  /| RO| Shift|  `---|Dow|---' |  1|  2|  3|   |
    `-----------------------------------------------------------'  .---=====---. |-----------|Ent|
           |Cap| Gui| Alt|   Space    | Alt| Gui| App|Ctl|         | F13 | F14 | |  0|  ,|  .|   |
           `---------------------------------------------'         `-----------' `---------------'
*/
const uint8_t PROGMEM unimap_trans[MATRIX_ROWS][MATRIX_COLS] = {
    { UNIMAP_NO,    UNIMAP_ESC,   UNIMAP_1,     UNIMAP_2,     UNIMAP_3,     UNIMAP_4,     UNIMAP_5,     UNIMAP_6     }, \
    { UNIMAP_7,     UNIMAP_8,     UNIMAP_9,     UNIMAP_0,     UNIMAP_MINUS, UNIMAP_EQUAL, UNIMAP_BSLS,  UNIMAP_BSPC  }, \
    { UNIMAP_TAB,   UNIMAP_Q,     UNIMAP_W,     UNIMAP_E,     UNIMAP_R,     UNIMAP_T,     UNIMAP_Y,     UNIMAP_U     }, \
    { UNIMAP_I,     UNIMAP_O,     UNIMAP_P,     UNIMAP_LBRC,  UNIMAP_RBRC,  UNIMAP_ENTER, UNIMAP_A,     UNIMAP_S     }, \
    { UNIMAP_D,     UNIMAP_F,     UNIMAP_G,     UNIMAP_H,     UNIMAP_J,     UNIMAP_K,     UNIMAP_L,     UNIMAP_SCLN  }, \
    { UNIMAP_QUOTE, UNIMAP_GRAVE, UNIMAP_Z,     UNIMAP_X,     UNIMAP_C,     UNIMAP_V,     UNIMAP_B,     UNIMAP_N     }, \
    { UNIMAP_M,     UNIMAP_COMMA, UNIMAP_DOT,   UNIMAP_SLASH, UNIMAP_RO,    UNIMAP_SPACE, UNIMAP_INSERT,UNIMAP_PGUP  }, \
    { UNIMAP_DELETE,UNIMAP_END,   UNIMAP_PGDOWN,UNIMAP_LEFT,  UNIMAP_UP,    UNIMAP_RIGHT, UNIMAP_DOWN,  UNIMAP_NLCK  }, \
    { UNIMAP_PSLS,  UNIMAP_PAST,  UNIMAP_PMNS,  UNIMAP_P7,    UNIMAP_P8,    UNIMAP_P9,    UNIMAP_PPLS,  UNIMAP_P4    }, \
    { UNIMAP_P5,    UNIMAP_P6,    UNIMAP_PEQL,  UNIMAP_P1,    UNIMAP_P2,    UNIMAP_P3,    UNIMAP_PENT,  UNIMAP_P0    }, \
    { UNIMAP_PCMM,  UNIMAP_PDOT,  UNIMAP_F16,   UNIMAP_F17,   UNIMAP_F18,   UNIMAP_LGUI,  UNIMAP_LALT,  UNIMAP_RALT  }, \
    { UNIMAP_RGUI,  UNIMAP_APP,   UNIMAP_PSCR,  UNIMAP_SLCK,  UNIMAP_PAUSE, UNIMAP_F15,   UNIMAP_HOME,  UNIMAP_CAPS  }, \
    { UNIMAP_RCTRL, UNIMAP_F11,   UNIMAP_F12,   UNIMAP_F1,    UNIMAP_F2,    UNIMAP_F3,    UNIMAP_F4,    UNIMAP_F5,   }, \
    { UNIMAP_F6,    UNIMAP_F7,    UNIMAP_F8,    UNIMAP_F9,    UNIMAP_F10,   UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO    }, \
    { UNIMAP_LSHIFT,UNIMAP_LCTRL, UNIMAP_F13,   UNIMAP_F14,   UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO,   }, \
    { UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO    }  \
};

#endif
