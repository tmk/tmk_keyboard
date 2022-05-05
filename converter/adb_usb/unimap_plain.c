/*
Copyright 2016-19 Jun Wako <wakojun@gmail.com>

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
#include "unimap_common.h"


#define AC_FN0  ACTION_LAYER_TAP_KEY(1, KC_GRV)
#define AC_FN1  ACTION_LAYER_TAP_KEY(1, KC_BSLS)
#define AC_PKEY ACTION_MODS_TAP_KEY(MOD_NONE, KC_PWR)

#ifdef KEYMAP_SECTION_ENABLE
const action_t actionmaps[][UNIMAP_ROWS][UNIMAP_COLS] __attribute__ ((section (".keymap.keymaps"))) = {
#else
const action_t actionmaps[][UNIMAP_ROWS][UNIMAP_COLS] PROGMEM = {
#endif
    /* ADB Keyboard unified layout
     * ,---.   .---------------. ,---------------. ,---------------. ,-----------.             ,---.
     * |Esc|   |F1 |F2 |F3 |F4 | |F5 |F6 |F7 |F8 | |F9 |F10|F11|F12| |PrS|ScL|Pau|             |Pwr|
     * `---'   `---------------' `---------------' `---------------' `-----------'             `---'
     * ,-----------------------------------------------------------. ,-----------. ,---------------. ,---.
     * |  `|  1|  2|  3|  4|  5|  6|  7|  8|  9|  0|  -|  =|JPY| BS| |Ins|Hom|PgU| |NmL|  =|  /|  *| |VUp|
     * |-----------------------------------------------------------| |-----------| |---------------| |---|
     * |Tab  |  Q|  W|  E|  R|  T|  Y|  U|  I|  O|  P|  [|  ]|  \  | |Del|End|PgD| |  7|  8|  9|  -| |VDn|
     * |-----------------------------------------------------------| `-----------' |---------------| |---|
     * |CapsLo|  A|  S|  D|  F|  G|  H|  J|  K|  L|  ;|  '|  #| Ret|               |  4|  5|  6|  +| |Mut|
     * |-----------------------------------------------------------|     ,---.     |---------------| `---'
     * |Shif|  <|  Z|  X|  C|  V|  B|  N|  M|  ,|  ,|  /| RO| Shift|     |Up |     |  1|  2|  3|   |
     * |-----------------------------------------------------------| ,-----------. |-----------|Ent| ,---.
     * |Ctrl |Opt |Gui  |MHN|    Space    |HNK|KAN|Gui* |Opt |Ctrl | |Lef|Dow|Rig| |  0|  ,|  .|   | |Ply|
     * `-----------------------------------------------------------' `-----------' `---------------' `---'
     */
    [0] = UNIMAP_ADB(
    ESC,      F1,  F2,  F3,  F4,  F5,  F6,  F7,  F8,  F9,  F10, F11, F12,        PSCR,SLCK,PAUS,                 PKEY,
    FN0, 1,   2,   3,   4,   5,   6,   7,   8,   9,   0,   MINS,EQL, JPY, BSPC,  INS, HOME,PGUP,  NLCK,EQL, PSLS,PAST,  VOLU,
    TAB, Q,   W,   E,   R,   T,   Y,   U,   I,   O,   P,   LBRC,RBRC,     FN1,   DEL, END, PGDN,  P7,  P8,  P9,  PMNS,  VOLD,
    LCAP,A,   S,   D,   F,   G,   H,   J,   K,   L,   SCLN,QUOT,     NUHS,ENT,                    P4,  P5,  P6,  PPLS,  MUTE,
    LSFT,NUBS,Z,   X,   C,   V,   B,   N,   M,   COMM,DOT, SLSH,     RO,  RSFT,       UP,         P1,  P2,  P3,  PENT,
    LCTL,LALT,LGUI,MHEN,          SPC,                HENK,KANA,     RALT,RCTL,  LEFT,DOWN,RGHT,  P0,  PCMM,PDOT,       MPLY
    ),

    /*
     * ,---.   .---------------. ,---------------. ,---------------. ,-----------.             ,---.
     * |  `|   |   |   |   |   | |   |   |   |   | |   |   |   |   | |   |   |   |             |   |
     * `---'   `---------------' `---------------' `---------------' `-----------'             `---'
     * ,-----------------------------------------------------------. ,-----------. ,---------------. ,---.
     * |Esc| F1| F2| F3| F4| F5| F6| F7| F8| F9|F10|F11|F12|Ins|Del| |   |   |   | |   |   |   |   | |   |
     * |-----------------------------------------------------------| |-----------| |---------------| |---|
     * |     |   |   |   |   |   |   |   |PrS|ScL|Pau|Up |INS|     | |   |   |   | |   |   |   |   | |   |
     * |-----------------------------------------------------------| `-----------' |---------------| |---|
     * |CapsLo|VoD|VoU|Mut|   |   |   |   |Hom|PgU|Lef|Rig|#  |Retn|               |   |   |   |   | |   |
     * |-----------------------------------------------------------|     ,---.     |---------------| `---'
     * |    |   |   |   |   |   |   |   |   |End|PgD|Dow| RO| Shift|     |PgU|     |   |   |   |   |
     * |-----------------------------------------------------------| ,-----------. |-----------|   | ,---.
     * |     |    |    |    |                 |   |     |    |     | |Hom|PgD|End| |   |   |   |   | |   |
     * `-----------------------------------------------------------' `-----------' `---------------' `---'
     */
    [1] = UNIMAP_ADB(
    GRV,      TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,       TRNS,TRNS,TRNS,                 TRNS,
    ESC, F1,  F2,  F3,  F4,  F5,  F6,  F7,  F8,  F9,  F10, F11, F12, INS, DEL,   TRNS,TRNS,TRNS,  TRNS,TRNS,TRNS,TRNS,  TRNS,
    TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,PSCR,SLCK,PAUS,UP,  INS,      TRNS,  TRNS,TRNS,TRNS,  TRNS,TRNS,TRNS,TRNS,  TRNS,
    TRNS,VOLD,VOLU,MUTE,TRNS,TRNS,TRNS,TRNS,HOME,PGUP,LEFT,RGHT,     TRNS,TRNS,                   TRNS,TRNS,TRNS,TRNS,  TRNS,
    TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,END, PGDN,DOWN,     TRNS,TRNS,       PGUP,       TRNS,TRNS,TRNS,TRNS,
    TRNS,TRNS,TRNS,TRNS,          TRNS,               TRNS,TRNS,     TRNS,TRNS,  HOME,PGDN,END,   TRNS,TRNS,TRNS,       TRNS
    ),
};
