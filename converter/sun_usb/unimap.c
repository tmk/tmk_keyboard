/*
Copyright 2020 Jun Wako <wakojun@gmail.com>

This file is part of TMK keyboard. https://github.com/tmk/tmk_keyboard

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


#define AC_FN0 ACTION_LAYER_TAP_KEY(1, KC_APPLICATION)

#ifdef KEYMAP_SECTION_ENABLE
const action_t actionmaps[][UNIMAP_ROWS][UNIMAP_COLS] __attribute__ ((section (".keymap.keymaps"))) = {
#else
const action_t actionmaps[][UNIMAP_ROWS][UNIMAP_COLS] PROGMEM = {
#endif
    UNIMAP_TYPE5(
    F23,        F24, F1,  F2,  F3,       F4,  F5,  F6,  F7,  F8,       F9,  F10, F11, F12,   PSCR,SLCK,PAUS,  MUTE,VOLD,VOLU,PWR,
    F13, F14,   ESC, 1,   2,   3,   4,   5,   6,   7,   8,   9,   0,   MINS,EQL, BSLS,GRV,   INS, HOME,PGUP,  NLCK,PSLS,PAST,PMNS,
    F15, F16,   TAB, Q,   W,   E,   R,   T,   Y,   U,   I,   O,   P,   LBRC,RBRC,     BSPC,  DEL, END, PGDN,  P7,  P8,  P9,  PPLS,
    F17, F18,   LCTL,A,   S,   D,   F,   G,   H,   J,   K,   L,   SCLN,QUOT,          ENT,                    P4,  P5,  P6,
    F19, F20,   LSFT,NUBS,Z,   X,   C,   V,   B,   N,   M,   COMM,DOT, SLSH,     RO,  RSFT,       UP,         P1,  P2,  P3,  PENT,
    F21, F22,   CAPS,LALT,LGUI,MHEN,          SPC,                HENK,KANA,RGUI,FN0, RALT,  LEFT,DOWN,RGHT,  P0,       PDOT
    ),
    UNIMAP_TYPE5(
    TRNS,       TRNS,TRNS,TRNS,TRNS,     TRNS,TRNS,TRNS,TRNS,TRNS,     TRNS,TRNS,TRNS,TRNS,  TRNS,TRNS,TRNS,  TRNS,TRNS,TRNS,BTLD,
    TRNS,TRNS,  GRV, F1,  F2,  F3,  F4,  F5,  F6,  F7,  F8,  F9,  F10, F11, F12, INS, DEL,   TRNS,TRNS,TRNS,  TRNS,TRNS,TRNS,TRNS,
    TRNS,TRNS,  TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,PSCR,SLCK,PAUS,UP,  TRNS,     TRNS,  TRNS,TRNS,TRNS,  TRNS,TRNS,TRNS,TRNS,
    TRNS,TRNS,  TRNS,VOLD,VOLU,MUTE,TRNS,TRNS,TRNS,TRNS,HOME,PGUP,LEFT,RGHT,          TRNS,                   TRNS,TRNS,TRNS,
    TRNS,TRNS,  TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,END, PGDN,DOWN,     TRNS,TRNS,       TRNS,       TRNS,TRNS,TRNS,TRNS,
    TRNS,TRNS,  TRNS,TRNS,TRNS,TRNS,          TRNS,               TRNS,TRNS,TRNS,TRNS,TRNS,  TRNS,TRNS,TRNS,  TRNS,     TRNS
    ),
};
/* Templates:
    UNIMAP_TYPE5(
    TRNS,       TRNS,TRNS,TRNS,TRNS,     TRNS,TRNS,TRNS,TRNS,TRNS,     TRNS,TRNS,TRNS,TRNS,  TRNS,TRNS,TRNS,  TRNS,TRNS,TRNS,TRNS,
    TRNS,TRNS,  TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,  TRNS,TRNS,TRNS,  TRNS,TRNS,TRNS,TRNS,
    TRNS,TRNS,  TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,     TRNS,  TRNS,TRNS,TRNS,  TRNS,TRNS,TRNS,TRNS,
    TRNS,TRNS,  TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,          TRNS,                   TRNS,TRNS,TRNS,
    TRNS,TRNS,  TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,     TRNS,TRNS,       TRNS,       TRNS,TRNS,TRNS,TRNS,
    TRNS,TRNS,  TRNS,TRNS,TRNS,TRNS,          TRNS,               TRNS,TRNS,TRNS,TRNS,TRNS,  TRNS,TRNS,TRNS,  TRNS,     TRNS
    ),

    UNIMAP_TYPE3(
    F10, F11,   F1,  F2,  F3,       F4,       F5,       F6,       F7,       F8,  F9,  BSPC,  VOLD,MUTE,VOLU,
    F12, F13,   ESC, 1,   2,   3,   4,   5,   6,   7,   8,   9,   0,   MINS,EQL, BSLS,GRV,   MPRV,MPLY,MNXT,
    F14, F15,   TAB, Q,   W,   E,   R,   T,   Y,   U,   I,   O,   P,   LBRC,RBRC,     DEL,   HOME,UP,  PGUP,
    F16, F17,   LCTL,A,   S,   D,   F,   G,   H,   J,   K,   L,   SCLN,QUOT,          ENT,   LEFT,INS, RGHT,
    F18, F19,   LSFT,Z,   X,   C,   V,   B,   N,   M,   COMM,DOT, SLSH,          RSFT,RCTL,  END, DOWN,PGDN,
                LGUI,LALT,                 SPC,                                  RALT,RGUI
    ),
*/
