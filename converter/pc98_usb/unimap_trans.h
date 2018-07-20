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
#ifndef _TRANS_H
#define _TRANS_H

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
 * |Shft|  <|  Z|  X|  C|  V|  B|  N|  M|  ,|  .|  /| RO|Shift |     |Up |     |  1|  2|  3|Ent|
 * |-----------------------------------------------------------| ,-----------. |---------------|
 * |Ctl|Gui|Alt|MHEN|     Space      |HENK|KANA|Alt|Gui|App|Ctl| |Lef|Dow|Rig| |  0    |  .|KP=|
 * `-----------------------------------------------------------' `-----------' `---------------'
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
    { UNIMAP_NO,   UNIMAP_NO,   UNIMAP_NO,   UNIMAP_NO,   UNIMAP_NO,   UNIMAP_NO,   UNIMAP_APP,  UNIMAP_NO    }, /* 58-5F */
    { UNIMAP_PAUS, UNIMAP_PSCR, UNIMAP_F1,   UNIMAP_F2,   UNIMAP_F3,   UNIMAP_F4,   UNIMAP_F5,   UNIMAP_F6    }, /* 60-67 */
    { UNIMAP_F7,   UNIMAP_F8,   UNIMAP_F9,   UNIMAP_F10,  UNIMAP_NO,   UNIMAP_NO,   UNIMAP_NO,   UNIMAP_NO    }, /* 68-6F */
    { UNIMAP_LSFT, UNIMAP_CAPS, UNIMAP_KANA, UNIMAP_LALT, UNIMAP_LCTL, UNIMAP_NO,   UNIMAP_NO,   UNIMAP_LGUI  }, /* 70-77 */
    { UNIMAP_RGUI, UNIMAP_RALT, UNIMAP_NO,   UNIMAP_NO,   UNIMAP_NO,   UNIMAP_RSFT, UNIMAP_NO,   UNIMAP_NO    }  /* 78-7F */
};


