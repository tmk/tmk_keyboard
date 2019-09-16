#include <stdint.h>
#include <stdbool.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <util/delay.h>
#include "bootloader.h"

#ifdef PROTOCOL_LUFA
#include <LUFA/Drivers/USB/USB.h>
#endif


/* Bootloader Size in *bytes*
 *
 * AVR Boot section size are defined by setting BOOTSZ fuse in fact. Consult with your MCU datasheet.
 * Note that 'Word'(2 bytes) size and address are used in datasheet while TMK uses 'Byte'.
 *
 * Size of Bootloaders in bytes:
 *   Atmel DFU loader(ATmega32U2)   4096
 *   Atmel DFU loader(ATmega32U4)   4096
 *   Atmel DFU loader(AT90USB128)   8192
 *   LUFA bootloader(ATmega32U4)    4096
 *   Arduino Caterina(ATmega32U4)   4096
 *   USBaspLoader(ATmega***)        2048
 *   Teensy   halfKay(ATmega32U4)   512
 *   Teensy++ halfKay(AT90USB128)   1024
 *
 *
 * AVR Boot section is located at the end of Flash memory.
 *
 * byte     ATMega32u4 with 4096-byte Boot section
 * 0x0000   +---------------+
 *          |               |
 *          |               |
 *          |  Application  | 28KB
 *          |               |
 *          =               =
 *          |               |
 * 0x7000   +---------------+ <---- BOOTLOADER_START
 *          |  Bootloader   | 4KB   BOOTLOADER_SIZE
 * 0x7FFF   +---------------+ <---- FLASHEND
 */
#ifndef BOOTLOADER_SIZE
#warning To use bootloader_jump() you need to define BOOTLOADER_SIZE in config.h.
#define BOOTLOADER_SIZE     4096
#endif

#define FLASH_SIZE          (FLASHEND + 1L)
#define BOOTLOADER_START    (FLASH_SIZE - BOOTLOADER_SIZE)


/*
 * Entering the Bootloader via Software
 * http://www.fourwalledcubicle.com/files/LUFA/Doc/120730/html/_page__software_bootloader_start.html
 */
#define BOOTLOADER_RESET_KEY 0xB007B007
uint32_t reset_key  __attribute__ ((section (".noinit")));

/* initialize MCU status by watchdog reset */
void bootloader_jump(void) {
#ifdef PROTOCOL_LUFA
    USB_Disable();
    cli();
    _delay_ms(2000);
#endif

#ifdef PROTOCOL_PJRC
    cli();
    UDCON = 1;
    USBCON = (1<<FRZCLK);
    UCSR1B = 0;
    _delay_ms(5);
#endif

#ifndef NO_BOOTLOADER_CATERINA_BOOTKEY
    // Set bootkey for Arduino Leonardo and Pro Micro bootloader
    // Watchdog reset with bootkey causes the bootloader to enter program mode instead of starting application.
    // https://github.com/arduino/ArduinoCore-avr/blob/master/bootloaders/caterina/Caterina.c#L68-L69
    // https://github.com/sparkfun/SF32u4_boards/blob/master/sparkfun/avr/bootloaders/caterina/Caterina.c#L88-L89
    *(volatile uint16_t *)0x0800 = 0x7777;
#endif

    // watchdog reset
    reset_key = BOOTLOADER_RESET_KEY;
    wdt_enable(WDTO_250MS);
    for (;;);
}


/* this runs before main() */
void bootloader_jump_after_watchdog_reset(void) __attribute__ ((used, naked, section (".init3")));
void bootloader_jump_after_watchdog_reset(void)
{
    if ((MCUSR & (1<<WDRF)) && reset_key == BOOTLOADER_RESET_KEY) {
        reset_key = 0;

        // My custom USBasploader requires this to come up.
        MCUSR = 0;

        // Seems like Teensy halfkay loader requires clearing WDRF and disabling watchdog.
        MCUSR &= ~(1<<WDRF);
        wdt_disable();

#ifndef NO_BOOTLOADER_CATERINA_BOOTKEY
        // Clear bootkey of Caterina bootloader for other bootloaders
        // Leonardo and Pro Micro with Arduino default fuse setting don't reach here
        // because bootloader section are executed before application everytime.
        *(volatile uint16_t *)0x0800 = 0;
#endif

        // This is compled into 'icall', address should be in word unit, not byte.
        ((void (*)(void))(BOOTLOADER_START/2))();
    }
}
