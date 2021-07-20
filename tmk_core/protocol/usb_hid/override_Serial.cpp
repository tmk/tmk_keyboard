/*
 * Null implementation of Serial to dump debug print into blackhole
 */
#include "Arduino.h"
#include "sendchar.h"

#include "USBAPI.h"


void Serial_::begin(unsigned long /* baud_count */)
{
}

void Serial_::begin(unsigned long /* baud_count */, byte /* config */)
{
}

void Serial_::end(void)
{
}

int Serial_::available(void)
{
    return 0;
}

int Serial_::peek(void)
{
    return -1;
}

int Serial_::read(void)
{
    return -1;
}

int Serial_::availableForWrite(void)
{
    return 1;
}

void Serial_::flush(void)
{
}

size_t Serial_::write(uint8_t c)
{
    sendchar(c);
    return 1;
}

size_t Serial_::write(const uint8_t *buffer, size_t size)
{
    for (int i = 0; i < size; i++) {
        sendchar(buffer[i]);
    }
    return size;
}

Serial_::operator bool() {
    return true;
}

Serial_ Serial;
