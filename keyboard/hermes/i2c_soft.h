#include <stdint.h>

#ifndef I2C_H
#define I2C_H

#define SDA_PIN 5
#define SCL_PIN 4

#define I2C_DELAY_USEC 4

// Initialize I2C.
void i2c_init(void);

// Uninitialize I2C.
void i2c_uninit(void);

// Start transmission.
void i2c_start(void);

// Restart transmission.
void i2c_restart(void);

// Stop this transmission.
void i2c_stop(void);

// Send one byte.
uint8_t i2c_send(uint8_t data);

// Send multiple bytes.
uint8_t i2c_send_data(const uint8_t* data, uint8_t len);

// Read and return one byte. last specifies whether this is the last byte to be read.
uint8_t i2c_read(uint8_t last);

#endif
