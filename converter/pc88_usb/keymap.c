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

#include <stdint.h>
#include <stdbool.h>
#include "keycode.h"
#include "action.h"
#include "util.h"
#include "serial.h"
#include "keymap.h"




// Following macros help you to define a keymap with the form of actual keyboard layout.

/* Keyboard Scan codes
 * ,---. ,---. ,------------------------. ,------------------------.  ,-------. 
 * | 60| | 61| | 62 | 63 | 64 | 65 | 66 | | 67 | 68 | 69 | 6A | 6B |  | 36| 37| 
 * `---' `---' `------------------------' `------------------------'  `-------' 
 * ,---------------------------------------------------------------.  ,-------.  ,---------------.
 * |  00 | 01| 02| 03| 04| 05| 06| 07| 08| 09| 0A| 0B| 0C| 0D| 0E  |  | 38| 39|  | 3E| 3F| 4D| 41|
 * |---------------------------------------------------------------|  `-------'  |---------------|
 * |   0F  | 10| 11| 12| 13| 14| 15| 16| 17| 18| 19| 1A| 1B|       |             | 42| 43| 44| 45|
 * |--------------------------------------------------------`  1C  |  ,-------.  |---------------|
 * |  74| 71| 1D| 1E| 1F| 20| 21| 22| 23| 24| 25| 26| 27| 28|      |  |   3A  |  | 46| 47| 48| 49|
 * |---------------------------------------------------------------|  |-------|  |---------------|
 * |    70    | 29| 2A| 2B| 2C| 2D| 2E| 2F| 30| 31| 32| 33|  78    |  | 3B| 3C|  | 4A| 4B| 4C| 4D|
 * `---------------------------------------------------------------'  |-------|  |---------------|
 *        | 72| 73|   51  |   34    |   35    | 7A| 7B|               |   3D  |  | 4E| 4F| 50| 79|
 *        `-------------------------------------------'               `-------'  `---------------'
 */

