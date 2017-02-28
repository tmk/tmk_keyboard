#ifndef FC660C_H
#define FC660C_H

#include <stdint.h>
#include <stdbool.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>


// Timer resolution check
#if (1000000/TIMER_RAW_FREQ > 20)
#   error "Timer resolution(>20us) is not enough for HHKB matrix scan tweak on V-USB."
#endif


/*
 * HHKB Matrix I/O
 *
 * row:     HC4051[A,B,C]  selects scan row0-7
 * row-ext: [En0,En1] row extention for JP
 * col:     LS145[A,B,C,D] selects scan col0-7 and enable(D)
 * key:     on: 0/off: 1
 * prev:    hysteresis control: assert(1) when previous key state is on
 */


/*
 * Pin configuration for ATMega32U4
 *
 * Row:     PD4-6, 7(~EN)
 * Col:     PB0-2, 3(Z5 ~EN), 4(Z4 ~EN)
 * Key:     PC6(pull-uped)
 * Hys:     PC7
 */
static inline void KEY_ENABLE(void) { (PORTD &= ~(1<<7)); }
static inline void KEY_UNABLE(void) { (PORTD |=  (1<<7)); }
static inline bool KEY_STATE(void) { return (PINC & (1<<6)); }
static inline void KEY_HYS_ON(void) { (PORTC |=  (1<<7)); }
static inline void KEY_HYS_OFF(void) { (PORTC &= ~(1<<7)); }
static inline void KEY_INIT(void)
{
    /* Col */
    DDRB  |=  0x1F;
    /* Key: input with pull-up */
    DDRC  &= ~(1<<6);
    PORTC |=  (1<<6);
    /* Hys */
    DDRC  |=  (1<<7);
    /* Row */
    DDRD  |=  0xF0;

    KEY_UNABLE();
    KEY_HYS_OFF();
}
static inline void SET_ROW(uint8_t ROW)
{
    // set row with unabling key
    PORTD = (PORTD & 0x0F) | (1<<7) | ((ROW & 0x07) << 4);
}
static inline void SET_COL(uint8_t COL)
{
    //         |PB3(Z5 ~EN)|PB4(Z4 ~EN)
    // --------|-----------|-----------
    // Col:0-7 |high       |low
    // Col:8-F |low        |high
    PORTB = (PORTB & 0xE0) | ((COL & 0x08) ? 1<<4 : 1<<3) | (COL & 0x07);
}

#endif
