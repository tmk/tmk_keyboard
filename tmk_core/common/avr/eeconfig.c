#include <stdint.h>
#include <stdbool.h>
#include <avr/eeprom.h>
#include "eeconfig.h"
#include "keymap.h"
#include "print.h"

void eeconfig_init(void)
{
    eeprom_write_word(EECONFIG_MAGIC,          EECONFIG_MAGIC_NUMBER);
    eeprom_write_byte(EECONFIG_DEBUG,          0);
    eeprom_write_byte(EECONFIG_DEFAULT_LAYER,  0);
    eeprom_write_byte(EECONFIG_KEYMAP,         0);
    eeprom_write_byte(EECONFIG_MOUSEKEY_ACCEL, 0);
#ifdef BACKLIGHT_ENABLE
    eeprom_write_byte(EECONFIG_BACKLIGHT,      0);
#endif
}

void eeconfig_enable(void)
{
    eeprom_write_word(EECONFIG_MAGIC, EECONFIG_MAGIC_NUMBER);
}

void eeconfig_disable(void)
{
    eeprom_write_word(EECONFIG_MAGIC,          0xFFFF);
    eeprom_write_byte(EECONFIG_DEBUG,          0xFF);
    eeprom_write_byte(EECONFIG_DEFAULT_LAYER,  0xFF);
    eeprom_write_byte(EECONFIG_KEYMAP,         0xFF);
    eeprom_write_byte(EECONFIG_MOUSEKEY_ACCEL, 0xFF);
    eeprom_write_byte(EECONFIG_BACKLIGHT,      0xFF);
}

bool eeconfig_is_enabled(void)
{
    return (eeprom_read_word(EECONFIG_MAGIC) == EECONFIG_MAGIC_NUMBER);
}

uint8_t eeconfig_read_debug(void)      { return eeprom_read_byte(EECONFIG_DEBUG); }
void eeconfig_write_debug(uint8_t val) { eeprom_write_byte(EECONFIG_DEBUG, val); }

uint8_t eeconfig_read_default_layer(void)      { return eeprom_read_byte(EECONFIG_DEFAULT_LAYER); }
void eeconfig_write_default_layer(uint8_t val) { eeprom_write_byte(EECONFIG_DEFAULT_LAYER, val); }

uint8_t eeconfig_read_keymap(void)      { return eeprom_read_byte(EECONFIG_KEYMAP); }
void eeconfig_write_keymap(uint8_t val) { eeprom_write_byte(EECONFIG_KEYMAP, val); }

#ifdef BACKLIGHT_ENABLE
uint8_t eeconfig_read_backlight(void)      { return eeprom_read_byte(EECONFIG_BACKLIGHT); }
void eeconfig_write_backlight(uint8_t val) { eeprom_write_byte(EECONFIG_BACKLIGHT, val); }
#endif

void eeconfig_debug(void)
{
    xprintf("eeprom:");
    for (int i = 0; i < 16 /* E2END + 1 */; i++) {
        if ((i % 16) == 0) { xprintf("\n%04X: ", i); }
        xprintf("%02X ", eeprom_read_byte((uint8_t *)i));
    }
    xprintf("\n");
}
