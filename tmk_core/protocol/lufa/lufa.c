/*
 * Copyright 2012 Jun Wako <wakojun@gmail.com>
 * This file is based on:
 *     LUFA-120219/Demos/Device/Lowlevel/KeyboardMouse
 *     LUFA-120219/Demos/Device/Lowlevel/GenericHID
 */

/*
             LUFA Library
     Copyright (C) Dean Camera, 2012.

  dean [at] fourwalledcubicle [dot] com
           www.lufa-lib.org
*/

/*
  Copyright 2012  Dean Camera (dean [at] fourwalledcubicle [dot] com)
  Copyright 2010  Denver Gingerich (denver [at] ossguy [dot] com)

  Permission to use, copy, modify, distribute, and sell this
  software and its documentation for any purpose is hereby granted
  without fee, provided that the above copyright notice appear in
  all copies and that both that the copyright notice and this
  permission notice and warranty disclaimer appear in supporting
  documentation, and that the name of the author not be used in
  advertising or publicity pertaining to distribution of the
  software without specific, written prior permission.

  The author disclaim all warranties with regard to this
  software, including all implied warranties of merchantability
  and fitness.  In no event shall the author be liable for any
  special, indirect or consequential damages or any damages
  whatsoever resulting from loss of use, data or profits, whether
  in an action of contract, negligence or other tortious action,
  arising out of or in connection with the use or performance of
  this software.
*/

#include "report.h"
#include "host.h"
#include "host_driver.h"
#include "keyboard.h"
#include "action.h"
#include "led.h"
#include "sendchar.h"
#include "ringbuf.h"
#include "debug.h"
#ifdef SLEEP_LED_ENABLE
#include "sleep_led.h"
#endif
#include "suspend.h"
#include "hook.h"
#include "timer.h"

#ifdef TMK_LUFA_DEBUG_SUART
#include "avr/suart.h"
#endif

#ifdef TMK_LUFA_DEBUG_UART
#include "uart.h"
#endif

#include "matrix.h"
#include "descriptor.h"
#include "lufa.h"


//#define TMK_LUFA_DEBUG


#ifndef NO_KEYBOARD
uint8_t keyboard_idle = 0;
/* 0: Boot Protocol, 1: Report Protocol(default) */
uint8_t keyboard_protocol = 1;
static uint8_t keyboard_led_stats = 0;
static report_keyboard_t keyboard_report_sent;
#endif

#ifdef MOUSE_ENABLE
uint8_t mouse_protocol = 1;
#endif


/* Host driver */
#ifndef NO_KEYBOARD
static uint8_t keyboard_leds(void);
static void send_keyboard(report_keyboard_t *report);
#endif
static void send_mouse(report_mouse_t *report);
static void send_system(uint16_t data);
static void send_consumer(uint16_t data);
host_driver_t lufa_driver = {
#ifndef NO_KEYBOARD
    .keyboard_leds = keyboard_leds,
    .send_keyboard = send_keyboard,
#endif
    .send_mouse = send_mouse,
    .send_system = send_system,
    .send_consumer = send_consumer
};


/*******************************************************************************
 * Console
 ******************************************************************************/
#ifdef CONSOLE_ENABLE
#define SENDBUF_SIZE 256
static uint8_t sbuf[SENDBUF_SIZE];
static ringbuf_t sendbuf = {
    .buffer = sbuf,
    .head = 0,
    .tail = 0,
    .size_mask = SENDBUF_SIZE - 1
};

// TODO: Around 2500ms delay often works anyhoo but proper startup would be better
// 1000ms delay of hid_listen affects this probably
/* wait for Console startup */
static bool console_is_ready(void)
{
    static bool hid_listen_ready = false;
    if (!hid_listen_ready) {
        if (timer_read32() < 2500)
            return false;
        hid_listen_ready = true;
    }
    return true;
}

