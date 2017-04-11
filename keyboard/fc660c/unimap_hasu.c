#include "action.h"
#include "actionmap.h"
#include "hook.h"
#include "fc660c.h"


#define AC_L3       ACTION_LAYER_MOMENTARY(3)
#define AC_L4       ACTION_LAYER_MOMENTARY(4)
#define AC_LS_2     ACTION_LAYER_MODS(2, MOD_LSFT)
#define AC_RS_2     ACTION_LAYER_MODS(2, MOD_RSFT)
#define AC_TGL1     ACTION_LAYER_TOGGLE(1)
#define AC_SPC5     ACTION_LAYER_TAP_KEY(5, KC_SPC)
#define AC_SLS6     ACTION_LAYER_TAP_KEY(6, KC_SLSH)

#define AC_BACK     ACTION_MODS_KEY(MOD_LALT, KC_LEFT)
#define AC_FRWD     ACTION_MODS_KEY(MOD_LALT, KC_RIGHT)

// emulates FC660C default keymap
#ifdef KEYMAP_SECTION_ENABLE
const action_t actionmaps[][UNIMAP_ROWS][UNIMAP_COLS] __attribute__ ((section (".keymap.keymaps"))) = {
#else
const action_t actionmaps[][UNIMAP_ROWS][UNIMAP_COLS] PROGMEM = {
#endif
    [0] = KMAP( \
        ESC, 1,   2,   3,   4,   5,   6,   7,   8,   9,   0,   MINS,EQL, BSLS,     INS,
        TAB, Q,   W,   E,   R,   T,   Y,   U,   I,   O,   P,   LBRC,RBRC,BSPC,     DEL,
        LCTL,A,   S,   D,   F,   G,   H,   J,   K,   L,   SCLN,QUOT,     ENT,
        LS_2,Z,   X,   C,   V,   B,   N,   M,   COMM,DOT, SLS6,          RS_2,UP,
        L3,  LGUI,LALT,          SPC5,                    RALT,RCTL,L3,  LEFT,DOWN,RGHT
    ),
    [1] = KMAP(
        GRV, 1,   2,   3,   4,   5,   6,   7,   8,   9,   0,   MINS,EQL, BSLS,     INS,
        TAB, Q,   W,   E,   R,   T,   Y,   U,   I,   O,   P,   LBRC,RBRC,BSPC,     DEL,
        LCTL,A,   S,   D,   F,   G,   H,   J,   K,   L,   SCLN,QUOT,     ENT,
        LSFT,Z,   X,   C,   V,   B,   N,   M,   COMM,DOT, SLSH,          RSFT,UP,
        L4,  LGUI,LALT,          SPC5,                    RALT,RCTL,L4,  LEFT,DOWN,RGHT
    ),
    [2] = KMAP(
        GRV, TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,     TRNS,
        TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,     TRNS,
        TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,     TRNS,
        TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,          TRNS,TRNS,
        TRNS,TRNS,TRNS,          TRNS,                    TRNS,TRNS,TRNS,TRNS,TRNS,TRNS
    ),
    [3] = KMAP(
        GRV, F1,  F2,  F3,  F4,  F5,  F6,  F7,  F8,  F9,  F10, F11, F12, TRNS,     TRNS,
        CAPS,TGL1,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,PSCR,SLCK,PAUS,TRNS,TRNS,TRNS,     TRNS,
        TRNS,VOLD,VOLU,MUTE,TRNS,TRNS,TRNS,TRNS,HOME,PGUP,TRNS,TRNS,     TRNS,
        TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,END, PGDN,TRNS,          TRNS,PGUP,
        L3,  TRNS,TRNS,          TRNS,                    TRNS,APP, L3,  HOME,PGDN,END
    ),
    [4] = KMAP(
        ESC, F1,  F2,  F3,  F4,  F5,  F6,  F7,  F8,  F9,  F10, F11, F12, TRNS,     TRNS,
        CAPS,TGL1,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,PSCR,SLCK,PAUS,TRNS,TRNS,TRNS,     TRNS,
        TRNS,VOLD,VOLU,MUTE,TRNS,TRNS,TRNS,TRNS,HOME,PGUP,TRNS,TRNS,     TRNS,
        TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,END, PGDN,TRNS,          TRNS,PGUP,
        L4,  TRNS,TRNS,          TRNS,                    TRNS,APP, L4,  HOME,PGDN,END
    ),
    [5] = KMAP(
        GRV, F1,  F2,  F3,  F4,  F5,  F6,  F7,  F8,  F9,  F10, F11, F12, GRV,      TRNS,
        TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,WH_L,WH_D,MS_U,WH_U,WH_R,BTN4,BTN5,TRNS,     TRNS,
        TRNS,VOLD,VOLU,MUTE,TRNS,TRNS,TRNS,MS_L,MS_D,MS_R,BTN1,TRNS,     TRNS,
        TRNS,TRNS,TRNS,TRNS,TRNS,BTN3,BTN2,BTN1,BACK,FRWD,TRNS,          TRNS,PGUP,
        TRNS,TRNS,TRNS,          TRNS,                    TRNS,TRNS,TRNS,HOME,PGDN,END
    ),
    [6] = KMAP(
        GRV, F1,  F2,  F3,  F4,  F5,  F6,  F7,  F8,  F9,  F10, F11, F12, TRNS,     TRNS,
        TRNS,HOME,PGDN,UP,  PGUP,END, HOME,PGDN,PGUP,END, TRNS,TRNS,TRNS,TRNS,     TRNS,
        TRNS,TRNS,LEFT,DOWN,RGHT,TRNS,LEFT,DOWN,UP,  RGHT,TRNS,TRNS,     TRNS,
        TRNS,     TRNS,TRNS,TRNS,TRNS,TRNS,HOME,PGDN,PGUP,END, TRNS,     TRNS,PGUP,
        TRNS,TRNS,TRNS,          TRNS,                    TRNS,TRNS,TRNS,HOME,PGDN,END
    ),
};


void hook_layer_change(uint32_t layer_state)
{
    // lights LED on Insert when layer 1 is enabled
    if (layer_state & (1L<<1)) {
        PORTB &= ~(1<<5);
    } else {
        PORTB |=  (1<<5);
    }
}
