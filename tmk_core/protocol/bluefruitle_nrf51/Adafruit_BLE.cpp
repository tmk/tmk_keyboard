/**************************************************************************/
/*!
    @file     Adafruit_BLE.c
    @author   hathach

    @section LICENSE

    Software License Agreement (BSD License)

    Copyright (c) 2014, Adafruit Industries (adafruit.com)
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
#include "Adafruit_BLE.h"
#include "Adafruit_BLEMIDI.h"

#ifndef min
  #define min(a,b) ((a) < (b) ? (a) : (b))
#endif

enum {
  EVENT_SYSTEM_CONNECT     = 0,
  EVENT_SYSTEM_DISCONNECT  = 1,

  EVENT_SYSTEM_BLE_UART_RX = 8,
  // 9 reserved

  EVENT_SYSTEM_BLE_MIDI_RX = 10,
  //  11 reserved
};

enum {
  NVM_USERDATA_SIZE = 256
};

/******************************************************************************/
/*!
    @brief  Constructor
*/
/******************************************************************************/
Adafruit_BLE::Adafruit_BLE(void)
{
  _timeout = BLE_DEFAULT_TIMEOUT;

  _reset_started_timestamp = 0;

  _disconnect_callback  = NULL;
  _connect_callback     = NULL;
  _ble_uart_rx_callback = NULL;
  _ble_midi_rx_callback = NULL;
  _ble_gatt_rx_callback = NULL;
}

/******************************************************************************/
/*!
    @brief Helper to install callback
    @param
*/
/******************************************************************************/
void Adafruit_BLE::install_callback(bool enable, int8_t system_id, int8_t gatts_id)
{
  uint8_t current_mode = _mode;

  // switch mode if necessary to execute command
  if ( current_mode == BLUEFRUIT_MODE_DATA ) setMode(BLUEFRUIT_MODE_COMMAND);

  print( enable ?  F("AT+EVENTENABLE=0x") : F("AT+EVENTDISABLE=0x") );
  print( (system_id < 0) ? 0 : bit(system_id), HEX );

  if ( gatts_id >= 0 )
  {
    print( F(",0x") );
    println( bit(gatts_id), HEX );
  }

  println();

  waitForOK();

  // switch back if necessary
  if ( current_mode == BLUEFRUIT_MODE_DATA ) setMode(BLUEFRUIT_MODE_DATA);
}

/******************************************************************************/
/*!
    @brief  Performs a system reset using AT command
    @param blocking blocking until bluefruit is ready, will take 1 second mostly
*/
/******************************************************************************/
bool Adafruit_BLE::reset(boolean blocking)
{
  bool isOK;
  // println();
  for (uint8_t t=0; t < 5; t++) {
    isOK = atcommand(F("ATZ"));

    if (isOK) break;
  }

  if (! isOK) {
    // ok we're going to get desperate
    delay(50);
    setMode(BLUEFRUIT_MODE_COMMAND);
    delay(50);
    
    for (uint8_t t=0; t < 5; t++) {
      isOK = atcommand(F("ATZ"));
      
      if (isOK) break;
    }

    if (!isOK) return false;
  }

  _reset_started_timestamp = millis();

  // Bluefruit need 1 second to reboot
  if (blocking)
  {
    delay(1000);
  }

  // flush all left over
  flush();

  return isOK;
}

/******************************************************************************/
/*!
    @brief  Performs a factory reset
*/
/******************************************************************************/
bool Adafruit_BLE::factoryReset(boolean blocking)
{
  println( F("AT+FACTORYRESET") );
  bool isOK = waitForOK();

  _reset_started_timestamp = millis();

  // Bluefruit need 1 second to reboot
  if (blocking)
  {
    delay(1000);
  }

  // flush all left over
  flush();

  return isOK;
}

/******************************************************************************/
/*!
    @brief  Check if the reset process is completed, should be used if user
    reset Bluefruit with non-blocking aka reset(false)
*/
/******************************************************************************/
bool Adafruit_BLE::resetCompleted(void)
{
  return millis() > (_reset_started_timestamp + 1000);
}

/******************************************************************************/
/*!
    @brief  Enable or disable AT Command echo from Bluefruit

    @parma[in] enable
               true to enable (default), false to disable
*/
/******************************************************************************/
bool Adafruit_BLE::echo(bool enable)
{
  return atcommand(F("ATE"), (int32_t) enable);
}