static bool console_putc(uint8_t c)
{
    // return immediately if called while interrupt
    if (!(SREG & (1<<SREG_I)))
        goto EXIT;

    if (USB_DeviceState != DEVICE_STATE_Configured && !ringbuf_is_full(&sendbuf))
        goto EXIT;

    if (!console_is_ready() && !ringbuf_is_full(&sendbuf))
        goto EXIT;

    /* Data lost considerations:
     * 1. When buffer is full at early satage of startup, we will have to start sending
     * before console_is_ready() returns true. Data can be lost even if sending data
     * seems to be successful on USB. hid_listen on host is not ready perhaps?
     * Sometime first few packets are lost when buffer is full at startup.
     * 2. When buffer is full and USB pipe is not ready, new coming data will be lost.
     * 3. console_task() cannot send data in buffer while main loop is blocked.
     */
    /* retry timeout */
    const uint8_t CONSOLE_TIMOUT = 5;   // 1 is too small, 2 seems to be enough for Linux
    static uint8_t timeout = CONSOLE_TIMOUT;
    uint16_t prev = timer_read();
    bool done = false;

    uint8_t ep = Endpoint_GetCurrentEndpoint();
    Endpoint_SelectEndpoint(CONSOLE_IN_EPNUM);

AGAIN:
    if (Endpoint_IsStalled() || !Endpoint_IsEnabled() || !Endpoint_IsConfigured()) {
        goto EXIT_RESTORE_EP;
    }

    // write from buffer to endpoint bank
    while (!ringbuf_is_empty(&sendbuf) && Endpoint_IsReadWriteAllowed()) {
        Endpoint_Write_8(ringbuf_get(&sendbuf));

        // clear bank when it is full
        if (!Endpoint_IsReadWriteAllowed() && Endpoint_IsINReady()) {
            Endpoint_ClearIN();
            timeout = CONSOLE_TIMOUT; // re-enable retry only when host can receive
        }
    }

    // write c to bank directly if there is no others in buffer
    if (ringbuf_is_empty(&sendbuf) && Endpoint_IsReadWriteAllowed()) {
        Endpoint_Write_8(c);
        done = true;
    }

    // clear bank when there are chars in bank
    if (Endpoint_BytesInEndpoint() && Endpoint_IsINReady()) {
        // Windows needs to fill packet with 0
        while (Endpoint_IsReadWriteAllowed()) {
                Endpoint_Write_8(0);
        }
        Endpoint_ClearIN();
        timeout = CONSOLE_TIMOUT; // re-enable retry only when host can receive
    }

    if (done) {
        Endpoint_SelectEndpoint(ep);
        return true;
    }

    /* retry when buffer is full.
     * once timeout this is disabled until host receives actually,
     * otherwise this will block or make main loop execution sluggish.
     */
    if (ringbuf_is_full(&sendbuf) && timeout) {
        uint16_t curr = timer_read();
        if (curr != prev) {
            timeout--;
            prev = curr;
        }
        goto AGAIN;
    }

EXIT_RESTORE_EP:
    Endpoint_SelectEndpoint(ep);
EXIT:
    return ringbuf_put(&sendbuf, c);
}

static void console_flush(void)
{
    if (!console_is_ready())
        return;

    if (USB_DeviceState != DEVICE_STATE_Configured)
        return;

    uint8_t ep = Endpoint_GetCurrentEndpoint();

    Endpoint_SelectEndpoint(CONSOLE_IN_EPNUM);
    if (!Endpoint_IsEnabled() || !Endpoint_IsConfigured()) {
        Endpoint_SelectEndpoint(ep);
        return;
    }

    // write from buffer to endpoint bank
    while (!ringbuf_is_empty(&sendbuf) && Endpoint_IsReadWriteAllowed()) {
        Endpoint_Write_8(ringbuf_get(&sendbuf));

        // clear bank when it is full
        if (!Endpoint_IsReadWriteAllowed() && Endpoint_IsINReady()) {
            Endpoint_ClearIN();
        }
    }

    // clear bank when there are chars in bank
    if (Endpoint_BytesInEndpoint() && Endpoint_IsINReady()) {
        // Windows needs to fill packet with 0
        while (Endpoint_IsReadWriteAllowed()) {
                Endpoint_Write_8(0);
        }
        Endpoint_ClearIN();
    }

    Endpoint_SelectEndpoint(ep);
}

