#include <avr/io.h>
#include <avr/interrupt.h>

#include "i2c_soft.h"
#include "issi.h"
#include "print.h"

const uint8_t initLedControl[] = {
    DEVICE_ADDRESS_WRITE,
    ADDRESS_LED_CONTROL_REGISTER,
    // LED on/off
    0x7F, 0x00, // C1-1 : C1-16
    0x7F, 0x00, // C2-1 : C2-16
    0x7F, 0x00, // C3-1 : C3-16
    0x00, 0x00, // C4-1 : C4-16
    0x00, 0x00, // C5-1 : C5-16
    0x00, 0x00, // C6-1 : C6-16
    0x00, 0x00, // C7-1 : C7-16
    0x00, 0x00, // C8-1 : C8-16
    0x00, 0x00  // C9-1 : C9-16
};

const uint8_t initLedBlink[] = {
    DEVICE_ADDRESS_WRITE,
    ADDRESS_LED_BLINK_REGISTER,
    // LED blink
    0x00, 0x00,
    0x00, 0x00,
    0x00, 0x00,
    0x00, 0x00,
    0x00, 0x00,
    0x00, 0x00,
    0x00, 0x00,
    0x00, 0x00,
    0x00, 0x00
};

const uint8_t clearShutdownBit[] = { DEVICE_ADDRESS_WRITE, 0x0A, 0x01 };
const uint8_t setShutdownBit[] = { DEVICE_ADDRESS_WRITE, 0x0A, 0x00 };

void issi_init(void) {
    i2c_init();

    // Start up chip. (pull SDB, INTB high)
    DDRF  |= 0b00000011;
    PORTF |= 0b00000011;

    // Zero out pages
    issi_zero_pages(0x0B, 1, 0x00, 0x0C);
    issi_zero_pages(0x00, 8, 0x00, 0xB4);

    // Send initial LED config.
    issi_send_data(initLedControl, sizeof(initLedControl), 0);
    issi_send_data(initLedBlink, sizeof(initLedBlink), 0);

    // Clear software shutdown bit.
    issi_send_data(clearShutdownBit, 3, 0x0B);
}

void issi_send_data(const uint8_t* data, uint8_t len, uint8_t page) {
    cli();
    uint8_t pageSelect[] = { DEVICE_ADDRESS_WRITE, ADDRESS_COMMAND_REGISTER, page };

    i2c_send_data(pageSelect, 3);
    i2c_send_data(data, len);
    sei();
}

uint8_t issi_read_byte(uint8_t page, uint8_t address) {
    cli();
    uint8_t pageSelect[] = { DEVICE_ADDRESS_WRITE, ADDRESS_COMMAND_REGISTER, page };
    uint8_t registerSelect[] = { DEVICE_ADDRESS_WRITE, address };
    uint8_t readAddress[] = { DEVICE_ADDRESS_READ };

    i2c_send_data(pageSelect, 3);
    i2c_send_data(registerSelect, 2);

    i2c_start();
    i2c_send_data(readAddress, 1);
    uint8_t ret = i2c_read(1);
    i2c_stop();
    sei();
    return ret;
}

void issi_zero_pages(uint8_t startPage, uint8_t numPages, uint8_t startRegister, uint8_t endRegister) {
    uint8_t pageSelect[] = { DEVICE_ADDRESS_WRITE, ADDRESS_COMMAND_REGISTER, 0 };

    uint8_t fullPage[ 0xB4 + 2 ] = { 0 };
    fullPage[0] = DEVICE_ADDRESS_WRITE;
    fullPage[1] = startRegister;

    for(uint8_t page = startPage; page < startPage + numPages; page++) {
        pageSelect[2] = page;
        i2c_send_data(pageSelect, sizeof(pageSelect));
        i2c_send_data(fullPage, endRegister - startRegister + 2);
    }
}