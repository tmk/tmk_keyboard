/*
Copyright 2020 Jun Wako <wakojun@gmail.com>

This file is part of TMK keyboard. https://github.com/tmk/tmk_keyboard

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
 * ,-------.  ,---,  ,---------------. ,---------------. ,---------------.   ,-----------. ,---------------.
 * |  76   |  | 0F|  | 05| 06| 08| 0A| | 0C| 0E| 10| 11| | 12| 07| 09| 0B|   | 16| 17| 15| | 2D| 02| 04| 30|
 * `-------'  `---'  `---------------' `---------------' `---------------'   `-----------' `---------------'
 * ,-------.  ,-----------------------------------------------------------.  ,-----------. ,---------------.
 * | 01| 03|  | 1D| 1E| 1F| 20| 21| 22| 23| 24| 25| 26| 27| 28| 29| 58| 2A|  | 2C| 34| 60| | 62| 2E| 2F| 47|
 * |-------|  |-----------------------------------------------------------|  |------------ |---------------|
 * | 19| 1A|  |  35 | 36| 37| 38| 39| 3A| 3B| 3C| 3D| 3E| 3F| 40| 41| 2B  |  | 42| 4A| 7B| | 44| 45| 46|   |
 * |-------|  |-----------------------------------------------------------|  `-----------' |-----------| 7D|
 * | 31| 33|  |  4C  | 4D| 4E| 4F| 50| 51| 52| 53| 54| 55| 56| 57|   59   |                | 5B| 5C| 5D|   |
 * |-------|  |-----------------------------------------------------------|      ,---.     |-----------|---|
 * | 48| 49|  | 63 |+7C| 64| 65| 66| 67| 68| 69| 6A| 6B| 6C| 6D|*6F|  6E  |      | 14|     | 70| 71| 72|   |
 * |-------|  |-----------------------------------------------------------|  .-----------. |-----------| 5A|
 * | 5F| 61|  | 77 | 13| 78 |*73 |       79         |*74 |*75| 7A | 43| 0D|  | 18| 1B| 1C| |   5E  | 32|   |
 * `-------'  `-----------------------------------------------------------'  `-----------' `---------------'
 * ,-------.  ,---,  ,---------------. ,---------------. ,---------------.   ,-----------. ,---------------.
 * |  F23  |  |F24|  | F1| F2| F3| F4| | F5| F6| F7| F8| | F9|F10|F11|F12|   |PrS|ScL|Pau| |Mut|VoD|VoU|Ctl|
 * `-------'  `---'  `---------------' `---------------' `---------------'   `-----------' `---------------'
 * ,-------.  ,-----------------------------------------------------------.  ,-----------. ,---------------.
 * |F13|F14|  |Esc|  1|  2|  3|  4|  5|  6|  7|  8|  9|  0|  -|  =|  \|  `|  |Ins|Hom|PgU| |NmL|  /|  *|  -|
 * |-------|  |-----------------------------------------------------------|  |------------ |---------------|
 * |F15|F16|  |Tab  |  Q|  W|  E|  R|  T|  Y|  U|  I|  O|  P|  [|  ]| Bspc|  |Del|End|PgD| |  7|  8|  9|   |
 * |-------|  |-----------------------------------------------------------|  `-----------' |-----------|  +|
 * |F17|F18|  |Ctrl  |  A|  S|  D|  F|  G|  H|  J|  K|  L|  ;|  '|  Return|                |  4|  5|  6|   |
 * |-------|  |-----------------------------------------------------------|      ,---.     |-----------|---|
 * |F19|F20|  |Shft| +\|  Z|  X|  C|  V|  B|  N|  M|  ,|  .|  /| RO| Shift|      | Up|     |  1|  2|  3|   |
 * |-------|  |-----------------------------------------------------------|  .-----------. |-----------|Ent|
 * |F21|F22|  |Caps|Alt|Gui |MHEN|      Space       |HENK|KAN| Gui|App|Alt|  |Lef|Dow|Rig| |      0|  .|   |
 * `-------'  `-----------------------------------------------------------'  `-----------' `---------------'
 * SUN scan codes: http://kentie.net/article/sunkbd/KBD.pdf
 * Japanese specific keys*:  http://shikasan.net/sunkey/sunkey_e.html
 * ISO key+:  https://bit.ly/3eennkS
 */
