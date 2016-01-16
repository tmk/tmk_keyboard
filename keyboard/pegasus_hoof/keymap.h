static const uint8_t PROGMEM keymaps[][MATRIX_COLS][MATRIX_ROWS] = {
  /* Default Layer */
  KEYMAP(\
         ESC,        F1,  F2,  F3,  F4,  F5,  F6,  F7,  F8,  F9, F10, F11, F12,  PSCR,SLCK,PAUS,  \
         GRV,    1,   2,   3,   4,   5,   6,   7,   8,   9,   0,MINS, EQL,BSPC,   INS,HOME,PGUP,  \
         TAB,    Q,   W,   E,   R,   T,   Y,   U,   I,   O,   P,LBRC,RBRC,BSLS,   DEL, END,PGDN,  \
         CAPS,   A,   S,   D,   F,   G,   H,   J,   K,   L,SCLN,QUOT,      ENT,                   \
         LSFT,NUBS,   Z,   X,   C,   V,   B,   N,   M,COMM, DOT,SLSH,     RSFT,         UP,       \
         LCTL,LGUI,LALT,                SPC,               RALT,RGUI, FN0,RCTL,  LEFT,DOWN,RGHT), \
  /* Layer 1: Function layer */
  KEYMAP(\
         MUTE,     TRNS,TRNS,TRNS,TRNS,VOLD,VOLU,MSEL,MSTP,MPLY,MPRV,MNXT,TRNS,  TRNS,TRNS,TRNS,  \
         TRNS,BTN1,BTN2,BTN3,BTN4,BTN5,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,MUTE,  MPLY,MPRV,VOLU,  \
         TRNS,TRNS,MS_U,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,MYCM,TRNS,TRNS,TRNS,  MSTP,MNXT,VOLD,  \
         TRNS,MS_L,MS_D,MS_R,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,     TRNS,                   \
         TRNS,TRNS,TRNS,TRNS,CALC,TRNS,TRNS,TRNS,MAIL,TRNS,TRNS,TRNS,     TRNS,       VOLU,       \
         TRNS,TRNS,TRNS,               TRNS,                TRNS,TRNS, FN0,FN1,  MPRV,VOLD,MNXT), \
  /* Layer 2: Function layer */
  KEYMAP(\
         TRNS,     TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,  WAKE, PWR,SLEP,  \
         TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,  TRNS,TRNS,TRNS,  \
         TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,  TRNS,TRNS,TRNS,  \
         TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,     TRNS,                   \
         TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,     TRNS,        TRNS,      \
         TRNS,TRNS,TRNS,               TRNS,               TRNS,TRNS, FN0, FN1,  TRNS,TRNS,TRNS), \
};

static const uint16_t PROGMEM fn_actions[] = {
    [0] = ACTION_LAYER_MOMENTARY(1),
    [1] = ACTION_LAYER_MOMENTARY(2)
};
