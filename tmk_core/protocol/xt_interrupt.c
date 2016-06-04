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
#include "xt.h"
#include "xt_io.h"
#include "wait.h"
#include "print.h"

void xt_host_init(void)
{
    XT_INT_INIT();
    XT_INT_ON();
}

/* get data received by interrupt */
uint8_t xt_host_recv(void)
{
    if (pbuf_has_data()) {
        return pbuf_dequeue();
    } else {
        return 0;
    }
}

ISR(XT_INT_VECT)
{
    static uint8_t state = 0;
    static uint8_t data = 0;

    if (state == 0) {
        if (data_in())
            state++;
    } else if (state >= 1 && state <= 8) {
        wait_clock_lo(20);
        data >>= 1;
        if (data_in())
            data |= 0x80;
        if (state == 8)
            goto END;
        state++;
    } else {
        goto DONE;
    }
    goto RETURN;
END:
    pbuf_enqueue(data);
DONE:
    state = 0;
    data = 0;
RETURN:
    return;
}
