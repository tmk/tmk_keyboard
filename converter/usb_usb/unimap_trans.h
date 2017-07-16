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
 * |Shft|  <|  Z|  X|  C|  V|  B|  N|  M|  ,|  .|  /| RO|Shift |     |Up |     |  1|  2|  3|Ent|
 * |-----------------------------------------------------------| ,-----------. |---------------|
 * |Ctl|Gui|Alt|MHEN|     Space      |HENK|KANA|Alt|Gui|App|Ctl| |Lef|Dow|Rig| |  0    |  .|KP=|
 * `-----------------------------------------------------------' `-----------' `---------------'
 */
const uint8_t PROGMEM unimap_trans[MATRIX_ROWS][MATRIX_COLS] = {
    { UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO,    UNIMAP_A,     UNIMAP_B,     UNIMAP_C,     UNIMAP_D,
      UNIMAP_E,     UNIMAP_F,     UNIMAP_G,     UNIMAP_H,     UNIMAP_I,     UNIMAP_J,     UNIMAP_K,     UNIMAP_L },
    { UNIMAP_M,     UNIMAP_N,     UNIMAP_O,     UNIMAP_P,     UNIMAP_Q,     UNIMAP_R,     UNIMAP_S,     UNIMAP_T,
      UNIMAP_U,     UNIMAP_V,     UNIMAP_W,     UNIMAP_X,     UNIMAP_Y,     UNIMAP_Z,     UNIMAP_1,     UNIMAP_2 },
    { UNIMAP_3,     UNIMAP_4,     UNIMAP_5,     UNIMAP_6,     UNIMAP_7,     UNIMAP_8,     UNIMAP_9,     UNIMAP_0,
      UNIMAP_ENTER, UNIMAP_ESCAPE,UNIMAP_BSPACE,UNIMAP_TAB,   UNIMAP_SPACE, UNIMAP_MINUS, UNIMAP_EQUAL, UNIMAP_LBRACKET },
    { UNIMAP_RBRACKET,UNIMAP_BSLASH,UNIMAP_NONUS_HASH, UNIMAP_SCOLON, UNIMAP_QUOTE, UNIMAP_GRAVE, UNIMAP_COMMA, UNIMAP_DOT,
      UNIMAP_SLASH, UNIMAP_CAPSLOCK,UNIMAP_F1,  UNIMAP_F2,    UNIMAP_F3,    UNIMAP_F4,    UNIMAP_F5,    UNIMAP_F6 },
    { UNIMAP_F7,    UNIMAP_F8,    UNIMAP_F9,    UNIMAP_F10,   UNIMAP_F11,   UNIMAP_F12,   UNIMAP_PSCREEN,UNIMAP_SCROLLLOCK,
      UNIMAP_PAUSE, UNIMAP_INSERT,UNIMAP_HOME,  UNIMAP_PGUP,  UNIMAP_DELETE,UNIMAP_END,   UNIMAP_PGDOWN, UNIMAP_RIGHT },
    { UNIMAP_LEFT,  UNIMAP_DOWN,  UNIMAP_UP, UNIMAP_NUMLOCK, UNIMAP_KP_SLASH, UNIMAP_KP_ASTERISK, UNIMAP_KP_MINUS, UNIMAP_KP_PLUS,
      UNIMAP_KP_ENTER,UNIMAP_KP_1,UNIMAP_KP_2,  UNIMAP_KP_3,  UNIMAP_KP_4,  UNIMAP_KP_5,  UNIMAP_KP_6,  UNIMAP_KP_7 },
    { UNIMAP_KP_8,  UNIMAP_KP_9,  UNIMAP_KP_0,  UNIMAP_KP_DOT,UNIMAP_NONUS_BSLASH,UNIMAP_APPLICATION,UNIMAP_KP_COMMA,UNIMAP_KP_EQUAL,
      UNIMAP_F13,   UNIMAP_F14,   UNIMAP_F15,   UNIMAP_F16,   UNIMAP_F17,   UNIMAP_F18,   UNIMAP_F19,   UNIMAP_F20 },
    { UNIMAP_F21,   UNIMAP_F22,   UNIMAP_F23,   UNIMAP_F24,   UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO,
      UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO,    UNIMAP_MUTE  }, /* 78-7F */ \
    { UNIMAP_VOLU,  UNIMAP_VOLD,  UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO,    UNIMAP_RO,      /* 80-87 */ \
      UNIMAP_KANA,  UNIMAP_JYEN,  UNIMAP_HENK,  UNIMAP_MHEN,  UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO    }, /* 88-8F */ \
    { UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO,      /* 90-97 */ \
      UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO    }, /* 98-9F */ \
    { UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO,      /* A0-A7 */ \
      UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO    }, /* A8-AF */ \
    { UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO,      /* B0-B7 */ \
      UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO    }, /* B8-BF */ \
    { UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO,      /* C0-C7 */ \
      UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO    }, /* C8-CF */ \
    { UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO,      /* D0-D7 */ \
      UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO    }, /* D8-DF */ \
    { UNIMAP_LCTRL, UNIMAP_LSHIFT,UNIMAP_LALT,  UNIMAP_LGUI,  UNIMAP_RCTRL, UNIMAP_RSHIFT,UNIMAP_RALT,  UNIMAP_RGUI,    /* E0-E7 */ \
      UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO    }, /* E8-EF */ \
    { UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO,      /* F0-F7 */ \
      UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO,    UNIMAP_NO    }, /* F8-FF */ \
};

#endif
