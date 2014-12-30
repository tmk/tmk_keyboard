/*
Copyright 2014 Ralf Schmitt <ralf@bunkertor.net>

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

#ifndef KEYMAP_COMMON_H
#define KEYMAP_COMMON_H

#include <stdint.h>
#include <avr/pgmspace.h>
#include "keycode.h"
#include "action.h"
#include "action_macro.h"
#include "report.h"
#include "host.h"
#include "print.h"
#include "debug.h"
#include "keymap.h"

extern const uint8_t keymaps[][MATRIX_ROWS][MATRIX_COLS];
extern const uint16_t fn_actions[];

#define KEYMAP( \
        K08, K01, K10, K11, K20, K21, K30, K31, K40, K41, K50, K51, K60, K61, \
        K02, K03, K12, K13, K22, K23, K32, K33, K42, K43, K52, K53, K62, K63, \
        K04, K14, K15, K24, K25, K34, K35, K44, K45, K54, K55, K64, K71, K65, \
        K07, K79, K16, K17, K26, K27, K36, K37, K46, K47, K56, K57, K66, K67, \
        K06, K05, K78,                K70,                K72, K73, K74, K75  \
) { \
/*          0         1         2         3         4         5         6         7         8         9      */\
/* 0 */ { KC_NO,    KC_##K01, KC_##K02, KC_##K03, KC_##K04, KC_##K05, KC_##K06, KC_##K07, KC_##K08, KC_NO },   \
/* 1 */ { KC_##K10, KC_##K11, KC_##K12, KC_##K13, KC_##K14, KC_##K15, KC_##K16, KC_##K17, KC_NO,    KC_NO },   \
/* 2 */ { KC_##K20, KC_##K21, KC_##K22, KC_##K23, KC_##K24, KC_##K25, KC_##K26, KC_##K27, KC_NO,    KC_NO },   \
/* 3 */ { KC_##K30, KC_##K31, KC_##K32, KC_##K33, KC_##K34, KC_##K35, KC_##K36, KC_##K37, KC_NO,    KC_NO },   \
/* 4 */ { KC_##K40, KC_##K41, KC_##K42, KC_##K43, KC_##K44, KC_##K45, KC_##K46, KC_##K47, KC_NO,    KC_NO },   \
/* 5 */ { KC_##K50, KC_##K51, KC_##K52, KC_##K53, KC_##K54, KC_##K55, KC_##K56, KC_##K57, KC_NO,    KC_NO },   \
/* 6 */ { KC_##K60, KC_##K61, KC_##K62, KC_##K63, KC_##K64, KC_##K65, KC_##K66, KC_##K67, KC_NO,    KC_NO },   \
/* 7 */ { KC_##K70, KC_##K71, KC_##K72, KC_##K73, KC_##K74, KC_##K75, KC_NO,    KC_NO,    KC_##K78, KC_##K79 } \
}

#define KEYMAP_ANSI150( \
        K08, K01, K10, K11, K20, K21, K30, K31, K40, K41, K50, K51, K60, K61, \
        K02, K03, K12, K13, K22, K23, K32, K33, K42, K43, K52, K53, K62, K63, \
        K04, K14, K15, K24, K25, K34, K35, K44, K45, K54, K55, K64,      K65, \
        K07,      K16, K17, K26, K27, K36, K37, K46, K47, K56, K57, K66,      \
        K06, K05, K78,                K70,                     K73, K74, K75  \
) KEYMAP( \
        K08, K01, K10, K11, K20, K21, K30, K31, K40, K41, K50, K51, K60, K61, \
        K02, K03, K12, K13, K22, K23, K32, K33, K42, K43, K52, K53, K62, K63, \
        K04, K14, K15, K24, K25, K34, K35, K44, K45, K54, K55, K64, NO,  K65, \
        K07, NO,  K16, K17, K26, K27, K36, K37, K46, K47, K56, K57, K66, NO,  \
        K06, K05, K78,                K70,                NO,  K73, K74, K75  \
)

#endif // KEYMAP_COMMON_H
