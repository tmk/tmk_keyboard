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
#include <avr/interrupt.h>
#include <util/delay.h>
#include "action.h"
#include "command.h"
#include "print.h"
#include "debug.h"
#include "mcdox.h"


void init_mcdox(void)
{
    //// keyboard LEDs (see "PWM on ports OC1(A|B|C)" in "teensy-2-0.md")
    //TCCR1A = 0b10101001;  // set and configure fast PWM
    //TCCR1B = 0b00001001;  // set and configure fast PWM

    // unused pins - B5, B6, D5, E6
    // set as input with internal pull-ip enabled
    DDRB  &= ~(1<<5 | 1<<6);
    PORTD |=  (1<<5 | 1<<6);
    DDRD  &= ~(1<<5);
    PORTD |=  (1<<5);
    DDRE  &= ~(1<<6);
    PORTE |=  (1<<6);

    // blink leds
    mcdox_led_all_off();
    mcdox_led_all_set(LED_BRIGHTNESS_HI);
    mcdox_led_all_on();
    _delay_ms(333);
    mcdox_led_all_off();
}

