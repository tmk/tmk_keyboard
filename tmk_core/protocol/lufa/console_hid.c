/*******************************************************************************
 * Console HID
 ******************************************************************************/
#ifdef CONSOLE_STDIO
#   include <stdio.h>
#else
#   include "avr/xprintf.h"
#endif

#include "ringbuf.h"
#include "timer.h"
#include "lufa.h"
#include "descriptor.h"
#include "console.h"


#define SENDBUF_SIZE 256
static uint8_t sbuf[SENDBUF_SIZE];
static ringbuf_t sendbuf = {
    .buffer = sbuf,
    .head = 0,
    .tail = 0,
    .size_mask = SENDBUF_SIZE - 1
};


static bool hid_putchar(uint8_t c);

#ifdef CONSOLE_STDIO
static int stdio_putchar(char c, FILE *stream)
{
    return hid_putchar(c) ? 0 : -1;
}

static int stdio_getchar(FILE *stream)
{
    return _FDEV_EOF;
}
#endif

void console_init(void)
{
#ifdef CONSOLE_STDIO
    // <stdio.h> stream
    static FILE HID_stdio = (FILE)FDEV_SETUP_STREAM(stdio_putchar, stdio_getchar, _FDEV_SETUP_RW);
    stdin = &HID_stdio;
    stdout = &HID_stdio;
#else
    // Setup xprintf
    xdev_out(hid_putchar);
#endif
}

bool console_configure(void)
{
    bool r = true;
    /* Setup Console HID Report Endpoints */
    r &= ENDPOINT_CONFIG(CONSOLE_IN_EPNUM, EP_TYPE_INTERRUPT, ENDPOINT_DIR_IN,
        CONSOLE_EPSIZE, ENDPOINT_BANK_SINGLE);
#if 0
    r &=ENDPOINT_CONFIG(CONSOLE_OUT_EPNUM, EP_TYPE_INTERRUPT, ENDPOINT_DIR_OUT,
        CONSOLE_EPSIZE, ENDPOINT_BANK_SINGLE);
#endif
    return r;
}

void console_control_request(void)
{
}

// TODO: Around 2500ms delay often works anyhoo but proper startup would be better
// 1000ms delay of hid_listen affects this probably
/* wait for Console startup */
bool console_is_ready(void)
{
    static bool hid_listen_ready = false;
    if (!hid_listen_ready) {
        if (timer_read32() < 2500)
            return false;
        hid_listen_ready = true;
    }
    return true;
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

void console_task(void)
{
    static uint16_t fn = 0;
    if (fn == USB_Device_GetFrameNumber()) {
        return;
    }
    fn = USB_Device_GetFrameNumber();
    console_flush();
}

static bool hid_putchar(uint8_t c)
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
