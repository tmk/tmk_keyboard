# SPI AT Command Transport Layer (SDEP)

This library transmits AT-style commands to the Bluefruit LE module over SPI using a custom protocol we've defined called SDEP, which stands for the **_Simple Data Exchange Protocol_**. 

SDEP is used to handle messages and responses, including error responses, and was designed to be _bus neutral_, meaning that we can use SDEP regardless of the transport mechanism (USB HID, SPI, I2C, Wireless data over the air, etc.).

SDEP messages have a four byte header, and up to a 16 byte payload, and larger messages are broken into several message chunks which are rebuilt at either end of the transport bus.  The 20 byte limit (4 byte header + 16 byte payload) was chosen to take into account the size limitations present in some transport layers.

# SPI Setup

The SPI interface uses the standard four SPI pins (MISO, MOSI, SCK and CS/SSEL), as well as an additional **IRQ** line (output from the nRF51822 to the SPI Master MCU).

## IRQ Pin

The IRQ line is asserted as long as an entire SDEP packet is available in the buffer on the nRF51822, at which point you should read the packet, keeping the CS line asserted for the entire transaction (as detailed below).  

The IRQ line will remain asserted as long as one or more packets are available, so the line may stay high after reading a packet, meaning that more packets are still available in the FIFO on the SPI slave side.

## SPI Bus Hardware Requirements

The SPI peripheral block on the nRF51822 MCU has some specific limitations that need to be taken into account when communicating with it as an SPI slave:

* The SPI clock should run <=2MHz
* A 100us delay should be added between the moment that the CS line is asserted, and before any data is transmitted on the SPI bus
* The CS line should remain asserted for the entire packet, rather than toggling CS every byte

## SDEP Packet and SPI Error Identifier

Once CS has been asserted and the mandatory 100us delay has passed, a single byte should be read from the SPI bus which will indicate the type of payload available on the nRF51822 (see **Message Type Indicator** below for more information on SDEP message types). Keep CS asserted after this byte has been read in case you need to continue reading the rest of the frame.

If a standard SDEP message type indicators (0x10, 0x20, 0x40 or 0x80) is encountered, keep reading as normal.  There are two other indicators that should be taken into account, though, which indicate a problem on the nRF51822 SPI slave side:

* **0xFE**: Slave device not ready (wait a bit and try again)
* **0xFF**: Slave device read overflow indicator (you've read more data than is available)

## Sample Transaction

The following image shows a sample SDEP response that is spread over two packets (since the response is > 20 bytes in size). Notice that the IRQ line stays asserted between the packets since more than one was available in the FIFO on the nRF51822 side:

![sdepexample_twopackets](https://cloud.githubusercontent.com/assets/181073/8234310/646b7498-15db-11e5-9c69-6366c5dd433a.png)

# SDEP (Simple Data Exchange Protocol)

The Simple Data Exchange Protocol (SDEP) can be used to send and receive binary messages between two connected devices using any binary serial bus (USB HID, USB Bulk, SPI, I2C, Wireless, etc.), exchanging data using one of four distinct message types (Command, Response, Alert and Error messages).

The protocol is designed to be flexible and extensible, with the only requirement being that **individual messages are 20 bytes or smaller**, and that the first byte of every message is a one byte (U8) identifier that indicates the message type, which defines the format for the remainder of the payload.

## Endianness

All values larger than 8-bits are encoded in little endian format.  Any deviation from this rule should be clearly documented.

## Message Type Indicator

The first byte of every message is an 8-bit identifier called the **Message Type Indicator**. This value indicates the type of message being sent, and allows us to determine the format for the remainder of the message.

| Message Type | ID (U8) | Description |
| ------------ | ------- | ----------- |
| Command      | 0x10    |             |
| Response     | 0x20    |             |
| Alert        | 0x40    |             |
| Error        | 0x80    |             |

## SDEP Data Transactions

Either connected device can initiate SDEP transactions, though certain transport protocols imposes restrictions on who can initiate a transfer. The _master_ device, for example, always initiates transactions with Bluetooth Low Energy or USB, meaning that _slave_ devices can only reply to incoming commands.

Every device that receives a _Command Message_ must reply with a _Response Message_, _Error Message_ or _Alert message_.

The following diagram illustrates how an SDEP exchange typically takes place. A Command Message is sent, and a reply will be generated based on whether the command was accepted (in which case a Response Message will be sent in reply), if the command was invalid or rejected (in which case an Error Message reply will be sent), or if the command was valid but some specific condition occurred that should be indicated to the master device (in which case an Alert Message will be sent):

**ToDo: Insert two line master/slave chart showing message flow in different scenarios**

## Message Types

### Command Messages

Command messages (Message Type = 0x10) have the following structure:

| Name            | Type | Meaning                                           |
| --------------- | ---- | ------------------------------------------------- |
| Message Type    | U8   | Always '0x10'                                     |
| Command ID      | U16  | Unique command identifier                         |
| Payload Length  | U8   | [7] More data <br> [6-5] Reserved <br> [4-0] Payload length (0..16) |
| Payload         | ...  | Optional command payload (parameters, etc.)       |

**Command ID** (bytes 1-2) and **Payload Length** (byte 3) are mandatory in any command message.  The message payload is optional, and will be ignored if Payload Length is set to 0 bytes.  When a message payload is present, it’s length can be anywhere from 1..16 bytes, to stay within the 20-byte maximum message length.

A long command (>16 bytes payload) must be divided into multiple packets. To facilitate this, the **More data** field (bit 7 of byte 3) is used to indicate whether additional packets are available for the same command. The SDEP receiver must continue to reads packets until it finds a packet with **More data == 0**, then assemble all sub-packets into one command if necessary.

The contents of the payload are user defined, and can change from one command to another.

A sample command message would be:

| 0: Message Type (U8) | 1+2: Command ID (U16) | 3: Payload Len (U8) | 4: Payload (...) |
| -------------------- | --------------------- | ------------------- | ---------------- |
| 10                   | 34 12                 | 01                  | FF               |

- The first byte is the Message Type (0x10), which identifies this as a command message.
- The second and third bytes are 0x1234 (34 12 in little-endian notation), which is the unique command ID. This value will be compared against the command lookup table and redirected to an appropriate command handler function if a matching entry was found.
- The fourth byte indicates that we have a message payload of 1 byte
- The fifth byte is the 1 byte payload: 0xFF

### Response Messages

Response messages (Message Type = 0x20) are generated in response to an incoming command, and have the following structure:

| Name            | Type | Meaning                                           |
| --------------- | ---- | ------------------------------------------------- |
| Message Type    | U8   | Always '0x20'                                     |
| Command ID      | U16  | Command ID of the command this message is a response to, to correlated responses and commands |
| Payload Length  | U8   | [7] More data <br> [6-5] Reserved <br> [4-0] Payload length (0..16) |
| Payload         | ...  | Optional response payload (parameters, etc.)      |

By including the **Command ID** that this response message is related to, the recipient can more easily correlate responses and commands.  This is useful in situations where multiple commands are sent, and some commands may take a longer period of time to execute than subsequent commands with a different command ID.

Response messages can only be generate in response to a command message, so the Command ID field should always be present.

A long response (>16 bytes payload) must be divided into multiple packets. Similar to long commands, the **More data** field (bit 7 of byte 3) is used to indicate whether additional packets are available for the same response. On responses that span more than one packet, the **More data** bit on the final packet will be set to `0` to indicate that this is the last packet in the sequence. The SDEP receiver must re-assemble all sub-packets in into one payload when necessary.

If more precise command/response correlation is required a custom protocol should be developed, where a unique message identifier is included in the payload of each command/response, but this is beyond the scope of this high-level protocol definition.

A sample response message would be:

| 0: Message Type (U8) | 1+2: Command ID (U16) | 3: Payload Len (U8) | 4: Payload (...) |
| -------------------- | --------------------- | ------------------- | ---------------- |
| 20                   | 34 12                 | 01                  | FF               |

- The first byte is the Message Type (0x20), which identifies this as a response message.
- The second and third bytes are 0x1234, which is the unique command ID that this response is related to.
- The fourth byte indicates that we have a message payload of 1 byte.
- The fifth byte is the 1 byte payload: 0xFF

### Alert Messages

Alert messages (Message Type = 0x40) are sent whenever an alert condition is present on the system (low battery, etc.), and have the following structure:

| Name            | Type | Meaning                                           |
| --------------- | ---- | ------------------------------------------------- |
| Message Type    | U8   | Always '0x40'                                     |
| Alert ID        | U16  | Unique ID for the alert condition                 |
| Payload Length  | U8   | Payload length (0..16)                            |
| Payload         | ...  | Optional response payload (parameters, etc.)      |

A sample alert message would be:

| 0: Message Type (U8) | 1+2: Alert ID (U16)   | 3: Payload Len (U8) | 4+5+6+7: Payload  |
| -------------------- | --------------------- | ------------------- | ----------------- |
| 40                   | CD AB                 | 04                  | 42 07 00 10       |

- The first byte is the Message Type (0x40), which identifies this as an alert message.
- The second and third bytes are 0xABCD, which is the unique alert ID.
- The fourth byte indicates that we have a message payload of 4 bytes.
- The last four bytes are the actual payload: `0x10000742` in this case, assuming we were transmitting a 32-bit value in little-endian format.

#### Standard Alert IDs

Alert IDs in the range of 0x0000 to 0x00FF are reserved for standard SDEP alerts, and may not be used by custom alerts.

The following alerts have been defined as a standard part of the protocol:

| ID     | Alert Description | Description                          |
| ------ | ----------------- | ------------------------------------ |
| 0x0000 | Reserved          | Reserved for future use              |
| 0x0001 | System Reset      | The system is about the reset        |
| 0x0002 | Battery Low       | The battery level is low             |
| 0x0003 | Battery Critical  | The battery level is critically low  |

### Error Messages

Error messages (Message Type = 0x80) are returned whenever an error condition is present on the system, and have the following structure:

| Name            | Type | Meaning                                           |
| --------------- | ---- | ------------------------------------------------- |
| Message Type    | U8   | Always '0x80'                                     |
| Error ID        | U16  | Unique ID for the error condition                 |
| Reserved        | U8   | Reserved for future use                           |

Whenever an error condition is present and the system needs to be alerted (such as a failed request, an attempt to access a non-existing resource, etc.) the system can return a specific error message with an appropriate Error ID.

A sample error message would be:

| 0: Message Type (U8) | 1+2: Error ID (U16)   | 3: Reserved (U8) |
| -------------------- | --------------------- | ---------------- |
| 80                   | 01 00                 | 00               |

- The first byte is the Message Type (0x80), which identifies this as an error message.
- The second and third bytes are 0x0001 (01 00 in little-endian notation), which indicates that the supplied Command ID was invalid (see Standard Error IDs below).
- The last byte is reserved and should be 0x00 and ignored.

#### Standard Error IDs

Error IDs in the range of 0x0000 to 0x00FF are reserved for standard SDEP errors, and may not be used by custom errors.

The following errors have been defined as a standard part of the protocol:

| ID     | Error Description | Description                                     |
| ------ | ----------------- | ----------------------------------------------- |
| 0x0000 | Reserved          | Reserved for future use                         |
| 0x0001 | Invalid Cmd ID    | The command ID wasn't found in the lookup table |
| 0x0003 | Invalid Payload   | The message payload was invalid                 |
