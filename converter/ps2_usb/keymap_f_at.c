#include "keymap_common.h"

const uint8_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /* 0: default
     * ,-------. ,-----------------------------------------------------------. ,---------------.
     * | F1| F2| |  `|  1|  2|  3|  4|  5|  6|  7|  8|  9|  0|  -|  =| \ |Bsp| |Esc|NmL|ScL|SRq|
     * |---|---| |-----------------------------------------------------------| |---------------|
     * | F3| F4| |Tab  |  Q|  W|  E|  R|  T|  Y|  U|  I|  O|  P|  [|  ]|     | |  7|  8|  9|  *|
     * |---|---| |------------------------------------------------------     | |---------------|
     * | F5| F6| |Ctrl  |  A|  S|  D|  F|  G|  H|  J|  K|  L|  ;|  '|   Enter| |  4|  5|  6|  -|
     * |---|---| |-----------------------------------------------------------| |---------------|
     * | F7| F8| |Shift   |  Z|  X|  C|  V|  B|  N|  M|  ,|  .|  /|    Shift | |  1|  2|  3|   |
     * |---|---| |-----------------------------------------------------------| |------------  +|
     * | F9|F10| |Alt|    |             Space                     |    |CapsL| |      0|  .|   |
     * `-------' `---'    `---------------------------------------'    `-----' `---------------'
     */
    KEYMAP_AT(
    F1, F2,   GRV, 1,   2,   3,   4,   5,   6,   7,   8,   9,   0,   MINS,EQL, BSLS,BSPC,  ESC, NLCK,SLCK,BRK,
    F3, F4,   TAB, Q,   W,   E,   R,   T,   Y,   U,   I,   O,   P,   LBRC,RBRC,     NO,    P7,  P8,  P9,  PAST,
    F5, F6,   LCTL,A,   S,   D,   F,   G,   H,   J,   K,   L,   SCLN,QUOT,      NO, ENT,   P4,  P5,  P6,  PMNS,
    F7, F8,   LSFT,NO,  Z,   X,   C,   V,   B,   N,   M,   COMM,DOT, SLSH,      NO,RSFT,   P1,  P2,  P3,  PPLS,
    F9, F10,  LALT,    NO,             SPC,                     NO,                CAPS,   NO,  P0,  PDOT,NO
    ),

};

const action_t PROGMEM fn_actions[] = {
};
