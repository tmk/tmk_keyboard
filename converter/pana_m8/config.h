#ifndef CONFIG_H
#define CONFIG_H


#define VENDOR_ID       0xFEED
#define PRODUCT_ID      0xFFFF
#define DEVICE_VER      0x0100
#define MANUFACTURER    TMK.
#define PRODUCT         Panasonic Cherry M8
#define DESCRIPTION     TMK. keyboard firmware


/* matrix size */
#define MATRIX_ROWS 8
#define MATRIX_COLS 8

/* matrix debounce time in ms */
#define DEBOUNCE 10


/* key combination for command */
#define IS_COMMAND() (keyboard_report->mods == (MOD_BIT(KC_LSHIFT) | MOD_BIT(KC_RSHIFT))) 


/* period of tapping(ms) */
#define TAPPING_TERM    300
/* tap count needed for toggling a feature */
#define TAPPING_TOGGLE  5
/* Oneshot timeout(ms) */
#define ONESHOT_TIMEOUT 300

/* Boot Magic salt key: Space */
#define BOOTMAGIC_KEY_SALT      KC_SPACE

#endif
