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
#include "unimap_trans.h"


#define AC_FN0  ACTION_LAYER_MOMENTARY(1)
// Undo, Cut, Copy and Paste
#define AC__UND ACTION_MODS_KEY(MOD_LCTL, KC_Z)
#define AC__CUT ACTION_MODS_KEY(MOD_LCTL, KC_X)
#define AC__CPY ACTION_MODS_KEY(MOD_LCTL, KC_C)
#define AC__PST ACTION_MODS_KEY(MOD_LCTL, KC_V)

#ifdef KEYMAP_SECTION_ENABLE
const action_t actionmaps[][UNIMAP_ROWS][UNIMAP_COLS] __attribute__ ((section (".keymap.keymaps"))) = {
#else
const action_t actionmaps[][UNIMAP_ROWS][UNIMAP_COLS] PROGMEM = {
#endif
    UNIMAP(
              NO,  NO,  NO,  NO,  NO,  NO,  NO,  NO,  NO,  NO,  NO,  NO,  
    NO,       NO,  NO,  NO,  NO,  NO,  NO,  NO,  NO,  NO,  NO,  NO,  NO,            NO,  NO,  NO,           NO,  NO,  NO,
    ESC, 1,   2,   3,   4,   5,   6,   7,   8,   9,   0,   MINS,EQL, NO,  BSPC,     VOLU,MUTE,PGUP,    GRV, BSLS,PSLS,PAST,
    TAB, Q,   W,   E,   R,   T,   Y,   U,   I,   O,   P,   LBRC,RBRC,     BSLS,     VOLD,NO,  PGDN,    P7,  P8,  P9,  PMNS,
    LCTL,A,   S,   D,   F,   G,   H,   J,   K,   L,   SCLN,QUOT,     NO,  ENT,                         P4,  P5,  P6,  PPLS,
    LSFT,NO,  Z,   X,   C,   V,   B,   N,   M,   COMM,DOT, SLSH,     NO,  RSFT,          UP,           P1,  P2,  P3,  PENT,
    LALT,LGUI,NO,  NO,            SPC,           NO,  NO,  NO,  NO,  FN0, RALT,     LEFT,DOWN,RGHT,    P0,       PDOT,NO
    ),
    UNIMAP(
              TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,
    TRNS,     TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,          TRNS,TRNS,TRNS,         TRNS,TRNS,TRNS,
    GRV, F1,  F2,  F3,  F4,  F5,  F6,  F7,  F8,  F9,  F10, F11, F12, TRNS,DEL,      TRNS,TRNS,TRNS,    NLCK,TRNS,TRNS,TRNS,
    CAPS,BSLS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,PSCR,SLCK,PAUS,UP,  INS,      TRNS,     TRNS,TRNS,TRNS,    TRNS,TRNS,TRNS,TRNS,
    TRNS,VOLD,VOLU,MUTE,TRNS,TRNS,PAST,PSLS,HOME,PGUP,LEFT,RGHT,     TRNS,TRNS,                        TRNS,TRNS,TRNS,TRNS,
    TRNS,TRNS,_UND,_CUT,_CPY,_PST,TRNS,PPLS,PMNS,END, PGDN,DOWN,     TRNS,TRNS,          PGUP,         TRNS,TRNS,TRNS,TRNS,
    TRNS,TRNS,TRNS,TRNS,          TRNS,          TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,     HOME,PGDN,END,     TRNS,     TRNS,TRNS
    ),
};
