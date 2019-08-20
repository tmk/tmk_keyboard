#ifndef __GAME_OBJECTS_H_
#define __GAME_OBJECTS_H_

#include <avr/pgmspace.h>

const uint8_t MAX_ASTEROIDS = 8;
const uint8_t MAX_BULLETS = 3;

enum GameObjectType : uint8_t {
  PLAYER, ASTEROID, BULLET, EXPLOSION
};

struct GameObject {
  int16_t x;
  int8_t y, vx, vy;
  bool is_active;
  GameObjectType type;
  uint8_t anim_frame;
  uint8_t frame_count;
};

extern GameObject bullets[MAX_BULLETS];
extern GameObject asteroids[MAX_ASTEROIDS];
extern GameObject player;

void init_objects(GameObject* objects, uint8_t max_objects);
void draw_objects(GameObject* objects, uint8_t max_objects);
void move_objects(GameObject* objects, uint8_t max_objects);
#endif

