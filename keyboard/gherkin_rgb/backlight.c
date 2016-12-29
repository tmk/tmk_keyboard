// http://blog.saikoled.com/post/43165849837/secret-konami-cheat-code-to-high-resolution-pwm-on
#include <avr/io.h>
#include "backlight.h"

#define FET OCR1A

uint8_t led_counter = 0;
uint8_t led_level = 0;


void backlight_init_ports()
{
    DDRB |= (1<<5);
    PORTB &= ~(1<<5);

    TCCR1A = _BV(COM1A1) | _BV(WGM11); // = 0b10000010;
    TCCR1B = _BV(WGM13) | _BV(WGM12) | _BV(CS10); // = 0b00011001;

    ICR1 = 0xFFFF;

    FET = 0x0000;

    backlight_init();
}

void backlight_set(uint8_t level)
{
    if ( level == 0 )
    {
        // Turn off PWM control on PB5, revert to output low.
        TCCR1A &= ~(_BV(COM1A1));
        FET = 0x0000;
        // Prevent backlight blink on lowest level
        PORTB &= ~(_BV(PORTB5));
    }
    else if ( level == BACKLIGHT_LEVELS )
    {
        // Prevent backlight blink on lowest level
        PORTB &= ~(_BV(PORTB5));
        // Turn on PWM control of PB5
        TCCR1A |= _BV(COM1A1);
        // Set the brightness
        FET = 0xFFFF;
    }
    else        
    {
        // Prevent backlight blink on lowest level
        PORTB &= ~(_BV(PORTB5));
        // Turn on PWM control of PB5
        TCCR1A |= _BV(COM1A1);
        // Set the brightness
        FET = 0xFFFF >> ((BACKLIGHT_LEVELS - level) * ((BACKLIGHT_LEVELS + 1) / 2));
    }
}