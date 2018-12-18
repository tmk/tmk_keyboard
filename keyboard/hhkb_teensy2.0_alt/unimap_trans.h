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
#ifndef UNIMAP_TRANS_H
#define UNIMAP_TRANS_H

#include <stdint.h>
#include <stdbool.h>
#include "keycode.h"
#include "action.h"
#include "action_code.h"
#include "action_layer.h"
#include "action_macro.h"
#include "action_util.h"
#include "report.h"
#include "host.h"
#include "print.h"
#include "debug.h"
#include "unimap.h"


#define UNIMAP_HHKB( \
    K35,K1E,K1F,K20,K21,K22,K23,K24,K25,K26,K27,K2D,K2E,K74,K2A, \
    K2B,K14,K1A,K08,K15,K17,K1C,K18,K0C,K12,K13,K2F,K30,    K31, \
    K39,K04,K16,K07,K09,K0A,K0B,K0D,K0E,K0F,K33,K34,        K28, \
    K79,    K1D,K1B,K06,K19,K05,K11,K10,K36,K37,K38,    K7D,K7C, \
        K7A,K7B,            K2C,                K7F,K7E          \
) UNIMAP( \
            NO, NO, NO, NO, NO, NO, NO, NO, NO, NO, NO, NO,                                     \
    NO,     NO, NO, NO, NO, NO, NO, NO, NO, NO, NO, NO, NO,       NO, NO, NO,       NO, NO, NO, \
    K35,K1E,K1F,K20,K21,K22,K23,K24,K25,K26,K27,K2D,K2E,K74,K2A,  NO, NO, NO,   NO, NO, NO, NO, \
    K2B,K14,K1A,K08,K15,K17,K1C,K18,K0C,K12,K13,K2F,K30,    K31,  NO, NO, NO,   NO, NO, NO, NO, \
    K39,K04,K16,K07,K09,K0A,K0B,K0D,K0E,K0F,K33,K34,    NO, K28,                NO, NO, NO, NO, \
    K79,NO, K1D,K1B,K06,K19,K05,K11,K10,K36,K37,K38,    NO, K7D,      NO,       NO, NO, NO, NO, \
    NO, K7B,K7A,NO,         K2C,        NO, NO, K7E,K7F,NO, K7C,  NO, NO, NO,   NO,     NO, NO  \
)

#define UNIMAP_JP( \
    K35,K1E,K1F,K20,K21,K22,K23,K24,K25,K26,K27,K2D,K2E,K74,K2A,  \
    K2B,K14,K1A,K08,K15,K17,K1C,K18,K0C,K12,K13,K2F,K30,          \
    K39,K04,K16,K07,K09,K0A,K0B,K0D,K0E,K0F,K33,K34,K32,    K28,  \
    K79,    K1D,K1B,K06,K19,K05,K11,K10,K36,K37,K38,K75,K52,K7D, \
    K78,K29,K7B,K7A,K77,    K2C,    K76,K00,K7E,K7C,K50,K51,K4F  \
) UNIMAP( \
            NO, NO, NO, NO, NO, NO, NO, NO, NO, NO, NO, NO,                                     \
    K29,    NO, NO, NO, NO, NO, NO, NO, NO, NO, NO, NO, NO,       NO, NO, NO,       NO, NO, NO, \
    K35,K1E,K1F,K20,K21,K22,K23,K24,K25,K26,K27,K2D,K2E,K74,K2A,  NO, NO, NO,   NO, NO, NO, NO, \
    K2B,K14,K1A,K08,K15,K17,K1C,K18,K0C,K12,K13,K2F,K30,    NO,   NO, NO, NO,   NO, NO, NO, NO, \
    K39,K04,K16,K07,K09,K0A,K0B,K0D,K0E,K0F,K33,K34,    K32,K28,                NO, NO, NO, NO, \
    K79,NO, K1D,K1B,K06,K19,K05,K11,K10,K36,K37,K38,    K75,K7D,      K52,      NO, NO, NO, NO, \
    K78,K7B,K7A,K77,        K2C,        K76,K00,K7E,NO, NO, K7C,  K50,K51,K4F,  NO,     NO, NO  \
)


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
 */

