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
#include <util/atomic.h>
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
    // initialize reset pin
    IBMPC_RST_PORT |=  (1<<IBMPC_RST_BIT1);
    IBMPC_RST_DDR  |=  (1<<IBMPC_RST_BIT1);
    IBMPC_RST_PORT |=  (1<<IBMPC_RST_BIT2);
    IBMPC_RST_DDR  |=  (1<<IBMPC_RST_BIT2);
    inhibit();
    IBMPC_INT_INIT();
    IBMPC_INT_OFF();
}

void ibmpc_host_enable(void)
{
    IBMPC_INT_ON();
    idle();
}

void ibmpc_host_disable(void)
{
    IBMPC_INT_OFF();
    inhibit();
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

    // clear buffer to get response correctly
    ringbuf_reset(&rb);
    ibmpc_host_isr_clear();

    idle();
    IBMPC_INT_ON();
    return ibmpc_host_recv_response();
ERROR:
    ibmpc_error |= IBMPC_ERR_SEND;
    idle();
    IBMPC_INT_ON();
    return -1;
}

/*
 * Receive data from keyboard with ISR
 */
static volatile int16_t recv_data = -1;
static volatile uint16_t isr_data = 0x8000;

void ibmpc_host_isr_clear(void)
{
    isr_data = 0x8000;
}

int16_t ibmpc_host_recv(void)
{
    int16_t data = 0;
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        data = recv_data;
        recv_data = -1;
    }
    if (data != -1) {
        dprintf("r%04X ", data);
    }
    return data;

}

int16_t ibmpc_host_recv_response(void)
{
    // Command may take 25ms/20ms at most([5]p.46, [3]p.21)
    uint8_t retry = 25;
    int16_t data = -1;
    while (retry-- && (data = ibmpc_host_recv()) == -1) {
        wait_ms(1);
    }
    return data;
}

ISR(IBMPC_INT_VECT)
{
    uint8_t dbit;
    dbit = IBMPC_DATA_PIN&(1<<IBMPC_DATA_BIT);
    isr_data = isr_data>>1;
    if (dbit) isr_data |= 0x8000;

    // isr_data:
    //            15 14 13 12   11 10  9  8    7  6  5  4    3  2  1  0
    //            -----------------------------------------------------
    // Initial:   *1  0  0  0    0  0  0  0 |  0  0  0  0    0  0  0  0
    // XT IBM:    b7 b6 b5 b4   b3 b2 b1 b0 | s1 s0 *1  0    0  0  0  0     after receiving **
    // XT Clone:  b7 b6 b5 b4   b3 b2 b1 b0 | s1 *1  0  0    0  0  0  0     after receiving
    // AT:        st pr b7 b6   b5 b4 b3 b2 | b1 b0 s0 *1    0  0  0  0     after receiving
    // AT**:      pr b7 b6 b5   b4 b3 b2 b1 | b0 s0 *1  0    0  0  0  0     before stop bit **
    //
    //             x  x  x  x    x  x  x  x |  0  0  0  0    0  0  0  0     midway(0-7 bits received)
    //             x  x  x  x    x  x  x  x |  1  0  0  0    0  0  0  0     midway(8 bits received)
    //             x  x  x  x    x  x  x  x |  0  1  0  0    0  0  0  0     XT IBM-midway or AT-midway
    //             x  x  x  x    x  x  x  x |  1  1  0  0    0  0  0  0     XT Clone-done
    //             x  x  x  x    x  x  x  x |  0  0  1  0    0  0  0  0     AT-midway
    //             x  x  x  x    x  x  x  x |  1  0  1  0    0  0  0  0     XT IBM-done or AT-midway **
    //             x  x  x  x    x  x  x  x |  x  1  1  0    0  0  0  0     illegal
    //             x  x  x  x    x  x  x  x |  x  x  0  1    0  0  0  0     AT-done
    //             x  x  x  x    x  x  x  x |  x  x  1  1    0  0  0  0     illegal
    //                                          other states than avobe     illegal
    //
    // **: AT can take same as end sate of XT IBM(1010 000) when b0 is 1,
    // to discriminate between them we will have to wait a while for stop bit.
    //
    // mask for isr_data:
    // 0x00A0(1010 0000) when XT IBM
    // 0x00C0(1100 0000) when XT Clone
    // 0x0010(xx01 0000) when AT
    //
    switch (isr_data & 0xFF) {
        case 0b00000000:
        case 0b10000000:
        case 0b01000000:
        case 0b00100000:
            // midway
            return;
            break;
        case 0b11000000:
            // XT Clone-done
            recv_data = (isr_data>>8) & 0xFF;
            goto DONE;
            break;
        case 0b10100000:
            // XT IBM-done or AT-midway
            // wait and check for clock of AT stop bit
            if (wait_clock_hi(100) && wait_clock_lo(100)) { // FIXME this makes ISR prologe long
                // AT-midway
                return;
            } else {
                // XT-IBM-done
                recv_data = (isr_data>>8) & 0xFF;
                goto DONE;
            }
            break;
        case 0b00010000:
        case 0b10010000:
        case 0b01010000:
        case 0b11010000:
            // AT-done
            recv_data = (isr_data>>6) & 0xFF;
            goto DONE;
            break;
        case 0b01100000:
        case 0b11100000:
        case 0b00110000:
        case 0b10110000:
        case 0b01110000:
        case 0b11110000:
        default:            // xxxx_oooo(any 1 in low nibble)
            recv_data = isr_data;
            break;
    }
DONE:
    // TODO: buffer for recv_data
    isr_data = 0x8000;  // clear to next data
    return;
}

/* send LED state to keyboard */
void ibmpc_host_set_led(uint8_t led)
{
    ibmpc_host_send(0xED);
    ibmpc_host_send(led);
}
