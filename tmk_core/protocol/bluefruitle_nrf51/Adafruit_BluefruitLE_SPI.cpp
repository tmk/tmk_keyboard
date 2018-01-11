/**************************************************************************/
/*!
    @file     Adafruit_BluefruitLE_SPI.cpp
    @author   hathach, ktown (Adafruit Industries)

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
#include "Adafruit_BluefruitLE_SPI.h"
#include <Arduino.h>
#include <stdlib.h>

#ifndef min
  #define min(a,b) ((a) < (b) ? (a) : (b))
#endif


SPISettings bluefruitSPI(4000000, MSBFIRST, SPI_MODE0);


/******************************************************************************/
/*!
    @brief Instantiates a new instance of the Adafruit_BluefruitLE_SPI class

    @param[in]  csPin
                The location of the CS pin for the SPI interface
    @param[in]  irqPin
                The location of the HW IRQ pin (pin 2 or pin 3 on the Arduino
                Uno). This must be a HW interrupt pin!
    @param[in]  rstPin
*/
/******************************************************************************/
Adafruit_BluefruitLE_SPI::Adafruit_BluefruitLE_SPI(int8_t csPin, int8_t irqPin, int8_t rstPin) :
    m_rx_fifo(m_rx_buffer, sizeof(m_rx_buffer), 1, true)
{
  _physical_transport = BLUEFRUIT_TRANSPORT_HWSPI;

  m_cs_pin  = csPin;
  m_irq_pin = irqPin;
  m_rst_pin = rstPin;

  m_miso_pin = m_mosi_pin = m_sck_pin = -1;

  m_tx_count = 0;
}

/******************************************************************************/
/*!
    @brief Instantiates a new instance of the Adafruit_BluefruitLE_SPI class
           using software SPI

    @param[in]  clkPin
                The location of the SCK/clock pin for the SPI interface
    @param[in]  misoPin
                The location of the MISO pin for the SPI interface
    @param[in]  mosiPin
                The location of the MOSI pin for the SPI interface
    @param[in]  csPin
                The location of the CS pin for the SPI interface
    @param[in]  irqPin
                The location of the HW IRQ pin (pin 2 or pin 3 on the Arduino
                Uno). This must be a HW interrupt pin!
    @param[in]  rstPin
*/
/******************************************************************************/
Adafruit_BluefruitLE_SPI::Adafruit_BluefruitLE_SPI(int8_t clkPin, int8_t misoPin,
    int8_t mosiPin, int8_t csPin, int8_t irqPin, int8_t rstPin) :
    m_rx_fifo(m_rx_buffer, sizeof(m_rx_buffer), 1, true)
{
  _physical_transport = BLUEFRUIT_TRANSPORT_SWSPI;

  m_sck_pin  = clkPin;
  m_miso_pin = misoPin;
  m_mosi_pin = mosiPin;
  m_cs_pin   = csPin;
  m_irq_pin  = irqPin;
  m_rst_pin  = rstPin;

  m_tx_count = 0;
}


/******************************************************************************/
/*!
    @brief Initialize the HW to enable communication with the BLE module

    @return Returns 'true' if everything initialised correctly, otherwise
            'false' if there was a problem during HW initialisation. If
            'irqPin' is not a HW interrupt pin false will be returned.
*/
/******************************************************************************/
bool Adafruit_BluefruitLE_SPI::begin(boolean v, boolean blocking)
{
  _verbose = v;

  pinMode(m_irq_pin, INPUT);

  // Set CS pin to output and de-assert by default
  pinMode(m_cs_pin, OUTPUT);
  digitalWrite(m_cs_pin, HIGH);

  if (m_sck_pin == -1) {
    // hardware SPI
    SPI.begin();
  } else {
    pinMode(m_sck_pin, OUTPUT);
    digitalWrite(m_sck_pin, LOW);
    pinMode(m_miso_pin, INPUT);
    pinMode(m_mosi_pin, OUTPUT);
  }

  bool isOK;

  // Always try to send Initialize command to reset
  // Bluefruit since user can define but not wiring RST signal
  isOK = sendInitializePattern();

  // use hardware reset if available
  if (m_rst_pin >= 0)
  {
    // pull the RST to GND for 10 ms
    pinMode(m_rst_pin, OUTPUT);
    digitalWrite(m_rst_pin, HIGH);
    digitalWrite(m_rst_pin, LOW);
    delay(10);
    digitalWrite(m_rst_pin, HIGH);

    isOK= true;
  }

  _reset_started_timestamp = millis();

  // Bluefruit takes 1 second to reboot
  if (blocking)
  {
    delay(1000);
  }

  return isOK;
}

