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


/* HHKB JP
 * ,-----------------------------------------------------------.
 * |`  |1  |2  |3  |4  |5  |6  |7  |8  |9  |0  |-  |=  |JPY|BS |
 * |-----------------------------------------------------------|
 * |Tab  |Q  |W  |E  |R  |T  |Y  |U  |I  |O  |P  |[  |]  |Enter|
 * |------------------------------------------------------`    |
 * |Caps  |A  |S  |D  |F  |G  |H  |J  |K  |L  |;  |'  |#  |    |
 * |-----------------------------------------------------------|
 * |Shift  |Z  |X  |C  |V  |B  |N  |M  |,  |.  |/  |Ro |Up |Shi|
 * |-----------------------------------------------------------|
 * |Ctl||Esc|Gui|Alt|Mhn|         |Hen|Kan|Alt|Ctl||Lef|Dow|Rig|
 * `-----------------------------------------------------------'
 * Esc          = Grave(`)
 * Control      = Caps Lock
 * Left Fn      = Left Control
 * Right Fn     = Right Control
 */
// row:16 x col:8
const uint8_t PROGMEM unimap_trans[MATRIX_ROWS][MATRIX_COLS] = {
    // 0
    {
        UNIMAP_NO,
        UNIMAP_NO,
        UNIMAP_GRAVE,
        UNIMAP_TAB,
        UNIMAP_LCTRL,
        UNIMAP_LSHIFT,
        UNIMAP_CAPSLOCK,
        UNIMAP_NO,
    },
    // 1
    {
        UNIMAP_NO,
        UNIMAP_NO,
        UNIMAP_4,
        UNIMAP_E,
        UNIMAP_MHEN,
        UNIMAP_C,
        UNIMAP_D,
        UNIMAP_NO,
    },
    // 2
    {
        UNIMAP_NO,
        UNIMAP_NO,
        UNIMAP_3,
        UNIMAP_W,
        UNIMAP_LALT,
        UNIMAP_X,
        UNIMAP_S,
        UNIMAP_NO,
    },
    // 3
    {
        UNIMAP_NO,
        UNIMAP_NO,
        UNIMAP_1,
        UNIMAP_NO,
        UNIMAP_ESCAPE,
        UNIMAP_NO,
        UNIMAP_NO,
        UNIMAP_NO,
    },
    // 4
    {
        UNIMAP_NO,
        UNIMAP_NO,
        UNIMAP_NO,
        UNIMAP_NO,
        UNIMAP_NO,
        UNIMAP_NO,
        UNIMAP_NO,
        UNIMAP_NO,
    },
    // 5
    {
        UNIMAP_NO,
        UNIMAP_NO,
        UNIMAP_5,
        UNIMAP_R,
        UNIMAP_NO,
        UNIMAP_V,
        UNIMAP_F,
        UNIMAP_NO,
    },
    // 6
    {
        UNIMAP_NO,
        UNIMAP_NO,
        UNIMAP_2,
        UNIMAP_Q,
        UNIMAP_LGUI,
        UNIMAP_Z,
        UNIMAP_A,
        UNIMAP_NO,
    },
    // 7
    {
        UNIMAP_NO,
        UNIMAP_NO,
        UNIMAP_6,
        UNIMAP_T,
        UNIMAP_SPACE,
        UNIMAP_B,
        UNIMAP_G,
        UNIMAP_NO,
    },
    // 8
    {
        UNIMAP_NO,
        UNIMAP_NO,
        UNIMAP_9,
        UNIMAP_I,
        UNIMAP_KANA,
        UNIMAP_COMMA,
        UNIMAP_K,
        UNIMAP_NO,
    },
    // 9
    {
        UNIMAP_NO,
        UNIMAP_NO,
        UNIMAP_8,
        UNIMAP_U,
        UNIMAP_HENK,
        UNIMAP_M,
        UNIMAP_J,
        UNIMAP_NO,
    },
    // A
    {
        UNIMAP_NO,
        UNIMAP_NO,
        UNIMAP_7,
        UNIMAP_Y,
        UNIMAP_NO,
        UNIMAP_N,
        UNIMAP_H,
        UNIMAP_NO,
    },
    // B
    {
        UNIMAP_NO,
        UNIMAP_NO,
        UNIMAP_0,
        UNIMAP_O,
        UNIMAP_RALT,
        UNIMAP_DOT,
        UNIMAP_L,
        UNIMAP_NO,
    },
    // C
    {
        UNIMAP_NO,
        UNIMAP_NO,
        UNIMAP_BSPACE,
        UNIMAP_NO,
        UNIMAP_RIGHT,
        UNIMAP_RSHIFT,
        UNIMAP_ENTER,
        UNIMAP_NO,
    },
    // D
    {
        UNIMAP_NO,
        UNIMAP_NO,
        UNIMAP_JYEN,
        UNIMAP_RBRACKET,
        UNIMAP_DOWN,
        UNIMAP_UP,
        UNIMAP_NONUS_HASH,
        UNIMAP_NO,
    },
    // E
    {
        UNIMAP_NO,
        UNIMAP_NO,
        UNIMAP_MINUS,
        UNIMAP_P,
        UNIMAP_RCTRL,
        UNIMAP_SLASH,
        UNIMAP_SCOLON,
        UNIMAP_NO,
    },
    // F
    {
        UNIMAP_NO,
        UNIMAP_NO,
        UNIMAP_EQUAL,
        UNIMAP_LBRACKET,
        UNIMAP_LEFT,
        UNIMAP_RO,
        UNIMAP_QUOTE,
        UNIMAP_NO,
    },
};

#endif