#ifndef HHKB_JP
/* HHKB
 * ,-----------------------------------------------------------.
 * |  `|  1|  2|  3|  4|  5|  6|  7|  8|  9|  0|  -|  =|JPY|BS |
 * |-----------------------------------------------------------|
 * |Tab  |  Q|  W|  E|  R|  T|  Y|  U|  I|  O|  P|  [|  ]|   \ |
 * |-----------------------------------------------------------|
 * |CapsL |  A|  S|  D|  F|  G|  H|  J|  K|  L|  ;|  '|Enter   |
 * |-----------------------------------------------------------|
 * |Shift   |  Z|  X|  C|  V|  B|  N|  M|  ,|  .|  /|Shift |Ctl|
 * `-----------------------------------------------------------'
 *       |Gui|Alt  |         Space         |Alt  |Gui|
 *       `-------------------------------------------'
 *
 * Matrix - row:8 x col:8
 * ,-----------------------------------------------------------.
 * |31 |30 |00 |10 |11 |20 |21 |40 |41 |60 |61 |70 |71 |50 |51 |
 * |-----------------------------------------------------------|
 * |32   |01 |02 |13 |12 |23 |22 |42 |43 |62 |63 |73 |72 |52   |
 * |-----------------------------------------------------------|
 * |33    |04 |03 |14 |15 |24 |25 |45 |44 |65 |64 |74 |     53 |
 * |-----------------------------------------------------------|
 * |34      |05 |06 |07 |16 |17 |26 |46 |66 |76 |75 |55    |54 |
 * `-----------------------------------------------------------'
 *       |35 |36   |          37           |57   |56 |
 *       `-------------------------------------------'
 */

const uint8_t PROGMEM unimap_trans[MATRIX_ROWS][MATRIX_COLS] = {
    { UNIMAP_2, UNIMAP_Q, UNIMAP_W, UNIMAP_S, UNIMAP_A, UNIMAP_Z, UNIMAP_X, UNIMAP_C, },
    { UNIMAP_3, UNIMAP_4, UNIMAP_R, UNIMAP_E, UNIMAP_D, UNIMAP_F, UNIMAP_V, UNIMAP_B, },
    { UNIMAP_5, UNIMAP_6, UNIMAP_Y, UNIMAP_T, UNIMAP_G, UNIMAP_H, UNIMAP_N, UNIMAP_NO, },
    { UNIMAP_1, UNIMAP_GRAVE, UNIMAP_TAB, UNIMAP_CAPSLOCK, UNIMAP_LSHIFT, UNIMAP_LALT, UNIMAP_LGUI, UNIMAP_SPACE, },
    { UNIMAP_7, UNIMAP_8, UNIMAP_U, UNIMAP_I, UNIMAP_K, UNIMAP_J, UNIMAP_M, UNIMAP_NO, },
    { UNIMAP_JYEN, UNIMAP_BSPACE, UNIMAP_BSLASH, UNIMAP_ENTER, UNIMAP_RCTRL, UNIMAP_RSHIFT, UNIMAP_RALT, UNIMAP_RGUI, },
    { UNIMAP_9, UNIMAP_0, UNIMAP_O, UNIMAP_P, UNIMAP_SCOLON, UNIMAP_L, UNIMAP_COMMA, UNIMAP_NO, },
    { UNIMAP_MINUS, UNIMAP_EQUAL, UNIMAP_RBRACKET, UNIMAP_LBRACKET, UNIMAP_QUOTE, UNIMAP_SLASH, UNIMAP_DOT, UNIMAP_NO, },
};

#else
/* HHKB JP
 * ,-----------------------------------------------------------.
 * |`  |1  |2  |3  |4  |5  |6  |7  |8  |9  |0  |-  |=  |JPY|BS |
 * |-----------------------------------------------------------|
 * |Tab  |Q  |W  |E  |R  |T  |Y  |U  |I  |O  |P  |[  |]  |     |
 * |------------------------------------------------------`    |
 * |CapsL |A  |S  |D  |F  |G  |H  |J  |K  |L  |;  |'  |#  |Entr|
 * |-----------------------------------------------------------|
 * |Shift  |Z  |X  |C  |V  |B  |N  |M  |,  |.  |/  |RO |Up |Shi|
 * |-----------------------------------------------------------|
 * |Ctl||Esc|Gui|Alt|MHE|         |HEN|KAN|Alt|Ctl||Lef|Dow|Rig|
 * `-----------------------------------------------------------'
 *
 * Matrix -  row:16 x col:8
 * ,-----------------------------------------------------------.
 * |02 |32 |62 |22 |12 |52 |72 |A2 |92 |82 |B2 |E2 |F2 |D2 |C2 |
 * |-----------------------------------------------------------|
 * |03   |63 |23 |13 |53 |73 |A3 |93 |83 |B3 |E3 |F3 |D3 |     |
 * |------------------------------------------------------`    |
 * |06    |66 |26 |16 |56 |76 |A6 |96 |86 |B6 |E6 |F6 |D6 | C6 |
 * |-----------------------------------------------------------|
 * |05     |65 |25 |15 |55 |75 |A5 |95 |85 |B5 |E5 |F5 |D5 |C5 |
 * |-----------------------------------------------------------|
 * |04 ||34 |64 |24 |14 |     74  |94 |84 |B4 |E4 ||F4 |D4 |C4 |
 * `-----------------------------------------------------------'
 */