const uint8_t PROGMEM unimap_trans[MATRIX_ROWS][MATRIX_COLS] = {
    { UNIMAP_NO,   UNIMAP_F13,  UNIMAP_VOLD, UNIMAP_F14,  UNIMAP_VOLU, UNIMAP_F1,   UNIMAP_F2,   UNIMAP_F10  }, /* 00-07 */
    { UNIMAP_F3,   UNIMAP_F11,  UNIMAP_F4,   UNIMAP_F12,  UNIMAP_F5,   UNIMAP_RALT, UNIMAP_F6,   UNIMAP_F24  }, /* 08-0F */
    { UNIMAP_F7,   UNIMAP_F8,   UNIMAP_F9,   UNIMAP_LALT, UNIMAP_UP,   UNIMAP_PAUS, UNIMAP_PSCR, UNIMAP_SLCK }, /* 10-17 */
    { UNIMAP_LEFT, UNIMAP_F15,  UNIMAP_F16,  UNIMAP_DOWN, UNIMAP_RGHT, UNIMAP_ESC,  UNIMAP_1,    UNIMAP_2    }, /* 18-1F */
    { UNIMAP_3,    UNIMAP_4,    UNIMAP_5,    UNIMAP_6,    UNIMAP_7,    UNIMAP_8,    UNIMAP_9,    UNIMAP_0    }, /* 20-27 */
    { UNIMAP_MINS, UNIMAP_EQL,  UNIMAP_GRV,  UNIMAP_BSPC, UNIMAP_INS,  UNIMAP_MUTE, UNIMAP_PSLS, UNIMAP_PAST }, /* 28-2F */
    { UNIMAP_RCTL, UNIMAP_F17,  UNIMAP_PDOT, UNIMAP_F18,  UNIMAP_HOME, UNIMAP_TAB,  UNIMAP_Q,    UNIMAP_W    }, /* 30-37 */
    { UNIMAP_E,    UNIMAP_R,    UNIMAP_T,    UNIMAP_Y,    UNIMAP_U,    UNIMAP_I,    UNIMAP_O,    UNIMAP_P    }, /* 38-3F */
    { UNIMAP_LBRC, UNIMAP_RBRC, UNIMAP_DEL,  UNIMAP_APP,  UNIMAP_P7,   UNIMAP_P8,   UNIMAP_P9,   UNIMAP_PMNS }, /* 40-47 */
    { UNIMAP_F19,  UNIMAP_F20,  UNIMAP_END,  UNIMAP_NO,   UNIMAP_LCTL, UNIMAP_A,    UNIMAP_S,    UNIMAP_D    }, /* 48-4F */
    { UNIMAP_F,    UNIMAP_G,    UNIMAP_H,    UNIMAP_J,    UNIMAP_K,    UNIMAP_L,    UNIMAP_SCLN, UNIMAP_QUOT }, /* 50-57 */
    { UNIMAP_BSLS, UNIMAP_ENT,  UNIMAP_PENT, UNIMAP_P4,   UNIMAP_P5,   UNIMAP_P6,   UNIMAP_P0,   UNIMAP_F21  }, /* 58-5F */
    { UNIMAP_PGUP, UNIMAP_F22,  UNIMAP_NLCK, UNIMAP_LSFT, UNIMAP_Z,    UNIMAP_X,    UNIMAP_C,    UNIMAP_V    }, /* 60-67 */
    { UNIMAP_B,    UNIMAP_N,    UNIMAP_M,    UNIMAP_COMM, UNIMAP_DOT,  UNIMAP_SLSH, UNIMAP_RSFT, UNIMAP_RO   }, /* 68-6F */
    { UNIMAP_P1,   UNIMAP_P2,   UNIMAP_P3,   UNIMAP_MHEN, UNIMAP_HENK, UNIMAP_KANA, UNIMAP_F23,  UNIMAP_CAPS }, /* 70-77 */
    { UNIMAP_LGUI, UNIMAP_SPC,  UNIMAP_RGUI, UNIMAP_PGDN, UNIMAP_NUBS, UNIMAP_PPLS, UNIMAP_NO,   UNIMAP_NO   }, /* 78-7F */
};


