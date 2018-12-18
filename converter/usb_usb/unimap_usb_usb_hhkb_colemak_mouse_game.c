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


enum macro_id {
    OPEN_TAB,
    SAVE_AS,
    COPY_LINK,
    RUN_NOTEPAD,
    M_1,
    //ALT_TAB,
};

#define AC_OPTB    ACTION_MACRO(OPEN_TAB)
// Macro: Save As in IE;
#define AC_SVAS    ACTION_MACRO(SAVE_AS)
#define AC_CPLK    ACTION_MACRO(COPY_LINK)
#define AC_NTPD    ACTION_MACRO(RUN_NOTEPAD)
#define AC_M_1     ACTION_MACRO(M_1)


#define AC_L5      ACTION_LAYER_MOMENTARY(5)
#define AC_L6      ACTION_LAYER_MOMENTARY(6)
#define AC_L7      ACTION_LAYER_MOMENTARY(7)
#define AC_DL0     ACTION_DEFAULT_LAYER_SET(0)
#define AC_DL1     ACTION_DEFAULT_LAYER_SET(1)
#define AC_DL2     ACTION_DEFAULT_LAYER_SET(2)
#define AC_SPC6    ACTION_LAYER_TAP_KEY(6, KC_SPC)
#define AC_BACK    ACTION_MODS_KEY(MOD_LALT, KC_LEFT)
#define AC_FRWD    ACTION_MODS_KEY(MOD_LALT, KC_RGHT)
#define AC_LTAB    ACTION_MODS_KEY(MOD_LCTL, KC_PGUP)
#define AC_RTAB    ACTION_MODS_KEY(MOD_LCTL, KC_PGDN)
#define AC_CLSE    ACTION_MODS_KEY(MOD_LALT, KC_F4)
#define AC_ENT_    ACTION_MODS_TAP_KEY(MOD_RCTL, KC_ENT)

