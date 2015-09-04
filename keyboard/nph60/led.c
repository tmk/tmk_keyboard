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

#include <avr/io.h>
#include <stdbool.h>
#include "stdint.h"
#include "led.h"
#include "action_layer.h"

void led_builtIn(bool enable)
{
    if (enable) {
        // set pin as output
        DDRD |= (1<<6);
        // output low
        //PORTD &= ~(1<<6);
        PORTD |= (1<<6); // Teensy 2.0 built-in LED is active high
    } else {
        // Hi-Z
        DDRD &= ~(1<<6);
        PORTD &= ~(1<<6);
    }
}

void led_escKey(bool enable)
{
    if (enable) {
        // output low
        DDRE |= (1<<6);
        PORTE &= ~(1<<6);
    } else {
        // Hi-Z
        DDRE &= ~(1<<6);
        PORTE &= ~(1<<6);
    }
}

void led_capsLockKey(bool enable)
{
    if (enable) {
        // output low
        DDRD |= (1<<4);
        PORTD &= ~(1<<4);
    } else {
        // Hi-Z
        DDRD &= ~(1<<4);
        PORTD &= ~(1<<4);
    }
}

void led_nKey(bool enable)
{
    if (enable) {
        // output low
        DDRB |= (1<<0);
        PORTB &= ~(1<<0);
    } else {
        // Hi-Z
        DDRB &= ~(1<<0);
        PORTB &= ~(1<<0);
    }
}

void led_set(uint8_t usb_led)
{
    // Teensy built-in LED, used as Scroll Lock indicator
    led_builtIn(usb_led & (1<<USB_LED_SCROLL_LOCK));

    // Esc key LED, used as Function layer indicator
    led_escKey(layer_state & (1<<3));    // turn LED on if layer 3 is active
    //led_escKey(biton32(layer_state) == 3); // turn LED on if layer 3 is the highest active layer

    // N key LED, used as Numpad layer indicator
    led_nKey(layer_state & (1<<1));

    // Caps Lock key LED
    if (layer_state & (1<<1)) {
        // Numpad layer active
        // used as Num Lock indicator
        led_capsLockKey(usb_led & (1<<USB_LED_NUM_LOCK));
    } else {
        // Numpad layer not active
        // used as Caps Lock indicator
        led_capsLockKey(usb_led & (1<<USB_LED_CAPS_LOCK));
    }
}
