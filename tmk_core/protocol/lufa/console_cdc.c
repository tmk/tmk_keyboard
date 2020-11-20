/*******************************************************************************
 * Console CDC
 ******************************************************************************/
#include <stdio.h>
#include "ringbuf.h"
#include "avr/xprintf.h"
#include "lufa.h"
#include "descriptor.h"
#include "console.h"


USB_ClassInfo_CDC_Device_t CDC_device = {
    .Config = {
        .ControlInterfaceNumber = SERIAL_CCI_INTERFACE,
        .DataINEndpoint = {
            .Address = (ENDPOINT_DIR_IN | SERIAL_TX_EPNUM),
            .Size = SERIAL_TXRX_EPSIZE,
            .Banks = 1,
        },
        .DataOUTEndpoint = {
            .Address = (ENDPOINT_DIR_OUT | SERIAL_RX_EPNUM),
            .Size = SERIAL_TXRX_EPSIZE,
            .Banks = 1,
        },
        .NotificationEndpoint = {
            .Address = (ENDPOINT_DIR_IN | SERIAL_NOTIF_EPNUM),
            .Size = SERIAL_NOTIF_EPSIZE,
            .Banks = 1,
        },
    },
};


#define CONSOLE_INBUF_SIZE 256
static uint8_t _inbuf[CONSOLE_INBUF_SIZE];
static ringbuf_t inbuf = {
    .buffer = _inbuf,
    .head = 0,
    .tail = 0,
    .size_mask = CONSOLE_INBUF_SIZE - 1
};


static bool cdc_putchar(uint8_t c)
{
    // return immediately if called while interrupt
    if (!(SREG & (1<<SREG_I)))
        goto BUFFER;

    if (!console_is_ready())
        goto BUFFER;

    // Host port is available hopefully but this may be still blocked in case of the port fails somehow?
    if (ringbuf_is_empty(&inbuf)) {
        if (CDC_Device_SendByte(&CDC_device, c) == 0)
            return true;
    }

BUFFER:
    return ringbuf_put(&inbuf, c);
}

void console_init(void)
{
    // <stdio.h> stream
    static FILE CDC_stdio;

    // Setup CDC stream for avr-libc <stdio.h>
    CDC_Device_CreateStream(&CDC_device, &CDC_stdio);
    stdin = &CDC_stdio;
    stdout = &CDC_stdio;

    // Setup xprintf
    xdev_out(cdc_putchar);
}

bool console_configure(void)
{
    return CDC_Device_ConfigureEndpoints(&CDC_device);
}

void console_control_request(void)
{
    CDC_Device_ProcessControlRequest(&CDC_device);
}

bool console_is_ready(void)
{
    if ((USB_DeviceState != DEVICE_STATE_Configured) || !(CDC_device.State.LineEncoding.BaudRateBPS))
        return false;

    // check if host port is ready with DTR(and RTS?)
    if ((CDC_device.State.ControlLineStates.HostToDevice & CDC_CONTROL_LINE_OUT_DTR) == 0)
        return false;

    return true;
}

static void console_flush(void)
{
    if (!console_is_ready())
        return;

    for (int16_t w; (w = ringbuf_get(&inbuf)) != -1; ) {
        // Host port is available hopefully but this may be still blocked in case of the port fails somehow?
        if (CDC_Device_SendByte(&CDC_device, (uint8_t)w) != 0) {
            ringbuf_put(&inbuf, (uint8_t)w);
            break;
        }
    }

    // Host port is available hopefully but this may be still blocked in case of the port fails somehow?
    CDC_Device_Flush(&CDC_device);
}

void console_task(void)
{
    static uint16_t fn = 0;
    if (fn == USB_Device_GetFrameNumber()) {
        return;
    }
    fn = USB_Device_GetFrameNumber();

    console_flush();
}
