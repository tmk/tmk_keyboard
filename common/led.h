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

#ifndef LED_H
#define LED_H
#include "stdint.h"


/* keyboard LEDs */
#define USB_LED_NUM_LOCK                0
#define USB_LED_CAPS_LOCK               1
#define USB_LED_SCROLL_LOCK             2
#define USB_LED_COMPOSE                 3
#define USB_LED_KANA                    4

#ifdef PHYSICAL_LEDS_ENABLE

/* CAPS_LOCK_LED_ENABLE */
#ifdef CAPS_LOCK_LED_ENABLE

#if !((defined CAPS_LOCK_LED_PORT) && \
      (defined CAPS_LOCK_LED_PIN) && \
      (defined CAPS_LOCK_LED_DDR) && \
      (defined CAPS_LOCK_LED_BIT))
#   error "CAPS_LOCK_LED is ENABLE but ports configuration is not defined in config.h" 
#endif

#define CAPS_LOCK_LED_INIT (CAPS_LOCK_LED_DDR  |= (1<<CAPS_LOCK_LED_BIT))
#define CAPS_LOCK_LED_ON   (CAPS_LOCK_LED_PORT |=  (1<<CAPS_LOCK_LED_BIT)) // Set the bit to 1
#define CAPS_LOCK_LED_OFF  (CAPS_LOCK_LED_PORT &= ~(1<<CAPS_LOCK_LED_BIT)) // Clears the bit to 0

#else

#define CAPS_LOCK_LED_INIT 
#define CAPS_LOCK_LED_ON   
#define CAPS_LOCK_LED_OFF  

#endif
/* CAPS_LOCK_LED_ENABLE */


/* NUM_LOCK_LED_ENABLE */
#ifdef NUM_LOCK_LED_ENABLE
#if !((defined NUM_LOCK_LED_PORT) && \
      (defined NUM_LOCK_LED_PIN) && \
      (defined NUM_LOCK_LED_DDR) && \
      (defined NUM_LOCK_LED_BIT))
#   error "NUM_LOCK_LED is ENABLE but ports configuration is not defined in config.h" 
#endif

#define NUM_LOCK_LED_INIT (NUM_LOCK_LED_DDR  |= (1<<NUM_LOCK_LED_BIT))
#define NUM_LOCK_LED_ON   (NUM_LOCK_LED_PORT |=  (1<<NUM_LOCK_LED_BIT)) // Set the bit to 1
#define NUM_LOCK_LED_OFF  (NUM_LOCK_LED_PORT &= ~(1<<NUM_LOCK_LED_BIT)) // Clears the bit to 0

#else

#define NUM_LOCK_LED_INIT 
#define NUM_LOCK_LED_ON   
#define NUM_LOCK_LED_OFF  

#endif
/* NUM_LOCK_LED_ENABLE */

/* SCROLL_LOCK_LED_ENABLE */
#ifdef SCROLL_LOCK_LED_ENABLE
#if !((defined SCROLL_LOCK_LED_PORT) && \
      (defined SCROLL_LOCK_LED_PIN) && \
      (defined SCROLL_LOCK_LED_DDR) && \
      (defined SCROLL_LOCK_LED_BIT))
#   error "SCROLL_LOCK_LED is ENABLE but ports configuration is not defined in config.h" 
#endif

#define SCROLL_LOCK_LED_INIT (SCROLL_LOCK_LED_DDR  |= (1<<SCROLL_LOCK_LED_BIT))
#define SCROLL_LOCK_LED_ON   (SCROLL_LOCK_LED_PORT |=  (1<<SCROLL_LOCK_LED_BIT)) // Set the bit to 1
#define SCROLL_LOCK_LED_OFF  (SCROLL_LOCK_LED_PORT &= ~(1<<SCROLL_LOCK_LED_BIT)) // Clears the bit to 0

#else

#define SCROLL_LOCK_LED_INIT
#define SCROLL_LOCK_LED_ON
#define SCROLL_LOCK_LED_OFF

#endif
/* SCROLL_LOCK_LED_ENABLE */

void physical_led_init();

#else

#define CAPS_LOCK_LED_ON   false;
#define CAPS_LOCK_LED_OFF  false;
#define NUM_LOCK_LED_ON   false;
#define NUM_LOCK_LED_OFF  false;
#define SCROLL_LOCK_LED_ON   false;
#define SCROLL_LOCK_LED_OFF  false;

#endif


void led_set(uint8_t usb_led);


#endif
