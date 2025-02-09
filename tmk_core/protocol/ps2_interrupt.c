/*
Copyright 2010,2011,2012,2013 Jun WAKO <wakojun@gmail.com>

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
 * PS/2 protocol Pin interrupt version
 */

#include <stdbool.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "pbuff.h"
#include "ps2.h"
#include "ps2_io.h"
#include "print.h"


#define WAIT(stat, us, err) do { \
    if (!wait_##stat(us)) { \
        ps2_error = err; \
        goto ERROR; \
    } \
} while (0)


uint8_t ps2_error = PS2_ERR_NONE;

void ps2_host_init(void)
{
    idle();
    PS2_INT_INIT();
    PS2_INT_ON();
    // POR(150-2000ms) plus BAT(300-500ms) may take 2.5sec([3]p.20)
    //_delay_ms(2500);
}

uint8_t ps2_host_send(uint8_t data)
{
    bool parity = true;
    ps2_error = PS2_ERR_NONE;

    // see http://www.burtonsys.com/ps2_chapweske.htm but note that
    // sending host->device samples on the rising not falling CLK edge

    PS2_INT_OFF();

    /* terminate a transmission if we have */
    inhibit();
    _delay_us(100); // 100us [4]p.13, [5]p.50

    /* 'Request to Send' and Start bit */
    data_lo();
    clock_hi();
    WAIT(clock_lo, 10000, 10);   // 10ms [5]p.50

    /* Data bit[2-9] */
    for (uint8_t i = 0; i < 8; i++) {
        if (data&(1<<i)) {
            parity = !parity;
            data_hi();
        } else {
            data_lo();
        }
        WAIT(clock_hi, 50, 2);
        // keyboard sampled the data, just before here, just as the clock goes high
        WAIT(clock_lo, 50, 3);
    }

    /* Parity bit */
    if (parity) { data_hi(); } else { data_lo(); }
    WAIT(clock_hi, 50, 4);
    WAIT(clock_lo, 50, 5);

    /* Stop bit */
    data_hi();

    /* Ack */
    WAIT(data_lo, 50, 6);
    WAIT(clock_lo, 50, 7);

    /* wait for idle state */
    WAIT(clock_hi, 50, 8);
    WAIT(data_hi, 50, 9);

    idle();
    PS2_INT_ON();
    return ps2_host_recv_response();
ERROR:
    idle();
    PS2_INT_ON();
    return 0;
}

uint8_t ps2_host_recv_response(void)
{
    // Command may take 25ms/20ms at most([5]p.46, [3]p.21)
    uint8_t retry = 25;
    while (retry-- && !pbuf_has_data()) {
        _delay_ms(1);
    }
    return pbuf_dequeue();
}

/* get data received by interrupt */
uint8_t ps2_host_recv(void)
{
    if (pbuf_has_data()) {
        ps2_error = PS2_ERR_NONE;
        return pbuf_dequeue();
    } else {
        ps2_error = PS2_ERR_NODATA;
        return 0;
    }
}

ISR(PS2_INT_VECT)
{
    static enum {
        INIT,
        START,
        BIT0, BIT1, BIT2, BIT3, BIT4, BIT5, BIT6, BIT7,
        PARITY,
        STOP,
    } state = INIT;
    static uint8_t data = 0;
    static uint8_t parity = 1;

    // TODO: abort if elapse 100us from previous interrupt

    // return unless falling edge
    if (clock_in()) {
        goto RETURN;
    }

    state++;
    switch (state) {
        case START:
            if (data_in())
                goto ERROR;
            break;
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
            pbuf_enqueue(data);
            goto DONE;
            break;
        default:
            goto ERROR;
    }
    goto RETURN;
ERROR:
    ps2_error = state;
DONE:
    state = INIT;
    data = 0;
    parity = 1;
RETURN:
    return;
}

/* send LED state to keyboard */
void ps2_host_set_led(uint8_t led)
{
    ps2_host_send(0xED);
    ps2_host_send(led);
}
