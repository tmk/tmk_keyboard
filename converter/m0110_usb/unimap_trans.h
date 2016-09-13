/*
Copyright 2016 Jun Wako <wakojun@gmail.com>

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
#ifndef UNIMAP_COMMON_H
#define UNIMAP_COMMON_H

#include <stdint.h>
#include <avr/pgmspace.h>
#include "unimap.h"


/* M0110A scan codes
 * ,---------------------------------------------------------. ,---------------.
 * | 32| 12| 13| 14| 15| 17| 16| 1A| 1C| 19| 1D| 1B| 18|   33| | 47| 68| 6D| 62|
 * |---------------------------------------------------------| |---------------|
 * |   30| 0C| 0D| 0E| 0F| 11| 10| 20| 22| 1F| 23| 21| 1E|   | | 59| 5B| 5C| 4E|
 * |-----------------------------------------------------'   | |---------------|
 * |    39| 00| 01| 02| 03| 05| 04| 26| 28| 25| 29| 27|    24| | 56| 57| 58| 66|
 * |---------------------------------------------------------| |---------------|
 * |      38| 06| 07| 08| 09| 0B| 2D| 2E| 2B| 2F| 2C|  38| 4D| | 53| 54| 55|   |
 * `---------------------------------------------------------' |-----------| 4C|
 * |   3A|     37|             31            | 2A| 46| 42| 48| |     52| 41|   |
 * `---------------------------------------------------------' `---------------'
 *
 * M0110 + M0120 scan codes
 * ,---------------------------------------------------------. ,---------------.
 * | 32| 12| 13| 14| 15| 17| 16| 1A| 1C| 19| 1D| 1B| 18|   33| | 47| 68| 6D| 62|
 * |---------------------------------------------------------| |---------------|
 * |   30| 0C| 0D| 0E| 0F| 11| 10| 20| 22| 1F| 23| 21| 1E| 2A| | 59| 5B| 5C| 4E|
 * |---------------------------------------------------------| |---------------|
 * |    39| 00| 01| 02| 03| 05| 04| 26| 28| 25| 29| 27|    24| | 56| 57| 58| 66|
 * |---------------------------------------------------------| |---------------|
 * |      38| 06| 07| 08| 09| 0B| 2D| 2E| 2B| 2F| 2C|      38| | 53| 54| 55|   |
 * `---------------------------------------------------------' |-----------| 4C|
 *      | 3A|  37|             31              |   34| 3A|     |     52| 41|   |
 *      `------------------------------------------------'     `---------------'
 *
 * International M0110
 * https://en.wikipedia.org/wiki/File:Apple_Macintosh_Plus_Keyboard.jpg
 * Probably International keyboard layout of M0110A doesn't exist.
 * ,---------------------------------------------------------.
 * | 32| 12| 13| 14| 15| 17| 16| 1A| 1C| 19| 1D| 1B| 18|   33|
 * |---------------------------------------------------------|
 * |   30| 0C| 0D| 0E| 0F| 11| 10| 20| 22| 1F| 23| 21| 1E| 2A|
 * |------------------------------------------------------,  |
 * |    39| 00| 01| 02| 03| 05| 04| 26| 28| 25| 29| 27| 24|  |
 * |---------------------------------------------------------|
 * |  38| 06| 07| 08| 09| 0B| 2D| 2E| 2B| 2F| 2C| 0A|      38|
 * `---------------------------------------------------------'
 *      | 3A|  37|             34              |   31| 3A|
 *      `------------------------------------------------'
 */
