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
#include <stdint.h>
#include <stdbool.h>
#include <util/delay.h>
#include "keycode.h"
#include "host.h"
#include "keymap.h"
#include "print.h"
#include "debug.h"
#include "util.h"
#include "timer.h"
#include "keyboard.h"
#include "bootloader.h"
#include "action_layer.h"
#include "eeconfig.h"
#include "sleep_led.h"
#include "led.h"
#include "command.h"

#ifdef MOUSEKEY_ENABLE
#include "mousekey.h"
#endif

#ifdef PROTOCOL_PJRC
#   include "usb_keyboard.h"
#   ifdef EXTRAKEY_ENABLE
#       include "usb_extra.h"
#   endif
#endif

#ifdef PROTOCOL_VUSB
#   include "usbdrv.h"
#endif


static bool command_common(uint8_t code);
static void command_common_help(void);
static bool command_console(uint8_t code);
static void command_console_help(void);
#ifdef MOUSEKEY_ENABLE
static bool mousekey_console(uint8_t code);
static void mousekey_console_help(void);
#endif

static uint8_t numkey2num(uint8_t code);
static void switch_default_layer(uint8_t layer);


typedef enum { ONESHOT, CONSOLE, MOUSEKEY } cmdstate_t;
static cmdstate_t state = ONESHOT;


bool command_proc(uint8_t code)
{
    switch (state) {
        case ONESHOT:
            if (!IS_COMMAND())
                return false;
            return (command_extra(code) || command_common(code));
        case CONSOLE:
            command_console(code);
            break;
#ifdef MOUSEKEY_ENABLE
        case MOUSEKEY:
            mousekey_console(code);
            break;
#endif
        default:
            state = ONESHOT;
            return false;
    }
    return true;
}

/* This allows to define extra commands. return false when not processed. */
bool command_extra(uint8_t code) __attribute__ ((weak));
bool command_extra(uint8_t code)
{
    return false;
}


/***********************************************************
 * Command common
 ***********************************************************/
static void command_common_help(void)
{
    print("\n\n----- Command Help -----\n");
    print("c:	enter console mode\n");
    print("d:	toggle debug enable\n");
    print("x:	toggle matrix debug\n");
    print("k:	toggle keyboard debug\n");
    print("m:	toggle mouse debug\n");
    print("p:	toggle print enable\n");
    print("v:	print device version & info\n");
    print("t:	print timer count\n");
    print("s:	print status\n");
    print("e:	print eeprom boot config\n");
#ifdef NKRO_ENABLE
    print("n:	toggle NKRO\n");
#endif
    print("0/F10:	switch to Layer0 \n");
    print("1/F1:	switch to Layer1 \n");
    print("2/F2:	switch to Layer2 \n");
    print("3/F3:	switch to Layer3 \n");
    print("4/F4:	switch to Layer4 \n");
    print("PScr:	power down/remote wake-up\n");
    print("Caps:	Lock Keyboard(Child Proof)\n");
    print("Paus:	jump to bootloader\n");
}

#ifdef BOOTMAGIC_ENABLE
static void print_eeprom_config(void)
{
    uint8_t eebyte;
    
    eebyte = eeconfig_read_debug();
    print("debug: "); print_hex8(eebyte); print("\n");

    eebyte = eeconfig_read_defalt_layer();
    print("defalt_layer: "); print_hex8(eebyte); print("\n");

    eebyte = eeconfig_read_keyconf();
    print("keyconf: "); print_hex8(eebyte); print("\n");

    keyconf kc;
    kc = (keyconf){ .raw = eebyte };
    print("keyconf.swap_control_capslock: "); print_hex8(kc.swap_control_capslock); print("\n");
    print("keyconf.capslock_to_control: "); print_hex8(kc.capslock_to_control); print("\n");
    print("keyconf.swap_lalt_lgui: "); print_hex8(kc.swap_lalt_lgui); print("\n");
    print("keyconf.swap_ralt_rgui: "); print_hex8(kc.swap_ralt_rgui); print("\n");
    print("keyconf.no_gui: "); print_hex8(kc.no_gui); print("\n");
    print("keyconf.swap_grave_esc: "); print_hex8(kc.swap_grave_esc); print("\n");
    print("keyconf.swap_backslash_backspace: "); print_hex8(kc.swap_backslash_backspace); print("\n");
}
#endif

