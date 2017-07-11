#include "action.h"
#include "actionmap.h"
#include "hook.h"
#include "fc980c.h"


#ifdef KEYMAP_SECTION_ENABLE
const action_t actionmaps[][UNIMAP_ROWS][UNIMAP_COLS] __attribute__ ((section (".keymap.keymaps"))) = {
#else
const action_t actionmaps[][UNIMAP_ROWS][UNIMAP_COLS] PROGMEM = {
#endif
    [0] = KMAP(
    ESC,      F1,  F2,  F3,  F4,  F5,  F6,  F7,  F8,  F9,  F10, F11, F12,                         DEL, INS, PGUP,PGDN,
    GRV, 1,   2,   3,   4,   5,   6,   7,   8,   9,   0,   MINS,EQL, BSPC,                        NLCK,PSLS,PAST,PMNS,
    TAB, Q,   W,   E,   R,   T,   Y,   U,   I,   O,   P,   LBRC,RBRC,BSLS,                        P7,  P8,  P9,  PPLS,
    CAPS,A,   S,   D,   F,   G,   H,   J,   K,   L,   SCLN,QUOT,     ENT,                         P4,  P5,  P6,       
    LSFT,     Z,   X,   C,   V,   B,   N,   M,   COMM,DOT, SLSH,     RSFT,          UP,           P1,  P2,  P3,  PENT,
    LCTL,LGUI,LALT,               SPC,                     RALT,RCTL,RGUI,     LEFT,DOWN,RGHT,         P0,  PDOT     
    ),
};
