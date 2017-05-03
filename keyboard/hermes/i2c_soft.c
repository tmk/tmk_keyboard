#include "i2c_soft.h"
#include <util/delay.h>
#include <avr/io.h>
#include "print.h"

// SDA: F5
// SCL: F4

void i2c_init(void) {
    // Set SDA high.
    DDRF |= (1 << SDA_PIN);
    PORTF |= (1 << SDA_PIN);
    // Set SCL high.
    DDRF |= (1 << SCL_PIN);
    PORTF |= (1<< SCL_PIN);
}

void i2c_uninit(void) {
    PORTF &= ~(1 << SDA_PIN);
    DDRF &= ~(1 << SDA_PIN);
    PORTF &= ~(1 << SCL_PIN);
    DDRF &= ~(1 << SCL_PIN);
}

void i2c_start(void) {
    PORTF &= ~(1 << SDA_PIN);
    _delay_us(I2C_DELAY_USEC);
    PORTF &= ~(1 << SCL_PIN);
    _delay_us(I2C_DELAY_USEC);
}

void i2c_restart(void) {
    PORTF |= (1 << SDA_PIN);
    PORTF |= (1 << SCL_PIN);
    _delay_us(I2C_DELAY_USEC);
}

void i2c_stop(void) {
    PORTF &= ~(1 << SDA_PIN);
    _delay_us(I2C_DELAY_USEC);
    PORTF |= (1 << SCL_PIN);
    _delay_us(I2C_DELAY_USEC);
    PORTF |= (1 << SDA_PIN);
    _delay_us(I2C_DELAY_USEC);
}

uint8_t i2c_send(uint8_t data) {
    // write
    for (uint8_t m = 0x80; m != 0; m >>= 1) {
        if (m & data) PORTF |= (1 << SDA_PIN);
        else PORTF &= ~(1 << SDA_PIN);

        PORTF |= (1 << SCL_PIN);
        _delay_us(I2C_DELAY_USEC);
        PORTF &= ~(1 << SCL_PIN);
        _delay_us(I2C_DELAY_USEC);
    }

    // get response
    DDRF &= ~(1 << SDA_PIN);
    PORTF |= (1 << SDA_PIN);
    PORTF |= (1 << SCL_PIN);
    _delay_us(I2C_DELAY_USEC);
    uint8_t ret = bit_is_set(PINF, SDA_PIN);
    PORTF &= ~(1 << SCL_PIN);
    _delay_us(I2C_DELAY_USEC);
    DDRF |= (1 << SDA_PIN);
    PORTF &= ~(1 << SDA_PIN);
    return ret;
}

// send multiple bytes
uint8_t i2c_send_data(const uint8_t* data, uint8_t len) {
    uint8_t error = 0;
    i2c_start();

    for (uint8_t i = 0; i < len; i++) {
        if (i2c_send(data[i])) {
            error = 1;
            break;
        }
        _delay_us(I2C_DELAY_USEC);
    }
    
    i2c_stop();
    _delay_us(I2C_DELAY_USEC);
    return error;
}

uint8_t i2c_read(uint8_t last) {
    uint8_t b = 0;
    // enable pullup
    PORTF |= (1 << SDA_PIN);
    DDRF &= ~(1 << SDA_PIN);

    for (uint8_t i = 0; i < 8; i++) {
        b <<= 1;
        PORTF |= (1 << SCL_PIN);
        _delay_us(I2C_DELAY_USEC);
        if (bit_is_set(PINF, SDA_PIN)) b |= 1;
        PORTF &= ~(1 << SCL_PIN);
        _delay_us(I2C_DELAY_USEC);
    }

    // send response
    DDRF |= (1 << SDA_PIN);
    if (last) PORTF |= (1 << SDA_PIN);
    else PORTF &= ~(1 << SDA_PIN);

    PORTF |= (1 << SCL_PIN);
    _delay_us(I2C_DELAY_USEC);
    PORTF &= ~(1 << SCL_PIN);
    PORTF &= ~(1 << SDA_PIN);
    return b;
}

