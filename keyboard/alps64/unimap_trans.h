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


#define UNIMAP_ALPS64( \
    K35,K1E,K1F,K20,K21,K22,K23,K24,K25,K26,K27,K2D,K2E,K74,K2A, \
    K2B,K14,K1A,K08,K15,K17,K1C,K18,K0C,K12,K13,K2F,K30,    K31, \
    K39,K04,K16,K07,K09,K0A,K0B,K0D,K0E,K0F,K33,K34,        K28, \
    K79,K64,K1D,K1B,K06,K19,K05,K11,K10,K36,K37,K38,    K7D,K29, \
    K78,K7B,K7A,            K2C,                K65,K7E,K7F,K7C  \
) UNIMAP( \
            NO, NO, NO, NO, NO, NO, NO, NO, NO, NO, NO, NO,                                     \
    K29,    NO, NO, NO, NO, NO, NO, NO, NO, NO, NO, NO, NO,       NO, NO, NO,       NO, NO, NO, \
    K35,K1E,K1F,K20,K21,K22,K23,K24,K25,K26,K27,K2D,K2E,K74,K2A,  NO, NO, NO,   NO, NO, NO, NO, \
    K2B,K14,K1A,K08,K15,K17,K1C,K18,K0C,K12,K13,K2F,K30,    K31,  NO, NO, NO,   NO, NO, NO, NO, \
    K39,K04,K16,K07,K09,K0A,K0B,K0D,K0E,K0F,K33,K34,    NO, K28,                NO, NO, NO, NO, \
    K79,K64,K1D,K1B,K06,K19,K05,K11,K10,K36,K37,K38,    NO, K7D,      NO,       NO, NO, NO, NO, \
    K78,K7B,K7A,NO,         K2C,        NO, NO, K7E,K7F,K65,K7C,  NO, NO, NO,   NO,     NO, NO  \
)


/* Mapping to Universal keyboard layout
 * ,-----------------------------------------------------------.
 * |`  |  1|  2|  3|  4|  5|  6|  7|  8|  9|  0|  -|  =|JPY|Bsp|
 * |-----------------------------------------------------------|
 * |Tab  |  Q|  W|  E|  R|  T|  Y|  U|  I|  O|  P|  [|  ]|  \  |
 * |-----------------------------------------------------------|
 * |Caps  |  A|  S|  D|  F|  G|  H|  J|  K|  L|  ;|  '|Enter   |
 * |-----------------------------------------------------------|
 * |Shft|  \|  Z|  X|  C|  V|  B|  N|  M|  ,|  .|  /|Shift |Esc|
 * |-----------------------------------------------------------'
 * |Ctrl|Gui |Alt |         Space         |App |Alt |Gui |Ctrl |
 * `-----------------------------------------------------------'
 *
 * ,-----------------------------------------------------------.
 * |36 |37 |46 |47 |56 |57 |66 |67 |76 |77 |06 |07 |17 |26 |27 |
 * |-----------------------------------------------------------|
 * |34   |35 |44 |45 |54 |55 |64 |65 |75 |05 |15 |16 |25 |24   |
 * |-----------------------------------------------------------|
 * |32    |33 |43 |52 |53 |63 |73 |74 |03 |04 |13 |14 |    23  |
 * |-----------------------------------------------------------|
 * |31  |41 |42 |51 |61 |62 |71 |72 |01 |02 |11 |12 |21    |22 |
 * |-----------------------------------------------------------|
 * |30  |40  |50  |         60            |70  |00  |10  |20   |
 * `-----------------------------------------------------------'
 */
const uint8_t PROGMEM unimap_trans[MATRIX_ROWS][MATRIX_COLS] = {
    { UNIMAP_RALT, UNIMAP_M,    UNIMAP_COMM, UNIMAP_K,    UNIMAP_L,    UNIMAP_O,    UNIMAP_0,    UNIMAP_MINS }, /* 00-07 */
    { UNIMAP_RGUI, UNIMAP_DOT,  UNIMAP_SLSH, UNIMAP_SCLN, UNIMAP_QUOT, UNIMAP_P,    UNIMAP_LBRC, UNIMAP_EQL  }, /* 10-17 */
    { UNIMAP_RCTL, UNIMAP_RSFT, UNIMAP_ESC,  UNIMAP_ENT,  UNIMAP_BSLS, UNIMAP_RBRC, UNIMAP_JYEN, UNIMAP_BSPC }, /* 20-27 */
    { UNIMAP_LCTL, UNIMAP_LSFT, UNIMAP_CAPS, UNIMAP_A,    UNIMAP_TAB,  UNIMAP_Q,    UNIMAP_GRV,  UNIMAP_1    }, /* 30-37 */
    { UNIMAP_LGUI, UNIMAP_NUBS, UNIMAP_Z,    UNIMAP_S,    UNIMAP_W,    UNIMAP_E,    UNIMAP_2,    UNIMAP_3    }, /* 40-47 */
    { UNIMAP_LALT, UNIMAP_X,    UNIMAP_D,    UNIMAP_F,    UNIMAP_R,    UNIMAP_T,    UNIMAP_4,    UNIMAP_5    }, /* 50-57 */
    { UNIMAP_SPC,  UNIMAP_C,    UNIMAP_V,    UNIMAP_G,    UNIMAP_Y,    UNIMAP_U,    UNIMAP_6,    UNIMAP_7    }, /* 60-67 */
    { UNIMAP_APP,  UNIMAP_B,    UNIMAP_N,    UNIMAP_H,    UNIMAP_J,    UNIMAP_I,    UNIMAP_8,    UNIMAP_9    }, /* 70-77 */
};

#endif


