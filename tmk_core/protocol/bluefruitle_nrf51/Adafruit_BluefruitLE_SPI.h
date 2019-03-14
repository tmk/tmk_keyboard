/**************************************************************************/
/*!
    @file     Adafruit_BluefruiLE_SPI.h
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
#ifndef _ADAFRUIT_BLE_SPI_H_
#define _ADAFRUIT_BLE_SPI_H_

#include <Adafruit_BLE.h>
#include <SPI.h>
#include "utility/Adafruit_FIFO.h"

#define SPI_CS_ENABLE()           digitalWrite(m_cs_pin, LOW)
#define SPI_CS_DISABLE()          digitalWrite(m_cs_pin, HIGH)

#define SPI_IGNORED_BYTE          0xFEu /**< SPI default character. Character clocked out in case of an ignored transaction. */
#define SPI_OVERREAD_BYTE         0xFFu /**< SPI over-read character. Character clocked out after an over-read of the transmit buffer. */
#define SPI_DEFAULT_DELAY_US      50

#define memclr(buffer, size)  memset(buffer, 0, size)


class Adafruit_BluefruitLE_SPI : public Adafruit_BLE
{
  private:
    // Hardware Pin
    int8_t          m_cs_pin;
    int8_t          m_irq_pin;
    int8_t          m_rst_pin;

    // software SPI pins
    int8_t          m_sck_pin;
    int8_t          m_mosi_pin;
    int8_t          m_miso_pin;

    // TX
    uint8_t         m_tx_buffer[SDEP_MAX_PACKETSIZE];
    uint8_t         m_tx_count;

    // RX
    uint8_t         m_rx_buffer[BLE_BUFSIZE];
    Adafruit_FIFO   m_rx_fifo;

    // Low level transportation I/O functions
    bool    sendInitializePattern(void);
    bool    sendPacket(uint16_t command, const uint8_t* buffer, uint8_t count, uint8_t more_data);
    bool    getPacket(sdepMsgResponse_t* p_response);

    bool    getResponse(void);
    void    simulateSwitchMode(void);
//    bool    handleSwitchCmdInDataMode(uint8_t ch);

    uint8_t spixfer(uint8_t x);
    void spixfer(void *x, size_t len);

  public:
    // Constructor
    Adafruit_BluefruitLE_SPI(int8_t csPin, int8_t irqPin, int8_t rstPin = -1);
    Adafruit_BluefruitLE_SPI(int8_t clkPin, int8_t misoPin, int8_t mosiPin, int8_t csPin, int8_t irqPin, int8_t rstPin);

    // HW initialisation
    bool begin(boolean v = false, boolean blocking = true);
    void end(void);

    bool setMode(uint8_t new_mode);

    // Class Print virtual function Interface
    virtual size_t write(uint8_t c);
    virtual size_t write(const uint8_t *buffer, size_t size);

    // pull in write(str) and write(buf, size) from Print
    using Print::write;

    // Class Stream interface
    virtual int  available(void);
    virtual int  read(void);
    virtual void flush(void);
    virtual int  peek(void);
};

#endif
