#include "hook.h"
#include "print.h"
#include "timer.h"
#include <avr/eeprom.h>

static void eeprom_show(void)
{
    xprintf("eeprom:");
    for (int i = 0; i < E2END + 1; i++) {
        if ((i % 16) == 0) { xprintf("\n%04X: ", i); }
        xprintf("%02X ", eeprom_read_byte((uint8_t *)i));
    }
    xprintf("\n");
}

static void eeprom_wipe(void)
{
    xprintf("eeprom wipe: ");
    for (int i = 0; i < E2END + 1; i++) {
        if ((i % 16) == 0) { xprintf("%04X: ", i); }
        eeprom_write_byte((uint8_t *)i, 0xFF);
    }
    xprintf("\n");
}

void hook_late_init(void)
{
    eeprom_show();
    xprintf("\n");
    eeprom_wipe();
    xprintf("\n");
    eeprom_show();
}

void hook_main_loop(void)
{
    static uint16_t last_time = 0;
    uint16_t t = timer_read();
    if (t != last_time && (t % 1000) == 0) xprintf(".");
    last_time = t;
}
