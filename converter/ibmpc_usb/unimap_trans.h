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


/*
 * Scan Code Set 1:
 * IBM XT 83-key:
 * ,-------.  ,--------------------------------------------------------------------------.
 * | F1| F2|  |Esc|  1|  2|  3|  4|  5|  6|  7|  8|  9|  0|  -|  =|  BS  |NumLck |ScrLck |
 * |-------|  |--------------------------------------------------------------------------|
 * | F3| F4|  | Tab |  Q|  W|  E|  R|  T|  Y|  U|  I|  O|  P|  [|  ] |   |  7|  8|  9|  -|
 * |-------|  |------------------------------------------------------|Ent|---------------|
 * | F5| F6|  | Ctrl |  A|  S|  D|  F|  G|  H|  J|  K|  L|  ;|  '|  `|   |  4|  5|  6|   |
 * |-------|  |----------------------------------------------------------------------|   |
 * | F7| F8|  |Shif|  \|  Z|  X|  C|  V|  B|  N|  M|  ,|  .|  /|Shift|  *|  1|  2|  3|  +|
 * |-------|  |----------------------------------------------------------------------|   |
 * | F9|F10|  |  Alt  |               Space                  |CapsLck|   0   |   .   |   |
 * `-------'  `--------------------------------------------------------------------------'
 *
 * ,-------.  ,--------------------------------------------------------------------------.
 * | 3B| 3C|  | 01| 02| 03| 04| 05| 06| 07| 08| 09| 0A| 0B| 0C| 0D|  0E  |  45   |  46   |
 * |-------|  |--------------------------------------------------------------------------|
 * | 3D| 3E|  | 0F  | 10| 11| 12| 13| 14| 15| 16| 17| 18| 19| 1A| 1B |   | 47| 48| 49| 4A|
 * |-------|  |------------------------------------------------------| 1C|---------------|
 * | 3F| 40|  | 1D   | 1E| 1F| 20| 21| 22| 23| 24| 25| 26| 27| 28| 29|   | 4B| 4C| 4D|   |
 * |-------|  |----------------------------------------------------------------------|   |
 * | 41| 42|  | 2A | 2B| 2C| 2D| 2E| 2F| 30| 31| 32| 33| 34| 35|  36 | 37| 4F| 50| 51| 4E|
 * |-------|  |----------------------------------------------------------------------|   |
 * | 43| 44|  |  38   |              39                      |  3A   |  52   |  53   |   |
 * `-------'  `--------------------------------------------------------------------------'
 * Full-key:
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
 * |CapsL |  A|  S|  D|  F|  G|  H|  J|  K|  L|  ;|  '|  #|Entr|               |  4|  5|  6|KP,|
 * |-----------------------------------------------------------|     ,---.     |---------------|
 * |Shft|  <|  Z|  X|  C|  V|  B|  N|  M|  ,|  .|  /| RO|Shift |     |Up |     |  1|  2|  3|Ent|
 * |-----------------------------------------------------------| ,-----------. |---------------|
 * |Ctl|Gui|Alt|MHEN|     Space      |HENK|KANA|Alt|Gui|App|Ctl| |Lef|Dow|Rig| |      0|  .|KP=|
 * `-----------------------------------------------------------' `-----------' `---------------'
 *         ,-----------------------------------------------.
 *         | 64| 65| 66| 67| 68| 69| 6A| 6B| 6C| 6D| 6E| 76|
 * ,---.   |-----------------------------------------------|     ,-----------.     ,-----------.
 * | 01|   | 3B| 3C| 3D| 3E| 3F| 40| 41| 42| 43| 44| 57| 58|     |*37| 46|*46|     |*2E|*30|*20|
 * `---'   `-----------------------------------------------'     `-----------'     `-----------'
 * ,-----------------------------------------------------------. ,-----------. ,---------------.
 * | 29| 02| 03| 04| 05| 06| 07| 08| 09| 0A| 0B| 0C| 0D| 7D| 0E| |*52|*47|*49| | 45|*35| 37| 4A|
 * |-----------------------------------------------------------| |-----------| |---------------|
 * | 0F  | 10| 11| 12| 13| 14| 15| 16| 17| 18| 19| 1A| 1B|  2B | |*53|*4F|*51| | 47| 48| 49| 4E|
 * |-----------------------------------------------------------| `-----------' |---------------|
 * | 3A   | 1E| 1F| 20| 21| 22| 23| 24| 25| 26| 27| 28| ^a| 1C |               | 4B| 4C| 4D| 7E|
 * |-----------------------------------------------------------|     ,---.     |---------------|
 * | 2A | 56| 2C| 2D| 2E| 2F| 30| 31| 32| 33| 34| 35| 73|  36  |     |*48|     | 4F| 50| 51|*1C|
 * |-----------------------------------------------------------| ,-----------. |---------------|
 * | 1D|*5B| 38| 7B |      39        | 79 | 70 |*38|*5C|*5D|*1D| |*4B|*50|*4D| |     52| 53| 59|
 * `-----------------------------------------------------------' `-----------' `---------------'
 * *: E0-prefixed codes. See cs1_e0code() for remapping to unimap array.
 * PrintScreen: E0 37 or 54(with Alt) -> 54
 * Pause: E1 1D 45 or E0 46(with Ctrl) -> 55
 * ^a: 2B(ISO XT) is handled as BSLASH. NUHS is unused.
 * Unsupported codes or error -> 00.
 */