/* Sun type 5/5c keyboard
 * ,-------.  ,---,  ,---------------. ,---------------. ,---------------.   ,-----------. ,---------------.
 * | Help  |  |   |  | F1| F2| F3| F4| | F5| F6| F7| F8| | F9|F10|F11|F12|   |PrS|ScL|Pau| |Mut|VoD|VoU|Pwr|
 * `-------'  `---'  `---------------' `---------------' `---------------'   `-----------' `---------------'
 * ,-------.  ,-----------------------------------------------------------.  ,-----------. ,---------------.
 * |Stp|Agn|  |Esc|  1|  2|  3|  4|  5|  6|  7|  8|  9|  0|  -|  =|  \|  `|  |Ins|Hom|PgU| |NmL|  /|  *|  -|
 * |-------|  |-----------------------------------------------------------|  |------------ |---------------|
 * |Prp|Und|  |Tab  |  Q|  W|  E|  R|  T|  Y|  U|  I|  O|  P|  [|  ]| Bspc|  |Del|End|PgD| |  7|  8|  9|   |
 * |-------|  |-----------------------------------------------------------|  `-----------' |-----------|  +|
 * |Frt|Cpy|  |Ctrl  |  A|  S|  D|  F|  G|  H|  J|  K|  L|  ;|  '|  Return|                |  4|  5|  6|   |
 * |-------|  |-----------------------------------------------------------|      ,---.     |-----------|---|
 * |Opn|Pst|  |Shft| +\|  Z|  X|  C|  V|  B|  N|  M|  ,|  .|  /|   | Shift|      | Up|     |  1|  2|  3|   |
 * |-------|  |-----------------------------------------------------------|  .-----------. |-----------|Ent|
 * |Fnd|Cut|  |Caps|Alt|Meta|    |      Space       |    |   |Meta|Cmp|Alt|  |Lef|Dow|Rig| |      0|  .|   |
 * `-------'  `-----------------------------------------------------------'  `-----------' `---------------'
 */
#define UNIMAP_TYPE5( \
    K76,      K0F,  K05,K06,K08,K0A,   K0C,K0E,K10,K11,   K12,K07,K09,K0B,  K16,K17,K15,  K2D,K02,K04,K30, \
    K01,K03,  K1D,K1E,K1F,K20,K21,K22,K23,K24,K25,K26,K27,K28,K29,K58,K2A,  K2C,K34,K60,  K62,K2E,K2F,K47, \
    K19,K1A,  K35,K36,K37,K38,K39,K3A,K3B,K3C,K3D,K3E,K3F,K40,K41,    K2B,  K42,K4A,K7B,  K44,K45,K46,K7D, \
    K31,K33,  K4C,K4D,K4E,K4F,K50,K51,K52,K53,K54,K55,K56,K57,        K59,                K5B,K5C,K5D,     \
    K48,K49,  K63,K7C,K64,K65,K66,K67,K68,K69,K6A,K6B,K6C,K6D,    K6F,K6E,      K14,      K70,K71,K72,K5A, \
    K5F,K61,  K77,K13,K78,K73,        K79,            K74,K75,K7A,K43,K0D,  K18,K1B,K1C,  K5E,    K32      \
) UNIMAP ( \
            K01,K03,K19,K1A,K31,K33,K48,K49,K5F,K61,K76,K0F,                                     \
    K1D,    K05,K06,K08,K0A,K0C,K0E,K10,K11,K12,K07,K09,K0B,      K16,K17,K15,      K02,K04,K2D, \
    K2A,K1E,K1F,K20,K21,K22,K23,K24,K25,K26,K27,K28,K29,NO, K2B,  K2C,K34,K60,  K62,K2E,K2F,K47, \
    K35,K36,K37,K38,K39,K3A,K3B,K3C,K3D,K3E,K3F,K40,K41,    K58,  K42,K4A,K7B,  K44,K45,K46,K7D, \
    K77,K4D,K4E,K4F,K50,K51,K52,K53,K54,K55,K56,K57,    NO, K59,                K5B,K5C,K5D,NO,  \
    K63,K7C,K64,K65,K66,K67,K68,K69,K6A,K6B,K6C,K6D,    K6F,K6E,      K14,      K70,K71,K72,K5A, \
    K4C,K78,K13,K73,        K79,        K74,K75,K0D,K7A,K43,K30,  K18,K1B,K1C,  K5E,    K32,NO   \
)


