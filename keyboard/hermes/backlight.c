#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "backlight.h"
#include "print.h"
#include "i2c_soft.h"
#include "issi.h"

struct color_t {
    uint8_t r;
    uint8_t g;
    uint8_t b;
};

struct backlight_state_t {
    struct color_t color;
    uint8_t brightness;
    uint8_t dirty;
};

static struct backlight_state_t backlight_state = { .brightness = 1 };
static uint8_t brightness_lut[BACKLIGHT_LEVELS + 1];
static const uint8_t gamma_lut[32] PROGMEM = { 0, 1, 2, 4, 6, 10, 13, 18, 22, 28, 33, 39, 46, 53, 61, 69, 78, 86, 96, 106, 116, 126, 138, 149, 161, 173, 186, 199, 212, 226, 240, 255 };

void backlight_init() {
    issi_init();

    for (uint8_t i = 0; i < BACKLIGHT_LEVELS + 1; i++) {
        brightness_lut[i] = i * 255 / BACKLIGHT_LEVELS;
    }
}

static void backlight_refresh() {
    uint8_t brightnessFactor = pgm_read_byte( &gamma_lut[brightness_lut[backlight_state.brightness] / 8] );

    uint8_t r = backlight_state.color.r * brightnessFactor / 255;
    uint8_t g = backlight_state.color.g * brightnessFactor / 255;
    uint8_t b = backlight_state.color.b * brightnessFactor / 255;

    uint8_t rData[9] = { DEVICE_ADDRESS_WRITE, 0x34, r, r, r, r, r, r, r };
    uint8_t gData[9] = { DEVICE_ADDRESS_WRITE, 0x44, g, g, g, g, g, g, g };
    uint8_t bData[9] = { DEVICE_ADDRESS_WRITE, 0x24, b, b, b, b, b, b, b };

    issi_send_data(rData, sizeof(rData), 0);
    issi_send_data(gData, sizeof(gData), 0);
    issi_send_data(bData, sizeof(bData), 0);
}

void backlight_set(uint8_t r, uint8_t g, uint8_t b) {
    if (backlight_state.color.r == r && backlight_state.color.g == g && backlight_state.color.b == b) {
        return;
    } else {
        backlight_state.color.r = r;
        backlight_state.color.g = g;
        backlight_state.color.b = b;

        backlight_refresh();
    }
}

void backlight_adjust() {
    backlight_state.brightness = (backlight_state.brightness + 1) % (BACKLIGHT_LEVELS + 1);

    backlight_refresh();
}