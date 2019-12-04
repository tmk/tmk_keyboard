/*
Copyright 2010,2011,2012,2013,2019 Jun WAKO <wakojun@gmail.com>

This software is licensed with a Modified BSD License.
All of this is supposed to be Free Software, Open Source, DFSG-free,
GPL-compatible, and OK to use in both free and proprietary applications.
Additions and corrections to this file are welcome.


Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright
  notice, this list of conditions and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright
  notice, this list of conditions and the following disclaimer in
  the documentation and/or other materials provided with the
  distribution.

* Neither the name of the copyright holders nor the names of
  contributors may be used to endorse or promote products derived
  from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.
*/

/*
 * IBM PC keyboard protocol
 */

#include <stdbool.h>
#include <avr/interrupt.h>
#include "ringbuf.h"
#include "ibmpc.h"
#include "debug.h"
#include "timer.h"
#include "wait.h"


#define WAIT(stat, us, err) do { \
    if (!wait_##stat(us)) { \
        ibmpc_error = err; \
        goto ERROR; \
    } \
} while (0)


#define BUF_SIZE 16
static uint8_t buf[BUF_SIZE];
static ringbuf_t rb = {
    .buffer = buf,
    .head = 0,
    .tail = 0,
    .size_mask = BUF_SIZE - 1
};

volatile uint8_t ibmpc_protocol = IBMPC_PROTOCOL_AT;
volatile uint8_t ibmpc_error = IBMPC_ERR_NONE;

void ibmpc_host_init(void)
{
    clock_init();
    data_init();
    idle();
    IBMPC_INT_INIT();
    IBMPC_INT_ON();
    // POR(150-2000ms) plus BAT(300-500ms) may take 2.5sec([3]p.20)
    //wait_ms(2500);
}

int16_t ibmpc_host_send(uint8_t data)
{
    bool parity = true;
    ibmpc_error = IBMPC_ERR_NONE;

    if (ibmpc_protocol == IBMPC_PROTOCOL_XT) return -1;

    dprintf("w%02X ", data);

    IBMPC_INT_OFF();

    /* terminate a transmission if we have */
    inhibit();
    wait_us(100); // 100us [4]p.13, [5]p.50

    /* 'Request to Send' and Start bit */
    data_lo();
    clock_hi();
    WAIT(clock_lo, 10000, 1);   // 10ms [5]p.50

    /* Data bit[2-9] */
    for (uint8_t i = 0; i < 8; i++) {
        wait_us(15);
        if (data&(1<<i)) {
            parity = !parity;
            data_hi();
        } else {
            data_lo();
        }
        WAIT(clock_hi, 50, 2);
        WAIT(clock_lo, 50, 3);
    }

    /* Parity bit */
    wait_us(15);
    if (parity) { data_hi(); } else { data_lo(); }
    WAIT(clock_hi, 50, 4);
    WAIT(clock_lo, 50, 5);

    /* Stop bit */
    wait_us(15);
    data_hi();

    /* Ack */
    WAIT(data_lo, 50, 6);
    WAIT(clock_lo, 50, 7);

    /* wait for idle state */
    WAIT(clock_hi, 50, 8);
    WAIT(data_hi, 50, 9);

    idle();
    IBMPC_INT_ON();
    return ibmpc_host_recv_response();
ERROR:
    ibmpc_error |= IBMPC_ERR_SEND;
    idle();
    IBMPC_INT_ON();
    return -1;
}

int16_t ibmpc_host_recv_response(void)
{
    // Command may take 25ms/20ms at most([5]p.46, [3]p.21)
    uint8_t retry = 25;
    while (retry-- && ringbuf_is_empty(&rb)) {
        wait_ms(1);
    }
    int16_t data = ringbuf_get(&rb);
    if (data != -1) dprintf("r%02X ", data);
    return data;
}

/* get data received by interrupt */
int16_t ibmpc_host_recv(void)
{
    int16_t data = ringbuf_get(&rb);
    if (data != -1) dprintf("r%02X ", data);
    return data;
}

ISR(IBMPC_INT_VECT)
{
    static uint16_t last_time = 0;
    static enum {
        START,
        BIT0, BIT1, BIT2, BIT3, BIT4, BIT5, BIT6, BIT7,
        PARITY,
        STOP,
    } state = START;
    static uint8_t data = 0;
    static uint8_t parity = 1;

    // return unless falling edge
    if (clock_in()) {
        return;
    }

    // Reset state when taking more than 1ms
    if (last_time && timer_elapsed(last_time) > 10) {
        ibmpc_error = IBMPC_ERR_TIMEOUT | IBMPC_ERR_RECV | state;
        state = START;
        data = 0;
        parity = 1;
    }
    last_time = timer_read();

    switch (state) {
        case START:
            if (ibmpc_protocol == IBMPC_PROTOCOL_XT) {
                // ignore start(0) bit
                if (!data_in()) return;
            } else {
                if (data_in())
                    goto ERROR;
            }
        case BIT0:
        case BIT1:
        case BIT2:
        case BIT3:
        case BIT4:
        case BIT5:
        case BIT6:
        case BIT7:
            data >>= 1;
            if (data_in()) {
                data |= 0x80;
                parity++;
            }
            if (state == BIT7 && ibmpc_protocol == IBMPC_PROTOCOL_XT) {
                if (!ringbuf_put(&rb, data)) {
                    ibmpc_error = IBMPC_ERR_FULL;
                    goto ERROR;
                }
                ibmpc_error = IBMPC_ERR_NONE;
                goto DONE;
            }
            break;
        case PARITY:
            if (data_in()) {
                if (!(parity & 0x01))
                    goto ERROR;
            } else {
                if (parity & 0x01)
                    goto ERROR;
            }
            break;
        case STOP:
            if (!data_in())
                goto ERROR;
            if (!ringbuf_put(&rb, data)) {
                ibmpc_error = IBMPC_ERR_FULL;
                goto ERROR;
            }
            ibmpc_error = IBMPC_ERR_NONE;
            goto DONE;
            break;
        default:
            goto ERROR;
    }
    goto NEXT;

ERROR:
    ibmpc_error |= state;
    ibmpc_error |= IBMPC_ERR_RECV;
    ringbuf_reset(&rb);
DONE:
    last_time = 0;
    state = START;
    data = 0;
    parity = 1;
    return;
NEXT:
    state++;
    return;
}

/* send LED state to keyboard */
void ibmpc_host_set_led(uint8_t led)
{
    ibmpc_host_send(0xED);
    ibmpc_host_send(led);
}
