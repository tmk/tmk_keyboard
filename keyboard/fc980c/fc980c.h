#ifndef FC980C_H
#define FC980C_H

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
 * Row:     PD4-6, PD7(~EN)
 * Col:     PB0-3
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
    DDRB  |=  0x0F;
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
    // PD4-6
    PORTD = (PORTD & 0x8F) | ((ROW & 0x07) << 4);
}
static inline void SET_COL(uint8_t COL)
{
    // PB0-3
    PORTB = (PORTB & 0xF0) | (COL & 0x0F);
}


#ifdef UNIMAP_ENABLE
/* unimap */
#define KMAP( \
    K29,    K3A,K3B,K3C,K3D,K3E,K3F,K40,K41,K42,K43,K44,K45,                K4C,K49,K4B,K4E, \
    K35,K1E,K1F,K20,K21,K22,K23,K24,K25,K26,K27,K2D,K2E,K2A,                K53,K54,K55,K56, \
    K2B,K14,K1A,K08,K15,K17,K1C,K18,K0C,K12,K13,K2F,K30,K31,                K5F,K60,K61,K57, \
    K39,K04,K16,K07,K09,K0A,K0B,K0D,K0E,K0F,K33,K34,    K28,                K5C,K5D,K5E,     \
    K79,    K1D,K1B,K06,K19,K05,K11,K10,K36,K37,K38,    K7D,      K52,      K59,K5A,K5B,K58, \
    K78,K7B,K7A,            K2C,                K7E,K7C,K7F,  K50,K51,K4F,      K62,K63      \
) UNIMAP( \
            NO, NO, NO, NO, NO, NO, NO, NO, NO, NO, NO, NO,                                      \
    K29,    K3A,K3B,K3C,K3D,K3E,K3F,K40,K41,K42,K43,K44,K45,      NO, NO, NO,       NO, NO, NO,  \
    K35,K1E,K1F,K20,K21,K22,K23,K24,K25,K26,K27,K2D,K2E,NO, K2A,  K49,NO, K4B,  K53,K54,K55,K56, \
    K2B,K14,K1A,K08,K15,K17,K1C,K18,K0C,K12,K13,K2F,K30,    K31,  K4C,NO, K4E,  K5F,K60,K61,K57, \
    K39,K04,K16,K07,K09,K0A,K0B,K0D,K0E,K0F,K33,K34,    NO, K28,                K5C,K5D,K5E,NO,  \
    K79,NO, K1D,K1B,K06,K19,K05,K11,K10,K36,K37,K38,    NO, K7D,      K52,      K59,K5A,K5B,K58, \
    K78,K7B,K7A,NO,         K2C,        NO, NO, K7E,K7F,NO, K7C,  K50,K51,K4F,      K62,K63,NO   \
)
#elif defined(ACTIONMAP_ENABLE)
/* actionmap */
#define KMAP( \
    K0D,    K2D,K5D,K3D,K6D,K7D,K7F,K7E,K7C,K72,K71,K70,K73,		    K74,K76,K77,K75, \
    K0A,K69,K59,K58,K5B,K6F,K5F,K5E,K6C,K5C,K52,K51,K50,K55,		    K54,K56,K57,K65, \
    K6A,K39,K68,K6B,K3B,K3F,K6E,K3E,K3C,K62,K61,K60,K33,K53,		    K64,K66,K67,K25, \
    K3A,K29,K38,K28,K2B,K2F,K2E,K2C,K1C,K32,K31,K20,    K63,   		    K24,K26,K27,     \
    K2A,    K18,K08,K1B,K0B,K1F,K1E,K0E,K22,K11,K21,	K30,	  K34,	    K14,K16,K17,K15, \
    K1A,K09,K19,            K0F,                K02,K01,K10,  K00,K04,K06,      K07,K05      \
) { \
    { AC_##K00, AC_##K01, AC_##K02, AC_NO,    AC_##K04, AC_##K05, AC_##K06, AC_##K07,   \
      AC_##K08, AC_##K09, AC_##K0A, AC_##K0B, AC_NO,    AC_##K0D, AC_##K0E, AC_##K0F }, \
    { AC_##K10, AC_##K11, AC_NO,    AC_NO,    AC_##K14, AC_##K15, AC_##K16, AC_##K17,   \
      AC_##K18, AC_##K19, AC_##K1A, AC_##K1B, AC_##K1C, AC_NO,    AC_##K1E, AC_##K1F }, \
    { AC_##K20, AC_##K21, AC_##K22, AC_NO,    AC_##K24, AC_##K25, AC_##K26, AC_##K27,   \
      AC_##K28, AC_##K29, AC_##K2A, AC_##K2B, AC_##K2C, AC_##K2D, AC_##K2E, AC_##K2F }, \
    { AC_##K30, AC_##K31, AC_##K32, AC_##K33, AC_##K34, AC_NO,    AC_NO,    AC_NO,      \
      AC_##K38, AC_##K39, AC_##K3A, AC_##K3B, AC_##K3C, AC_##K3D, AC_##K3E, AC_##K3F }, \
    { AC_NO,    AC_NO,    AC_NO,    AC_NO,    AC_NO,    AC_NO,    AC_NO,    AC_NO,      \
      AC_NO,    AC_NO,    AC_NO,    AC_NO,    AC_NO,    AC_NO,    AC_NO,    AC_NO    }, \
    { AC_##K50, AC_##K51, AC_##K52, AC_##K53, AC_##K54, AC_##K55, AC_##K56, AC_##K57,   \
      AC_##K58, AC_##K59, AC_NO,    AC_##K5B, AC_##K5C, AC_##K5D, AC_##K5E, AC_##K5F }, \
    { AC_##K60, AC_##K61, AC_##K62, AC_##K63, AC_##K64, AC_##K65, AC_##K66, AC_##K67,   \
      AC_##K68, AC_##K69, AC_##K6A, AC_##K6B, AC_##K6C, AC_##K6D, AC_##K6E, AC_##K6F }, \
    { AC_##K70, AC_##K71, AC_##K72, AC_##K73, AC_##K74, AC_##K75, AC_##K76, AC_##K77,   \
      AC_NO,    AC_NO,    AC_NO,    AC_NO,    AC_##K7C, AC_##K7D, AC_##K7E, AC_##K7F }  \
}
#else
/* keymap */
#define KMAP( \
    K0D,    K2D,K5D,K3D,K6D,K7D,K7F,K7E,K7C,K72,K71,K70,K73,		    K74,K76,K77,K75, \
    K0A,K69,K59,K58,K5B,K6F,K5F,K5E,K6C,K5C,K52,K51,K50,K55,		    K54,K56,K57,K65, \
    K6A,K39,K68,K6B,K3B,K3F,K6E,K3E,K3C,K62,K61,K60,K33,K53,		    K64,K66,K67,K25, \
    K3A,K29,K38,K28,K2B,K2F,K2E,K2C,K1C,K32,K31,K20,    K63,   		    K24,K26,K27,     \
    K2A,    K18,K08,K1B,K0B,K1F,K1E,K0E,K22,K11,K21,	K30,	  K34,	    K14,K16,K17,K15, \
    K1A,K09,K19,            K0F,                K02,K01,K10,  K00,K04,K06,      K07,K05      \
) { \
    { KC_##K00, KC_##K01, KC_##K02, KC_NO,    KC_##K04, KC_##K05, KC_##K06, KC_##K07,   \
      KC_##K08, KC_##K09, KC_##K0A, KC_##K0B, KC_NO,    KC_##K0D, KC_##K0E, KC_##K0F }, \
    { KC_##K10, KC_##K11, KC_NO,    KC_NO,    KC_##K14, KC_##K15, KC_##K16, KC_##K17,   \
      KC_##K18, KC_##K19, KC_##K1A, KC_##K1B, KC_##K1C, KC_NO,    KC_##K1E, KC_##K1F }, \
    { KC_##K20, KC_##K21, KC_##K22, KC_NO,    KC_##K24, KC_##K25, KC_##K26, KC_##K27,   \
      KC_##K28, KC_##K29, KC_##K2A, KC_##K2B, KC_##K2C, KC_##K2D, KC_##K2E, KC_##K2F }, \
    { KC_##K30, KC_##K31, KC_##K32, KC_##K33, KC_##K34, KC_NO,    KC_NO,    KC_NO,      \
      KC_##K38, KC_##K39, KC_##K3A, KC_##K3B, KC_##K3C, KC_##K3D, KC_##K3E, KC_##K3F }, \
    { KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,      \
      KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO    }, \
    { KC_##K50, KC_##K51, KC_##K52, KC_##K53, KC_##K54, KC_##K55, KC_##K56, KC_##K57,   \
      KC_##K58, KC_##K59, KC_NO,    KC_##K5B, KC_##K5C, KC_##K5D, KC_##K5E, KC_##K5F }, \
    { KC_##K60, KC_##K61, KC_##K62, KC_##K63, KC_##K64, KC_##K65, KC_##K66, KC_##K67,   \
      KC_##K68, KC_##K69, KC_##K6A, KC_##K6B, KC_##K6C, KC_##K6D, KC_##K6E, KC_##K6F }, \
    { KC_##K70, KC_##K71, KC_##K72, KC_##K73, KC_##K74, KC_##K75, KC_##K76, KC_##K77,   \
      KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_##K7C, KC_##K7D, KC_##K7E, KC_##K7F }  \
}
#endif

#endif
