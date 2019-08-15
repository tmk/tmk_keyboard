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
}
