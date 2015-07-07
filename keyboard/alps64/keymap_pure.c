#include "keymap_common.h"

/*
 * Pure
 */
const uint8_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

    [0] = KEYMAP_AEK( \
        ESC, 1,   2,   3,   4,   5,   6,   7,   8,   9,   0,   MINS,EQL, BSPC, \
        TAB, Q,   W,   E,   R,   T,   Y,   U,   I,   O,   P,   LBRC,RBRC,BSLS, \
        CAPS,A,   S,   D,   F,   G,   H,   J,   K,   L,   SCLN, QUOT,ENT, \
        LSFT,Z,   X,   C,   V,   B,   N,   M,   COMM,DOT, SLSH, RSFT, \
        LCTL,LGUI,LALT,          SPC,                          FN0,RALT, RCTL),


    [1] = KEYMAP_AEK( \
        GRV, F1,  F2,  F3,  F4,  F5,  F6,  F7,  F8,  F9,  F10, F11, F12, DEL, \
        TRNS ,TRNS,  TRNS,  TRNS,  TRNS,  TRNS,  TRNS,  TRNS,  INS,UP,PSCR,VOLD,  VOLU,  MUTE, \
        TRNS ,TRNS,TRNS,TRNS,TRNS,  TRNS,  TRNS,PGUP,LEFT,DOWN,RGHT,SLCK, TRNS, \
        FN1,TRNS,  TRNS,  TRNS,  TRNS,  TRNS,  TRNS,PGDN,HOME, END,PAUS,RSFT, \
        TRNS,TRNS,TRNS,          TRNS,                         TRNS,TRNS,TRNS)
};


/* id for user defined function/macro */
enum function_id {
    NONE,
};

enum macro_id {
    CTRLALTDEL,
};


/*
 * Fn action definition
 */
const uint16_t PROGMEM fn_actions[] = {
    [0] = ACTION_LAYER_MOMENTARY(1),                  // Pure Layer
    [1] = ACTION_MACRO(CTRLALTDEL)
};


/*
 * Macro definition
 */
const macro_t *action_get_macro(keyrecord_t *record, uint8_t id, uint8_t opt)
{
    switch (id) {
        case CTRLALTDEL:
            return (record->event.pressed ?
                    MACRO( D(LALT), D(LCTL), D(DEL), END ) :
                    MACRO( U(TAB), END ));
    }
    return MACRO_NONE;
}
