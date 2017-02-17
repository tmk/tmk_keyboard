#include "actionmap.h"
#include "action_code.h"
#include "actionmap_common.h"


/*
* Actions
*/
#define AC_BLD    ACTION_BACKLIGHT_DECREASE()
#define AC_BLI    ACTION_BACKLIGHT_INCREASE()
#define AC_TL1    ACTION_LAYER_TAP_KEY(1, KC_ESC)

const action_t PROGMEM actionmaps[][MATRIX_ROWS][MATRIX_COLS] = {
	[0] = ACTIONMAP( \
	TL1,    PSCR,    SLCK,   PAUS, \
	VOLU,    INS,    HOME,   PGUP, \
	VOLD,    DEL,     END,   PGDN),
	[1] = ACTIONMAP( \
	TRNS,   LSFT,   RSFT,    TRNS, \
	TRNS,   TRNS,   TRNS,    TRNS, \
	TRNS,   TRNS,    BLD,    BLI),
};
