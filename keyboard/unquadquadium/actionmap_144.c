#include "actionmap.h"
#include "action_code.h"
#include "actionmap_common.h"

/*
 * Actions
 */

const action_t PROGMEM actionmaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = ACTIONMAP( \
    EQL,  BSPC, BSPC, 3,    BSLS, GRV,  ENT,  ENT,  PGUP, RSFT, UP,   PGDN,
    ESC,  1,    2,    3,    4,    5,    6,    7,    8,    9,    0,    MINS,
    TAB,  Q,    W,    E,    R,    T,    Y,    U,    I,    O,    P,    LBRC,
    CAPS, A,    S,    D,    F,    G,    H,    J,    K,    L,    SCLN, QUOT,
    F1,   F2,   F3,   F4,   F5,   F6,   F7,   F8,   F9,   F10,  F11,  F12,
    LCTL, LGUI, LALT, APP,  SPC,  SPC,  SPC,  SPC,  X,    RALT, RGUI, RCTL,
    EQL,  BSPC, BSPC, 3,    BSLS, GRV,  ENT,  ENT,  PGUP, RSFT, UP,   PGDN,
    ESC,  1,    2,    3,    4,    5,    6,    7,    8,    9,    0,    MINS,
    TAB,  Q,    W,    E,    R,    T,    Y,    U,    I,    O,    P,    LBRC,
    CAPS, A,    S,    D,    F,    G,    H,    J,    K,    L,    SCLN, QUOT,
    LSFT, Z,    X,    C,    V,    B,    N,    M,    COMM, DOT,  SLSH, RSFT,
    LCTL, LGUI, LALT, APP,  SPC,  SPC,  SPC,  SPC,  X,    RALT, RGUI, RCTL),
    [1] = ACTIONMAP( \
    TRNS, TRNS, TRNS, TRNS, TRNS, TRNS, TRNS, TRNS, TRNS, TRNS, TRNS, TRNS,
    TRNS, TRNS, TRNS, TRNS, TRNS, TRNS, TRNS, TRNS, TRNS, TRNS, TRNS, TRNS,
    TRNS, TRNS, TRNS, TRNS, TRNS, TRNS, TRNS, TRNS, TRNS, TRNS, TRNS, TRNS,
    TRNS, TRNS, TRNS, TRNS, TRNS, TRNS, TRNS, TRNS, TRNS, TRNS, TRNS, TRNS,
    TRNS, TRNS, TRNS, TRNS, TRNS, TRNS, TRNS, TRNS, TRNS, TRNS, TRNS, TRNS,
    TRNS, TRNS, TRNS, TRNS, TRNS, TRNS, TRNS, TRNS, TRNS, TRNS, TRNS, TRNS,
    TRNS, TRNS, TRNS, TRNS, TRNS, TRNS, TRNS, TRNS, TRNS, TRNS, TRNS, TRNS,
    TRNS, TRNS, TRNS, TRNS, TRNS, TRNS, TRNS, TRNS, TRNS, TRNS, TRNS, TRNS,
    TRNS, TRNS, TRNS, TRNS, TRNS, TRNS, TRNS, TRNS, TRNS, TRNS, TRNS, TRNS,
    TRNS, TRNS, TRNS, TRNS, TRNS, TRNS, TRNS, TRNS, TRNS, TRNS, TRNS, TRNS,
    TRNS, TRNS, TRNS, TRNS, TRNS, TRNS, TRNS, TRNS, TRNS, TRNS, TRNS, TRNS,
    TRNS, TRNS, TRNS, TRNS, TRNS, TRNS, TRNS, TRNS, TRNS, TRNS, TRNS, TRNS),
};
