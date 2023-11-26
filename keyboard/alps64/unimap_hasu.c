/*
Copyright 2023 Jun Wako <wakojun@gmail.com>

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


#define AC_L1      ACTION_LAYER_MOMENTARY(1)
#define AC_L2      ACTION_LAYER_MOMENTARY(2)
#define AC_SLS2    ACTION_LAYER_TAP_KEY(2, KC_SLSH)
#define AC_SCL3    ACTION_LAYER_TAP_KEY(3, KC_SCLN)
#define AC_SPC4    ACTION_LAYER_TAP_KEY(4, KC_SPC)
#define AC_BACK    ACTION_MODS_KEY(MOD_LALT, KC_LEFT)
#define AC_FRWD    ACTION_MODS_KEY(MOD_LALT, KC_RIGHT)
#define AC_ENT_    ACTION_MODS_TAP_KEY(MOD_RCTL, KC_ENT)


#ifdef KEYMAP_SECTION_ENABLE
const action_t actionmaps[][UNIMAP_ROWS][UNIMAP_COLS] __attribute__ ((section (".keymap.keymaps"))) = {
#else
const action_t actionmaps[][UNIMAP_ROWS][UNIMAP_COLS] PROGMEM = {
#endif
    /* 0:
     * ,-----------------------------------------------------------.
     * |`  |  1|  2|  3|  4|  5|  6|  7|  8|  9|  0|  -|  =|   \   |
     * |-----------------------------------------------------------|
     * |Tab  |  Q|  W|  E|  R|  T|  Y|  U|  I|  O|  P|  [|  ]| Bspc|
     * |-----------------------------------------------------------|
     * |Ctrl  |  A|  S|  D|  F|  G|  H|  J|  K|  L|  ;|  '|Enter   |
     * |-----------------------------------------------------------|
     * |Shft|  \|  Z|  X|  C|  V|  B|  N|  M|  ,|  .|  /|Shift     |
     * |-----------------------------------------------------------'
     * |Ctrl|Gui |Alt |         Space              |Alt |App |  L1 |
     * `-----------------------------------------------------------'
     */
    [0] = UNIMAP_ALPS64( \
        ESC, 1,   2,   3,   4,   5,   6,   7,   8,   9,   0,   MINS,EQL, GRV, BSLS,
        TAB, Q,   W,   E,   R,   T,   Y,   U,   I,   O,   P,   LBRC,RBRC,     BSPC,
        LCTL,A,   S,   D,   F,   G,   H,   J,   K,   L,   SCL3,QUOT,          ENT_,
        LSFT,NUBS,Z,   X,   C,   V,   B,   N,   M,   COMM,DOT, SLS2,     RSFT,L1,
        L2,  LGUI,LALT,          SPC4,                         APP, RALT,RGUI,L1),

    [1] = UNIMAP_ALPS64( \
        GRV, F1,  F2,  F3,  F4,  F5,  F6,  F7,  F8,  F9,  F10, F11, F12, TRNS,GRV,
        CAPS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,PSCR,SLCK,PAUS,UP,  INS,      DEL,
        TRNS,VOLD,VOLU,MUTE,TRNS,TRNS,PAST,PSLS,HOME,PGUP,LEFT,RGHT,          TRNS,
        TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,PPLS,PMNS,END, PGDN,DOWN,TRNS,     TRNS,TRNS,
        TRNS,TRNS,TRNS,          TRNS,                         TRNS,TRNS,TRNS,TRNS),

    [2] = UNIMAP_ALPS64(
        GRV, F1,  F2,  F3,  F4,  F5,  F6,  F7,  F8,  F9,  F10, F11, F12, INS, DEL,
        TRNS,HOME,PGDN,UP,  PGUP,END, HOME,PGDN,PGUP,END, NO,  NO,  NO,       TRNS,
        TRNS,NO,  LEFT,DOWN,RGHT,NO,  LEFT,DOWN,UP,  RGHT,NO,  NO,            TRNS,
        TRNS,TRNS,NO,  NO,  NO,  NO,  NO,  HOME,PGDN,PGUP,END, TRNS,     TRNS,TRNS,
        TRNS,TRNS,TRNS,          TRNS,                         TRNS,TRNS,TRNS,TRNS),

    [3] = UNIMAP_ALPS64(
        GRV, F1,  F2,  F3,  F4,  F5,  F6,  F7,  F8,  F9,  F10, F11, F12, INS, DEL,
        TAB, NO,  NO,  NO,  NO,  NO,  WH_L,WH_D,MS_U,WH_U,WH_R,BACK,FRWD,     TRNS,
        TRNS,ACL0,ACL1,ACL2,ACL2,NO,  NO,  MS_L,MS_D,MS_R,TRNS,NO,            TRNS,
        TRNS,TRNS,NO,  NO,  NO,  NO,  BTN3,BTN2,BTN1,BACK,FRWD,NO,       TRNS,TRNS,
        TRNS,TRNS,TRNS,          BTN1,                         TRNS,TRNS,TRNS,TRNS),

    [4] = UNIMAP_ALPS64(
        GRV, F1,  F2,  F3,  F4,  F5,  F6,  F7,  F8,  F9,  F10, F11, F12, INS, DEL,
        TAB, NO,  NO,  NO,  NO,  NO,  WH_L,WH_D,MS_U,WH_U,WH_R,BTN4,BTN5,     TAB,
        LCTL,VOLD,VOLU,MUTE,NO,  NO,  NO,  MS_L,MS_D,MS_R,BTN1,NO,            ENT,
        LSFT,TRNS,NO,  NO,  NO,  NO,  BTN3,BTN2,BTN1,BACK,FRWD,NO,       TRNS,TRNS,
        TRNS,TRNS,TRNS,          TRNS,                         TRNS,TRNS,TRNS,TRNS),
};