/******************************************************************************/
/*!
    @brief  Uninitializes the SPI interface
*/
/******************************************************************************/
void Adafruit_BluefruitLE_SPI::end(void)
{
  if (m_sck_pin == -1) {
    SPI.end();
  }
}

/******************************************************************************/
/*!
    @brief Handle direct "+++" input command from user.
           User should use setMode instead
*/
/******************************************************************************/
void Adafruit_BluefruitLE_SPI::simulateSwitchMode(void)
{
  _mode = 1 - _mode;

  char ch = '0' + _mode;
  m_rx_fifo.write(&ch);
  m_rx_fifo.write_n("\r\nOK\r\n", 6);
}

/******************************************************************************/
/*!
    @brief Simulate "+++" switch mode command
*/
/******************************************************************************/
bool Adafruit_BluefruitLE_SPI::setMode(uint8_t new_mode)
{
  // invalid mode
  if ( !(new_mode == BLUEFRUIT_MODE_COMMAND || new_mode == BLUEFRUIT_MODE_DATA) ) return false;

  // Already in the wanted mode
  if ( _mode == new_mode ) return true;

  // SPI use different SDEP command when in DATA/COMMAND mode.
  // --> does not switch using +++ command
  _mode = new_mode;

  return true;
}

/******************************************************************************/
/*!
    @brief Send initialize pattern to Bluefruit LE to force a reset. This pattern
    follow the SDEP command syntax with command_id = SDEP_CMDTYPE_INITIALIZE.
    The command has NO response, and is expected to complete within 1 second
*/
/******************************************************************************/
bool Adafruit_BluefruitLE_SPI::sendInitializePattern(void)
{
  return sendPacket(SDEP_CMDTYPE_INITIALIZE, NULL, 0, 0);
}

/******************************************************************************/
/*!
    @brief  Send out an packet with data in m_tx_buffer

    @param[in]  more_data
                More Data bitfield, 0 indicates this is not end of transfer yet
*/
/******************************************************************************/
bool Adafruit_BluefruitLE_SPI::sendPacket(uint16_t command, const uint8_t* buf, uint8_t count, uint8_t more_data)
{
  // flush old response before sending the new command
  if (more_data == 0) flush();

  sdepMsgCommand_t msgCmd;

  msgCmd.header.msg_type    = SDEP_MSGTYPE_COMMAND;
  msgCmd.header.cmd_id_high = highByte(command);
  msgCmd.header.cmd_id_low  = lowByte(command);
  msgCmd.header.length      = count;
  msgCmd.header.more_data   = (count == SDEP_MAX_PACKETSIZE) ? more_data : 0;

  // Copy payload
  if ( buf != NULL && count > 0) memcpy(msgCmd.payload, buf, count);

  // Starting SPI transaction
  if (m_sck_pin == -1)
    SPI.beginTransaction(bluefruitSPI);

  SPI_CS_ENABLE();

  TimeoutTimer tt(_timeout);

  // Bluefruit may not be ready
  while ( ( spixfer(msgCmd.header.msg_type) == SPI_IGNORED_BYTE ) && !tt.expired() )
  {
    // Disable & Re-enable CS with a bit of delay for Bluefruit to ready itself
    SPI_CS_DISABLE();
    delayMicroseconds(SPI_DEFAULT_DELAY_US);
    SPI_CS_ENABLE();
  }

  bool result = !tt.expired();
  if ( result )
  {
    // transfer the rest of the data
    spixfer((void*) (((uint8_t*)&msgCmd) +1), sizeof(sdepMsgHeader_t)+count-1);
  }

  SPI_CS_DISABLE();
  if (m_sck_pin == -1)
    SPI.endTransaction();

  return result;
}

