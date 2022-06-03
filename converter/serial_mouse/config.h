/*
Copyright 2022 Jun Wako <wakojun@gmail.com>
*/

#ifndef CONFIG_H
#define CONFIG_H


/* USB Device descriptor parameter */
#define VENDOR_ID       0xFEED
#define PRODUCT_ID      0x31CE
#define DEVICE_VER      0x0001
#define MANUFACTURER    TMK
#define PRODUCT         Serial Mouse
#define DESCRIPTION     TMK keyboard firmware

/* key matrix size */
#define MATRIX_ROWS 1
#define MATRIX_COLS 1

/* key combination for command */
#define IS_COMMAND() ( \
    keyboard_report->mods == (MOD_BIT(KC_LSHIFT) | MOD_BIT(KC_RSHIFT)) \
)



/*
 * Feature disable options
 *  These options are also useful to firmware size reduction.
 */

/* disable debug print */
//#define NO_DEBUG

/* disable print */
//#define NO_PRINT

/* disable action features */
//#define NO_ACTION_LAYER
//#define NO_ACTION_TAPPING
//#define NO_ACTION_ONESHOT
//#define NO_ACTION_MACRO
//#define NO_ACTION_FUNCTION



/*
 * Hardware Serial(UART)
 *   1200 baud, 7-bit data, no parity, 1-bit stop, lsb-first
 */
#if defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega32U2__)
    #define SERIAL_UART_BAUD       1200
    #define SERIAL_UART_DATA       UDR1
    #define SERIAL_UART_UBRR       ((F_CPU/(16UL*SERIAL_UART_BAUD))-1)
    #define SERIAL_UART_RXD_VECT   USART1_RX_vect
    #define SERIAL_UART_TXD_READY  (UCSR1A&(1<<UDRE1))
    #define SERIAL_UART_INIT()     do { \
        UBRR1L = (uint8_t) SERIAL_UART_UBRR;       /* baud rate */ \
        UBRR1H = (uint8_t) (SERIAL_UART_UBRR>>8);  /* baud rate */ \
        UCSR1B |= (1<<RXCIE1) | (1<<RXEN1); /* RX interrupt, RX: enable */ \
        UCSR1B |= (0<<TXCIE1) | (1<<TXEN1); /* TX interrupt, TX: enable */ \
        UCSR1C =  (1<<UCSZ11) | (0<<UCSZ10) | /* data: 7-bit */ \
                  (0<<UPM11)  | (0<<UPM10);  /* parity: none(00), even(01), odd(11) */ \
        DDRD  &= ~(1<<2); PORTD |=  (1<<2); /* Pull-up RXD pin */ \
        sei(); \
    } while(0)
#else
    #error "USART configuration is needed."
#endif


/*
 * Software Serial
 *   1200 baud, 7-bit data, no parity, 1-bit stop, lsb-first
 */
#define SERIAL_SOFT_BAUD                1200
#define SERIAL_SOFT_PARITY_NONE
#define SERIAL_SOFT_BIT_ORDER_LSB
//#define SERIAL_SOFT_LOGIC_NEGATIVE
#define SERIAL_SOFT_DATA_7BIT
#define SERIAL_SOFT_DEBUG
/* RXD Port */
#define SERIAL_SOFT_RXD_DDR             DDRD
#define SERIAL_SOFT_RXD_PORT            PORTD
#define SERIAL_SOFT_RXD_PIN             PIND
#define SERIAL_SOFT_RXD_BIT             2
#define SERIAL_SOFT_RXD_READ()          (SERIAL_SOFT_RXD_PIN&(1<<SERIAL_SOFT_RXD_BIT))
/* RXD Interupt */
#define SERIAL_SOFT_RXD_VECT            INT2_vect
#define SERIAL_SOFT_RXD_INIT()          do { \
    /* pin configuration: input with pull-up */ \
    SERIAL_SOFT_RXD_DDR &= ~(1<<SERIAL_SOFT_RXD_BIT); \
    SERIAL_SOFT_RXD_PORT |= (1<<SERIAL_SOFT_RXD_BIT); \
    /* enable interrupt: INT2(Positive:falling edge, Negative:rising edge) */ \
    EICRA |= ((1<<ISC21)|(0<<ISC20)); \
    EIMSK |= (1<<INT2); \
    sei(); \
} while (0)
#define SERIAL_SOFT_RXD_INT_ENTER()
#define SERIAL_SOFT_RXD_INT_EXIT()      do { \
    /* clear interrupt  flag */ \
    EIFR = (1<<INTF2); \
} while (0)
/* TXD Port */
#define SERIAL_SOFT_TXD_DDR             DDRD
#define SERIAL_SOFT_TXD_PORT            PORTD
#define SERIAL_SOFT_TXD_PIN             PIND
#define SERIAL_SOFT_TXD_BIT             3
#define SERIAL_SOFT_TXD_HI()            do { SERIAL_SOFT_TXD_PORT |=  (1<<SERIAL_SOFT_TXD_BIT); } while (0)
#define SERIAL_SOFT_TXD_LO()            do { SERIAL_SOFT_TXD_PORT &= ~(1<<SERIAL_SOFT_TXD_BIT); } while (0)
#define SERIAL_SOFT_TXD_INIT()          do { \
    /* pin configuration: output */ \
    SERIAL_SOFT_TXD_DDR |= (1<<SERIAL_SOFT_TXD_BIT); \
    /* idle */ \
    SERIAL_SOFT_TXD_ON(); \
} while (0)


#endif
