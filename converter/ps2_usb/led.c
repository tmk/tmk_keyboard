/*
Copyright 2011 Jun Wako <wakojun@gmail.com>

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

#include "stdint.h"
#include "ps2.h"
#include "led.h"


void led_set(uint8_t usb_led)
{
    uint8_t ps2_led = 0;
    if (usb_led &  (1<<USB_LED_SCROLL_LOCK)) {
        ps2_led |= (1<<PS2_LED_SCROLL_LOCK);
	SCROLL_LOCK_LED_ON; }
    else SCROLL_LOCK_LED_OFF;
    if (usb_led &  (1<<USB_LED_NUM_LOCK)) {
        ps2_led |= (1<<PS2_LED_NUM_LOCK);
	NUM_LOCK_LED_ON; }
    else NUM_LOCK_LED_OFF;
    if (usb_led &  (1<<USB_LED_CAPS_LOCK)) {
        ps2_led |= (1<<PS2_LED_CAPS_LOCK);
	CAPS_LOCK_LED_ON; }
    else CAPS_LOCK_LED_OFF;
    ps2_host_set_led(ps2_led);
}


#ifdef PHYSICAL_LEDS_ENABLE

void physical_led_init()
{

    uint8_t counter;

    for (counter=0; counter<LED_INIT_LOOPS; counter++) {
#ifdef NUM_LOCK_LED_ENABLE
    NUM_LOCK_LED_INIT;
    NUM_LOCK_LED_ON;
    _delay_ms(LED_INIT_DELAY);
    NUM_LOCK_LED_OFF;
#endif

#ifdef CAPS_LOCK_LED_ENABLE
    CAPS_LOCK_LED_INIT;
    CAPS_LOCK_LED_ON;
    _delay_ms(LED_INIT_DELAY);
    CAPS_LOCK_LED_OFF;
#endif

#ifdef SCROLL_LOCK_LED_ENABLE
    SCROLL_LOCK_LED_INIT;
    SCROLL_LOCK_LED_ON;
    _delay_ms(LED_INIT_DELAY);
    SCROLL_LOCK_LED_OFF;
#endif 
    }

}

#endif
    