#ifdef KEYMAP_SECTION_ENABLE
const action_t actionmaps[][UNIMAP_ROWS][UNIMAP_COLS] __attribute__ ((section (".keymap.keymaps"))) = {
#else
const action_t actionmaps[][UNIMAP_ROWS][UNIMAP_COLS] PROGMEM = {
#endif
//0, HHKB, Colemak
    UNIMAP(
              F13, F14, F15, F16, F17, F18, F19, F20, F21, F22, F23, F24,
    ESC,      F1,  F2,  F3,  F4,  F5,  F6,  F7,  F8,  F9,  F10, F11, F12,           RGUI,SLCK,PAUS,         VOLD,VOLU,MUTE,
    GRV, 1,   2,   3,   4,   5,   6,   7,   8,   9,   0,   MINS,EQL, JYEN,BSPC,     INS, HOME,PGUP,    NLCK,PSLS,PAST,PMNS,
    TAB, Q,   W,   F,   P,   G,   J,   L,   U,   Y,   SCLN,LBRC,RBRC,     BSLS,     DEL, END, PGDN,    P7,  P8,  P9,  PPLS,
    CAPS,A,   R,   S,   T,   D,   H,   N,   E,   I,   O,   QUOT,     NUHS,ENT_,                        P4,  P5,  P6,  PCMM,
    LSFT,NUBS,Z,   X,   C,   V,   B,   K,   M,   COMM,DOT, SLSH,     RO,  RSFT,          UP,           P1,  P2,  P3,  PENT,
    LCTL,L5  ,LALT,MHEN,          SPC6,          HENK,KANA,RALT,RGUI,L7,  L7,       LEFT,DOWN,RGHT,    P0,       PDOT,PEQL
    ),
//1, HHKB & ThinkPad, QWERTY, Game
    UNIMAP(
              F13, F14, F15, F16, F17, F18, F19, F20, F21, F22, F23, F24,
    ESC,      F1,  F2,  F3,  F4,  F5,  F6,  F7,  F8,  F9,  F10, F11, F12,           RGUI,SLCK,PAUS,         VOLD,VOLU,MUTE,
    GRV, 1,   2,   3,   4,   5,   6,   7,   8,   9,   0,   MINS,EQL, JYEN,BSPC,     INS, HOME,PGUP,    NLCK,PSLS,PAST,PMNS,
    TAB, Q,   W,   E,   R,   T,   Y,   U,   I,   O,   P,   LBRC,RBRC,     BSLS,     DEL, END, PGDN,    P7,  P8,  P9,  PPLS,
    CAPS,A,   S,   D,   F,   G,   H,   J,   K,   L,   SCLN,QUOT,     NUHS,ENT_,                        P4,  P5,  P6,  PCMM,
    LSFT,NUBS,Z,   X,   C,   V,   B,   N,   M,   COMM,DOT, SLSH,     RO,  RSFT,          UP,           P1,  P2,  P3,  PENT,
    LCTL,L5  ,LALT,MHEN,          SPC,           HENK,KANA,RALT,RGUI,L7,  L7,       LEFT,DOWN,RGHT,    P0,       PDOT,PEQL
    ),
//2, ThinkPad, Colemak
    UNIMAP(
              F13, F14, F15, F16, F17, F18, F19, F20, F21, F22, F23, F24,
    ESC,      F1,  F2,  F3,  F4,  F5,  F6,  F7,  F8,  F9,  F10, F11, F12,           RGUI,SLCK,PAUS,         VOLD,VOLU,MUTE,
    GRV, 1,   2,   3,   4,   5,   6,   7,   8,   9,   0,   MINS,EQL, JYEN,BSLS,     INS, HOME,PGUP,    NLCK,PSLS,PAST,PMNS,
    TAB, Q,   W,   F,   P,   G,   J,   L,   U,   Y,   SCLN,LBRC,RBRC,     BSPC,     DEL, END, PGDN,    P7,  P8,  P9,  PPLS,
    LCTL,A,   R,   S,   T,   D,   H,   N,   E,   I,   O,   QUOT,     NUHS,ENT_,                        P4,  P5,  P6,  PCMM,
    LSFT,NUBS,Z,   X,   C,   V,   B,   K,   M,   COMM,DOT, SLSH,     RO,  RSFT,          UP,           P1,  P2,  P3,  PENT,
    CAPS,L5  ,LALT,MHEN,          SPC6,          HENK,KANA,RALT,RGUI,L7,  L7,       LEFT,DOWN,RGHT,    P0,       PDOT,PEQL
    ),
//3, TRNS
    UNIMAP(
              TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,
    TRNS,     TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,          TRNS,TRNS,TRNS,         TRNS,TRNS,TRNS,
    TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,     TRNS,TRNS,TRNS,    TRNS,TRNS,TRNS,TRNS,
    TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,     TRNS,     TRNS,TRNS,TRNS,    TRNS,TRNS,TRNS,TRNS,
    TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,     TRNS,TRNS,                        TRNS,TRNS,TRNS,TRNS,
    TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,     TRNS,TRNS,          TRNS,         TRNS,TRNS,TRNS,TRNS,
    TRNS,TRNS,TRNS,TRNS,          TRNS,          TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,     TRNS,TRNS,TRNS,    TRNS,     TRNS,TRNS
    ),
//4, TRNS
    UNIMAP(
              TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,
    TRNS,     TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,          TRNS,TRNS,TRNS,         TRNS,TRNS,TRNS,
    TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,     TRNS,TRNS,TRNS,    TRNS,TRNS,TRNS,TRNS,
    TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,     TRNS,     TRNS,TRNS,TRNS,    TRNS,TRNS,TRNS,TRNS,
    TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,     TRNS,TRNS,                        TRNS,TRNS,TRNS,TRNS,
    TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,     TRNS,TRNS,          TRNS,         TRNS,TRNS,TRNS,TRNS,
    TRNS,TRNS,TRNS,TRNS,          TRNS,          TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,     TRNS,TRNS,TRNS,    TRNS,     TRNS,TRNS
    ),
//5, HHKB, L3
    UNIMAP(
              TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,
    ESC,      MUTE,VOLD,VOLU,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,          TRNS,TRNS,TRNS,         TRNS,TRNS,TRNS,
    GRV, F1,  F2,  F3,  F4,  F5,  F6,  F7,  F8,  F9,  F10, F11, F12, INS, DEL,      TRNS,TRNS,BACK,    TRNS,TRNS,TRNS,TRNS,
    CAPS,M_1 ,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,DL2, DL1, DL0,      BSPC,     TRNS,TRNS,FRWD,    TRNS,TRNS,TRNS,TRNS,
    TRNS,VOLD,VOLU,MUTE,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,     TRNS,ENT,                         TRNS,TRNS,TRNS,TRNS,
    TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,     TRNS,TRNS,          PGUP,         TRNS,TRNS,TRNS,TRNS,
    CAPS,TRNS,TRNS,TRNS,          SPC,           TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,     HOME,PGDN,END,     TRNS,     TRNS,TRNS
    ),
//6, HHKB, L2
    UNIMAP(
              TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,
    CLSE,     MUTE,VOLD,VOLU,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,          TRNS,TRNS,TRNS,         TRNS,TRNS,TRNS,
    GRV, F1,  F2,  F3,  F4,  F5,  F6,  F7,  F8,  F9,  F10, F11, F12, INS, DEL,      TRNS,TRNS,BACK,    TRNS,TRNS,TRNS,TRNS,
    CAPS,CPLK,WH_U,MS_U,WH_D,OPTB,OPTB,WH_U,MS_U,WH_D,CPLK,WH_L,WH_R,     BSPC,     TRNS,TRNS,FRWD,    TRNS,TRNS,TRNS,TRNS,
    TRNS,LTAB,MS_L,MS_D,MS_R,RTAB,LTAB,MS_L,MS_D,MS_R,RTAB,FRWD,     TRNS,ENT,                         TRNS,TRNS,TRNS,TRNS,
    TRNS,TRNS,BTN4,BTN5,BTN1,BTN2,BTN3,BTN2,BTN1,BTN4,BTN5,BACK,     TRNS,TRNS,          PGUP,         TRNS,TRNS,TRNS,TRNS,
    CAPS,MYCM,NTPD,TRNS,          SPC,           TRNS,TRNS,CALC,MYCM,TRNS,TRNS,     HOME,PGDN,END,     TRNS,     TRNS,TRNS
    ),
//7, HHKB, L1
    UNIMAP(
              TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,
    ESC,      MUTE,VOLD,VOLU,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,          TRNS,TRNS,TRNS,         TRNS,TRNS,TRNS,
    GRV, F1,  F2,  F3,  F4,  F5,  F6,  F7,  F8,  F9,  F10, F11, F12, INS, DEL,      TRNS,TRNS,BACK,    TRNS,TRNS,TRNS,TRNS,
    CAPS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,PSCR,SLCK,PAUS,UP,  TRNS,     TRNS,     TRNS,TRNS,FRWD,    TRNS,TRNS,TRNS,TRNS,
    TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,HOME,PGUP,LEFT,RGHT,     TRNS,ENT,                         TRNS,TRNS,TRNS,TRNS,
    TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,END, PGDN,DOWN,     TRNS,TRNS,          PGUP,         TRNS,TRNS,TRNS,TRNS,
    CAPS,TRNS,TRNS,TRNS,          SPC,           TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,     HOME,PGDN,END,     TRNS,     TRNS,TRNS
    ),

};



/*
 * Macro definition
 */
const macro_t *action_get_macro(keyrecord_t *record, uint8_t id, uint8_t opt)
{
    switch (id) {
        case OPEN_TAB:
            return (record->event.pressed ?
                    MACRO( D(BTN2), U(BTN2), W(255), T(T), END ) :
                    MACRO_NONE );
        case SAVE_AS:
            return (record->event.pressed ?
                    MACRO( D(BTN2), U(BTN2), W(255), T(K), W(255), W(255), W(255), W(255), W(255), W(255), W(255), W(255), T(ENT), END ) :
                    MACRO_NONE );
        case COPY_LINK:
            return (record->event.pressed ?
                    MACRO( D(BTN2), U(BTN2), W(255), T(E), END ) :
                    MACRO_NONE );
        case RUN_NOTEPAD:
            return (record->event.pressed ?
                    MACRO( D(LGUI), D(R), U(R), U(LGUI), W(255), T(N), T(O), T(T), T(E), T(P), T(A), T(D), T(ENT), END ) :
                    MACRO_NONE );
        case M_1:
            return (record->event.pressed ?
                    MACRO( T(1),T(2),T(1),T(0),T(0),T(0),T(0),T(0),T(4),T(0),T(0),T(0),T(0),T(0),T(6),T(2),T(4),D(LSFT),T(D),U(LSFT), END ) :
                    MACRO_NONE );
        //case ALT_TAB:
        //    // XXX: doesn't work after stuck key fix
        //    return (record->event.pressed ?
        //            MACRO( D(LALT), D(TAB), END ) :
        //            MACRO( U(TAB), END ));
    }
    return MACRO_NONE;
}

