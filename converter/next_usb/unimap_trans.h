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
#ifndef UNIMAP_COMMON_H
#define UNIMAP_COMMON_H

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
 * NeXT Matrix(Scan code)
 * ,-----------------------------------------------------------. ,-----------. ,---------------.
 * | 49| 4A| 4B| 4C| 4D| 50| 4F| 4E| 1E| 1F| 20| 1D| 1C|   1B  | | 1A| 58| 19| | 26| 27| 28| 25|
 * |-----------------------------------------------------------| |-----------| |---------------|
 * | 41  | 42| 43| 44| 45| 48| 47| 46| 06| 07| 08| 05| 04| 03  | | 02|   | 01| | 21| 22| 23| 24|
 * |-----------------------------------------------------------| `---'   `---' |-----------|---|
 * | 57   | 39| 3A| 3B| 3C| 3D| 40| 3F| 3E| 2D| 2C| 2B|   2A   |               | 12| 18| 13| 15|
 * |-----------------------------------------------------------|     ,---.     |---------------|
 * | 56     | 31| 32| 33| 34| 35| 37| 36| 2E| 2F| 30|  55      |     | 16|     | 11| 17| 14|   |
 * |-----------------------------------------------------------| ,-----------. |-----------|   |
 * | 52  | 54  |                38                 | 53  | 51  | | 09| 0F| 10| |0B     | 0C| 0D|
 * `-----------------------------------------------------------' `-----------' `---------------'
 * ,-----------------------------------------------------------. ,-----------. ,---------------.
 * |  `|  1|  2|  3|  4|  5|  6|  7|  8|  9|  0|  -|  =| BSpc  | |Ins|Hom|PgU| |NmL|  /|  *|  -|
 * |-----------------------------------------------------------| |-----------| |---------------|
 * |Tab  |  Q|  W|  E|  R|  T|  Y|  U|  I|  O|  P|  [|  ]|  \  | |Del|   |PgD| |  7|  8|  9|  +|
 * |-----------------------------------------------------------| `---'   `---' |---------------|
 * |CapsL |  A|  S|  D|  F|  G|  H|  J|  K|  L|  ;|  '|  Return|               |  4|  5|  6|KP,|
 * |-----------------------------------------------------------|     ,---.     |---------------|
 * |Shft|  <|  Z|  X|  C|  V|  B|  N|  M|  ,|  .|  /|    Shift |     |Up |     |  1|  2|  3|   |
 * |-----------------------------------------------------------| ,-----------. |-----------|   |
 * |LCtl |Gui  |              Space                | App | RCtl| |Lef|Dow|Rig| |  0    |  .|Ent|
 * `-----------------------------------------------------------' `-----------' `---------------'
 */

// Matrix 12 * 8
const uint8_t PROGMEM unimap_trans[MATRIX_ROWS][MATRIX_COLS] = {
    { UNIMAP_NO,   UNIMAP_PGDN, UNIMAP_DEL,  UNIMAP_BSLS, UNIMAP_RBRC, UNIMAP_LBRC, UNIMAP_I,    UNIMAP_O },    // 0x00-07
    { UNIMAP_P,    UNIMAP_LEFT, UNIMAP_NO,   UNIMAP_P0,   UNIMAP_PDOT, UNIMAP_PENT, UNIMAP_NO,   UNIMAP_DOWN }, // 0x08-0F
    { UNIMAP_RGHT, UNIMAP_P1,   UNIMAP_P4,   UNIMAP_P6,   UNIMAP_P3,   UNIMAP_PCMM, UNIMAP_UP,   UNIMAP_P2 },   // 0x10-17
    { UNIMAP_P5,   UNIMAP_PGUP, UNIMAP_INS,  UNIMAP_BSPC, UNIMAP_EQL,  UNIMAP_MINS, UNIMAP_8,    UNIMAP_9 },    // 0x18-1f
    { UNIMAP_0,    UNIMAP_P7,   UNIMAP_P8,   UNIMAP_P9,   UNIMAP_PPLS, UNIMAP_PMNS, UNIMAP_NLCK, UNIMAP_PSLS }, // 0x20-27
    { UNIMAP_PAST, UNIMAP_NO,   UNIMAP_ENT,  UNIMAP_QUOT, UNIMAP_SCLN, UNIMAP_L,    UNIMAP_COMM, UNIMAP_DOT },  // 0x28-2f
    { UNIMAP_SLSH, UNIMAP_Z,    UNIMAP_X,    UNIMAP_C,    UNIMAP_V,    UNIMAP_B,    UNIMAP_M,    UNIMAP_N },    // 0x30-37
    { UNIMAP_SPC,  UNIMAP_A,    UNIMAP_S,    UNIMAP_D,    UNIMAP_F,    UNIMAP_G,    UNIMAP_K,    UNIMAP_J },    // 0x38-3f
    { UNIMAP_H,    UNIMAP_TAB,  UNIMAP_Q,    UNIMAP_W,    UNIMAP_E,    UNIMAP_R,    UNIMAP_U,    UNIMAP_Y },    // 0x40-47
    { UNIMAP_T,    UNIMAP_GRV,  UNIMAP_1,    UNIMAP_2,    UNIMAP_3,    UNIMAP_4,    UNIMAP_7,    UNIMAP_6 },    // 0x48-4f
    { UNIMAP_5,    UNIMAP_RCTL, UNIMAP_LCTL, UNIMAP_APP,  UNIMAP_LGUI, UNIMAP_RSFT, UNIMAP_LSFT, UNIMAP_CAPS }, // 0x50-57
    { UNIMAP_HOME, UNIMAP_NO,   UNIMAP_NO,   UNIMAP_NO,   UNIMAP_NO,   UNIMAP_NO,   UNIMAP_NO,   UNIMAP_NO }    // 0x58-5f
};

#endif
