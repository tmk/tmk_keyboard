#include <avr/interrupt.h>
#include "sendchar.h"
#include "capture.h"

// iqueue placed at SRAM 0x0100-02FF before .data section
// place this in Makeilfe: EXTRALDFLAGS += -Wl,--section-start,.data=0x800300
volatile uint8_t *iqhead = (uint8_t *)0x0100;
volatile uint8_t *iqtail = (uint8_t *)0x0100;

void capture_init(void)
{
    // Timer1 - Prescaler:clk/1(clk=16MHz), Mode:normal(overflow at 0xFFFF)
    TCCR1A = 0x00;
    TCCR1B = 0x01;
    TIMSK1 = (1<<TOIE1);
}

static void print_hh(uint8_t v)
{
    sendchar((v >> 4) + ((v >> 4) < 10 ? '0' : 'A' - 10));
    sendchar((v & 0x0F) + ((v & 0x0F) < 10 ? '0' : 'A' - 10));
}

void print_capture(void)
{
    if (iqhead == iqtail) return;

    // Capture record in iqueue buffer:
    // Pin change:
    //     [0]: Timer low | Pin
    //     [1]: Timer high
    // Timer overflow:
    //     [0]: 0         | Pin(no change)
    //     [1]: Overflow count

    // Print record data in hexdecimal with format below:
    print_hh(*(iqtail+1));
    print_hh(*(iqtail+0));
    sendchar(' ');

    iqtail += 2;
    if (iqtail == (uint8_t *)0x0300)
        iqtail = (uint8_t *)0x0100;
}

void print_capture_all(void)
{
    while (iqhead != iqtail) {
        print_capture();
    }
}


volatile uint8_t tovf = 0;
ISR(TIMER1_OVF_vect, ISR_NAKED)
{
    asm volatile (
        "push   r0"                     "\n\t"
        "in     r0, __SREG__"           "\n\t"
        "push   r0"                     "\n\t"

        "lds    r0, %[tov]"             "\n\t"
        "inc    r0"                     "\n\t"
        "sts    %[tov], r0"             "\n\t"
        "brne   tovf_end"               "\n\t"

        "push   r26"                    "\n\t"
        "push   r27"                    "\n\t"
        "lds    r26,    %[iqh]"         "\n\t"
        "lds    r27,    %[iqh]+1"       "\n\t"

        "push   r16"                    "\n\t"
        "in     r16,    %[sto]"         "\n\t"  /* last pin state */
        "andi   r16,    0x0f"           "\n\t"
        "st     X+,     r16"            "\n\t"  /* [0]: 0x00 | pin state */

        "ldi    r16,    0x00"           "\n\t"
        "st     X+,     r16"            "\n\t"  /* [1]: 0x00 */

        "sts    %[iqh], r26"            "\n\t"  /* if (iqhead == 0x0300) iqhead = 0x0100; */
        "cpi    r27,    3"              "\n\t"
        "brne   1f"                     "\n\t"
        "ldi    r27,    1"              "\n\t"
    "1:"                                "\n\t"
        "sts    %[iqh]+1, r27"          "\n\t"

        "pop    r16"                    "\n\t"
        "pop    r27"                    "\n\t"
        "pop    r26"                    "\n\t"

    "tovf_end:"                         "\n\t"
        "pop    r0"                     "\n\t"
        "out    __SREG__, r0"           "\n\t"
        "pop    r0"                     "\n\t"
        "reti"                          "\n\t"
        : [tov] "+X" (tovf),
          [iqh] "+X" (iqhead)
        : [sto] "X" (_SFR_IO_ADDR(CAPTURE_PIN_STORED))
    );
}
