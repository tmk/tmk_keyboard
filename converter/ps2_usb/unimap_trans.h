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


/* Mapping to Universal keyboard layout
 *
 * Universal keyboard layout
 *         ,-----------------------------------------------.
 *         |F13|F14|F15|F16|F17|F18|F19|F20|F21|F22|F23|F24|
 * ,---.   |-----------------------------------------------|     ,-----------.     ,-----------.
 * |Esc|   |F1 |F2 |F3 |F4 |F5 |F6 |F7 |F8 |F9 |F10|F11|F12|     |PrS|ScL|Pau|     |VDn|VUp|Mut|
 * `---'   `-----------------------------------------------'     `-----------'     `-----------'
 * ,-----------------------------------------------------------. ,-----------. ,---------------.
 * |  `|  1|  2|  3|  4|  5|  6|  7|  8|  9|  0|  -|  =|JPY|Bsp| |Ins|Hom|PgU| |NmL|  /|  *|  -|
 * |-----------------------------------------------------------| |-----------| |---------------|
 * |Tab  |  Q|  W|  E|  R|  T|  Y|  U|  I|  O|  P|  [|  ]|  \  | |Del|End|PgD| |  7|  8|  9|  +|
 * |-----------------------------------------------------------| `-----------' |---------------|
 * |CapsL |  A|  S|  D|  F|  G|  H|  J|  K|  L|  ;|  '|  #|Retn|               |  4|  5|  6|KP,|
 * |-----------------------------------------------------------|     ,---.     |---------------|
 * |Shft|  <|  Z|  X|  C|  V|  B|  N|  M|  ,|  .|  /| RO|Shift |     |Up |     |  1|  2|  3|KP=|
 * |-----------------------------------------------------------| ,-----------. |---------------|
 * |Ctl|Gui|Alt|MHEN|     Space      |HENK|KANA|Alt|Gui|App|Ctl| |Lef|Dow|Rig| |  0    |  .|Ent|
 * `-----------------------------------------------------------' `-----------' `---------------'
 *
 * PS/2 scan codes
 * http://download.microsoft.com/download/1/6/1/161ba512-40e2-4cc9-843a-923143f3456c/translate.pdf
 *         ,-----------------------------------------------.
 *         | 08| 10| 18| 20| 28| 30| 38| 40| 48| 50| 57| 5F|
 * ,---.   |-----------------------------------------------|     ,-----------.     ,-----------.
 * | 76|   | 05| 06| 04| 0C| 03| 0B| 83| 0A| 01| 09| 78| 07|     | FC| 7E| FE|     | A1| B2| A3|
 * `---'   `-----------------------------------------------'     `-----------'     `-----------'
 * ,-----------------------------------------------------------. ,-----------. ,---------------.
 * | 0E| 16| 1E| 26| 25| 2E| 36| 3D| 3E| 46| 45| 4E| 55| 6A| 66| | F0| EC| FD| | 77| CA| 7C| 7B|
 * |-----------------------------------------------------------| |-----------| |---------------|
 * | 0D  | 15| 1D| 24| 2D| 2C| 35| 3C| 43| 44| 4D| 54| 5B|  5D | | F1| E9| FA| | 6C| 75| 7D| 79|
 * |-----------------------------------------------------------| `-----------' |---------------|
 * | 58   | 1C| 1B| 23| 2B| 34| 33| 3B| 42| 4B| 4C| 52| --| 5A |               | 6B| 73| 74| 6D|
 * |-----------------------------------------------------------|     ,---.     |---------------|
 * | 12 | 61| 1A| 22| 21| 2A| 32| 31| 3A| 41| 49| 4A| 51|  59  |     | F5|     | 69| 72| 7A| 0F|
 * |-----------------------------------------------------------| ,-----------. |---------------|
 * | 14| 9F| 11| 67 |     29         | 64 | 13 | 91| A7| AF| 94| | EB| F2| F4| |   70  | 71| DA|
 * `-----------------------------------------------------------' `-----------' `---------------'
 */
