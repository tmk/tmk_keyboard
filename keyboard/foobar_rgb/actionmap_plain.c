#include "actionmap.h"
#include "action_code.h"
#include "actionmap_common.h"
#include "rgblight.h"

/*
 * Actions
 */
#define AC_BLD    ACTION_BACKLIGHT_DECREASE()
#define AC_BLI    ACTION_BACKLIGHT_INCREASE()
#define AC_TL1    ACTION_LAYER_TAP_KEY(1, KC_SPACE)
#define AC_TL2    ACTION_LAYER_TAP_KEY(2, KC_BSPACE)
#define AC_TL3    ACTION_LAYER_TAP_KEY(3, KC_C)
#define AC_TL4    ACTION_LAYER_TAP_KEY(4, KC_V)
#define AC_TL5    ACTION_LAYER_TAP_KEY(5, KC_B)
#define AC_TM1    ACTION_MODS_TAP_KEY(MOD_RSFT, KC_ENT)
#define AC_TM2    ACTION_MODS_TAP_KEY(MOD_LCTL, KC_Z)
#define AC_TM3    ACTION_MODS_TAP_KEY(MOD_LALT, KC_X)
#define AC_TM4    ACTION_MODS_TAP_KEY(MOD_RALT, KC_N)
#define AC_TM5    ACTION_MODS_TAP_KEY(MOD_RCTL, KC_M)
#define AC_S01    ACTION_MODS_KEY(MOD_LSFT, KC_1)
#define AC_S02    ACTION_MODS_KEY(MOD_LSFT, KC_2)
#define AC_S03    ACTION_MODS_KEY(MOD_LSFT, KC_3)
#define AC_S04    ACTION_MODS_KEY(MOD_LSFT, KC_4)
#define AC_S05    ACTION_MODS_KEY(MOD_LSFT, KC_5)
#define AC_S06    ACTION_MODS_KEY(MOD_LSFT, KC_6)
#define AC_S07    ACTION_MODS_KEY(MOD_LSFT, KC_7)
#define AC_S08    ACTION_MODS_KEY(MOD_LSFT, KC_8)
#define AC_S09    ACTION_MODS_KEY(MOD_LSFT, KC_9)
#define AC_S10    ACTION_MODS_KEY(MOD_LSFT, KC_0)
#define AC_S11    ACTION_MODS_KEY(MOD_LSFT, KC_MINS)
#define AC_S12    ACTION_MODS_KEY(MOD_LSFT, KC_EQL)
#define AC_S13    ACTION_MODS_KEY(MOD_LSFT, KC_LBRC)
#define AC_S14    ACTION_MODS_KEY(MOD_LSFT, KC_RBRC)
#define AC_S15    ACTION_MODS_KEY(MOD_LSFT, KC_BSLS)
#define AC_S16    ACTION_MODS_KEY(MOD_LSFT, KC_COMM)
#define AC_S17    ACTION_MODS_KEY(MOD_LSFT, KC_DOT)
#define AC_S18    ACTION_MODS_KEY(MOD_LSFT, KC_SLSH)
#define AC_S19    ACTION_MODS_KEY(MOD_LSFT, KC_SCLN)
#define AC_S20    ACTION_MODS_KEY(MOD_LSFT, KC_QUOT)
#define AC_L01    ACTION_FUNCTION(RGBLED_TOGGLE)
#define AC_L02    ACTION_FUNCTION(RGBLED_STEP_MODE)
#define AC_L03    ACTION_FUNCTION(RGBLED_INCREASE_HUE)
#define AC_L04    ACTION_FUNCTION(RGBLED_DECREASE_HUE)
#define AC_L05    ACTION_FUNCTION(RGBLED_INCREASE_SAT)
#define AC_L06    ACTION_FUNCTION(RGBLED_DECREASE_SAT)
#define AC_L07    ACTION_FUNCTION(RGBLED_INCREASE_VAL)
#define AC_L08    ACTION_FUNCTION(RGBLED_DECREASE_VAL)

const action_t PROGMEM actionmaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = ACTIONMAP( 
    Q,      W,      E,      R,      T,
    A,      S,      D,      F,      G,
    TM2,    TM3,    TL3,    TL4,    TL2,

    Y,      U,      I,      O,      P,
    H,      J,      K,      L,      ESC,
    TL1,    TL5,    TM4,    TM5,    TM1),

    [1] = ACTIONMAP( 
    1,      2,      3,      4,      5,
    F1,     F2,     F3,     F4,     F5,
    TRNS,   TRNS,   TRNS,   TRNS,   DEL,

    6,      7,      8,      9,      0,
    F6,     F7,     F8,     F9,     F10,
    TRNS,   TRNS,   TRNS,   TRNS,   TRNS),

    [2] = ACTIONMAP( 
    S01,    S02,    S03,    S04,    S05,
    F11,    F12,    TRNS,   TRNS,   TRNS,
    TRNS,   TRNS,   TRNS,   TRNS,   TRNS,

    S06,    S07,    S08,    S09,    S10,
    TRNS,   TRNS,   TRNS,   TRNS,   GRV,
    TRNS,   TRNS,   TRNS,   TRNS,   TRNS),
   
    [3] = ACTIONMAP( 
    TRNS,   TRNS,   TRNS,   TRNS,   TRNS,
    TAB,    TRNS,   TRNS,   TRNS,   TRNS,
    TRNS,   TRNS,   TRNS,   TRNS,   TRNS,

    MINS,   EQL,    LBRC,   RBRC,   BSLS,
    COMM,   DOT,    SLSH,   SCLN,   QUOT,
    TRNS,   LEFT,   DOWN,   UP,     RGHT),

    [4] = ACTIONMAP( 
    TRNS,   TRNS,   TRNS,   TRNS,   TRNS,
    TAB,    TRNS,   TRNS,   TRNS,   TRNS,
    TRNS,   TRNS,   TRNS,   TRNS,   TRNS,

    S11,    S12,    S13,    S14,    S15,
    S16,    S17,    S18,    S19,    S20,
    TRNS,   HOME,   PGDN,   PGUP,   END),

    [5] = ACTIONMAP( 
    CALC,   WHOM,   MAIL,   MYCM,   TRNS,
    L01,    L02,    L03,    L04,    L05,
    TRNS,   TRNS,   TRNS,   TRNS,   BTLD,

    TRNS,   TRNS,   TRNS,   TRNS,   PSCR,
	L06,    L07,    L08,    BLD,    BLI,
    TRNS,   TRNS,   TRNS,   TRNS,   TRNS),
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