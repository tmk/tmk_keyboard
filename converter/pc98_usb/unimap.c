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
    /*
    ,---. ,---. ,------------------------. ,------------------------. ,------------------------.
    |Pau| |PrS| | F1 | F2 | F3 | F4 | F5 | | F6 | F7 | F8 | F9 | F10| | F11| F12| F13| F14| F15|
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
       |Kana|Gui| Alt|MHEN|       Space        |HENK|Alt|Gui|App|   | Down  |  |  0|  ,|  .|Ent|
       `--------------------------------------------------------'   `-------'  `---------------'
    Caps and Kana are locking keys.
    */
    UNIMAP_PC98(
    PAUS,PSCR,     F1,  F2,  F3,  F4,  F5,            F6,  F7,  F8,  F9,  F10,          F11, F12, F13, F14, F15,
    ESC, 1,   2,   3,   4,   5,   6,   7,   8,   9,   0,   MINS,EQL, JYEN,BSPC,  INS, DEL,   HOME,END, PMNS,PSLS,
    TAB,      Q,   W,   E,   R,   T,   Y,   U,   I,   O,   P,   LBRC,RBRC,       PGDN,PGUP,  P7,  P8,  P9,  PAST,
    LCTL,LCAP,A,   S,   D,   F,   G,   H,   J,   K,   L,   SCLN,QUOT,BSLS,ENT,      UP,      P4,  P5,  P6,  PPLS,
    LSFT,     Z,   X,   C,   V,   B,   N,   M,   COMM,DOT, SLSH, RO,      RSFT,  LEFT,RGHT,  P1,  P2,  P3,  PEQL,
         KANA,LGUI,LALT,MHEN,        SPC,        HENK,RALT,RGUI,APP,                DOWN,    P0,  PCMM,PDOT,PENT
    ),
};