const uint8_t PROGMEM unimap_trans[MATRIX_ROWS][MATRIX_COLS] = {
    { UNIMAP_NO,    UNIMAP_F9,    UNIMAP_NO,    UNIMAP_F5,    UNIMAP_F3,    UNIMAP_F1,    UNIMAP_F2,    UNIMAP_F12   }, /* 00-07 */
    { UNIMAP_F13,   UNIMAP_F10,   UNIMAP_F8,    UNIMAP_F6,    UNIMAP_F4,    UNIMAP_TAB,   UNIMAP_GRV,   UNIMAP_PEQL  }, /* 08-0F */
    { UNIMAP_F14,   UNIMAP_LALT,  UNIMAP_LSHIFT,UNIMAP_KANA,  UNIMAP_LCTL,  UNIMAP_Q,     UNIMAP_1,     UNIMAP_NO    }, /* 10-17 */
    { UNIMAP_F15,   UNIMAP_NO,    UNIMAP_Z,     UNIMAP_S,     UNIMAP_A,     UNIMAP_W,     UNIMAP_2,     UNIMAP_NO    }, /* 18-1F */
    { UNIMAP_F16,   UNIMAP_C,     UNIMAP_X,     UNIMAP_D,     UNIMAP_E,     UNIMAP_4,     UNIMAP_3,     UNIMAP_NO    }, /* 20-27 */
    { UNIMAP_F17,   UNIMAP_SPACE, UNIMAP_V,     UNIMAP_F,     UNIMAP_T,     UNIMAP_R,     UNIMAP_5,     UNIMAP_NO    }, /* 28-2F */
    { UNIMAP_F18,   UNIMAP_N,     UNIMAP_B,     UNIMAP_H,     UNIMAP_G,     UNIMAP_Y,     UNIMAP_6,     UNIMAP_NO    }, /* 30-37 */
    { UNIMAP_F19,   UNIMAP_NO,    UNIMAP_M,     UNIMAP_J,     UNIMAP_U,     UNIMAP_7,     UNIMAP_8,     UNIMAP_NO    }, /* 38-3F */
    { UNIMAP_F20,   UNIMAP_COMMA, UNIMAP_K,     UNIMAP_I,     UNIMAP_O,     UNIMAP_0,     UNIMAP_9,     UNIMAP_NO    }, /* 40-47 */
    { UNIMAP_F21,   UNIMAP_DOT,   UNIMAP_SLASH, UNIMAP_L,     UNIMAP_SCOLON,UNIMAP_P,     UNIMAP_MINUS, UNIMAP_NO    }, /* 48-4F */
    { UNIMAP_F22,   UNIMAP_RO,    UNIMAP_QUOTE, UNIMAP_NO,    UNIMAP_LBRC,  UNIMAP_EQUAL, UNIMAP_NO,    UNIMAP_F23   }, /* 50-57 */
    { UNIMAP_CAPS,  UNIMAP_RSHIFT,UNIMAP_ENTER, UNIMAP_RBRC,  UNIMAP_NO,    UNIMAP_BSLASH,UNIMAP_NO,    UNIMAP_F24   }, /* 58-5F */
    { UNIMAP_NO,    UNIMAP_NUBS,  UNIMAP_NO,    UNIMAP_NO,    UNIMAP_HENK,  UNIMAP_NO,    UNIMAP_BSPACE,UNIMAP_MHEN  }, /* 60-67 */
    { UNIMAP_NO,    UNIMAP_P1,    UNIMAP_JYEN,  UNIMAP_P4,    UNIMAP_P7,    UNIMAP_PCMM,  UNIMAP_NO,    UNIMAP_NO    }, /* 68-6F */
    { UNIMAP_P0,    UNIMAP_PDOT,  UNIMAP_P2,    UNIMAP_P5,    UNIMAP_P6,    UNIMAP_P8,    UNIMAP_ESC,   UNIMAP_NLCK  }, /* 70-77 */
    { UNIMAP_F11,   UNIMAP_PPLS,  UNIMAP_P3,    UNIMAP_PMNS,  UNIMAP_PAST,  UNIMAP_P9,    UNIMAP_SLCK,  UNIMAP_NO    }, /* 78-7F */
    { UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO,    UNIMAP_F7,    UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO    }, /* 80-87 */
    { UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO    }, /* 88-8F */
    { UNIMAP_NO,    UNIMAP_RALT,  UNIMAP_NO,    UNIMAP_NO,    UNIMAP_RCTRL, UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO    }, /* 90-97 */
    { UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO,    UNIMAP_LGUI  }, /* 98-9F */
    { UNIMAP_NO,    UNIMAP_VOLD,  UNIMAP_NO,    UNIMAP_MUTE,  UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO,    UNIMAP_RGUI  }, /* A0-A7 */
    { UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO,    UNIMAP_APP   }, /* A8-AF */
    { UNIMAP_NO,    UNIMAP_NO,    UNIMAP_VOLU,  UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO    }, /* B0-B7 */
    { UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO    }, /* B8-BF */
    { UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO    }, /* C0-C7 */
    { UNIMAP_NO,    UNIMAP_NO,    UNIMAP_PSLS,  UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO    }, /* C8-CF */
    { UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO    }, /* D0-D7 */
    { UNIMAP_NO,    UNIMAP_NO,    UNIMAP_PENT,  UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO    }, /* D8-DF */
    { UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO    }, /* E0-E7 */
    { UNIMAP_NO,    UNIMAP_END,   UNIMAP_NO,    UNIMAP_LEFT,  UNIMAP_HOME,  UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO    }, /* E8-EF */
    { UNIMAP_INS,   UNIMAP_DEL,   UNIMAP_DOWN,  UNIMAP_NO,    UNIMAP_RIGHT, UNIMAP_UP,    UNIMAP_NO,    UNIMAP_NO    }, /* F0-F7 */
    { UNIMAP_NO,    UNIMAP_NO,    UNIMAP_PGDOWN,UNIMAP_NO,    UNIMAP_PSCR,  UNIMAP_PGUP,  UNIMAP_PAUSE, UNIMAP_NO    }, /* F8-FF */
};

#endif