/******************************************************************************/
/*!
    @brief  Print API. Either buffer the data internally or send it to bus
            if possible. \r and \n are command terminators and will force the
            packet to be sent to the Bluefruit LE module.

    @param[in]  c
                Character to send
*/
/******************************************************************************/
size_t Adafruit_BluefruitLE_SPI::write(uint8_t c)
{
  if (_mode == BLUEFRUIT_MODE_DATA)
  {
    sendPacket(SDEP_CMDTYPE_BLE_UARTTX, &c, 1, 0);
    getResponse();
    return 1;
  }

  // Following code handle BLUEFRUIT_MODE_COMMAND

  // Final packet due to \r or \n terminator
  if (c == '\r' || c == '\n')
  {
    if (m_tx_count > 0)
    {
      // +++ command to switch mode
      if ( memcmp(m_tx_buffer, "+++", 3) == 0)
      {
        simulateSwitchMode();
      }else
      {
        sendPacket(SDEP_CMDTYPE_AT_WRAPPER, m_tx_buffer, m_tx_count, 0);
      }
      m_tx_count = 0;
    }
  }
  // More than max packet buffered --> send with more_data = 1
  else if (m_tx_count == SDEP_MAX_PACKETSIZE)
  {
    sendPacket(SDEP_CMDTYPE_AT_WRAPPER, m_tx_buffer, m_tx_count, 1);

    m_tx_buffer[0] = c;
    m_tx_count = 1;
  }
  // Not enough data, continue to buffer
  else
  {
    m_tx_buffer[m_tx_count++] = c;
  }

  if (_verbose) SerialDebug.print((char) c);

  return 1;
}

/******************************************************************************/
/*!

*/
/******************************************************************************/
size_t Adafruit_BluefruitLE_SPI::write(const uint8_t *buf, size_t size)
{
  if ( _mode == BLUEFRUIT_MODE_DATA )
  {
    if ((size >= 3) &&
        !memcmp(buf, "+++", 3) &&
        !(size > 3 && buf[3] != '\r' && buf[3] != '\n') )
    {
      simulateSwitchMode();
    }else
    {
      size_t remain = size;
      while(remain)
      {
        size_t len = min(remain, SDEP_MAX_PACKETSIZE);
        remain -= len;

        sendPacket(SDEP_CMDTYPE_BLE_UARTTX, buf, (uint8_t) len, remain ? 1 : 0);
        buf += len;
      }

      getResponse();
    }

    return size;
  }
  // Command mode
  else
  {
    size_t n = 0;
    while (size--) {
      n += write(*buf++);
    }
    return n;
  }
}

/******************************************************************************/
/*!
    @brief Check if the response from the previous command is ready

    @return 'true' if a response is ready, otherwise 'false'
*/
/******************************************************************************/
int Adafruit_BluefruitLE_SPI::available(void)
{
  if (! m_rx_fifo.empty() ) {
    return m_rx_fifo.count();
  }

  if ( _mode == BLUEFRUIT_MODE_DATA )
  {
    // DATA Mode: query for BLE UART data
    sendPacket(SDEP_CMDTYPE_BLE_UARTRX, NULL, 0, 0);

    // Waiting to get response from Bluefruit
    getResponse();

    return m_rx_fifo.count();
  }else
  {
    return (digitalRead(m_irq_pin));
  }
}

