# Firmware Changelog

## 0.7.7

### Features

- Add `AT+BLEUARTTXF` (F for force) to immediately send data as it is in an BLE Packet
- Adjust bleuart sending interval based on min connection interval
- Add `AT+DFUIRQ` to enable using DFU Pin for IRQ purpose when there is new events from nrf51
- Enable CS pull up for Bluefruit SPI
- Add `AT+MODESWITCHEN` to enable/disable +++ mode switch from local (serial/spi) or bleuart. Default local = enabled, ble = disable. Command can only be executed via local interface.
- Implement '\+' escape to immediate send '+' without trigger +++ waiting
- Add `AT+BLEHIDGAMEPADEN` to separately enable HID Gamepad, since iOS/OSX conflict with gamepad device that cause the HID keyboard not working properly.

### Bug fixes

- fix factory reset after a long time no reset in app_error_handler()
- fix Strings truncated at 64 chars in UART
- fix HID keyboard does not work with iOS 9 & 10

## 0.7.0

### Features

- BTLE UART speed and stability improvements
- HW UART
	- `AT+Baudrate` to change HW UART baud rate
	- `AT+uartflow` to enable/disable HW UART flow control
- MIDI (work in progress)
	- `AT+BLEMIDIEN=on/off/0/1` to enable/disable MIDI service, requires a reset to take affect
	- `AT+BLEMIDITX=midi event` to send MIDI event(s)
	- `AT+BLEMIDIRX` to receive MIDI event(s)
- GATT server
	- Added `DATATYPE` option to `AT+GATTADDCHAR`. Valid options are : AUTO (0, default), STRING (1), BYTEARRAY (2), INTEGER (3) 
	- Added `AT+GATTCHARRAW` (read-only) to read binary data (instead of ascii) from a characteristic. It is non-printable but has less overhead and is easier to use when writing libraries for Arduino. 
	- Reworked memory management for Service & Characteristics in NVM
	- Increased MAX_LEN for each characteristic from 20 to 32 bytes
	- Added Characteristic User Description support via the `DESCRIPTION` flag
	- Added Characteristic Presentation Format support via the `PRESENTATION` flag
- NVM user data allowing users to store data in a dedicated NVM section (up to 256 bytes)
	- `AT+NVMWRITE=offset,datatype,data` where datatype must be STRING (1), BYTEARRAY (2), or INTEGER (3)
	- `AT+NVMREAD=offset,size,datatype` to read data back
	- `AT+NVMREADRAW=offset,size`  binary data (instead of ascii) is returned ended with OK\r\n. It is non-printable but less overhead and easier to use when writing libraries in Arduino. 
- Simple callback implementation (work in progress)
	- BLTE UART rx
	- GATT chars rx
	- MIDI rx
	- Connect/Disconnect
- HID Service: 
	- Added pre-defined consumer "EJECT" which can be used to hide/show keyboard layout on iOS
	- Added HID Gamepad `at+blehidgamepad=x,y.buttons` where
		- X corresponds to LEFT, RIGHT: X=-1 means LEFT is pressed, X=1 means RIGHT is pressed, X=0 nothing pressed
		- Y corresponds to UP, DOWN: Y=-1 means UP, Y=1 means DOWN, Y=0 nothing pressed
		- Button [0x00-0xFF] is a bit mask for 8 buttons, 0-7 
- GAP: changed default parameters
	- ADV interval = 20 ms
	- Min connection interval = 20 ms
	- Max connection interval = 40 ms
	- Added `AT+GAPCONNECTABLE=on/off/1/0` to allow/disallow connection to device.
	- Added LOW Power Adv Interval to `AT+GAPINTERVALS=min_conn,max_conn,adv_interval,adv_timeout,adv_lowpower_interval`, default is 417.5 ms
	- Increased maximum number of CCCD records saved to flash from 8 to 16
- EddyStone Service
	- Eddystone config service disabled by default
	- Removed AT+EDDYSTONEENABLE to avoid confusion due to name (use AT+EDDYSTONESERVICEEN)
	- Added AT+EDDYSTONESERVICEEN to add/remove EddyStone service to GATT table (requires a reset)
	- Added AT+EDDYSTONEBROADCAST=on/off/0/1 to start/stop broadcasting url using nvm saved setting
	- Changed adv timeout for Eddystone to unlimited
- Battery Service
	- Added `AT+BLEBATTEN=on/off/1/0` to enable Battery service. Reset required.
	- Added `AT+BLEBATTVAL=percent` to update the Battery level, percent is 0 to 100

### Bug Fixes