const uint8_t PROGMEM unimap_cs1[MATRIX_ROWS][MATRIX_COLS] = {
    { UNIMAP_NO,    UNIMAP_ESC,   UNIMAP_1,     UNIMAP_2,     UNIMAP_3,     UNIMAP_4,     UNIMAP_5,     UNIMAP_6,       /* 00-07 */
      UNIMAP_7,     UNIMAP_8,     UNIMAP_9,     UNIMAP_0,     UNIMAP_MINUS, UNIMAP_EQUAL, UNIMAP_BSPACE,UNIMAP_TAB   }, /* 08-0F */
    { UNIMAP_Q,     UNIMAP_W,     UNIMAP_E,     UNIMAP_R,     UNIMAP_T,     UNIMAP_Y,     UNIMAP_U,     UNIMAP_I,       /* 10-17 */
      UNIMAP_O,     UNIMAP_P,     UNIMAP_LBRC,  UNIMAP_RBRC,  UNIMAP_ENTER, UNIMAP_LCTL,  UNIMAP_A,     UNIMAP_S,    }, /* 18-1F */
    { UNIMAP_D,     UNIMAP_F,     UNIMAP_G,     UNIMAP_H,     UNIMAP_J,     UNIMAP_K,     UNIMAP_L,     UNIMAP_SCLN,    /* 20-27 */
      UNIMAP_QUOTE, UNIMAP_GRAVE, UNIMAP_LSHIFT,UNIMAP_BSLASH,UNIMAP_Z,     UNIMAP_X,     UNIMAP_C,     UNIMAP_V,    }, /* 28-2F */
    { UNIMAP_B,     UNIMAP_N,     UNIMAP_M,     UNIMAP_COMMA, UNIMAP_DOT,   UNIMAP_SLASH, UNIMAP_RSHIFT,UNIMAP_PAST,    /* 30-37 */
      UNIMAP_LALT,  UNIMAP_SPACE, UNIMAP_CAPS,  UNIMAP_F1,    UNIMAP_F2,    UNIMAP_F3,    UNIMAP_F4,    UNIMAP_F5    }, /* 38-3F */
    { UNIMAP_F6,    UNIMAP_F7,    UNIMAP_F8,    UNIMAP_F9,    UNIMAP_F10,   UNIMAP_NLCK,  UNIMAP_SLCK,  UNIMAP_P7,      /* 40-47 */
      UNIMAP_P8,    UNIMAP_P9,    UNIMAP_PMNS,  UNIMAP_P4,    UNIMAP_P5,    UNIMAP_P6,    UNIMAP_PPLS,  UNIMAP_P1    }, /* 48-4F */
    { UNIMAP_P2,    UNIMAP_P3,    UNIMAP_P0,    UNIMAP_PDOT,  UNIMAP_PSCR,  UNIMAP_PAUSE, UNIMAP_NUBS,  UNIMAP_F11,     /* 50-57 */
      UNIMAP_F12,   UNIMAP_PEQL,  UNIMAP_LGUI,  UNIMAP_RGUI,  UNIMAP_APP,   UNIMAP_MUTE,  UNIMAP_VOLD,  UNIMAP_VOLU  }, /* 58-5F */
    { UNIMAP_UP,    UNIMAP_LEFT,  UNIMAP_DOWN,  UNIMAP_RIGHT, UNIMAP_F13,   UNIMAP_F14,   UNIMAP_F15,   UNIMAP_F16,     /* 60-67 */
      UNIMAP_F17,   UNIMAP_F18,   UNIMAP_F19,   UNIMAP_F20,   UNIMAP_F21,   UNIMAP_F22,   UNIMAP_F23,   UNIMAP_PENT  }, /* 68-6F */
    { UNIMAP_KANA,  UNIMAP_INSERT,UNIMAP_DELETE,UNIMAP_RO,    UNIMAP_HOME,  UNIMAP_END,   UNIMAP_F24,   UNIMAP_PGUP,    /* 70-77 */
      UNIMAP_PGDN,  UNIMAP_HENK,  UNIMAP_RCTL,  UNIMAP_MHEN,  UNIMAP_RALT,  UNIMAP_JYEN,  UNIMAP_PCMM,  UNIMAP_PSLS  }, /* 78-7F */
};


