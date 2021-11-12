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

#ifdef MOUSEKEY_ENABLE
#   include "mousekey.h"
#endif
#ifdef PS2_MOUSE_ENABLE
#   include "ps2_mouse.h"
#endif
#ifdef SERIAL_MOUSE_ENABLE
#   include "serial_mouse.h"
#endif
#ifdef ADB_MOUSE_ENABLE
#   include "adb.h"
#endif
#ifdef IBMPC_MOUSE_ENABLE
uint8_t ibmpc_mouse_buttons(void);
#endif


#if defined(NKRO_ENABLE) || defined(NKRO_6KRO_ENABLE)
bool keyboard_nkro = true;
#endif

static host_driver_t *driver;
static uint16_t last_system_report = 0;
static uint16_t last_consumer_report = 0;


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
/* send report */
void host_keyboard_send(report_keyboard_t *report)
{
    if (!driver) return;
    (*driver->send_keyboard)(report);

    if (debug_keyboard) {
        dprint("keyboard: ");
        for (uint8_t i = 0; i < KEYBOARD_REPORT_SIZE; i++) {
            dprintf("%02X ", report->raw[i]);
        }
        dprint("\n");
    }
}

void host_mouse_send(report_mouse_t *report)
{
    if (!driver) return;
#ifdef MOUSE_EXT_REPORT
    // clip and copy to Boot protocol XY
    report->boot_x = (report->x > 127) ? 127 : ((report->x < -127) ? -127 : report->x);
    report->boot_y = (report->y > 127) ? 127 : ((report->y < -127) ? -127 : report->y);
#endif

    /* Mouse buttons integration */
    uint8_t b = report->buttons;
#ifdef MOUSEKEY_ENABLE
    report->buttons |= mousekey_buttons();
#endif
#ifdef PS2_MOUSE_ENABLE
    report->buttons |= ps2_mouse_buttons();
#endif
#ifdef SERIAL_MOUSE_ENABLE
    report->buttons |= serial_mouse_buttons();
#endif
#ifdef ADB_MOUSE_ENABLE
    report->buttons |= adb_mouse_buttons();
#endif
#ifdef IBMPC_MOUSE_ENABLE
    report->buttons |= ibmpc_mouse_buttons();
#endif
    (*driver->send_mouse)(report);
    report->buttons = b;
}

void host_system_send(uint16_t report)
{
    if (report == last_system_report) return;
    last_system_report = report;

    if (!driver) return;
    (*driver->send_system)(report);

    if (debug_keyboard) {
        dprintf("system: %04X\n", report);
    }
}

void host_consumer_send(uint16_t report)
{
    if (report == last_consumer_report) return;
    last_consumer_report = report;

    if (!driver) return;
    (*driver->send_consumer)(report);

    if (debug_keyboard) {
        dprintf("consumer: %04X\n", report);
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
