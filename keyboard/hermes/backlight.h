#ifndef BACKLIGHT_H
#define BACKLIGHT_H

void backlight_init(void);

void backlight_set(uint8_t, uint8_t, uint8_t);

void backlight_adjust(void);
#endif