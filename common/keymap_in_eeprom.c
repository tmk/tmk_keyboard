/*
Copyright 2013,2014 Kai Ryu <kai1103@gmail.com>

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

#include <avr/pgmspace.h>
#include <avr/eeprom.h>
#include "eeconfig.h"
#include "keymap_in_eeprom.h"
#include "debug.h"

#ifdef KEYMAP_IN_EEPROM_ENABLE

void keymap_in_eeprom_init(void) {
    if (!check_keymap_in_eeprom()) {
        write_keymap_to_eeprom();
    }
}

void keymap_in_eeprom_disable(void) {
    eeprom_write_word(EECONFIG_KEYMAP_CHECKSUM, eeprom_read_word(EECONFIG_KEYMAP_CHECKSUM) + 1);
}

bool check_keymap_in_eeprom(void) {
    uint16_t checksum_in_eeprom = eeprom_read_word(EECONFIG_KEYMAP_CHECKSUM);
    uint16_t checksum = EECONFIG_MAGIC_NUMBER;
    for (uint16_t i = 0; i < KEYMAP_WORD_SIZE; i++) {
        checksum += eeprom_read_word(EECONFIG_KEYMAP_FN_ACTIONS + i);
    }
#ifdef DEBUG
    eeprom_write_word(EECONFIG_KEYMAP_DEBUG, checksum);
#endif
    return (checksum_in_eeprom == checksum);
}

void write_keymap_to_eeprom(void) {
    uint16_t checksum = EECONFIG_MAGIC_NUMBER;
    const uint16_t *fn_actions = fn_actions_pointer();
    const uint8_t *keymaps = keymaps_pointer();
    // write fn_actions
    if (fn_actions != NULL) {
        uint16_t fn_actions_count_in_flash = fn_actions_count();
        for (uint16_t i = 0; i < FN_ACTIONS_COUNT; i++) {
            uint16_t fn_action = 0;
            if (i < fn_actions_count_in_flash) {
                fn_action = pgm_read_word(fn_actions + i);
            }
            eeconfig_write_keymap_fn_action(i, fn_action);
            checksum += fn_action;
        }
    }
    // write keymaps
    if (keymaps != NULL) {
        uint16_t keys_count_in_flash = keys_count();
        for (uint16_t i = 0; i < KEYS_COUNT; i++) {
            uint8_t keymap = 0;
            if (i < keys_count_in_flash) {
                keymap = pgm_read_byte(keymaps + i);
            }
            eeconfig_write_keymap_key_by_index(i, keymap);
            uint16_t keymap_word = keymap;
            if (i & 1) {
                keymap_word = keymap << 8;
            }
            checksum += keymap_word;
        }
    }
    // write checksum
    eeprom_write_word(EECONFIG_KEYMAP_CHECKSUM, checksum);
}

uint16_t eeconfig_read_keymap_fn_action(uint8_t index) {
    return eeprom_read_word(EECONFIG_KEYMAP_FN_ACTIONS + index);
}

void eeconfig_write_keymap_fn_action(uint8_t index, uint16_t fn_action) {
    return eeprom_write_word(EECONFIG_KEYMAP_FN_ACTIONS + index, fn_action);
}

uint8_t eeconfig_read_keymap_key(uint8_t layer, uint8_t row, uint8_t col) {
    return eeprom_read_byte(EECONFIG_KEYMAP_KEYMAPS + KEY_OFFSET(layer, row, col));
}

void eeconfig_write_keymap_key(uint8_t layer, uint8_t row, uint8_t col, uint8_t key) {
    return eeprom_write_byte(EECONFIG_KEYMAP_KEYMAPS + KEY_OFFSET(layer, row, col), key);
}

uint8_t eeconfig_read_keymap_key_by_index(uint16_t index) {
    return eeprom_read_byte(EECONFIG_KEYMAP_KEYMAPS + index);
}

void eeconfig_write_keymap_key_by_index(uint16_t index, uint8_t key) {
    return eeprom_write_byte(EECONFIG_KEYMAP_KEYMAPS + index, key);
}

#endif
