#ifndef I2C_H
#define I2C_H

#include <stdint.h>

#define SLAVE_BUFFER_SIZE 0x40

// i2c SCL clock frequency
#define SCL_CLOCK  100000L


void i2c_master_init(void);
void i2c_slave_init(uint8_t address);

bool i2c_master_write(uint8_t i2c_device_addr, uint8_t addr, uint8_t *dest, uint8_t len);
bool i2c_master_read(uint8_t i2c_device_addr, uint8_t addr, uint8_t *data, uint8_t len);

void i2c_slave_write(uint8_t addr, uint8_t data);
uint8_t i2c_slave_read(uint8_t addr);

#endif
