/*
Copyright 2015 Vinícius Nery Cordeiro <vinicius.nery.cordeiro@gmail.com>

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

#include "keymap_common.h"

const uint8_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
/*

┌────┬────┬────┬────┐
│NLCK│ /  │ *  │ -  │
├────┼────┼────┼────┤
│ 7  │ 8  │ 9  │    │
├────┼────┼────┤ +  │
│ 4  │ 5  │ 6  │    │
├────┼────┼────┼────┤
│ 1  │ 2  │ 3  │ EN-│
├────┴────┼────┤ TER│
│    0    │ .  │    │
└─────────┴────┴────┘

*/
    KEYMAP(
        NUMLOCK, KP_SLASH, KP_ASTERISK, KP_MINUS, \
        KP_7,    KP_8,     KP_9,                  \
        KP_4,    KP_5,     KP_6,        KP_PLUS,  \
        KP_1,    KP_2,     KP_3,                  \
        KP_0,              KP_DOT,      KP_ENTER),
};
const uint16_t PROGMEM fn_actions[] = {};
