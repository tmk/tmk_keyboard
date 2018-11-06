#include "ch.h"

#include "timer.h"

void timer_init(void) {}

void timer_clear(void) {}

uint16_t timer_read(void)
{
    return (uint16_t)TIME_I2MS(chVTGetSystemTime());
}

uint32_t timer_read32(void)
{
    return TIME_I2MS(chVTGetSystemTime());
}

uint16_t timer_elapsed(uint16_t last)
{
    return (uint16_t)(TIME_I2MS(chVTTimeElapsedSinceX(TIME_MS2I(last))));
}

uint32_t timer_elapsed32(uint32_t last)
{
    return TIME_I2MS(chVTTimeElapsedSinceX(TIME_MS2I(last)));
}
