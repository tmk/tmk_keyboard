#include <avr/io.h>
#include <avr/power.h>
#include <avr/wdt.h>
#include "lufa.h"
#include "print.h"
#include "sendchar.h"
#include "matrix.h"
#include "rn42.h"
#include "rn42_task.h"
#include "serial.h"
#include "keyboard.h"
#include "keycode.h"
#include "action.h"
#include "action_util.h"
#include "wait.h"
#include "suart.h"
#include "suspend.h"

static int8_t sendchar_func(uint8_t c)
{
#ifdef LUFA_DEBUG_SUART
    xmit(c);        // SUART
#endif
    sendchar(c);    // LUFA
    return 0;
}

static void SetupHardware(void)
{
    /* Disable watchdog if enabled by bootloader/fuses */
    MCUSR &= ~(1 << WDRF);
    wdt_disable();

    /* Disable clock division */
    clock_prescale_set(clock_div_1);

    // Leonardo needs. Without this USB device is not recognized.
    USB_Disable();

    USB_Init();

    // for Console_Task
    USB_Device_EnableSOFEvents();
    print_set_sendchar(sendchar_func);

#ifdef LUFA_DEBUG_SUART
    // SUART PD0:output, PD1:input
    DDRD |= (1<<0);
    PORTD |= (1<<0);
    DDRD &= ~(1<<1);
    PORTD |= (1<<1);
#endif
}

int main(void)  __attribute__ ((weak));
int main(void)
{
    SetupHardware();
    print("\nSetupHardware\n");
    sei();

    /* wait for USB startup to get ready for debug output */
    uint8_t timeout = 255;  // timeout when USB is not available(Bluetooth)
    while (timeout-- && USB_DeviceState != DEVICE_STATE_Configured) {
        wait_ms(4);
#if defined(INTERRUPT_CONTROL_ENDPOINT)
        ;
#else
        USB_USBTask();
        print("\nUSB init\n");
#endif
    }

    rn42_init();
    print("RN-42 init\n");
    
    rn42_task_init();
    print("RN-42 task init\n");

    /* init modules */
    keyboard_init();
    print("Keyboard init\n");

#ifdef SLEEP_LED_ENABLE
    sleep_led_init();
#endif

    print("Keyboard start\n");
    while (1) {
        while (
                !rn42_powered() && // RN42 is not powered
                //rn42_rts() && // RN42 is off
                USB_DeviceState == DEVICE_STATE_Suspended) {
            print("[s]");
            matrix_power_down();
            suspend_power_down();
            suspend_power_down();
            suspend_power_down();
            suspend_power_down();
            suspend_power_down();
            suspend_power_down();
            suspend_power_down();
            if (USB_Device_RemoteWakeupEnabled && suspend_wakeup_condition()) {
                    USB_Device_SendRemoteWakeup();
            }
        }

        keyboard_task();
        //print("Keyboard task\n");

#if !defined(INTERRUPT_CONTROL_ENDPOINT)
        USB_USBTask();
        //print("USB task\n");
#endif

        rn42_task();
        //print("rn42 task\n");
    }
}
