#include "keymap_common.h"

const uint8_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

KEYMAP(
  ESC,  Q,    W,    E,    R,    T,    Y,    U,    I,    O,    P,    BSPC,
  FN2,  A,    S,    D,    F,    G,    H,    J,    K,    L,    SCLN, QUOT,
  LSFT, Z,    X,    C,    V,    B,    N,    M,    COMM, DOT,  SLSH, FN3
),

KEYMAP(
  GRV,  1,    2,    3,    4,    5,    6,    7,    8,    9,    0,    DELETE,
  TRNS, TRNS, TRNS, TRNS, TRNS, TRNS, TRNS, MINS, EQL,  LBRC, RBRC, BSLS,
  TRNS, F11,  F12,  F13,  F14,  F15,  F16,  F17,  F18,  F19,  F20,  TRNS
),

KEYMAP(
  FN6,  FN7,  FN8,  FN9,  FN10, FN11, FN12, FN13, FN14, FN15, FN16, DELETE,
  TRNS, TRNS, TRNS, TRNS, TRNS, TRNS, TRNS, FN17, FN18, FN19, FN20, FN21,
  TRNS, F1,   F2,   F3,   F4,   F5,   F6,   F7,   F8,   F9,   F10,  TRNS
),

KEYMAP( /* Tab */
  ESC,  CALC, WHOM, MAIL, MYCM, TRNS, TRNS, TRNS, TRNS, TRNS, PSCR, TRNS,
  TRNS, TRNS, TRNS, TRNS, TRNS, TRNS, TRNS, TRNS, TRNS, TRNS, TRNS, TRNS,
  TRNS, FN4,  FN5,  TRNS, TRNS, TRNS, TRNS, TRNS, TRNS, TRNS, TRNS, TRNS
),
};

const action_t PROGMEM fn_actions[] = {
    [0]  = ACTION_LAYER_MOMENTARY(1),
    [1]  = ACTION_LAYER_MOMENTARY(2),
    [2]  = ACTION_LAYER_TAP_KEY(3, KC_TAB),
    [3]  = ACTION_MODS_TAP_KEY(MOD_RSFT, KC_ENT),
    [4]  = ACTION_BACKLIGHT_DECREASE(),
    [5]  = ACTION_BACKLIGHT_INCREASE(),
    [6]  = ACTION_MODS_KEY(MOD_LSFT, KC_GRV),
    [7]  = ACTION_MODS_KEY(MOD_LSFT, KC_1),
    [8]  = ACTION_MODS_KEY(MOD_LSFT, KC_2),
    [9]  = ACTION_MODS_KEY(MOD_LSFT, KC_3),
    [10] = ACTION_MODS_KEY(MOD_LSFT, KC_4),
    [11] = ACTION_MODS_KEY(MOD_LSFT, KC_5),
    [12] = ACTION_MODS_KEY(MOD_LSFT, KC_6),
    [13] = ACTION_MODS_KEY(MOD_LSFT, KC_7),
    [14] = ACTION_MODS_KEY(MOD_LSFT, KC_8),
    [15] = ACTION_MODS_KEY(MOD_LSFT, KC_9),
    [16] = ACTION_MODS_KEY(MOD_LSFT, KC_0),
    [17] = ACTION_MODS_KEY(MOD_LSFT, KC_MINS),
    [18] = ACTION_MODS_KEY(MOD_LSFT, KC_EQL),
    [19] = ACTION_MODS_KEY(MOD_LSFT, KC_LBRC),
    [20] = ACTION_MODS_KEY(MOD_LSFT, KC_RBRC),
    [21] = ACTION_MODS_KEY(MOD_LSFT, KC_BSLS),
};

void action_function(keyrecord_t *record, uint8_t id, uint8_t opt) {
}