const uint8_t PROGMEM unimap_trans[MATRIX_ROWS][MATRIX_COLS] = {
    { UNIMAP_NO, UNIMAP_NO, UNIMAP_GRAVE, UNIMAP_TAB, UNIMAP_LCTRL, UNIMAP_LSHIFT, UNIMAP_CAPSLOCK, UNIMAP_NO, },
    { UNIMAP_NO, UNIMAP_NO, UNIMAP_4, UNIMAP_E, UNIMAP_MHEN, UNIMAP_C, UNIMAP_D, UNIMAP_NO, },
    { UNIMAP_NO, UNIMAP_NO, UNIMAP_3, UNIMAP_W, UNIMAP_LALT, UNIMAP_X, UNIMAP_S, UNIMAP_NO, },
    { UNIMAP_NO, UNIMAP_NO, UNIMAP_1, UNIMAP_NO, UNIMAP_ESCAPE, UNIMAP_NO, UNIMAP_NO, UNIMAP_NO, },
    { UNIMAP_NO, UNIMAP_NO, UNIMAP_NO, UNIMAP_NO, UNIMAP_NO, UNIMAP_NO, UNIMAP_NO, UNIMAP_NO, },
    { UNIMAP_NO, UNIMAP_NO, UNIMAP_5, UNIMAP_R, UNIMAP_NO, UNIMAP_V, UNIMAP_F, UNIMAP_NO, },
    { UNIMAP_NO, UNIMAP_NO, UNIMAP_2, UNIMAP_Q, UNIMAP_LGUI, UNIMAP_Z, UNIMAP_A, UNIMAP_NO, },
    { UNIMAP_NO, UNIMAP_NO, UNIMAP_6, UNIMAP_T, UNIMAP_SPACE, UNIMAP_B, UNIMAP_G, UNIMAP_NO, },
    { UNIMAP_NO, UNIMAP_NO, UNIMAP_9, UNIMAP_I, UNIMAP_KANA, UNIMAP_COMMA, UNIMAP_K, UNIMAP_NO, },
    { UNIMAP_NO, UNIMAP_NO, UNIMAP_8, UNIMAP_U, UNIMAP_HENK, UNIMAP_M, UNIMAP_J, UNIMAP_NO, },
    { UNIMAP_NO, UNIMAP_NO, UNIMAP_7, UNIMAP_Y, UNIMAP_NO, UNIMAP_N, UNIMAP_H, UNIMAP_NO, },
    { UNIMAP_NO, UNIMAP_NO, UNIMAP_0, UNIMAP_O, UNIMAP_RALT, UNIMAP_DOT, UNIMAP_L, UNIMAP_NO, },
    { UNIMAP_NO, UNIMAP_NO, UNIMAP_BSPACE, UNIMAP_NO, UNIMAP_RIGHT, UNIMAP_RSHIFT, UNIMAP_ENTER, UNIMAP_NO, },
    { UNIMAP_NO, UNIMAP_NO, UNIMAP_JYEN, UNIMAP_RBRACKET, UNIMAP_DOWN, UNIMAP_UP, UNIMAP_NONUS_HASH, UNIMAP_NO, },
    { UNIMAP_NO, UNIMAP_NO, UNIMAP_MINUS, UNIMAP_P, UNIMAP_RCTRL, UNIMAP_SLASH, UNIMAP_SCOLON, UNIMAP_NO, },
    { UNIMAP_NO, UNIMAP_NO, UNIMAP_EQUAL, UNIMAP_LBRACKET, UNIMAP_LEFT, UNIMAP_RO, UNIMAP_QUOTE, UNIMAP_NO, },
};
#endif

#endif
