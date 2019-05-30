/*
Copyright 2012 Jun Wako <wakojun@gmail.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef CONFIG_H
#define CONFIG_H


/* USB Device descriptor parameter */

#define VENDOR_ID       0xFEED
#define PRODUCT_ID      0x0A0C
#define DEVICE_VER      0x0534
#define MANUFACTURER    di0ib
#define PRODUCT         The semaphore Keyboard
#define DESCRIPTION     A split 60 key keyboard

/* key matrix size */
#define ROWS_PER_HAND 5
#define MATRIX_COLS 6

#define MATRIX_ROWS ROWS_PER_HAND*2

#define MATRIX_COL_PINS { F4, F5, F6, F7, B1, B3 }
#define MATRIX_ROW_PINS { B2, B6, D4, C6, D7 }


/* use i2c instead of serial */
//#define USE_I2C

//#define I2C_WRITE_TEST_CODE

/* define if matrix has ghost */
//#define MATRIX_HAS_GHOST

/* Set 0 if debouncing isn't needed */
#define DEBOUNCE    5

/* Mechanical locking support. Use KC_LCAP, KC_LNUM or KC_LSCR instead in keymap */
#define LOCKING_SUPPORT_ENABLE
/* Locking resynchronize hack */
#define LOCKING_RESYNC_ENABLE

/*
 * Feature disable options
 *  These options are also useful to firmware size reduction.
 */

/* disable debug print */
//#define NO_DEBUG

/* disable print */
//#define NO_PRINT

/* disable action features */
//#define NO_ACTION_LAYER
//#define NO_ACTION_TAPPING
//#define NO_ACTION_ONESHOT
//#define NO_ACTION_MACRO
//#define NO_ACTION_FUNCTION

/* key combination for command */
#define IS_COMMAND() ( \
    keyboard_report->mods == (MOD_BIT(KC_LCTL) | MOD_BIT(KC_LALT) | MOD_BIT(KC_LGUI)) \
)

/* boot magic key */
#define BOOTMAGIC_KEY_SALT              KC_Q

#ifdef SPACE_ON_LEFT_HALF
#define BOOTMAGIC_KEY_DEFAULT_LAYER_0   KC_Z
#define BOOTMAGIC_KEY_DEFAULT_LAYER_1   KC_X
#define BOOTMAGIC_KEY_DEFAULT_LAYER_2   KC_C
#define BOOTMAGIC_HOST_NKRO             KC_V
#else
#define BOOTMAGIC_KEY_DEFAULT_LAYER_0   KC_M
#define BOOTMAGIC_KEY_DEFAULT_LAYER_1   KC_COMM
#define BOOTMAGIC_KEY_DEFAULT_LAYER_2   KC_DOT
#define BOOTMAGIC_HOST_NKRO             KC_N
#endif

/* Mousekey settings */
#define MOUSEKEY_MOVE_MAX          127 // default 127
#define MOUSEKEY_WHEEL_MAX         127 // default 127
#define MOUSEKEY_MOVE_DELTA        5   // default 5
#define MOUSEKEY_WHEEL_DELTA       1   // default 1
#define MOUSEKEY_DELAY             300 // default 300
#define MOUSEKEY_INTERVAL          50  // default 50
#define MOUSEKEY_MAX_SPEED         5   // default 10
#define MOUSEKEY_TIME_TO_MAX       10  // default 20
#define MOUSEKEY_WHEEL_MAX_SPEED   8   // default 8
#define MOUSEKEY_WHEEL_TIME_TO_MAX 40  // default 40

/* Action tapping settings */
#define TAPPING_TERM    200 // default 200
/* #define TAPPING_TOGGLE  2   // default 5 */
/* #define ONESHOT_TIMEOUT 5000 // default undefined */
#define ONESHOT_TAP_TOGGLE 2

#endif
