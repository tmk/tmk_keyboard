/*
Copyright 2014 Dave McEwan cogitocumimpune@hotmail.com

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


This work is heavily based on TMK port for Ergodox keyboard.
Copyright 2013 Oleg Kostyuk <cub.uanic@gmail.com>


This work is heavily based on initial firmware for Ergodox keyboard.
Copyright (c) 2012, 2013 Ben Blazak <benblazak.dev@gmail.com>
Released under The MIT License (see "doc/licenses/MIT.md")
Project located at <https://github.com/benblazak/ergodox-firmware>
*/

#include <stdint.h>
#include <stdbool.h>
#include <avr/io.h>

#define CPU_PRESCALE(n) (CLKPR = 0x80, CLKPR = (n))
#define CPU_16MHz       0x00

void init_mcdox(void);

#define LED_BRIGHTNESS_LO       31
#define LED_BRIGHTNESS_HI       255

inline void mcdox_board_led_on(void)      { DDRD |=  (1<<6); PORTD |=  (1<<6); }
inline void mcdox_right_led_on(void)      { DDRB |=  (1<<5); PORTB |=  (1<<5); }
inline void mcdox_left_led_on(void)       { DDRB |=  (1<<6); PORTB |=  (1<<6); }

inline void mcdox_board_led_off(void)     { DDRD &= ~(1<<6); PORTD &= ~(1<<6); }
inline void mcdox_right_led_off(void)     { DDRB &= ~(1<<5); PORTB &= ~(1<<5); }
inline void mcdox_left_led_off(void)      { DDRB &= ~(1<<6); PORTB &= ~(1<<6); }

inline void mcdox_led_all_on(void)
{
    mcdox_board_led_on();
    mcdox_right_led_on();
    mcdox_left_led_on();
}

inline void mcdox_led_all_off(void)
{
    mcdox_board_led_off();
    mcdox_right_led_off();
    mcdox_left_led_off();
}

inline void mcdox_right_led_set(uint8_t n)    { OCR1A = n; }
inline void mcdox_left_led_set(uint8_t n)     { OCR1B = n; }

inline void mcdox_led_all_set(uint8_t n)
{
    mcdox_right_led_set(n);
    mcdox_left_led_set(n);
}

