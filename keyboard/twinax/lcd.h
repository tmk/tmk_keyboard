/*
 * This file is part of lcd library for ssd1306/sh1106 oled-display.
 *
 * lcd library for ssd1306/sh1106 oled-display is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or any later version.
 *
 * lcd library for ssd1306/sh1106 oled-display is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Diese Datei ist Teil von lcd library for ssd1306/sh1106 oled-display.
 *
 * lcd library for ssd1306/sh1106 oled-display ist Freie Software: Sie können es unter den Bedingungen
 * der GNU General Public License, wie von der Free Software Foundation,
 * Version 3 der Lizenz oder jeder späteren
 * veröffentlichten Version, weiterverbreiten und/oder modifizieren.
 *
 * lcd library for ssd1306/sh1106 oled-display wird in der Hoffnung, dass es nützlich sein wird, aber
 * OHNE JEDE GEWÄHRLEISTUNG, bereitgestellt; sogar ohne die implizite
 * Gewährleistung der MARKTFÄHIGKEIT oder EIGNUNG FÜR EINEN BESTIMMTEN ZWECK.
 * Siehe die GNU General Public License für weitere Details.
 *
 * Sie sollten eine Kopie der GNU General Public License zusammen mit diesem
 * Programm erhalten haben. Wenn nicht, siehe <http://www.gnu.org/licenses/>.
 *
 *  lcd.h
 *
 *  Created by Michael Köhler on 22.12.16.
 *  Copyright 2016 Skie-Systems. All rights reserved.
 *
 *  lib for OLED-Display with ssd1306/sh1106-Controller
 *  first dev-version only for I2C-Connection
 *  at ATMega328P like Arduino Uno
 *
 *  at GRAPHICMODE lib needs SRAM for display
 *  DISPLAY-WIDTH * DISPLAY-HEIGHT + 2 bytes
 */

#ifndef LCD_H
#define LCD_H

#ifdef __cplusplus
extern "C" {
#endif
    
#if (__GNUC__ * 100 + __GNUC_MINOR__) < 303
#error "This library requires AVR-GCC 3.3 or later, update to newer AVR-GCC compiler !"
#endif
    
#include <inttypes.h>
#include <avr/pgmspace.h>
#include "i2c.h"                                // library for I2C-communication
    // if you want to use other lib for I2C
    // edit i2c_xxx commands in this library
    // i2c_start(), i2c_byte(), i2c_stop()
    
    /* TODO: define displaycontroller */
#define SSD1306                                 // or SSD1306, check datasheet of your display
    /* TODO: define displaymode */
#define TEXTMODE                // TEXTMODE for only text to display,
    // GRAPHICMODE for text and graphic
    /* TODO: define font */
#define FONT            ssd1306oled_font// set font here, refer font-name at font.h/font.c
    
    /* TODO: define I2C-adress for display */
    
    // using 7-bit-adress for lcd-library
    // if you use your own library for twi check I2C-adress-handle
#define LCD_I2C_ADR         (0x78 >> 1)    // 7 bit slave-adress without r/w-bit
    // r/w-bit are set/unset by library
    // e.g. 8 bit slave-adress:
    // 0x78 = adress 0x3C with cleared r/w-bit (write-mode)
    
    
#ifndef YES
#define YES        1
#endif

#define NORMALSIZE 1
#define DOUBLESIZE 2
    
#define LCD_DISP_OFF        0xAE
#define LCD_DISP_ON        0xAF
    
#define WHITE            0x01
#define BLACK            0x00
    
#define DISPLAY_WIDTH        128
#define DISPLAY_HEIGHT        64
    
    
    
    void lcd_command(uint8_t cmd[], uint8_t size);    // transmit command to display
    void lcd_data(uint8_t data[], uint16_t size);    // transmit data to display
    void lcd_init(uint8_t dispAttr);
    void lcd_home(void);                            // set cursor to 0,0
    void lcd_invert(uint8_t invert);        // invert display
    void lcd_sleep(uint8_t sleep);            // display goto sleep (power off)
    void lcd_set_contrast(uint8_t contrast);    // set contrast for display
    void lcd_puts(const char* s);            // print string, \n-terminated, from ram on screen (TEXTMODE)
    // or buffer (GRAPHICMODE)
    void lcd_puts_p(const char* progmem_s);        // print string from flash on screen (TEXTMODE)
    // or buffer (GRAPHICMODE)
    
    void lcd_clrscr(void);                // clear screen (and buffer at GRFAICMODE)
    void lcd_gotoxy(uint8_t x, uint8_t y);        // set curser at pos x, y. x means character,
    // y means line (page, refer lcd manual)
    void lcd_putc(char c);                // print character on screen at TEXTMODE
    // at GRAPHICMODE print character to buffer
    void lcd_charMode(uint8_t mode);            // set size of chars
#if defined GRAPHICMODE
    void lcd_drawPixel(uint8_t x, uint8_t y, uint8_t color);
    void lcd_drawLine(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t color);
    void lcd_drawRect(uint8_t px1, uint8_t py1, uint8_t px2, uint8_t py2, uint8_t color);
    void lcd_fillRect(uint8_t px1, uint8_t py1, uint8_t px2, uint8_t py2, uint8_t color);
    void lcd_drawCircle(uint8_t center_x, uint8_t center_y, uint8_t radius, uint8_t color);
    void lcd_fillCircle(uint8_t center_x, uint8_t center_y, uint8_t radius, uint8_t color);
    void lcd_drawBitmap(uint8_t x, uint8_t y, const uint8_t picture[], uint8_t width, uint8_t height, uint8_t color);
    void lcd_display(void);                // copy buffer to display RAM
#endif
    
#ifdef __cplusplus
}
#endif
#endif /*  LCD_H  */
