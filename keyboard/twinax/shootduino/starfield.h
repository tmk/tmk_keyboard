#ifndef __STARFIELD_H_
#define __STARFIELD_H_

#include <avr/pgmspace.h>

const uint8_t MAX_STARS = 10;

struct Star {
  int8_t x, y, vx;
};

extern Star starfield[MAX_STARS];

void init_starfield();
void move_stars();
void draw_stars();

long random_range(uint8_t range);

#endif

