
static const uint8_t PROGMEM keymaps[][MATRIX_COLS][MATRIX_ROWS] = {
    /* Layer 0: Standard ANSI layer */
    KEYMAP(A, B, C, D, E, F, G), \
    /* Layer 1: Function layer */
    KEYMAP(F1, F2, F3, F4, F5, F6, F7)
};

static const uint16_t PROGMEM fn_actions[] = {
    [0] = ACTION_LAYER_MOMENTARY(1)
};
