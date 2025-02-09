#include "keymap_common.h"

/*
 * SpaceFN
 * http://geekhack.org/index.php?topic=51069.0
 */
const uint8_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /* Keymap 0: Default Layer
     * ,-----------------------------------------------------------.
     * |Esc|  1|  2|  3|  4|  5|  6|  7|  8|  9|  0|  -|  =|Backsp |
     * |-----------------------------------------------------------|
     * |Tab  |  Q|  W|  E|  R|  T|  Y|  U|  I|  O|  P|  [|  ]|    \|
     * |-----------------------------------------------------------|
     * |Caps  |  A|  S|  D|  F|  G|  H|  J|  K|  L|  ;|  '|Return  |
     * |-----------------------------------------------------------|
     * |Shift   |  Z|  X|  C|  V|  B|  N|  M|  ,|  .|  /|Shift     |
     * |-----------------------------------------------------------|
     * |Ctrl|Gui |Alt |  FN0      |    Space   |Alt |Gui |App |Ctrl|
     * `-----------------------------------------------------------'
     */
    KEYMAP_Z62(
        ESC, 1,   2,   3,   4,   5,   6,   7,   8,   9,   0,   MINS,EQL, BSPC, \
        TAB, Q,   W,   E,   R,   T,   Y,   U,   I,   O,   P,   LBRC,RBRC,BSLS, \
        CAPS,A,   S,   D,   F,   G,   H,   J,   K,   L,   SCLN,QUOT,     ENT,  \
        LSFT,Z,   X,   C,   V,   B,   N,   M,   COMM,DOT, SLSH,          RSFT, \
        LCTL,LGUI,LALT,          FN0,                SPC, RALT,FN0,RGUI, RCTL),
    /* Overlay 1: SpaceFN
     * ,-----------------------------------------------------------.
     * |`    | F1| F2| F3| F4| F5| F6| F7| F8| F9|F10|F11|F12|Del  |
     * |-----------------------------------------------------------|
     * |     |   |   |   |   |   |   |PgU|Hom|End|PgU|   |   |Ins  |
     * |-----------------------------------------------------------|
     * |      |   |   |   |   |   |Lef|Dow|Up |Rig|PgD|   |        |
     * |-----------------------------------------------------------|
     * |        |   |   |   |   |   |PgD|Mut|VoD|VoU|   |          |
     * |-----------------------------------------------------------|
     * |    |    |    |           |            |    |    |    |    |
     * `-----------------------------------------------------------'
     */
    KEYMAP_Z62(
        GRV, F1,  F2,  F3,  F4,  F5,  F6,  F7,  F8,  F9,  F10, F11, F12, DEL,  \
        TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,PGUP,HOME,END, PGUP,TRNS,TRNS,INS,  \
        TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,LEFT,DOWN,UP,  RGHT,PGDN,TRNS,     TRNS, \
        TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,PGDN,MUTE,VOLD,VOLU,TRNS,          TRNS, \
        TRNS,TRNS,TRNS,          TRNS,               SPC, TRNS,TRNS,TRNS,TRNS),
};

/*
 * Fn action definition
 */
const action_t PROGMEM fn_actions[] = {
    [0] = ACTION_LAYER_MOMENTARY(1), 
    [1] = ACTION_MODS_KEY(MOD_LSFT, KC_GRV),    // tilde
};
