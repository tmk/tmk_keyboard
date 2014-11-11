/* Copyright 2012 Jun Wako <wakojun@gmail.com> */
/* Very basic print functions, intended to be used with usb_debug_only.c
 * http://www.pjrc.com/teensy/
 * Copyright (c) 2008 PJRC.COM, LLC
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#ifndef PRINT_H__
#define PRINT_H__ 1

#include <stdint.h>
#include <stdbool.h>
#include <avr/pgmspace.h>
#include "xprintf.h"
#include "util.h"


// this macro allows you to write print("some text") and
// the string is automatically placed into flash memory :)
#define print(s)                print_P(PSTR(s))
#define println(s)              print_P(PSTR(s "\n"))

/* for old name */
#define pdec(data)              print_dec(data)
#define pdec16(data)            print_dec(data)
#define phex(data)              print_hex8(data)
#define phex16(data)            print_hex16(data)
#define pbin(data)              print_bin8(data)
#define pbin16(data)            print_bin16(data)
#define pbin_reverse(data)      print_bin_reverse8(data)
#define pbin_reverse16(data)    print_bin_reverse16(data)

/* print value utility */
#define print_val_dec(v)           xprintf(#v ": %u\n", v)
#define print_val_decs(v)          xprintf(#v ": %d\n", v)
#define print_val_hex8(v)          xprintf(#v ": %X\n", v)
#define print_val_hex16(v)         xprintf(#v ": %02X\n", v)
#define print_val_hex32(v)         xprintf(#v ": %04lX\n", v)
#define print_val_bin8(v)          xprintf(#v ": %08b\n", v)
#define print_val_bin16(v)         xprintf(#v ": %016b\n", v)
#define print_val_bin32(v)         xprintf(#v ": %032lb\n", v)
#define print_val_bin_reverse8(v)  xprintf(#v ": %08b\n", bitrev(v))
#define print_val_bin_reverse16(v) xprintf(#v ": %016b\n", bitrev16(v))
#define print_val_bin_reverse32(v) xprintf(#v ": %032lb\n", bitrev32(v))



#ifndef NO_PRINT

#ifdef __cplusplus
extern "C" {
#endif

/* function pointer of sendchar to be used by print utility */
void print_set_sendchar(int8_t (*print_sendchar_func)(uint8_t));

/* print string stored in data memory(SRAM)
 *     print_S("hello world");
 * This consumes precious SRAM memory space for string.
 */
void print_S(const char *s);

void print_lf(void);
void print_crlf(void);


/* print string stored in program memory(FLASH)
 *     print_P(PSTR("hello world");
 * This consumes relatively abundant FLASH memory area not SRAM.
 */
#define print_P(s)          xputs(s)

/* decimal */
#define print_dec(i)        xprintf("%u", i)
#define print_decs(i)       xprintf("%d", i)

/* hex */
#define print_hex4(i)       xprintf("%X", i)
#define print_hex8(i)       xprintf("%02X", i)
#define print_hex16(i)      xprintf("%04X", i)
#define print_hex32(i)      xprintf("%08lX", i)

/* binary */
#define print_bin4(i)       xprintf("%04b", i)
#define print_bin8(i)       xprintf("%08b", i)
#define print_bin16(i)      xprintf("%016b", i)
#define print_bin32(i)      xprintf("%032lb", i)

#define print_bin_reverse8(i)   xprintf("%08b", bitrev(i))
#define print_bin_reverse16(i)  xprintf("%016b", bitrev16(i))
#define print_bin_reverse32(i)  xprintf("%032lb", bitrev32(i))

#ifdef __cplusplus
}
#endif

#else

#define print_set_sendchar(func)
#define print_S(s)
#define print_P(s)
#define print_dec(data)
#define print_decs(data)
#define print_hex4(data)
#define print_hex8(data)
#define print_hex16(data)
#define print_hex32(data)
#define print_bin4(data)
#define print_bin8(data)
#define print_bin16(data)
#define print_bin32(data)
#define print_bin_reverse8(data)
#define print_bin_reverse16(data)
#define print_bin_reverse32(data)

#endif


#endif
