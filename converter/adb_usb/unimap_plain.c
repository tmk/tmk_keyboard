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
#include "unimap_common.h"


#define AC_FN0 ACTION_LAYER_TAP_KEY(1, KC_GRV)
#define AC_FN1 ACTION_LAYER_TAP_KEY(1, KC_BSLS)

#ifdef KEYMAP_SECTION_ENABLE
const action_t actionmaps[][UNIMAP_ROWS][UNIMAP_COLS] __attribute__ ((section (".keymap.keymaps"))) = {
#else
const action_t actionmaps[][UNIMAP_ROWS][UNIMAP_COLS] PROGMEM = {
#endif
    UNIMAP(
              F13, F14, F15, F16, F17, F18, F19, F20, F21, F22, F23, F24,
    ESC,      F1,  F2,  F3,  F4,  F5,  F6,  F7,  F8,  F9,  F10, F11, F12,           PSCR,SLCK,PAUS,         VOLD,VOLU,MUTE,
    FN0, 1,   2,   3,   4,   5,   6,   7,   8,   9,   0,   MINS,EQL, JYEN,BSPC,     INS, HOME,PGUP,    NLCK,PSLS,PAST,PMNS,
    TAB, Q,   W,   E,   R,   T,   Y,   U,   I,   O,   P,   LBRC,RBRC,     FN1,      DEL, END, PGDN,    P7,  P8,  P9,  PPLS,
    CAPS,A,   S,   D,   F,   G,   H,   J,   K,   L,   SCLN,QUOT,     NUHS,ENT,                         P4,  P5,  P6,  PCMM,
    LSFT,NUBS,Z,   X,   C,   V,   B,   N,   M,   COMM,DOT, SLSH,     RO,  RSFT,          UP,           P1,  P2,  P3,  PEQL,
    LCTL,LGUI,LALT,MHEN,          SPC,           HENK,KANA,RALT,RGUI,APP, RCTL,     LEFT,DOWN,RGHT,    P0,       PDOT,PENT
    ),
    UNIMAP(
              F13, F14, F15, F16, F17, F18, F19, F20, F21, F22, F23, F24,
    ESC,      F1,  F2,  F3,  F4,  F5,  F6,  F7,  F8,  F9,  F10, F11, F12,           PSCR,SLCK,PAUS,         VOLD,VOLU,MUTE,
    TRNS,F1,  F2,  F3,  F4,  F5,  F6,  F7,  F8,  F9,  F10, F11, F12, JYEN,DEL,      INS, HOME,PGUP,    NLCK,BTN2,BTN3,WH_D,
    TAB, Q,   W,   E,   R,   T,   Y,   U,   PSCR,SLCK,PAUS,UP,  INS,      TRNS,     DEL, END, PGDN,    WH_D,MS_U,WH_U,WH_U,
    CAPS,VOLD,VOLU,MUTE,F,   G,   H,   J,   HOME,PGUP,LEFT,RGHT,     NUHS,ENT,                         MS_L,MS_D,MS_R,BTN4,
    LSFT,NUBS,Z,   X,   C,   V,   B,   N,   M,   END, PGDN,DOWN,     RO,  RSFT,          PGUP,         WH_L,MS_D,WH_R,BTN1,
    LCTL,LGUI,LALT,MHEN,          SPC,           HENK,KANA,RALT,RGUI,APP, RCTL,     HOME,PGDN,END,     BTN1,     BTN2,BTN3
    ),
};
