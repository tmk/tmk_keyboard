/*--------------------------------------------------------------------
 * Ring buffer to store scan codes from keyboard
 *------------------------------------------------------------------*/

#ifndef PBUFF_H
#define PBUFF_H

#include "print.h"

#define PBUF_SIZE 32
static uint16_t pbuf[PBUF_SIZE];
static uint16_t pbuf_head = 0;
static uint16_t pbuf_tail = 0;
static inline void pbuf_enqueue(uint16_t data)
{
    uint8_t sreg = SREG;
    cli();
    uint16_t next = (pbuf_head + 1) % PBUF_SIZE;
    if (next != pbuf_tail) {
        pbuf[pbuf_head] = data;
        pbuf_head = next;
    } else {
        print("pbuf: full\n");
    }
    SREG = sreg;
}
static inline uint16_t pbuf_dequeue(void)
{
    uint16_t val = 0;

    uint8_t sreg = SREG;
    cli();
    if (pbuf_head != pbuf_tail) {
        val = pbuf[pbuf_tail];
        pbuf_tail = (pbuf_tail + 1) % PBUF_SIZE;
    }
    SREG = sreg;

    return val;
}
static inline bool pbuf_has_data(void)
{
    uint8_t sreg = SREG;
    cli();
    bool has_data = (pbuf_head != pbuf_tail);
    SREG = sreg;
    return has_data;
}
static inline void pbuf_clear(void)
{
    uint8_t sreg = SREG;
    cli();
    pbuf_head = pbuf_tail = 0;
    SREG = sreg;
}

#endif
