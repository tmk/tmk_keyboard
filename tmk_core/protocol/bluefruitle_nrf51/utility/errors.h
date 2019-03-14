/******************************************************************************/
/*!
    @file     errors.h

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
/******************************************************************************/
#ifndef _ERRORS_H_
#define _ERRORS_H_

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************/
/*!
    @brief  Standard error codes used throughout this codebase
*/
/******************************************************************************/
typedef enum
{
  /*=======================================================================
    CUSTOM GLOBAL ERRORS                                   0x8000 .. 0x801F
    -----------------------------------------------------------------------
    Custom global errors (not defined by Nordic)
    -----------------------------------------------------------------------*/
    ERROR_INITFAILED                              = 0x8001,   ///< Function or driver initialisation failed
    ERROR_NOT_INITIALISED                         = 0x8002,   ///< Attempted to call a function in an unitialised device/driver
    ERROR_I2C_XFER_FAILED                         = 0x8003,   ///< I2C transfer failure
  /*=======================================================================*/


  /*=======================================================================
    SIMPLE BINARY PROTOCOL ERRORS                          0x8020 .. 0x803F
    -----------------------------------------------------------------------
    Errors relating to the simple binary protocol
    -----------------------------------------------------------------------*/
    ERROR_SDEP_INVALIDMSGTYPE                     = 0x8021,    ///< Unexpected msg type encountered
    ERROR_SDEP_INVALIDCOMMANDID                   = 0x8022,    ///< Unknown or out of range command ID
    ERROR_SDEP_INVALIDPAYLOAD                     = 0x8023,    ///< Message payload has a problem (invalid len, etc.)
  /*=======================================================================*/


  /*=======================================================================
    COMMAND LINE INTERFACE ERRORS                          0x8040 .. 0x805F
    -----------------------------------------------------------------------
    Errors relating to the simple binary protocol
    -----------------------------------------------------------------------*/
    ERROR_CLI_UNKNOWN_COMMAND                     = 0x8041,    ///< Unknown Command entered
    ERROR_CLI_TOO_FEW_ARGUMENTS                   = 0x8042,    ///< Too Few Arguments
    ERROR_CLI_TOO_MANY_ARGUMENTS                  = 0x8043,    ///< Too Few Arguments
  /*=======================================================================*/

  /*=======================================================================
    AT PARSER ERRORS                                       0x8060 .. 0x807F
    -----------------------------------------------------------------------
    Errors relating to AT Command
    -----------------------------------------------------------------------*/
    ERROR_AT_INVALID_INPUT                        = 0x8060,
    ERROR_AT_UNKNOWN_COMMAND                      = 0x8061,
    ERROR_AT_INVALID_PARAMETER                    = 0x8062,
  /*=======================================================================*/

  /*=======================================================================
    NVM ERRORS                                              0x8080 .. 0x809F
    -----------------------------------------------------------------------
    Errors relating to the NVM
    -----------------------------------------------------------------------*/
    ERROR_NVM_BUSY                                = 0x8060,
  /*=======================================================================*/

  /*=======================================================================
    OS Abstraction Layer                                   0x80A0 .. 0x80BF
    -----------------------------------------------------------------------
    Errors relating to the NVM
    -----------------------------------------------------------------------*/
    ERROR_OSAL_WAITING                            = 0x80A0,
  /*=======================================================================*/

  /*=======================================================================
    NORDIC GLOBAL ERRORS                                   0x0000 .. 0x00FF
    -----------------------------------------------------------------------
    Errors mapped from nrf_error.h
    -----------------------------------------------------------------------*/
    ERROR_NONE                                    = 0x0000,   ///< Successful command
    ERROR_SVC_HANDLER_MISSING                     = 0x0001,   ///< SVC handler is missing
    ERROR_SOFTDEVICE_NOT_ENABLED                  = 0x0002,   ///< SoftDevice has not been enabled
    ERROR_INTERNAL                                = 0x0003,   ///< Internal Error
    ERROR_NO_MEM                                  = 0x0004,   ///< No Memory for operation
    ERROR_NOT_FOUND                               = 0x0005,   ///< Not found
    ERROR_NOT_SUPPORTED                           = 0x0006,   ///< Not supported
    ERROR_INVALID_PARAM                           = 0x0007,   ///< Invalid Parameter
    ERROR_INVALID_STATE                           = 0x0008,   ///< Invalid state, operation disallowed in this state
    ERROR_INVALID_LENGTH                          = 0x0009,   ///< Invalid Length
    ERROR_INVALID_FLAGS                           = 0x000A,   ///< Invalid Flags
    ERROR_INVALID_DATA                            = 0x000B,   ///< Invalid Data
    ERROR_DATA_SIZE                               = 0x000C,   ///< Data size exceeds limit
    ERROR_TIMEOUT                                 = 0x000D,   ///< Operation timed out
    ERROR_NULL                                    = 0x000E,   ///< Null Pointer
    ERROR_FORBIDDEN                               = 0x000F,   ///< Forbidden Operation
    ERROR_INVALID_ADDR                            = 0x0010,   ///< Bad Memory Address
    ERROR_BUSY                                    = 0x0011,   ///< Busy
  /*=======================================================================*/


  /*=======================================================================
    NORDIC SDM ERRORS                                      0x1000 .. 0x1FFF
    -----------------------------------------------------------------------
    Errors based on Nordic's SDM nrf_error_sdm.h
    -----------------------------------------------------------------------*/
    ERROR_SDM_LFCLK_SOURCE_UNKNOWN                = 0x1000,   ///< Unknown lfclk source
    ERROR_SDM_INCORRECT_INTERRUPT_CONFIGURATION   = 0x1001,   ///< Incorrect interrupt configuration (can be caused by using illegal priority levels, or having enabled SoftDevice interrupts)
    ERROR_SDM_INCORRECT_CLENR0                    = 0x1002,   ///< Incorrect CLENR0 (can be caused by erronous SoftDevice flashing)
  /*=======================================================================*/


  /*=======================================================================
    NORDIC SOC ERRORS                                      0x2000 .. 0x2FFF
    -----------------------------------------------------------------------
    Errors based on Nordic's nrf_error_soc.h
    -----------------------------------------------------------------------*/
    /* Mutex Errors */
    ERROR_SOC_MUTEX_ALREADY_TAKEN                 = 0x2000,   ///< Mutex already taken

    /* NVIC errors */
    ERROR_SOC_NVIC_INTERRUPT_NOT_AVAILABLE        = 0x2001,   ///< NVIC interrupt not available
    ERROR_SOC_NVIC_INTERRUPT_PRIORITY_NOT_ALLOWED = 0x2002,   ///< NVIC interrupt priority not allowed
    ERROR_SOC_NVIC_SHOULD_NOT_RETURN              = 0x2003,   ///< NVIC should not return

    /* Power errors */
    ERROR_SOC_POWER_MODE_UNKNOWN                  = 0x2004,   ///< Power mode unknown
    ERROR_SOC_POWER_POF_THRESHOLD_UNKNOWN         = 0x2005,   ///< Power POF threshold unknown
    ERROR_SOC_POWER_OFF_SHOULD_NOT_RETURN         = 0x2006,   ///< Power off should not return

    /* Rand errors */
    ERROR_SOC_RAND_NOT_ENOUGH_VALUES              = 0x2007,   ///< RAND not enough values

    /* PPI errors */
    ERROR_SOC_PPI_INVALID_CHANNEL                 = 0x2008,   ///< Invalid PPI Channel
    ERROR_SOC_PPI_INVALID_GROUP                   = 0x2009,   ///< Invalid PPI Group
  /*=======================================================================*/


  /*=======================================================================
    NORDIC BLE ERRORS                                      0x3000 .. 0x3FFF
    -----------------------------------------------------------------------
    Errors based on Nordic's ble_err.h
    -----------------------------------------------------------------------*/
    ERROR_BLE_NOT_ENABLED                         = 0x3001,
    ERROR_BLE_INVALID_CONN_HANDLE                 = 0x3002,   ///< Invalid connection handle.
    ERROR_BLE_INVALID_ATTR_HANDLE                 = 0x3003,   ///< Invalid attribute handle.
    ERROR_BLE_NO_TX_BUFFERS                       = 0x3004,   ///< Buffer capacity exceeded.

    /* L2CAP */
    ERROR_BLE_L2CAP_CID_IN_USE                    = 0x3100,   ///< CID already in use.

    /* GAP */
    ERROR_BLE_GAP_UUID_LIST_MISMATCH              = 0x3200,   ///< UUID list does not contain an integral number of UUIDs.
    ERROR_BLE_GAP_DISCOVERABLE_WITH_WHITELIST     = 0x3201,   ///< Use of Whitelist not permitted with discoverable advertising.
    ERROR_BLE_GAP_INVALID_BLE_ADDR                = 0x3202,   ///< The upper two bits of the address do not correspond to the specified address type.

    /* GATTC */
    ERROR_BLE_GATTC_PROC_NOT_PERMITTED            = 0x3300,

    /* GATTS */
    ERROR_BLEGATTS_INVALID_ATTR_TYPE              = 0x3400,   ///< Invalid attribute type.
    ERROR_BLEGATTS_SYS_ATTR_MISSING               = 0x3401,   ///< System Attributes missing.
  /*=======================================================================*/
} err_t;

#ifdef __cplusplus
}
#endif

#endif /* _ERROR_H_ */