/*
 * Scan Code Set 2:
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
 * |CapsL |  A|  S|  D|  F|  G|  H|  J|  K|  L|  ;|  '| ^a|Entr|               |  4|  5|  6|KP,|
 * |-----------------------------------------------------------|     ,---.     |---------------|
 * |Shft|  <|  Z|  X|  C|  V|  B|  N|  M|  ,|  .|  /| RO|Shift |     |Up |     |  1|  2|  3|Ent|
 * |-----------------------------------------------------------| ,-----------. |---------------|
 * |Ctl|Gui|Alt|MHEN|     Space      |HENK|KANA|Alt|Gui|App|Ctl| |Lef|Dow|Rig| |  #|  0|  .|KP=|
 * `-----------------------------------------------------------' `-----------' `---------------'
 *
 *         ,-----------------------------------------------.
 *         | 08| 10| 18| 20| 28| 30| 38| 40| 48| 50| 57| 5F|
 * ,---.   |-----------------------------------------------|     ,-----------.     ,-----------.
 * | 76|   | 05| 06| 04| 0C| 03| 0B| 83| 0A| 01| 09| 78| 07|     |+7C| 7E|+77|     |*21|*32|*23|
 * `---'   `-----------------------------------------------'     `-----------'     `-----------'
 * ,-----------------------------------------------------------. ,-----------. ,---------------.
 * | 0E| 16| 1E| 26| 25| 2E| 36| 3D| 3E| 46| 45| 4E| 55| 6A| 66| |*70|*6C|*7D| | 77|*4A| 7C| 7B|
 * |-----------------------------------------------------------| |-----------| |---------------|
 * | 0D  | 15| 1D| 24| 2D| 2C| 35| 3C| 43| 44| 4D| 54| 5B|  5D | |*71|*69|*7A| | 6C| 75| 7D| 79|
 * |-----------------------------------------------------------| `-----------' |---------------|
 * | 58   | 1C| 1B| 23| 2B| 34| 33| 3B| 42| 4B| 4C| 52| ^a| 5A |               | 6B| 73| 74| 6D|
 * |-----------------------------------------------------------|     ,---.     |---------------|
 * | 12 | 61| 1A| 22| 21| 2A| 32| 31| 3A| 41| 49| 4A| 51|  59  |     |*75|     | 69| 72| 7A|*5A|
 * |-----------------------------------------------------------| ,-----------. |---------------|
 * | 14|*1F| 11| 67 |     29         | 64 | 13 |*11|*27|*2F|*14| |*6B|*72|*74| | 68| 70| 71| 63|
 * `-----------------------------------------------------------' `-----------' `---------------'
 * *: E0-prefixed codes. See cs2_e0code() for remapping to unimap array.
 * +: Special codes sequence
 * ^a: ISO hash key and US backslash use identical code 5D.
 * 51, 63, 68, 6A, 6D: Hidden keys in IBM model M [6]
 */
