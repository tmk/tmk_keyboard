#include "keymap_common.h"
#include "protocol/serial.h"

const uint8_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

/* Qwerty
 * ,-----------------------------------------------------------------------------------.
 * | Esc  |   Q  |   W  |   E  |   R  |   T  |   Y  |   U  |   I  |   O  |   P  | Bksp |
 * |------+------+------+------+------+-------------+------+------+------+------+------|
 * | Tab  |   A  |   S  |   D  |   F  |   G  |   H  |   J  |   K  |   L  |   ;  |  "   |
 * |------+------+------+------+------+------|------+------+------+------+------+------|
 * | Shift|   Z  |   X  |   C  |   V  |   B  |   N  |   M  |   ,  |   .  |   /  |Enter |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | CTRL | ALT  | GUI  | APP  |Lower |    Space    |Raise | Left | Down |  Up  |Right |
 * `-----------------------------------------------------------------------------------'
 */

KEYMAP(
  KC_ESC,   KC_Q,    KC_W,    KC_E,    KC_R,     KC_T,    KC_Y,     KC_U,      KC_I,      KC_O,       KC_P,       KC_BSPC,
  KC_FN2,   KC_A,    KC_S,    KC_D,    KC_F,     KC_G,    KC_H,     KC_J,      KC_K,      KC_L,       KC_SCLN,    KC_QUOT,
  KC_LSFT,  KC_Z,    KC_X,    KC_C,    KC_V,     KC_B,    KC_N,     KC_M,      KC_COMM,   KC_DOT,     KC_SLSH,    KC_SLSH,
  KC_LCTL,  KC_LALT, KC_LGUI, KC_APP,  KC_FN1,   KC_SPC,            KC_FN0,    KC_LEFT,   KC_DOWN,    KC_UP,      KC_RGHT
),

KEYMAP(
  KC_GRV,   KC_1,    KC_2,    KC_3,    KC_4,     KC_5,    KC_6,     KC_7,      KC_8,      KC_9,       KC_0,       KC_DELETE,
  KC_TRNS,  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,  KC_TRNS, KC_TRNS,  KC_MINS,   KC_EQL,    KC_LBRC,    KC_RBRC,    KC_BSLS,
  KC_TRNS,  KC_F11,  KC_F12,  KC_F13,  KC_F14,   KC_F15,  KC_F16,   KC_F17,    KC_F18,    KC_F19,     KC_F20,     KC_TRNS,
  KC_TRNS,  KC_TRNS, KC_TRNS, KC_SLSH, KC_TRNS,  KC_TRNS,           KC_TRNS,   KC_HOME,   KC_PGDN,    KC_PGUP,    KC_END
),

KEYMAP(
  KC_FN5,   KC_FN6,  KC_FN7,  KC_FN8,  KC_FN9,   KC_FN10, KC_FN11,  KC_FN12,   KC_FN13,   KC_FN14,    KC_FN15,    KC_DELETE,
  KC_TRNS,  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,  KC_TRNS, KC_TRNS,  KC_MINS,   KC_EQL,    KC_LBRC,    KC_RBRC,    KC_BSLS,
  KC_TRNS,  KC_F11,  KC_F12,  KC_F13,  KC_F14,   KC_F15,  KC_F16,   KC_F17,    KC_F18,    KC_F19,     KC_F20,     KC_TRNS,
  KC_TRNS,  KC_TRNS, KC_TRNS, KC_SLSH, KC_TRNS,  KC_TRNS,           KC_TRNS,   KC_HOME,   KC_PGDN,    KC_PGUP,    KC_END
),

KEYMAP( /* Tab */
  KC_ESC,   KC_CALC, KC_WHOM, KC_MAIL, KC_MYCM,  KC_TRNS, KC_TRNS,  KC_TRNS,   KC_TRNS,   KC_TRNS,    KC_PSCR,    KC_TRNS,
  KC_TRNS,  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,  KC_TRNS, KC_TRNS,  KC_TRNS,   KC_TRNS,   KC_TRNS,    KC_TRNS,    KC_TRNS,
  KC_TRNS,  KC_FN3,  KC_FN4,  KC_TRNS, KC_TRNS,  KC_TRNS, KC_TRNS,  KC_TRNS,   KC_TRNS,   KC_TRNS,    KC_TRNS,    KC_TRNS,
  KC_TRNS,  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,  KC_TRNS,           KC_TRNS,   KC_MS_L,   KC_MS_D,    KC_MS_U,    KC_MS_R
),
};

enum function_id {
   GNAPLED_TOGGLE,
   GNAPLED_STEP_MODE,
};

const action_t PROGMEM fn_actions[] = {
    [0]  = ACTION_LAYER_MOMENTARY(1),
    [1]  = ACTION_LAYER_MOMENTARY(2),
    [2]  = ACTION_LAYER_TAP_KEY(3, KC_TAB),
    [3]  = ACTION_FUNCTION(GNAPLED_TOGGLE),
    [4]  = ACTION_FUNCTION(GNAPLED_STEP_MODE),
    [5]  = ACTION_MODS_KEY(MOD_LSFT, KC_GRV),
    [6]  = ACTION_MODS_KEY(MOD_LSFT, KC_1),
    [7]  = ACTION_MODS_KEY(MOD_LSFT, KC_2),
    [8]  = ACTION_MODS_KEY(MOD_LSFT, KC_3),
    [9]  = ACTION_MODS_KEY(MOD_LSFT, KC_4),
    [10] = ACTION_MODS_KEY(MOD_LSFT, KC_5),
    [11] = ACTION_MODS_KEY(MOD_LSFT, KC_6),
    [12] = ACTION_MODS_KEY(MOD_LSFT, KC_7),
    [13] = ACTION_MODS_KEY(MOD_LSFT, KC_8),
    [14] = ACTION_MODS_KEY(MOD_LSFT, KC_9),
    [15] = ACTION_MODS_KEY(MOD_LSFT, KC_0),
};

void action_function(keyrecord_t *record, uint8_t id, uint8_t opt) {
  switch (id) {
    case GNAPLED_TOGGLE:
      if (record->event.pressed) {
        gnaplight_toggle();
      }
      break;
    case GNAPLED_STEP_MODE:
      if (record->event.pressed) {
        gnaplight_step();
      }
      break;
  }
}

//GNAP keymap functions
void gnaplight_step(void) {
    serial_send(101);
}

void gnaplight_toggle(void) {
    serial_send(100);
}
