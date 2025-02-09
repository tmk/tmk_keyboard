#ifndef __GAME_STATE_H_
#define __GAME_STATE_H_

#include <avr/pgmspace.h>

enum GameState : uint8_t {
  INTRO, RUNNING, PAUSED, LOST_LIVE, DONE,
  ENTER_HS, SHOW_HS
};

void change_state(GameState new_state);
#endif

