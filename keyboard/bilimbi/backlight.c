#include <avr/io.h>
#include <util/delay.h>
#include "backlight.h"

uint8_t led_counter = 0;
uint8_t led_level = 0;
uint8_t led_col = 0;

void bl_set()
{
	if (led_counter > 7)
	{
		led_counter = 0;
	}


	for (led_col = 0;led_col < 5;led_col++)
	{
		switch (led_col) {
		case 0:
			if (led_counter < led_level)
			{
				PORTF |= (1 << 4);
				PORTF &= ~(1 << 5 | 1 << 6 | 1 << 7);
				for(uint8_t i = 0;i < led_level;i++)
				{
					_delay_us(3);
				}
			}
			break;
		case 1:
			if (led_counter < led_level)
			{
				PORTF |= (1 << 5);
				PORTF &= ~(1 << 4 | 1 << 6 | 1 << 7);
				for(uint8_t i = 0;i < led_level;i++)
				{
					_delay_us(3);
				}
			}
			break;
		case 2:

			if (led_counter < led_level)
			{
				PORTF |= (1 << 6);
				PORTF &= ~(1 << 4 | 1 << 5 | 1 << 7);
				for(uint8_t i = 0;i < led_level;i++)
				{
					_delay_us(3);
				}
			}
			break;
		case 3:

			if (led_counter < led_level)
			{
				PORTF |= (1 << 7);
				PORTF &= ~(1 << 4 | 1 << 5 | 1 << 6);
				for(uint8_t i = 0;i < led_level;i++)
				{
					_delay_us(3);
				}
			}
			break;
		case 4:
			PORTF &= ~(1 << 4 | 1 << 5 | 1 << 6 | 1 << 7);
			break;
		}
	}
	led_counter++;
}

void backlight_init_ports()
{
	DDRB |= (1 << 1 | 1 << 2 | 1 << 3);
	PORTB &= ~(1 << 1 | 1 << 2 | 1 << 3);

	DDRF |= (1 << 4 | 1 << 5 | 1 << 6 | 1 << 7);
	PORTF |= (1 << 4 | 1 << 5 | 1 << 6 | 1 << 7);

	backlight_init();
}

void backlight_set(uint8_t level)
{
	led_level = level;
}