/******************************************************************************/
/*!
    @brief Get a byte from response data, perform SPI transaction if needed

    @return -1 if no data is available
*/
/******************************************************************************/
int Adafruit_BluefruitLE_SPI::read(void)
{
  uint8_t ch;

  // try to grab from buffer first...
  if (!m_rx_fifo.empty()) {
    m_rx_fifo.read(&ch);
    return (int)ch;
  }

  if ( _mode == BLUEFRUIT_MODE_DATA )
  {
    // DATA Mode: query for BLE UART data
    sendPacket(SDEP_CMDTYPE_BLE_UARTRX, NULL, 0, 0);

    // Waiting to get response from Bluefruit
    getResponse();
  }else
  {
    // COMMAND Mode: Only read data from Bluefruit if IRQ is raised
    if ( digitalRead(m_irq_pin) ) getResponse();
  }

  return m_rx_fifo.read(&ch) ? ((int) ch) : EOF;

}

/******************************************************************************/
/*!
    @brief Get a byte from response without removing it, perform SPI transaction
           if needed

    @return -1 if no data is available
*/
/******************************************************************************/
int Adafruit_BluefruitLE_SPI::peek(void)
{
  uint8_t ch;

  // try to grab from buffer first...
  if ( m_rx_fifo.peek(&ch) ) {
    return (int)ch;
  }

  if ( _mode == BLUEFRUIT_MODE_DATA )
  {
    // DATA Mode: query for BLE UART data
    sendPacket(SDEP_CMDTYPE_BLE_UARTRX, NULL, 0, 0);

    // Waiting to get response from Bluefruit
    getResponse();
  }else
  {
    // Read data from Bluefruit if possible
    if ( digitalRead(m_irq_pin) ) getResponse();
  }

  return m_rx_fifo.peek(&ch) ? ch : EOF;
}

/******************************************************************************/
/*!
    @brief Flush current response data in the internal FIFO

    @return -1 if no data is available
*/
/******************************************************************************/
void Adafruit_BluefruitLE_SPI::flush(void)
{
  m_rx_fifo.clear();
}

/******************************************************************************/
/*!
    @brief  Try to perform an full AT response transfer from Bluefruit, or execute
            as many SPI transaction as internal FIFO can hold up.

    @note   If verbose is enabled, all the received data will be print to Serial

    @return
      - true  : if succeeded
      - false : if failed
*/
/******************************************************************************/
bool Adafruit_BluefruitLE_SPI::getResponse(void)
{
  // Try to read data from Bluefruit if there is enough room in the fifo
  while ( m_rx_fifo.remaining() >= SDEP_MAX_PACKETSIZE )
  {
    // Get a SDEP packet
    sdepMsgResponse_t msg_response;
    memclr(&msg_response, sizeof(sdepMsgResponse_t));

    if ( !getPacket(&msg_response) ) return false;

    // Write to fifo
    if ( msg_response.header.length > 0)
    {
      m_rx_fifo.write_n(msg_response.payload, msg_response.header.length);
    }

    // No more packet data
    if ( !msg_response.header.more_data ) break;

    // It takes a bit since all Data received to IRQ to get LOW
    // May need to delay a bit for it to be stable before the next try
    // delayMicroseconds(SPI_DEFAULT_DELAY_US);
  }

  return true;
}

