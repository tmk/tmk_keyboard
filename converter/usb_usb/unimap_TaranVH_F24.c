// https://github.com/TaranVH/2nd-keyboard/blob/bf7a70def94538c8475dd928a59f7c603e72c94e/HASU_USB/NO_REPLUG_ISSUES/keymap.c
// https://geekhack.org/index.php?topic=69169.msg2781175#msg2781175
//
// how to build:
// $ make -f Makefile.unimap KEYMAP=TaranVH_F24 clean
// $ make -f Makefile.unimap KEYMAP=TaranVH_F24


#include "unimap_trans.h"
#include "action_layer.h"

#ifndef PREFIX_KEY
#define PREFIX_KEY  KC_F24
#endif

#ifdef KEYMAP_SECTION_ENABLE
const action_t actionmaps[][UNIMAP_ROWS][UNIMAP_COLS] __attribute__ ((section (".keymap.keymaps"))) = {
#else
const action_t actionmaps[][UNIMAP_ROWS][UNIMAP_COLS] PROGMEM = {
#endif
    UNIMAP(
              F13, F14, F15, F16, F17, F18, F19, F20, F21, F22, F23,  NO,
    ESC,      F1,  F2,  F3,  F4,  F5,  F6,  F7,  F8,  F9,  F10, F11, F12,           PSCR,SLCK,PCMM,         VOLD,VOLU,MUTE,
    GRV, 1,   2,   3,   4,   5,   6,   7,   8,   9,   0,   MINS,EQL, JYEN,BSPC,     INS, HOME,PGUP,    INT6,PSLS,PAST,PMNS,
    TAB, Q,   W,   E,   R,   T,   Y,   U,   I,   O,   P,   LBRC,RBRC,     BSLS,     DEL, END, PGDN,    P7,  P8,  P9,  PPLS,
    F20, A,   S,   D,   F,   G,   H,   J,   K,   L,   SCLN,QUOT,     NUHS,ENT,                         P4,  P5,  P6,  PCMM,
    INT2,NUBS,Z,   X,   C,   V,   B,   N,   M,   COMM,DOT, SLSH,     RO,  INT3,          UP,           P1,  P2,  P3,  PENT,
    LANG2,LANG1,INT1,MHEN,         SPC,        HENK,KANA,LANG4,LANG3,INT4,INT5,     LEFT,DOWN,RGHT,    P0,       PDOT,PEQL
    ),
};


bool hook_process_action(keyrecord_t *record) {
    static uint8_t prefix_tracker;
    action_t action = layer_switch_get_action(record->event);

    switch (action.code) {
    case KC_A ... KC_EXSEL:
        if (record->event.pressed) {
            register_code(PREFIX_KEY); //this means to send F24 down
            prefix_tracker++;
            register_code(action.code);
        } else {
            unregister_code(action.code);
            prefix_tracker--;
            if (!prefix_tracker) {
                unregister_code(PREFIX_KEY); //this means to send F24 up
            }
        }
        return true;
        break;
    }
    return false;
}
