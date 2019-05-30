#include "actionmap.h"
#include "action_code.h"
#include "actionmap_common.h"


/*
 * Actions
 */
#define AC_TL1    ACTION_LAYER_TAP_KEY(3, KC_TAB)
#define AC_TM1    ACTION_LAYER_MOMENTARY(1)
#define AC_TM2    ACTION_LAYER_MOMENTARY(2)
#define AC_MT1    ACTION_MODS_TAP_KEY(MOD_RSFT, KC_ENT)
#define AC_S01    ACTION_MODS_KEY(MOD_LSFT, KC_1)
#define AC_S02    ACTION_MODS_KEY(MOD_LSFT, KC_2)
#define AC_S03    ACTION_MODS_KEY(MOD_LSFT, KC_3)
#define AC_S04    ACTION_MODS_KEY(MOD_LSFT, KC_4)
#define AC_S05    ACTION_MODS_KEY(MOD_LSFT, KC_5)
#define AC_S06    ACTION_MODS_KEY(MOD_LSFT, KC_6)
#define AC_S07    ACTION_MODS_KEY(MOD_LSFT, KC_7)
#define AC_S08    ACTION_MODS_KEY(MOD_LSFT, KC_8)
#define AC_S09    ACTION_MODS_KEY(MOD_LSFT, KC_9)
#define AC_S10    ACTION_MODS_KEY(MOD_LSFT, KC_0)
#define AC_S11    ACTION_MODS_KEY(MOD_LSFT, KC_MINS)
#define AC_S12    ACTION_MODS_KEY(MOD_LSFT, KC_EQL)
#define AC_S13    ACTION_MODS_KEY(MOD_LSFT, KC_LBRC)
#define AC_S14    ACTION_MODS_KEY(MOD_LSFT, KC_RBRC)
#define AC_S15    ACTION_MODS_KEY(MOD_LSFT, KC_BSLS)
#define AC_S16    ACTION_MODS_KEY(MOD_LSFT, KC_GRV)

const action_t PROGMEM actionmaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = ACTIONMAP(
  ESC,  1,    2,    3,    4,    5,
  GRV,  Q,    W,    E,    R,    T,
  TL1,  A,    S,    D,    F,    G,
  LSFT, Z,    X,    C,    V,    B,
  LCTL, LALT, LGUI, APP,  TM2,  SPC,
  
  6,    7,    8,    9,    0,    BSPC,
  Y,    U,    I,    O,    P,    DELETE,
  H,    J,    K,    L,    SCLN, QUOT,
  N,    M,    COMM, DOT,  SLSH, MT1,
  SPC,  TM1,  LEFT, DOWN, UP,   RGHT
  ),

    [1] = ACTIONMAP(
  TRNS, F1,   F2,   F3,   F4,   F5,
  GRV,  1,    2,    3,    4,    5,
  TRNS, TRNS, TRNS, TRNS, TRNS, TRNS,
  TRNS, F11,  F12,  F13,  F14,  F15,
  TRNS, TRNS, TRNS, CAPS, TRNS, TRNS,
  
  F6,   F7,   F8,   F9,   F10,  TRNS,
  6,    7,    8,    9,    0,    DELETE,
  TRNS, MINS, EQL,  LBRC, RBRC, BSLS,
  F16,  F17,  F18,  F19,  F20,  TRNS,
  TRNS, TRNS, HOME, PGDN, PGUP, END
  ),

    [2] = ACTIONMAP(
  TRNS, TRNS, TRNS, TRNS, TRNS, TRNS,
  S16,  S01,  S02,  S03,  S04,  S05,
  TRNS, TRNS, TRNS, TRNS, TRNS, TRNS,
  TRNS, F1,   F2,   F3,   F4,   F5,
  TRNS, TRNS, TRNS, CAPS, TRNS, TRNS,

  TRNS, TRNS, TRNS, TRNS, TRNS, TRNS,
  S06,  S07,  S08,  S09,  S10,  DELETE,
  TRNS, S11,  S12,  S13,  S14,  S15,
  F6,   F7,   F8,   F9,   F10,  TRNS,
  TRNS, TRNS, HOME, PGDN, PGUP, END
  ),

    [3] = ACTIONMAP(
  TRNS, TRNS, TRNS, TRNS, TRNS, TRNS,
  TRNS, CALC, WHOM, MAIL, MYCM, TRNS,
  TRNS, TRNS, TRNS, TRNS, TRNS, TRNS,
  TRNS, TRNS, TRNS, TRNS, TRNS, TRNS,
  TRNS, TRNS, TRNS, TRNS, TRNS, TRNS,
  
  TRNS, TRNS, TRNS, TRNS, TRNS, TRNS,
  TRNS, TRNS, TRNS, TRNS, PSCR, TRNS,
  TRNS, TRNS, TRNS, TRNS, TRNS, TRNS,
  TRNS, TRNS, TRNS, TRNS, TRNS, TRNS,
  TRNS, TRNS, TRNS, TRNS, TRNS, TRNS
  ),
};