const uint8_t PROGMEM unimap_cs2[MATRIX_ROWS][MATRIX_COLS] = {
    { UNIMAP_PAUS,  UNIMAP_F9,    UNIMAP_F7,    UNIMAP_F5,    UNIMAP_F3,    UNIMAP_F1,    UNIMAP_F2,    UNIMAP_F12,     /* 00-07 */
      UNIMAP_F13,   UNIMAP_F10,   UNIMAP_F8,    UNIMAP_F6,    UNIMAP_F4,    UNIMAP_TAB,   UNIMAP_GRV,   UNIMAP_RALT  }, /* 08-0F */
    { UNIMAP_F14,   UNIMAP_LALT,  UNIMAP_LSHIFT,UNIMAP_KANA,  UNIMAP_LCTL,  UNIMAP_Q,     UNIMAP_1,     UNIMAP_LGUI,    /* 10-17 */
      UNIMAP_F15,   UNIMAP_RCTL,  UNIMAP_Z,     UNIMAP_S,     UNIMAP_A,     UNIMAP_W,     UNIMAP_2,     UNIMAP_RGUI  }, /* 18-1F */
    { UNIMAP_F16,   UNIMAP_C,     UNIMAP_X,     UNIMAP_D,     UNIMAP_E,     UNIMAP_4,     UNIMAP_3,     UNIMAP_APP,     /* 20-27 */
      UNIMAP_F17,   UNIMAP_SPACE, UNIMAP_V,     UNIMAP_F,     UNIMAP_T,     UNIMAP_R,     UNIMAP_5,     UNIMAP_HOME  }, /* 28-2F */
    { UNIMAP_F18,   UNIMAP_N,     UNIMAP_B,     UNIMAP_H,     UNIMAP_G,     UNIMAP_Y,     UNIMAP_6,     UNIMAP_DEL,     /* 30-37 */
      UNIMAP_F19,   UNIMAP_INS,   UNIMAP_M,     UNIMAP_J,     UNIMAP_U,     UNIMAP_7,     UNIMAP_8,     UNIMAP_DOWN  }, /* 38-3F */
    { UNIMAP_F20,   UNIMAP_COMMA, UNIMAP_K,     UNIMAP_I,     UNIMAP_O,     UNIMAP_0,     UNIMAP_9,     UNIMAP_RIGHT,   /* 40-47 */
      UNIMAP_F21,   UNIMAP_DOT,   UNIMAP_SLASH, UNIMAP_L,     UNIMAP_SCOLON,UNIMAP_P,     UNIMAP_MINUS, UNIMAP_UP    }, /* 48-4F */
    { UNIMAP_F22,   UNIMAP_RO,    UNIMAP_QUOTE, UNIMAP_LEFT,  UNIMAP_LBRC,  UNIMAP_EQUAL, UNIMAP_PGDN,  UNIMAP_F23,     /* 50-57 */
      UNIMAP_CAPS,  UNIMAP_RSHIFT,UNIMAP_ENTER, UNIMAP_RBRC,  UNIMAP_END,   UNIMAP_BSLASH,UNIMAP_PGUP,  UNIMAP_F24   }, /* 58-5F */
    { UNIMAP_PSLS,  UNIMAP_NUBS,  UNIMAP_PENT,  UNIMAP_PEQL,  UNIMAP_HENK,  UNIMAP_VOLD,  UNIMAP_BSPACE,UNIMAP_MHEN,    /* 60-67 */
      UNIMAP_NUHS,  UNIMAP_P1,    UNIMAP_JYEN,  UNIMAP_P4,    UNIMAP_P7,    UNIMAP_PCMM,  UNIMAP_VOLU,  UNIMAP_MUTE  }, /* 68-6F */
    { UNIMAP_P0,    UNIMAP_PDOT,  UNIMAP_P2,    UNIMAP_P5,    UNIMAP_P6,    UNIMAP_P8,    UNIMAP_ESC,   UNIMAP_NLCK,    /* 70-77 */
      UNIMAP_F11,   UNIMAP_PPLS,  UNIMAP_P3,    UNIMAP_PMNS,  UNIMAP_PAST,  UNIMAP_P9,    UNIMAP_SLCK,  UNIMAP_PSCR  }, /* 78-7F */
};


