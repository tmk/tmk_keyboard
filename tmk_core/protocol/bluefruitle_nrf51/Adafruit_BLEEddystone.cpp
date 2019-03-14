/**************************************************************************/
/*!
    @file     Adafruit_BLEEddystone.cpp
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

#include "Adafruit_BLEEddystone.h"

#define EDDYSTONE_MINIMUM_FIRMWARE_VERSION    "0.7.0"


/******************************************************************************/
/*!
    @brief Constructor
*/
/******************************************************************************/
Adafruit_BLEEddystone::Adafruit_BLEEddystone(Adafruit_BLE& ble) :
  _ble(ble)
{
}

/******************************************************************************/
/*!
    @brief Enable Eddystone service if not already enabled
    @param reset true will reset Bluefruit
*/
/******************************************************************************/
bool Adafruit_BLEEddystone::begin(bool reset)
{
  VERIFY_( _ble.isVersionAtLeast(EDDYSTONE_MINIMUM_FIRMWARE_VERSION) );

  int32_t enabled = 0;
  VERIFY_( _ble.atcommandIntReply( F("AT+EDDYSTONESERVICEEN"), &enabled) );

  if ( enabled ) return true;
  VERIFY_( _ble.atcommand( F("AT+EDDYSTONESERVICEEN=1") ) );

  // Perform Bluefruit reset if needed
  if (reset) _ble.reset();

  return true;
}

/******************************************************************************/
/*!
    @brief Stop Eddystone service if it is enabled
    @param reset true will reset Bluefruit
*/
/******************************************************************************/
bool Adafruit_BLEEddystone::stop(bool reset)
{
  int32_t enabled = 0;
  VERIFY_( _ble.atcommandIntReply( F("AT+EDDYSTONESERVICEEN"), &enabled) );
  if ( !enabled ) return true;

  VERIFY_( _ble.atcommand( F("AT+EDDYSTONESERVICEEN=0") ) );

  // Perform Bluefruit reset if needed
  if (reset) _ble.reset();

  return true;
}

/******************************************************************************/
/*!
    @brief Change Bluefruit's URL setting in NVM
    @param url  URL to be advertized
    @param broadcastEvenConnect Keep broadcasting even Bluefruit is connected
    @param rssi_at_0m RSSI value at 0m (check out EddyStone specs)
*/
/******************************************************************************/
bool Adafruit_BLEEddystone::setURL(const char* url, bool broadcastEvenConnect, int8_t rssi_at_0m)
{
  bool result;
  uint8_t current_mode = _ble.getMode();

  // switch mode if necessary to execute command
  if ( current_mode == BLUEFRUIT_MODE_DATA ) _ble.setMode(BLUEFRUIT_MODE_COMMAND);

  // send command and integer parameters separated by comma
  _ble.print(  F("AT+EDDYSTONEURL=") );
  _ble.print(url);

  _ble.print(','); _ble.print(broadcastEvenConnect, DEC);
  _ble.print(','); _ble.print(rssi_at_0m);

  _ble.println(); // execute command

  result = _ble.waitForOK();

  // switch back if necessary
  if ( current_mode == BLUEFRUIT_MODE_DATA ) _ble.setMode(BLUEFRUIT_MODE_DATA);

  return result;
}

/******************************************************************************/
/*!
    @brief Start Broadcasting (advertising) specified URL
*/
/******************************************************************************/
bool Adafruit_BLEEddystone::startBroadcast(void)
{
  return _ble.atcommand( F("AT+EDDYSTONEBROADCAST=1") );
}

/******************************************************************************/
/*!
    @brief Stop Broadcasting (advertising) specified URL
*/
/******************************************************************************/
bool Adafruit_BLEEddystone::stopBroadcast(void)
{
  return _ble.atcommand( F("AT+EDDYSTONEBROADCAST=0") );
}

/******************************************************************************/
/*!
    @brief Broadcast (advertising) specified URL
*/
/******************************************************************************/
bool Adafruit_BLEEddystone::startConfigMode(uint32_t seconds)
{
  return _ble.atcommand( F("AT+EDDYSTONECONFIGEN"), (int32_t) seconds );
}
