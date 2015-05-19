#include "keymap_common.h"

/*
 * JD
 */
const uint8_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
/* Keymap 0: Default Layer
     * ,-------------------------------------------------------.
     * |Esc  |  Q|  W|  E|  R|  T|  Y|  U|  I|  O|  P|  [| Bksp|
     * |-------------------------------------------------------|
     * |Tab   |  A|  S|  D|  F|  G|  H|  J|  K|  L|  ;|Return  |
     * |-------------------------------------------------------|
     * |Shift   |  Z|  X|  C|  V|  B|  N|  M|  ,|  .|  /| Shift|
     * |-------------------------------------------------------|
     * |Ctrl| Fn |Win|Alt | Space  | Space  |Alt |Win|Menu|Ctrl|
     * `-------------------------------------------------------'
     */
    KEYMAP_ANSI(
        ESC,    Q,  W,  E,  R,  T,  Y,  U,  I,  O,  P,LBRC,BSPC, \
        TAB,     A,  S,  D,  F,  G,  H,  J,  K,  L,   SCLN, ENT, \
        LSFT,      Z,  X,  C,  V,  B,  N,  M,COMM,DOT,SLSH,RSFT, \
        LCTL,FN0,LGUI,LALT,      SPC,     SPC,RALT,RGUI,APP,RCTL),
   
    /* Keymap 1: Fn mode
     * ,-------------------------------------------------------.
     * |`    |   | Up|   |  7|  8|  9|  0|  -|  =| F5|  ]|   \ |
     * |-------------------------------------------------------|
     * |Caps  |Lft| Dn|Rgt|  4|  5|  6|Ins|Home|PgUp| '|Return |
     * |-------------------------------------------------------|
     * |Shift   |PrSc|ScLk|Paus| 1| 2| 3|Del|End|PgDn| /| Shift|
     * |-------------------------------------------------------|
     * |Ctrl| Fn |Win|Alt | Space  | Space  |Alt |Win|Menu|Ctrl|
     * `-------------------------------------------------------'
     */
    KEYMAP_ANSI(
        GRV,TRNS, UP,TRNS,  7,  8,  9,  0,MINS,EQL,F5,RBRC,BSLS, \
        CAPS, LEFT,DOWN,RGHT,4,  5,  6,INS,HOME,PGUP, QUOT, ENT, \
        LSFT, PSCR, SLCK,PAUS, 1,  2,  3,DEL,END,PGDN,SLSH,RSFT, \
        LCTL,FN0,LGUI,LALT,      SPC,     SPC,RALT,RGUI,APP,RCTL),
};
 
/*
 * Fn action definition
 */
const uint16_t PROGMEM fn_actions[] = {
    [0] = ACTION_LAYER_MOMENTARY(1),
/*  [1] = ACTION_LAYER_TAP_KEY(5, KC_SLASH),
    [2] = ACTION_LAYER_TAP_KEY(6, KC_SCLN),
    [3] = ACTION_LAYER_MOMENTARY(6),
    [4] = ACTION_LAYER_MOMENTARY(7),   // to Layout selector
    [5] = ACTION_DEFAULT_LAYER_SET(0),  // set qwerty layout
    [6] = ACTION_DEFAULT_LAYER_SET(1),  // set colemak layout
    [7] = ACTION_DEFAULT_LAYER_SET(2),  // set dvorak layout
    [8] = ACTION_DEFAULT_LAYER_SET(3),  // set workman layout
    [9] = ACTION_MODS_TAP_KEY(MOD_RSFT, KC_GRV),*/
};
