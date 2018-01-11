/**************************************************************************/
/*!
    @file     Adafruit_BLEMIDI.h
    @author   hathach

    @section LICENSE

    Software License Agreement (BSD License)

    Copyright (c) 2016, Adafruit Industries (adafruit.com)
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

#ifndef _ADAFRUIT_BLEMIDI_H_
#define _ADAFRUIT_BLEMIDI_H_

#include <Arduino.h>
#include "Adafruit_BLE.h"

typedef struct ATTR_PACKED
{
  union {
    struct {
      uint8_t timestamp_hi : 6;
      uint8_t reserved     : 1;
      uint8_t start_bit    : 1;
    };

    uint8_t byte;
  };
} midi_header_t;

ASSERT_STATIC_ ( sizeof(midi_header_t) == 1 );

typedef struct ATTR_PACKED
{
  union {
    struct {
      uint8_t timestamp_low : 7;
      uint8_t start_bit : 1;
    };

    uint8_t byte;
  };
} midi_timestamp_t;

ASSERT_STATIC_ ( sizeof(midi_timestamp_t) == 1 );

class Adafruit_BLEMIDI
{
private:
  Adafruit_BLE& _ble;

public:
  typedef Adafruit_BLE::midiRxCallback_t midiRxCallback_t;
  Adafruit_BLEMIDI(Adafruit_BLE& ble);

  bool begin(bool reset = true);
  bool stop (bool reset = true);

  bool send(const uint8_t bytes[3]);

  bool send(uint8_t status, const uint8_t bytes[2])
  {
    uint8_t buffer[3] = { status, bytes[0], bytes[1] };
    return send(buffer);
  }

  bool send(uint8_t status, uint8_t byte1, uint8_t byte2)
  {
    uint8_t buffer[3] = { status, byte1, byte2 };
    return send(buffer);
  }

  bool send_n(uint8_t status, const uint8_t bytes[], uint8_t count);

  void setRxCallback(midiRxCallback_t fp);

  static void processRxCallback(uint8_t data[], uint16_t len, Adafruit_BLE::midiRxCallback_t callback_func);
};

#endif /* _ADAFRUIT_BLEMIDI_H_ */
