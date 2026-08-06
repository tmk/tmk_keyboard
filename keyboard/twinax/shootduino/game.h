#ifndef __GAME_H_
#define __GAME_H_

#include <avr/pgmspace.h>


#include "game_state.h"
#include "highscores.h"

const uint8_t MAX_LIVES = 3;
const uint8_t MAX_MISSES = 5;
const uint16_t ASTEROID_DELAY = 800;
const uint16_t BULLET_DELAY = 600;
const uint16_t MIN_DELAY_AFTER_STATE_CHANGE = 300;
const uint16_t DEBOUNCE_DELAY = 200;
const uint16_t SHOW_HIGHSCORES_DELAY = 7000;
const uint8_t MAX_SCORE_LEN = 7;

struct Game {
  GameState state;
  uint16_t score;
  uint32_t ticks;
  uint32_t state_changed;
  uint32_t bullet_fired; // Timestamp of the last bullet fired by player.
  uint32_t asteroid_started; // Timestamp of the last asteroid.
  bool player_hit;
  uint8_t asteroids_missed;
  uint8_t lives;
  HighScoreEntry highscore_entry;
  bool keys_were_idle;
};
#endif

