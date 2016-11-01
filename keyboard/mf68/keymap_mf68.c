#include "keymap_common.h"
#include "backlight.h"

const uint8_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

KEYMAP(
    ESC,  1,    2,    3,    4,    5,    6,    7,    8,    9,    0,    MINS, EQL,  BSPC, INS,  PGUP,
    TAB,  Q,    W,    E,    R,    T,    Y,    U,    I,    O,    P,    LBRC, RBRC, BSLS, DEL,  PGDN,
    CAPS, A,    S,    D,    F,    G,    H,    J,    K,    L,    SCLN, QUOT,   ENT, 
    LSFT, Z,    X,    C,    V,    B,    N,    M,    COMM, DOT,  SLSH,        RSFT,      UP,
    LCTL, LGUI, LALT,                       SPC,                FN0,  RALT, RCTL, LEFT, DOWN, RGHT
),
KEYMAP(
    GRV,  F1,   F2,   F3,   F4,   F5,   F6,   F7,   F8,   F9,   F10,  F11,  F12,  TRNS, TRNS, HOME,
    TRNS, TRNS, TRNS, TRNS, TRNS, TRNS, TRNS, TRNS, TRNS, PSCR, SLCK, PAUS, CALC, TRNS, TRNS, END,
    TRNS, TRNS, TRNS, TRNS, TRNS, TRNS, TRNS, TRNS, TRNS, TRNS, HOME, TRNS,   TRNS, 
    TRNS, MSEL, MPLY, MSTP, MPRV, MNXT, VOLD, VOLU, MUTE, TRNS, END,         TRNS,      FN3, 
    TRNS, TRNS, TRNS,                       TRNS,               TRNS, TRNS, APP,  FN2,  FN1,  TRNS
),
};

const action_t PROGMEM fn_actions[] = {
    [0] = ACTION_LAYER_MOMENTARY(1),
    [1] = ACTION_BACKLIGHT_DECREASE(),
    [2] = ACTION_BACKLIGHT_TOGGLE(),
    [3] = ACTION_BACKLIGHT_INCREASE()
};

void action_function(keyrecord_t *record, uint8_t id, uint8_t opt) {

}