/******************************************************************************/
/*!
    @brief  Check connection state, returns true is connected!
*/
/******************************************************************************/
bool Adafruit_BLE::isConnected(void)
{
  int32_t connected = 0;
  atcommandIntReply(F("AT+GAPGETCONN"), &connected);
  return connected;
}

/******************************************************************************/
/*!
    @brief  Disconnect if currently connected
*/
/******************************************************************************/
void Adafruit_BLE::disconnect(void)
{
  atcommand( F("AT+GAPDISCONNECT") );
}

/******************************************************************************/
/*!
    @brief  Print Bluefruit's information retrieved by ATI command
*/
/******************************************************************************/
void Adafruit_BLE::info(void)
{
  uint8_t current_mode = _mode;

  bool v = _verbose;
  _verbose = false;

  SerialDebug.println(F("----------------"));

  // switch mode if necessary to execute command
  if ( current_mode == BLUEFRUIT_MODE_DATA ) setMode(BLUEFRUIT_MODE_COMMAND);

  println(F("ATI"));

  while ( readline() ) {
    if ( !strcmp(buffer, "OK") || !strcmp(buffer, "ERROR")  ) break;
    SerialDebug.println(buffer);
  }

  // switch back if necessary
  if ( current_mode == BLUEFRUIT_MODE_DATA ) setMode(BLUEFRUIT_MODE_DATA);

  SerialDebug.println(F("----------------"));

  _verbose = v;
}

/**************************************************************************/
/*!
    @brief  Checks if firmware is equal or later than specified version
*/
/**************************************************************************/
bool Adafruit_BLE::isVersionAtLeast(const char * versionString)
{
  uint8_t current_mode = _mode;

  // switch mode if necessary to execute command
  if ( current_mode == BLUEFRUIT_MODE_DATA ) setMode(BLUEFRUIT_MODE_COMMAND);

  // requesting version number
  println(F("ATI=4"));

  readline();
  bool result = ( strcmp(buffer, versionString) >= 0 );
  waitForOK();

  // switch back if necessary
  if ( current_mode == BLUEFRUIT_MODE_DATA ) setMode(BLUEFRUIT_MODE_DATA);

  return result;
}

/******************************************************************************/
/*!
    @brief  Get (multiple) lines of response data into internal buffer.

    @param[in] period_ms
               period in milliseconds between each event scanning
    @return    None
*/
/******************************************************************************/
void Adafruit_BLE::update(uint32_t period_ms)
{
  static TimeoutTimer tt;

  if ( tt.expired() )
  {
    tt.set(period_ms);

    bool v = _verbose;
    _verbose = false;

    uint8_t current_mode = _mode;

    // switch mode if necessary to execute command
    if ( current_mode == BLUEFRUIT_MODE_DATA ) setMode(BLUEFRUIT_MODE_COMMAND);

    println( F("AT+EVENTSTATUS") );
    readline();
    waitForOK();

    // parse event status system_event, gatts_event
    uint8_t tempbuf[BLE_BUFSIZE+1];
    uint32_t system_event, gatts_event;
    char * p_comma = NULL;

    system_event = strtoul(this->buffer, &p_comma, 16);
    gatts_event  = strtoul(p_comma+1, NULL, 16);

    //--------------------------------------------------------------------+
    // System Event
    //--------------------------------------------------------------------+
    if ( this->_connect_callback    && bitRead(system_event, EVENT_SYSTEM_CONNECT   ) ) this->_connect_callback();
    if ( this->_disconnect_callback && bitRead(system_event, EVENT_SYSTEM_DISCONNECT) ) this->_disconnect_callback();

    if ( this->_ble_uart_rx_callback && bitRead(system_event, EVENT_SYSTEM_BLE_UART_RX) )
    {
      // _verbose = true;
      println( F("AT+BLEUARTRX") );
      uint16_t len = readline(tempbuf, BLE_BUFSIZE);
      waitForOK();

      this->_ble_uart_rx_callback( (char*) tempbuf, len);
    }

    if ( this->_ble_midi_rx_callback && bitRead(system_event, EVENT_SYSTEM_BLE_MIDI_RX) )
    {
//      _verbose = true;
      while(1)
      {
        // use RAW command version
        println( F("AT+BLEMIDIRXRAW") );

        // readraw swallow OK/ERROR already
        uint16_t len = readraw();

        // break if there is no more MIDI event
        if ( len == 0 ) break;

        // copy to internal buffer for other usage !
        memcpy(tempbuf, this->buffer, len);

        Adafruit_BLEMIDI::processRxCallback(tempbuf, len, this->_ble_midi_rx_callback);
      }
    }

    //--------------------------------------------------------------------+
    // Gatt Event
    //--------------------------------------------------------------------+
    if ( this->_ble_gatt_rx_callback && gatts_event )
    {
//      _verbose = true;
      for(uint8_t charid=1; charid < 30; charid++)
      {
        if ( bitRead(gatts_event, charid-1) )
        {
          print( F("AT+GATTCHARRAW=") ); // use RAW command version
          println(charid);

          uint16_t len = readraw(); // readraw swallow OK/ERROR already
          memcpy(tempbuf, this->buffer, len);

          this->_ble_gatt_rx_callback(charid, tempbuf, len);
        }
      }
    }

    // switch back if necessary
    if ( current_mode == BLUEFRUIT_MODE_DATA ) setMode(BLUEFRUIT_MODE_DATA);

    _verbose = v;
  }
}

