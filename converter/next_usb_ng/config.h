/*
Copyright 2024 Jun Wako <wakojun@gmail.com>
*/

#ifndef CONFIG_H
#define CONFIG_H

#define VENDOR_ID       0xFEED
#define PRODUCT_ID      0xBCBC
#define DEVICE_VER      0x0600
#define MANUFACTURER    TMK
#define PRODUCT         NeXT keyboard converter


/* matrix size */
#define MATRIX_ROWS 12
#define MATRIX_COLS 8


/* key combination for command */
#define IS_COMMAND() ( \
    keyboard_report->mods == (MOD_BIT(KC_LALT) | MOD_BIT(KC_RALT)) || \
    keyboard_report->mods == (MOD_BIT(KC_LGUI) | MOD_BIT(KC_RGUI)) || \
    keyboard_report->mods == (MOD_BIT(KC_LSHIFT) | MOD_BIT(KC_RSHIFT)) \
)

/* Mouse scroll by button(right) */
#define MOUSE_SCROLL_BUTTON     MOUSE_BTN2

/* debug */
#define NEXT_DEBUG  0

#endif //config.h
