#include "hook.h"
#include "print.h"
#include "timer.h"

void hook_main_loop(void)
{
    static uint16_t last_time = 0;
    uint16_t t = timer_read();
    if (t != last_time && (t % 1000) == 0) xprintf(".");
    last_time = t;
}