static bool command_common(uint8_t code)
{
    static host_driver_t *host_driver = 0;
    switch (code) {
        case KC_Z:
            // test breathing sleep LED
            print("Sleep LED test\n");
            sleep_led_toggle();
            led_set(host_keyboard_leds());
            break;
#ifdef BOOTMAGIC_ENABLE
        case KC_E:
            print("eeprom config\n");
            print_eeprom_config();
            break;
#endif
        case KC_CAPSLOCK:
            if (host_get_driver()) {
                host_driver = host_get_driver();
                host_set_driver(0);
                print("Locked.\n");
            } else {
                host_set_driver(host_driver);
                print("Unlocked.\n");
            }
            break;
        case KC_H:
        case KC_SLASH: /* ? */
            command_common_help();
            break;
        case KC_C:
            debug_matrix   = false;
            debug_keyboard = false;
            debug_mouse    = false;
            debug_enable   = false;
            command_console_help();
            print("\nEnter Console Mode\n");
            print("C> ");
            state = CONSOLE;
            break;
        case KC_PAUSE:
            clear_keyboard();
            print("\n\nJump to bootloader... ");
            _delay_ms(1000);
            bootloader_jump(); // not return
            print("not supported.\n");
            break;
        case KC_D:
            if (debug_enable) {
                print("\nDEBUG: disabled.\n");
                debug_matrix   = false;
                debug_keyboard = false;
                debug_mouse    = false;
                debug_enable   = false;
            } else {
                print("\nDEBUG: enabled.\n");
                debug_enable   = true;
            }
            break;
        case KC_X: // debug matrix toggle
            debug_matrix = !debug_matrix;
            if (debug_matrix) {
                print("\nDEBUG: matrix enabled.\n");
                debug_enable = true;
            } else {
                print("\nDEBUG: matrix disabled.\n");
            }
            break;
        case KC_K: // debug keyboard toggle
            debug_keyboard = !debug_keyboard;
            if (debug_keyboard) {
                print("\nDEBUG: keyboard enabled.\n");
                debug_enable = true;
            } else {
                print("\nDEBUG: keyboard disabled.\n");
            }
            break;
        case KC_M: // debug mouse toggle
            debug_mouse = !debug_mouse;
            if (debug_mouse) {
                print("\nDEBUG: mouse enabled.\n");
                debug_enable = true;
            } else {
                print("\nDEBUG: mouse disabled.\n");
            }
            break;
        case KC_V: // print version & information
            print("\n\n----- Version -----\n");
            print(STR(DESCRIPTION) "\n");
            print(STR(MANUFACTURER) "(" STR(VENDOR_ID) ")/");
            print(STR(PRODUCT) "(" STR(PRODUCT_ID) ") ");
            print("VERSION: " STR(DEVICE_VER) "\n");
            break;
        case KC_T: // print timer
            print_val_hex32(timer_count);
            break;
        case KC_S:
            print("\n\n----- Status -----\n");
            print_val_hex8(host_keyboard_leds());
#ifdef PROTOCOL_PJRC
            print_val_hex8(UDCON);
            print_val_hex8(UDIEN);
            print_val_hex8(UDINT);
            print_val_hex8(usb_keyboard_leds);
            print_val_hex8(usb_keyboard_protocol);
            print_val_hex8(usb_keyboard_idle_config);
            print_val_hex8(usb_keyboard_idle_count);
#endif

#ifdef PROTOCOL_PJRC
#   if USB_COUNT_SOF
            print_val_hex8(usbSofCount);
#   endif
#endif
            break;
#ifdef NKRO_ENABLE
        case KC_N:
            clear_keyboard(); //Prevents stuck keys.
            keyboard_nkro = !keyboard_nkro;
            if (keyboard_nkro)
                print("NKRO: enabled\n");
            else
                print("NKRO: disabled\n");
            break;
#endif
#ifdef EXTRAKEY_ENABLE
        case KC_PSCREEN:
            // TODO: Power key should take this feature? otherwise any key during suspend.
#ifdef PROTOCOL_PJRC
            if (suspend && remote_wakeup) {
                usb_remote_wakeup();
            } else {
                host_system_send(SYSTEM_POWER_DOWN);
                host_system_send(0);
                _delay_ms(500);
            }
#else
            host_system_send(SYSTEM_POWER_DOWN);
            _delay_ms(100);
            host_system_send(0);
            _delay_ms(500);
#endif
            break;
#endif
        case KC_ESC:
        case KC_GRV:
        case KC_0:
            switch_default_layer(0);
            break;
        case KC_1 ... KC_9:
            switch_default_layer((code - KC_1) + 1);
            break;
        case KC_F1 ... KC_F12:
            switch_default_layer((code - KC_F1) + 1);
            break;
        default:
            print("?");
            return false;
    }
    return true;
}