/******************************************************************************/
/*!
    @brief Set custom ADV data packet
    @param
*/
/******************************************************************************/
bool Adafruit_BLE::setAdvData(uint8_t advdata[], uint8_t size)
{
  return this->atcommand(F("AT+GAPSETADVDATA"), advdata, size);
}

/******************************************************************************/
/*!
    @brief Save user information to NVM section, current size limit is 256 bytes
    @param data buffer holding data
    @param size number of bytes
    @param offset relative offset in the NVM section
*/
/******************************************************************************/
bool Adafruit_BLE::writeNVM(uint16_t offset, uint8_t const data[], uint16_t size)
{
  VERIFY_(offset + size <= NVM_USERDATA_SIZE );

  uint16_t type[] = { AT_ARGTYPE_UINT16, AT_ARGTYPE_UINT8, (uint16_t) (AT_ARGTYPE_BYTEARRAY + size) };
  uint32_t args[] = { offset, BLE_DATATYPE_BYTEARRAY, (uint32_t) data };

  return this->atcommand_full(F("AT+NVMWRITE"), NULL, 3, type, args);
}

/******************************************************************************/
/*!
    @brief Save String to NVM section, current size limit is 256 bytes
    @param data buffer holding data
    @param size number of bytes
    @param offset relative offset in the NVM section
*/
/******************************************************************************/
bool Adafruit_BLE::writeNVM(uint16_t offset, char const* str)
{
  VERIFY_(offset + strlen(str) <= NVM_USERDATA_SIZE );

  uint16_t type[] = { AT_ARGTYPE_UINT16, AT_ARGTYPE_UINT8, AT_ARGTYPE_STRING };
  uint32_t args[] = { offset, BLE_DATATYPE_STRING, (uint32_t) str };

  return this->atcommand_full(F("AT+NVMWRITE"), NULL, 3, type, args);
}

/******************************************************************************/
/*!
    @brief Save an 32-bit number to NVM
    @param number Number to be saved
    @param offset relative offset in the NVM section
*/
/******************************************************************************/
bool Adafruit_BLE::writeNVM(uint16_t offset, int32_t number)
{
  VERIFY_(offset + 4 <= NVM_USERDATA_SIZE );

  uint16_t type[] = { AT_ARGTYPE_UINT16, AT_ARGTYPE_UINT8, AT_ARGTYPE_INT32 };
  uint32_t args[] = { offset, BLE_DATATYPE_INTEGER, (uint32_t) number };

  return this->atcommand_full(F("AT+NVMWRITE"), NULL, 3, type, args);
}

/******************************************************************************/
/*!
    @brief Read an number of bytes from NVM at offset to buffer
    @param
*/
/******************************************************************************/
bool Adafruit_BLE::readNVM(uint16_t offset, uint8_t data[], uint16_t size)
{
  VERIFY_(offset < NVM_USERDATA_SIZE);

  uint8_t current_mode = _mode;

  // switch mode if necessary to execute command
  if ( current_mode == BLUEFRUIT_MODE_DATA ) setMode(BLUEFRUIT_MODE_COMMAND);

  // use RAW command version
  print( F("AT+NVMREADRAW=") );
  print(offset);

  print(',');
  println(size);

  readraw(); // readraw swallow OK/ERROR already

  // skip if NULL is entered
  if (data) memcpy(data, this->buffer, min(size, BLE_BUFSIZE));

  // switch back if necessary
  if ( current_mode == BLUEFRUIT_MODE_DATA ) setMode(BLUEFRUIT_MODE_DATA);

  return true;
}

