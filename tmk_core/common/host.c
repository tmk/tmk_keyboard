/*
Copyright 2011,2012 Jun Wako <wakojun@gmail.com>

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
//#include <avr/interrupt.h>
#include "keycode.h"
#include "host.h"
#include "util.h"
#include "debug.h"


#ifdef NKRO_ENABLE
bool keyboard_nkro = true;
#endif

static host_driver_t *driver;
static uint16_t last_system_report = 0;
static uint16_t last_consumer_report = 0;
static report_keyboard_t *last_keyboard_report = &(report_keyboard_t){};


void host_set_driver(host_driver_t *d)
{
    driver = d;
}

host_driver_t *host_get_driver(void)
{
    return driver;
}

uint8_t host_keyboard_leds(void)
{
    if (!driver) return 0;
    return (*driver->keyboard_leds)();
}

#ifndef NO_PRINT
static void print_changed_bits (uint8_t index, uint8_t byte, uint8_t last)
{
    uint8_t new = byte ^ last;
    if (!new) return;
    uint8_t mask = 1, bit = 0;
    for (; mask; mask<<=1, bit++) {
        if (new & mask) {
            char state = (byte & mask) ? '_' : '^';
            if (index) print_scancode (((index-1)<<3)|bit, 0, state);
            else       print_modcode  (bit, state);
        }
    }
}

void print_keyboard_report (report_keyboard_t *report, report_keyboard_t *last_keyboard_report)
{
    if (!debug_keyboard) return;
#ifndef DEBUG_KEYBOARD_VERBOSE
    if (debug_inline) print(" <");
    debug_empty_report = true;
#ifdef NKRO_ENABLE
    if (keyboard_protocol && keyboard_nkro) {
        for (uint8_t i = 0; i < KEYBOARD_REPORT_SIZE; i++) {
            uint8_t byte = report->raw[i];
            if (byte) debug_empty_report = false;
            print_changed_bits (i, byte, last_keyboard_report->raw[i]);
            last_keyboard_report->raw[i] = byte;
        }
    }
    else
#endif
    {
        uint8_t mods = report->mods;
        if (mods) debug_empty_report = false;
        print_changed_bits (0, mods, last_keyboard_report->mods);
        last_keyboard_report->mods = mods;

        for (uint8_t i = 0; i < KEYBOARD_REPORT_KEYS; i++) {
            uint8_t key = report->keys[i], last = last_keyboard_report->keys[i];
            if (key) debug_empty_report = false;
            if (key != last) {
                if (last) print_scancode (last, 0, '^');
                if (key)  print_scancode (key,  0, '_');
            }
            last_keyboard_report->keys[i] = key;
        }
    }
    if (debug_inline) {
        print(" >");
        if (debug_empty_matrix && debug_empty_report) {
            print("\r\n");
            debug_inline = false;
        }
    }
#else /* DEBUG_KEYBOARD_VERBOSE */
    dprint("keyboard: ");
    for (uint8_t i = 0; i < KEYBOARD_REPORT_SIZE; i++) {
        dprintf("%02X ", report->raw[i]);
    }
    dprint("\n");
#endif
}
#endif

/* send report */
void host_keyboard_send(report_keyboard_t *report)
{
    if (!driver) return;
    (*driver->send_keyboard)(report);
    print_keyboard_report (report, last_keyboard_report);
}

void host_mouse_send(report_mouse_t *report)
{
    if (!driver) return;
    (*driver->send_mouse)(report);
}

void host_system_send(uint16_t report)
{
    if (report == last_system_report) return;
    last_system_report = report;

    if (!driver) return;
    (*driver->send_system)(report);

#ifdef DEBUG_KEYBOARD_VERBOSE
    if (debug_keyboard) {
      dprintf("system: %04X\n", report);
    }
#endif
}

void host_consumer_send(uint16_t report)
{
    if (report == last_consumer_report) return;
    last_consumer_report = report;

    if (!driver) return;
    (*driver->send_consumer)(report);

#ifdef DEBUG_KEYBOARD_VERBOSE
    if (debug_keyboard) {
        dprintf("consumer: %04X\n", report);
    }
#endif
}

void host_set_last_keyboard_report(const report_keyboard_t *report)
{
    for (uint8_t i = 0; i < KEYBOARD_REPORT_SIZE; i++) {
        last_keyboard_report->raw[i] = report->raw[i];
    }
}

uint16_t host_last_system_report(void)
{
    return last_system_report;
}

uint16_t host_last_consumer_report(void)
{
    return last_consumer_report;
}
