#include "actionmap.h"
#include "action_code.h"
#include "actionmap_common.h"


/*
 * Actions
 */

const action_t PROGMEM actionmaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = ACTIONMAP( \
	1,      2,      3,      4,      5,      6,      7,      8,      9,      0, \
	Q,      W,      E,      R,      T,      U,      I,      O,      P,      LBRC,\
	A,      S,      D,      F,      G,      J,      K,      L,      SCLN,   QUOT,\
							C,      V,      N,      M ),

};
