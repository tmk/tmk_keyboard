
static const uint8_t PROGMEM keymaps[][MATRIX_COLS][MATRIX_ROWS] = {
    /* Layer 0: Standard ANSI layer */
    KEYMAP(\
         ESC, F1,  F2,  F3,  F4,  F5,  F6,  F7,  F8,  F9,  F10, F11,  F12,        PSCR,SLCK,PAUS, \
         GRV, 1,   2,   3,   4,   5,   6,   7,   8,   9,   0,   MINS, EQL,BSPC,   INS, HOME,PGUP, \
         TAB, Q,   W,   E,   R,   T,   Y,   U,   I,   O,   P,   LBRC,RBRC,BSLS,   DEL, END, PGDN, \
         LCTL,A,   S,   D,   F,   G,   H,   J,   K,   L,   SCLN,QUOT,      ENT,                   \
         LSFT,NUBS,Z,   X,   C,   V,   B,   N,   M,   COMM,DOT, SLSH,     RSFT,        UP,        \
         LCTL,LALT,LGUI,               SPC,                RGUI, FN0, FN1,RCTL,   LEFT,DOWN,RGHT), \

    /* Layer 1: Change GRV to ESC */
    KEYMAP(\
         ESC, F1,  F2,  F3,  F4,  F5,  F6,  F7,  F8,  F9,  F10, F11,  F12,       PSCR,SLCK,PAUS, \
         ESC, 1,   2,   3,   4,   5,   6,   7,   8,   9,   0,   MINS, EQL,BSPC,  INS, HOME,PGUP, \
         TAB, Q,   W,   E,   R,   T,   Y,   U,   I,   O,   P,   LBRC,RBRC,BSLS,  DEL, END, PGDN, \
         LCTL,A,   S,   D,   F,   G,   H,   J,   K,   L,   SCLN,QUOT,      ENT,                  \
         LSFT,NUBS,Z,   X,   C,   V,   B,   N,   M,   COMM,DOT, SLSH,     RSFT,       UP,        \
         LCTL,LALT,LGUI,               SPC,                RGUI, FN0, FN1, GRV,  LEFT,DOWN,RGHT), \

    /* Layer 2: FN Layer */
    KEYMAP(\
         ESC, F1,  F2,  F3,  F4,  F5,  F6,  F7,  F8,  F9,  F10, F11, F12,         PSCR,SLCK,PAUS, \
         GRV, F1,  F2,  F3,  F4,  F5,  F6,  F7,  F8,  F9,  F10, F11, F12, DEL,    INS, HOME,PGUP, \
         TRNS,TRNS,UP,  TRNS,TRNS,TRNS,CALC,TRNS,INS, TRNS,PSCR,SLCK,PAUS,TRNS,   DEL, END, PGDN, \
         TRNS,LEFT,DOWN,RGHT,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,HOME,PGUP,     TRNS,                   \
         TRNS,TRNS,TRNS,APP, TRNS,TRNS,TRNS,VOLD,VOLU,MUTE,END, PGDN,     TRNS,        UP,        \
         TRNS,TRNS,TRNS,          SPC,                     TRNS,TRNS,TRNS,TRNS,   LEFT,DOWN,RGHT), \

    /* Layer 3: Layout selector */
    KEYMAP(\
        FN3 ,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,        TRNS,TRNS,TRNS, \
        FN4 ,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,   TRNS,TRNS,TRNS, \
        TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,   TRNS,TRNS,TRNS, \
        TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,     TRNS,                   \
        TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,     TRNS,        TRNS,      \
        TRNS,TRNS,TRNS,               TRNS,               TRNS,TRNS,TRNS,TRNS,   TRNS,TRNS,TRNS)

/*
 *    KEYMAP(\
 *         CALC,MYCM,WSCH,WHOM,MAIL,VOLD,VOLU,MSEL,MSTP,MPLY,MPRV,MNXT,TRNS,        WAKE, PWR,SLEP, \
 *         TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,   TRNS,TRNS,TRNS, \
 *         TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,   TRNS,TRNS,TRNS, \
 *         TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,     TRNS,                   \
 *         TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,     TRNS,        TRNS,      \
 *         TRNS,TRNS,TRNS,               TRNS,               TRNS,TRNS,TRNS,TRNS,   TRNS,TRNS,TRNS)
 */
};

static const uint16_t PROGMEM fn_actions[] = {
    [0] = ACTION_LAYER_MOMENTARY(2),  // to Fn overlay
    [1] = ACTION_LAYER_MOMENTARY(3),
    [2] = ACTION_LAYER_TOGGLE(2),    // toggle arrow overlay
    [3] = ACTION_DEFAULT_LAYER_SET(1),
    [4] = ACTION_DEFAULT_LAYER_SET(0)
};
