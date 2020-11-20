/*******************************************************************************
 * Console SUART
 ******************************************************************************/
#include <stdio.h>
#include "avr/suart.h"

#include "lufa.h"
#include "descriptor.h"
#include "console.h"


static int stdio_putchar(char c, FILE *stream)
{
    xmit(c);
    return 0;
}

static int stdio_getchar(FILE *stream)
{
    return _FDEV_EOF;
}

void console_init(void)
{
    // <stdio.h> stream
    static FILE stdio = (FILE)FDEV_SETUP_STREAM(stdio_putchar, stdio_getchar, _FDEV_SETUP_RW);
    stdin = &stdio;
    stdout = &stdio;

    // Setup xprintf
    xdev_out(uart_putchar);
}

bool console_configure(void)
{
}

void console_control_request(void)
{
}

bool console_is_ready(void)
{
    return true;
}

void console_task(void)
{
}
