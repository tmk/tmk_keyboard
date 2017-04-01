# I2C Master library

## Table of contents

1. What is this library?
2. Contents of this library
3. Supported devices
4. Functions in this library

------------------------------------------------------------------------------------

### 1. What is this library?
This library is an I2C master library which uses the TWI peripheral inside the
AVR microcontroller to establish connections using the I2C protocol

------------------------------------------------------------------------------------

### 2. Contents of this library

The library contains the following files:

* main.c
* I2C_master.c
* I2C_master.h

##### main.c
This is a piece of example code which uses this library to read out
an HMC5338L I2C magnetometer.

##### I2C_master.c
This file contains all the function declarations to setup and work with the
TWI hardware peripheral inside the AVR.

*Make sure you add this source file to your Makefile!*

##### I2C_master.h
This file contains the function prototypes and the definition of the
Read / Write bit (0 = read, 1 = write)

*This file has to be included in your source file*

------------------------------------------------------------------------------------

### 3. Supported devices
Though I have only tested this library on an ATmega328P it should be running
on all major ATmega AVRs like:

* ATmega644
* ATmega32
* ATmega16
* ATmega328/P
* ATmega168/P
* ATmega88/P
* ATmega44/P
* ATmega8

If your device is not supported you can probably adapt this library easily to your
needs by having a look at the your device's data sheet and changing the register names
appropriately

The ATtiny range of microcontrolles is not supported as they only provide a USI
peripheral which is completely different from the TWI peripheral used on the
other controllers

------------------------------------------------------------------------------------

### 4. Functions in this library

* void i2c_init(void)
* uint8_t i2c_start(uint8_t address)
* uint8_t i2c_write(uint8_t data)
* uint8_t i2c_read_ack(void)
* uint8_t i2c_read_nack(void)
* uint8_t i2c_transmit(uint8_t address, uint8_t* data, uint16_t length)
* uint8_t i2c_receive(uint8_t address, uint8_t* data, uint16_t length)
* void i2c_stop(void)

##### void I2C_init(void)
This function needs to be called only once to set up the correct SCL frequency
for the bus

##### uint8_t I2C_start(uint8_t address)
This function needs to be called any time a connection to a new slave device should
be established. The function returns 1 if an error has occurred, otherwise it returns
0.

The syntax to start a operation write to a device is either:
`I2C_start(SLAVE_ADDRESS+I2C_WRITE);`
or
`I2C_start(SLAVE_WRITE_ADDRESS);`

The syntax to start a read operation from a device is either:
`I2C_start(SLAVE_ADDRESS+I2C_READ);`
or
`I2C_start(SLAVE_READ_ADDRESS);`

##### uint8_t I2C_write(uint8_t data)
This function is used to write data to the currently active device.
The only parameter this function takes is the 8 bit unsigned integer to be sent.
The function returns 1 if an error has occurred, otherwise it returns
0.

##### uint8_t I2C_read_ack(void)
This function is used to read one byte from a device and request another byte of data
after the transmission is complete by sending the acknowledge bit.
This function returns the received byte.

##### uint8_t I2C_read_nack(void)
This function is used to read one byte from a device an then not requesting another
byte and therefore stopping the current transmission.
This function returns the received byte.

##### uint8_t i2c_transmit(uint8_t address, uint8_t* data, uint16_t length);
This function is used to transmit [length] number of bytes to an I2C device with the given
I2C address from [data].
The [address] passed to this function is the 7-bit slave address, left
shifted by one bit (i.e. 7-bit slave address is `0x2F` -> `(0x2F)<<1` = `0x5E`)

##### uint8_t i2c_receive(uint8_t address, uint8_t* data, uint16_t length);
This function is used to read [length] number of bytes from the I2C device with the given
I2C address into the [data].
The [address] passed to this function is the 7-bit slave address, left
shifted by one bit (i.e. 7-bit slave address is `0x2F` -> `(0x2F)<<1` = `0x5E`)

##### void I2C_stop(void)
This function disables the TWI peripheral completely
and therefore disconnects the device from the bus.
