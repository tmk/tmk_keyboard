/*
Copyright 2024 Jun Wako <wakojun@gmail.com>
*/

#include <stdint.h>
#include <stdbool.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "print.h"
#include "util.h"
#include "matrix.h"
#include "debug.h"
#include "software_uart.h"
#include "led.h"
#include "host.h"
#include "timer.h"
#include "wait.h"
#include "mouse.h"
#include "ringbuf16.h"


//debug
#if !defined(NEXT_DEBUG)
#   define NEXT_DEBUG 0
#endif
#if NEXT_DEBUG
#   define NEXT_DEBUG_PIN_INIT()    do { DDRD |= (1 << 4); PORTD |= (1 << 4); } while (0)
#   define NEXT_DEBUG_PIN_TOGGLE()  do { PIND |= (1 << 4); } while (0)
#else
#   define NEXT_DEBUG_PIN_INIT()
#   define NEXT_DEBUG_PIN_TOGGLE()
#endif

// Timer count
#define BAUD                18958
#define PRESCALE            8
#define TCNT_PER_BIT        TICK_PER_BIT(BAUD, PRESCALE)
#define TCNT_PER_0_5_BIT    TICK_PER_0_5_BIT(BAUD, PRESCALE)
#define TCNT_PER_1_5_BIT    TICK_PER_1_5_BIT(BAUD, PRESCALE)

// PSW pin
pin_t psw_pin = { &PORTD, 4 };

//
// keyboard uart
//
static uint16_t _rb[4];
static ringbuf_t _rbuf = {
    .buffer = _rb,
    .size_mask = 3
};
static uint16_t _sb[4];
static ringbuf_t _sbuf = {
    .buffer = _sb,
    .size_mask = 3
};
static suart_t next_suart = {
    .recv_pin = { &PORTD, 0 },
    .send_pin = { &PORTD, 1 },
    .rbuf = &_rbuf,
    .sbuf = &_sbuf
/*
    .parity = PARITY_NONE,
    .order = ORDER_LSB,
    .logic = LOGIC_POSITIVE,
*/
};

static void next_send(uint16_t data)
{
    suart_send(&next_suart, data);
    if (data != 0x110 && data != 0x111) xprintf("s%03X ", data);
}

// receive keyboard data
ISR(INT0_vect)
{
    NEXT_DEBUG_PIN_TOGGLE();
    // start bit falling edge
    // setup TIMER1_COMPB interrupt to sample at mid of bit width
    //OCR1B = (TCNT1 + TCNT_PER_0_5_BIT) % TCNT_PER_BIT;    // this calculate takes long
    OCR1B = (TCNT1 + TCNT_PER_0_5_BIT);
    if (OCR1B > TCNT_PER_BIT)
        OCR1B -= TCNT_PER_BIT;
    NEXT_DEBUG_PIN_TOGGLE();

    // disable sending
    TIMSK1 &= ~(1 << OCIE1A);   // disable TIMER1_COMPA interrupt
    TIFR1  |=  (1 << OCF1A);    // clear TIMER1_COMPA interrupt flag

    TIFR1  |=  (1 << OCF1B);    // clear TIMER1_COMPB interrupt flag
    TIMSK1 |=  (1 << OCIE1B);   // enable TIMER1_COMPB interrupt
    EIMSK  &= ~(1 << INT0);     // disable INT0 interrupt
    EIFR   |=  (1 << INTF0);    // clear INT0 interrupt flag
}