/******************************************************************************/
/*!
    @brief Read a string from NVM at offset to buffer
    @param
*/
/******************************************************************************/
bool Adafruit_BLE::readNVM(uint16_t offset, char* str, uint16_t size)
{
  VERIFY_(offset < NVM_USERDATA_SIZE);

  uint16_t type[] = { AT_ARGTYPE_UINT16, AT_ARGTYPE_UINT16, AT_ARGTYPE_UINT8 };
  uint32_t args[] = { offset, size, BLE_DATATYPE_STRING};

  bool isOK =  this->atcommand_full(F("AT+NVMREAD"), NULL, 3, type, args);

  // skip if NULL is entered
  if ( isOK && str ) strncpy(str, this->buffer, min(size, BLE_BUFSIZE));

  return isOK;
}

/******************************************************************************/
/*!
    @brief Read an 32-bit number from NVM
    @param
*/
/******************************************************************************/
bool Adafruit_BLE::readNVM(uint16_t offset, int32_t* number)
{
  return this->readNVM(offset, (uint8_t*)number, 4);
}

/**
 *
 * @param buffer
 * @param size
 * @return
 */
int Adafruit_BLE::writeBLEUart(uint8_t const * buffer, int size)
{
  uint8_t current_mode = _mode;

  // switch mode if necessary to execute command
  if ( current_mode == BLUEFRUIT_MODE_COMMAND ) setMode(BLUEFRUIT_MODE_DATA);

  size_t n = write(buffer, size);

  // switch back if necessary
  if ( current_mode == BLUEFRUIT_MODE_COMMAND ) setMode(BLUEFRUIT_MODE_COMMAND);

  return n;
}

/**
 *
 * @param buffer
 * @param size
 * @return
 */
int  Adafruit_BLE::readBLEUart(uint8_t* buffer, int size)
{
  uint8_t current_mode = _mode;

  // switch mode if necessary to execute command
  if ( current_mode == BLUEFRUIT_MODE_COMMAND ) setMode(BLUEFRUIT_MODE_DATA);

  size_t n = readBytes(buffer, size);

  // switch back if necessary
  if ( current_mode == BLUEFRUIT_MODE_COMMAND ) setMode(BLUEFRUIT_MODE_COMMAND);

  return n;
}

/******************************************************************************/
/*!
    @brief  Set handle for connect callback

    @param[in] fp function pointer, NULL will discard callback
*/
/******************************************************************************/
void Adafruit_BLE::setConnectCallback( void (*fp) (void) )
{
  this->_connect_callback = fp;
  install_callback(fp != NULL, EVENT_SYSTEM_CONNECT, -1);
}

/******************************************************************************/
/*!
    @brief  Set handle for disconnection callback

    @param[in] fp function pointer, NULL will discard callback
*/
/******************************************************************************/
void Adafruit_BLE::setDisconnectCallback( void (*fp) (void) )
{
  this->_disconnect_callback = fp;
  install_callback(fp != NULL, EVENT_SYSTEM_DISCONNECT, -1);
}

/******************************************************************************/
/*!
    @brief  Set handle for BLE Uart Rx callback

    @param[in] fp function pointer, NULL will discard callback
*/
/******************************************************************************/
void Adafruit_BLE::setBleUartRxCallback( void (*fp) (char data[], uint16_t len) )
{
  this->_ble_uart_rx_callback = fp;
  install_callback(fp != NULL, EVENT_SYSTEM_BLE_UART_RX, -1);
}

/******************************************************************************/
/*!
    @brief  Set handle for BLE MIDI Rx callback

    @param[in] fp function pointer, NULL will discard callback
*/
/******************************************************************************/
void Adafruit_BLE::setBleMidiRxCallback( midiRxCallback_t fp )
{
  this->_ble_midi_rx_callback = fp;
  install_callback(fp != NULL, EVENT_SYSTEM_BLE_MIDI_RX, -1);
}

/******************************************************************************/
/*!
    @brief  Set handle for BLE Gatt Rx callback

    @param[in] fp function pointer, NULL will discard callback
*/
/******************************************************************************/
void Adafruit_BLE::setBleGattRxCallback(int32_t chars_idx,  void (*fp) (int32_t, uint8_t[], uint16_t) )
{
  if ( chars_idx == 0) return;

  this->_ble_gatt_rx_callback = fp;
  install_callback(fp != NULL, -1, chars_idx-1);
}

