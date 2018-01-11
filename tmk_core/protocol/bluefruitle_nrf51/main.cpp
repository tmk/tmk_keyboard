/*
Bluefruit Protocol for TMK firmware
Author: Benjamin Gould, 2013
Based on code Copyright 2011 Jun Wako <wakojun@gmail.com>

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

extern "C" {
#include <stdint.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <avr/sleep.h>
#include <util/delay.h>
#include "serial.h"
#include "keyboard.h"
#include "usb.h"
#include "host.h"
#include "timer.h"
#include "print.h"
#include "sendchar.h"
#include "suspend.h"
#include "pjrc.h"
}
#include "bluefruitle.h"

extern "C" void __cxa_pure_virtual(void);

void __cxa_pure_virtual(void) {};


int main(void)
{   
    ble_init();

    dprintf("Initializing keyboard...");
    keyboard_init();
    
    dprintf("Setting host driver to bluefruit...");
    _delay_ms(1000);
    host_set_driver(bluefruitle_driver());

    // wait an extra second for the PC's operating system
    // to load drivers and do whatever it does to actually
    // be ready for input
    _delay_ms(1000);
    dprintf("Starting main loop");
    while (1) {
//        ble_task();
        keyboard_task();
    }
}
