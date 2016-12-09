#include "keymap_common.h"
#include "backlight.h"

const uint8_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

KEYMAP(
  ESC,  Q,    W,    E,    R,    T,    Y,    U,    I,    O,    P,   
  FN0,  A,    S,    D,    F,    G,    H,    J,    K,    L,    SCLN,
  LSFT, Z,    X,    C,    V,    B,    N,    M,    COMM, DOT,  SLSH,
   LCTL,  LALT,   LGUI,    BSPC,   SPC,     LEFT, DOWN, UP,   RGHT   
),
KEYMAP(
  GRV,  1,    2,    3,    4,    5,    6,    7,    8,    9,    0,   
  TRNS, TRNS, TRNS, TRNS, TRNS, TRNS, TRNS, MINS, EQL,  LBRC, RBRC,
  TRNS, F11,  F12,  F13,  F14,  F15,  F16,  F17,  F18,  F19,  F20, 
   TRNS,  TRNS,   TRNS,    TRNS,   TRNS,    TRNS, FN2,  FN1,  FN3   
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
