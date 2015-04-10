#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include "led.h"
#include "sleep_led.h"

#ifdef HARDWARE_PWM
void sleep_led_init(void)
{
    /* Timer1 setup */
    /* PWM mode */
    TCCR1A = _BV(WGM10);
    /* PWM mode */
    TCCR1B = _BV(WGM12);
    /* Set initial compare value */
    OCR1AL = 0;
    /* Enable Overflow Interrupt */
    TIMSK1 |= _BV(TOIE1);
}

void sleep_led_enable(void)
{
    led_set(0);
    /* Clock select: clk/1024 */
    TCCR1B |= _BV(CS12) | _BV(CS10);
}

void sleep_led_disable(void)
{
    /* Clock select: disable */
    TCCR1B &= ~(_BV(CS12) | _BV(CS10));
    /* Disable output */
    TCCR1A &= ~(_BV(COM1A0) | _BV(COM1A1));
}

void sleep_led_toggle(void)
{
    /* Clock select: toggle */
    TCCR1B ^= _BV(CS12) | _BV(CS10);
    /* Disable output */
    TCCR1A &= ~(_BV(COM1A0) | _BV(COM1A1));
}

/* Breathing Sleep LED brighness(PWM On period) table */
#define M 128

static const uint8_t breathing_table[256] PROGMEM = {
M, M, M, M, M, M, M, M, M, M, M, M, M, M, M, M, M, M, M, M, M, M, M,
M, M, M, M, M, M, M, M, M, M, M, M, M, M, M, M, M, M, M, M, 0, 0, 0,
0, 1, 1, 1, 2, 2, 3, 3, 4, 4, 5, 6, 7, 8, 9, 10, 12, 13, 15, 16, 18,
20, 22, 24, 26, 29, 31, 34, 37, 40, 43, 46, 50, 53, 57, 61, 65, 69,
73, 78, 83, 87, 92, 97, 102, 107, 113, 118, 123, 129, 134, 140, 145,
151, 157, 162, 168, 173, 179, 184, 189, 194, 199, 204, 209, 213, 218,
222, 226, 230, 233, 236, 239, 242, 245, 247, 249, 251, 252, 253, 254,
254, 255, 254, 254, 253, 252, 251, 249, 247, 245, 242, 239, 236, 233,
230, 226, 222, 218, 213, 209, 204, 199, 194, 189, 184, 179, 173, 168,
162, 157, 151, 145, 140, 134, 129, 123, 118, 113, 107, 102, 97, 92,
87, 83, 78, 73, 69, 65, 61, 57, 53, 50, 46, 43, 40, 37, 34, 31, 29,
26, 24, 22, 20, 18, 16, 15, 13, 12, 10, 9, 8, 7, 6, 5, 4, 4, 3, 3, 2,
2, 1, 1, 1, 0, 0, 0, 0, M, M, M, M, M, M, M, M, M, M, M, M, M, M, M, 
M, M, M, M, M, M, M, M, M, M, M, M, M, M, M, M, M, M, M, M, M, M, M,
M, M, M, M };

ISR(TIMER1_OVF_vect)
{
    static uint8_t index = 0;
    uint8_t pwm = pgm_read_byte(&breathing_table[index++]);

    if (pwm != M) {
        OCR1AL = pwm;
        /* Enable output */
        TCCR1A |= _BV(COM1A0) | _BV(COM1A1);
    } else {
        /* Disable output */
        TCCR1A &= ~(_BV(COM1A0) | _BV(COM1A1));
    }
}
#else
/* Software PWM
 *  ______           ______           __
 * |  ON  |___OFF___|  ON  |___OFF___|   ....
 * |<-------------->|<-------------->|<- ....
 *     PWM period       PWM period
 *
 * 256              interrupts/period[resolution]
 * 64               periods/second[frequency]
 * 256*64           interrupts/second
 * F_CPU/(256*64)   clocks/interrupt
 */
#define SLEEP_LED_TIMER_TOP F_CPU/(256*64)

void sleep_led_init(void)
{
    /* Timer1 setup */
    /* CTC mode */
    TCCR1B |= _BV(WGM12);
    /* Clock selelct: clk/1 */
    TCCR1B |= _BV(CS10);
    /* Set TOP value */
    uint8_t sreg = SREG;
    cli();
    OCR1AH = (SLEEP_LED_TIMER_TOP>>8)&0xff;
    OCR1AL = SLEEP_LED_TIMER_TOP&0xff;
    SREG = sreg;
}

void sleep_led_enable(void)
{
    /* Enable Compare Match Interrupt */
    TIMSK1 |= _BV(OCIE1A);
}

void sleep_led_disable(void)
{
    /* Disable Compare Match Interrupt */
    TIMSK1 &= ~_BV(OCIE1A);
}

void sleep_led_toggle(void)
{
    /* Disable Compare Match Interrupt */
    TIMSK1 ^= _BV(OCIE1A);
}


/* Breathing Sleep LED brighness(PWM On period) table
 * (64[steps] * 4[duration]) / 64[PWM periods/s] = 4 second breath cycle
 *
 * http://www.wolframalpha.com/input/?i=%28sin%28+x%2F64*pi%29**8+*+255%2C+x%3D0+to+63
 * (0..63).each {|x| p ((sin(x/64.0*PI)**8)*255).to_i }
 */
static const uint8_t breathing_table[64] PROGMEM = {
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 4, 6, 10,
15, 23, 32, 44, 58, 74, 93, 113, 135, 157, 179, 199, 218, 233, 245, 252,
255, 252, 245, 233, 218, 199, 179, 157, 135, 113, 93, 74, 58, 44, 32, 23,
15, 10, 6, 4, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

ISR(TIMER1_COMPA_vect)
{
    /* Software PWM
     * timer:1111 1111 1111 1111
     *       \_____/\/ \_______/____  count(0-255)
     *          \    \______________  duration of step(4)
     *           \__________________  index of step table(0-63)
     */
    static union {
        uint16_t row;
        struct {
            uint8_t count:8;
            uint8_t duration:2;
            uint8_t index:6;
        } pwm;
    } timer = { .row = 0 };

    timer.row++;
    
    // LED on
    if (timer.pwm.count == 0) {
        led_set(1<<USB_LED_CAPS_LOCK);
    }
    // LED off
    if (timer.pwm.count == pgm_read_byte(&breathing_table[timer.pwm.index])) {
        led_set(0);
    }
}
#endif