static void console_task(void)
{
    static uint16_t fn = 0;
    if (fn == USB_Device_GetFrameNumber()) {
        return;
    }
    fn = USB_Device_GetFrameNumber();
    console_flush();
}
#endif


/*******************************************************************************
 * USB Events
 ******************************************************************************/
/*
 * Event Order of Plug in:
 * 0) EVENT_USB_Device_Connect
 * 1) EVENT_USB_Device_Suspend
 * 2) EVENT_USB_Device_Reset
 * 3) EVENT_USB_Device_Wake
*/
void EVENT_USB_Device_Connect(void)
{
#ifdef TMK_LUFA_DEBUG
    print("[C]");
#endif
    /* For battery powered device */
    if (!USB_IsInitialized) {
        USB_Disable();
        USB_Init();
    }
}

void EVENT_USB_Device_Disconnect(void)
{
#ifdef TMK_LUFA_DEBUG
    print("[D]");
#endif
    /* For battery powered device */
    USB_IsInitialized = false;
/* TODO: This doesn't work. After several plug in/outs can not be enumerated.
    if (USB_IsInitialized) {
        USB_Disable();  // Disable all interrupts
	USB_Controller_Enable();
        USB_INT_Enable(USB_INT_VBUSTI);
    }
*/
}

void EVENT_USB_Device_Reset(void)
{
#ifdef TMK_LUFA_DEBUG
    print("[R]");
#endif
}

void EVENT_USB_Device_Suspend()
{
#ifdef TMK_LUFA_DEBUG
    print("[S]");
#endif
    hook_usb_suspend_entry();
}

void EVENT_USB_Device_WakeUp()
{
#ifdef TMK_LUFA_DEBUG
    print("[W]");
#endif
    hook_usb_wakeup();
}

/** Event handler for the USB_ConfigurationChanged event.
 * This is fired when the host sets the current configuration of the USB device after enumeration.
 *
 * ATMega32u2 supports dual bank(ping-pong mode) only on endpoint 3 and 4,
 * it is safe to use singl bank for all endpoints.
 */
void EVENT_USB_Device_ConfigurationChanged(void)
{
#ifdef TMK_LUFA_DEBUG
    print("[c]");
#endif
    bool ConfigSuccess = true;

#ifndef NO_KEYBOARD
    /* Setup Keyboard HID Report Endpoints */
    ConfigSuccess &= ENDPOINT_CONFIG(KEYBOARD_IN_EPNUM, EP_TYPE_INTERRUPT, ENDPOINT_DIR_IN,
#ifdef NKRO_ENABLE
                                     NKRO_EPSIZE,
#else
                                     KEYBOARD_EPSIZE,
#endif
                                     ENDPOINT_BANK_SINGLE);
#endif

#if defined(MOUSE_ENABLE) || defined(EXTRAKEY_ENABLE)
    /* Setup Mouse HID Report Endpoint */
    ConfigSuccess &= ENDPOINT_CONFIG(MOUSE_IN_EPNUM, EP_TYPE_INTERRUPT, ENDPOINT_DIR_IN,
                                     MOUSE_EPSIZE, ENDPOINT_BANK_SINGLE);
#endif

#ifdef CONSOLE_ENABLE
    // ATMega32U2 doesn't support double bank on endpoint 1 and 2, use 3 or 4
    /* Setup Console HID Report Endpoints */
    ConfigSuccess &= ENDPOINT_CONFIG(CONSOLE_IN_EPNUM, EP_TYPE_INTERRUPT, ENDPOINT_DIR_IN,
                                     CONSOLE_EPSIZE, ENDPOINT_BANK_DOUBLE);
#if 0
    ConfigSuccess &= ENDPOINT_CONFIG(CONSOLE_OUT_EPNUM, EP_TYPE_INTERRUPT, ENDPOINT_DIR_OUT,
                                     CONSOLE_EPSIZE, ENDPOINT_BANK_SINGLE);
#endif
#endif

#ifdef NKRO_6KRO_ENABLE
    /* Setup NKRO HID Report Endpoints */
    ConfigSuccess &= ENDPOINT_CONFIG(NKRO_IN_EPNUM, EP_TYPE_INTERRUPT, ENDPOINT_DIR_IN,
                                     NKRO_EPSIZE, ENDPOINT_BANK_SINGLE);
#endif
}

