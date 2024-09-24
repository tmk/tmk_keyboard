/*
Copyright 2024 Jun Wako <wakojun@gmail.com>
*/
#ifndef SOFTWARE_UART_H
#define SOFTWARE_UART_H
#include "ringbuf.h"
#include <util/delay.h>


// timer tick per bit
#define TICK_PER_BIT(_baud, _prescale)      (F_CPU / _prescale / _baud - 1)
#define TICK_PER_0_5_BIT(_baud, _prescale)  (F_CPU / _prescale / _baud / 2 - 1)
#define TICK_PER_1_5_BIT(_baud, _prescale)  (F_CPU / _prescale / _baud / 2 * 3 - 1)

// pin register
#define PIN_PORT(_pin)  *((_pin).port)
#define PIN_DDR(_pin)   *((_pin).port - 1)
#define PIN_PIN(_pin)   *((_pin).port - 2)
#define PIN_MASK(_pin)  (1 << (_pin).bit)
#define PIN_STATE(_pin) !!(PIN_PIN(_pin) & PIN_MASK(_pin))

// pin assert for negative logic
#define ASSERT1(_s) do { PIN_PORT(_s->send_pin) &= ~PIN_MASK(_s->send_pin); } while (0)
#define ASSERT0(_s) do { PIN_PORT(_s->send_pin) |=  PIN_MASK(_s->send_pin); } while (0)
/*
#define ASSERT1(_s)     do { \
    if (_s->logic == LOGIC_NEGATIVE) \
	PIN_PORT(_s->send_pin) &= ~PIN_MASK(_s->send_pin); \
    else \
	PIN_PORT(_s->send_pin) |=  PIN_MASK(_s->send_pin); \
} while (0)
#define ASSERT0(_s)     do { \
    if (_s->logic == LOGIC_NEGATIVE) \
	PIN_PORT(_s->send_pin) |=  PIN_MASK(_s->send_pin); \
    else \
	PIN_PORT(_s->send_pin) &= ~PIN_MASK(_s->send_pin); \
} while (0)
*/


// TODO: parity, logic, order
// uart configurations
enum parity {
    PARITY_NONE,
    PARITY_ODD,
    PARITY_EVEN
};

enum order {
    ORDER_LSB,
    ORDER_MSB
};

enum logic {
    LOGIC_POSITIVE = 0,
    LOGIC_NEGATIVE = 1
};

typedef struct pin {
    volatile uint8_t *port;
    uint8_t bit;
} pin_t;

typedef struct {
    pin_t recv_pin;
    pin_t send_pin;
    ringbuf_t *rbuf;
    ringbuf_t *sbuf;
/*
    uint8_t rbit;
    uint8_t sbit;
    uint8_t rdata;
    uint8_t sdata;

    uint8_t parity;
    bool order;
    bool logic;
*/
} suart_t;


void suart_init(const suart_t *s)
{
    // recv pin: input with pull-up
    if (s->recv_pin.port) {
        PIN_DDR(s->recv_pin)  &= ~PIN_MASK(s->recv_pin);
        PIN_PORT(s->recv_pin) |=  PIN_MASK(s->recv_pin);
    }

    // send pin: set to idle(1)
    if (s->send_pin.port) {
        PIN_DDR(s->send_pin)  |=  PIN_MASK(s->send_pin);
        ASSERT1(s);
    }

}

int16_t suart_receive(const suart_t *s)
{
    int16_t d;
    cli();
    d = ringbuf_get(s->rbuf);
    sei();
    return d;
}

void suart_send(const suart_t *s, uint8_t data)
{
    cli();
    ringbuf_put(s->sbuf, data);
    sei();
}


#if 0
void suart_receive_in_isr(suart_t *_suart) {
}

// should be called repeatedly in bit width interval
void suart_send_in_isr(suart_t *_suart) {
    switch (_suart->sbit) {
    case 0: /* start bit */
        if (ringbuf_is_empty(_suart->sbuf)) return;
        _suart->sdata = ringbuf_get(_suart->sbuf);
        ASSERT0(_suart);
        _suart->sbit++;
        break;
    case 1: case 2: case 3: case 4: case 5: case 6: case 7: case 8: /* data */
        if (_suart->sdata & 1) {
            ASSERT1(_suart);
        } else {
            ASSERT0(_suart);
        }
        _suart->sdata >>= 1;
        _suart->sbit++;
        break;
    case 9: /* stop bit */
        ASSERT1(_suart);
        _suart->sbit++;
        break;
    case 10: /* done */
        _suart->sbit = 0;
        _suart->sdata = 0;
        break;
    }
}
#endif

#endif
