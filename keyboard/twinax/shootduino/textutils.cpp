#include "textutils.h"

uint8_t center_pos(const char* str, const uint8_t text_size) {
  return (DISPLAY_WIDTH - (strlen_P(str) * text_size * BASE_FONT_W)) / 2;
}

void pmem_print(uint8_t x, uint8_t y, uint8_t size, const char* str, uint16_t color) {
  uint8_t font_width = size * BASE_FONT_W;
  char c;
  uint8_t i = 0;
//  display.setTextSize(size);
//  display.setTextColor(color);
  while (c = pgm_read_byte(str++)) {
    set_cursor(x + i++ * font_width, y);
    lcd_putc(c);
  }
}

void pmem_print_center(uint8_t y, uint8_t size, const char* str, uint16_t color) {
  pmem_print(center_pos(str, size), y, size, str, color);
}

void set_cursor(uint8_t x, uint8_t y) {
  uint8_t xt = (x + BASE_FONT_W / 2) / BASE_FONT_W;
  uint8_t yt = (y + BASE_FONT_H / 2) / BASE_FONT_H;

  lcd_gotoxy(xt, yt);
}
