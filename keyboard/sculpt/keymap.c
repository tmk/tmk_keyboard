#include "keymap_common.h"

const uint8_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /* 0: qwerty */
    KEYMAP(
        PAUS, DEL, 0, 9, 8, BSPC, 7, TAB, Q, 2, 1, \
        PGUP, F12,LBRC,MINS,RBRC, INS, Y, F5, F3, W, 4, F6, \
        HOME, CALC, P, O, I, U, R, E, CAPS, 3, T, \
        SLCK, ENT, SCLN, L, K, BSLS, J, F, D, A, LGUI, \
        RALT, FN1, SLSH, QUOT, LEFT, H, G, F4, S, ESC, LALT, \
        END, RSFT, PGDN, DOT, COMM, M, V, C, X, Z, LSFT, \
        RCTL, RGHT, UP, DOWN, SPC, N, B, SPC, LCTL, \
        PSCR, F11, EQL, F9, F8, F10, F7, 5, F2, F1, GRV, 6), 
    /* 1: FN1 (Replaces arrows with pagination, home and end and activates
     */        multimedia keys on top row).
    KEYMAP(
        TRNS, TRNS, TRNS, TRNS, TRNS, TRNS, TRNS, TRNS, TRNS, TRNS, TRNS, \
        TRNS, TRNS, TRNS, TRNS, TRNS, TRNS, TRNS, TRNS, VOLD, TRNS, TRNS, TRNS, \
        TRNS, TRNS, TRNS, TRNS, TRNS, TRNS, TRNS, TRNS, TRNS, TRNS, TRNS, \
        TRNS, TRNS, TRNS, TRNS, TRNS, TRNS, TRNS, TRNS, TRNS, TRNS, TRNS, \
        TRNS, TRNS, TRNS, TRNS, HOME, TRNS, TRNS, VOLU, TRNS, TRNS, TRNS, \
        TRNS, TRNS, TRNS, TRNS, TRNS, TRNS, TRNS, TRNS, TRNS, TRNS, TRNS, \
        TRNS, END, PGUP, PGDN, TRNS, TRNS, TRNS, TRNS, TRNS, \
        TRNS, TRNS, TRNS, TRNS, TRNS, TRNS, TRNS, TRNS, MUTE, MPLY, TRNS, TRNS ),
};
const uint16_t PROGMEM fn_actions[] = {
    ACTION_DEFAULT_LAYER_SET(0),    // Default layer. Not used.
    ACTION_LAYER_TAP_KEY(1, KC_APP),  // FN1
};
