#include "extended_keymap_common.h"

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [0] = { /* Joe soft Colemak */
    {FUNC(3),  CM_Q,    CM_W,    CM_F,    CM_P,    CM_G,   CM_J,   CM_L,    CM_U,    CM_Y,    CM_SCLN, KC_MINS},
    {KC_BSPC,  CM_A,    CM_R,    CM_S,    CM_T,    CM_D,   CM_H,   CM_N,    CM_E,    CM_I,    CM_O,    KC_ENT },
    {FUNC(15), CM_Z,    CM_X,    CM_C,    CM_V,    CM_B,   CM_K,   CM_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_QUOT},
    {KC_LCTL,  KC_LGUI, KC_LALT, KC_LSFT, FUNC(1), KC_SPC, KC_SPC, FUNC(2), KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT}
  },
  [1] = { /* Joe colemak */
    {FUNC(3),  KC_Q,    KC_W,    KC_F,    KC_P,    KC_G,   KC_J,   KC_L,    KC_U,    KC_Y,    KC_SCLN, KC_MINS},
    {KC_BSPC,  KC_A,    KC_R,    KC_S,    KC_T,    KC_D,   KC_H,   KC_N,    KC_E,    KC_I,    KC_O,    KC_ENT },
    {FUNC(15), KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,   KC_K,   KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_QUOT},
    {KC_LCTL,  KC_LGUI, KC_LALT, KC_LSFT, FUNC(1), KC_SPC, KC_SPC, FUNC(2), KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT}
  },
  [2] = { /* Joe NUMPAD */
    {FUNC(3), KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, LSFT(KC_9), LSFT(KC_0), KC_P7, KC_P8,   KC_P9,   KC_PSLS},
    {KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, LSFT(KC_5), KC_PEQL,    KC_P4, KC_P5,   KC_P6,   KC_PAST},
    {KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,    KC_PCMM,    KC_P1, KC_P2,   KC_P3,   KC_PMNS},
    {KC_LCTL, KC_LGUI, KC_LALT, KC_LSFT, FUNC(1), KC_TRNS, KC_TRNS,    FUNC(2),    KC_P0, KC_PDOT, KC_PENT, KC_PPLS}
  },
  [4] = { /* Joe LOWER fn1 */
    {KC_GRV,  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,  FUNC(28), FUNC(27), FUNC(26), FUNC(25)},
    {KC_TRNS, KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,     KC_8,     KC_9,     KC_0,     KC_TRNS },
    {KC_BSLS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,  KC_TRNS,  KC_LBRC,  KC_RBRC,  KC_EQL  },
    {KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, FUNC(1), KC_TRNS, KC_TRNS, FUNC(2),  KC_HOME,  KC_PGDN,  KC_PGUP,  KC_END  }
  },
  [5] = { /* Joe UPPER fn2 */
    {KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,               KC_F10,              KC_F11,            KC_F12             },
    {KC_DEL,  KC_BTN1, KC_MS_U, KC_BTN2, KC_BTN3, KC_TRNS, KC_TRNS, KC_LEFT, KC_DOWN,             KC_UP,               KC_RGHT,           RESET              },
    {KC_TRNS, KC_MS_L, KC_MS_D, KC_MS_R, KC_BTN4, KC_MENU, KC_CAPS, KC_INS,  KC_PSCR,             KC_TRNS,             LCTL(KC_PGUP),     LCTL(KC_PGDN)      },
    {KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, FUNC(1), KC_TRNS, KC_TRNS, FUNC(2), LCTL(LALT(KC_LEFT)), LCTL(LALT(KC_DOWN)), LCTL(LALT(KC_UP)), LCTL(LALT(KC_RGHT))}
  },
  [6] = { /* Joe SPECIAL fn3 */
    {KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, LCTL(LSFT(CM_U)), KC_TRNS, KC_TRNS, KC_SLEP},
    {KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,          KC_TRNS, KC_TRNS, RESET  },
    {KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,          KC_TRNS, KC_TRNS, KC_TRNS},
    {FUNC(6), FUNC(7), FUNC(8), KC_TRNS, FUNC(1), KC_TRNS, KC_TRNS, FUNC(2), KC_MUTE,          KC_VOLD, KC_VOLU, KC_MPLY}
  }
};

enum macro_id {
    M_Q0,
    M_Q1,
    M_Q2,
    M_Q3
};

const uint16_t PROGMEM fn_actions[] = {
    [1] = ACTION_LAYER_MOMENTARY(4),  // fn1
    [2] = ACTION_LAYER_MOMENTARY(5),  // fn2

    /* ESC on tap, fn3 on hold */
    [3] = ACTION_LAYER_TAP_KEY(6, KC_ESC),

    /* toggle layers */
    [6]  = ACTION_DEFAULT_LAYER_SET(0), 
    [7]  = ACTION_DEFAULT_LAYER_SET(1),
    [8]  = ACTION_DEFAULT_LAYER_SET(2),

    /* tab on tap, shift on hold */
    [15] = ACTION_MODS_TAP_KEY(MOD_LSFT, KC_TAB),

    /* smiley macros */
    [25] = ACTION_MACRO(M_Q0),
    [26] = ACTION_MACRO(M_Q1),
    [27] = ACTION_MACRO(M_Q2),
    [28] = ACTION_MACRO(M_Q3),
};

const macro_t *action_get_macro(keyrecord_t *record, uint8_t id, uint8_t opt)
{
    switch (id) {
        case M_Q0:
            /* :( */
            return MACRODOWN( D(LSFT), T(CM_SCLN), T(0), U(LSFT), END );
        case M_Q1:
            /* :) */
            return MACRODOWN( D(LSFT), T(CM_SCLN), T(9), U(LSFT), END );
        case M_Q2:
            /* (: | :) */
            return MACRODOWN( D(LSFT), T(9), T(CM_SCLN), T(SPC), T(SPC), T(CM_SCLN), T(0), U(LSFT), T(LEFT), T(LEFT), T(LEFT), END );
        case M_Q3:
            /* :( | ): */
            return MACRODOWN( D(LSFT), T(CM_SCLN), T(9), T(SPC), T(SPC), T(0), T(CM_SCLN), U(LSFT), T(LEFT), T(LEFT), T(LEFT), END );
        default:
            break;
    }
    return MACRO_NONE;
}