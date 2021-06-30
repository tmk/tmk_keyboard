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


#define VENDOR_ID       0xFEED
#define PRODUCT_ID      0x005B
#define DEVICE_VER      0x0814
#define MANUFACTURER    TMK
#define PRODUCT         USB Descriptor Dumper


#define DESCRIPTION     Product from TMK keyboard firmware project


/* matrix size */
#define MATRIX_ROWS 0
#define MATRIX_COLS 0

/* key combination for command */
#define IS_COMMAND() (keyboard_report->mods == (MOD_BIT(KC_LSHIFT) | MOD_BIT(KC_RSHIFT))) 


// Disable power saving in USB suspend loop but remote wakeup is still valid.
// This allows keep USB::Task() going during suspend without power down time delay.
//#define NO_SUSPEND_POWER_DOWN


// Disable USB startup wait, which can delays starting UHS2 Task() for 350-600ms.
//#define NO_USB_STARTUP_WAIT_LOOP

// Disable USB suspend loop, which blocks UHS2 Task() while power saving.
// Note that this also disables power saving and remote wakeup from keyboard completely.
//#define NO_USB_SUSPEND_LOOP

#endif
