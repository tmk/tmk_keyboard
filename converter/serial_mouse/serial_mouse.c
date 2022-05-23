/*
Copyright 2022 Jun Wako <wakojun@gmail.com>
*/

#include <stdint.h>
#include <stdbool.h>
#include "debug.h"
#include "serial_mouse.h"


void hook_early_init(void)
{
    debug_enable = true;
    debug_mouse = true;
    serial_mouse_init();
}

void hook_main_loop(void)
{
    serial_mouse_task();
}
