/**************************************************************************/
/*!
    @file     Adafruit_BLE.h
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

#ifndef _Adafruit_BLE_H_
#define _Adafruit_BLE_H_

#include <stdint.h>
#include <Arduino.h>
#include "utility/common_header.h"
#include "utility/errors.h"
#include "utility/TimeoutTimer.h"
#include "Adafruit_ATParser.h"

#define BLE_DEFAULT_TIMEOUT      250

enum BLEDataType_t
{
  BLE_DATATYPE_AUTO = 0,
  BLE_DATATYPE_STRING,
  BLE_DATATYPE_BYTEARRAY,
  BLE_DATATYPE_INTEGER,
};


class Adafruit_BLE : public Adafruit_ATParser
{
  protected:
    enum
    {
      BLUEFRUIT_TRANSPORT_INVALID,
      BLUEFRUIT_TRANSPORT_HWUART,
      BLUEFRUIT_TRANSPORT_SWUART,
      BLUEFRUIT_TRANSPORT_HWSPI,
      BLUEFRUIT_TRANSPORT_SWSPI,
    };

//    uint8_t  _mode;
//    uint16_t _timeout;
    uint8_t  _physical_transport;
    uint32_t _reset_started_timestamp;

  public:
    typedef void (*midiRxCallback_t) (uint16_t timestamp, uint8_t status, uint8_t byte1, uint8_t byte2);

    // Constructor
    Adafruit_BLE(void);

    // Functions implemented in this base class
    bool reset(boolean blocking = true);
    bool factoryReset(boolean blocking = true);
    bool resetCompleted(void);

    void info(void);
    bool echo(bool enable);

    bool isConnected(void);
    bool isVersionAtLeast(const char * versionString);
    void disconnect(void);

    bool setAdvData(uint8_t advdata[], uint8_t size);

    bool writeNVM(uint16_t offset, uint8_t const  data[], uint16_t size);
    bool writeNVM(uint16_t offset, char    const* str);
    bool writeNVM(uint16_t offset, int32_t number);

    bool readNVM(uint16_t offset, uint8_t data[], uint16_t size);
    bool readNVM(uint16_t offset, char  * str   , uint16_t size);
    bool readNVM(uint16_t offset, int32_t* number);

    // helper with bleuart
    int writeBLEUart(uint8_t const * buffer, int size);
    int writeBLEUart(char const * str) { return writeBLEUart( (uint8_t const*) str, strlen(str)); }

    int readBLEUart(uint8_t* buffer, int size);


    // No parameters
    bool sendCommandCheckOK(const __FlashStringHelper *cmd) { return this->atcommand(cmd); }
    bool sendCommandCheckOK(const char cmd[])               { return this->atcommand(cmd); }

    bool sendCommandWithIntReply(const __FlashStringHelper *cmd, int32_t *reply) { return this->atcommandIntReply(cmd, reply); }
    bool sendCommandWithIntReply(const char cmd[]              , int32_t *reply) { return this->atcommandIntReply(cmd, reply); }

    // Physical transportation checking
    bool isTransportHwUart (void) { return _physical_transport == BLUEFRUIT_TRANSPORT_HWUART; }
    bool isTransportSwUart (void) { return _physical_transport == BLUEFRUIT_TRANSPORT_SWUART; }
    bool isTransportUart   (void) { return isTransportHwUart() || isTransportSwUart();        }

    bool isTransportHwSpi  (void) { return _physical_transport == BLUEFRUIT_TRANSPORT_HWSPI;  }
    bool isTransportSwSpi  (void) { return _physical_transport == BLUEFRUIT_TRANSPORT_SWSPI;  }
    bool isTransportSpi    (void) { return isTransportHwSpi() || isTransportSwSpi();          }

    /////////////////////
    // callback functions
    /////////////////////
    void update(uint32_t period_ms = 200);
    void handleDfuIrq(void)
    {
      this->update(0);
    }

    void setDisconnectCallback( void (*fp) (void) );
    void setConnectCallback   ( void (*fp) (void) );

    void setBleUartRxCallback( void (*fp) (char data[], uint16_t len) );
    void setBleMidiRxCallback( midiRxCallback_t fp );
    void setBleGattRxCallback( int32_t chars_idx, void (*fp) (int32_t, uint8_t[], uint16_t) );

  protected:
    // helper
    void install_callback(bool enable, int8_t system_id, int8_t gatts_id);

    void (*_disconnect_callback) (void);
    void (*_connect_callback) (void);

    void (*_ble_uart_rx_callback) (char data[], uint16_t len);
    midiRxCallback_t _ble_midi_rx_callback;

    void (*_ble_gatt_rx_callback) (int32_t chars_id, uint8_t data[], uint16_t len);
};

//--------------------------------------------------------------------+
// DEBUG HELPER
//--------------------------------------------------------------------+
#ifndef DBG_ENABLE
#define DBG_ENABLE      0
#endif

#if DBG_ENABLE
  #define DBG_LOCATION()  Serial.printf("%s: %d: \r\n", __PRETTY_FUNCTION__, __LINE__)
  #define DBG_INT(x)      do { Serial.print(#x " = "); Serial.println(x); } while(0)
  #define DBG_HEX(x)      do { Serial.print(#x " = "); Serial.println(x, HEX); } while(0)
  #define DBG_STR(x)      Serial.printf(#x " = %s\r\n", (char*)(x) )
  #define DBG_BUFFER(buf, n) \
    do {\
      uint8_t* p8 = (uint8_t*) (buf);\
      Serial.print(#buf ": ");\
      for(uint32_t i=0; i<(n); i++) Serial.printf("%02x ", p8[i]);\
      Serial.print("\r\n");\
    }while(0)
#endif

#endif /* _Adafruit_BLE_H_ */
