#ifndef FC660C_H
#define FC660C_H

#include <stdint.h>
#include <stdbool.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "timer.h"
#include "unimap.h"


// Timer resolution check
#if (1000000/TIMER_RAW_FREQ > 20)
#   error "Timer resolution(>20us) is not enough for HHKB matrix scan tweak on V-USB."
#endif


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


#ifdef UNIMAP_ENABLE
/* unimap */
#define KMAP( \
    K35,K1E,K1F,K20,K21,K22,K23,K24,K25,K26,K27,K2D,K2E,K2A,    K49, \
    K2B,K14,K1A,K08,K15,K17,K1C,K18,K0C,K12,K13,K2F,K30,K31,    K4C, \
    K39,K04,K16,K07,K09,K0A,K0B,K0D,K0E,K0F,K33,K34,    K28,         \
    K79,K1D,K1B,K06,K19,K05,K11,K10,K36,K37,K38,        K7D,K52,     \
    K78,K7B,K7A,            K2C,            K7E,K7C,K7F,K50,K51,K4F  \
) UNIMAP( \
            NO, NO, NO, NO, NO, NO, NO, NO, NO, NO, NO, NO,                                     \
    NO,     NO, NO, NO, NO, NO, NO, NO, NO, NO, NO, NO, NO,       NO, NO, NO,       NO, NO, NO, \
    K35,K1E,K1F,K20,K21,K22,K23,K24,K25,K26,K27,K2D,K2E,NO, K2A,  K49,NO, NO,   NO, NO, NO, NO, \
    K2B,K14,K1A,K08,K15,K17,K1C,K18,K0C,K12,K13,K2F,K30,    K31,  K4C,NO, NO,   NO, NO, NO, NO, \
    K39,K04,K16,K07,K09,K0A,K0B,K0D,K0E,K0F,K33,K34,    NO, K28,                NO, NO, NO, NO, \
    K79,NO, K1D,K1B,K06,K19,K05,K11,K10,K36,K37,K38,    NO, K7D,      K52,      NO, NO, NO, NO, \
    K78,K7B,K7A,NO,         K2C,        NO, NO, K7E,K7F,NO, K7C,  K50,K51,K4F,  NO,     NO, NO  \
)
#else
/* actionmap */
#define KMAP( \
    K13, K10, K11, K12, K14, K16, K17, K15, K1B, K18, K19, K1A, K1C, K1E,      K1F, \
    K03, K00, K01, K02, K04, K06, K07, K05, K0B, K08, K09, K0A, K0C, K0E,      K0F, \
    K43, K40, K41, K42, K44, K46, K47, K45, K4B, K48, K49, K4A,      K4E,           \
    K33, K31, K32, K34, K36, K37, K35, K3B, K38, K39, K3A,           K3C, K3D,      \
    K23, K21, K22,                K27,                K28, K2A, K2C, K2E, K2D, K2F  \
) { \
    { AC_##K00, AC_##K01, AC_##K02, AC_##K03, AC_##K04, AC_##K05, AC_##K06, AC_##K07,   \
      AC_##K08, AC_##K09, AC_##K0A, AC_##K0B, AC_##K0C, AC_NO,    AC_##K0E, AC_##K0F }, \
    { AC_##K10, AC_##K11, AC_##K12, AC_##K13, AC_##K14, AC_##K15, AC_##K16, AC_##K17,   \
      AC_##K18, AC_##K19, AC_##K1A, AC_##K1B, AC_##K1C, AC_NO,    AC_##K1E, AC_##K1F }, \
    { AC_NO,    AC_##K21, AC_##K22, AC_##K23, AC_NO,    AC_NO,    AC_NO,    AC_##K27,   \
      AC_##K28, AC_NO,    AC_##K2A, AC_NO,    AC_##K2C, AC_##K2D, AC_##K2E, AC_##K2F }, \
    { AC_NO,    AC_##K31, AC_##K32, AC_##K33, AC_##K34, AC_##K35, AC_##K36, AC_##K37,   \
      AC_##K38, AC_##K39, AC_##K3A, AC_##K3B, AC_##K3C, AC_##K3D, AC_NO,    AC_NO    }, \
    { AC_##K40, AC_##K41, AC_##K42, AC_##K43, AC_##K44, AC_##K45, AC_##K46, AC_##K47,   \
      AC_##K48, AC_##K49, AC_##K4A, AC_##K4B, AC_NO,    AC_NO,    AC_##K4E, AC_NO    }, \
    { AC_NO,    AC_NO,    AC_NO,    AC_NO,    AC_NO,    AC_NO,    AC_NO,    AC_NO,      \
      AC_NO,    AC_NO,    AC_NO,    AC_NO,    AC_NO,    AC_NO,    AC_NO,    AC_NO    }, \
    { AC_NO,    AC_NO,    AC_NO,    AC_NO,    AC_NO,    AC_NO,    AC_NO,    AC_NO,      \
      AC_NO,    AC_NO,    AC_NO,    AC_NO,    AC_NO,    AC_NO,    AC_NO,    AC_NO    }, \
    { AC_NO,    AC_NO,    AC_NO,    AC_NO,    AC_NO,    AC_NO,    AC_NO,    AC_NO,      \
      AC_NO,    AC_NO,    AC_NO,    AC_NO,    AC_NO,    AC_NO,    AC_NO,    AC_NO    }  \
}
#endif

#endif