/***********************************************************
 * Command console
 ***********************************************************/
static void command_console_help(void)
{
    print("\n\n----- Console Help -----\n");
    print("ESC/q:	quit\n");
#ifdef MOUSEKEY_ENABLE
    print("m:	mousekey\n");
#endif
}

static bool command_console(uint8_t code)
{
    switch (code) {
        case KC_H:
        case KC_SLASH: /* ? */
            command_console_help();
            break;
        case KC_Q:
        case KC_ESC:
            print("\nQuit Console Mode\n");
            state = ONESHOT;
            return false;
#ifdef MOUSEKEY_ENABLE
        case KC_M:
            mousekey_console_help();
            print("\nEnter Mousekey Console\n");
            print("M0>");
            state = MOUSEKEY;
            return true;
#endif
        default:
            print("?");
            return false;
    }
    print("C> ");
    return true;
}


#ifdef MOUSEKEY_ENABLE
/***********************************************************
 * Mousekey console
 ***********************************************************/
static uint8_t mousekey_param = 0;

static void mousekey_param_print(void)
{
    print("\n\n----- Mousekey Parameters -----\n");
    print("1: mk_delay(*10ms): "); pdec(mk_delay); print("\n");
    print("2: mk_interval(ms): "); pdec(mk_interval); print("\n");
    print("3: mk_max_speed: "); pdec(mk_max_speed); print("\n");
    print("4: mk_time_to_max: "); pdec(mk_time_to_max); print("\n");
    print("5: mk_wheel_max_speed: "); pdec(mk_wheel_max_speed); print("\n");
    print("6: mk_wheel_time_to_max: "); pdec(mk_wheel_time_to_max); print("\n");
}

#define PRINT_SET_VAL(v)  print(#v " = "); print_dec(v); print("\n");
static void mousekey_param_inc(uint8_t param, uint8_t inc)
{
    switch (param) {
        case 1:
            if (mk_delay + inc < UINT8_MAX)
                mk_delay += inc;
            else
                mk_delay = UINT8_MAX;
            PRINT_SET_VAL(mk_delay);
            break;
        case 2:
            if (mk_interval + inc < UINT8_MAX)
                mk_interval += inc;
            else
                mk_interval = UINT8_MAX;
            PRINT_SET_VAL(mk_interval);
            break;
        case 3:
            if (mk_max_speed + inc < UINT8_MAX)
                mk_max_speed += inc;
            else
                mk_max_speed = UINT8_MAX;
            PRINT_SET_VAL(mk_max_speed);
            break;
        case 4:
            if (mk_time_to_max + inc < UINT8_MAX)
                mk_time_to_max += inc;
            else
                mk_time_to_max = UINT8_MAX;
            PRINT_SET_VAL(mk_time_to_max);
            break;
        case 5:
            if (mk_wheel_max_speed + inc < UINT8_MAX)
                mk_wheel_max_speed += inc;
            else
                mk_wheel_max_speed = UINT8_MAX;
            PRINT_SET_VAL(mk_wheel_max_speed);
            break;
        case 6:
            if (mk_wheel_time_to_max + inc < UINT8_MAX)
                mk_wheel_time_to_max += inc;
            else
                mk_wheel_time_to_max = UINT8_MAX;
            PRINT_SET_VAL(mk_wheel_time_to_max);
            break;
    }
}