/*
Appendix G: HID Request Support Requirements

The following table enumerates the requests that need to be supported by various types of HID class devices.

Device type     GetReport   SetReport   GetIdle     SetIdle     GetProtocol SetProtocol
------------------------------------------------------------------------------------------
Boot Mouse      Required    Optional    Optional    Optional    Required    Required
Non-Boot Mouse  Required    Optional    Optional    Optional    Optional    Optional
Boot Keyboard   Required    Optional    Required    Required    Required    Required
Non-Boot Keybrd Required    Optional    Required    Required    Optional    Optional
Other Device    Required    Optional    Optional    Optional    Optional    Optional
*/
/** Event handler for the USB_ControlRequest event.
 *  This is fired before passing along unhandled control requests to the library for processing internally.
 */
void EVENT_USB_Device_ControlRequest(void)
{
    /* Handle HID Class specific requests */
    switch (USB_ControlRequest.bRequest)
    {
        case HID_REQ_GetReport:
#ifndef NO_KEYBOARD
            if (USB_ControlRequest.bmRequestType == (REQDIR_DEVICETOHOST | REQTYPE_CLASS | REQREC_INTERFACE))
            {
                uint8_t* ReportData = NULL;
                uint8_t  ReportSize = 0;

                Endpoint_ClearSETUP();

                // Interface
                switch (USB_ControlRequest.wIndex) {
                case KEYBOARD_INTERFACE:
                    // TODO: test/check
                    ReportData = (uint8_t*)&keyboard_report_sent;
                    ReportSize = sizeof(keyboard_report_sent);
                    break;
                }

                /* Write the report data to the control endpoint */
                Endpoint_Write_Control_Stream_LE(ReportData, ReportSize);
                Endpoint_ClearOUT();
#ifdef TMK_LUFA_DEBUG
                xprintf("[r%d]", USB_ControlRequest.wIndex);
#endif
            }
#endif

            break;
        case HID_REQ_SetReport:
            if (USB_ControlRequest.bmRequestType == (REQDIR_HOSTTODEVICE | REQTYPE_CLASS | REQREC_INTERFACE))
            {

                // Interface
                switch (USB_ControlRequest.wIndex) {
#ifndef NO_KEYBOARD
                case KEYBOARD_INTERFACE:
#ifdef NKRO_6KRO_ENABLE
                case NKRO_INTERFACE:
#endif
                    Endpoint_ClearSETUP();

                    while (!(Endpoint_IsOUTReceived())) {
                        if (USB_DeviceState == DEVICE_STATE_Unattached)
                          return;
                    }
                    keyboard_led_stats = Endpoint_Read_8();

                    Endpoint_ClearOUT();
                    Endpoint_ClearStatusStage();
#ifdef TMK_LUFA_DEBUG
                    xprintf("[L%d]", USB_ControlRequest.wIndex);
#endif
                    break;
#endif
                }

            }

            break;

        case HID_REQ_GetProtocol:
            if (USB_ControlRequest.bmRequestType == (REQDIR_DEVICETOHOST | REQTYPE_CLASS | REQREC_INTERFACE))
            {
#ifndef NO_KEYBOARD
                if (USB_ControlRequest.wIndex == KEYBOARD_INTERFACE) {
                    Endpoint_ClearSETUP();
                    while (!(Endpoint_IsINReady()));
                    Endpoint_Write_8(keyboard_protocol);
                    Endpoint_ClearIN();
                    Endpoint_ClearStatusStage();
#ifdef TMK_LUFA_DEBUG
                    print("[p]");
#endif
                }
#endif
#if defined(MOUSE_ENABLE)
                if (USB_ControlRequest.wIndex == MOUSE_INTERFACE) {
                    Endpoint_ClearSETUP();
                    while (!(Endpoint_IsINReady()));
                    Endpoint_Write_8(mouse_protocol);
                    Endpoint_ClearIN();
                    Endpoint_ClearStatusStage();
                }
#endif
            }

            break;
        case HID_REQ_SetProtocol:
            if (USB_ControlRequest.bmRequestType == (REQDIR_HOSTTODEVICE | REQTYPE_CLASS | REQREC_INTERFACE))
            {
#ifndef NO_KEYBOARD
                if (USB_ControlRequest.wIndex == KEYBOARD_INTERFACE) {
                    Endpoint_ClearSETUP();
                    Endpoint_ClearStatusStage();

                    keyboard_protocol = (USB_ControlRequest.wValue & 0xFF);
                    clear_keyboard();
#ifdef TMK_LUFA_DEBUG
                    print("[P]");
#endif
                }
#endif
#if defined(MOUSE_ENABLE)
                if (USB_ControlRequest.wIndex == MOUSE_INTERFACE) {
                    Endpoint_ClearSETUP();
                    Endpoint_ClearStatusStage();

                    mouse_protocol = (USB_ControlRequest.wValue & 0xFF);
                    clear_keyboard();
                }
#endif
            }

            break;
        case HID_REQ_SetIdle:
            if (USB_ControlRequest.bmRequestType == (REQDIR_HOSTTODEVICE | REQTYPE_CLASS | REQREC_INTERFACE))
            {
#ifndef NO_KEYBOARD
                Endpoint_ClearSETUP();
                Endpoint_ClearStatusStage();

                keyboard_idle = ((USB_ControlRequest.wValue & 0xFF00) >> 8);
#ifdef TMK_LUFA_DEBUG
                xprintf("[I%d]%d", USB_ControlRequest.wIndex, (USB_ControlRequest.wValue & 0xFF00) >> 8);
#endif
#endif
            }

            break;
        case HID_REQ_GetIdle:
            if (USB_ControlRequest.bmRequestType == (REQDIR_DEVICETOHOST | REQTYPE_CLASS | REQREC_INTERFACE))
            {
#ifndef NO_KEYBOARD
                Endpoint_ClearSETUP();
                while (!(Endpoint_IsINReady()));
                Endpoint_Write_8(keyboard_idle);
                Endpoint_ClearIN();
                Endpoint_ClearStatusStage();
#ifdef TMK_LUFA_DEBUG
                print("[i]");
#endif
#endif
            }

            break;
    }
}

