#include "keymap_common.h"

const uint8_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

KEYMAP(
  FN0,   VOLD,  MUTE,  VOLU,
  MPRV,  MPLY,  MSTP,  MNXT
),

KEYMAP(
  TRNS,  F2,    F3,    F4,
  F5,    F6,    F7,    F8
),
};

enum function_id {
    RGBLED_TOGGLE,
    RGBLED_STEP_MODE,
    RGBLED_INCREASE_HUE,
    RGBLED_DECREASE_HUE,
    RGBLED_INCREASE_SAT,
    RGBLED_DECREASE_SAT,
    RGBLED_INCREASE_VAL,
    RGBLED_DECREASE_VAL,
};

const action_t PROGMEM fn_actions[] = {
    [0] = ACTION_LAYER_TAP_KEY(1, KC_ESC),
    [1] = ACTION_FUNCTION(RGBLED_TOGGLE),
    [2] = ACTION_FUNCTION(RGBLED_STEP_MODE),
    [3] = ACTION_FUNCTION(RGBLED_INCREASE_HUE),
    [4] = ACTION_FUNCTION(RGBLED_DECREASE_HUE),
    [5] = ACTION_FUNCTION(RGBLED_INCREASE_SAT),
    [6] = ACTION_FUNCTION(RGBLED_DECREASE_SAT),
    [7] = ACTION_FUNCTION(RGBLED_INCREASE_VAL),
    [8] = ACTION_FUNCTION(RGBLED_DECREASE_VAL),
};

void action_function(keyrecord_t *record, uint8_t id, uint8_t opt) {
  switch (id) {
    case RGBLED_TOGGLE:
        if (record->event.pressed) {
        rgblight_toggle();
        }
        break;
    case RGBLED_INCREASE_HUE:
        if (record->event.pressed) {
        rgblight_increase_hue();
        }
        break;
    case RGBLED_DECREASE_HUE:
        if (record->event.pressed) {
          rgblight_decrease_hue();
        }
        break;
    case RGBLED_INCREASE_SAT:
        if (record->event.pressed) {
        rgblight_increase_sat();
        }
        break;
    case RGBLED_DECREASE_SAT:
        if (record->event.pressed) {
          rgblight_decrease_sat();
        }
        break;
    case RGBLED_INCREASE_VAL:
        if (record->event.pressed) {
          rgblight_increase_val();
        }
        break;
    case RGBLED_DECREASE_VAL:
        if (record->event.pressed) {
          rgblight_decrease_val();
        }
        break;
    case RGBLED_STEP_MODE:
        if (record->event.pressed) {
          rgblight_step();
        }
        break;
  }
}


