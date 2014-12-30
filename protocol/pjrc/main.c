/* Keyboard example with debug channel, for Teensy USB Development Board
 * http://www.pjrc.com/teensy/usb_keyboard.html
 * Copyright (c) 2008 PJRC.COM, LLC
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <stdbool.h>
#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "keyboard.h"
#include "usb.h"
#include "matrix.h"
#include "print.h"
#include "debug.h"
#include "sendchar.h"
#include "util.h"
#include "suspend.h"
#include "host.h"
#include "pjrc.h"


#define CPU_PRESCALE(n)    (CLKPR = 0x80, CLKPR = (n))


int main(void)
{
    // Set for 16 MHz clock
    CPU_PRESCALE(0);

    // Initialize the USB, and then wait for the host to set configuration.
    // If the Teensy is powered without a PC connected to the USB port,
    // this will wait forever.
    usb_init();
    while (!usb_configured()) /* wait */ ;

    // TODO: This currently does nothing.
    // TODO: To select output destinations: UART/USBSerial.
    print_set_sendchar(sendchar);

    // Call all the initialisation routines fro setting up IO and such defined
    // per keyboard.
    keyboard_init();

    // Setup driver pointers defined per protocol.
    host_set_driver(pjrc_driver());

#ifdef SLEEP_LED_ENABLE
    sleep_led_init();
#endif

    while (1) {

        // The suspend flag (extern bool defined in usb.c) is flipped by the ISR
        // called within suspend_power_down().
        // The remote_wakeup flag is used the same way.
        while (suspend) {
            // Use watchdog timer to sleep for a while.
            suspend_power_down(WDTO_120MS);

            if (remote_wakeup && suspend_wakeup_condition()) {
                usb_remote_wakeup();
            }
        }

        // Main functionallity to scan matrix, update leds, etc.
        // Simply called as fast as possible when suspend flag is false.
        keyboard_task(); 
    }
}
