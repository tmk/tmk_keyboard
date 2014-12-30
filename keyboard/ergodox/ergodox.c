/*
Copyright 2013 Oleg Kostyuk <cub.uanic@gmail.com>

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


This work is heavily based on initial firmware for Ergodox keyboard.
Copyright (c) 2012, 2013 Ben Blazak <benblazak.dev@gmail.com>
Released under The MIT License (see "doc/licenses/MIT.md")
Project located at <https://github.com/benblazak/ergodox-firmware>

Most used files are located at
<https://github.com/benblazak/ergodox-firmware/tree/partial-rewrite/firmware/keyboard/ergodox/controller>

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
#include "ergodox.h"
#include "i2cmaster.h"

bool i2c_initialized = 0;

bool ergodox_left_led_1 = 0;  // left top
bool ergodox_left_led_2 = 0;  // left middle
bool ergodox_left_led_3 = 0;  // left bottom


void init_ergodox(void)
{
    // keyboard LEDs (see "PWM on ports OC1(A|B|C)" in "teensy-2-0.md")
    TCCR1A = 0b10101001;  // set and configure fast PWM
    TCCR1B = 0b00001001;  // set and configure fast PWM

    // (tied to Vcc for hardware convenience)
    DDRB  &= ~(1<<4);  // set B(4) as input
    PORTB &= ~(1<<4);  // set B(4) internal pull-up disabled

    // unused pins - C7, D4, D5, D7, E6
    // set as input with internal pull-ip enabled
    DDRC  &= ~(1<<7);
    DDRD  &= ~(1<<7 | 1<<5 | 1<<4);
    DDRE  &= ~(1<<6);
    PORTC |=  (1<<7);
    PORTD |=  (1<<7 | 1<<5 | 1<<4);
    PORTE |=  (1<<6);

    // blink leds
    ergodox_led_all_off();
    ergodox_led_all_set(LED_BRIGHTNESS_HI);
    ergodox_led_all_on();
    _delay_ms(333);
    ergodox_led_all_off();
}

uint8_t init_mcp23018(void) {
    uint8_t err = 0x20;

    // I2C subsystem
    if (i2c_initialized == 0) {
        i2c_init();  // on pins D(1,0)
        i2c_initialized++;
        _delay_ms(1000);
    }

    // set pin direction
    // - unused  : input  : 1
    // - input   : input  : 1
    // - driving : output : 0
    err = i2c_start(I2C_ADDR_WRITE);    if (err) goto out;
    err = i2c_write(IODIRA);            if (err) goto out;
    err = i2c_write(0b00000000);        if (err) goto out;
    err = i2c_write(0b00111111);        if (err) goto out;
    i2c_stop();
    // set pull-up
    // - unused  : on  : 1
    // - input   : on  : 1
    // - driving : off : 0
    err = i2c_start(I2C_ADDR_WRITE);    if (err) goto out;
    err = i2c_write(GPPUA);             if (err) goto out;
    err = i2c_write(0b00000000);        if (err) goto out;
    err = i2c_write(0b00111111);        if (err) goto out;
    i2c_stop();

    // set logical value (doesn't matter on inputs)
    // - unused  : hi-Z : 1
    // - input   : hi-Z : 1
    // - driving : hi-Z : 1
    err = i2c_start(I2C_ADDR_WRITE);    if (err) goto out;
    err = i2c_write(OLATA);             if (err) goto out;
    err = i2c_write(0b11111111
            & ~(ergodox_left_led_3<<LEFT_LED_3_SHIFT)
          );                            if (err) goto out;
    err = i2c_write(0b11111111
            & ~(ergodox_left_led_2<<LEFT_LED_2_SHIFT)
            & ~(ergodox_left_led_1<<LEFT_LED_1_SHIFT)
          );                            if (err) goto out;

out:
    i2c_stop();
    return err;
}