ISR(TIMER1_COMPB_vect)
{
    NEXT_DEBUG_PIN_TOGGLE();
    suart_t _suart = next_suart;

    static uint8_t bit = 0;
    static uint8_t data = 0;
    static bool xbit = false;

    switch (bit) {
    case 0:
        // start bit
        bit++;
        break;
    case 1: case 2: case 3: case 4: case 5: case 6: case 7: case 8:
        // data bit
        data >>= 1;
        if (PIN_STATE(_suart.recv_pin)) {
            data |= 0x80;
        }
        bit++;
        break;
    case 9:
        // X bit
        xbit = PIN_STATE(_suart.recv_pin);
        bit++;
        break;
    case 10:
        // stop bit
        if (PIN_STATE(_suart.recv_pin)) {
            // no event when xbit is 1 and data is 0
            //if (!xbit) {
                ringbuf_put(_suart.rbuf, (xbit << 8) | data);
            //}
        }

        // enable sending
        TIFR1  |=  (1 << OCF1A);    // clear TIMER1_COMPA interrupt flag
        TIMSK1 |=  (1 << OCIE1A);   // enable TIMER1_COMPA interrupt

        TIMSK1 &= ~(1 << OCIE1B);   // disable TIMER1_COMPB interrupt
        TIFR1  |=  (1 << OCF1B);    // clear TIMER1_COMPB interrupt flag
        EIFR   |=  (1 << INTF0);    // clear INT0 interrupt flag
        EIMSK  |=  (1 << INT0);     // enable INT0 interrupt
        bit = 0;
        data = 0;
        break;
    }
    NEXT_DEBUG_PIN_TOGGLE();
}

// send keyboard data
ISR(TIMER1_COMPA_vect)
{
    suart_t *_suart = &next_suart;
    static uint8_t bit = 0;
    static int16_t data = 0;

    switch (bit) {
    case 0:
        // start bit
        data = ringbuf_get(_suart->sbuf);
        if (data == -1) return;
        NEXT_DEBUG_PIN_TOGGLE();
        ASSERT0(_suart);
        bit++;
        break;
    case 1: case 2: case 3: case 4: case 5: case 6: case 7: case 8:
        // data bit
        NEXT_DEBUG_PIN_TOGGLE();
        if (data & (1 << (bit - 1))) {
            ASSERT1(_suart);
        } else {
            ASSERT0(_suart);
        }
        bit++;
        break;
    case 9:
        // X bit
        NEXT_DEBUG_PIN_TOGGLE();
        if (data & 0x100) {
            ASSERT1(_suart);
        } else {
            ASSERT0(_suart);
        }
        bit++;
        break;
    case 10:
        // stop bit
        NEXT_DEBUG_PIN_TOGGLE();
        ASSERT1(_suart);
        bit++;
        break;
    case 11:
        // done
        NEXT_DEBUG_PIN_TOGGLE();
        bit = 0;
        data = 0;
        break;
    }
    NEXT_DEBUG_PIN_TOGGLE();
}

void hook_early_init(void)
{
    // psw pin is used when debug
    NEXT_DEBUG_PIN_INIT();

#if NEXT_DEBUG == 0
    // psw pin: input with pull-up
    PIN_DDR(psw_pin)  &= ~PIN_MASK(psw_pin);
    PIN_PORT(psw_pin) |=  PIN_MASK(psw_pin);
#endif

    // Timer1 is cofigured as CTC mode.
    // OCR1A is set to 1-bit period at startup and not updated. OCR1A is used as TOP.
    // COMPA is triggered repeatedly to send data when there is data in send queue.
    // OCR1B/C is set dynamically to sample data at center of bits when start
    // bit is detected. ( OCR1B/C = (TCNT1 + 0.5-bit_tick) % TOP )
    // COMPB/C is enabled at start bit and disabled at stop bit.
    //
    //     - - -|--------|--------|------ - - - - - - |--------|--------|--------|- - COMPA(send)
    //    0 ____          ________ ______ _ _ _ _ _ _  ________ ________ ________________________ _ _ _ _
    //    1     \________/________X______ _ _ _ _ _ _ X________X________X________/
    //          |  start   data0     data1               data7             stop
    //          |    |--------|--------|- - - - - - - - --|--------|--------| COMPB/COMPC(receive)
    //          \_ falling edge interrupt
    //
    //                     bit period
    //            - - -|------|------|------|------|------|- - - COMPA
    //      TOP        *      *      *      *      *      *   OCR1A(keyboard send)
    //                /|     /|     /|     /|     /|     /|
    //               / |    * |    * |    * |    * |    * |   OCR1B(keyboard receive)
    //    TCNT1     /  |   /  |   /  |   /  |   /  |   /  |   OCR1C(not used)
    //             /   |  /   |  /   |  /   |  /   |  /   |
    //            /    | /    | /    | /    | /    | /    |
    //           /     |/     |/     |/     |/     |/     |/
    //        0 ---------------------------------------------
    //                      |------|------|------|------|- - - COMPB/COMPC
    TCCR1A =  0x00;
    TCCR1B =  (1 << WGM12) | (1 << CS11);   // CTC, clk/8 prescaling
    OCR1A  =  TCNT_PER_BIT;                 // for send isr
    TIMSK1 =  (1 << OCIE1A);                // enable TIMER1_COMPA interrupt for sending

    // suart init
    suart_init(&next_suart);
    // enable interrupt on falling edge for keyboard
    EICRA  |=  (1 << ISC01) | (0 << ISC00);
    EIMSK  |=  (1 << INT0);

    sei();
}



