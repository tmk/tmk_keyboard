avrdude -p m32u4 -c avrisp -P com1 -b 19200 -e -v
avrdude -p m32u4 -c avrisp -P com1 -b 19200 -U lfuse:w:0xff:m -U hfuse:w:0xd8:m -U efuse:w:0xc9:m -v
avrdude -p m32u4 -c avrisp -P com1 -b 19200 -B 4 -U flash:w:"BootMS_B5C6.hex" -v