const uint8_t PROGMEM unimap_trans[MATRIX_ROWS][MATRIX_COLS] = {
        //  Position(unimap)        scan code(matrix)
        //  ---------------------------------------------
        {
            UNIMAP_A,               // 0x00
            UNIMAP_S,               // 0x01
            UNIMAP_D,               // 0x02
            UNIMAP_F,               // 0x03
            UNIMAP_H,               // 0x04
            UNIMAP_G,               // 0x05
#ifdef M0110_INTL
            UNIMAP_NONUS_BSLASH,    // 0x06
            UNIMAP_Z,               // 0x07
        },
        {
            UNIMAP_X,               // 0x08
            UNIMAP_C,               // 0x09
            UNIMAP_SLASH,           // 0x0A
            UNIMAP_V,               // 0x0B
#else
            UNIMAP_Z,               // 0x06
            UNIMAP_X,               // 0x07
        },
        {
            UNIMAP_C,               // 0x08
            UNIMAP_V,               // 0x09
            UNIMAP_NO,              // 0x0A
            UNIMAP_B,               // 0x0B
#endif
            UNIMAP_Q,               // 0x0C
            UNIMAP_W,               // 0x0D
            UNIMAP_E,               // 0x0E
            UNIMAP_R,               // 0x0F
        },
        {
            UNIMAP_Y,               // 0x10
            UNIMAP_T,               // 0x11
            UNIMAP_1,               // 0x12
            UNIMAP_2,               // 0x13
            UNIMAP_3,               // 0x14
            UNIMAP_4,               // 0x15
            UNIMAP_6,               // 0x16
            UNIMAP_5,               // 0x17
        },
        {
            UNIMAP_EQUAL,           // 0x18
            UNIMAP_9,               // 0x19
            UNIMAP_7,               // 0x1A
            UNIMAP_MINUS,           // 0x1B
            UNIMAP_8,               // 0x1C
            UNIMAP_0,               // 0x1D
            UNIMAP_RBRACKET,        // 0x1E
            UNIMAP_O,               // 0x1F
        },
        {
            UNIMAP_U,               // 0x20
            UNIMAP_LBRACKET,        // 0x21
            UNIMAP_I,               // 0x22
            UNIMAP_P,               // 0x23
#ifdef M0110_INTL
            UNIMAP_NONUS_HASH,      // 0x24
#else
            UNIMAP_ENTER,           // 0x24
#endif
            UNIMAP_L,               // 0x25
            UNIMAP_J,               // 0x26
            UNIMAP_QUOTE,           // 0x27
        },
        {
            UNIMAP_K,               // 0x28
            UNIMAP_SCOLON,          // 0x29
#ifdef M0110_INTL
            UNIMAP_ENTER,           // 0x2A
            UNIMAP_M,               // 0x2B
            UNIMAP_DOT,             // 0x2C
            UNIMAP_B,               // 0x2D
            UNIMAP_N,               // 0x2E
            UNIMAP_COMMA,           // 0x2F
#else
            UNIMAP_BSLASH,          // 0x2A
            UNIMAP_COMMA,           // 0x2B
            UNIMAP_SLASH,           // 0x2C
            UNIMAP_N,               // 0x2D
            UNIMAP_M,               // 0x2E
            UNIMAP_DOT,             // 0x2F
#endif
        },
        {
            UNIMAP_TAB,             // 0x30
#ifdef M0110_INTL
            UNIMAP_RGUI,            // 0x31
#else
            UNIMAP_SPACE,           // 0x31
#endif
            UNIMAP_GRAVE,           // 0x32
            UNIMAP_BSPACE,          // 0x33
#ifdef M0110_INTL
            UNIMAP_SPACE,           // 0x34
#else
            UNIMAP_RGUI,            // 0x34
#endif
            UNIMAP_NO,              // 0x35
            UNIMAP_NO,              // 0x36
            UNIMAP_LGUI,            // 0x37
        },
        {
            UNIMAP_LSHIFT,          // 0x38
            UNIMAP_CAPSLOCK,        // 0x39
            UNIMAP_LALT,            // 0x3A
            UNIMAP_NO,              // 0x3B
            UNIMAP_NO,              // 0x3C
            UNIMAP_NO,              // 0x3D
            UNIMAP_NO,              // 0x3E
            UNIMAP_NO,              // 0x3F
        },
        {
            UNIMAP_NO,              // 0x40
            UNIMAP_KP_DOT,          // 0x41
            UNIMAP_RIGHT,           // 0x42
            UNIMAP_NO,              // 0x43
            UNIMAP_NO,              // 0x44
            UNIMAP_NO,              // 0x45
            UNIMAP_LEFT,            // 0x46
            UNIMAP_NUMLOCK,         // 0x47
        },
        {
            UNIMAP_DOWN,            // 0x48
            UNIMAP_NO,              // 0x49
            UNIMAP_NO,              // 0x4A
            UNIMAP_NO,              // 0x4B
            UNIMAP_KP_ENTER,        // 0x4C
            UNIMAP_UP,              // 0x4D
            UNIMAP_KP_PLUS,         // 0x4E     keypad minus
            UNIMAP_NO,              // 0x4F
        },
        {
            UNIMAP_NO,              // 0x50
            UNIMAP_NO,              // 0x51
            UNIMAP_KP_0,            // 0x52
            UNIMAP_KP_1,            // 0x53
            UNIMAP_KP_2,            // 0x54
            UNIMAP_KP_3,            // 0x55
            UNIMAP_KP_4,            // 0x56
            UNIMAP_KP_5,            // 0x57
        },
        {
            UNIMAP_KP_6,            // 0x58
            UNIMAP_KP_7,            // 0x59
            UNIMAP_NO,              // 0x5A
            UNIMAP_KP_8,            // 0x5B
            UNIMAP_KP_9,            // 0x5C
            UNIMAP_NO,              // 0x5D
            UNIMAP_NO,              // 0x5E
            UNIMAP_NO,              // 0x5F
        },
        {
            UNIMAP_NO,              // 0x60
            UNIMAP_NO,              // 0x61
            UNIMAP_KP_MINUS,        // 0x62     keypad asterisk
            UNIMAP_NO,              // 0x63
            UNIMAP_NO,              // 0x64
            UNIMAP_NO,              // 0x65
            UNIMAP_KP_COMMA,        // 0x66
            UNIMAP_NO,              // 0x67
        },
        {
            UNIMAP_KP_SLASH,        // 0x68     keypad equal
            UNIMAP_NO,              // 0x69
            UNIMAP_NO,              // 0x6A
            UNIMAP_NO,              // 0x6B
            UNIMAP_NO,              // 0x6C
            UNIMAP_KP_ASTERISK,     // 0x6D     keypad slash
            UNIMAP_NO,              // 0x6E     compose
            UNIMAP_NO,              // 0x6F
        },
};

#endif

