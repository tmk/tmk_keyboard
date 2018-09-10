/*
Copyright 2018 Jun WAKO <wakojun@gmail.com>
Copyright 2016 Ethan Apodaca <papodaca@gmail.com>

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

#include <stdbool.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "xt.h"
#include "wait.h"
#include "print.h"
#include "ringbuf.h"


#define BUF_SIZE 16
static uint8_t buf[BUF_SIZE];
static ringbuf_t rb = {
    .buffer = buf,
    .head = 0,
    .tail = 0,
    .size_mask = BUF_SIZE - 1
};

void xt_host_init(void)
{
    XT_INT_INIT();
    XT_INT_OFF();

    /* hard reset */
#ifdef XT_RESET
    XT_RESET();
#endif

    /* soft reset: pull clock line down for 20ms */
    XT_DATA_LO();
    XT_CLOCK_LO();
    _delay_ms(20);

    /* input mode with pullup */
    XT_CLOCK_IN();
    XT_DATA_IN();

    XT_INT_ON();
}

/* get data received by interrupt */
uint8_t xt_host_recv(void)
{
    if (ringbuf_is_empty(&rb)) {
        return 0;
    } else {
        int16_t d = ringbuf_get(&rb);
        XT_DATA_IN();  // ready to receive from keyboard
        return d;
    }
}

ISR(XT_INT_VECT)
{
    /*
     * XT signal format consits of 10 or 9 clocks and sends start bits and 8-bit data,
     * which should be read on falling edge of clock.
     *
     *  start(0), start(1), bit0, bit1, bit2, bit3, bit4, bit5, bit6, bit7
     *
     * Original IBM XT keyboard sends start(0) bit while some of clones don't.
     * Start(0) bit is read as low on data line while start(1) as high.
     *
     * https://github.com/tmk/tmk_keyboard/wiki/IBM-PC-XT-Keyboard-Protocol
     */
    static enum {
        START, BIT0, BIT1, BIT2, BIT3, BIT4, BIT5, BIT6, BIT7
    } state = START;
    static uint8_t data = 0;

    uint8_t dbit = XT_DATA_READ();

    // This is needed if using PCINT which can be called on both falling and rising edge
    //if (XT_CLOCK_READ()) return;

    switch (state) {
        case START:
            // ignore start(0) bit
            if (!dbit) return;
            break;
        case BIT0 ... BIT7:
            data >>= 1;
            if (dbit)
                data |= 0x80;
            break;
    }
    if (state++ == BIT7) {
        ringbuf_put(&rb, data);
        if (ringbuf_is_full(&rb)) {
            XT_DATA_LO();  // inhibit keyboard sending
            print("Full");
        }
        state = START;
        data = 0;
    }
    return;
}
