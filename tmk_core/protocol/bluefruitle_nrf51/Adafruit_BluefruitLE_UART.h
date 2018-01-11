/**************************************************************************/
/*!
    @file     Adafruit_BluefruitLE_UART.h
    @author   hathach

    @section LICENSE

    Software License Agreement (BSD License)

    Copyright (c) 2015, Adafruit Industries (adafruit.com)
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:
    1. Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.
    3. Neither the name of the copyright holders nor the
    names of its contributors may be used to endorse or promote products
    derived from this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ''AS IS'' AND ANY
    EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
    WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER BE LIABLE FOR ANY
    DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
    (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
    LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
    ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
    SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
/**************************************************************************/

#ifndef _ADAFRUIT_BLE_UART_H_
#define _ADAFRUIT_BLE_UART_H_

#include "Arduino.h"
#include <Adafruit_BLE.h>

#define SOFTWARE_SERIAL_AVAILABLE   ( ! (defined (_VARIANT_ARDUINO_DUE_X_) || defined (_VARIANT_ARDUINO_ZERO_) || defined (ARDUINO_STM32_FEATHER)) )

#if SOFTWARE_SERIAL_AVAILABLE
  #include <SoftwareSerial.h>
#endif

class Adafruit_BluefruitLE_UART : public Adafruit_BLE
{
  private:
    // Hardware Pins
    int8_t  _mode_pin, _cts_pin, _rts_pin;
    Stream *mySerial;
#if SOFTWARE_SERIAL_AVAILABLE
    SoftwareSerial *ss;
#endif
    HardwareSerial *hs;
    boolean _debug;
    uint8_t _intercharwritedelay;

  public:
    // Software Serial Constructor (0, 1, 2, or 3 pins)
    Adafruit_BluefruitLE_UART(HardwareSerial &port,
		      int8_t mode_pin = -1, 
		      int8_t cts_pin = -1, 
		      int8_t rts_pin = -1);
#if SOFTWARE_SERIAL_AVAILABLE
    Adafruit_BluefruitLE_UART(SoftwareSerial &port,
		      int8_t mode_pin = -1, 
		      int8_t cts_pin = -1, 
		      int8_t rts_pin = -1);
#endif

    void setInterCharWriteDelay(uint8_t x) { _intercharwritedelay = x; };

    virtual ~Adafruit_BluefruitLE_UART();

    // HW initialisation
    bool begin(boolean debug = false, boolean blocking = true);
    void end(void);

    bool setMode(uint8_t new_mode);

    // Class Print virtual function Interface
    virtual size_t write(uint8_t c);

    // pull in write(str) and write(buf, size) from Print
    using Print::write;

    // Class Stream interface
    virtual int  available(void);
    virtual int  read(void);
    virtual void flush(void);
    virtual int  peek(void);
};

#endif
