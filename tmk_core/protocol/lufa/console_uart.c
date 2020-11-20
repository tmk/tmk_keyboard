/*******************************************************************************
 * Console UART
 ******************************************************************************/
#include <stdio.h>
#include "uart.h"

#include "lufa.h"
#include "descriptor.h"
#include "console.h"


static int stdio_putchar(char c, FILE *stream)
{
    uart_putchar(c);
    return 0;
}

static int stdio_getchar(FILE *stream)
{
    if (uart_available())
        return uart_getchar();
    return _FDEV_EOF;
}

void console_init(void)
{
    uart_init(115200);

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
