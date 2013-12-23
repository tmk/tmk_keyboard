#include <stdbool.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "xt.h"
#include "debug.h"

#define WAIT(stat, us, err) do { \
    if (!wait_##stat(us)) { \
        goto ERROR; \
    } \
} while (0)

void xt_host_init(void) {
    inhibit();
}

uint8_t xt_host_recv(void) {
    
}

static inline void clock_lo()
{
    XT_CLOCK_PORT &= ~(1<<XT_CLOCK_BIT);
    XT_CLOCK_DDR  |=  (1<<XT_CLOCK_BIT);
}
static inline void clock_hi()
{
    /* input with pull up */
    XT_CLOCK_DDR  &= ~(1<<XT_CLOCK_BIT);
    XT_CLOCK_PORT |=  (1<<XT_CLOCK_BIT);
}
static inline bool clock_in()
{
    XT_CLOCK_DDR  &= ~(1<<XT_CLOCK_BIT);
    XT_CLOCK_PORT |=  (1<<XT_CLOCK_BIT);
    _delay_us(1);
    return XT_CLOCK_PIN&(1<<XT_CLOCK_BIT);
}
static inline void data_lo()
{
    XT_DATA_PORT &= ~(1<<XT_DATA_BIT);
    XT_DATA_DDR  |=  (1<<XT_DATA_BIT);
}
static inline void data_hi()
{
    /* input with pull up */
    XT_DATA_DDR  &= ~(1<<XT_DATA_BIT);
    XT_DATA_PORT |=  (1<<XT_DATA_BIT);
}
static inline bool data_in()
{
    XT_DATA_DDR  &= ~(1<<XT_DATA_BIT);
    XT_DATA_PORT |=  (1<<XT_DATA_BIT);
    _delay_us(1);
    return XT_DATA_PIN&(1<<XT_DATA_BIT);
}

static inline uint16_t wait_clock_lo(uint16_t us)
{
    while (clock_in()  && us) { asm(""); _delay_us(1); us--; }
    return us;
}
static inline uint16_t wait_clock_hi(uint16_t us)
{
    while (!clock_in() && us) { asm(""); _delay_us(1); us--; }
    return us;
}
static inline uint16_t wait_data_lo(uint16_t us)
{
    while (data_in() && us)  { asm(""); _delay_us(1); us--; }
    return us;
}
static inline uint16_t wait_data_hi(uint16_t us)
{
    while (!data_in() && us)  { asm(""); _delay_us(1); us--; }
    return us;
}


/* idle state that device can send */
static inline void idle(void)
{
    clock_hi();
    data_hi();
}

/* inhibit device to send */
static inline void inhibit(void)
{
    clock_lo();
    data_hi();
}


