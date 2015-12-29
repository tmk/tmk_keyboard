
static const uint8_t PROGMEM keymaps[][MATRIX_COLS][MATRIX_ROWS] =
  {
    {
      /*          0     1     2     3     4     5     6     7   */
      /* A  0 */ {KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A},
      /* B  1 */ {KC_B, KC_B, KC_B, KC_B, KC_B, KC_B, KC_B, KC_B},
      /* C  2 */ {KC_C, KC_C, KC_C, KC_C, KC_C, KC_C, KC_C, KC_C},
      /* D  3 */ {KC_D, KC_D, KC_D, KC_D, KC_D, KC_D, KC_D, KC_D},
      /* E  4 */ {KC_E, KC_E, KC_E, KC_E, KC_E, KC_E, KC_E, KC_E},
      /* F  5 */ {KC_F, KC_F, KC_F, KC_F, KC_F, KC_F, KC_F, KC_F},
      /* G  6 */ {KC_G, KC_G, KC_G, KC_G, KC_G, KC_G, KC_G, KC_G},
      /* H  7 */ {KC_H, KC_H, KC_H, KC_H, KC_H, KC_H, KC_H, KC_H},
      /* I  8 */ {KC_I, KC_I, KC_I, KC_I, KC_I, KC_I, KC_I, KC_I},
      /* J  9 */ {KC_J, KC_J, KC_J, KC_J, KC_J, KC_J, KC_J, KC_J},
      /* K 10 */ {KC_K, KC_K, KC_K, KC_K, KC_K, KC_K, KC_K, KC_K},
      /* L 11 */ {KC_L, KC_L, KC_L, KC_L, KC_L, KC_L, KC_L, KC_L},
      /* M 12 */ {KC_M, KC_M, KC_M, KC_M, KC_M, KC_M, KC_M, KC_M},
      /* N 13 */ {KC_N, KC_N, KC_N, KC_N, KC_N, KC_N, KC_N, KC_N},
      /* O 14 */ {KC_O, KC_O, KC_O, KC_O, KC_O, KC_O, KC_O, KC_O},
      /* P 15 */ {KC_P, KC_P, KC_P, KC_P, KC_P, KC_P, KC_P, KC_P}
    },
    {
      /*          0     1     2     3     4     5     6     7   */
      /* A  0 */ {KC_0, KC_1, KC_2, KC_3, KC_4, KC_5, KC_6, KC_7},
      /* B  1 */ {KC_0, KC_1, KC_2, KC_3, KC_4, KC_5, KC_6, KC_7},
      /* C  2 */ {KC_0, KC_1, KC_2, KC_3, KC_4, KC_5, KC_6, KC_7},
      /* D  3 */ {KC_0, KC_1, KC_2, KC_3, KC_4, KC_5, KC_6, KC_7},
      /* E  4 */ {KC_0, KC_1, KC_2, KC_3, KC_4, KC_5, KC_6, KC_7},
      /* F  5 */ {KC_0, KC_1, KC_2, KC_3, KC_4, KC_5, KC_6, KC_7},
      /* G  6 */ {KC_0, KC_1, KC_2, KC_3, KC_4, KC_5, KC_6, KC_7},
      /* H  7 */ {KC_0, KC_1, KC_2, KC_3, KC_4, KC_5, KC_6, KC_7},
      /* I  8 */ {KC_0, KC_1, KC_2, KC_3, KC_4, KC_5, KC_6, KC_7},
      /* J  9 */ {KC_0, KC_1, KC_2, KC_3, KC_4, KC_5, KC_6, KC_7},
      /* K 10 */ {KC_0, KC_1, KC_2, KC_3, KC_4, KC_5, KC_6, KC_7},
      /* L 11 */ {KC_0, KC_1, KC_2, KC_3, KC_4, KC_5, KC_6, KC_7},
      /* M 12 */ {KC_0, KC_1, KC_2, KC_3, KC_4, KC_5, KC_6, KC_7},
      /* N 13 */ {KC_0, KC_1, KC_2, KC_3, KC_4, KC_5, KC_6, KC_7},
      /* O 14 */ {KC_0, KC_1, KC_2, KC_3, KC_4, KC_5, KC_6, KC_7},
      /* P 15 */ {KC_0, KC_1, KC_2, KC_3, KC_4, KC_5, KC_6, KC_7}
    }
  };

// static const uint8_t PROGMEM keymaps[][MATRIX_COLS][MATRIX_ROWS] = {
//     /* Layer 0: Standard ANSI layer */
//     KEYMAP(\
//          ESC, F1,  F2,  F3,  F4,  F5,  F6,  F7,  F8,  F9,  F10, F11,  F12,        PSCR,SLCK,PAUS,                         \
//          GRV, 1,   2,   3,   4,   5,   6,   7,   8,   9,   0,   MINS, EQL,BSPC,   INS, HOME,PGUP,    NLCK,PSLS,PAST,PMNS, \
//          TAB, Q,   W,   E,   R,   T,   Y,   U,   I,   O,   P,   LBRC,RBRC,BSLS,   DEL, END, PGDN,    P7,  P8,  P9,  PPLS, \
//          CAPS,A,   S,   D,   F,   G,   H,   J,   K,   L,   SCLN,QUOT,      ENT,                      P4,  P5,  P6,        \
//          LSFT,NUBS,Z,   X,   C,   V,   B,   N,   M,   COMM,DOT, SLSH,     RSFT,        UP,           P1,  P2,  P3,  PENT, \
//          LCTL,LGUI,LALT,               SPC,                RALT,RGUI, FN0,RCTL,   LEFT,DOWN,RGHT,    P0,  PDOT),          \
//     /* Layer 1: Function layer */
//     KEYMAP(\
//          CALC,MYCM,WSCH,WHOM,MAIL,VOLD,VOLU,MSEL,MSTP,MPLY,MPRV,MNXT,TRNS,        WAKE, PWR,SLEP,                         \
//          TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,   TRNS,TRNS,TRNS,    TRNS,TRNS,TRNS,TRNS, \
//          TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,   TRNS,TRNS,TRNS,    TRNS,TRNS,TRNS,TRNS, \
//          TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,     TRNS,                      TRNS,TRNS,TRNS,      \
//          TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,     TRNS,        TRNS,         TRNS,TRNS,TRNS,TRNS, \
//          TRNS,TRNS,TRNS,               TRNS,               TRNS,TRNS,TRNS,TRNS,   TRNS,TRNS,TRNS,    TRNS,TRNS)
// };

static const uint16_t PROGMEM fn_actions[] = {
    [0] = ACTION_LAYER_MOMENTARY(1)
};
