/*
Copyright 2013 Oleg Kostyuk <cub.uanic@gmail.com>

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
#include <avr/io.h>
#include "print.h"
#include "debug.h"
#include "led.h"
#include "ergodox.h"


void led_set(uint8_t usb_led)
{
    // topmost - NumLock
#ifdef INVERT_NUMLOCK
    if (usb_led & (1<<USB_LED_NUM_LOCK)) {
        ergodox_right_led_1_on();
    } else {
        ergodox_right_led_1_off();
    }
#else
    if (usb_led & (1<<USB_LED_NUM_LOCK)) {
        ergodox_right_led_1_off();
    } else {
        ergodox_right_led_1_on();
    }
#endif

    // middle - CapsLock
    if (usb_led & (1<<USB_LED_CAPS_LOCK)) {
        ergodox_right_led_2_on();
    } else {
        ergodox_right_led_2_off();
    }

    // bottommost - ScrollLock
    if (usb_led & (1<<USB_LED_SCROLL_LOCK)) {
        ergodox_right_led_3_on();
    } else {
        ergodox_right_led_3_off();
    }
}

