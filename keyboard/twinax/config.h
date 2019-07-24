/*
 * Configuration for IBM Twinax keyboard(1394312, german version)
 * (C) Copyright 2019
 * Dirk Eibach <dirk.eibach@gmail.com>
 * 
 * Based on Model M support by Les Orchard.
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef CONFIG_H
#define CONFIG_H

/* USB Device descriptor parameter */
#define VENDOR_ID       0xFEED
#define PRODUCT_ID      0x6057
#define DEVICE_VER      0x0001
#define MANUFACTURER    IBM
#define PRODUCT         Twinax

/* message strings */
#define DESCRIPTION     t.m.k. keyboard firmware for IBM Twinax

/* matrix size */
#define MATRIX_ROWS 20
#define MATRIX_COLS 8

/* define if matrix has ghost */
#define MATRIX_HAS_GHOST

/* Set 0 if need no debouncing */
#define DEBOUNCE    5

/* key combination for command */
#define IS_COMMAND() ( \
    keyboard_report->mods == (MOD_BIT(KC_LALT) | MOD_BIT(KC_RALT)) \
)

#endif