/*******************************************************************************
 * Host driver
 ******************************************************************************/
#ifndef NO_KEYBOARD
static uint8_t keyboard_leds(void)
{
    return keyboard_led_stats;
}

static void send_keyboard(report_keyboard_t *report)
{
    uint8_t timeout = 128;

    if (USB_DeviceState != DEVICE_STATE_Configured)
        return;

    /* Select the Keyboard Report Endpoint */
#if defined(NKRO_ENABLE) || defined(NKRO_6KRO_ENABLE)
    if (keyboard_protocol && keyboard_nkro) {
        /* Report protocol - NKRO */
        #if defined(NKRO_6KRO_ENABLE)
        Endpoint_SelectEndpoint(NKRO_IN_EPNUM);
        #else
        Endpoint_SelectEndpoint(KEYBOARD_IN_EPNUM);
        #endif

        /* Check if write ready for a polling interval around 1ms */
        while (timeout-- && !Endpoint_IsReadWriteAllowed()) _delay_us(8);
        if (!Endpoint_IsReadWriteAllowed()) return;

        /* Write Keyboard Report Data */
        Endpoint_Write_Stream_LE(report, NKRO_EPSIZE, NULL);
    }
    else
#endif
    {
        /* Boot protocol */
        Endpoint_SelectEndpoint(KEYBOARD_IN_EPNUM);

        /* Check if write ready for a polling interval around 10ms */
        while (timeout-- && !Endpoint_IsReadWriteAllowed()) _delay_us(80);
        if (!Endpoint_IsReadWriteAllowed()) return;

        /* Write Keyboard Report Data */
        Endpoint_Write_Stream_LE(report, KEYBOARD_EPSIZE, NULL);
    }

    /* Finalize the stream transfer to send the last packet */
    Endpoint_ClearIN();

    keyboard_report_sent = *report;
}
#endif