/*
 * Scan Code Set 3:
 *               ,-----------------------------------------------.
 *               |F13|F14|F15|F16|F17|F18|F19|F20|F21|F22|F23|F24|
 *               |-----------------------------------------------|
 *               |F1 |F2 |F3 |F4 |F5 |F6 |F7 |F8 |F9 |F10|F11|F12|
 *               `-----------------------------------------------'
 * ,-------. ,-----------------------------------------------------------. ,-----------. ,---------------.
 * |Mut|HEN| |  `|  1|  2|  3|  4|  5|  6|  7|  8|  9|  0|  -|  =|JPY| BS| |  /|PgU|PgD| |Esc|NmL|ScL|  *|
 * |-------| |-----------------------------------------------------------| |-----------| |---------------|
 * |VUp|Pau| |Tab  |  Q|  W|  E|  R|  T|  Y|  U|  I|  O|  P|  [|  ]|    \| |End|Ins|Del| |  7|  8|  9|  +|
 * |-------| |-----------------------------------------------------------| `-----------' |-----------|---|
 * |VDn|MHE| |CapsL |  A|  S|  D|  F|  G|  H|  J|  K|  L|  ;|  '|  #| Ret| |JPY|Up |  #| |  4|  5|  6|  -|
 * |-------| |-----------------------------------------------------------| ,-----------. |---------------|
 * |PrS|App| |Shif|  \|  Z|  X|  C|  V|  B|  N|  M|  ,|  ,|  /| RO| Shift| |Lef|Hom|Rig| |  1|  2|  3|Ent|
 * |-------| |-----------------------------------------------------------| `-----------' |-----------|---|
 * |Gui|Gui| |Ctrl|    | Alt | P,  |  Space  | P=  | KANA| Alt |    |Ctrl| | RO|Dow|KAN| |  ,|  0|  .|  =|
 * `-------' `----'    `---------------------------------------'    `----'  ---`---'---  `---------------'
 *
 *               ,-----------------------------------------------.
 *               | 08| 10| 18| 20| 28| 30| 38| 40| 48| 50| 57| 5F|
 *               |-----------------------------------------------|
 *               | 07| 0F| 17| 1F| 27| 2F| 37| 3F| 47| 4F| 56| 5E|
 *               `-----------------------------------------------'
 * ,-------. ,-----------------------------------------------------------. ,-----------. ,---------------.
 * | 05| 06| | 0E| 16| 1E| 26| 25| 2E| 36| 3D| 3E| 46| 45| 4E| 55| 5D| 66| | 67| 6E| 6F| | 76| 77| 7E| 84|
 * |-------| |-----------------------------------------------------------| |-----------| |---------------|
 * | 04| 0C| | 0D  | 15| 1D| 24| 2D| 2C| 35| 3C| 43| 44| 4D| 54| 5B|  5C | | 64| 65| 6D| | 6C| 75| 7D| 7C|
 * |-------| |-----------------------------------------------------------| `-----------' |---------------|
 * | 03| 0B| | 14   | 1C| 1B| 23| 2B| 34| 33| 3B| 42| 4B| 4C| 52| 53| 5A | |-26| 63|-25| | 6B| 73| 74| 7B|
 * |-------| |-----------------------------------------------------------| ,-----------. |---------------|
 * | 83| 0A| | 12 | 13| 1A| 22| 21| 2A| 32| 31| 3A| 41| 49| 4A| 51|  59  | | 61| 62| 6A| | 69| 72| 7A| 79|
 * |-------| |-----------------------------------------------------------| `-----------' |---------------|
 * | 01| 09| | 11  |   | 19  | ^85 |   29    | ^86 |  87 | 39  |   | 58  | |-16| 60|-1E| | 68| 70| 71| 78|
 * `-------' `-----'   `---------------------------------------'   `-----'  ---`---'---  `---------------'
 * -: G80-2551 specific 80-prefixed codes remapped: 26->5D, 25->53, 16->51, 1E->00
 * ^: 5576-001 Japanese keys
 * 51, 5C, 5D, 68, 78: Hidden keys in IBM 122-key terminal keyboard [7]
 */
