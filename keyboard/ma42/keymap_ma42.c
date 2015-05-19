#include "keymap_common.h"

/*
 * MA42
 */
const uint8_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  /* Keymap 0: Default Layer
   * ,---------------------------------------------------.
   * |Esc |  '|  ,|  .|  P|  Y|  F|  G|  C|  R|  L| Bksp |
   * |---------------------------------------------------|
   * |Tab  |  A|  O|  E|  U|  I|  D|  H|  T|  N|  S|  -  |
   * |---------------------------------------------------|
   * |LSFT |  ;|  Q|  J|  K|  X|  B|  M|  W|  V|  Z| RSFT|
   * |---------------------------------------------------|
   * |LCTL|Alt| GUI |   RET   |   SPC  | Fn |GUI|ALT|RCTL|
   * `---------------------------------------------------'
   */
  KEYMAP_ANSI(
              GRV,QUOT,COMM, DOT, P,  Y,  F,  G,  C,  R,  L, BSPC, \
              TAB,   A,   O,   E, U,  I,  D,  H,  T,  N,  S,MINS, \
              LSFT,    SCLN,Q,   J, K,  X,  B,  M,  W,  V, Z,RSFT,  \
              LCTL,LALT,LGUI,ENT,            SPC,FN0,RGUI,RALT,RCTL),

  /* Keymap 1: Fn mode
   * ,---------------------------------------------------.
   * |  ` |  1|  2|  3|  4|  5|  6|  7|  8|  9|  0|   \  |
   * |---------------------------------------------------|
   * |Tab  |   |   |   |   |   |   |   |   |  [|  ]|  /  |
   * |---------------------------------------------------|
   * |LSFT |   |   |   |   |   |   |   |  -|  =| UP| RSFT|
   * |---------------------------------------------------|
   * |LCTL|Alt| GUI |   RET   |   SPC  | Fn |LFT|DWN|RGHT|
   * `---------------------------------------------------'
   */
  KEYMAP_ANSI(
              GRV, 1,  2,  3,  4,  5,  6,  7,  8,  9,    0,      BSLS, \
              TRNS, NO, NO, NO, NO, NO, NO, NO, NO, LBRC, RBRC,  SLSH, \
              TRNS, NO, NO, NO, NO, NO, NO, NO, MINS, EQL,  UP,   TRNS, \
                         TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,LEFT,DOWN,RGHT),
};

/*
 * Fn action definition
 */
const uint16_t PROGMEM fn_actions[] = {
  [0] = ACTION_LAYER_MOMENTARY(1),
};
