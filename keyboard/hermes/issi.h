#ifndef ISSI_H
#define ISSI_H

#define DEVICE_ADDRESS_WRITE 0xE8
#define DEVICE_ADDRESS_READ 0xE9

#define ADDRESS_COMMAND_REGISTER 0xFD
#define ADDRESS_PAGE_ONE 0x00
#define ADDRESS_PAGE_NINE 0x0b

#define ADDRESS_LED_CONTROL_REGISTER 0x00
#define ADDRESS_LED_BLINK_REGISTER 0x12
#define ADDRESS_LED_PWM_REGISTER 0x24

void issi_init(void);

void issi_send_data(const uint8_t* data, uint8_t len, uint8_t page);

uint8_t issi_read_byte(uint8_t page, uint8_t address);

void issi_zero_pages(uint8_t startPage, uint8_t numPages, uint8_t startRegister, uint8_t endRegister);

#endif