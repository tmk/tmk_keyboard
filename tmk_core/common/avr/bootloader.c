#include <stdint.h>
#include <stdbool.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <avr/boot.h>
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

/* bootloader start address in byte */
#define BOOTLOADER_START      (FLASHEND - bootloader_size() + 1)

/* boot section size in byte */
static inline uint16_t bootloader_size(void)
{
#if defined(BOOTLOADER_SIZE)
    return BOOTLOADER_SIZE;
#else
    #if defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega32U2__) || \
            defined(__AVR_ATmega16U4__) || defined(__AVR_ATmega16U2__) || \
            defined(__AVR_ATmega328__) || defined(__AVR_ATmega328P__)
        uint8_t hfuse = boot_lock_fuse_bits_get(GET_HIGH_FUSE_BITS);
        switch ((hfuse >> 1) & 3) {
            case 0: return 4096;
            case 1: return 2048;
            case 2: return 1024;
            case 3: return 512;
        }
        return 4096;
    #elif defined(__AVR_AT90USB646__) || defined(__AVR_AT90USB647__) || \
            defined(__AVR_AT90USB1286__) || defined(__AVR_AT90USB1287__)
        uint8_t hfuse = boot_lock_fuse_bits_get(GET_HIGH_FUSE_BITS);
        switch ((hfuse >> 1) & 3) {
            case 0: return 8192;
            case 1: return 4096;
            case 2: return 2048;
            case 3: return 1024;
        }
        return 8192;
    #else
        #error Set Boot section size to BOOTLOADER_SIZE in config.h
    #endif
#endif
}


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

        // some of bootloaders may need to preseve?
        MCUSR = 0;

        // disable watchdog timer
        wdt_disable();

#ifndef NO_BOOTLOADER_CATERINA_BOOTKEY
        // Clear bootkey of Caterina bootloader for other bootloaders
        // Leonardo and Pro Micro with Arduino default fuse setting don't reach here
        // because bootloader section are executed before application everytime.
        *(volatile uint16_t *)0x0800 = 0;
#endif

        // This is compled into 'icall', address should be in word unit, not byte.
        ((void (*)(void))( (uint16_t)(BOOTLOADER_START / 2) ))();
    }
}