static void mousekey_param_dec(uint8_t param, uint8_t dec)
{
    switch (param) {
        case 1:
            if (mk_delay > dec)
                mk_delay -= dec;
            else
                mk_delay = 0;
            PRINT_SET_VAL(mk_delay);
            break;
        case 2:
            if (mk_interval > dec)
                mk_interval -= dec;
            else
                mk_interval = 0;
            PRINT_SET_VAL(mk_interval);
            break;
        case 3:
            if (mk_max_speed > dec)
                mk_max_speed -= dec;
            else
                mk_max_speed = 0;
            PRINT_SET_VAL(mk_max_speed);
            break;
        case 4:
            if (mk_time_to_max > dec)
                mk_time_to_max -= dec;
            else
                mk_time_to_max = 0;
            PRINT_SET_VAL(mk_time_to_max);
            break;
        case 5:
            if (mk_wheel_max_speed > dec)
                mk_wheel_max_speed -= dec;
            else
                mk_wheel_max_speed = 0;
            PRINT_SET_VAL(mk_wheel_max_speed);
            break;
        case 6:
            if (mk_wheel_time_to_max > dec)
                mk_wheel_time_to_max -= dec;
            else
                mk_wheel_time_to_max = 0;
            PRINT_SET_VAL(mk_wheel_time_to_max);
            break;
    }
}

static void mousekey_console_help(void)
{
    print("\n\n----- Mousekey Parameters Help -----\n");
    print("ESC/q:	quit\n");
    print("1:	select mk_delay(*10ms)\n");
    print("2:	select mk_interval(ms)\n");
    print("3:	select mk_max_speed\n");
    print("4:	select mk_time_to_max\n");
    print("5:	select mk_wheel_max_speed\n");
    print("6:	select mk_wheel_time_to_max\n");
    print("p:	print prameters\n");
    print("d:	set default values\n");
    print("up:	increase prameters(+1)\n");
    print("down:	decrease prameters(-1)\n");
    print("pgup:	increase prameters(+10)\n");
    print("pgdown:	decrease prameters(-10)\n");
    print("\nspeed = delta * max_speed * (repeat / time_to_max)\n");
    print("where delta: cursor="); pdec(MOUSEKEY_MOVE_DELTA);
    print(", wheel="); pdec(MOUSEKEY_WHEEL_DELTA); print("\n");
    print("See http://en.wikipedia.org/wiki/Mouse_keys\n");
}

static bool mousekey_console(uint8_t code)
{
    switch (code) {
        case KC_H:
        case KC_SLASH: /* ? */
            mousekey_console_help();
            break;
        case KC_Q:
        case KC_ESC:
            mousekey_param = 0;
            print("\nQuit Mousekey Console\n");
            print("C> ");
            state = CONSOLE;
            return false;
        case KC_P:
            mousekey_param_print();
            break;
        case KC_1:
        case KC_2:
        case KC_3:
        case KC_4:
        case KC_5:
        case KC_6:
        case KC_7:
        case KC_8:
        case KC_9:
        case KC_0:
            mousekey_param = numkey2num(code);
            print("selected parameter: "); pdec(mousekey_param); print("\n");
            break;
        case KC_UP:
            mousekey_param_inc(mousekey_param, 1);
            break;
        case KC_DOWN:
            mousekey_param_dec(mousekey_param, 1);
            break;
        case KC_PGUP:
            mousekey_param_inc(mousekey_param, 10);
            break;
        case KC_PGDN:
            mousekey_param_dec(mousekey_param, 10);
            break;
        case KC_D:
            mk_delay = MOUSEKEY_DELAY/10;
            mk_interval = MOUSEKEY_INTERVAL;
            mk_max_speed = MOUSEKEY_MAX_SPEED;
            mk_time_to_max = MOUSEKEY_TIME_TO_MAX;
            mk_wheel_max_speed = MOUSEKEY_WHEEL_MAX_SPEED;
            mk_wheel_time_to_max = MOUSEKEY_WHEEL_TIME_TO_MAX;
            print("set default values.\n");
            break;
        default:
            print("?");
            return false;
    }
    print("M"); pdec(mousekey_param); print("> ");
    return true;
}
#endif


/***********************************************************
 * Utilities
 ***********************************************************/
static uint8_t numkey2num(uint8_t code)
{
    switch (code) {
        case KC_1: return 1;
        case KC_2: return 2;
        case KC_3: return 3;
        case KC_4: return 4;
        case KC_5: return 5;
        case KC_6: return 6;
        case KC_7: return 7;
        case KC_8: return 8;
        case KC_9: return 9;
        case KC_0: return 0;
    }
    return 0;
}

static void switch_default_layer(uint8_t layer)
{
    print("switch_default_layer: "); print_dec(biton32(default_layer_state));
    print(" to "); print_dec(layer); print("\n");
    default_layer_set(layer);
    clear_keyboard();
}