static void send_mouse(report_mouse_t *report)
{
#ifdef MOUSE_ENABLE
    uint8_t timeout = 255;

    if (USB_DeviceState != DEVICE_STATE_Configured)
        return;

    /* Select the Mouse Report Endpoint */
    Endpoint_SelectEndpoint(MOUSE_IN_EPNUM);

    /* Check if write ready for a polling interval around 10ms */
    while (timeout-- && !Endpoint_IsReadWriteAllowed()) _delay_us(40);
    if (!Endpoint_IsReadWriteAllowed()) return;

    /* Write Mouse Report Data */
    if (mouse_protocol) {
        // Report
        Endpoint_Write_8(REPORT_ID_MOUSE);
        Endpoint_Write_Stream_LE(report, sizeof(report_mouse_t), NULL);
    } else {
        // Boot
        Endpoint_Write_Stream_LE(report, 3, NULL);
    }

    /* Finalize the stream transfer to send the last packet */
    Endpoint_ClearIN();
#endif
}

static void send_system(uint16_t data)
{
#ifdef EXTRAKEY_ENABLE
    uint8_t timeout = 255;

    if (USB_DeviceState != DEVICE_STATE_Configured)
        return;

    report_extra_t r = { .report_id = REPORT_ID_SYSTEM };
    if (data < SYSTEM_POWER_DOWN) {
        r.usage = 0;
    } else {
        r.usage = data - SYSTEM_POWER_DOWN + 1;
    }
    Endpoint_SelectEndpoint(MOUSE_IN_EPNUM);

    /* Check if write ready for a polling interval around 10ms */
    while (timeout-- && !Endpoint_IsReadWriteAllowed()) _delay_us(40);
    if (!Endpoint_IsReadWriteAllowed()) return;

    Endpoint_Write_Stream_LE(&r, sizeof(report_extra_t), NULL);
    Endpoint_ClearIN();
#endif
}

static void send_consumer(uint16_t data)
{
#ifdef EXTRAKEY_ENABLE
    uint8_t timeout = 255;

    if (USB_DeviceState != DEVICE_STATE_Configured)
        return;

    report_extra_t r = {
        .report_id = REPORT_ID_CONSUMER,
        .usage = data
    };
    Endpoint_SelectEndpoint(MOUSE_IN_EPNUM);

    /* Check if write ready for a polling interval around 10ms */
    while (timeout-- && !Endpoint_IsReadWriteAllowed()) _delay_us(40);
    if (!Endpoint_IsReadWriteAllowed()) return;

    Endpoint_Write_Stream_LE(&r, sizeof(report_extra_t), NULL);
    Endpoint_ClearIN();
#endif
}


/*******************************************************************************
 * sendchar
 ******************************************************************************/
int8_t sendchar(uint8_t c)
{
    #ifdef TMK_LUFA_DEBUG_SUART
    xmit(c);
    #endif

    #ifdef TMK_LUFA_DEBUG_UART
    uart_putchar(c);
    #endif

    #ifdef CONSOLE_ENABLE
    console_putc(c);
    #endif

    return 0;
}


/*******************************************************************************
 * main
 ******************************************************************************/
static void setup_mcu(void)
{
    /* Disable watchdog if enabled by bootloader/fuses */
    MCUSR &= ~(1 << WDRF);
    wdt_disable();

    /* Disable clock division */
    clock_prescale_set(clock_div_1);
}