const uint8_t PROGMEM unimap_cs3[MATRIX_ROWS][MATRIX_COLS] = {
    { UNIMAP_KANA,  UNIMAP_LGUI,  UNIMAP_PSCR,  UNIMAP_VOLD,  UNIMAP_VOLU,  UNIMAP_MUTE,  UNIMAP_HENK,  UNIMAP_F1,      /* 00-07 */
      UNIMAP_F13,   UNIMAP_RGUI,  UNIMAP_APP,   UNIMAP_MHEN,  UNIMAP_PAUS,  UNIMAP_TAB,   UNIMAP_GRV,   UNIMAP_F2    }, /* 08-0F */
    { UNIMAP_F14,   UNIMAP_LCTL,  UNIMAP_LSHIFT,UNIMAP_NUBS,  UNIMAP_CAPS,  UNIMAP_Q,     UNIMAP_1,     UNIMAP_F3,      /* 10-17 */
      UNIMAP_F15,   UNIMAP_LALT,  UNIMAP_Z,     UNIMAP_S,     UNIMAP_A,     UNIMAP_W,     UNIMAP_2,     UNIMAP_F4    }, /* 18-1F */
    { UNIMAP_F16,   UNIMAP_C,     UNIMAP_X,     UNIMAP_D,     UNIMAP_E,     UNIMAP_4,     UNIMAP_3,     UNIMAP_F5,      /* 20-27 */
      UNIMAP_F17,   UNIMAP_SPACE, UNIMAP_V,     UNIMAP_F,     UNIMAP_T,     UNIMAP_R,     UNIMAP_5,     UNIMAP_F6    }, /* 28-2F */
    { UNIMAP_F18,   UNIMAP_N,     UNIMAP_B,     UNIMAP_H,     UNIMAP_G,     UNIMAP_Y,     UNIMAP_6,     UNIMAP_F7,      /* 30-37 */
      UNIMAP_F19,   UNIMAP_RALT,  UNIMAP_M,     UNIMAP_J,     UNIMAP_U,     UNIMAP_7,     UNIMAP_8,     UNIMAP_F8    }, /* 38-3F */
    { UNIMAP_F20,   UNIMAP_COMMA, UNIMAP_K,     UNIMAP_I,     UNIMAP_O,     UNIMAP_0,     UNIMAP_9,     UNIMAP_F9,      /* 40-47 */
      UNIMAP_F21,   UNIMAP_DOT,   UNIMAP_SLASH, UNIMAP_L,     UNIMAP_SCOLON,UNIMAP_P,     UNIMAP_MINUS, UNIMAP_F10   }, /* 48-4F */
    { UNIMAP_F22,   UNIMAP_RO,    UNIMAP_QUOTE, UNIMAP_NUHS,  UNIMAP_LBRC,  UNIMAP_EQUAL, UNIMAP_F11,   UNIMAP_F23,     /* 50-57 */
      UNIMAP_RCTL,  UNIMAP_RSHIFT,UNIMAP_ENTER, UNIMAP_RBRC,  UNIMAP_BSLASH,UNIMAP_JYEN,  UNIMAP_F12,   UNIMAP_F24   }, /* 58-5F */
    { UNIMAP_DOWN,  UNIMAP_LEFT,  UNIMAP_HOME,  UNIMAP_UP,    UNIMAP_END,   UNIMAP_INS,   UNIMAP_BSPACE,UNIMAP_PSLS,    /* 60-67 */
      UNIMAP_PCMM,  UNIMAP_P1,    UNIMAP_RIGHT, UNIMAP_P4,    UNIMAP_P7,    UNIMAP_DEL,   UNIMAP_PGUP,  UNIMAP_PGDN  }, /* 68-6F */
    { UNIMAP_P0,    UNIMAP_PDOT,  UNIMAP_P2,    UNIMAP_P5,    UNIMAP_P6,    UNIMAP_P8,    UNIMAP_ESC,   UNIMAP_NLCK,    /* 70-77 */
      UNIMAP_PEQL,  UNIMAP_PENT,  UNIMAP_P3,    UNIMAP_PMNS,  UNIMAP_PPLS,  UNIMAP_P9,    UNIMAP_SLCK,  UNIMAP_PAST  }, /* 78-7F */
};

#endif
