#include "starfield.h"

#include <stdlib.h>

#include <lcd.h>

Star starfield[MAX_STARS];

long random_range(uint8_t range)
{
  return random() % range;
}

void init_starfield() {
  for (uint8_t i = 0; i < MAX_STARS; i++) {
    starfield[i].x = random_range(DISPLAY_WIDTH);
    starfield[i].y = random_range(DISPLAY_HEIGHT);
    starfield[i].vx = random_range(3) + 1;
  }
}

void move_stars() {
  for (uint8_t i = 0; i < MAX_STARS; i++) {
    starfield[i].x -= starfield[i].vx;
    if (starfield[i].x < 0) {
      starfield[i].x = DISPLAY_WIDTH;
      starfield[i].y = random_range(DISPLAY_HEIGHT);
      starfield[i].vx = random_range(3) + 1;
    }
  }
}

void draw_stars() {
  for (uint8_t i = 0; i < MAX_STARS; i++) {
    lcd_drawPixel(starfield[i].x, starfield[i].y, WHITE);
  }
}

