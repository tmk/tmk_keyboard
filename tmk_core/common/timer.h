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


#define TIMER_DIFF(a, b, max)   ((a) >= (b) ?  (a) - (b) : (max) - (b) + (a))
#define TIMER_DIFF_8(a, b)      TIMER_DIFF(a, b, UINT8_MAX)
#define TIMER_DIFF_16(a, b)     TIMER_DIFF(a, b, UINT16_MAX)
#define TIMER_DIFF_32(a, b)     TIMER_DIFF(a, b, UINT32_MAX)
#define TIMER_DIFF_RAW(a, b)    TIMER_DIFF_8(a, b)


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

uint16_t timer_hook_interval_1ms;
uint16_t timer_hook_interval_10ms;
uint16_t timer_hook_interval_100ms;
uint16_t timer_hook_interval_1000ms;

#ifdef __cplusplus
}
#endif

#endif
