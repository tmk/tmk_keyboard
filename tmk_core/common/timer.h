/*
Copyright 2011 Jun Wako <wakojun@gmail.com>

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
*/

#ifndef TIMER_H
#define TIMER_H 1

#include <stdint.h>

#if defined(__AVR__)
#include "avr/timer_avr.h"
#endif


static inline uint8_t TIMER_DIFF_8(uint8_t a, uint8_t b)
{
    return (a >= b) ?  (a - b) : (UINT8_MAX - b + a);
}
static inline uint16_t TIMER_DIFF_16(uint16_t a, uint16_t b)
{
    return (a >= b) ?  (a - b) : (UINT16_MAX - b + a);
}
static inline uint32_t TIMER_DIFF_32(uint32_t a, uint32_t b)
{
    return (a >= b) ?  (a - b) : (UINT32_MAX - b + a);
}


#ifdef __cplusplus
extern "C" {
#endif

extern volatile uint32_t timer_count;


void timer_init(void);
void timer_clear(void);
uint16_t timer_read(void);
uint32_t timer_read32(void);
uint16_t timer_elapsed(uint16_t last);
uint32_t timer_elapsed32(uint32_t last);

#ifdef __cplusplus
}
#endif

#endif