static void setup_usb(void)
{
    // Leonardo needs. Without this USB device is not recognized.
    USB_Disable();

    USB_Init();
}

int main(void)  __attribute__ ((weak));
int main(void)
{
    setup_mcu();

#ifdef TMK_LUFA_DEBUG_SUART
    SUART_OUT_DDR |= (1<<SUART_OUT_BIT);
    SUART_OUT_PORT |= (1<<SUART_OUT_BIT);
#endif

#ifdef TMK_LUFA_DEBUG_UART
    uart_init(115200);
#endif

    // setup sendchar: DO NOT USE print functions before this line
    print_set_sendchar(sendchar);
    host_set_driver(&lufa_driver);

    print("\nTMK:" STR(TMK_VERSION) "/LUFA:" STR(TMK_LUFA_VERSION)
#ifdef TMK_USB_HOST_SHIELD_VERSION
            "/UHS2:" STR(TMK_USB_HOST_SHIELD_VERSION)
#endif
            "\n");

    hook_early_init();

#ifndef NO_KEYBOARD
    keyboard_setup();
#endif

    setup_usb();

#ifdef SLEEP_LED_ENABLE
    sleep_led_init();
#endif

    sei();

#ifndef NO_KEYBOARD
    keyboard_init();
#else
    // TODO: keyboard_init() should be used only for things related to keyboard
    timer_init();
#endif

#ifndef NO_USB_STARTUP_WAIT_LOOP
    /* wait for USB startup */
    while (USB_DeviceState != DEVICE_STATE_Configured
#ifdef CONSOLE_ENABLE
            || !console_is_ready()
#endif
            ) {
#if defined(INTERRUPT_CONTROL_ENDPOINT)
        ;
#else
        USB_USBTask();
#endif
        hook_usb_startup_wait_loop();
    }
    print("\nUSB configured.\n");
#endif

    hook_late_init();

    print("\nLoop start.\n");
    while (1) {
#ifndef NO_USB_SUSPEND_LOOP
        while (USB_DeviceState == DEVICE_STATE_Suspended) {
            hook_usb_suspend_loop();
        }
#endif

        hook_main_loop();

#ifndef NO_KEYBOARD
        keyboard_task();
#endif

#ifdef CONSOLE_ENABLE
        console_task();
#endif

#if !defined(INTERRUPT_CONTROL_ENDPOINT)
        USB_USBTask();
#endif
    }
}


/* hooks */
__attribute__((weak))
void hook_early_init(void) {}

__attribute__((weak))
void hook_late_init(void) {}

#ifndef NO_KEYBOARD
static uint8_t _led_stats = 0;
#endif
 __attribute__((weak))
void hook_usb_suspend_entry(void)
{
#ifndef NO_KEYBOARD
    // Turn off LED to save power and keep its status to resotre it later.
    // LED status will be updated by keyboard_task() in main loop hopefully.
    _led_stats = keyboard_led_stats;
    keyboard_led_stats = 0;

    // Calling long task here can prevent USB state transition

    matrix_clear();
    clear_keyboard();
#endif
#ifdef SLEEP_LED_ENABLE
    sleep_led_enable();
#endif
}

__attribute__((weak))
void hook_usb_suspend_loop(void)
{
#ifndef TMK_LUFA_DEBUG_UART
    // This corrupts debug print when suspend
    suspend_power_down();
#endif
    if (USB_Device_RemoteWakeupEnabled && suspend_wakeup_condition()) {
        USB_Device_SendRemoteWakeup();
    }
}

__attribute__((weak))
void hook_usb_wakeup(void)
{
    suspend_wakeup_init();
#ifdef SLEEP_LED_ENABLE
    sleep_led_disable();
#endif

#ifndef NO_KEYBOARD
    // Restore LED status and update at keyboard_task() in main loop
    keyboard_led_stats = _led_stats;
#endif

    // Calling long task here can prevent USB state transition
}

__attribute__((weak))
void hook_usb_startup_wait_loop(void) {}
