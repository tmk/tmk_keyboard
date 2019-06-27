/*
Copyright 2018 Jun Wako <wakojun@gmail.com>

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
#include "unimap_trans.h"


#define AC_FN0 ACTION_LAYER_MOMENTARY(1)

#ifdef KEYMAP_SECTION_ENABLE
const action_t actionmaps[][UNIMAP_ROWS][UNIMAP_COLS] __attribute__ ((section (".keymap.keymaps"))) = {
#else
const action_t actionmaps[][UNIMAP_ROWS][UNIMAP_COLS] PROGMEM = {
#endif
    /* TODO: test
    ,---------------------------------------------------------------.
    |Pau|PrS| F1| F2| F3| F4| F5| F6| F7| F8| F9|F10|PgD|PgU|End|Hom|
    `---------------------------------------------------------------'
    ,---------------------------------------------------------------.
    |  Esc |  1|  2|  3|  4|  5| F21|Caps|  6|  7|  8|  9|  0|  BSpc|
    |---------------------------------------------------------------|
    |  Tab|  Q|  W|  E|  R|  T|    Up     |  Y|  U|  I|  O|  P|     |
    |----------------------------------------------------------` Ent|
    | Ctl|  A|  S|  D|  F|  G|  Left| Right|  H|  J|  K|  L|  ;|    |
    |---------------------------------------------------------------|
    |Sft|  Z|  X|  C|  V|  B|Ins|  Down |Del|  N|  M|  ,|  .|  /| ^1|
    `---------------------------------------------------------------'
            |Alt| MHEN|  Ctl|  FN0|   Spc   |  FN0| HENK| ^2|
            `-----------------------------------------------'
    ^1: Right Shift is identical to Left Shift
    ^2: Ctrl+HENK(Ctrl+XFER)
    */
    UNIMAP_RAKURAKU(
    PAUS,PSCR,  F1,  F2,  F3,  F4,  F5,  F6,  F7,  F8,  F9, F10,PGUP,PGDN, END,HOME,
     ESC,     1,   2,   3,   4,   5,  LNUM,LCAP,    6,   7,   8,   9,   0,     BSPC,
     TAB,     Q,   W,   E,   R,   T,     UP,        Y,   U,   I,   O,   P,      ENT,
    LCTL,    A,   S,   D,   F,   G,  LEFT,  RGHT,    H,   J,   K,   L,SCLN,
    LSFT,   Z,   X,   C,   V,   B,  INS, DOWN, DEL,   N,   M,COMM, DOT,SLSH,
              LALT, MHEN, LGUI, FN0,       SPC,     RALT,   HENK
    ),
    UNIMAP_RAKURAKU(
    TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,SLCK,PAUS,
     GRV,  TRNS,TRNS,TRNS,TRNS,TRNS,  TRNS,TRNS, TRNS,TRNS,TRNS,MINS, EQL,     BSLS,
    TRNS,  TRNS,TRNS,TRNS,TRNS,TRNS,    PGUP,    TRNS,TRNS,TRNS,LBRC,RBRC,     TRNS,
    TRNS, TRNS,TRNS,TRNS,TRNS,TRNS,  HOME,   END, TRNS,TRNS,TRNS,TRNS,QUOT,
    TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS, PGDN, TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,
              TRNS, TRNS, TRNS, TRNS,     TRNS,     TRNS,   TRNS
    ),
};
