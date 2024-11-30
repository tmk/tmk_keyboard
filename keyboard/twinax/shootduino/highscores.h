#ifndef __HIGHSCORES_H_
#define __HIGHSCORES_H_

#include <avr/pgmspace.h>

extern const char INITIALS_LETTERS[] PROGMEM;

const uint16_t HIGHSCORE_ADDR = 0;
const uint8_t HIGHSCORE_MARKER = 42;
const uint8_t LEN_INITIALS = 3;
const uint8_t MAX_HIGHSCORES = 3;

const uint16_t HS_CONTROL_DELAY = 70;
const uint8_t LEN_HIGHSCORE_ENTRY = 9;

struct HighScoreEntry {
  char initials[LEN_INITIALS];
  uint16_t score;
};

void init_highscores();
void get_entry(uint8_t index, HighScoreEntry& entry);
void set_entry(uint8_t index, const HighScoreEntry& entry);
void insert_entry(const HighScoreEntry& entry);
int8_t get_highscore_index(uint16_t score);
void show_highscore_entry(uint8_t y, HighScoreEntry entry);
void init_highscore_entry(uint16_t score);
void handle_highscore_controls();
void copy_initials_letters();
uint8_t score_entry_xpos();
void show_highscore_display();
#endif

