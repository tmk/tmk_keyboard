#include "actionmap.h"
#include "action_code.h"
#include "actionmap_common.h"
#include "rgblight.h"


/*
* Actions
*/
#define AC_BLD    ACTION_BACKLIGHT_DECREASE()
#define AC_BLI    ACTION_BACKLIGHT_INCREASE()
#define AC_TL1    ACTION_LAYER_TAP_KEY(1, KC_ESC)
#define AC_L01    ACTION_FUNCTION(RGBLED_TOGGLE)
#define AC_L02    ACTION_FUNCTION(RGBLED_STEP_MODE)
#define AC_L03    ACTION_FUNCTION(RGBLED_INCREASE_HUE)
#define AC_L04    ACTION_FUNCTION(RGBLED_DECREASE_HUE)
#define AC_L05    ACTION_FUNCTION(RGBLED_INCREASE_SAT)
#define AC_L06    ACTION_FUNCTION(RGBLED_DECREASE_SAT)
#define AC_L07    ACTION_FUNCTION(RGBLED_INCREASE_VAL)
#define AC_L08    ACTION_FUNCTION(RGBLED_DECREASE_VAL)

const action_t PROGMEM actionmaps[][MATRIX_ROWS][MATRIX_COLS] = {
	[0] = ACTIONMAP( \
	TL1,    PSCR,    SLCK,   PAUS, \
	VOLU,    INS,    HOME,   PGUP, \
	VOLD,    DEL,     END,   PGDN),
	[1] = ACTIONMAP( \
	TRNS,   LSFT,   RSFT,    TRNS, \
	L01,    L02,     L03,    L04, \
	L07,    L08,     BLD,    BLI),
};