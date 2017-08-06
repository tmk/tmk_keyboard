#include <avr/io.h>
#include <avr/pgmspace.h>
#include "matrix.h"
#include "led.h"
#include "wait.h"
#include "debug.h"

#define CLK_HI() (PORTD |=  (1<<0))
#define CLK_LO() (PORTD &= ~(1<<0))
#define STATE()  (PIND & (1<<1))
#define RST_HI() (PORTD |=  (1<<3))
#define RST_LO() (PORTD &= ~(1<<3))
#define SENSE()  (PIND & (1<<2))

static matrix_row_t matrix[8] = {};


void matrix_init(void)
{
    debug_enable = true;
    debug_keyboard = true;
    debug_matrix = true;

    // PD0: Clock. Counter couts up at falling edge.
    // PD1: Key State. Hi if selected key is activated.
    // PD2: Sense. Lo if any key is activated while Reset is Hi.
    // PD3: Reset. Resets counters at riging edge.
    DDRD  |=   (1<<3) | (1<<0);     // output
    DDRD  &= ~((1<<2) | (1<<1));    // input
    PORTD &= ~((1<<3) | (1<<0));    // low
    PORTD |=   (1<<2) | (1<<1);     // pull-up

    dprintf("init\n");
}

uint8_t matrix_scan(void)
{
    // TODO: debouce & unplug detect
    // Reset counters
    RST_HI();
    wait_us(10);
    RST_LO();
    wait_us(10);

    // 8x8 matrix:  row:sense, col:drive, key_on:hi
    for (uint8_t col = 0; col < 8; col++) {
        for (uint8_t row = 0; row < 8; row++) {
            CLK_HI();
            wait_us(10);

            if (STATE()) {
                matrix[row] |=  (1<<col);
            } else {
                matrix[row] &= ~(1<<col);
            }
            
            // proceed counter - next row
            CLK_LO();
            wait_us(10);
        }
    }
    return 1;
}

matrix_row_t matrix_get_row(uint8_t row)
{
    return matrix[row];
}

void led_set(uint8_t usb_led)
{
}
