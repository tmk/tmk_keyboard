# Arduino Changelog

## 1.10

- Add non-blokcing option for .reset(blocking) & .factoryReset(blocking). The reset progress can be polled by .resetCompleted()
- Add callback using dfu as irq pin example, .handleDfuIrq() is expected to be call in loop()

## 1.9

### Features

- Added **Adafruit_ATParser** helper class to facilitate sending and receiving AT commands:
	- `.atcommand()` : Send a command without reply (several variants defined for various input parameters)
	- `.atcommandIntReply()` : Send a command with integer reply (several variants defined for various input parameters)
	- `.atcommand_full()` : General purpose command execution with a pointer to the reply buffer
	- `.printByteArray()` : Outputs a byte array in the `AA-BB-CC` format from a buffer. Useful for executing AT commands.
	- `.waitForOK()` : Uses a separate temporary buffer to avoid overwriting response content.
- Callback support in **Adafruit_BLE** class
	- Supported Events are:
		- **Connect**: Set using `setConnectCallback()`
		- **Disconnect**: Set using `setDisconnectCallback()`
		- **BLE UART RX**: Set using `.setBleUartRxCallback()`
		- **MIDI RX**: Set using `.setBleMidiRxCallback()`
		- **GATT Characteristic RX**: Set using `.setBleGattRxCallback()`
	- `.update(ms)` must be placed in the loop() function to fire the callbacks, where `ms` is the interval in milliseconds to poll for new events
	- See 'examples/callbacks' for more details
- Added **Adafruit_BLEGatt** helper class to make working with custom GATT services and characteristics easier:
	- Helpers to add custom GATT services and characteristics
	- Helpers to read/write previously defined characteristics
	- Callback support for characteristic updates
	- Added **User Description** and **Presentation Format** support for GATT characteristics
	- Add `BLEDataType_t` typedef for GATT characteristics
	- See 'example/healththermometer' for an example of using the Adafruit_BLEGatt class
- Added BLE MIDI service support with the **Adafruit_BLEMIDI** class
	-  See 'examples/midi' for details
- Added BLE Battery service support via the **Adafruit_BLEBattery** class
	- See 'example/battery' for more details
- Added BLE Eddystone helper class to facilitate using Eddystone beacon
	- See 'example/eddystone' for more details
- Add a 256 byte user NVM data section that can be accessed via `.writeNVM()` and `.readNVM()` in **Adafruit_BLE**. User can use this small chunk of NVM memory to store application specific data.
	- See 'example/nvmdata' for more details
- Additional **Adafruit_BLE** class changes:
	- Added a `setAdvData()` helper to advertise custom data payloads
