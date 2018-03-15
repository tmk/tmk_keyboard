#include <avr/io.h>
#include "lufa.h"
#include "print.h"
#include "sendchar.h"
#include "rn42.h"
#include "suart.h"
#include "suspend.h"

static int8_t sendchar_func(uint8_t c)
{
    xmit(c);        // SUART
    sendchar(c);    // LUFA
    return 0;
}

static host_driver_configuration_t driver_configuration = {
    .num_drivers = 2,
	.connection_timeout = 4000,
	.connection_delay = 50,
	.try_connect_all = true,
    .drivers = {
        &rn42_driver,
        &lufa_driver,
    }
};

host_driver_configuration_t* hook_get_driver_configuration(void) {
    return &driver_configuration;
}

void hook_early_init(void) {
    print_set_sendchar(sendchar_func);
    // SUART PD0:output, PD1:input
    DDRD |= (1<<0);
    PORTD |= (1<<0);
    DDRD &= ~(1<<1);
    PORTD |= (1<<1);
}

void hook_usb_suspend_loop(void) {
	// Is it really needed to call this multiple times?
	suspend_power_down();
	suspend_power_down();
	suspend_power_down();
	suspend_power_down();
	suspend_power_down();
	suspend_power_down();
	suspend_power_down();
}
