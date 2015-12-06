#include "keymap_common.h"

/* Main layer:
   Test layout, using all keys.

      0     1     2     3     4     5     6     7     8     9     A     B     C     D     E
   ┌─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┐
 0 │GRAVE│  1  │  2  │  3  │  4  │  5  │  6  │  7  │  8  │  9  │  0  │  -  │  =  │PGUP │BKSPC│
   ├─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┤
 1 │ TAB │  Q  │  W  │  E  │  R  │  T  │  Y  │  U  │  I  │  O  │  P  │  [  │  ]  │  \  │█████│
   ├─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┤
 2 │CAPSL│  A  │  S  │  D  │  F  │  G  │  H  │  J  │  K  │  L  │  ;  │  '  │PGDN │ENTER│█████│
   ├─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┤
 3 │LSHFT│HOME │  Z  │  X  │  C  │  V  │  B  │  N  │  M  │  ,  │  .  │  /  │ END │RSHFT│  UP │
   ├─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┤
 4 │LCTRL│L_WIN│L_ALT│█████│█████│█████│ SPC │█████│█████│█████│R_ALT│R_WIN│ APP │RCTRL│█████│
   └─────┴─────┴─────┴─────┴─────┴─────┴─────┴─────┴─────┴─────┴─────┴─────┴─────┴─────┴─────┘
*/

const uint8_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /* 0: ANSI qwerty */
    KEYMAP(GRV,    1,    2,    3,    4,    5,    6,    7,    8,    9,    0, MINS,  EQL, PGUP, BSPC, \
           TAB,    Q,    W,    E,    R,    T,    Y,    U,    I,    O,    P, LBRC, RBRC, BSLS,       \
          CAPS,    A,    S,    D,    F,    G,    H,    J,    K,    L, SCLN, QUOT, PGDN, ENT ,       \
          LSFT, HOME,    Z,    X,    C,    V,    B,    N,    M, COMM,  DOT, SLSH,  END, RSFT,   UP, \
          LCTL, LGUI, LALT,                    SPC,                   RALT, RGUI,  APP, RCTL),
};
const uint16_t PROGMEM fn_actions[] = {
    /* Poker Layout */
/*    [0] = ACTION_LAYER_MOMENTARY(6),  // to Fn overlay
    [1] = ACTION_LAYER_TOGGLE(4),     // toggle arrow overlay
    [2] = ACTION_LAYER_TOGGLE(5),     // toggle Esc overlay
    [3] = ACTION_MODS_KEY(MOD_RCTL|MOD_RSFT, KC_ESC), // Task(RControl,RShift+Esc)
    [4] = ACTION_LAYER_MOMENTARY(7),  // to Layout selector
    [5] = ACTION_DEFAULT_LAYER_SET(0),  // set qwerty layout
    [6] = ACTION_DEFAULT_LAYER_SET(1),  // set colemak layout
    [7] = ACTION_DEFAULT_LAYER_SET(2),  // set dvorak layout
    [8] = ACTION_DEFAULT_LAYER_SET(3),  // set workman layout
*/
};

