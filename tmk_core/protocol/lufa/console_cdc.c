/*******************************************************************************
 * Console CDC
 ******************************************************************************/
#ifdef CONSOLE_STDIO
#   include <stdio.h>
#else
#   include "avr/xprintf.h"
#endif

#include "ringbuf.h"
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


#define CONSOLE_SNDBUF_SIZE 256
static uint8_t _sndbuf[CONSOLE_SNDBUF_SIZE];
static ringbuf_t sndbuf = {
    .buffer = _sndbuf,
    .head = 0,
    .tail = 0,
    .size_mask = CONSOLE_SNDBUF_SIZE - 1
};

#define CONSOLE_RCVBUF_SIZE 32
static uint8_t _rcvbuf[CONSOLE_RCVBUF_SIZE];
static ringbuf_t rcvbuf = {
    .buffer = _rcvbuf,
    .head = 0,
    .tail = 0,
    .size_mask = CONSOLE_RCVBUF_SIZE - 1
};

static bool cdc_putchar(uint8_t c)
{
    // return immediately if called while interrupt
    if (!(SREG & (1<<SREG_I)))
        goto BUFFER;

    if (!console_is_ready())
        goto BUFFER;

    // Host port is available hopefully but this may be still blocked in case of the port fails somehow?
    if (ringbuf_is_empty(&sndbuf)) {
        if (CDC_Device_SendByte(&CDC_device, c) == 0)
            return true;
    }

BUFFER:
    return ringbuf_put(&sndbuf, c);
}

#ifdef CONSOLE_STDIO
static int stdio_putchar(char c, FILE *stream)
{
    return cdc_putchar(c) ? 0 : -1;
}

static int stdio_getchar(FILE *stream)
{
    int16_t w = ringbuf_get(&rcvbuf);
    if (w < 0) {
        w = CDC_Device_ReceiveByte(&CDC_device);
        if (w < 0) return _FDEV_EOF;
    }
    return w;
}
#endif

void console_init(void)
{
#ifdef CONSOLE_STDIO
    // <stdio.h> stream
    static FILE stdio = (FILE)FDEV_SETUP_STREAM(stdio_putchar, stdio_getchar, _FDEV_SETUP_RW);
    stdin = &stdio;
    stdout = &stdio;
#else
    // Setup xprintf
    xdev_out(cdc_putchar);
#endif
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

void console_task(void)
{
    static uint16_t fn = 0;
    if (fn == USB_Device_GetFrameNumber())
        return;
    fn = USB_Device_GetFrameNumber();

    if (!console_is_ready())
        return;

    // Flush buffer data
    for (int16_t w; (w = ringbuf_get(&sndbuf)) != -1; ) {
        if (CDC_Device_SendByte(&CDC_device, (uint8_t)w) != 0) {
            ringbuf_put(&sndbuf, (uint8_t)w);
            break;
        }
    }
    CDC_Device_Flush(&CDC_device);

    // Receive data into buffer
    int16_t w = CDC_Device_ReceiveByte(&CDC_device);
    if (w < 0) return;
    ringbuf_put(&rcvbuf, (uint8_t)w);
}
