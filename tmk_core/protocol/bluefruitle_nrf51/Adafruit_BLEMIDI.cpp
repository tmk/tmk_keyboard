/**************************************************************************/
/*!
    @file     Adafruit_BLEMIDI.cpp
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

#include "Adafruit_BLEMIDI.h"

#define MIDI_MINIMUM_FIRMWARE_VERSION    "0.7.0"

/******************************************************************************/
/*!
    @brief Constructor
*/
/******************************************************************************/
Adafruit_BLEMIDI::Adafruit_BLEMIDI(Adafruit_BLE& ble) :
  _ble(ble)
{
}

/******************************************************************************/
/*!
    @brief Set callback
*/
/******************************************************************************/
void Adafruit_BLEMIDI::setRxCallback(midiRxCallback_t fp)
{
  _ble.setBleMidiRxCallback(fp);
}


/******************************************************************************/
/*!
    @brief Enable MIDI service if not already enabled
    @param reset true will reset Bluefruit
*/
/******************************************************************************/
bool Adafruit_BLEMIDI::begin(bool reset)
{
  VERIFY_( _ble.isVersionAtLeast(MIDI_MINIMUM_FIRMWARE_VERSION) );

  int32_t enabled = 0;
  VERIFY_( _ble.atcommandIntReply( F("AT+BLEMIDIEN"), &enabled) );

  if ( enabled ) return true;
  VERIFY_( _ble.atcommand( F("AT+BLEMIDIEN=1") ) );

  // Perform Bluefruit reset if needed
  if (reset) _ble.reset();

  return true;
}

/******************************************************************************/
/*!
    @brief Stop MIDI service if it is enabled
    @param reset true will reset Bluefruit
*/
/******************************************************************************/
bool Adafruit_BLEMIDI::stop(bool reset)
{
  int32_t enabled = 0;
  VERIFY_( _ble.atcommandIntReply( F("AT+BLEMIDIEN"), &enabled) );
  if ( !enabled ) return true;

  VERIFY_( _ble.atcommand( F("AT+BLEMIDIEN=0") ) );

  // Perform Bluefruit reset if needed
  if (reset) _ble.reset();

  return true;
}

/******************************************************************************/
/*!
    @brief Send a MIDI event data
    @param bytes MIDI event data
*/
/******************************************************************************/
bool Adafruit_BLEMIDI::send(const uint8_t bytes[3])
{
  return _ble.atcommand( F("AT+BLEMIDITX"), bytes, 3);
}

/******************************************************************************/
/*!
    @brief Send multiple MIDI event which shared the same status
    @param status MIDI status
    @param bytes MIDI events data
    @param count number of data in bytes (must be multiple of 2)

    @note count + 1 must less than (20-3) --> count <= 16
*/
/******************************************************************************/
bool Adafruit_BLEMIDI::send_n(uint8_t status, const uint8_t bytes[], uint8_t count)
{
  VERIFY_(count <= 16);

  uint8_t data[17] = { status };
  memcpy(data+1, bytes, count);

  return _ble.atcommand( F("AT+BLEMIDITX"), data, count+1);
}

/******************************************************************************/
/*!
    @brief
    @param
*/
/******************************************************************************/
void Adafruit_BLEMIDI::processRxCallback(uint8_t data[], uint16_t len, Adafruit_BLE::midiRxCallback_t callback_func)
{
  if ( len < 3 ) return;

  // First 3 bytes is always : Header + Timestamp + Status
  midi_header_t header;
  uint16_t tstamp = 0;
  uint8_t status = 0;

  header.byte = *data++;
  len--;

  while (len)
  {
    /* event : 0x00 - 0x7F
       status: 0x80 - 0xEF
       sysex : 0xF0 - 0xFF
     */

    if ( bitRead(data[0], 7) )
    {
      // Start of new full event
      midi_timestamp_t timestamp;
      timestamp.byte = *data++;

      tstamp = (header.timestamp_hi << 7) | timestamp.timestamp_low;
      status = *data++;

      // Status must have 7th-bit set, must have something wrong
      if ( !bitRead(status, 7) ) return;

      callback_func( tstamp, status, data[0], data[1]);

      len  -= 4;
      data += 2;
    }
    else
    {
      // Running event
      callback_func( tstamp, status, data[0], data[1]);

      len  -= 2;
      data += 2;
    }
  }
}