/*
 * Next keyboard
 *
 * scan code format: 1 byte
 *
 *  76543210
 *  Rrrrrccc
 *  ||  |\_\___ column(3-bit) 0-7
 *  |\__\______ row(4-bit) 0-0xF
 *  \__________ release flag(1:relase)
 */
static matrix_row_t matrix[MATRIX_ROWS];
#define ROW(code)       (((code) >> 3) & 0xF)
#define COL(code)       ((code) & 0x07)
#define key_on(code)    do { matrix[ROW(code)] |=  (1 << COL(code)); } while (0)
#define key_off(code)   do { matrix[ROW(code)] &= ~(1 << COL(code)); } while (0)

#define NEXT_RALT       0x51
#define NEXT_LALT       0x52
#define NEXT_RCMD       0x53
#define NEXT_LCMD       0x54
#define NEXT_RSFT       0x55
#define NEXT_LSFT       0x56
#define NEXT_LCTL       0x57
#define NEXT_PSW        0x58
#define NEXT_LED_L      (1 << 0)
#define NEXT_LED_R      (1 << 1)

static void interp_mods(uint8_t mods)
{
    if (mods & 0b00000001) { key_on(NEXT_LCTL); } else { key_off(NEXT_LCTL); }
    if (mods & 0b00000010) { key_on(NEXT_LSFT); } else { key_off(NEXT_LSFT); }
    if (mods & 0b00000100) { key_on(NEXT_RSFT); } else { key_off(NEXT_RSFT); }
    if (mods & 0b00001000) { key_on(NEXT_LCMD); } else { key_off(NEXT_LCMD); }
    if (mods & 0b00010000) { key_on(NEXT_RCMD); } else { key_off(NEXT_RCMD); }
    if (mods & 0b00100000) { key_on(NEXT_LALT); } else { key_off(NEXT_LALT); }
    if (mods & 0b01000000) { key_on(NEXT_RALT); } else { key_off(NEXT_RALT); }
}

static volatile bool led_changed = false;
static uint8_t next_led(void)
{
    uint8_t usb_led = host_keyboard_leds();
    uint8_t next_led = 0;
    if (usb_led & (1<<USB_LED_NUM_LOCK))    next_led =              NEXT_LED_R;
    if (usb_led & (1<<USB_LED_SCROLL_LOCK)) next_led = NEXT_LED_L;
    if (usb_led & (1<<USB_LED_CAPS_LOCK))   next_led = NEXT_LED_L | NEXT_LED_R;
    return next_led;
}

static void next_reset(void)
{
    next_send(0x1EF);
    next_send(0x000);
}

static void next_set_led(void)
{
    next_send(0x100);
    next_send(next_led());
}

void hook_late_init(void)
{
    next_reset();
    wait_ms(100);
}

void matrix_clear(void)
{
    for (uint8_t i=0; i < MATRIX_ROWS; i++) matrix[i] = 0x00;
}

void matrix_init(void)
{
    matrix_clear();

    // to set led at startup
    led_changed = true;
}