/******************************************************************************/
/*!
    @brief      Perform a single SPI SDEP transaction and is used by getReponse to
                get a full response composed of multiple packets.

    @param[in]  buf
                Memory location where payload is copied to

    @return number of bytes in SDEP payload
*/
/******************************************************************************/
bool Adafruit_BluefruitLE_SPI::getPacket(sdepMsgResponse_t* p_response)
{
  // Wait until IRQ is asserted, double timeout since some commands take long time to start responding
  TimeoutTimer tt(2*_timeout);
  
  while ( !digitalRead(m_irq_pin) ) {
    if (tt.expired()) return false;
  }
  
  sdepMsgHeader_t* p_header = &p_response->header;

  if (m_sck_pin == -1)
    SPI.beginTransaction(bluefruitSPI);
  SPI_CS_ENABLE();

  tt.set(_timeout);

  do {
    if ( tt.expired() ) break;

    p_header->msg_type = spixfer(0xff);

    if (p_header->msg_type == SPI_IGNORED_BYTE)
    {
      // Bluefruit may not be ready
      // Disable & Re-enable CS with a bit of delay for Bluefruit to ready itself
      SPI_CS_DISABLE();
      delayMicroseconds(SPI_DEFAULT_DELAY_US);
      SPI_CS_ENABLE();
    }
    else if (p_header->msg_type == SPI_OVERREAD_BYTE)
    {
      // IRQ may not be pulled down by Bluefruit when returning all data in previous transfer.
      // This could happen when Arduino MCU is running at fast rate comparing to Bluefruit's MCU,
      // causing an SPI_OVERREAD_BYTE to be returned at stage.
      //
      // Walkaround: Disable & Re-enable CS with a bit of delay and keep waiting
      // TODO IRQ is supposed to be OFF then ON, it is better to use GPIO trigger interrupt.

      SPI_CS_DISABLE();
      // wait for the clock to be enabled..
//      while (!digitalRead(m_irq_pin)) {
//        if ( tt.expired() ) break;
//      }
//      if (!digitalRead(m_irq_pin)) break;
      delayMicroseconds(SPI_DEFAULT_DELAY_US);
      SPI_CS_ENABLE();
    }
  }  while (p_header->msg_type == SPI_IGNORED_BYTE || p_header->msg_type == SPI_OVERREAD_BYTE);

  bool result=false;

  // Not a loop, just a way to avoid goto with error handling
  do
  {
    // Look for the header
    // note that we should always get the right header at this point, and not doing so will really mess up things.
    while ( p_header->msg_type != SDEP_MSGTYPE_RESPONSE && p_header->msg_type != SDEP_MSGTYPE_ERROR && !tt.expired() )
    {
      p_header->msg_type = spixfer(0xff);
    }
    
    if ( tt.expired() ) break;
    
    memset( (&p_header->msg_type)+1, 0xff, sizeof(sdepMsgHeader_t) - 1);
    spixfer((&p_header->msg_type)+1, sizeof(sdepMsgHeader_t) - 1);

    // Command is 16-bit at odd address, may have alignment issue with 32-bit chip
    uint16_t cmd_id = word(p_header->cmd_id_high, p_header->cmd_id_low);

    // Error Message Response
    if ( p_header->msg_type == SDEP_MSGTYPE_ERROR ) break;

    // Invalid command
    if (!(cmd_id == SDEP_CMDTYPE_AT_WRAPPER ||
          cmd_id == SDEP_CMDTYPE_BLE_UARTTX ||
          cmd_id == SDEP_CMDTYPE_BLE_UARTRX) )
    {
      break;
    }

    // Invalid length
    if(p_header->length > SDEP_MAX_PACKETSIZE) break;

    // read payload
    memset(p_response->payload, 0xff, p_header->length);
    spixfer(p_response->payload, p_header->length);

    result = true;
  }while(0);

  SPI_CS_DISABLE();
  if (m_sck_pin == -1)
    SPI.endTransaction();

  return result;
}

/******************************************************************************/
/*!

*/
/******************************************************************************/
void Adafruit_BluefruitLE_SPI::spixfer(void *buff, size_t len) {
  uint8_t *p = (uint8_t *)buff;

  while (len--) {
    p[0] = spixfer(p[0]);
    p++;
  }
}

/******************************************************************************/
/*!

*/
/******************************************************************************/
uint8_t Adafruit_BluefruitLE_SPI::spixfer(uint8_t x) {
  if (m_sck_pin == -1) {
    uint8_t reply = SPI.transfer(x);
    //SerialDebug.println(reply, HEX);
    return reply;
  }

  // software spi
  uint8_t reply = 0;
  for (int i=7; i>=0; i--) {
    reply <<= 1;
    digitalWrite(m_sck_pin, LOW);
    digitalWrite(m_mosi_pin, x & (1<<i));
    digitalWrite(m_sck_pin, HIGH);
    if (digitalRead(m_miso_pin))
      reply |= 1;
  }
  digitalWrite(m_sck_pin, LOW);
  //SerialDebug.println(reply, HEX);
  return reply;
}
