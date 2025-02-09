/*
 * LED support for IBM Twinax keyboard(1394312, german version)
 * (C) Copyright 2019
 * Dirk Eibach <dirk.eibach@gmail.com>
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "led.h"
#include "lcd.h"

#include "debug.h"
#include "timer.h"
#include "led.h"
#include "keyboard.h"
#include "ibm_logo.h"
#include "keycode.h"
#include "keymap.h"
#include "shootduino/joystick.h"
#include "shootduino/shootduino.h"

#define SLEEP_TIMEOUT_MS (1000L * 60 * 10)
bool sleep_status = 0;
bool must_init = 1;
bool game_active = 0;
bool f24_lock = 0;
uint32_t sleep_timer;
uint8_t last_usb_led = 0;

void led_set(uint8_t usb_led)
{
    /* this keyboard has no leds, but I added an OLED */
    lcd_gotoxy(0,0);
    if (usb_led & (1<<0))
        lcd_puts("NumLock");
    else
        lcd_puts("       ");

    lcd_gotoxy(0,1);
    if (usb_led & (1<<1))
        lcd_puts("CapsLock");
    else
        lcd_puts("        ");

    lcd_gotoxy(0,2);
    if (usb_led & (1<<2))
        lcd_puts("ScrollLock");
    else
        lcd_puts("          ");

#ifdef GRAPHICMODE
        lcd_display();
#endif

    last_usb_led = usb_led;
}

void start_lcd(void)
{
    lcd_init(LCD_DISP_ON);
    lcd_gotoxy(0,6);
    lcd_puts("IBM Twinax TMK port");
    lcd_gotoxy(0,7);
    lcd_puts("(C)2019 Dirk Eibach");
    lcd_drawBitmap(64, 5, ibm_logo, 64, 26, WHITE);
}

void hook_matrix_change(keyevent_t event)
{
    if (sleep_status) {
        sleep_status = 0;
        start_lcd();
        led_set(last_usb_led);
    }
    sleep_timer = timer_read32();

    switch (keymap_key_to_keycode(0, event.key)) {
    case KC_F24:
        if (!f24_lock && event.pressed) {
            f24_lock = 1;
            if (!game_active) {
                shootduino_setup();
                game_active = 1;
            } else {
                game_active = 0;
                lcd_init(LCD_DISP_OFF);
                start_lcd();
                led_set(last_usb_led);
            }
        } else {
            f24_lock = 0;
        }
        break;
    }
}

void hook_keyboard_loop(void)
{
    if (must_init) {
        start_lcd();
        sleep_timer = timer_read32();
#ifdef GRAPHICMODE
        lcd_display();
#endif
        must_init = 0;
    }

    if (!sleep_status && (timer_elapsed32(sleep_timer) > SLEEP_TIMEOUT_MS)) {
        lcd_init(LCD_DISP_OFF);
        sleep_status = 1;
        game_active = 0;
    }

    if (game_active) {
        uint32_t loop_timer = timer_read32();
        shootduino_loop();
        xprintf("Loop %u ms\n", timer_elapsed32(loop_timer));
    }
}

void hook_usb_wakeup(void)
{
    sleep_status = 0;
    start_lcd();
    led_set(last_usb_led);
}

void hook_usb_suspend_entry(void)
{
    lcd_init(LCD_DISP_OFF);
    sleep_status = 1;
    game_active = 0;
}

void init_joystick(void) {
}

void update_joystick(void) {
}
