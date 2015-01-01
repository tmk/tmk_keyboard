/*
Copyright 2014 Dave McEwan cogitocumimpune@hotmail.com

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


This work is heavily based on TMK port for Ergodox keyboard.
Copyright 2013 Oleg Kostyuk <cub.uanic@gmail.com>


This work is heavily based on initial firmware for Ergodox keyboard.
Copyright (c) 2012, 2013 Ben Blazak <benblazak.dev@gmail.com>
Released under The MIT License (see "doc/licenses/MIT.md")
Project located at <https://github.com/benblazak/ergodox-firmware>
*/

#include <stdint.h>
#include <avr/io.h>
#include "print.h"
#include "debug.h"
#include "led.h"
#include "mcdox.h"


void led_set(uint8_t usb_led)
{
    // NumLock - Represented by LHS backlight
#ifdef INVERT_NUMLOCK
    if (usb_led & (1<<USB_LED_NUM_LOCK)) {
        mcdox_right_led_on();
    } else {
        mcdox_right_led_off();
    }
#else
    if (usb_led & (1<<USB_LED_NUM_LOCK)) {
        mcdox_right_led_off();
    } else {
        mcdox_right_led_on();
    }
#endif

    // CapsLock - Represented by LHS backlight
    if (usb_led & (1<<USB_LED_CAPS_LOCK)) {
        mcdox_left_led_on();
    } else {
        mcdox_left_led_off();
    }

    // ScrollLock - Nothing uses this...
    /*
    if (usb_led & (1<<USB_LED_SCROLL_LOCK)) {
        ergodox_right_led_3_on();
    } else {
        ergodox_right_led_3_off();
    }
    */
}

