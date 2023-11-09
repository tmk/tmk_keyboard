/* 
  2023 Based on:
  https://github.com/arduino/ArduinoCore-avr/blob/a7edf4d66f66d42ebb7e166762ac7c11cb299a3f/cores/arduino/Tone.cpp

  A Tone Generator Library

  Written by Brett Hagman

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/
#include <avr/interrupt.h>

// Pin for buzzer: PD0
#define BZ_PIN_PORT     PORTD
#define BZ_PIN_DDR      DDRD
#define BZ_PIN_MASK     (1<<1)

volatile long toggle_count;

static void toneBegin(void)
{
    // Timer 1
    //TCCR1A = 0;
    //TCCR1B = 0;

    // CTC mode
    //TCCR1B |= WGM12;
    TCCR1B |= _BV(WGM12);
    // No prescaling
    //TCCR1B |= CS10;
    TCCR1B |= _BV(CS10);

    BZ_PIN_DDR |= BZ_PIN_MASK;
    BZ_PIN_PORT |= BZ_PIN_MASK;
}

// frequency (in hertz) and duration (in milliseconds).
void tone(unsigned int frequency, unsigned long duration)
{
    uint8_t prescalarbits = 0b001;
    uint32_t ocr = 0;

    toneBegin();

    // Set the pinMode as OUTPUT
    BZ_PIN_DDR |= BZ_PIN_MASK;
    
    // two choices for the 16 bit timers: ck/1 or ck/64
    ocr = F_CPU / frequency / 2 - 1;

    prescalarbits = 0b001;    // CLKio/1
    if (ocr > 0xffff) {
        ocr = F_CPU / frequency / 2 / 64 - 1;
        prescalarbits = 0b011;  // CLKio/64
    }
    TCCR1B = (TCCR1B & 0b11111000) | prescalarbits;

    // Calculate the toggle count
    if (duration > 0) {
        toggle_count = 2 * frequency * duration / 1000;
    } else {
        toggle_count = -1;
    }

    uint8_t sreg = SREG;
    cli();
    OCR1AH = (ocr >> 8) & 0xff;
    OCR1AL = ocr & 0xff;
    SREG = sreg;

    // enable interrupt
    TIMSK1 |= (1<<OCIE1A);
}

// XXX: this function only works properly for timer 2 (the only one we use
// currently).  for the others, it should end the tone, but won't restore
// proper PWM functionality for the timer.
void disableTimer(void)
{
    // disable interrupt
    TIMSK1 &= ~(1<<OCIE1A);

    TCCR1A = 0;
    TCCR1B = 0;

    uint8_t sreg = SREG;
    cli();
    OCR1AH = 0;
    OCR1AL = 0;
    SREG = sreg;
}

void noTone(void)
{
    disableTimer();
    BZ_PIN_PORT &= ~(BZ_PIN_MASK);
}

ISR(TIMER1_COMPA_vect)
{
    if (toggle_count != 0) {
        // toggle the pin
        BZ_PIN_PORT ^= BZ_PIN_MASK;

        if (toggle_count > 0)
            toggle_count--;
    } else {
        disableTimer();
        BZ_PIN_PORT &= ~(BZ_PIN_MASK);
    }
}

