EEPROM Troubleshoot
===================

AVR Microcontroller
-------------------
`eeprom_show.hex` shows content of eeprom and `eeprom_wipe.hex` overwrite with 0xFF to erase eeprom.

Prebuilt firmware files for ATmega32u4 and ATmega32u2 are available.

- eeprom_show_atmega32u4.hex
- eeprom_wipe_atmega32u4.hex
- eeprom_show_atmega32u2.hex
- eeprom_wipe_atmega32u2.hex

Just flash the hex file and replug. Then, flash default application firmware again.

Check keyboard/eeprom_tool/ for source code.


AVR Arduino
-----------
For arduino boards(Leonardo, Pro Micro and Uno) you can use `eeprom_blank.hex` file also.

You can execute avrdude to erase eeprom like below:

    avrdude -patmega32u4 -cavr109 -b57600 -Ueeprom:w:eeprom_blank1024.hex -P/dev/ttyACM0

    avrdude -patmega328p -carduino -b115200 -Ueeprom:w:eeprom_blank1024.hex -P/dev/ttyACM0

Check this also. https://support.arduino.cc/hc/en-us/articles/5779192727068-Reset-your-board
