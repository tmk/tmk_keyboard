#include <stdbool.h>
#include <avr/io.h>
#include <util/delay.h>

/* Check port settings for clock and data line */
#if !(defined(XT_CLOCK_PORT) && \
      defined(XT_CLOCK_PIN) && \
      defined(XT_CLOCK_DDR) && \
      defined(XT_CLOCK_BIT))
#   error "XT clock port setting is required in config.h"
#endif

#if !(defined(XT_DATA_PORT) && \
      defined(XT_DATA_PIN) && \
      defined(XT_DATA_DDR) && \
      defined(XT_DATA_BIT))
#   error "XT data port setting is required in config.h"
#endif

bool clock_in(void)
{
    XT_CLOCK_DDR  &= ~(1<<XT_CLOCK_BIT);
    XT_CLOCK_PORT |=  (1<<XT_CLOCK_BIT);
    _delay_us(1);
    return XT_CLOCK_PIN&(1<<XT_CLOCK_BIT);
}

bool data_in(void)
{
    XT_DATA_DDR  &= ~(1<<XT_DATA_BIT);
    XT_DATA_PORT |=  (1<<XT_DATA_BIT);
    _delay_us(1);
    return XT_DATA_PIN&(1<<XT_DATA_BIT);
}
