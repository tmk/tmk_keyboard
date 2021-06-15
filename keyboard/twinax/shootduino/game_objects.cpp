#include "game_objects.h"
#include "game_state.h"
#include "gfx.h"
#include "game.h"
#include <lcd.h>

extern Game shootduino;

GameObject bullets[MAX_BULLETS];
GameObject asteroids[MAX_ASTEROIDS];
GameObject player;

void init_objects(GameObject* objects, uint8_t max_objects) {
  for (uint8_t i = 0; i < max_objects; i++) {
    objects[i].is_active = false;
  }
}

void draw_objects(GameObject* objects, uint8_t max_objects) {
  const uint8_t* bmp = NULL;
  for (uint8_t i = 0; i < max_objects; i++) {
    if (objects[i].is_active) {
      switch (objects[i].type) {
        case ASTEROID:
          bmp = asteroid_anim + (2 * ASTEROID_H) * objects[i].anim_frame;
          lcd_drawBitmap(asteroids[i].x, asteroids[i].y, bmp, ASTEROID_W, ASTEROID_H, WHITE);
          if (shootduino.state == RUNNING)
            objects[i].frame_count++;
          if (objects[i].frame_count == ANIM_FRAME_DELAY) {
            objects[i].anim_frame++;
            objects[i].anim_frame %= NUM_ASTEROID_FRAMES;
            objects[i].frame_count = 0;            
          }
          break;
          
        case EXPLOSION:
          bmp = explosion_anim + (2 * EXPLOSION_H) * objects[i].anim_frame;
          lcd_drawBitmap(asteroids[i].x, asteroids[i].y, bmp, EXPLOSION_W, EXPLOSION_H, WHITE);
          objects[i].frame_count++;
          if (objects[i].frame_count == ANIM_FRAME_DELAY) {
            objects[i].frame_count = 0;            
            objects[i].anim_frame++;
            if (objects[i].anim_frame == NUM_EXPLOSION_FRAMES)
              objects[i].is_active = false;
          }
          break;
          
        case BULLET:
          lcd_drawFastHLine(objects[i].x, objects[i].y, BULLET_W, WHITE);
          break;
      }
    }
  }
}

void move_objects(GameObject* objects, uint8_t max_objects) {
  for (uint8_t i = 0; i < max_objects; i++) {
    if (objects[i].is_active) {
      objects[i].x += objects[i].vx;      
      switch (objects[i].type) {
        case ASTEROID:
          if (objects[i].x + (int)ASTEROID_W <= 0) {
            objects[i].is_active = false;
            shootduino.asteroids_missed++;
          }          
          break;
          
        case BULLET:
          if (objects[i].x >= DISPLAY_WIDTH) {
            objects[i].is_active = false;
          }
          break;
      }
    }
  }
}

