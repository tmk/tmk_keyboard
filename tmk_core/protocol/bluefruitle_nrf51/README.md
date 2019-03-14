This library is for all nRF51 based Adafruit Bluefruit LE modules that use SPI or UART.

Current nRF51 based Bluefruit LE products include:

* [Bluefruit LE Friend](https://www.adafruit.com/product/2267)
* [Bluefruit LE UART Friend](https://www.adafruit.com/product/2479)
* [Bluefruit LE SPI Friend](https://www.adafruit.com/product/2633)
* [Bluefruit LE Shield](https://www.adafruit.com/products/2746)
* [Bluefruit LE Micro](https://www.adafruit.com/product/2661)
* [Feather 32u4 Bluefruit LE](https://www.adafruit.com/product/2829)
* [Feather M0 Bluefruit LE](https://www.adafruit.com/products/2995)

# AT Commands

The Bluefruit LE modules this library talks to use AT-style commands and responses.

If you are using a UART board, the commands are sent directly as text using a SW serial transport.

If your are using an SPI board, the AT commands are wrapped in a thin **[SDEP](SDEP.md)** (Simple Data Exchange Protocol) wrapper to transmit and received text data over the binary SPI transport.  Details of this SPI transport layer are detailed in [SDEP.md](SDEP.md) in this same folder.

# Hardware Setup

There are two variants of the nRF51 Bluefruit LE modules.  One uses SPI to communicate, the other uses UART with flow control (TXD, RXD, CTS, RTS).  The wiring you use will depend on the module you are trying to connect.

On both boards, power should be connected as shown below:

Bluefruit LE | Arduino Uno
-------------|------------
VIN          | 5V (assuming a 5V board)
GND          | GND

## Software UART Pinout

If you are using a UART Bluefruit LE board, your Arduino should be connected to the Bluefruit LE UART module using the following pinout:

Bluefruit LE UART | Arduino Uno
------------------|------------
RTS               | 8
RXI               | 9
TXO               | 10
CTS               | 11

Optional Pins

Bluefruit LE UART | Arduino Uno
------------------|------------
MODE              | 12

## SPI Pinout

If you are using an SPI Bluefruit LE board, your Arduino should be connected to the Bluefruit LE SPI module using the following pinout:

Bluefruit LE SPI | Arduino Uno
-----------------|------------
SCLK             | 13
MISO             | 12
MOSI             | 11
CS               | 8
IRQ              | 7

Optional Pins (enable these in the sample sketches)

Bluefruit LE SPI | Arduino Uno
-----------------|------------
RESET            | 6
