#include <util/twi.h>
#include <avr/io.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include <util/twi.h>
#include <stdbool.h>
#include "i2c.h"

#define I2C_READ 1
#define I2C_WRITE 0

#define I2C_ACK 1
#define I2C_NACK 0

// Limits the amount of we wait for any one i2c transaction.
// Since were running SCL line 100kHz (=> 10μs/bit), and each transactions is
// 9 bits, a single transaction will take around 90μs to complete.
//
// (F_CPU/SCL_CLOCK)  =>  # of mcu cycles to transfer a bit
// poll loop takes at least 8 clock cycles to execute
#define I2C_LOOP_TIMEOUT (9+1)*(F_CPU/SCL_CLOCK)/8

#define BUFFER_POS_INC() (slave_buffer_pos = (slave_buffer_pos+1)%SLAVE_BUFFER_SIZE)

static volatile uint8_t i2c_slave_buffer[SLAVE_BUFFER_SIZE] = {0};

static volatile uint8_t slave_buffer_pos;
static volatile bool slave_has_register_set = false;

static uint8_t i2c_start(uint8_t address);
static void i2c_stop(void);
static uint8_t i2c_write(uint8_t data);
static uint8_t i2c_read(uint8_t ack);

// Wait for an i2c operation to finish
inline static
void i2c_delay(void) {
  uint16_t lim = 0;
  while(!(TWCR & (1<<TWINT)) && lim < I2C_LOOP_TIMEOUT)
    lim++;

  // easier way, but will wait slightly longer
  // _delay_us(100);
}

// i2c_device_addr: the i2c device to communicate with
// addr: the memory address to read from the i2c device
// dest: pointer to where read data is saved
// len: the number of bytes to read
//
// NOTE: on error, the data in dest may have been modified
bool i2c_master_read(uint8_t i2c_device_addr, uint8_t addr, uint8_t *dest, uint8_t len) {
  bool err;
  if (len == 0) return 0;

  err = i2c_start(i2c_device_addr + I2C_WRITE);
  if (err) return err;
  err = i2c_write(addr);
  if (err) return err;

  err = i2c_start(i2c_device_addr + I2C_READ);
  if (err) return err;

  for (uint8_t i = 0; i < len-1; ++i) {
    dest[i] = i2c_read(I2C_ACK);
  }
  dest[len-1] = i2c_read(I2C_NACK);
  i2c_stop();

  return 0;
}

// i2c_device_addr: the i2c device to communicate with
// addr: the memory address at which to write in the i2c device
// data: the data to be written
// len: the number of bytes to write
bool i2c_master_write(uint8_t i2c_device_addr, uint8_t addr, uint8_t *data, uint8_t len) {
  bool err;

  if (len == 0) return 0;

  err = i2c_start(i2c_device_addr + I2C_WRITE);
  if (err) return err;
  err = i2c_write(addr);
  if (err) return err;

  for (uint8_t i = 0; i < len; ++i) {
    err = i2c_write(data[i]);
    if (err) return err;
  }
  i2c_stop();
  return 0;
}

void i2c_slave_write(uint8_t addr, uint8_t data) {
  i2c_slave_buffer[addr] = data;
}

uint8_t i2c_slave_read(uint8_t addr) {
  return i2c_slave_buffer[addr];
}

// Setup twi to run at 100kHz
void i2c_master_init(void) {
  // no prescaler
  TWSR = 0;
  // Set TWI clock frequency to SCL_CLOCK. Need TWBR>10.
  // Check datasheets for more info.
  TWBR = ((F_CPU/SCL_CLOCK)-16)/2;
}

// Start a transaction with the given i2c slave address. The direction of the
// transfer is set with I2C_READ and I2C_WRITE.
// returns: 0 => success
//          1 => error
uint8_t i2c_start(uint8_t address) {
  TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWSTA);

  i2c_delay();

  // check that we started successfully
  if ( (TW_STATUS != TW_START) && (TW_STATUS != TW_REP_START))
    return 1;

  TWDR = address;
  TWCR = (1<<TWINT) | (1<<TWEN);

  i2c_delay();

  if ( (TW_STATUS != TW_MT_SLA_ACK) && (TW_STATUS != TW_MR_SLA_ACK) )
    return 1; // slave did not acknowledge
  else
    return 0; // success
}


// Finish the i2c transaction.
void i2c_stop(void) {
  TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWSTO);

  uint16_t lim = 0;
  while(!(TWCR & (1<<TWSTO)) && lim < I2C_LOOP_TIMEOUT)
    lim++;
}

// Write one byte to the i2c slave.
// returns 0 => slave ACK
//         1 => slave NACK
uint8_t i2c_write(uint8_t data) {
  TWDR = data;
  TWCR = (1<<TWINT) | (1<<TWEN);

  i2c_delay();

  // check if the slave acknowledged us
  return (TW_STATUS == TW_MT_DATA_ACK) ? 0 : 1;
}

// Read one byte from the i2c slave. If ack=1 the slave is acknowledged,
// if ack=0 the acknowledge bit is not set.
// returns: byte read from i2c device
uint8_t i2c_read(uint8_t ack) {
  TWCR = (1<<TWINT) | (1<<TWEN) | (ack<<TWEA);

  i2c_delay();
  return TWDR;
}

void i2c_slave_init(uint8_t address) {
  TWAR = address << 0; // slave i2c address
  // TWEN  - twi enable
  // TWEA  - enable address acknowledgement
  // TWINT - twi interrupt flag
  // TWIE  - enable the twi interrupt
  TWCR = (1<<TWIE) | (1<<TWEA) | (1<<TWINT) | (1<<TWEN);
}

ISR(TWI_vect);

ISR(TWI_vect) {
  uint8_t ack = 1;
  switch(TW_STATUS) {
    case TW_SR_SLA_ACK:
      // this device has been addressed as a slave receiver
      slave_has_register_set = false;
      break;

    case TW_SR_DATA_ACK:
      // this device has received data as a slave receiver
      // The first byte that we receive in this transaction sets the location
      // of the read/write location of the slaves memory that it exposes over
      // i2c.  After that, bytes will be written at slave_buffer_pos, incrementing
      // slave_buffer_pos after each write.
      if(!slave_has_register_set) {
        slave_buffer_pos = TWDR;
        // don't acknowledge the master if this memory loctaion is out of bounds
        if ( slave_buffer_pos >= SLAVE_BUFFER_SIZE ) {
          ack = 0;
          slave_buffer_pos = 0;
        }
        slave_has_register_set = true;
      } else {
        i2c_slave_buffer[slave_buffer_pos] = TWDR;
        BUFFER_POS_INC();
      }
      break;

    case TW_ST_SLA_ACK:
    case TW_ST_DATA_ACK:
      // master has addressed this device as a slave transmitter and is
      // requesting data.
      TWDR = i2c_slave_buffer[slave_buffer_pos];
      BUFFER_POS_INC();
      break;

    case TW_BUS_ERROR: // something went wrong, reset twi state
      TWCR = 0;
    default:
      break;
  }
  // Reset everything, so we are ready for the next TWI interrupt
  TWCR |= (1<<TWIE) | (1<<TWINT) | (ack<<TWEA) | (1<<TWEN);
}
