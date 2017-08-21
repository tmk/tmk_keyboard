#include "unimap.h"
#include "action.h"
#include "actionmap.h"
#include "hook.h"


#define UMAP( \
    K29,K1E,K1F,K20,K21,K22,K23,K24,K25,K26,K27,K2D,K2E,K2A,    K49,K4C,    K3A, \
    K2B,K14,K1A,K08,K15,K17,K1C,K18,K0C,K12,K13,K35,K2F,K30,                K3B, \
    K78,K04,K16,K07,K09,K0A,K0B,K0D,K0E,K0F,K33,K34,    K28,      K52,      K3C, \
    K79,K1D,K1B,K06,K19,K05,K11,K10,K36,K37,K38,                K50,K4F,    K3D, \
                            K2C,                                  K51,      K4A  \
) UNIMAP ( \
            NO, NO, NO, NO, NO, NO, NO, NO, NO, NO, NO, NO,                                     \
    K29,    K3A,K3B,K3C,K3D,NO, NO, NO, NO, NO, NO, NO, NO,       NO, NO, NO,       NO, NO, NO, \
    K35,K1E,K1F,K20,K21,K22,K23,K24,K25,K26,K27,K2D,K2E,NO, K2A,  K49,K4A,NO,   NO, NO, NO, NO, \
    K2B,K14,K1A,K08,K15,K17,K1C,K18,K0C,K12,K13,K2F,K30,    NO,   K4C,NO, NO,   NO, NO, NO, NO, \
    NO, K04,K16,K07,K09,K0A,K0B,K0D,K0E,K0F,K33,K34,    NO, K28,                NO, NO, NO, NO, \
    K79,NO, K1D,K1B,K06,K19,K05,K11,K10,K36,K37,K38,    NO, NO,       K52,      NO, NO, NO, NO, \
    K78,NO, NO, NO,         K2C,        NO, NO, NO, NO, NO, NO,   K50,K51,K4F,  NO,     NO, NO  \
)

#define AC_L1       ACTION_LAYER_MOMENTARY(1)
#define AC_SPC1     ACTION_LAYER_TAP_KEY(1, KC_SPC)
#define AC_AENT     ACTION_MODS_TAP_KEY(MOD_LALT, KC_ENT)

#ifdef KEYMAP_SECTION_ENABLE
const action_t actionmaps[][UNIMAP_ROWS][UNIMAP_COLS] __attribute__ ((section (".keymap.keymaps"))) = {
#else
const action_t actionmaps[][UNIMAP_ROWS][UNIMAP_COLS] PROGMEM = {
#endif
    [0] = UMAP(
    ESC, 1,   2,   3,   4,   5,   6,   7,   8,   9,   0,   MINS,EQL, BSPC,     INS, DEL,    F1,
    TAB, Q,   W,   E,   R,   T,   Y,   U,   I,   O,   P,   LBRC,RBRC,GRV,                   F2,
    LCTL,A,   S,   D,   F,   G,   H,   J,   K,   L,   SCLN,QUOT,     AENT,        UP,       F3,
    LSFT,Z,   X,   C,   V,   B,   N,   M,   COMM,DOT, SLSH,                    LEFT,RGHT,   F4,
                             SPC1,                                                DOWN,     HOME
    ),
    [1] = UMAP(
    GRV, F1,  F2,  F3,  F4,  F5,  F6,  F7,  F8,  F9,  F10, F11, F12, BSLS,     PSCR,PAUS,   F5,
    TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,PGDN,UP,  PGUP,TRNS,BSLS,TRNS,TRNS,                  F6,
    TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,HOME,LEFT,DOWN,RGHT,END, TRNS,     LGUI,        PGUP,     F7,
    TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,                    HOME,END,    F8,
                             TRNS,                                                PGDN,     END
    ),
/*
    [2] = UMAP(
    TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,     TRNS,TRNS,   TRNS,
    TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,                  TRNS,
    TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,     TRNS,        TRNS,     TRNS,
    TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,                    TRNS,TRNS,   TRNS,
                             TRNS,                                                TRNS,     TRNS 
    ),
*/
};


const uint8_t PROGMEM unimap_trans[MATRIX_ROWS][MATRIX_COLS] = {
    { UNIMAP_ESC,     UNIMAP_1,       UNIMAP_2,       UNIMAP_3,       UNIMAP_4,       UNIMAP_5,       UNIMAP_6,       UNIMAP_7      },
    { UNIMAP_8,       UNIMAP_9,       UNIMAP_0,       UNIMAP_MINS,    UNIMAP_EQL,     UNIMAP_BSPC,    UNIMAP_INS,     UNIMAP_DEL    },
    { UNIMAP_TAB,     UNIMAP_Q,       UNIMAP_W,       UNIMAP_E,       UNIMAP_R,       UNIMAP_T,       UNIMAP_Y,       UNIMAP_U      },
    { UNIMAP_I,       UNIMAP_O,       UNIMAP_P,       UNIMAP_GRV,     UNIMAP_LBRC,    UNIMAP_RBRC,    UNIMAP_F2,      UNIMAP_F1     },
    { UNIMAP_LCTL,    UNIMAP_A,       UNIMAP_S,       UNIMAP_D,       UNIMAP_F,       UNIMAP_G,       UNIMAP_H,       UNIMAP_J      },
    { UNIMAP_K,       UNIMAP_L,       UNIMAP_SCLN,    UNIMAP_QUOT,    UNIMAP_ENT,     UNIMAP_UP,      UNIMAP_F3,      UNIMAP_F4     },
    { UNIMAP_LSFT,    UNIMAP_Z,       UNIMAP_X,       UNIMAP_C,       UNIMAP_V,       UNIMAP_B,       UNIMAP_N,       UNIMAP_M      },
    { UNIMAP_SPC,     UNIMAP_COMM,    UNIMAP_DOT,     UNIMAP_SLSH,    UNIMAP_LEFT,    UNIMAP_RIGHT,   UNIMAP_DOWN,    UNIMAP_HOME   }
};