#define KEYMAP( \
    K60,  K61,   K62, K63, K64, K65, K66,  K67, K68, K69, K6A, K6B,                    K36, K37,                        \
    K00,      K01, K02, K03, K04, K05, K06, K07, K08, K09, K0A, K0B,  K0C, K0D, K0E,   K38, K39,    K3E, K3F, K40, K41, \
    K0F,      K10, K11, K12, K13, K14, K15, K16, K17, K18, K19, K1A, K1B,       K1C,                K42, K43, K44, K45, \
	K74, K71, K1D, K1E, K1F, K20, K21, K22, K23, K24, K25, K26, K27,K28,                  K3A,      K46, K47, K48, K49, \
	K70,      K29, K2A, K2B, K2C, K2D, K2E, K2F, K30, K31, K32, K33,            K78,   K3B, K3C,    K4A, K4B, K4C, K4D, \
         K72, K73, K51,      K34,      K35,      K7A, K7B,                                K3D,      K4E, K4F, K50, K79  \
) { \
    { KC_##K00, KC_##K01, KC_##K02, KC_##K03, KC_##K04, KC_##K05, KC_##K06, KC_##K07 }, \
    { KC_##K08, KC_##K09, KC_##K0A, KC_##K0B, KC_##K0C, KC_##K0D, KC_##K0E, KC_##K0F }, \
    { KC_##K10, KC_##K11, KC_##K12, KC_##K13, KC_##K14, KC_##K15, KC_##K16, KC_##K17 }, \
    { KC_##K18, KC_##K19, KC_##K1A, KC_##K1B, KC_##K1C, KC_##K1D, KC_##K1E, KC_##K1F }, \
    { KC_##K20, KC_##K21, KC_##K22, KC_##K23, KC_##K24, KC_##K25, KC_##K26, KC_##K27 }, \
    { KC_##K28, KC_##K29, KC_##K2A, KC_##K2B, KC_##K2C, KC_##K2D, KC_##K2E, KC_##K2F }, \
    { KC_##K30, KC_##K31, KC_##K32, KC_##K33, KC_##K34, KC_##K35, KC_##K36, KC_##K37 }, \
    { KC_##K38, KC_##K39, KC_##K3A, KC_##K3B, KC_##K3C, KC_##K3D, KC_##K3E, KC_##K3F }, \
    { KC_##K40, KC_##K41, KC_##K42, KC_##K43, KC_##K44, KC_##K45, KC_##K46, KC_##K47 }, \
    { KC_##K48, KC_##K49, KC_##K4A, KC_##K4B, KC_##K4C, KC_##K4D, KC_##K4E, KC_##K4F }, \
    { KC_##K50, KC_##K51, KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO    }, \
    { KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO    }, \
    { KC_##K60, KC_##K61, KC_##K62, KC_##K63, KC_##K64, KC_##K65, KC_##K66, KC_##K67 }, \
    { KC_##K68, KC_##K69, KC_##K6A, KC_##K6B, KC_NO,    KC_NO,    KC_NO,    KC_NO    }, \
    { KC_##K70, KC_##K71, KC_##K72, KC_##K73, KC_##K74, KC_NO,    KC_NO,    KC_NO    }, \
    { KC_##K78, KC_##K79, KC_##K7A, KC_##K7B, KC_NO,    KC_NO,    KC_NO,    KC_NO    }  \
}


const action_t fn_actions[] PROGMEM = {
    [0] = ACTION_FUNCTION(0),   // toggle all LEDs
};

void action_function(keyrecord_t *record, uint8_t id, uint8_t opt)
{
    static bool led = false;
    switch (id) {
        case 0:
            if (record->event.pressed) {
                if ((led = !led))
                    serial_send(0x80);  // all on
                else
                    serial_send(0xff);  // all off
            }
            break;
    }
}


const uint8_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
/* Keyboard Scan codes
 * ,---. ,---. ,------------------------. ,------------------------.  ,-------. 
 * | 60| | 61| | 62 | 63 | 64 | 65 | 66 | | 67 | 68 | 69 | 6A | 6B |  | 36| 37| 
 * `---' `---' `------------------------' `------------------------'  `-------' 
 * ,---------------------------------------------------------------.  ,-------.  ,---------------.
 * |  00 | 01| 02| 03| 04| 05| 06| 07| 08| 09| 0A| 0B| 0C| 0D| 0E  |  | 38| 39|  | 3E| 3F| 4D| 41|
 * |---------------------------------------------------------------|  `-------'  |---------------|
 * |   0F  | 10| 11| 12| 13| 14| 15| 16| 17| 18| 19| 1A| 1B|       |             | 42| 43| 44| 45|
 * |--------------------------------------------------------`  1C  |  ,-------.  |---------------|
 * |  74| 71| 1D| 1E| 1F| 20| 21| 22| 23| 24| 25| 26| 27| 28|      |  |   3A  |  | 46| 47| 48| 49|
 * |---------------------------------------------------------------|  |-------|  |---------------|
 * |    70    | 29| 2A| 2B| 2C| 2D| 2E| 2F| 30| 31| 32| 33|  78    |  | 3B| 3C|  | 4A| 4B| 4C| 4D|
 * `---------------------------------------------------------------'  |-------|  |---------------|
 *        | 72| 73|   51  |   34    |   35    | 7A| 7B|               |   3D  |  | 4E| 4F| 50| 79|
 *        `-------------------------------------------'               `-------'  `---------------'
 */
    /* ANSI */
    KEYMAP(
    PAUS,PSCR,   F1,  F2,  F3,  F4,  F5,   F6,  F7,  F8,  F9,  F10,                    PGDN,PGUP,
    ESC,      1,   2,   3,   4,   5,   6,   7,   8,   9,   0,   MINS, EQL, BSLS,BSPC,  INS, DEL,    HOME, END,PMNS,PSLS,
    TAB,      Q,   W,   E,   R,   T,   Y,   U,   I,   O,   P,   LBRC,RBRC,      ENT,                P7,  P8,  P9,  PAST,
    LCTL,LCAP,A,   S,   D,   F,   G,   H,   J,   K,   L,   SCLN,QUOT,F18,                 UP,       P4,  P5,  P6,  PPLS,
    LSFT,     Z,   X,   C,   V,   B,   N,   M,   COMM,DOT, SLSH,GRV,            RSFT,  LEFT,RGHT,   P1,  P2,  P3,  PEQL,
         F19, LGUI,LALT,     SPC,      RALT,     APP, RGUI,                              DOWN,      P0,  PCMM,PDOT,PENT
    ),
    /* JIS */
    KEYMAP(
    PAUS, PSCR,  F1,  F2,  F3,  F4,  F5,   F6,  F7,  F8,  F9,  F10,                    PGDN,PGUP,
    ESC,      1,   2,   3,   4,   5,   6,   7,   8,   9,   0,   MINS, EQL, JYEN,BSPC,  INS, DEL,    HOME, END,PMNS,PSLS,
    TAB,      Q,   W,   E,   R,   T,   Y,   U,   I,   O,   P,   LBRC,RBRC,      ENT,                P7,  P8,  P9,  PAST,
    LCTL,LCAP,A,   S,   D,   F,   G,   H,   J,   K,   L,   SCLN,QUOT,NUHS,                UP,       P4,  P5,  P6,  PPLS,
    LSFT,     Z,   X,   C,   V,   B,   N,   M,   COMM,DOT, SLSH,RO,             RSFT,  LEFT,RGHT,   P1,  P2,  P3,  PEQL,
         KANA,LALT,MHEN,     SPC,      HENK,     RGUI,ZKHK,                              DOWN,      P0,  PCMM,PDOT,PENT
    ),
};
