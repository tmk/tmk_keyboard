/*
Copyright 2015 Julien Pecqueur <julien@peclu.net>

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

/*
 * JPEC40 KEYMAP
 */
const uint8_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /* Keymap 0: Default Layer
     * ,-----------------------------------------------.
     * |  ]|  Q|  W|  E|  R|  T|  Y|  U|  I|  O|  P|  [|
     * |-----------------------------------------------|
     * |Ctr|  A|  S|  D|  F|  G|  H|  J|  K|  L|  ;|  '|
     * |-----------------------------------------------|
     * |Shf|  Z|  X|  C|  V|  B|  N|  M|  ,|  .|  /|  \|
     * |-----------------------------------------------|
     * |Tab|  `|Win|Alt|Fn1| Space |Fn2|Alt|Del|  =|Ent|
     * `-----------------------------------------------'
     */
    KEYMAP_ANSI(
        RBRC,Q,   W,   E,   R,   T,   Y,   U,   I,   O,   P,   LBRC, \
        LCTL,A,   S,   D,   F,   G,   H,   J,   K,   L,   SCLN,QUOT, \
        LSFT,Z,   X,   C,   V,   B,   N,   M,   COMM,DOT, SLSH,BSLS, \
        TAB, GRV, LGUI,LALT,FN0,      SPC, FN1, RALT,BSPC,EQL, ENT ),
    /* Keymap 1: Alternate Layer (Fn1)
     * ,-----------------------------------------------.
     * |  `|  1|  2|  3|  4|  5|  6|  7|  8|  9|  0|  -|
     * |-----------------------------------------------|
     * |   | F1| F2| F3| F4| F5| F6| F7| F8| F9|F10|F11|
     * |-----------------------------------------------|
     * |   |   |   |   |   |   |   |   |   |   |   |F12|
     * |-----------------------------------------------|
     * |   |   |   |   |Fn1|       |Fn2|   |Del|   |Ent|
     * `-----------------------------------------------'
     */
    KEYMAP_ANSI(
        GRV, 1,   2,   3,   4,   5,   6,   7,   8,   9,   0,   MINS, \
        TRNS,F1,  F2,  F3,  F4,  F5,  F6,  F7,  F8,  F9,  F10, F11, \
        TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,F12, \
        TRNS,TRNS,TRNS,TRNS,TRNS,     TRNS,TRNS, TRNS,DEL ,TRNS,PENT),
    /* Keymap 2: Function Layer (Fn2)
     * ,-----------------------------------------------.
     * |Esc|   | UP|   | <-|Hom|Pup|   | UP|   |Prt|Slk|
     * |-----------------------------------------------|
     * |   |LFT|DWN|RGT|Del|End|Pdn|LFT|DWN|RGT|Ins|App|
     * |-----------------------------------------------|
     * |   |Prv|Ply|Nxt|Vo-|Vo+|Mut|   |   |   |   |   |
     * |-----------------------------------------------|
     * |   |   |   |   |Fn1|       |Fn2|   |Del|   |Ent|
     * `-----------------------------------------------'
     */
    KEYMAP_ANSI(
        ESC, TRNS,UP,  TRNS,BSPC,HOME,PGUP,TRNS,UP,  TRNS,PSCR,SLCK, \
        TRNS,LEFT,DOWN,RGHT,DEL, END, PGDN,LEFT,DOWN,RGHT,INS, RGUI, \
        TRNS,MPRV,MPLY,MNXT,VOLD,VOLU,MUTE,TRNS,TRNS,TRNS,TRNS,TRNS, \
        TRNS,TRNS,TRNS,TRNS,TRNS,     TRNS,TRNS,TRNS,DEL ,TRNS,PENT),
};

/*
 * Fn action definition
 */
const uint16_t PROGMEM fn_actions[] = {
    [0] = ACTION_LAYER_MOMENTARY(1),
    [1] = ACTION_LAYER_MOMENTARY(2),

};
