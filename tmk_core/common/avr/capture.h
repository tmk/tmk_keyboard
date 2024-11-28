#ifndef CAPTURE_H
#define CAPTURE_H

// capture.h should be included in every compile unit when defining 'fixed register'
//#include "config.h"

#ifndef __ASSEMBLER__

#ifdef __cplusplus
extern "C" {
#endif

/*
 * This is based on Soarers's work on sctrace
 * https://deskthority.net/viewtopic.php?t=4567
 */


// timer overflow count
extern volatile uint8_t tovf;

// circular buffer queue for capture data
extern volatile uint8_t *iqhead;
extern volatile uint8_t *iqtail;

void capture_init(void);

// print capture data in iqueue
void print_capture(void);
void print_capture_all(void);

#if defined(__AVR__)
// pin to be captured
#define CAPTURE_PIN         PIND

// pin state stored in capture()
#define CAPTURE_PIN_STORED  _SFR_IO8(0x20)

// store 4-byte capture records in SRAM iqueue area(0x0100-0x02FF)
// capture() takes around 4-5us
static inline void capture(void) __attribute__ ((always_inline));
static inline void capture(void) {
    asm volatile (
        "push   r0"                     "\n\t"
        "in     r0,     %[pin]"         "\n\t"

        "push   r1"                     "\n\t"
        "in     r1,     __SREG__"       "\n\t"
        "push   r1"                     "\n\t"
        "push   r16"                    "\n\t"

        "push   r26"                    "\n\t"
        "push   r27"                    "\n\t"
        "lds    r26,    %[iqh]"         "\n\t"  /* X: iqhead */
        "lds    r27,    %[iqh]+1"       "\n\t"

        "lds    r1,     %[tov]"         "\n\t"
        "sbic   0x16,   0"              "\n\t"  /* tov++ if TIFR1:TOV1 == 1 */
        "inc    r1"                     "\n\t"
        "tst    r1"                     "\n\t"
        "breq   2f"                     "\n\t"  /* if tovf == 0 */

        /* tovf record if tovf != 0 */
        "in     r16,    %[sto]"         "\n\t"  /* last pin state */
        "andi   r16,    0x0f"           "\n\t"
        "st     X+,     r16"            "\n\t"  /* [0]: 0x00 | pin state */
        "st     X+,     r1"             "\n\t"  /* [1]: tovf */

        "eor    r1,     r1"             "\n\t"
        "sts    %[tov], r1"             "\n\t"  /* tovf = 0 */

        "cpi    r27,    3"              "\n\t"  /* if (iqhead == 0x0300) iqhead = 0x0100; */
        "brne   2f"                     "\n\t"
        "ldi    r27,    1"              "\n\t"

        /* pin change record if tovf == 0 */
    "2:"
        "sbi    0x16,   0"              "\n\t"  /* "write" to clear TIFR1:TOV1 cancel overflow interrupt */
        "out    %[sto], r0"             "\n\t"  /* Store pin state into unused IO address */
        "ldi    r16,    0x0f"           "\n\t"
        "and    r0,     r16"            "\n\t"  /* pin & 0x0f */

        "lds    r16,    %[tcl]"         "\n\t"
        "andi   r16,    0xf0"           "\n\t"  /* TCNT1L & 0xf0 */
        "or     r0,     r16"            "\n\t"
        "st     X+,     r0"             "\n\t"  /* [0]: timer low | pin state */

        "lds    r0,     %[tch]"         "\n\t"
        "st     X+,     r0"             "\n\t"  /* [1]: timer high */

        "sts    %[iqh], r26"            "\n\t"
        "cpi    r27,    3"              "\n\t"  /* if (iqhead == 0x0300) iqhead = 0x0100; */
        "brne   1f"                     "\n\t"
        "ldi    r27,    1"              "\n\t"
    "1:"                                "\n\t"
        "sts    %[iqh]+1, r27"          "\n\t"

        "pop    r27"                    "\n\t"
        "pop    r26"                    "\n\t"
        "pop    r16"                    "\n\t"
        "pop    r1"                     "\n\t"
        "out    __SREG__, r1"           "\n\t"
        "pop    r1"                     "\n\t"
        "pop    r0"                     "\n\t"
        : [iqh] "+X" (iqhead),
          [tov] "+X" (tovf)
        : [pin] "I" (_SFR_IO_ADDR(CAPTURE_PIN)),
          [sto] "X" (_SFR_IO_ADDR(CAPTURE_PIN_STORED)),
          [tcl] "X" (TCNT1L),
          [tch] "X" (TCNT1H),
          [tfr] "I" (_SFR_IO_ADDR(TIFR1))
    );
}
#else
#   error "This microcontroller doesn't support, currently only for AVR"
#endif

#ifdef __cplusplus
}
#endif

#endif

#endif
