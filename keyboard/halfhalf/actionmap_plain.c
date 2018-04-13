#include "actionmap.h"
#include "action_code.h"
#include "actionmap_common.h"


/*
 * Actions
 */

const action_t PROGMEM actionmaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = ACTIONMAP( 
    ESC,    Q,      W,      E,      R,       T,   LCTL,
    TAB,    A,      S,      D,      F,       G,   LALT,
    LSFT,   Z,      X,      C,      V,       B,   LGUI,
                                  SPC,

    RCTL,   Y,      U,      I,      O,       P,   BSPC,
    RALT,   H,      J,      K,      L,    SCLN,   QUOT,
    APP,    N,      M,   COMM,    DOT,    SLSH,    ENT,
                 SPC),
   
};