/* PC-9801/9821 keyboard
,---. ,---. ,------------------------. ,------------------------. ,------------------------.
|Stp| |PrS| | F1 | F2 | F3 | F4 | F5 | | F6 | F7 | F8 | F9 | F10| | F11| F12| F13| F14| F15|
`---' `---' `------------------------' `------------------------' `------------------------'
,-------------------------------------------------------------. ,-------.  ,---------------.
| Esc|  1|  2|  3|  4|  5|  6|  7|  8|  9|  0|  -|  =|JPY|Bspc| |Ins|Del|  |Hom|End|  -|  /|
|-------------------------------------------------------------| |-------|  |---------------|
| Tab  |  Q|  W|  E|  R|  T|  Y|  U|  I|  O|  P|  [|  ]|      | |PgD|PgU|  |  7|  8|  9|  *|
|-------------------------------------------------------`Enter| |-------|  |---------------|
|Ctl|Cap|  A|  S|  D|  F|  G|  H|  J|  K|  L|  ;|  '|  \|     | |  Up   |  |  4|  5|  6|  +|
|-------------------------------------------------------------| |-------|  |---------------|
| Shift   |  Z|  X|  C|  V|  B|  N|  M|  ,|  .|  /| RO| Shift | |Lef|Rig|  |  1|  2|  3|KP=|
`-------------------------------------------------------------' |-------|  |---------------|
   |Kana|Gui|GRPH|NFER|       Space        |XFER|Alt|Gui|App|^2 | Down  |  |  0|  ,|  .| ^1|
   `--------------------------------------------------------'   `-------'  `---------------'
^1: Enter on keypad is identical to Enter on alphanumeric portion.
^2: Alt is ALT on DIGITAL WAVE Dboard while Menu on PC-9821 keyboard.
    App is HOME key on PC98XL keyboard.
*/
#define UNIMAP_PC98( \
    K48, K46,   K3A,K3B,K3C,K3D,K3E,        K3F,K40,K41,K42,K43,        K44,K45,K68,K69,K6A, \
    K29,K1E,K1F,K20,K21,K22,K23,K24,K25,K26,K27,K2D,K2E,K74,K2A,  K49,K4C,  K4A,K4D,K56,K54, \
    K2B,    K14,K1A,K08,K15,K17,K1C,K18,K0C,K12,K13,K2F,K30,      K4B,K4E,  K5F,K60,K61,K55, \
    K78,K39,K04,K16,K07,K09,K0A,K0B,K0D,K0E,K0F,K33,K34,K31,K28,    K52,    K5C,K5D,K5E,K57, \
    K79,    K1D,K1B,K06,K19,K05,K11,K10,K36,K37,K38,K75,    K7D,  K50,K4F,  K59,K5A,K5B,K67, \
        K00,K7B,K7A,K77,        K2C,        K76,K7E,K7F,K65,        K51,    K62,K66,K63,K58  \
) { \
    { AC_##K00, AC_NO,    AC_NO,    AC_NO,    AC_##K04, AC_##K05, AC_##K06, AC_##K07,   /* 00-07 */ \
      AC_##K08, AC_##K09, AC_##K0A, AC_##K0B, AC_##K0C, AC_##K0D, AC_##K0E, AC_##K0F }, /* 08-0F */ \
    { AC_##K10, AC_##K11, AC_##K12, AC_##K13, AC_##K14, AC_##K15, AC_##K16, AC_##K17,   /* 10-17 */ \
      AC_##K18, AC_##K19, AC_##K1A, AC_##K1B, AC_##K1C, AC_##K1D, AC_##K1E, AC_##K1F }, /* 18-1F */ \
    { AC_##K20, AC_##K21, AC_##K22, AC_##K23, AC_##K24, AC_##K25, AC_##K26, AC_##K27,   /* 20-27 */ \
      AC_##K28, AC_##K29, AC_##K2A, AC_##K2B, AC_##K2C, AC_##K2D, AC_##K2E, AC_##K2F }, /* 28-2F */ \
    { AC_##K30, AC_##K31, AC_NO,    AC_##K33, AC_##K34, AC_NO,    AC_##K36, AC_##K37,   /* 30-37 */ \
      AC_##K38, AC_##K39, AC_##K3A, AC_##K3B, AC_##K3C, AC_##K3D, AC_##K3E, AC_##K3F }, /* 38-3F */ \
    { AC_##K40, AC_##K41, AC_##K42, AC_##K43, AC_##K44, AC_##K45, AC_##K46, AC_NO,      /* 40-47 */ \
      AC_##K48, AC_##K49, AC_##K4A, AC_##K4B, AC_##K4C, AC_##K4D, AC_##K4E, AC_##K4F }, /* 48-4F */ \
    { AC_##K50, AC_##K51, AC_##K52, AC_NO,    AC_##K54, AC_##K55, AC_##K56, AC_##K57,   /* 50-57 */ \
      AC_##K58, AC_##K59, AC_##K5A, AC_##K5B, AC_##K5C, AC_##K5D, AC_##K5E, AC_##K5F }, /* 58-5F */ \
    { AC_##K60, AC_##K61, AC_##K62, AC_##K63, AC_NO,    AC_##K65, AC_##K66, AC_##K67,   /* 60-67 */ \
      AC_##K68, AC_##K69, AC_##K6A, AC_NO,    AC_NO,    AC_NO,    AC_NO,    AC_NO,   }, /* 68-6F */ \
    { AC_NO,    AC_NO,    AC_NO,    AC_NO,    AC_##K74, AC_##K75, AC_##K76, AC_##K77,   /* 70-77 */ \
      AC_##K78, AC_##K79, AC_##K7A, AC_##K7B, AC_NO,    AC_##K7D, AC_##K7E, AC_##K7F }  /* 78-7F */ \
}

#endif
