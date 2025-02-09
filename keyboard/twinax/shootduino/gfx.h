#ifndef __GFX_H_
#define __GFX_H_

#include <avr/pgmspace.h>

const uint8_t PLAYER_W = 13;
const uint8_t PLAYER_H = 13;
extern const uint8_t spaceship_bmp[];

const uint8_t NUM_ASTEROID_FRAMES = 3;
const uint8_t ASTEROID_W = 11;
const uint8_t ASTEROID_H = 12;
extern const uint8_t asteroid_anim[];

const uint8_t NUM_EXPLOSION_FRAMES = 4;
const uint8_t EXPLOSION_W = 10;
const uint8_t EXPLOSION_H = 10;
extern const uint8_t explosion_anim[];

const uint8_t BULLET_W = 5;
const uint8_t BULLET_H = 1;

const uint8_t ANIM_FRAME_DELAY = 4;
#endif