uint8_t matrix_scan(void)
{
#if NEXT_DEBUG == 0
    // psw debounce
    static uint8_t psw_bounce = 0;
    static uint16_t psw_t = 0;
    if (timer_elapsed(psw_t) > 0) {
        if (!PIN_STATE(psw_pin)) { psw_bounce |= 1; }
        if (psw_bounce == 0x7F) { key_on(NEXT_PSW); }
        if (psw_bounce == 0x80) { key_off(NEXT_PSW); }
        psw_bounce <<= 1;
        psw_t = timer_read();
    }
#endif

    // query
    static uint16_t last_query = 0;
    if (timer_elapsed(last_query) < 8) {
        return 0;
    }

    // send keyboard query
    next_send(0x110);
    last_query = timer_read();

    // wait for response with timeout
    int16_t data1, data2;
    do {
        data1 = suart_receive(&next_suart);
    } while (data1 == -1 && timer_elapsed(last_query) < 4);
    if (data1 == -1) {
        next_reset();
        matrix_init();
        wait_ms(100);
        return 0;
    }

    do {
        data2 = suart_receive(&next_suart);
    } while (data2 == -1 && timer_elapsed(last_query) < 4);
    if (data2 == -1) {
        next_reset();
        matrix_init();
        wait_ms(100);
        return 0;
    }

    // update led
    if (led_changed) {
        next_set_led();
        wait_ms(2);
        led_changed = false;
    }

    // no event
    if (data1 == 0x100 && data2 == 0x100) return 0;
    // not expected
    if (data1 & 0x100 || data2 & 0x100) {
        xprintf("%03X%03X? ", data1, data2);
        return 0;
    }

    xprintf("%02X%02X ", data1, data2);
    switch (data1) {
    case 0x00:
        // not expected
        print("? ");
        break;
    case 0x80:
        // mods: 0x80, 0xMM
        if ((data2 & 0x80) == 0) {
            interp_mods(data2);
        } else {
            print("? ");
        }
        break;
    default:
        // key: 0xKK, 0xMM
        if (data1 & 0x80) {
            key_off(data1 & 0x7F);
        } else {
            key_on(data1 & 0x7F);
        }
        interp_mods(data2 & 0x7F);
        break;
    }

    return 1;
}

uint8_t matrix_get_row(uint8_t row)
{
    return matrix[row];
}

void led_set(uint8_t usb_led)
{
    led_changed = true;
    xprintf("L:%02X ", usb_led);
}


// mouse
void hook_main_loop(void)
{
    static uint16_t last_query = 0;
    if (timer_elapsed(last_query) < 8) {
        return;
    }

    // send mouse query
    next_send(0x111);
    last_query = timer_read();

    // wait for response with timeout
    int16_t data1, data2;
    do {
        data1 = suart_receive(&next_suart);
    } while (data1 == -1 && timer_elapsed(last_query) < 4);
    if (data1 == -1) {
        return;
    }

    do {
        data2 = suart_receive(&next_suart);
    } while (data2 == -1 && timer_elapsed(last_query) < 4);
    if (data2 == -1) {
        return;
    }

    // no event
    if (data1 == 0x100 && data2 == 0x100) return;
    // not expected
    if (data1 & 0x100 || data2 & 0x100) {
        xprintf("M:%03X%03X? ", data1, data2);
        return;
    }

    dprintf("M:%02X%02X ", data1, data2);

    // TODO: mouse coordinates direction
    report_mouse_t mouse_report = {};
    mouse_report.buttons = 0;
    if ((data1 & 0x01) == 0) mouse_report.buttons |= MOUSE_BTN1;
    if ((data2 & 0x01) == 0) mouse_report.buttons |= MOUSE_BTN2;
    mouse_report.x = -((int8_t)(data1 & 0xFE) / 2);
    mouse_report.y = -((int8_t)(data2 & 0xFE) / 2);
    mouse_report.x = (mouse_report.x > -127 ? mouse_report.x : -127);
    mouse_report.y = (mouse_report.y > -127 ? mouse_report.y : -127);
    mouse_send(&mouse_report);
}
