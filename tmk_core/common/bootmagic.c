#include <stdint.h>
#include <stdbool.h>
#include "wait.h"
#include "matrix.h"
#include "bootloader.h"
#include "debug.h"
#include "keymap.h"
#include "actionmap.h"
#include "host.h"
#include "action_layer.h"
#include "eeconfig.h"
#include "bootmagic.h"
#include "hook.h"

keymap_config_t keymap_config;

void bootmagic(void)
{
    /* check signature */
    if (!eeconfig_is_enabled()) {
        eeconfig_init();
    }

    /* do scans in case of bounce */
    print("bootmagic scan: ... ");
    uint8_t scan = 100;
    while (scan--) { matrix_scan(); wait_ms(10); }
    print("done.\n");

    /* bootmagic skip */
    if (bootmagic_scan_key(BOOTMAGIC_KEY_SKIP)) {
        return;
    }

    /* eeconfig clear */
    if (bootmagic_scan_key(BOOTMAGIC_KEY_EEPROM_CLEAR)) {
        eeconfig_disable();
    }

    /* bootloader */
    if (bootmagic_scan_key(BOOTMAGIC_KEY_BOOTLOADER)) {
        bootloader_jump();
    }

    /* user-defined checks */
    hook_bootmagic();

    if (!eeconfig_is_enabled()) {
        goto WAIT;
    }

    /* debug enable */
    debug_config.raw = eeconfig_read_debug();
    if (bootmagic_scan_key(BOOTMAGIC_KEY_DEBUG_ENABLE)) {
        if (bootmagic_scan_key(BOOTMAGIC_KEY_DEBUG_MATRIX)) {
            debug_config.matrix = !debug_config.matrix;
        } else if (bootmagic_scan_key(BOOTMAGIC_KEY_DEBUG_KEYBOARD)) {
            debug_config.keyboard = !debug_config.keyboard;
        } else if (bootmagic_scan_key(BOOTMAGIC_KEY_DEBUG_MOUSE)) {
            debug_config.mouse = !debug_config.mouse;
        } else {
            debug_config.enable = !debug_config.enable;
        }
    }
    eeconfig_write_debug(debug_config.raw);

    /* keymap config */
    keymap_config_t kc;
    kc.raw = eeconfig_read_keymap();
    if (bootmagic_scan_key(BOOTMAGIC_KEY_SWAP_CONTROL_CAPSLOCK)) {
        kc.swap_control_capslock = !kc.swap_control_capslock;
    }
    if (bootmagic_scan_key(BOOTMAGIC_KEY_CAPSLOCK_TO_CONTROL)) {
        kc.capslock_to_control = !kc.capslock_to_control;
    }
    if (bootmagic_scan_key(BOOTMAGIC_KEY_SWAP_LALT_LGUI)) {
        kc.swap_lalt_lgui = !kc.swap_lalt_lgui;
    }
    if (bootmagic_scan_key(BOOTMAGIC_KEY_SWAP_RALT_RGUI)) {
        kc.swap_ralt_rgui = !kc.swap_ralt_rgui;
    }
    if (bootmagic_scan_key(BOOTMAGIC_KEY_NO_GUI)) {
        kc.no_gui = !kc.no_gui;
    }
    if (bootmagic_scan_key(BOOTMAGIC_KEY_SWAP_GRAVE_ESC)) {
        kc.swap_grave_esc = !kc.swap_grave_esc;
    }
    if (bootmagic_scan_key(BOOTMAGIC_KEY_SWAP_BACKSLASH_BACKSPACE)) {
        kc.swap_backslash_backspace = !kc.swap_backslash_backspace;
    }
    if (bootmagic_scan_key(BOOTMAGIC_HOST_NKRO)) {
        kc.nkro = !kc.nkro;
    }
    eeconfig_write_keymap(kc.raw);

#if defined(NKRO_ENABLE) || defined(NKRO_6KRO_ENABLE)
    if (kc.nkro) {
        keyboard_nkro = !keyboard_nkro;
    }
#endif

    /* default layer */
    uint8_t default_layer = 0;
    if (bootmagic_scan_key(BOOTMAGIC_KEY_DEFAULT_LAYER_0)) { default_layer |= (1<<0); }
    if (bootmagic_scan_key(BOOTMAGIC_KEY_DEFAULT_LAYER_1)) { default_layer |= (1<<1); }
    if (bootmagic_scan_key(BOOTMAGIC_KEY_DEFAULT_LAYER_2)) { default_layer |= (1<<2); }
    if (bootmagic_scan_key(BOOTMAGIC_KEY_DEFAULT_LAYER_3)) { default_layer |= (1<<3); }
    if (bootmagic_scan_key(BOOTMAGIC_KEY_DEFAULT_LAYER_4)) { default_layer |= (1<<4); }
    if (bootmagic_scan_key(BOOTMAGIC_KEY_DEFAULT_LAYER_5)) { default_layer |= (1<<5); }
    if (bootmagic_scan_key(BOOTMAGIC_KEY_DEFAULT_LAYER_6)) { default_layer |= (1<<6); }
    if (bootmagic_scan_key(BOOTMAGIC_KEY_DEFAULT_LAYER_7)) { default_layer |= (1<<7); }
    if (default_layer) {
        eeconfig_write_default_layer(default_layer);
        default_layer_set((uint32_t)default_layer);
    } else {
        default_layer = eeconfig_read_default_layer();
        default_layer_set((uint32_t)default_layer);
    }

    keymap_config = kc;

    // wait until all key is released
WAIT:
    matrix_scan();
    for (uint8_t r = 0; r < MATRIX_ROWS; r++) {
        if (matrix_get_row(r)) {
            goto WAIT;
            wait_ms(10);
        }
    }
}

static bool scan_key(uint16_t code)
{
    for (uint8_t r = 0; r < MATRIX_ROWS; r++) {
        matrix_row_t matrix_row = matrix_get_row(r);
        for (uint8_t c = 0; c < MATRIX_COLS; c++) {
            if (matrix_row & ((matrix_row_t)1<<c)) {
                // read key from Layer 0
                action_t action = action_for_key(0, (keypos_t){ .row = r, .col = c });
                if (action.code == code ||
                    ((action.kind.id == ACT_LMODS ||
                      action.kind.id == ACT_RMODS ||
                      action.kind.id == ACT_LMODS_TAP ||
                      action.kind.id == ACT_RMODS_TAP ||
                      action.kind.id == ACT_LAYER_TAP ||
                      action.kind.id == ACT_LAYER_TAP_EXT) &&
                     action.key.code == code)) {
                    return true;
                }
            }
        }
    }
    return false;
}

bool bootmagic_scan_key(uint16_t code)
{
    if (!scan_key(BOOTMAGIC_KEY_SALT)) return false;

    return scan_key(code);
}