/* Sun type 3 keyboard 
 * ,-------.  ,-----------------------------------------------------------.  ,-----------.
 * | 01| 03|  | 05| 06|     08|     0A|     0C|     0E|     10| 11| 12| 2B|  | 15| 16| 17|
 * |-------|  |-----------------------------------------------------------|  |-----------|
 * | 19| 1A|  | 1D| 1E| 1F| 20| 21| 22| 23| 24| 25| 26| 27| 28| 29| 58| 2A|  | 2D| 2E| 2F|
 * |-------|  |-----------------------------------------------------------|  |-----------|
 * | 31| 33|  |  35 | 36| 37| 38| 39| 3A| 3B| 3C| 3D| 3E| 3F| 40| 41| 42  |  | 44| 45| 46|
 * |-------|  |-----------------------------------------------------------|  |-----------|
 * | 48| 49|  |  4C  | 4D| 4E| 4F| 50| 51| 52| 53| 54| 55| 56| 57|   59   |  | 5B| 5C| 5D|
 * |-------|  |-----------------------------------------------------------|  |-----------|
 * | 5F| 61|  |   63   | 64| 65| 66| 67| 68| 69| 6A| 6B| 6C| 6D|    6E| 6F|  | 70| 71| 72|
 * `-------'  |-----------------------------------------------------------|  `-----------'
 *            | 77 | 78  |               79                  | 7A  |   13 |  
 *            `-----------------------------------------------------------'  
 * http://blog.daveastels.com.s3-website-us-west-2.amazonaws.com/2014/12/27/type-3-keyboard.html
 * https://github.com/dastels/tmk_keyboard/blob/master/converter/sun3_usb/keymap.c
 */
#define UNIMAP_TYPE3( \
    K01,K03,  K05,K06,    K08,    K0A,    K0C,    K0E,    K10,K11,K12,K2B,  K15,K16,K17, \
    K19,K1A,  K1D,K1E,K1F,K20,K21,K22,K23,K24,K25,K26,K27,K28,K29,K58,K2A,  K2D,K2E,K2F, \
    K31,K33,  K35, K36,K37,K38,K39,K3A,K3B,K3C,K3D,K3E,K3F,K40,K41,   K42,  K44,K45,K46, \
    K48,K49,  K4C,  K4D,K4E,K4F,K50,K51,K52,K53,K54,K55,K56,K57,      K59,  K5B,K5C,K5D, \
    K5F,K61,  K63,   K64,K65,K66,K67,K68,K69,K6A,K6B,K6C,K6D,     K6E,K6F,  K70,K71,K72, \
              K77,K78,                    K79,                    K7A,K13                \
) UNIMAP ( \
            K01,K03,K19,K1A,K31,K33,K48,K49,K5F,K61,NO, NO,                                      \
    K1D,    K05,K06,K08,K0A,K0C,K0E,K10,K11,K12,NO, NO, NO,       K16,K17,K15,      NO, NO, K2D, \
    K2A,K1E,K1F,K20,K21,K22,K23,K24,K25,K26,K27,K28,K29,NO, K2B,  NO, NO, NO,   NO, K2E,K2F,NO,  \
    K35,K36,K37,K38,K39,K3A,K3B,K3C,K3D,K3E,K3F,K40,K41,    K58,  K42,NO, NO,   K44,K45,K46,NO,  \
    K77,K4D,K4E,K4F,K50,K51,K52,K53,K54,K55,K56,K57,    NO, K59,                K5B,K5C,K5D,NO,  \
    K63,NO, K64,K65,K66,K67,K68,K69,K6A,K6B,K6C,K6D,    K6F,K6E,      NO,       K70,K71,K72,NO,  \
    K4C,K78,K13,NO,         K79,        NO, NO, NO, K7A,NO, NO,   NO, NO, NO,   NO,     NO, NO   \
)

#endif
