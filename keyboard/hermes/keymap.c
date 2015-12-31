#include "keymap_common.h"
#include "action_layer.h"
#include "backlight.h"

#define KC_DVQW KC_FN0
#define KC_LCBR KC_FN1
#define KC_RCBR KC_FN2
#define KC_DVFN KC_FN3
#define KC_QWFN KC_FN4
#define KC_PIPE KC_FN5
#define KC_LEDB KC_FN6
#define KC_MOUS KC_FN7

const uint8_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

    // default dvorak
    KEYMAP(ESC, 1,   2,   3,   4,   5,   DVQW,LEDB,6,   7,   8,   9,   0,   EQL,  \
           TAB, QUOT,COMM,DOT, P,   Y,   DEL, INS, F,   G,   C,   R,   L,   SLSH, \
           GRV, A,   O,   E,   U,   I,   HOME,PGUP,D,   H,   T,   N,   S,   MINS, \
           LSFT,SCLN,Q,   J,   K,   X,   END, PGDN,B,   M,   W,   V,   Z,   RSFT, \
           LCTL,LGUI,LALT,DVFN,     BSPC,ENT,      SPC,      MOUS,RALT,RGUI,RCTL  ),

    // dvorak fn
    KEYMAP(BTLD,F1,  F2,  F3,  F4,  F5,  NO,  NO,  F6,  F7,  F8,  F9,  F10, NO,   \
           NO,  NO,  NO,  UP,  NO,  NO,  NO,  MUTE,NO,  NO,  EQL, LBRC,RBRC,BSLS, \
           NO,  NO,  LEFT,DOWN,RGHT,NO,  NO,  VOLU,NO,  NO,  NO,  LCBR,RCBR,PIPE, \
           TRNS,NO,  NO,  NO,  NO,  NO,  NO,  VOLD,NO,  NO,  NO,  NO,  NO,  TRNS, \
           TRNS,TRNS,TRNS,DVFN,     TRNS,TRNS,     TRNS,     NO,  TRNS,TRNS,TRNS  ),

    // qwerty
    KEYMAP(ESC, 1,   2,   3,   4,   5,   DVQW,TRNS,6,   7,   8,   9,   0,   MINS, \
           TAB, Q,   W,   E,   R,   T,   DEL, INS, Y,   U,   I,   O,   P,   EQL,  \
           GRV, A,   S,   D,   F,   G,   HOME,PGUP,H,   J,   K,   L,   SCLN,QUOT, \
           LSFT,Z,   X,   C,   V,   B,   END, PGDN,N,   M,   COMM,DOT, SLSH,RSFT, \
           TRNS,TRNS,TRNS,QWFN,     TRNS,TRNS,     TRNS,     MOUS,TRNS,TRNS,TRNS  ),

    // qwerty fn
    KEYMAP(BTLD,F1,  F2,  F3,  F4,  F5,  NO,  NO,  F6,  F7,  F8,  F9,  F10, NO,   \
           NO,  NO,  NO,  UP,  NO,  NO,  NO,  MUTE,NO,  NO,  EQL, LBRC,RBRC,BSLS, \
           NO,  NO,  LEFT,DOWN,RGHT,NO,  NO,  VOLU,NO,  NO,  NO,  LCBR,RCBR,PIPE, \
           TRNS,NO,  NO,  NO,  NO,  NO,  NO,  VOLD,NO,  NO,  NO,  NO,  NO,  TRNS, \
           TRNS,TRNS,TRNS,QWFN,     TRNS,TRNS,     TRNS,     NO,  TRNS,TRNS,TRNS  ),

    // mouse
    KEYMAP(ESC, 1,   2,   3,   4,   5,   NO,  TRNS,NO,  NO,  NO,  NO,  NO,  NO,   \
           TAB, Q,   W,   E,   R,   T,   NO,  NO,  NO,  BTN3,MS_U,NO,  WH_U,NO,   \
           GRV, A,   S,   D,   F,   G,   NO,  NO,  NO,  MS_L,MS_D,MS_R,WH_D,NO,   \
           LSFT,Z,   X,   C,   V,   B,   NO,  NO,  NO,  NO,  NO,  NO,  NO,  NO,   \
           TRNS,TRNS,TRNS,NO,       NO,  BTN2,     BTN1,     MOUS,NO,  NO,  NO    ),

    // numpad
    KEYMAP(NO,  NO,  NO,  NO,  NO,  NO,  NO,  TRNS,NO,  NLCK,PMNS,PAST,PSLS,NO,   \
           NO,  NO,  NO,  NO,  NO,  NO,  NO,  NO,  NO,  P7,  P8,  P9,  PPLS,NO,   \
           NO,  NO,  NO,  NO,  NO,  NO,  NO,  NO,  NO,  P4,  P5,  P6,  PPLS,NO,   \
           NO,  NO,  NO,  NO,  NO,  NO,  NO,  NO,  NO,  P1,  P2,  P3,  PENT,NO,   \
           TRNS,TRNS,TRNS,NO,       TRNS,TRNS,     TRNS,     P0,  PDOT,PENT,NO    )
};

enum function_action {
    BACKLIGHT_BRIGHTNESS
};

void action_function(keyrecord_t* record, uint8_t id, uint8_t opt) {
    switch (id) {
        case BACKLIGHT_BRIGHTNESS:
            if (record->event.pressed)
                backlight_adjust();
            break;
    }
}

const uint16_t PROGMEM fn_actions[] = {
    ACTION_LAYER_TOGGLE(2),                // 0 toggle qwerty
    ACTION_MODS_KEY(MOD_LSFT, KC_LBRC),    // 1 left curly brace '{'
    ACTION_MODS_KEY(MOD_LSFT, KC_RBRC),    // 2 right curly brace '}'
    ACTION_LAYER_MOMENTARY(1),             // 3 fn layer for dvorak
    ACTION_LAYER_MOMENTARY(3),             // 4 fn layer for qwerty
    ACTION_MODS_KEY(MOD_LSFT, KC_BSLS),    // 5 pipe '|'
    ACTION_FUNCTION(BACKLIGHT_BRIGHTNESS), // 6 light brightness
    ACTION_LAYER_TOGGLE(4)                 // 7 mouse
};