- Fixed a bug where 'Write-No-Response' characteristic properties weren't being handled properly
- Fixed timing constraints to meet Apple design guidelines
- Corrected systick to ms calculation, optimized 64bit division by using shifter
- Fixed all the tests with google Eddystone validator except for writing tx_power = 1 dB (not valid on nrf51)
- Fixed a big where writing from a central does not update value on characteristic correctly
- Fixed an issue with HID examples, where when paired with a central, a disconnect then reconnect meant you could not send reports anymore.

## 0.6.7

### Features

- EddyStone/Uribeacon
	- Add TX Power to AT+EddyStoneUrl: `AT+EDDYSTONEURL=URL,[ADVWHENCONNECTED=0],[RSSI@0M=-18]`
	- Enable reading the current url: `AT+EDDYSTONEURL`
- Increase FIFO size for 
	- AT command fifo from 160 to 256 byte
	- BLE UART TX fifo from 160 to 1024 bytes
	- SPI RX fifo from 160 to 1024 bytes
- Add command to return free FIFO size for BLE UART:
	AT+BLEUARTFIFO
	0,1024
	AT+BLEUARTFIFO=TX
	0
	AT+BLEUARTFIFO=RX
	1024
- Keep URI UUID
	- AT+BLEURIBEACON will use the legacy uuid 0xFED8
	- AT+EDDYSTONEURL will use the eddystone uuid 0xFEAA
- Add escape '\' when sending A SINGLE '?' due to conflict with AT command existence test mode
	- AT+BLEUARTTX=\?
	- AT+BLEKEYBOARD=\?

### Bug Fixes

- Fix and improve BLE UART TX fifo overflow, wait up to 200 ms for fifo to free up some spaces, return ERROR if timed out. Note: a single BLE UART TX command can have ~200 bytes, the command may need to wait several turns (each is 200ms timeout), thus the total timeout can be longer depending on the FIFO state and the sending rate from the Central/Host device. Arudino's library has its own timer and can be timeout before the sending is done !!!!!
- Fix the factory reset issue if the Bluefruit is running for a long time (> 4 hours).
- fix a problem with gatt server: incorrect value_len parsing for characteristics's integer with max_len >4 

## 0.6.6

### Features

- Added basic [Eddystone](https://github.com/google/eddystone) support (currently [URL mode](https://github.com/google/eddystone/tree/master/eddystone-url) only):
	- AT+EDDYSTONEURL : Update the url for the beacon and go to beacon mode
	- AT+EDDYSTONEENABLE : Enable/disable beacon mode using the configured url
	- AT+EDDYSTONECONFIGEN : Enable the Eddystone configuration service 
- AT+HWMODELED enhancements. The MODE LED can now be set to:
	- DISABLE - No LED activity (to save power)
	- UART/DATA Mode - Indicate current operating mode (command or DATA) - DEFAULT
	- HWUART - LED toggles on HW UART activity
	- BLEUART - LED toggles on nRF/BLE UART activity (the Nordic UART Service)
	- SPI - LED toggles on SPI activity
	- MANUAL - Allows manual control of the LED via a second ON/OFF/TOGGLE parameter
- Added HID Keyboard Consumer Control support via AT+BLECONTROLKEY
- Added HID mouse support:
	- AT+BLEHIDEN=on/off/0/1 added to enable all HID devices (including keyboard & mouse).
	- AT+BLEKEYBOARDEN is now an alias to AT+BLEHIDEN
	- AT+BLEMOUSEMOVE to move the cursor (does not affect mouse buttons)
	- AT+BLEMOUSEBUTTON to perform button actions
- Added `uuid128` option for AT+GATTADDCHAR: Custom characteristics can now have a different UUID base than the parent service UUID.
- Lower requirement of at+blekeyboardcode to minimum 1 parameter instead of 2

### Bug Fixes

- Fixed #156 UriBeacon Resets After n Hours
- Fixed issues with long beacon URLs, e.g at+bleuribeacon=http://www.adafruit.com/012345678
- Fixed big endian issue in at+blebeacon for major & minor number

### Known Issues

- It seems Windows 10 has a limited number of characteristics for DIS service. We had to disable the Serial Number characteristic to enable HID support with windows 10. 

## 0.6.5

* **AT Parser**: Fixed https://github.com/adafruit/Adafruit_BluefruitLE_nRF51/issues/2 (When resetting, dynamic GATT characteristics always use min_len when repopulating char values, not the actual value length.)
* **SDEP/SPIS**: rework SPI slave communication implementation to increase performance and stability.
