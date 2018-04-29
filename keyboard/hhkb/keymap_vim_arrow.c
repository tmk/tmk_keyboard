/*
 * HHKB Layout + Vim Arrow
 */
#include "keymap_common.h"


#ifdef KEYMAP_SECTION_ENABLE
const uint8_t keymaps[][MATRIX_ROWS][MATRIX_COLS] __attribute__ ((section (".keymap.keymaps"))) = {
#else
const uint8_t keymaps[][MATRIX_ROWS][MATRIX_COLS] PROGMEM = {
#endif
    /* Layer 0: Default Layer
     * ,-----------------------------------------------------------.
     * |Esc|  1|  2|  3|  4|  5|  6|  7|  8|  9|  0|  -|  =|  \|  `|
     * |-----------------------------------------------------------|
     * |Tab  |  Q|  W|  E|  R|  T|  Y|  U|  I|  O|  P|  [|  ]|Backs|
     * |-----------------------------------------------------------|
     * |Contro|  A|  S|  D|  F|  G|  H|  J|  K|  L|  ;|  '|Enter   |
     * |-----------------------------------------------------------|
     * |Shift   |  Z|  X|  C|  V|  B|  N|  M|  ,|  .|  /|Shift |Fn0|
     * `-----------------------------------------------------------'
     *       |Alt|Gui  |         Space         |Gui  |Alt|
     *       `-------------------------------------------'
     */
    KEYMAP(ESC, 1,   2,   3,   4,   5,   6,   7,   8,   9,   0,   MINS,EQL, BSLS,GRV,   \
           TAB, Q,   W,   E,   R,   T,   Y,   U,   I,   O,   P,   LBRC,RBRC,BSPC,       \
           LCTL,A,   S,   D,   F,   G, FN1, FN2, FN3, FN4,   SCLN,QUOT,ENT,             \
           LSFT,Z,   X,   C,   V,   B,   N,   M,   COMM,DOT, SLSH,RSFT,FN0,             \
                LALT,LGUI,          SPC,                RGUI,RALT),

    /* Layer 1: HHKB mode (HHKB Fn)
     * ,-----------------------------------------------------------.
     * |Pwr| F1| F2| F3| F4| F5| F6| F7| F8| F9|F10|F11|F12|Ins|Del|
     * |-----------------------------------------------------------|
     * |Caps |   |   |   |   |   |   |   |Psc|Slk|Pus|Up |   |Backs|
     * |-----------------------------------------------------------|
     * |      |VoD|VoU|Mut|   |   |  *|  /|Hom|PgU|Lef|Rig|Enter   |
     * |-----------------------------------------------------------|
     * |        |   |   |   |   |   |  +|  -|End|PgD|Dow|      |   |
     * `-----------------------------------------------------------'
     *       |   |     |                       |     |   |
     *       `-------------------------------------------'
     */
    KEYMAP(PWR, F1,  F2,  F3,  F4,  F5,  F6,  F7,  F8,  F9,  F10, F11, F12, INS, DEL,   \
           CAPS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,PSCR,SLCK,PAUS, UP, TRNS, BSPC,      \
           TRNS,VOLD,VOLU,MUTE,TRNS,TRNS,PAST,PSLS,HOME,PGUP,LEFT,RGHT,PENT,            \
           TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,PPLS,PMNS,END, PGDN,DOWN,TRNS,TRNS,            \
                TRNS,TRNS,          TRNS,               TRNS,TRNS),
};

/*
 * Custom defined action function
 */

enum function_id {
    ARROW_H,
    ARROW_J,
    ARROW_K,
    ARROW_L
};

void action_function(keyrecord_t *record, uint8_t id, uint8_t opt)
{
#   define ARROW_MOD    MOD_BIT(KC_LCTL)
    static uint8_t isArrowModPressed;

    switch (id) {
        case ARROW_H:
            isArrowModPressed = get_mods() & ARROW_MOD;

            if (record->event.pressed) {
                if (isArrowModPressed) {
                    del_mods(ARROW_MOD);
                    send_keyboard_report();
                    add_key(KC_LEFT);
                    send_keyboard_report();
                    add_mods(ARROW_MOD);
                }
                else {
                    add_key(KC_H);
                    send_keyboard_report();
                }
            }
            else {
                if (isArrowModPressed) {
                    del_mods(ARROW_MOD);
                    del_key(KC_LEFT);
                    del_key(KC_H);
                    send_keyboard_report();
                    add_mods(ARROW_MOD);
                }
                else {
                    del_key(KC_LEFT);
                    del_key(KC_H);
                    send_keyboard_report();
                }
            }
            break;

        case ARROW_J:
            isArrowModPressed  = get_mods() & ARROW_MOD;

            if (record->event.pressed) {
                if (isArrowModPressed) {
                    del_mods(ARROW_MOD);
                    send_keyboard_report();
                    add_key(KC_DOWN);
                    send_keyboard_report();
                    add_mods(ARROW_MOD);
                }
                else {
                    add_key(KC_J);
                    send_keyboard_report();
                }
            }
            else {
                if (isArrowModPressed) {
                    del_mods(ARROW_MOD);
                    del_key(KC_DOWN);
                    del_key(KC_J);
                    send_keyboard_report();
                    add_mods(ARROW_MOD);
                }
                else {
                    del_key(KC_DOWN);
                    del_key(KC_J);
                    send_keyboard_report();
                }
            }
            break;

        case ARROW_K:
            isArrowModPressed = get_mods() & ARROW_MOD;

            if (record->event.pressed) {
                if (isArrowModPressed) {
                    del_mods(ARROW_MOD);
                    send_keyboard_report();
                    add_key(KC_UP);
                    send_keyboard_report();
                    add_mods(ARROW_MOD);
                }
                else {
                    add_key(KC_K);
                    send_keyboard_report();
                }
            }
            else {
                if (isArrowModPressed) {
                    del_mods(ARROW_MOD);
                    del_key(KC_UP);
                    del_key(KC_K);
                    send_keyboard_report();
                    add_mods(ARROW_MOD);
                }
                else {
                    del_key(KC_UP);
                    del_key(KC_K);
                    send_keyboard_report();
                }
            }
            break;

        case ARROW_L:
            isArrowModPressed = get_mods() & ARROW_MOD;

            if (record->event.pressed) {
                if (isArrowModPressed) {
                    del_mods(ARROW_MOD);
                    send_keyboard_report();
                    add_key(KC_RIGHT);
                    send_keyboard_report();
                    add_mods(ARROW_MOD);
                }
                else {
                    add_key(KC_L);
                    send_keyboard_report();
                }
            }
            else {
                if (isArrowModPressed) {
                    del_mods(ARROW_MOD);
                    del_key(KC_RIGHT);
                    del_key(KC_L);
                    send_keyboard_report();
                    add_mods(ARROW_MOD);
                }
                else {
                    del_key(KC_RIGHT);
                    del_key(KC_L);
                    send_keyboard_report();
                }
            }
            break;
    }
}

/*
 * Fn action definition
 */

const action_t fn_actions[] PROGMEM = {
    [0] = ACTION_LAYER_MOMENTARY(1),
    [1] = ACTION_FUNCTION(ARROW_H),
    [2] = ACTION_FUNCTION(ARROW_J),
    [3] = ACTION_FUNCTION(ARROW_K),
    [4] = ACTION_FUNCTION(ARROW_L)
};
