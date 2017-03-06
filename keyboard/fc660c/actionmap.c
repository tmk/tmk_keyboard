#include "action.h"
#include "actionmap.h"
#include "hook.h"


#define AMAP( \
    K13, K10, K11, K12, K14, K16, K17, K15, K1B, K18, K19, K1A, K1C, K1E,      K1F, \
    K03, K00, K01, K02, K04, K06, K07, K05, K0B, K08, K09, K0A, K0C, K0E,      K0F, \
    K43, K40, K41, K42, K44, K46, K47, K45, K4B, K48, K49, K4A,      K4E,           \
    K33, K31, K32, K34, K36, K37, K35, K3B, K38, K39, K3A,           K3C, K3D,      \
    K23, K21, K22,                K27,      K28, K2A, K2C,           K2E, K2D, K2F  \
) { \
    { AC_##K00, AC_##K01, AC_##K02, AC_##K03, AC_##K04, AC_##K05, AC_##K06, AC_##K07,   \
      AC_##K08, AC_##K09, AC_##K0A, AC_##K0B, AC_##K0C, AC_NO,    AC_##K0E, AC_##K0F }, \
    { AC_##K10, AC_##K11, AC_##K12, AC_##K13, AC_##K14, AC_##K15, AC_##K16, AC_##K17,   \
      AC_##K18, AC_##K19, AC_##K1A, AC_##K1B, AC_##K1C, AC_NO,    AC_##K1E, AC_##K1F }, \
    { AC_NO,    AC_##K21, AC_##K22, AC_##K23, AC_NO,    AC_NO,    AC_NO,    AC_##K27,   \
      AC_##K28, AC_NO,    AC_##K2A, AC_NO,    AC_##K2C, AC_##K2D, AC_##K2E, AC_##K2F }, \
    { AC_NO,    AC_##K31, AC_##K32, AC_##K33, AC_##K34, AC_##K35, AC_##K36, AC_##K37,   \
      AC_##K38, AC_##K39, AC_##K3A, AC_##K3B, AC_##K3C, AC_##K3D, AC_NO,    AC_NO    }, \
    { AC_##K40, AC_##K41, AC_##K42, AC_##K43, AC_##K44, AC_##K45, AC_##K46, AC_##K47,   \
      AC_##K48, AC_##K49, AC_##K4A, AC_##K4B, AC_NO,    AC_NO,    AC_##K4E, AC_NO    }, \
    { AC_NO,    AC_NO,    AC_NO,    AC_NO,    AC_NO,    AC_NO,    AC_NO,    AC_NO,      \
      AC_NO,    AC_NO,    AC_NO,    AC_NO,    AC_NO,    AC_NO,    AC_NO,    AC_NO    }, \
    { AC_NO,    AC_NO,    AC_NO,    AC_NO,    AC_NO,    AC_NO,    AC_NO,    AC_NO,      \
      AC_NO,    AC_NO,    AC_NO,    AC_NO,    AC_NO,    AC_NO,    AC_NO,    AC_NO    }, \
    { AC_NO,    AC_NO,    AC_NO,    AC_NO,    AC_NO,    AC_NO,    AC_NO,    AC_NO,      \
      AC_NO,    AC_NO,    AC_NO,    AC_NO,    AC_NO,    AC_NO,    AC_NO,    AC_NO    }  \
}


#define AC_L3       ACTION_LAYER_MOMENTARY(3)
#define AC_L4       ACTION_LAYER_MOMENTARY(4)
#define AC_LS_2     ACTION_LAYER_MODS(2, MOD_LSFT)
#define AC_RS_2     ACTION_LAYER_MODS(2, MOD_RSFT)
#define AC_TGL1     ACTION_LAYER_TOGGLE(1)

// emulates FC660C default keymap
const action_t actionmaps[][MATRIX_ROWS][MATRIX_COLS] PROGMEM = {
    [0] = AMAP( \
        ESC, 1,   2,   3,   4,   5,   6,   7,   8,   9,   0,   MINS,EQL, BSPC,     INS,  \
        TAB, Q,   W,   E,   R,   T,   Y,   U,   I,   O,   P,   LBRC,RBRC,BSLS,     DEL,  \
        LCTL,A,   S,   D,   F,   G,   H,   J,   K,   L,   SCLN,QUOT,     ENT,            \
        LS_2,Z,   X,   C,   V,   B,   N,   M,   COMM,DOT, SLSH,          RS_2,UP,        \
        LCTL,LGUI,LALT,          SPC,           RALT,RCTL,L3,            LEFT,DOWN,RGHT  \
    ),
    [1] = AMAP( \
        GRV, 1,   2,   3,   4,   5,   6,   7,   8,   9,   0,   MINS,EQL, BSPC,     INS,  \
        TAB, Q,   W,   E,   R,   T,   Y,   U,   I,   O,   P,   LBRC,RBRC,BSLS,     DEL,  \
        LCTL,A,   S,   D,   F,   G,   H,   J,   K,   L,   SCLN,QUOT,     ENT,            \
        LSFT,Z,   X,   C,   V,   B,   N,   M,   COMM,DOT, SLSH,          RSFT,UP,        \
        LCTL,LGUI,LALT,          SPC,           RALT,RCTL,L4,            LEFT,DOWN,RGHT  \
    ),
    [2] = AMAP( \
        GRV, TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,     TRNS, \
        TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,     TRNS, \
        TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,     TRNS,           \
        TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,          TRNS,TRNS,      \
        TRNS,TRNS,TRNS,          TRNS,          TRNS,TRNS,TRNS,          TRNS,TRNS,TRNS  \
    ),
    [3] = AMAP( \
        GRV, F1,  F2,  F3,  F4,  F5,  F6,  F7,  F8,  F9,  F10, F11, F12, TRNS,     TRNS, \
        CAPS,TGL1,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,PSCR,SLCK,PAUS,TRNS,TRNS,TRNS,     TRNS, \
        TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,HOME,PGUP,TRNS,TRNS,     TRNS,           \
        TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,END, PGDN,TRNS,          TRNS,PGUP,      \
        TRNS,TRNS,TRNS,          TRNS,          TRNS,APP, L3,            HOME,PGDN,END   \
    ),
    [4] = AMAP( \
        ESC, F1,  F2,  F3,  F4,  F5,  F6,  F7,  F8,  F9,  F10, F11, F12, TRNS,     TRNS, \
        CAPS,TGL1,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,PSCR,SLCK,PAUS,TRNS,TRNS,TRNS,     TRNS, \
        TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,HOME,PGUP,TRNS,TRNS,     TRNS,           \
        TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,END, PGDN,TRNS,          TRNS,PGUP,      \
        TRNS,TRNS,TRNS,          TRNS,          TRNS,APP, L4,            HOME,PGDN,END   \
    ),
};


void hook_layer_change(uint32_t layer_state)
{
    // lights LED on Insert when layer 1 is enabled
    if (layer_state & (1L<<1)) {
        PORTB &= ~(1<<7);
    } else {
        PORTB |=  (1<<7);
    }
} 
